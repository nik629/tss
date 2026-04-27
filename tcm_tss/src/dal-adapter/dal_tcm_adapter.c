#include "dal_tcm_adapter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 实例化全局变量，供 tcti-secmodule.c 使用
TPCM_TCM_cmd p_tpcm_tcm = {0};

// 声明外部底层接口及类型
typedef long  gint32;
typedef unsigned long guint32;
typedef unsigned int guint16;
typedef uint8_t  guint8;

// 依赖外部库的真实函数声明
extern gint32 A55SendDataToLinxTcm(guint16 data_type, guint8 *buffer, guint32 buffer_len);

int dal_get_bmc_tpcm_tcm(void) {
    // // 1. 调用底层接口。此时 buffer 是请求包
    // gint32 ret = A55SendDataToLinxTcm(0x0001, p_tpcm_tcm.data, p_tpcm_tcm.req_len);
    
    // // 如果核间通信失败，直接返回 -1，上层 TSS 会报错
    // if (ret != 0) {
    //     printf("[DAL] A55SendDataToLinxTcm failed: %d\n", ret);
    //     return -1;
    // }

    // // 2. 此时底层已经将响应包覆盖写回了 p_tpcm_tcm.data
    // // 解析响应头中的 Size 字段 (第 2 到 5 字节，大端序)
    // guint32 resp_len = (p_tpcm_tcm.data[2] << 24) | 
    //                    (p_tpcm_tcm.data[3] << 16) | 
    //                    (p_tpcm_tcm.data[4] << 8)  | 
    //                     p_tpcm_tcm.data[5];

    // return resp_len;
    // 1. 分配一个足够大的通信缓冲区 (头部 8 字节 + 最大 TCM 数据 4096 字节)
    uint8_t comm_buf[4096 + sizeof(scmi_tcm_header_t)] = {0};
    
    // 2. 将缓冲区强转为我们的结构体指针，方便操作
    scmi_tcm_header_t *req_hdr = (scmi_tcm_header_t *)comm_buf;

    // 3. 【封装信封】：填入 SCMI 头部信息
    req_hdr->inst_id = 0; // 假设默认为实例 0，如果你有多个实例，可以动态传入
    req_hdr->payload_len = p_tpcm_tcm.req_len;

    // 4. 【装入信件】：将 TSS 传下来的真实 TCM 字节流拷贝到 payload 区域
    memcpy(req_hdr->payload, p_tpcm_tcm.data, p_tpcm_tcm.req_len);

    // 5. 计算整个 SCMI 数据包的总长度
    guint32 total_req_len = sizeof(scmi_tcm_header_t) + p_tpcm_tcm.req_len;

    // 6. 调用底层接口发送整个数据包
    gint32 ret = A55SendDataToLinxTcm(DATA_TYPE_TCM_CMD, comm_buf, total_req_len);
    
    if (ret != 0) {
        printf("[DAL] A55SendDataToLinxTcm failed: %d\n", ret);
        return -1;
    }

    // 7. 【拆开信封】：此时底层已经把带头部的响应包覆盖写回了 comm_buf
    scmi_tcm_header_t *resp_hdr = (scmi_tcm_header_t *)comm_buf;
    guint32 resp_payload_len = resp_hdr->payload_len;

    // 8. 【提取信件】：将纯净的 TCM 响应字节流剥离出来，写回给 TSS
    memcpy(p_tpcm_tcm.data, resp_hdr->payload, resp_payload_len);

    // 返回纯净的负载长度给上层
    return resp_payload_len;
}
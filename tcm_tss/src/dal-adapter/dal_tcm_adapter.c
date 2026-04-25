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
    // 1. 调用底层接口。此时 buffer 是请求包
    gint32 ret = A55SendDataToLinxTcm(0x0001, p_tpcm_tcm.data, p_tpcm_tcm.req_len);
    
    // 如果核间通信失败，直接返回 -1，上层 TSS 会报错
    if (ret != 0) {
        printf("[DAL] A55SendDataToLinxTcm failed: %d\n", ret);
        return -1;
    }

    // 2. 此时底层已经将响应包覆盖写回了 p_tpcm_tcm.data
    // 解析响应头中的 Size 字段 (第 2 到 5 字节，大端序)
    guint32 resp_len = (p_tpcm_tcm.data[2] << 24) | 
                       (p_tpcm_tcm.data[3] << 16) | 
                       (p_tpcm_tcm.data[4] << 8)  | 
                        p_tpcm_tcm.data[5];

    return resp_len;
}
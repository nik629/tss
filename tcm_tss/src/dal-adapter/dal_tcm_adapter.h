#ifndef DAL_TCM_ADAPTER_H
#define DAL_TCM_ADAPTER_H

#include <stdint.h>

/* * 数据结构定义 
 * 注意：如果你的 "pme_app/dalib/dal_m3_msg_intf.h" 中已经定义了该结构体，
 * 请将此处的定义注释掉，以避免重复定义报错。
 */
typedef struct {
    uint8_t *data;      // 指向存放 TCM 报文的缓冲区指针
    uint32_t req_len;   // 请求报文的长度
    uint32_t resp_len;  // 响应报文的长度
} TPCM_TCM_cmd;

/* * 导出全局变量 
 * 该变量在 dal_tcm_adapter.c 中实例化，并被 tcti-secmodule.c 通过 extern 引用。
 * 用于作为 A55 与底层通信的“数据中转站”。
 */
extern TPCM_TCM_cmd p_tpcm_tcm;

/*
 * 核心调用接口
 * 触发下行通信，将 p_tpcm_tcm.data 发送给 TCM，并阻塞等待响应。
 * 返回值：成功返回接收到的响应数据的长度，失败返回 -1。
 */
int dal_get_bmc_tpcm_tcm(void);

#endif /* DAL_TCM_ADAPTER_H */
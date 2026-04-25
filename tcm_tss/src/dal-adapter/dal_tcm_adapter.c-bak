#include "dal_tcm_adapter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 实例化全局变量，供 tcti-secmodule.c 使用
TPCM_TCM_cmd p_tpcm_tcm = {0};

/* ===================================================================
 * 编译控制宏
 * 在 x86 上编译测试时，请在 Makefile 或 CMake 中加上 -DSIMULATE_X86
 * 在真实 ARM 板子上编译时，去掉该宏即可切换为真实的核间调用。
 * =================================================================== */
 #define SIMULATE_X86 1 // 如果不通过编译脚本传参，也可以直接在这里取消注释

#ifdef SIMULATE_X86

/* ===================================================================
 * 【x86 模拟端】伪造 TCM 响应 (Mock)
 * 智能解析上层下发的命令，并按 TSS2 规范伪造合法的返回包
 * =================================================================== */
int dal_get_bmc_tpcm_tcm(void) {
    printf("[TCM_MOCK] Intercepted TCM command on x86 host.\n");

    if (p_tpcm_tcm.data == NULL || p_tpcm_tcm.req_len < 10) {
        printf("[TCM_MOCK] Error: Invalid request buffer or command too short!\n");
        return -1;
    }

    // 1. 解析上层发来的命令码 (Command Code，位于请求包的第 6-9 字节，大端序)
    uint32_t cmd_code = (p_tpcm_tcm.data[6] << 24) | 
                        (p_tpcm_tcm.data[7] << 16) | 
                        (p_tpcm_tcm.data[8] << 8)  | 
                         p_tpcm_tcm.data[9];

    printf("[TCM_MOCK] Decoded Command Code: 0x%08X\n", cmd_code);

    // 2. 准备基础的成功响应包
    // 默认 Header: Tag (0x8001), Size (10), Response Code (0x00000000)
    uint8_t mock_resp[2048] = {
        0x80, 0x01,             // Tag
        0x00, 0x00, 0x00, 0x0A, // Size (默认 10 字节)
        0x00, 0x00, 0x00, 0x00  // Response Code (0 = 成功)
    };
    uint32_t resp_len = 10;

    // 3. 根据不同的命令，伪造对应的 Payload
    switch (cmd_code) {
        case 0x00000144: // TPM2_CC_Startup
            printf("[TCM_MOCK] Simulating TPM2_Startup success.\n");
            // Startup 只需要返回 10 字节的成功头即可
            break;

        case 0x0000017B: // TPM2_CC_GetRandom
        {
            printf("[TCM_MOCK] Simulating TPM2_GetRandom.\n");
            // 解析请求中想要的随机数长度 (通常在第 10-11 字节)
            uint16_t req_bytes = (p_tpcm_tcm.data[10] << 8) | p_tpcm_tcm.data[11];
            
            // 响应包格式: Header(10) + 随机数长度(2) + 随机数据(req_bytes)
            resp_len = 10 + 2 + req_bytes;
            
            // 修正 Header 中的 Size 字段
            mock_resp[2] = (resp_len >> 24) & 0xFF;
            mock_resp[3] = (resp_len >> 16) & 0xFF;
            mock_resp[4] = (resp_len >> 8) & 0xFF;
            mock_resp[5] = resp_len & 0xFF;
            
            // 填入返回的字节长度
            mock_resp[10] = (req_bytes >> 8) & 0xFF;
            mock_resp[11] = req_bytes & 0xFF;
            
            // 填入伪造的随机数 (统一填充 0xAA 作为测试标记)
            memset(&mock_resp[12], 0xAA, req_bytes);
            break;
        }

        case 0x0000017A: // TPM2_CC_GetCapability
            printf("[TCM_MOCK] Simulating TPM2_GetCapability (Empty props).\n");
            // 返回一个空的 Capability 结构，防止 tpm2_startup 报错
            resp_len = 19; // Header(10) + moreData(1) + cap(4) + count(4)
            mock_resp[5] = 19; // Size
            mock_resp[10] = 0; // moreData = NO
            mock_resp[11] = 0; mock_resp[12] = 0; mock_resp[13] = 0; mock_resp[14] = 6; // TPM_CAP_TPM_PROPERTIES
            mock_resp[15] = 0; mock_resp[16] = 0; mock_resp[17] = 0; mock_resp[18] = 0; // Count = 0
            break;

        default:
            printf("[TCM_MOCK] Unknown command (0x%08X). Returning generic 10-byte success.\n", cmd_code);
            // 对于未特殊处理的命令，直接返回通用的 10 字节成功包
            break;
    }

    // 4. 将伪造的响应数据覆盖写回 p_tpcm_tcm.data 缓冲区
    // 供 tcti_secmodule_receive 函数读取
    memcpy(p_tpcm_tcm.data, mock_resp, resp_len);

    return resp_len;
}

#else

/* ===================================================================
 * 【真实 ARM 端】调用底层的核间通信接口
 * =================================================================== */
#include <unistd.h>

// 声明外部底层接口及类型
typedef int32_t  gint32;
typedef uint32_t guint32;
typedef uint16_t guint16;
typedef uint8_t  guint8;

extern gint32 A55SendDataToLinxTcm(guint16 data_type, guint8 *buffer, guint32 buffer_len);

#define DATA_TYPE_TCM_CMD 0x0001

int dal_get_bmc_tpcm_tcm(void) {
    if (p_tpcm_tcm.data == NULL || p_tpcm_tcm.req_len == 0) {
        printf("[DAL_TCM] Error: Invalid request buffer or length.\n");
        return -1;
    }

    // 1. 发送请求给 Linx 侧
    gint32 ret = A55SendDataToLinxTcm(DATA_TYPE_TCM_CMD, p_tpcm_tcm.data, p_tpcm_tcm.req_len);
    if (ret != 0) {
        printf("[DAL_TCM] Error: A55SendDataToLinxTcm failed with code %d.\n", ret);
        return -1;
    }

    // 2. 阻塞等待回包
    // TODO: 在这里接入你的真实阻塞接收逻辑 (如等待 Semaphore、Event 或轮询标志位)
    // ---------------------------------------------------------
    // 伪代码示例：
    // Wait_Semaphore(&tcm_rx_sem, TIMEOUT);
    // int actual_resp_len = Read_From_Rx_Queue(p_tpcm_tcm.data);
    // return actual_resp_len;
    // ---------------------------------------------------------

    printf("[DAL_TCM] Warning: Blocking receive logic is not yet implemented.\n");
    
    return -1; // 真实接收逻辑补全后，替换为实际接收到的长度
}

#endif
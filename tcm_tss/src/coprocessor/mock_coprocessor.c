/* mock_coprocessor.c - x86 专用的假硬件库 */
#include "mock_coprocessor.h"  // <--- 引入头文件
#include <stdio.h>
#include <string.h>
/* ===================================================================
 * 该文件实现了一个模拟的核间通信接口，供 x86 主机编译测试使用。
 * 通过解析上层发送的 TCM 命令，伪造合法的 TPM 响应数据包。
 * =================================================================== */
gint32 A55SendDataToLinxTcm(guint16 data_type, guint8 *buffer, guint32 buffer_len) {
    // 1. 将传入的原始 buffer 映射为结构体
    scmi_tcm_header_t *hdr = (scmi_tcm_header_t *)buffer;
    
    // 提取请求信息
    uint32_t inst_id = hdr->inst_id;
    uint32_t req_payload_len = hdr->payload_len;
    
    printf("[STUB_LIB] 拦截 SCMI 请求 | 实例ID: %d | 负载长度: %d 字节\n", inst_id, req_payload_len);

    // 2. 从【payload】区域读取命令码 (第 6-9 字节)
    uint32_t cmd_code = (hdr->payload[6] << 24) | 
                        (hdr->payload[7] << 16) | 
                        (hdr->payload[8] << 8)  | 
                         hdr->payload[9];
    
    // 准备存放伪造纯 TCM 数据的临时数组
    uint8_t mock_payload[1024];
    uint32_t resp_payload_len = 10;
    
    // 基础成功头: Tag(0x8001), Size(0x0A), RC(0x00)
    mock_payload[0] = 0x80; mock_payload[1] = 0x01;
    mock_payload[6] = 0; mock_payload[7] = 0; mock_payload[8] = 0; mock_payload[9] = 0;

    // 3. 根据命令组装假数据
    switch (cmd_code) {
        case 0x00000144: // Startup
            mock_payload[2] = 0; mock_payload[3] = 0; mock_payload[4] = 0; mock_payload[5] = 10;
            break;
            
        case 0x0000017B: // GetRandom
            {
                uint16_t req_bytes = (hdr->payload[10] << 8) | hdr->payload[11];
                resp_payload_len = 10 + 2 + req_bytes;
                
                mock_payload[2] = (resp_payload_len >> 24) & 0xFF;
                mock_payload[3] = (resp_payload_len >> 16) & 0xFF;
                mock_payload[4] = (resp_payload_len >> 8)  & 0xFF;
                mock_payload[5] =  resp_payload_len        & 0xFF;
                
                mock_payload[10] = (req_bytes >> 8) & 0xFF;
                mock_payload[11] =  req_bytes       & 0xFF;
                memset(&mock_payload[12], 0xCC, req_bytes); // 填充 0xCC
            }
            break;
            
        default:
            mock_payload[2] = 0; mock_payload[3] = 0; mock_payload[4] = 0; mock_payload[5] = 10;
            break;
    }

    // 4. 【核心】：在原 buffer 上组装返回包 (头部 + 负载)
    // 实例 ID 原样返回，或者按照真实硬件的规矩修改
    hdr->inst_id = inst_id; 
    // 更新返回包的真实负载长度
    hdr->payload_len = resp_payload_len;
    // 将伪造的 TCM 数据写入 payload 区域
    memcpy(hdr->payload, mock_payload, resp_payload_len);
    
    uint32_t total_resp_len = sizeof(scmi_tcm_header_t) + resp_payload_len;
    printf("[STUB_LIB] 伪造 SCMI 回包完毕，总长: %d 字节\n", total_resp_len);

    return 0;
}
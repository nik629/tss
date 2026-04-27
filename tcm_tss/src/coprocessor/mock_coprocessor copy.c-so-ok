/* mock_coprocessor.c - x86 专用的假硬件库 */
#include "mock_coprocessor.h"  // <--- 引入头文件
#include <stdio.h>
#include <string.h>
/* ===================================================================
 * 该文件实现了一个模拟的核间通信接口，供 x86 主机编译测试使用。
 * 通过解析上层发送的 TCM 命令，伪造合法的 TPM 响应数据包。
 * =================================================================== */
gint32 A55SendDataToLinxTcm(guint16 data_type, guint8 *buffer, guint32 buffer_len) {
    printf("[STUB_LIB] 拦截核间通信请求，输入长度: %d 字节\n", buffer_len);

    // 1. 读取上层发来的命令码 (Command Code，第 6-9 字节)
    uint32_t cmd_code = (buffer[6] << 24) | (buffer[7] << 16) | (buffer[8] << 8) | buffer[9];
    
    // 准备一个临时数组存放假回包
    uint8_t mock_resp[1024];
    uint32_t resp_len = 10;
    
    // 基础成功头: Tag(0x8001), Size(0x0A), RC(0x00)
    mock_resp[0] = 0x80; mock_resp[1] = 0x01;
    mock_resp[6] = 0; mock_resp[7] = 0; mock_resp[8] = 0; mock_resp[9] = 0;

    // 2. 根据命令组装数据
    switch (cmd_code) {
        case 0x00000144: // Startup
            mock_resp[2] = 0; mock_resp[3] = 0; mock_resp[4] = 0; mock_resp[5] = 10;
            break;
            
        case 0x0000017B: // GetRandom
            {
                // 解析请求的随机数长度
                uint16_t req_bytes = (buffer[10] << 8) | buffer[11];
                resp_len = 10 + 2 + req_bytes;
                
                // 修正 Size 字段
                mock_resp[2] = (resp_len >> 24) & 0xFF;
                mock_resp[3] = (resp_len >> 16) & 0xFF;
                mock_resp[4] = (resp_len >> 8)  & 0xFF;
                mock_resp[5] =  resp_len        & 0xFF;
                
                // 填入长度和数据
                mock_resp[10] = (req_bytes >> 8) & 0xFF;
                mock_resp[11] =  req_bytes       & 0xFF;
                memset(&mock_resp[12], 0xBB, req_bytes); // 用 0xBB 填充
            }
            break;
            
        default: // 其他未实现命令默认返回 10 字节成功
            mock_resp[2] = 0; mock_resp[3] = 0; mock_resp[4] = 0; mock_resp[5] = 10;
            break;
    }

    // 3. 【核心】直接覆盖原 buffer，模拟硬件 In-place 操作
    memcpy(buffer, mock_resp, resp_len);
    
    printf("[STUB_LIB] 伪造回包写入完毕，输出长度: %d 字节\n", resp_len);

    return 0; // 返回 0 表示通信链路正常
}
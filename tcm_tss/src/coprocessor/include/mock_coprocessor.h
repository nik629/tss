#ifndef MOCK_COPROCESSOR_H
#define MOCK_COPROCESSOR_H

#include <stdint.h>

/* ===================================================================
 * 基础数据类型定义 (对应 ARM 底层 SDK 的类型)
 * =================================================================== */
typedef long  gint32;
typedef unsigned long guint32;
typedef unsigned int guint16;
typedef uint8_t  guint8;

/* ===================================================================
 * 协处理器通信宏定义
 * =================================================================== */
#define DATA_TYPE_TCM_CMD 0x0001


/* ===================================================================
 * 核心通信接口
 * =================================================================== */

/**
 * @brief 向底层 Linx TCM 发送命令并阻塞等待回包
 * * 此函数采用 In-place (原地修改) 模式。传入的 buffer 既是请求数据，
 * 也是接收响应数据的容器。
 *
 * @param data_type  数据类型标识 (如 DATA_TYPE_TCM_CMD)
 * @param buffer     [IN/OUT] 缓冲区：调用前存请求包，返回后存响应包
 * @param buffer_len [IN] 请求数据的长度
 * * @return gint32    0 表示底层通信链路成功，非 0 表示链路错误
 */
gint32 A55SendDataToLinxTcm(guint16 data_type, guint8 *buffer, guint32 buffer_len);

#endif /* MOCK_COPROCESSOR_H */
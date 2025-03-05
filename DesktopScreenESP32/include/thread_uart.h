#ifndef _THREADUART_H_
#define _THREADUART_H_

#include "thread_config.h"

// uart 资源
typedef struct {
    uint8_t* buffer;                    // 接收缓存区
    uint16_t data_len;                  // 数据长度
    HardwareSerial* uart;               // 串口外设句柄
    QueueHandle_t queue_tx;             // 发送消息队列
    TaskHandle_t htask_send;            // 发送任务句柄
    TaskHandle_t htask_recv;            // 接收任务句柄
} UartResource;
extern UartResource uarts_res[1];

/// @brief 初始化并创建 uart 线程
/// @param  
void uartThreadCreate(void);

#endif // !_THREADUART_H_

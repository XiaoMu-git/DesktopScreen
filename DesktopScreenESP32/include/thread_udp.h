#ifndef _THREADUDP_H_
#define _THREADUDP_H_

#include "thread_config.h"

// udp 资源
typedef struct {
    uint8_t* buffer;                    // 接收缓存区
    uint16_t data_len;                  // 数据长度
    WiFiClass* wifi;                    // wifi 外设句柄
    WiFiUDP* udp;                       // udp 外设句柄
    QueueHandle_t queue_tx;             // 发送消息队列
    TaskHandle_t htask_send;            // 发送任务句柄
    TaskHandle_t htask_recv;            // 接收任务句柄
} UdpResource;
extern UdpResource udp_res[1];

/// @brief 初始化并创建 udp 线程
/// @param  
void udpThreadCreate(void);

#endif // !_THREADUDP_H_

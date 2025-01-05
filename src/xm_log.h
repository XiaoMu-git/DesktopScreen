#ifndef _LOG_H_
#define _LOG_H_

#include <Arduino.h>
#include "config.h"
#include "message.h"
#include "xm_uart.h"
#include "xm_udp.h"

#define LOG_INFO(info, ...)     XM_logSendMsg(MSG_TYPE_LOG_INFO, info, ##__VA_ARGS__)
#define LOG_WARN(info, ...)     XM_logSendMsg(MSG_TYPE_LOG_WARNING, info, ##__VA_ARGS__)
#define LOG_ERROR(info, ...)    XM_logSendMsg(MSG_TYPE_LOG_ERROR, info, ##__VA_ARGS__)

typedef struct {
    char name[16];
    UartInfo *uart_info;
    UdpInfo *udp_info;
    QueueHandle_t tx_queue;
    TaskHandle_t task;
} LogInfo;

extern LogInfo log_info[1];

void XM_logStart();
void XM_logTask(void *param);
void XM_logSendMsg(uint8_t level, const char* info, ...);

#endif // !_LOG_H_

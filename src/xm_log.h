#ifndef _LOG_H_
#define _LOG_H_

#include <Arduino.h>
#include "config.h"
#include "message.h"
#include "xm_uart.h"

#define LOG_INFO(info, ...)     XM_logSend(MSG_TYPE_LOG_INFO, info, ##__VA_ARGS__)
#define LOG_WARN(info, ...)     XM_logSend(MSG_TYPE_LOG_WARNING, info, ##__VA_ARGS__)
#define LOG_ERROR(info, ...)    XM_logSend(MSG_TYPE_LOG_ERROR, info, ##__VA_ARGS__)

typedef struct {
    char name[16];
    QueueHandle_t tx_queue;
    TaskHandle_t task;
} LogInfo;

extern LogInfo log_info;

void XM_logStart();
void XM_logTask(void *param);
void XM_logSend(uint8_t level, const char* info, ...);

#endif // !_LOG_H_

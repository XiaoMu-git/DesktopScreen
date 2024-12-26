#ifndef _XM_CONSOLE_H_
#define _XM_CONSOLE_H_

#include <Arduino.h>
#include "xm_uart.h"

#define     CONSOLE_BUFFER_SIZE     10 * 1024

typedef struct {
    char name[16];
    uint8_t *buffer;
    TaskHandle_t task;
} ConsoleInfo;

extern ConsoleInfo console_info;

void XM_consoleInit();
void XM_consoleTask(void *param);

#endif // !_XM_CONSOLE_H_

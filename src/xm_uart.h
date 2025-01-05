#ifndef _XM_UART_H_
#define _XM_UART_H_

#include <Arduino.h>
#include "config.h"
#include "message.h"

typedef struct {
    char name[16];
    uint8_t *buffer;
    uint16_t data_len;
    HardwareSerial *uart;
    QueueHandle_t rx_queue;
    QueueHandle_t tx_queue;
    TaskHandle_t task;
} UartInfo;

extern UartInfo uart_info[2];

void XM_uart0Start();
// void XM_uart1Start();
void XM_uart0Task(void *param);
// void XM_uart1Task(void *param);
template <typename MsgType>
bool XM_uartCheckMsg(uint8_t *buffer, uint16_t &data_len, QueueHandle_t rx_queue);

#endif // !_XM_UART_H_

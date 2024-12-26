#ifndef _XM_UART_H_
#define _XM_UART_H_

#include <Arduino.h>

#define     UART_QUEUE_SIZE         128
#define     UART_RECV_BUFFER_SIZE   128

typedef struct {
    uint16_t length;
    uint8_t *data;
} Message;

typedef struct {
    char name[16];
    HardwareSerial *uart;
    QueueHandle_t rx_queue;
    QueueHandle_t tx_queue;
    TaskHandle_t task;
} UartInfo;

extern UartInfo uart0_info;
extern UartInfo uart1_info;
extern UartInfo uart2_info;

void XM_uart0Init();
void XM_uart1Init();
void XM_uart2Init();
void XM_uartTask(void *param);

#endif // !_XM_UART_H_

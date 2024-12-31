#include "xm_uart.h"

UartInfo uart0_info;
UartInfo uart1_info;

void XM_uart0Init() {
    if (uart0_info.task == nullptr) {
        Serial.begin(115200);
        strcpy(uart0_info.name, "uart0");
        uart0_info.buffer = nullptr;
        uart0_info.data_len = 0;
        uart0_info.uart = &Serial;
        uart0_info.rx_queue = nullptr;
        uart0_info.tx_queue = xQueueCreate(UART_MESSAGE_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_uart0Task, "uart0_task", STACK_SIZE_MEDIUM, &uart0_info, TASK_PRIORITY_MEDIUM, &(uart0_info.task));
    }
}

void XM_uart1Init() {
    if (uart1_info.task == nullptr) {
        Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
        strcpy(uart1_info.name, "uart1");
        uart1_info.buffer = new uint8_t[UART_RECV_BUFFER_SIZE];
        uart0_info.data_len = 0;
        uart1_info.uart = &Serial1;
        uart1_info.rx_queue = xQueueCreate(UART_MESSAGE_QUEUE_SIZE, sizeof(void*));
        uart1_info.tx_queue = xQueueCreate(UART_MESSAGE_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_uart1Task, "uart1_task", STACK_SIZE_MEDIUM, &uart1_info, TASK_PRIORITY_MEDIUM, &(uart1_info.task));
    }
}

void XM_uart0Task(void *param) {
    UartInfo *uart_info = (UartInfo*)param;
    auto &name = uart_info->name;
    auto &uart = uart_info->uart;
    auto &tx_queue = uart_info->tx_queue;
    Message *message = nullptr;
    while (true) {
        if (xQueueReceive(tx_queue, &message, portMAX_DELAY) != pdPASS) {
            if (message = nullptr) {


                delete message;
                message = nullptr;
            }
        }
    }
}

void XM_uart1Task(void *param) {
    UartInfo *uart_info = (UartInfo*)param;
    while (true) {

    }
}

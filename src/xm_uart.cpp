#include "xm_uart.h"

UartInfo uart0_info;
UartInfo uart1_info;
UartInfo uart2_info;

void XM_uart0Init() {
    if (uart0_info.task == nullptr) {
        Serial.begin(115200);
        strcpy(uart0_info.name, "uart0");
        uart0_info.uart = &Serial;
        uart0_info.rx_queue = xQueueCreate(UART_QUEUE_SIZE, sizeof(Message));
        uart0_info.tx_queue = xQueueCreate(UART_QUEUE_SIZE, sizeof(Message));
        xTaskCreate(XM_uartTask, "uart0_task", 2048, &uart0_info, 10, &(uart0_info.task));
    }
}

void XM_uart1Init() {
    if (uart1_info.task == nullptr) {
        Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
        strcpy(uart1_info.name, "uart1");
        uart1_info.uart = &Serial1;
        uart1_info.rx_queue = xQueueCreate(UART_QUEUE_SIZE, sizeof(Message));
        uart1_info.tx_queue = xQueueCreate(UART_QUEUE_SIZE, sizeof(Message));
        xTaskCreate(XM_uartTask, "uart1_task", 2048, &uart1_info, 10, &(uart1_info.task));
    }
}

void XM_uart2Init() {
    if (uart2_info.task == nullptr) {
        Serial2.begin(115200, SERIAL_8N1, RX2, TX2);
        strcpy(uart2_info.name, "uart2");
        uart2_info.uart = &Serial2;
        uart2_info.rx_queue = xQueueCreate(UART_QUEUE_SIZE, sizeof(Message));
        uart2_info.tx_queue = xQueueCreate(UART_QUEUE_SIZE, sizeof(Message));
        xTaskCreate(XM_uartTask, "uart2_task", 2048, &uart2_info, 10, &(uart2_info.task));
    }
}

void XM_uartTask(void *param) {
    UartInfo *uart_info = (UartInfo*)param;
    uint8_t buffer[UART_RECV_BUFFER_SIZE];
    uint16_t data_len = 0;
    while (true) {
        while (uart_info->uart->available() && data_len <= UART_RECV_BUFFER_SIZE) buffer[data_len++] = uart_info->uart->read();
        if (data_len > 0) {
            Message message = { data_len ,nullptr };
            data_len = 0;
            message.data = new uint8_t[message.length];
            memcpy(message.data, buffer, message.length);
            if (xQueueSend(uart_info->rx_queue, &message, 0) != pdPASS) {
                Serial.printf("%s tx queue is full.\n", uart_info->name);
                delete[] message.data;
            }
        }
        else vTaskDelay(100);
        Serial.printf("%s_thread running.\n", uart_info->name);
    }
}

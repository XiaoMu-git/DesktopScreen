#include "xm_uart.h"

UartInfo uart0_info;
// UartInfo uart1_info;

void XM_uart0Start() {
    if (uart0_info.task == nullptr) {
        Serial.begin(115200);
        strcpy(uart0_info.name, "uart0");
        uart0_info.buffer = nullptr;
        uart0_info.data_len = 0;
        uart0_info.uart = &Serial;
        uart0_info.rx_queue = nullptr;
        uart0_info.tx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_uart0Task, "uart0_task", STACK_SIZE_MEDIUM, &uart0_info, TASK_PRIORITY_MEDIUM, &(uart0_info.task));
    }
}

// void XM_uart1Start() {
//     if (uart1_info.task == nullptr) {
//         Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
//         strcpy(uart1_info.name, "uart1");
//         uart1_info.buffer = new uint8_t[UART_RECV_BUFFER_SIZE];
//         uart0_info.data_len = 0;
//         uart1_info.uart = &Serial1;
//         uart1_info.rx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
//         uart1_info.tx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
//         xTaskCreate(XM_uart1Task, "uart1_task", STACK_SIZE_MEDIUM, &uart1_info, TASK_PRIORITY_MEDIUM, &(uart1_info.task));
//     }
// }

void XM_uart0Task(void *param) {
    UartInfo *uart_info = (UartInfo*)param;
    auto &name = uart_info->name;
    auto &uart = uart_info->uart;
    auto &tx_queue = uart_info->tx_queue;
    MsgLog *msg_log = nullptr;
    while (true) {
        while (uart->available()) uart->read();
        if (xQueueReceive(tx_queue, &msg_log, portMAX_DELAY) == pdPASS) {
            if (msg_log != nullptr) {
                uart->write((uint8_t*)msg_log, msg_log->length);
                delete msg_log;
                msg_log = nullptr;
            }
        }
    }
}

// void XM_uart1Task(void *param) {
//     UartInfo *uart_info = (UartInfo*)param;
//     Message *msg = nullptr;
//     auto &name = uart_info->name;
//     auto &buffer = uart_info->buffer;
//     auto &data_len = uart_info->data_len;
//     auto &uart = uart_info->uart;
//     auto &rx_queue = uart_info->rx_queue;
//     auto &tx_queue = uart_info->tx_queue;
//     while (true) {
//         /*接收数据*/
//         while (uart->available() && data_len < UART_RECV_BUFFER_SIZE) buffer[data_len++] = uart->read();
//         while (data_len > 0 && buffer[0] != MSG_HEAD) memmove(buffer, buffer + 1, --data_len);  // 校验包头
//         if (data_len > sizeof(Message)) {
//             msg = (Message*)buffer;
//             if (data_len > msg->length) {
//                 uint8_t check = 0x00;
//                 for (int i = 4; i < msg->length - 2; i++) check += buffer[i];
//                 if (buffer[msg->length - 2] != check || buffer[msg->length - 1] != MSG_END) buffer[0] = 0xFF;   // 检查校验和和包尾
//                 else {
//                     Message *new_msg = new Message();
//                     memcpy(new_msg, buffer, msg->length);
//                     memmove(buffer, buffer + msg->length, data_len -= msg->length);
//                     if (xQueueSend(rx_queue, &new_msg, 0) != pdPASS) {
//                         Serial.println("溢出");
//                         delete new_msg;
//                     }
//                 }

//             }
//             msg = nullptr;
//         }

//         /*发送数据*/
//         if (xQueueReceive(tx_queue, &msg, 0) == pdPASS) {
//             if (msg != nullptr) {
//                 uart->write((uint8_t*)msg, msg->length);
//                 delete msg;
//                 msg = nullptr;
//             }
//         }
//         vTaskDelay(100);
//     }
// }

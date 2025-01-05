#include "xm_uart.h"

UartInfo uart_info[1];

void XM_uart0Start() {
    if (uart_info[0].task == nullptr) {
        Serial.begin(115200);
        strcpy(uart_info[0].name, "uart0");
        uart_info[0].buffer = new uint8_t[UART_RECV_BUFFER_SIZE];
        uart_info[0].data_len = 0;
        uart_info[0].uart = &Serial;
        uart_info[0].rx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
        uart_info[0].tx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_uart0Task, "uart0_task", STACK_SIZE_MEDIUM, &uart_info[0], TASK_PRIORITY_MEDIUM, &(uart_info[0].task));
    }
}

void XM_uart0Task(void *param) {
    UartInfo *uart_info = (UartInfo*)param;
    Message *msg = nullptr;
    auto &name = uart_info->name;
    auto &buffer = uart_info->buffer;
    auto &data_len = uart_info->data_len;
    auto &uart = uart_info->uart;
    auto &rx_queue = uart_info->rx_queue;
    auto &tx_queue = uart_info->tx_queue;
    while (true) {
        /*接收数据*/
        while (uart->available() && data_len < UART_RECV_BUFFER_SIZE) buffer[data_len++] = uart->read();
        while (data_len > 0 && buffer[0] != MSG_HEAD) memmove(buffer, buffer + 1, --data_len);
        if (data_len >= sizeof(Message)) {
            msg = (Message*)buffer;
            if (data_len >= msg->length) {
                bool is_success = false;
                if ((msg->type & 0xF0) == MSG_TYPE_CMD) {
                    XM_uartCheckMsg<MsgCmd>(buffer, data_len, rx_queue);
                }
                else if ((msg->type & 0xF0) == MSG_TYPE_DATA) {
                    XM_uartCheckMsg<MsgData>(buffer, data_len, rx_queue);
                }
                else if ((msg->type & 0xF0) == MSG_TYPE_REQUEST) {
                    XM_uartCheckMsg<MsgReq>(buffer, data_len, rx_queue);
                }
                else if ((msg->type & 0xF0) == MSG_TYPE_STATE) {
                    XM_uartCheckMsg<MsgState>(buffer, data_len, rx_queue);
                }
                else if ((msg->type & 0xF0) == MSG_TYPE_LOG) {
                    XM_uartCheckMsg<MsgLog>(buffer, data_len, rx_queue);
                }
            }
            else if (msg->length > MSG_MAX_LENGTH) buffer[0] = 0xFF;
            msg = nullptr;
        }

        /*发送数据*/
        if (xQueueReceive(tx_queue, &msg, 0) == pdPASS) {
            if (msg != nullptr) {
                uart->write((uint8_t*)msg, msg->length);
                delete msg;
                msg = nullptr;
            }
        }
        vTaskDelay(10);
    }
}

template <typename MsgType>
bool XM_uartCheckMsg(uint8_t *buffer, uint16_t &data_len, QueueHandle_t rx_queue) {
    MsgType *buf_msg = (MsgType*)buffer;
    if (buf_msg->calculate()) {
        MsgType *recv_msg = new MsgType();
        memcpy(recv_msg, buffer, buf_msg->length);
        memmove(buffer, buffer + buf_msg->length, data_len -= buf_msg->length);
        if (xQueueSend(rx_queue, &recv_msg, 0) == pdPASS) return true;
        else delete recv_msg;
    }
    buffer[0] = 0xFF;
    return false;
}
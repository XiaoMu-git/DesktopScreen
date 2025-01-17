#include "xm_uart.h"

UartInfo uart_info[1];

void XM_uartStart() {
    Serial2.begin(115200, SERIAL_8N1, RX2, TX2);
    strcpy(uart_info[0].name, "uart");
    uart_info[0].buffer = new uint8_t[UART_RECV_BUFFER_SIZE];
    uart_info[0].data_len = 0;
    uart_info[0].uart = &Serial2;
    uart_info[0].tx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
    xTaskCreate(XM_uartRecvTask, "uart_recv_task", STACK_SIZE_MEDIUM, &uart_info[0], TASK_PRIORITY_MEDIUM, &(uart_info[0].recv_task));
    xTaskCreate(XM_uartSendTask, "uart_send_task", STACK_SIZE_MEDIUM, &uart_info[0], TASK_PRIORITY_MEDIUM, &(uart_info[0].send_task));
}

void XM_uartRecvTask(void* param) {
    UartInfo* uart_info = (UartInfo*)param;
    Message* recv_msg = nullptr;
    auto &buffer = uart_info->buffer;
    auto &data_len = uart_info->data_len;
    auto &uart = uart_info->uart;
    
    while (true) {
        while (uart->available() && data_len < UART_RECV_BUFFER_SIZE) buffer[data_len++] = uart->read();    // 读取数据到缓存区
        while (data_len > 0 && buffer[0] != MSG_HEAD) memmove(buffer, buffer + 1, --data_len);  // 删除非包头的数据
        if (data_len >= sizeof(Message)) {
            if (data_len >= ((Message*)buffer)->length) {
                /* 检查不同报文的报文格式和校验和是否正确 */
                if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_CMD) {
                    recv_msg = XM_uartCheckMsg<MsgCmd>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgCmd* cmd = new MsgCmd();
                        memcpy(cmd, recv_msg, sizeof(MsgCmd));
                        xQueueSend(console_info[0].rx_queue, &cmd, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_DATA) {
                    recv_msg = XM_uartCheckMsg<MsgData>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgData* data = new MsgData();
                        memcpy(data, recv_msg, sizeof(MsgData));
                        xQueueSend(console_info[0].rx_queue, &data, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_REQUEST) {
                    recv_msg = XM_uartCheckMsg<MsgReq>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgReq* req = new MsgReq();
                        memcpy(req, recv_msg, sizeof(MsgReq));
                        xQueueSend(console_info[0].rx_queue, &req, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_STATE) {
                    recv_msg = XM_uartCheckMsg<MsgState>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgState* state = new MsgState();
                        memcpy(state, recv_msg, sizeof(MsgState));
                        xQueueSend(console_info[0].rx_queue, &state, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_LOG) {
                    recv_msg = XM_uartCheckMsg<MsgLog>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgLog* log = new MsgLog();
                        memcpy(log, recv_msg, sizeof(MsgLog));
                        xQueueSend(console_info[0].rx_queue, &log, 0);
                    }
                }
                
                if (recv_msg != nullptr) {
                    delete recv_msg;
                    recv_msg = nullptr;
                }
            }
            else if (((Message*)buffer)->length > MSG_MAX_LENGTH) buffer[0] = 0xFF;    // 处理报文长度异常
            else vTaskDelay(10);
        }
        else vTaskDelay(100);
    }
}

void XM_uartSendTask(void* param) {
    UartInfo* uart_info = (UartInfo*)param;
    Message* send_msg = nullptr;
    auto &uart = uart_info->uart;
    auto &tx_queue = uart_info->tx_queue;
    
    while (true) {
        if (xQueueReceive(tx_queue, &send_msg, portMAX_DELAY) == pdPASS) {
            if (send_msg != nullptr) {
                uart->write((uint8_t*)send_msg, send_msg->length);
                delete send_msg;
                send_msg = nullptr;
            }
        }
    }
}

template <typename MsgType>
MsgType* XM_uartCheckMsg(uint8_t* buffer, uint16_t &data_len) {
    MsgType* buf_msg = (MsgType*)buffer;
    if (buf_msg->calculate()) {
        MsgType* recv_msg = new MsgType();
        memcpy(recv_msg, buffer, buf_msg->length);
        memmove(buffer, buffer + buf_msg->length, data_len -= buf_msg->length);
        return recv_msg;
    }
    buffer[0] = 0xFF;
    return nullptr;
}

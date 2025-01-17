#include "xm_udp.h"

UdpInfo udp_info[1];

void XM_udpStart() {
    strcpy(udp_info[0].name, "udp");
    udp_info[0].buffer = new uint8_t[UDP_RECV_BUFFER_SIZE];
    udp_info[0].data_len = 0;
    udp_info[0].tx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));

    xTaskCreate(XM_udpRecvTask, "udp_recv_task", STACK_SIZE_MEDIUM, &udp_info[0], TASK_PRIORITY_MEDIUM, &(udp_info[0].recv_task));
    xTaskCreate(XM_udpSendTask, "udp_send_task", STACK_SIZE_MEDIUM, &udp_info[0], TASK_PRIORITY_MEDIUM, &(udp_info[0].send_task));
}

void XM_udpRecvTask(void* param) {
    UdpInfo* udp_info = (UdpInfo*)param;
    Message* recv_msg = nullptr;
    auto &buffer = udp_info[0].buffer;
    auto &data_len = udp_info[0].data_len;
    auto &udp = wifi_info[0].udp;
    auto &tx_queue = udp_info[0].tx_queue;

    while (true) {
        while (WiFi.status() == WL_CONNECTED && udp.parsePacket() > 0) {
            int read_size = udp.read(buffer + data_len, UDP_RECV_BUFFER_SIZE - data_len);
            if (read_size > 0) data_len += read_size;
        }
        while (data_len > 0 && buffer[0] != MSG_HEAD) memmove(buffer, buffer + 1, --data_len);  // 删除非包头的数据
        if (data_len >= sizeof(Message)) {
            if (data_len >= ((Message*)buffer)->length) {
                /* 检查不同报文的报文格式和校验和是否正确 */
                if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_CMD) {
                    recv_msg = XM_udpCheckMsg<MsgCmd>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgCmd* cmd = new MsgCmd();
                        memcpy(cmd, recv_msg, sizeof(MsgCmd));
                        xQueueSend(console_info[0].rx_queue, &cmd, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_DATA) {
                    recv_msg = XM_udpCheckMsg<MsgData>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgData* data = new MsgData();
                        memcpy(data, recv_msg, sizeof(MsgData));
                        xQueueSend(console_info[0].rx_queue, &data, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_REQUEST) {
                    recv_msg = XM_udpCheckMsg<MsgReq>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgReq* req = new MsgReq();
                        memcpy(req, recv_msg, sizeof(MsgReq));
                        xQueueSend(console_info[0].rx_queue, &req, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_STATE) {
                    recv_msg = XM_udpCheckMsg<MsgState>(buffer, data_len);
                    if (recv_msg != nullptr) {
                        MsgState* state = new MsgState();
                        memcpy(state, recv_msg, sizeof(MsgState));
                        xQueueSend(console_info[0].rx_queue, &state, 0);
                    }
                }
                else if ((((Message*)buffer)->type & 0xF0) == MSG_TYPE_LOG) {
                    recv_msg = XM_udpCheckMsg<MsgLog>(buffer, data_len);
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

void XM_udpSendTask(void* param) {
    UdpInfo* udp_info = (UdpInfo*)param;
    Message* send_msg = nullptr;
    auto &udp = wifi_info[0].udp;
    auto &tx_queue = udp_info[0].tx_queue;

    while (true) {       
        /*发送数据*/
        if (xQueueReceive(tx_queue, &send_msg, portMAX_DELAY) == pdPASS) {
            if (send_msg != nullptr) {
                if (WiFi.status() == WL_CONNECTED) {
                    udp.beginPacket(udp.remoteIP(), udp.remotePort());
                    udp.write((uint8_t*)send_msg, send_msg->length);
                    udp.endPacket();
                }
                delete send_msg;
                send_msg = nullptr;
            }
        }
    }
}

template <typename MsgType>
MsgType* XM_udpCheckMsg(uint8_t* buffer, uint16_t &data_len) {
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

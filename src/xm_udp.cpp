#include "xm_udp.h"

UdpInfo udp_info[1];

void XM_udpStart() {
    if (udp_info[0].task == nullptr) {
        Serial.begin(115200);
        strcpy(udp_info[0].name, "udp");
        udp_info[0].buffer = new uint8_t[UDP_RECV_BUFFER_SIZE];
        udp_info[0].data_len = 0;
        udp_info[0].udp = WiFiUDP();
        udp_info[0].rx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
        udp_info[0].tx_queue = xQueueCreate(UART_MSG_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_udpTask, "udp_task", STACK_SIZE_MEDIUM, &udp_info[0], TASK_PRIORITY_MEDIUM, &(udp_info[0].task));
    }
}

void XM_udpTask(void *param) {
    UdpInfo *udp_info = (UdpInfo*)param;
    Message *msg = nullptr;
    auto &name = udp_info->name;
    auto &buffer = udp_info->buffer;
    auto &data_len = udp_info->data_len;
    auto &udp = udp_info->udp;
    auto &rx_queue = udp_info->rx_queue;
    auto &tx_queue = udp_info->tx_queue;
    while (true) {
        /*接收数据*/
        if (XM_wifiConnect(udp)) {
            while (udp.parsePacket()) {
                udp.read(buffer + data_len, UDP_RECV_BUFFER_SIZE - data_len);
            }
        }
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
                udp.beginPacket(udp.remoteIP(), udp.remotePort());
                udp.write((uint8_t*)msg, msg->length);
                udp.endPacket();
                delete msg;
                msg = nullptr;
            }
        }
        vTaskDelay(10);
    }
}

bool XM_wifiConnect(WiFiUDP &udp) {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        for (int i = 0; i < 30; i++) {
            if (WiFi.status() == WL_CONNECTED) {
                udp.begin(UDP_PORT);
                break;
            }
            vTaskDelay(100);
        }
        return WiFi.status() == WL_CONNECTED;
    }
    return true;
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

#include "thread_udp.h"

#include "thread_console.h"

UdpResource udp_res[1];

/// @brief UDP发送线程任务函数
/// @param param 
void udpThreadSendTask(void* param) {
    UdpResource* udp_res = (UdpResource*)param;
    Message send_msg;

    while (1) {
        if (xQueueReceive(udp_res->queue_tx, &send_msg, portMAX_DELAY) == pdPASS) {
            // 等待 WiFi 连接
            if (udp_res->wifi->status() != WL_CONNECTED) {
                while (udp_res->wifi->status() != WL_CONNECTED) vTaskDelay(100);
            }
            
            // 发送数据
            udp_res->udp->beginPacket(udp_res->udp->remoteIP(), udp_res->udp->remotePort());
            udp_res->udp->write((uint8_t*)&send_msg, send_msg.length);
            udp_res->udp->endPacket();
            DEBUG_LOG("报文发送成功");
        }
    }
}

/// @brief UDP接收线程任务函数
/// @param param 
void udpThreadRecvTask(void* param) {
    UdpResource* udp_res = (UdpResource*)param;
    Message recv_msg;
    
    while (1) {
        // 等待 WiFi 连接
        while (udp_res->wifi->status() != WL_CONNECTED) vTaskDelay(100);

        // 读取网络数据
        if (udp_res->udp->parsePacket() > 0) {
            while (udp_res->udp->available() && udp_res->data_len < UDP_BUFFER_SIZE) {
                udp_res->buffer[udp_res->data_len++] = udp_res->udp->read();
            }
        }

        // 处理消息头
        while (udp_res->data_len > 0 && udp_res->buffer[0] != MSG_HEADER) {
            memmove(udp_res->buffer, udp_res->buffer + 1, --udp_res->data_len);
        }

        // 处理完整消息
        if (udp_res->data_len >= 4) {
            uint16_t recv_msg_len = ((Message*)udp_res->buffer)->length;
            // 报文长度异常
            if (recv_msg_len < 0 || recv_msg_len > MSG_SIZE) {
                memmove(udp_res->buffer, udp_res->buffer + 1, --udp_res->data_len);
            }
            else if (udp_res->data_len >= recv_msg_len + 4) {
                memcpy(&recv_msg, udp_res->buffer, recv_msg_len + 4);
                udp_res->data_len -= recv_msg_len + 4;
                memmove(udp_res->buffer, udp_res->buffer + recv_msg_len + 4, udp_res->data_len);
                // 发送至控制台
                DEBUG_LOG("接收到一个报文, data_len = %d", recv_msg.length);
            }
        } 
        else vTaskDelay(10);
    }
}

/// @brief 初始化并创建UDP线程
/// @param  
void udpThreadCreate(void) {
    udp_res[0].buffer = new uint8_t[UDP_BUFFER_SIZE];
    udp_res[0].data_len = 0;
    udp_res[0].wifi = &WiFi;
    udp_res[0].udp = new WiFiUDP();
    udp_res[0].udp->begin(UDP_PORT);
    udp_res[0].queue_tx = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(Message));
    udp_res[0].htask_send = NULL;
    udp_res[0].htask_recv = NULL;
    xTaskCreate(udpThreadSendTask, "udp_send_task", STACK_SMALL, &udp_res[0], PRIORITY_MEDIUM, &(udp_res[0].htask_send));
    xTaskCreate(udpThreadRecvTask, "udp_recv_task", STACK_MEDIUM, &udp_res[0], PRIORITY_MEDIUM, &(udp_res[0].htask_recv));
}

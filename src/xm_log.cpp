#include "xm_log.h"

/*
目前是用的串口0
但是最终目标是
串口1发送报文
串口0发送明文
*/

LogInfo log_info[1];

void XM_logStart() {
    XM_uart0Start();
    XM_udpStart();
    if (log_info[0].task == nullptr) {
        strcpy(log_info[0].name, "log");
        log_info[0].uart_info = &uart_info[0];
        log_info[0].udp_info = &udp_info[0];
        log_info[0].tx_queue = xQueueCreate(LOG_MSG_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_logTask, "log_task", STACK_SIZE_MEDIUM, &log_info[0], TASK_PRIORITY_MEDIUM, &(log_info[0].task));
    }
}

void XM_logTask(void *param) {
    LogInfo *log_info = (LogInfo*)param;
    auto &name = log_info->name;
    auto &uart_info = log_info->uart_info;
    auto &udp_info = log_info->udp_info;
    auto &tx_queue = log_info->tx_queue;
    MsgLog *msg_log = nullptr;
    while (true) {
        if (xQueueReceive(tx_queue, &msg_log, portMAX_DELAY) == pdPASS) {
            MsgLog *msg_log_uart = new MsgLog();
            memcpy(msg_log_uart, msg_log, sizeof(MsgLog));
            xQueueSend(uart_info->tx_queue, &msg_log_uart, 0);

            MsgLog *msg_log_udp = new MsgLog();
            memcpy(msg_log_udp, msg_log, sizeof(MsgLog));
            xQueueSend(udp_info->tx_queue, &msg_log_udp, 0);
            
            delete msg_log;
        }
        vTaskDelay(10);
    }
}

void XM_logSendMsg(uint8_t level, const char* info, ...) {
    MsgLog *msg_log = new MsgLog();
    msg_log->type = level;

    va_list args;
    va_start(args, info);
    vsnprintf((char*)msg_log->data, sizeof(msg_log->data), info, args);
    va_end(args);

    msg_log->updateCheck();
    if (xQueueSend(log_info[0].tx_queue, &msg_log, 0) != pdPASS) delete msg_log;
}

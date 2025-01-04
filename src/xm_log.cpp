#include "xm_log.h"

LogInfo log_info;

void XM_logStart() {
    XM_uart0Start();
    if (log_info.task == nullptr) {
        strcpy(log_info.name, "log");
        log_info.tx_queue = xQueueCreate(LOG_MSG_QUEUE_SIZE, sizeof(void*));
        xTaskCreate(XM_logTask, "log_task", STACK_SIZE_MEDIUM, &log_info, TASK_PRIORITY_MEDIUM, &(log_info.task));
    }
}

void XM_logTask(void *param) {
    LogInfo *log_info = (LogInfo*)param;
    auto &name = log_info->name;
    auto &tx_queue = log_info->tx_queue;
    MsgLog *msg_log = nullptr;
    while (true) {
        if (xQueueReceive(tx_queue, &msg_log, portMAX_DELAY) == pdPASS) {
            xQueueSend(uart0_info.tx_queue, &msg_log, portMAX_DELAY);
        }
    }
}

void XM_logSend(uint8_t level, const char* info, ...) {
    MsgLog *msg_log = new MsgLog();
    msg_log->type = level;

    va_list args;
    va_start(args, info);
    vsnprintf((char*)msg_log->data, sizeof(msg_log->data), info, args);
    va_end(args);

    msg_log->updateCheck();
    if (xQueueSend(log_info.tx_queue, &msg_log, 0) != pdPASS) delete msg_log;
}

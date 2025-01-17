#include "xm_console.h"

ConsoleInfo console_info[1];

void XM_consoleStart() {
    strcpy(console_info[0].name, "console");
    console_info[0].rx_queue = xQueueCreate(CONSOLE_MSG_QUEUE_SIZE, sizeof(void*));
    xTaskCreate(XM_consoleTask, "console_task", STACK_SIZE_MEDIUM, &console_info[0], TASK_PRIORITY_MEDIUM, &(console_info[0].task));
}

void XM_consoleTask(void* param) {
    ConsoleInfo* console_info = (ConsoleInfo*)param;
    Message* msg = nullptr;
    auto &rx_queue = console_info[0].rx_queue;

    LOG_INFO("log_task is running.");
    while (true) {
        if (xQueueReceive(rx_queue, &msg, portMAX_DELAY) == pdPASS) {
            /* 根据类型不同区别处理 */
            if ((msg->type & 0xF0) == MSG_TYPE_CMD) {
                XM_consoleDoCmd((MsgCmd*)msg);
            }
            else if ((msg->type & 0xF0) == MSG_TYPE_DATA) {
                XM_consoleDoData((MsgData*)msg);
            }
            else if ((msg->type & 0xF0) == MSG_TYPE_REQUEST) {
                XM_consoleDoState((MsgState*)msg);
            }
            else if ((msg->type & 0xF0) == MSG_TYPE_STATE) {
                XM_consoleDoReq((MsgReq*)msg);
            }
            else if ((msg->type & 0xF0) == MSG_TYPE_LOG) {
                XM_consoleDoLog((MsgLog*)msg);
            }

            if (msg != nullptr) {
                delete msg;
                msg = nullptr;
            }
        }
    }
}

void XM_consoleDoCmd(MsgCmd* cmd) {
    Serial.printf("recv a MsgCmd.\n");
}

void XM_consoleDoData(MsgData* data) {
    Serial.printf("recv a MsgData.\n");
}

void XM_consoleDoState(MsgState* state) {
    Serial.printf("recv a MsgState.\n");
}

void XM_consoleDoReq(MsgReq* req) {
    Serial.printf("recv a MsgReq.\n");
}

void XM_consoleDoLog(MsgLog* log) {
    Serial.printf("recv a MsgLog.\n");
}

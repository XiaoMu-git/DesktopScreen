#include "thread_console.h"

ConsoleResource consoles_res[1];

/// @brief 处理命令类型消息
/// @param msg 
void XM_consoleDoCmd(Message* msg) {
    Serial.printf("recv a MsgCmd.\n");
}

/// @brief 处理命令类型消息
/// @param msg 
void XM_consoleDoData(Message* msg) {
    Serial.printf("recv a MsgData.\n");
}

/// @brief 处理命令类型消息
/// @param msg 
void XM_consoleDoState(Message* msg) {
    Serial.printf("recv a MsgState.\n");
}

/// @brief 处理命令类型消息
/// @param msg 
void XM_consoleDoReq(Message* msg) {
    Serial.printf("recv a MsgReq.\n");
}

/// @brief console 线程任务函数
/// @param param 
void consoleThreadTask(void* param) {
    ConsoleResource* console_res = (ConsoleResource*)param;
    Message recv_msg;

    while (1) {
        if (xQueueReceive(console_res->queue_rx, &recv_msg, portMAX_DELAY) == pdPASS) {
            /* 根据类型不同区别处理 */
            if ((recv_msg.type & 0xF0) == MSG_TYPE_CMD) {
                XM_consoleDoCmd(&recv_msg);
            }
            else if ((recv_msg.type & 0xF0) == MSG_TYPE_DATA) {
                XM_consoleDoData(&recv_msg);
            }
            else if ((recv_msg.type & 0xF0) == MSG_TYPE_REQUEST) {
                XM_consoleDoState(&recv_msg);
            }
            else if ((recv_msg.type & 0xF0) == MSG_TYPE_STATE) {
                XM_consoleDoReq(&recv_msg);
            }
        }
    }
}

/// @brief 初始化并创建 console 线程
/// @param  
void consoleThreadCreate(void) {
    consoles_res[0].queue_rx = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(Message));
    consoles_res[0].htask = NULL;
    xTaskCreate(consoleThreadTask, "console_thread", STACK_SMALL, &consoles_res[0], PRIORITY_MEDIUM, &consoles_res[0].htask);
}

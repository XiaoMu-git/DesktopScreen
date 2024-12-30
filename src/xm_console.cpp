#include "xm_console.h"

ConsoleInfo console_info;

void XM_consoleInit() {
    if (console_info.task == nullptr) {
        XM_uart0Init();
        strcpy(console_info.name, "console");
        console_info.buffer = new uint8_t[CONSOLE_BUFFER_SIZE];
        console_info.data_length = 0;
        xTaskCreate(XM_consoleTask, "console_task", 2048, &console_info, 10, &(console_info.task));
    }
}

void XM_consoleTask(void *param) {
    ConsoleInfo *console_info = (ConsoleInfo*)param;
    Message message;
    while (true) {
        if (xQueueReceive(uart0_info.rx_queue, &message, portMAX_DELAY) == pdPASS) {
            if (console_info->data_length + message.length <= CONSOLE_BUFFER_SIZE) {
                // 将数据读到缓存区
                
                // 然后从缓存区读取正确的报文

                // 之后执行报文指令
            }
            else Serial.printf("%s rx buffer is full.\n", console_info->name);
            if (message.data != nullptr) delete[] message.data;
        }
        Serial.printf("%s_thread running.\n", console_info->name);
    }
}

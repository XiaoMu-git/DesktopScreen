#include "xm_console.h"

ConsoleInfo console_info;

void XM_consoleInit() {
    XM_uart0Init();
    console_info.name = "console";
    console_info.buffer = new uint8_t[CONSOLE_BUFFER_SIZE];
    xTaskCreate(XM_consoleTask, "console_task", 1024, &console_info, 10, &(console_info.task));
}

void XM_consoleTask(void *param) {
    ConsoleInfo *console_info = (ConsoleInfo*)param;
    while (true) {
        // 将数据读到缓存区
        // 然后从缓存区读取正确的报文
        // 之后执行报文指令

    }
}

#include "thread_display.h"

DisplayResource displays_res[1];

/// @brief display 线程任务函数
/// @param param 
void displayThreadTask(void* param)  {
    DisplayResource* display_res = (DisplayResource*)param;
    // 初始化 oled
    display_res->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display_res->display->setTextColor(SSD1306_WHITE);
    display_res->display->setTextSize(1);

    while (1) {
        display_res->display->clearDisplay();
        display_res->display->setCursor(0, 0);
        display_res->display->printf("Hello World.\n");
        display_res->display->printf("Hello World.\n");
        display_res->display->display();
        vTaskDelay(1000);
    }
}

/// @brief 初始化并创建 display 线程
/// @param  
void displayThreadCreate(void) {
    // 初始化 display 资源
    displays_res[0].i2c = &Wire;
    displays_res[0].display = new Adafruit_SSD1306(128, 64, &Wire);
    displays_res[0].htask = NULL;
    xTaskCreate(displayThreadTask, "display_thread", STACK_SMALL, &displays_res[0], PRIORITY_MEDIUM, &displays_res[0].htask);
}

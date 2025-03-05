#ifndef _THREADDISPLAY_H_
#define _THREADDISPLAY_H_

#include <Adafruit_SSD1306.h>
#include "thread_config.h"

// display 资源
typedef struct {
    TwoWire* i2c;                   // i2c 句柄
    Adafruit_SSD1306* display;      // 显示器驱动句柄
    TaskHandle_t htask;             // 任务句柄
} DisplayResource;
extern DisplayResource displays_res[1];

/// @brief 初始化并创建 display 线程
/// @param  
void displayThreadCreate(void);

#endif // !_THREADDISPLAY_H_

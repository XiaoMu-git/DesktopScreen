#ifndef _THREADLED_H_
#define _THREADLED_H_

#include "thread_config.h"

// 控制 led 的消息类型
typedef struct {
    uint8_t state;                  // 默认 led 状态
    uint32_t switch_time;           // 亮灭切换时间
} LedCmd;

// led 信息
typedef struct {
    uint8_t led_pin;                // led 引脚
    TaskHandle_t htask;             // 任务句柄
    QueueHandle_t queue_cmd;        // led 命令队列
    TimerHandle_t timer_switch;     // 控制闪烁的定时器
} LedResource;
extern LedResource leds_res[4];

/// @brief 对外开放的接口, 用来控制 led
/// @param led_res 
/// @param state 
/// @param switch_time 
/// @return 
uint8_t setLedState(LedResource* led_res, uint8_t state, uint32_t switch_time);

/// @brief 初始化并创建 led 线程
/// @param  
void ledThreadCreate(void);

#endif // !_THREADLED_H_

#ifndef _THREADBUZZER_H_
#define _THREADBUZZER_H_

#include "thread_config.h"

// 控制 buzzer 的消息类型
typedef struct {
    uint8_t state;                  // 默认 buzzer 状态
    uint32_t switch_time;           // 亮灭切换时间
} BuzzerCmd;

// led 信息
typedef struct {
    uint8_t buzzer_pin;             // buzzer 引脚
    TaskHandle_t htask;             // 任务句柄
    QueueHandle_t queue_cmd;        // buzzer 命令队列
    TimerHandle_t timer_switch;     // 控制蜂鸣的定时器
} BuzzerResource;
extern BuzzerResource buzzers_res[1];

/// @brief 对外开放的接口, 用来控制 buzzer
/// @param buzzer_res 
/// @param state 
/// @param switch_time 
/// @return 
uint8_t setBuzzerState(BuzzerResource* buzzer_res, uint8_t state, uint32_t switch_time);

/// @brief 初始化并创建 buzzer 线程
/// @param  
void buzzerThreadCreate(void);

#endif // !_THREADBUZZER_H_

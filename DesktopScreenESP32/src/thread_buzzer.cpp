#include "thread_buzzer.h"

BuzzerResource buzzers_res[1];

/// @brief 对外开放的接口, 用来控制 buzzer
/// @param buzzer_res 
/// @param state 
/// @param switch_time 
/// @return 
uint8_t setBuzzerState(BuzzerResource* buzzer_res, uint8_t state, uint32_t switch_time) {
    BuzzerCmd buzzer_cmd;
    buzzer_cmd.state = state;
    buzzer_cmd.switch_time = switch_time;
    return xQueueSend(buzzer_res->queue_cmd, &buzzer_cmd, 0) == pdTRUE;
}

/// @brief buzzer 反转定时器函数
/// @param xTimer 
void switchBuzzerTimer(TimerHandle_t xTimer) {
    BuzzerResource* buzzer_res = (BuzzerResource*)pvTimerGetTimerID(xTimer);
    // 检查有效性
    if (buzzer_res == NULL) return;
    // 反转电平
    digitalWrite(buzzer_res->buzzer_pin, !digitalRead(buzzer_res->buzzer_pin));
}

/// @brief buzzer 线程任务函数
/// @param param 
void buzzerThreadTask(void* param) {
    BuzzerResource* buzzer_res = (BuzzerResource*)param;
    BuzzerCmd buzzer_cmd;

    while (1) {
        if (xQueueReceive(buzzer_res->queue_cmd, &buzzer_cmd, portMAX_DELAY) == pdTRUE) {
            // 删除之前的定时器
            if (buzzer_res->timer_switch != NULL) {
                xTimerDelete(buzzer_res->timer_switch, 0);
                buzzer_res->timer_switch = NULL;
            }

            // 初始化引脚
            if (buzzer_cmd.state == 0) digitalWrite(buzzer_res->buzzer_pin, HIGH);
            else digitalWrite(buzzer_res->buzzer_pin, LOW);
            
            // 创建新的定时器，并启动
            if (buzzer_cmd.switch_time != 0) {
                buzzer_res->timer_switch = xTimerCreate("buzzer1_timer", pdMS_TO_TICKS(buzzer_cmd.switch_time), pdTRUE, param, switchBuzzerTimer);
                xTimerStart(buzzer_res->timer_switch, 0);
                DEBUG_LOG("定时器模式改变, timer_switch = %d", buzzer_res->timer_switch);
            }
        }
    }
}

/// @brief 初始化并创建 buzzer 线程
/// @param  
void buzzerThreadCreate(void) {
    // 初始化 buzzer 资源
    buzzers_res[0].buzzer_pin = BUZZER_PIN;
    buzzers_res[0].htask = NULL;
    buzzers_res[0].timer_switch = NULL;
    buzzers_res[0].queue_cmd = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(BuzzerCmd));
    xTaskCreate(buzzerThreadTask, "buzzer_thread", STACK_SMALL, &buzzers_res[0], PRIORITY_MEDIUM, &buzzers_res[0].htask);
}
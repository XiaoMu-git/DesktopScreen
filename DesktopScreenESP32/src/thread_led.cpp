#include "thread_led.h"

LedResource leds_res[4];

/// @brief 对外开放的接口, 用来控制 led
/// @param led_res 
/// @param state 
/// @param switch_time 
/// @return 
uint8_t setLedState(LedResource* led_res, uint8_t state, uint32_t switch_time) {
    LedCmd led_cmd;
    led_cmd.state = state;
    led_cmd.switch_time = switch_time;
    return xQueueSend(led_res->queue_cmd, &led_cmd, 0) == pdTRUE;
}

/// @brief led 反转定时器函数
/// @param xTimer 
void switchLedTimer(TimerHandle_t xTimer) {
    LedResource* led_res = (LedResource*)pvTimerGetTimerID(xTimer);
    // 检查有效性
    if (led_res == NULL) return;
    // 反转电平
    digitalWrite(led_res->led_pin, !digitalRead(led_res->led_pin));
}

/// @brief led 线程任务函数
/// @param param 
void ledThreadTask(void* param) {
    LedResource* led_res = (LedResource*)param;
    LedCmd led_cmd;

    while (1) {
        if (xQueueReceive(led_res->queue_cmd, &led_cmd, portMAX_DELAY) == pdTRUE) {
            // 删除之前的定时器
            if (led_res->timer_switch != NULL) {
                xTimerDelete(led_res->timer_switch, 0);
                led_res->timer_switch = NULL;
            }

            // 初始化引脚
            if (led_cmd.state == 0) digitalWrite(led_res->led_pin, HIGH);
            else digitalWrite(led_res->led_pin, LOW);
            
            // 创建新的定时器，并启动
            if (led_cmd.switch_time != 0) {
                led_res->timer_switch = xTimerCreate("led1_timer", pdMS_TO_TICKS(led_cmd.switch_time), pdTRUE, param, switchLedTimer);
                xTimerStart(led_res->timer_switch, 0);
                DEBUG_LOG("灯光模式改变, timer_switch = %d", led_res->timer_switch);
            }
        }
        DEBUG_LOG("剩余栈空间: %04u", uxTaskGetStackHighWaterMark(NULL));
    }
}

/// @brief 初始化并创建 led 线程
/// @param 
void ledThreadCreate() {
    // 初始化 led 资源
    leds_res[0].led_pin = LED1_PIN;
    leds_res[0].htask = NULL;
    leds_res[0].timer_switch = NULL;
    leds_res[0].queue_cmd = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(LedCmd));
    xTaskCreate(ledThreadTask, "led1_thread", STACK_SMALL, &leds_res[0], PRIORITY_MEDIUM, &leds_res[0].htask);
    
    leds_res[1].led_pin = LED2_PIN;
    leds_res[1].htask = NULL;
    leds_res[1].timer_switch = NULL;
    leds_res[1].queue_cmd = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(LedCmd));
    xTaskCreate(ledThreadTask, "led2_thread", STACK_SMALL, &leds_res[1], PRIORITY_MEDIUM, &leds_res[1].htask);
    
    leds_res[2].led_pin = LED3_PIN;
    leds_res[2].htask = NULL;
    leds_res[2].timer_switch = NULL;
    leds_res[2].queue_cmd = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(LedCmd));
    xTaskCreate(ledThreadTask, "led3_thread", STACK_SMALL, &leds_res[2], PRIORITY_MEDIUM, &leds_res[2].htask);
    
    leds_res[3].led_pin = LED4_PIN;
    leds_res[3].htask = NULL;
    leds_res[3].timer_switch = NULL;
    leds_res[3].queue_cmd = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(LedCmd));
    xTaskCreate(ledThreadTask, "led4_thread", STACK_SMALL, &leds_res[3], PRIORITY_MEDIUM, &leds_res[3].htask);
}

#include "thread_key.h"

KeyResource keys_res[4];

void IRAM_ATTR key1ISR();
void IRAM_ATTR key2ISR();
void IRAM_ATTR key3ISR();
void IRAM_ATTR key4ISR();

/// @brief key 线程任务函数
/// @param param 
void keyThreadTask(void* param) {
    KeyResource* key_res = (KeyResource*)param;
    
    while (1) {
        if (xSemaphoreTake(key_res->sem_push, portMAX_DELAY) == pdTRUE) {
            // 软件消抖
            uint16_t time = 0;
            for (; digitalRead(key_res->key_pin) == LOW; time++) {
                if (time >= 5) break;
                vTaskDelay(100);
            }
            
            // 判断按下时间
            if (time >= 5) {
                /* 处理长按 */
                DEBUG_LOG("按钮按下, 长按");
            }
            else if (time > 0) {
                /* 处理短按 */
                DEBUG_LOG("按钮按下, 短按");
            }

            // 删除可能由于抖动快速生产出来的新的信号量
            while (xSemaphoreTake(key_res->sem_push, 0) == pdTRUE);
        }
    }
}

/// @brief 初始化并创建 key 线程
/// @param  
void keyThreadCreate(void) {
    // 配置外设中断
    attachInterrupt(digitalPinToInterrupt(KEY1_PIN), key1ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(KEY2_PIN), key2ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(KEY3_PIN), key3ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(KEY4_PIN), key4ISR, FALLING);

    // 初始化 key 资源
    keys_res[0].key_pin = KEY1_PIN;
    keys_res[0].htask = NULL;
    keys_res[0].sem_push = xSemaphoreCreateBinary();
    xTaskCreate(keyThreadTask, "key1_thread", STACK_SMALL, &keys_res[0], PRIORITY_MEDIUM, &keys_res[0].htask);
    
    keys_res[1].key_pin = KEY2_PIN;
    keys_res[1].htask = NULL;
    keys_res[1].sem_push = xSemaphoreCreateBinary();
    xTaskCreate(keyThreadTask, "key2_thread", STACK_SMALL, &keys_res[1], PRIORITY_MEDIUM, &keys_res[1].htask);
    
    keys_res[2].key_pin = KEY3_PIN;
    keys_res[2].htask = NULL;
    keys_res[2].sem_push = xSemaphoreCreateBinary();
    xTaskCreate(keyThreadTask, "key3_thread", STACK_SMALL, &keys_res[2], PRIORITY_MEDIUM, &keys_res[2].htask);
    
    keys_res[3].key_pin = KEY4_PIN;
    keys_res[3].htask = NULL;
    keys_res[3].sem_push = xSemaphoreCreateBinary();
    xTaskCreate(keyThreadTask, "key4_thread", STACK_SMALL, &keys_res[3], PRIORITY_MEDIUM, &keys_res[3].htask);
}


/// @brief key1 外部中断
/// @return 
void IRAM_ATTR key1ISR() {
    BaseType_t haveHighThread = pdFALSE;

    // 释放资源
    xSemaphoreGiveFromISR(keys_res[0].sem_push, &haveHighThread);

    // 如果需要，触发任务切换
    if (haveHighThread == pdTRUE) portYIELD_FROM_ISR();
}

/// @brief key2 外部中断
/// @return 
void IRAM_ATTR key2ISR() {
    BaseType_t haveHighThread = pdFALSE;

    // 释放资源
    xSemaphoreGiveFromISR(keys_res[1].sem_push, &haveHighThread);

    // 如果需要，触发任务切换
    if (haveHighThread == pdTRUE) portYIELD_FROM_ISR();
}

/// @brief key1 外部中断
/// @return 
void IRAM_ATTR key3ISR() {
    BaseType_t haveHighThread = pdFALSE;

    // 释放资源
    xSemaphoreGiveFromISR(keys_res[2].sem_push, &haveHighThread);

    // 如果需要，触发任务切换
    if (haveHighThread == pdTRUE) portYIELD_FROM_ISR();
}

/// @brief key1 外部中断
/// @return 
void IRAM_ATTR key4ISR() {
    BaseType_t haveHighThread = pdFALSE;

    // 释放资源
    xSemaphoreGiveFromISR(keys_res[3].sem_push, &haveHighThread);

    // 如果需要，触发任务切换
    if (haveHighThread == pdTRUE) portYIELD_FROM_ISR();
}

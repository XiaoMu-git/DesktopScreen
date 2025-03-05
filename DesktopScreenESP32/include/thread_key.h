#ifndef _THREADKEY_H_
#define _THREADKEY_H_

#include "thread_config.h"

// key 资源
typedef struct {
    uint8_t key_pin;                // key 引脚
    TaskHandle_t htask;             // 任务句柄
    SemaphoreHandle_t sem_push;     // 按下信号量
} KeyResource;
extern KeyResource keys_res[4];

/// @brief 初始化并创建 key 线程
/// @param  
void keyThreadCreate(void);

#endif // !_THREADKEY_H_

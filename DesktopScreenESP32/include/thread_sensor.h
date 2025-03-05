#ifndef _THREADSENSOR_H_
#define _THREADSENSOR_H_

#include "thread_config.h"
#include <Adafruit_AHTX0.h>
#include <BH1750.h>

// 传感器数据
typedef struct {
    float temp;
    float humi;
    float light;
    float volume;
} SensorData;

// sensor 资源
typedef struct {
    uint8_t mike_pin;               // key 引脚
    TwoWire* i2c;                   // i2c 句柄
    Adafruit_AHTX0* aht10;          // 温湿度
    BH1750* bh1750;                 // 光照
    TaskHandle_t htask;             // 任务句柄
} SensorResource;
extern SensorResource sensors_res[1];

/// @brief 初始化并创建 sensor 线程
/// @param  
void sensorThreadCreate(void);

#endif // !_THREADSENSOR_H_

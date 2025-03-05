#ifndef _THREADCHTTP_H_
#define _THREADCHTTP_H_

#include "thread_config.h"

// 天气数据
typedef struct {
    char date[16];
    uint8_t week;
    char dayweather[8];
    char nightweather[8];
    int8_t daytemp;
    int8_t nighttemp;
    char daywind[8];
    char nightwind[8];
} Weather;

// http 数据
typedef struct {
    uint64_t time;
    uint32_t adcode;
    char province[16];
    char city[16];
    Weather weathers;
} HttpData;

// http 资源
typedef struct {
    TaskHandle_t htask;             // 任务句柄
    WiFiClass* wifi;                // wifi 句柄
    HTTPClient* http;               // http 句柄
} HttpResource;
extern HttpResource https_res[1];

/// @brief 初始化并创建 http 线程
/// @param  
void httpThreadCreate(void);

#endif // !_THREADCHTTP_H_

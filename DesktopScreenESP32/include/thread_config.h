#ifndef _THREADCONFIG_H_
#define _THREADCONFIG_H_

#include <Arduino.h>
#include <FreeRTOSConfig.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/* freertos */
// 常用栈大小
#define STACK_LARGE                 8192
#define STACK_MEDIUM                4096
#define STACK_SMALL                 2048

// 常用优先级
#define PRIORITY_HIGH               12
#define PRIORITY_MEDIUM             8
#define PRIORITY_LOW                4

// 常用消息队列长度
#define QUEUE_SIZE_LARGE            64
#define QUEUE_SIZE_MEDIUM           32
#define QUEUE_SIZE_SMALL            16

/* wifi */
#define WIFI_SSID                   "XiaoMi_WiFi"
#define WIFI_PASSWORD               "123/liumx"

/* debug */
#define USE_DEBUG
#ifdef USE_DEBUG
    #define DEBUG_LOG(format, ...) Serial.printf("[%s] " format "\n", pcTaskGetName(NULL), ##__VA_ARGS__)
#else
    #define DEBUG_LOG(format, ...) // 忽略所有日志
#endif

/* iic */
#define I2C0_SDA_PIN                14
#define I2C0_SCL_PIN                13

/* led */
#define LED1_PIN                    19
#define LED2_PIN                    21
#define LED3_PIN                    22
#define LED4_PIN                    23

/* key */
#define KEY1_PIN                    16
#define KEY2_PIN                    17
#define KEY3_PIN                    5
#define KEY4_PIN                    18

/* buzzer */
#define BUZZER_PIN                  15

/* sensor */
#define MIKE_PIN                    34
#define SENSOR_SAMPLING_RATE        10000

/* uart */
#define UART0_BAUD                  115200
#define UART0_BUFFER_SIZE           10240

/* udp */
#define UDP_BUFFER_SIZE             10240
#define UDP_PORT                    12345

/* console */
#define MSG_HEADER                  0xAA
#define MSG_SIZE                    256

/* http */
#define HTTP_SAMPLING_RATE          300000
#define HTTP_TIME_URL               "https://cn.apihz.cn/api/time/getapi.php?id=88888888&key=88888888&type=1"
#define HTTP_ADDRESS_URL            "https://restapi.amap.com/v3/ip?key=ad87b9d11bdc6e97a4e746f5656536b5"
#define HTTP_WEATHER_URL            "https://restapi.amap.com/v3/weather/weatherInfo?key=ad87b9d11bdc6e97a4e746f5656536b5&city=370600&extensions=all"

/// @brief 初始化所有外设
/// @param  
void peripheralInit(void);

/// @brief 启动所有线程
/// @param  
void createAllThread(void);

#endif // !_THREADCONFIG_H_

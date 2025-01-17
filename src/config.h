#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

/* FreeRTOS */
#define TASK_PRIORITY_LOW       5
#define TASK_PRIORITY_MEDIUM    10
#define TASK_PRIORITY_HIGH      15

#define STACK_SIZE_SMALL        2048
#define STACK_SIZE_MEDIUM       4096
#define STACK_SIZE_LARGE        8192

/* Message */
#define MSG_HEAD                0xBE
#define MSG_END                 0xED
#define MSG_MAX_LENGTH          256

/* Uart */
#define UART_RECV_BUFFER_SIZE   10240
#define UART_MSG_QUEUE_SIZE     64

typedef struct {
    char name[16];
    uint8_t* buffer;
    uint16_t data_len;
    HardwareSerial* uart;
    QueueHandle_t tx_queue;
    TaskHandle_t recv_task;
    TaskHandle_t send_task;
} UartInfo;
extern UartInfo uart_info[1];

/* WiFi */
#define WIFI_SSID               "MeiKaiLin"
#define WIFI_PASSWORD           "12345678a"
typedef struct {
    char name[16];
    char ssid[64];
    char password[64];
    uint16_t udp_port;
    WiFiClass* wifi;
    WiFiUDP udp;
    TaskHandle_t task;
} WifiInfo;
extern WifiInfo wifi_info[1];

/* UDP */
#define UDP_RECV_BUFFER_SIZE    10240
#define UDP_MSG_QUEUE_SIZE      64
#define UDP_PORT                20000

typedef struct {
    char name[16];
    uint8_t* buffer;
    uint16_t data_len;
    QueueHandle_t tx_queue;
    TaskHandle_t recv_task;
    TaskHandle_t send_task;
} UdpInfo;
extern UdpInfo udp_info[1];

/* Log */
#define LOG_MSG_QUEUE_SIZE      64

typedef struct {
    char name[16];
    QueueHandle_t tx_queue;
    TaskHandle_t task;
} LogInfo;
extern LogInfo log_info[1];

/* Console */
#define CONSOLE_MSG_QUEUE_SIZE  64

typedef struct {
    char name[16];
    QueueHandle_t rx_queue;
    TaskHandle_t task;
} ConsoleInfo;
extern ConsoleInfo console_info[1];

/* HTTP */
#define HTTP_ADDRESS_URL        "https://restapi.amap.com/v3/ip?key=ad87b9d11bdc6e97a4e746f5656536b5"
#define HTTP_WEATHRE_URL        "https://restapi.amap.com/v3/weather/weatherInfo?key=ad87b9d11bdc6e97a4e746f5656536b5&city=370214&extensions=all"
#define HTTP_TIME_URL           "https://sapi.k780.com/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json"

#endif // !_CONFIG_H_

#ifndef _CONFIG_H_
#define _CONFIG_H_

/*FreeRTOS*/
#define TASK_PRIORITY_LOW       5
#define TASK_PRIORITY_MEDIUM    10
#define TASK_PRIORITY_HIGH      15

#define STACK_SIZE_SMALL        2048
#define STACK_SIZE_MEDIUM       4096
#define STACK_SIZE_LARGE        8192

/*Message*/
#define MSG_HEAD                0xBE
#define MSG_END                 0xED
#define MSG_MAX_LENGTH          256

/*Uart*/
#define UART_RECV_BUFFER_SIZE   10240
#define UART_MSG_QUEUE_SIZE     64

/*Log*/
#define LOG_MSG_QUEUE_SIZE      64

/*WiFi*/
#define WIFI_SSID               "XiaoMi_WiFi"
#define WIFI_PASSWORD           "123/liumx"

/*UDP*/
#define UDP_RECV_BUFFER_SIZE    10240
#define UDP_MSG_QUEUE_SIZE      64
#define UDP_PORT                20000

/*HTTP*/
#define HTTP_ADDRESS_URL        "https://restapi.amap.com/v3/ip?key=ad87b9d11bdc6e97a4e746f5656536b5"
#define HTTP_WEATHRE_URL        "https://restapi.amap.com/v3/weather/weatherInfo?key=ad87b9d11bdc6e97a4e746f5656536b5&city=370214&extensions=all"
#define HTTP_TIME_URL           "https://sapi.k780.com/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json"

#endif // !_CONFIG_H_

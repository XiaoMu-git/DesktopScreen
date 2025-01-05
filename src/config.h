#ifndef _CONFIG_H_
#define _CONFIG_H_

/*System*/
#define DEBU

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

#endif // !_CONFIG_H_

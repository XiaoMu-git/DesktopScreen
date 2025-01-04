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
#define MSG_MAX_LENGTH          0x00FF

/*Uart*/
#define UART_RECV_BUFFER_SIZE   65536
#define UART_MSG_QUEUE_SIZE     64

/*Log*/
#define LOG_MSG_QUEUE_SIZE      64

#endif // !_CONFIG_H_

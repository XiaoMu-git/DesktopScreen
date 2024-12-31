#ifndef _CONFIG_H_
#define _CONFIG_H_

/*FreeRTOS*/
#define TASK_PRIORITY_LOW       5
#define TASK_PRIORITY_MEDIUM    10
#define TASK_PRIORITY_HIGH      15

#define STACK_SIZE_SMALL        2048
#define STACK_SIZE_MEDIUM       4096
#define STACK_SIZE_LARGE        8192

/*uart*/
#define UART_RECV_BUFFER_SIZE   10240
#define UART_MESSAGE_QUEUE_SIZE 64



#endif // !_CONFIG_H_

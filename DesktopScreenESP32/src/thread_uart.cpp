#include "thread_uart.h"

#include "thread_console.h"

UartResource uarts_res[1];

/// @brief uart 发送线程任务函数
/// @param param
void uartThreadSendTask(void* param) {
    UartResource* uart_res = (UartResource*)param;
    Message send_msg;

    while (1) {
        if (xQueueReceive(uart_res->queue_tx, &send_msg, portMAX_DELAY) == pdPASS) {
            uart_res->uart->write((uint8_t*)(&send_msg), send_msg.length);
            DEBUG_LOG("报文发送成功");
        }
    }
}

/// @brief uart 接收线程任务函数
/// @param param 
void uartThreadRecvTask(void* param) {
    UartResource* uart_res = (UartResource*)param;
    Message recv_msg;
    
    while (1) {
        // 读取缓存区数据
        while (uart_res->uart->available() && uart_res->data_len < UART0_BUFFER_SIZE) {
            uart_res->buffer[uart_res->data_len++] = uart_res->uart->read();
        }
        
        // 处理消息头
        while (uart_res->data_len > 0 && uart_res->buffer[0] != MSG_HEADER) {
            memmove(uart_res->buffer, uart_res->buffer + 1, --uart_res->data_len);
        }

        // 处理完整消息
        if (uart_res->data_len >= 4) {
            uint16_t recv_msg_len = ((Message*)uart_res->buffer)->length;
            // 报文长度异常
            if (recv_msg_len < 0 || recv_msg_len > MSG_SIZE) {
                memmove(uart_res->buffer, uart_res->buffer + 1, --uart_res->data_len);
            }
            else if (uart_res->data_len >= recv_msg_len + 4) {
                memcpy(&recv_msg, uart_res->buffer, recv_msg_len + 4);
                uart_res->data_len -= recv_msg_len + 4;
                memmove(uart_res->buffer, uart_res->buffer + recv_msg_len + 4, uart_res->data_len);
                // 发送至控制台
                DEBUG_LOG("接收到一个报文, data_len = %d", recv_msg.length);
            }
        } 
        else vTaskDelay(10);
    }
}

/// @brief 初始化并创建 uart 线程
/// @param  
void uartThreadCreate(void) {
    uarts_res[0].buffer = new uint8_t[UART0_BUFFER_SIZE];
    uarts_res[0].data_len = 0;
    uarts_res[0].uart = &Serial;
    uarts_res[0].queue_tx = xQueueCreate(QUEUE_SIZE_LARGE, sizeof(Message));
    uarts_res[0].htask_send = NULL;
    uarts_res[0].htask_recv = NULL;
    xTaskCreate(uartThreadSendTask, "uart_send_task", STACK_SMALL, &uarts_res[0], PRIORITY_MEDIUM, &(uarts_res[0].htask_send));
    xTaskCreate(uartThreadRecvTask, "uart_recv_task", STACK_MEDIUM, &uarts_res[0], PRIORITY_MEDIUM, &(uarts_res[0].htask_recv));
}

#ifndef _THREADCONSOLE_H_
#define _THREADCONSOLE_H_

#include "thread_config.h"

// 命令类型
enum MSG_TYPE {
    MSG_TYPE_CMD = 0x10,
    MSG_TYPE_CMD_RESTART,
    MSG_TYPE_DATA = 0x20,
    MSG_TYPE_DATA_ALL_CONFIG,
    MSG_TYPE_REQUEST = 0x30,
    MSG_TYPE_REQUEST_ALL_CONFIG,
    MSG_TYPE_STATE = 0x40,
    MSG_TYPE_STATE_ALIVE
};

// 消息类基类
typedef struct {
public:
    uint8_t head;
    uint8_t type;
    uint16_t length;
    uint8_t data[MSG_SIZE];
} Message;

// console 资源
typedef struct {
    QueueHandle_t queue_rx;         // 命令接收队列
    TaskHandle_t htask;             // 任务句柄
} ConsoleResource;
extern ConsoleResource consoles_res[1];

/// @brief 初始化并创建 console 线程
/// @param  
void consoleThreadCreate(void);

#endif // !_THREADCONSOLE_H_

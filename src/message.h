#ifndef _MSG_H_
#define _MSG_H_

#include <Arduino.h>
#include "config.h"

enum MSG_TYPE {
    MSG_TYPE_CMD = 0x10,
    MSG_TYPE_CMD_RESTART,
    MSG_TYPE_DATA = 0x20,
    MSG_TYPE_DATA_ALL_CONFIG,
    MSG_TYPE_DATA_WIFI_CONFIG,
    MSG_TYPE_REQUEST = 0x30,
    MSG_TYPE_REQUEST_ALL_CONFIG,
    MSG_TYPE_REQUEST_WIFI_CONFIG,
    MSG_TYPE_STATE = 0x40,
    MSG_TYPE_STATE_RECEIVE_OK,
    MSG_TYPE_LOG = 0x50,
    MSG_TYPE_LOG_PROMPT,
    MSG_TYPE_LOG_WARNING,
    MSG_TYPE_LOG_ERROR
};

class Message {
public:
    uint8_t head;
    uint8_t type;
    uint16_t length;
    Message();
};

class MsgCmd : Message {
public:
    uint8_t data[8];
    uint8_t check;
    uint8_t end;
    MsgCmd();
};

class MsgData : Message {
public:
    uint8_t data[128];
    uint8_t check;
    uint8_t end;
    MsgData();
};

class MsgState : Message {
public:
    uint8_t data[8];
    uint8_t check;
    uint8_t end;
    MsgState();
};

class MsgReq : Message {
public:
    uint8_t data[8];
    uint8_t check;
    uint8_t end;
    MsgReq();
};

class MsgLog : Message {
public:
    uint8_t data[64];
    uint8_t check;
    uint8_t end;
    MsgLog();
};

#endif // !_MSG_H_

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <Arduino.h>

enum MESSAGE_TYPE {
    MESSAGE_TYPE_CMD = 0x10,
    MESSAGE_TYPE_CMD_RESTART,
    MESSAGE_TYPE_DATA = 0x20,
    MESSAGE_TYPE_DATA_ALL_CONFIG,
    MESSAGE_TYPE_DATA_WIFI_CONFIG,
    MESSAGE_TYPE_REQUEST = 0x30,
    MESSAGE_TYPE_REQUEST_ALL_CONFIG,
    MESSAGE_TYPE_REQUEST_WIFI_CONFIG
};

class Message {
public:
    uint8_t head;
    uint8_t type;
    uint16_t length;
};

class MsgCmd : Message {
public:
    uint8_t data[8];
    uint8_t check;
    uint8_t end;
};

class MsgData : Message {
public:
    uint8_t data[128];
    uint8_t check;
    uint8_t end;
};

class MsgReq : Message {
public:
    uint8_t data[8];
    uint8_t check;
    uint8_t end;
};

#endif // !_MESSAGE_H_

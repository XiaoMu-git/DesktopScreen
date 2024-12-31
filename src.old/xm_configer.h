#ifndef _XM_CONFIGER_H_
#define _XM_CONFIGER_H_

#include <Arduino.h>
#include "xm_uart.h"

#define CONFIGER_BUFFER_SIZE 10240

typedef struct {
    char name[16];
    uint8_t *buffer;
    uint16_t data_length;
    TaskHandle_t task;
} ConfigerInfo;

typedef struct {
    uint8_t head;
    uint8_t type;
    uint16_t length;
    uint8_t *data;
    uint8_t check;
    uint8_t end;
} Message;

extern ConfigerInfo configer_info;

void XM_configerInit();
void XM_configerTask(void *param);
Message* XM_configerGetMessage(ConfigerInfo *configer_info);
bool XM_configerDoCmd(Message* message);

#endif // !_XM_CONFIGER_H_

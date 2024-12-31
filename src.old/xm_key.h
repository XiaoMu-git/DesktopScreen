#ifndef _XM_KEY_H_
#define _XM_KEY_H_

#include <Arduino.h>

#define     KEY_UP_PIN      5
#define     KEY_DOWN_PIN    6

typedef struct {
    char name[16];
    uint8_t pin;
    uint16_t mode;
    EventGroupHandle_t event_group;
    TaskHandle_t task;
} LedInfo;

extern LedInfo led_blue_info;

void XM_keyInit();
void XM_keyTask(void *param);

#endif // !_XM_KEY_H_

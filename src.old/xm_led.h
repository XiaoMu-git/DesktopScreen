#ifndef _XM_LED_H_
#define _XM_LED_H_

#include <Arduino.h>

#define     LED_BLUE_PIN    2

#define     LED_MODE_OFF            (1 << 0)
#define     LED_MODE_ON             (1 << 1)
#define     LED_MODE_QUICK_FLASH    (1 << 2)
#define     LED_MODE_SLOW_FLASH     (1 << 3)

typedef struct {
    char name[16];
    uint8_t pin;
    uint16_t mode;
    EventGroupHandle_t event_group;
    TaskHandle_t task;
} LedInfo;

extern LedInfo led_blue_info;

void XM_ledInit();
void XM_ledTask(void *param);

#endif // !_XM_LED_H_

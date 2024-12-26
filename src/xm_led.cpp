#include "xm_led.h"

LedInfo led_blue_info;

void XM_ledInit() {
    if (led_blue_info.task == nullptr) {
        strcpy(led_blue_info.name, "led_blue");
        led_blue_info.pin = 2;
        led_blue_info.mode = LED_MODE_OFF;
        led_blue_info.event_group = xEventGroupCreate();
        xTaskCreate(XM_ledTask, "led_task", 2048, &led_blue_info, 10, &(led_blue_info.task));
    }
}

void XM_ledTask(void *param) {
    LedInfo *led_info = (LedInfo*)param;
    pinMode(led_info->pin, OUTPUT);
    uint16_t event_flag = LED_MODE_OFF | LED_MODE_ON | LED_MODE_QUICK_FLASH | LED_MODE_SLOW_FLASH;
    TickType_t start_tick = xTaskGetTickCount();
    while (true) {
        EventBits_t event_bit = xEventGroupWaitBits(led_info->event_group, event_flag, pdTRUE, pdFALSE, 0);
        if (event_bit != 0) led_blue_info.mode = event_bit;
        switch (led_blue_info.mode) {
            case LED_MODE_OFF:
                digitalWrite(led_info->pin, LOW);
                xEventGroupWaitBits(led_info->event_group, event_flag, pdFALSE, pdFALSE, portMAX_DELAY);
                break;
            case LED_MODE_ON:
                digitalWrite(led_info->pin, HIGH);
                xEventGroupWaitBits(led_info->event_group, event_flag, pdFALSE, pdFALSE, portMAX_DELAY);
                break;
            case LED_MODE_QUICK_FLASH:
                digitalWrite(led_info->pin, !digitalRead(led_info->pin));
                xEventGroupWaitBits(led_info->event_group, event_flag, pdFALSE, pdFALSE, 100);
                break;
            case LED_MODE_SLOW_FLASH:
                digitalWrite(led_info->pin, !digitalRead(led_info->pin));
                xEventGroupWaitBits(led_info->event_group, event_flag, pdFALSE, pdFALSE, 500);
                break;
            default:
                break;
        }
        Serial.printf("led_task running.\n");
    }
}

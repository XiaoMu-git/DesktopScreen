#include <Arduino.h>
#include "xm_led.h"
#include "xm_uart.h"
#include "xm_console.h"

uint32_t count = 0;

void setup() {
    XM_ledInit();
    XM_uart0Init();
}

void loop() {
    xEventGroupSetBits(led_blue_info.event_group, 1 << (count++ % 4));
    Serial.printf("loop_thread running.\n");
    vTaskDelay(3000);
}

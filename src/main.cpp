#include <Arduino.h>
#include "xm_uart.h"

void setup() {
	XM_uart0Start();
}

void loop() {
    vTaskDelay(1000);
}

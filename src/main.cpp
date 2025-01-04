#include <Arduino.h>
#include "xm_log.h"

void setup() {
	XM_logStart();
}

void loop() {
    // LOG_INFO("Hello Log %d\n", 10);
    vTaskDelay(1000);
}

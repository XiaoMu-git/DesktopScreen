#include <Arduino.h>
#include "xm_uart.h"
#include "xm_wifi.h"
#include "xm_udp.h"
#include "xm_log.h"
#include "xm_console.h"

void setup() {
    Serial.begin(115200);
    XM_wifiStart();
    XM_uartStart();
    XM_udpStart();
    XM_logStart();
    XM_consoleStart();
}

void loop() {
    LOG_INFO("loop task is running.");
    vTaskDelay(1000);
}

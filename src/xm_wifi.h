#ifndef _XM_WIFI_H_
#define _XM_WIFI_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

void XM_wifiStart();
void XM_wifiTask(void* param);

#endif // !_XM_WIFI_H_

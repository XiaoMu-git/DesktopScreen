#ifndef _XM_HTTP_H_
#define _XM_HTTP_H_

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "message.h"

typedef struct {
    char name[16];
    QueueHandle_t rx_queue;
    TaskHandle_t task;
} HttpInfo;

extern HttpInfo http_info[1];

void XM_httpStart();
void XM_httpTask(void *param);
void XM_httpGet();
bool XM_wifiConnect();

#endif // !_XM_HTTP_H_

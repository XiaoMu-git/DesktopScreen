#ifndef _XM_UDP_H_
#define _XM_UDP_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "message.h"

typedef struct {
    char name[16];
    uint8_t *buffer;
    uint16_t data_len;
    WiFiUDP udp;
    QueueHandle_t rx_queue;
    QueueHandle_t tx_queue;
    TaskHandle_t task;
} UdpInfo;

extern UdpInfo udp_info[1];

void XM_udpStart();
void XM_udpTask(void *param);
bool XM_wifiConnect(WiFiUDP &udp);
template <typename MsgType>
bool XM_uartCheckMsg(uint8_t *buffer, uint16_t &data_len, QueueHandle_t rx_queue);

#endif // !_XM_UDP_H_

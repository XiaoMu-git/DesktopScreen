#ifndef _XM_UDP_H_
#define _XM_UDP_H_

#include <Arduino.h>
#include "config.h"
#include "message.h"
#include "xm_log.h"

void XM_udpStart();
void XM_udpRecvTask(void* param);
void XM_udpSendTask(void* param);
bool XM_wifiConnect(WiFiUDP &udp);
template <typename MsgType>
MsgType* XM_udpCheckMsg(uint8_t* buffer, uint16_t &data_len);

#endif // !_XM_UDP_H_

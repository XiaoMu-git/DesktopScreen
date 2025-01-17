#ifndef _XM_UART_H_
#define _XM_UART_H_

#include <Arduino.h>
#include "config.h"
#include "message.h"
#include "xm_log.h"

void XM_uartStart();
void XM_uartRecvTask(void* param);
void XM_uartSendTask(void* param);
template <typename MsgType>
MsgType* XM_uartCheckMsg(uint8_t* buffer, uint16_t &data_len);

#endif // !_XM_UART_H_

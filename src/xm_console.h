#ifndef _XM_CONSOLE_H_
#define _XM_CONSOLE_H_

#include <Arduino.h>
#include "config.h"
#include "message.h"
#include "xm_log.h"

void XM_consoleStart();
void XM_consoleTask(void* param);
void XM_consoleDoCmd(MsgCmd* cmd);
void XM_consoleDoData(MsgData* cmd);
void XM_consoleDoState(MsgState* cmd);
void XM_consoleDoReq(MsgReq* cmd);
void XM_consoleDoLog(MsgLog* cmd);

#endif // !_XM_CONSOLE_H_

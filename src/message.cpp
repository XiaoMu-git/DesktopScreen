#include "message.h"

Message::Message() {
    this->head = MSG_HEAD;
    this->length = sizeof(Message);
}

MsgCmd::MsgCmd() {
    this->length = sizeof(MsgCmd);
    this->end = MSG_END;
}

MsgData::MsgData() {
    this->length = sizeof(MsgData);
    this->end = MSG_END;
}

MsgState::MsgState() {
    this->length = sizeof(MsgState);
    this->end = MSG_END;
}

MsgReq::MsgReq() {
    this->length = sizeof(MsgReq);
    this->end = MSG_END;
}

MsgLog::MsgLog() {
    this->length = sizeof(MsgLog);
    this->end = MSG_END;
}

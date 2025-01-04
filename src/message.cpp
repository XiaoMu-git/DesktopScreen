#include "message.h"

/*Message*/
Message::Message() {
    this->head = MSG_HEAD;
    this->type = 0x00;
    this->length = sizeof(Message);
}

/*MsgCmd*/
MsgCmd::MsgCmd() {
    this->length = sizeof(MsgCmd);
    memset(this->data, 0x00, sizeof(this->data));
    this->check = 0x00;
    this->end = MSG_END;
}

void MsgCmd::updateCheck() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    this->check = check;
}

bool MsgCmd::calculate() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    return this->head == MSG_HEAD && this->check == check && this->end == MSG_END;
}

/*MsgData*/
MsgData::MsgData() {
    this->length = sizeof(MsgData);
    memset(this->data, 0x00, sizeof(this->data));
    this->check = 0x00;
    this->end = MSG_END;
}

void MsgData::updateCheck() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    this->check = check;
}

bool MsgData::calculate() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    return this->head == MSG_HEAD && this->check == check && this->end == MSG_END;
}

/*MsgState*/
MsgState::MsgState() {
    this->length = sizeof(MsgState);
    memset(this->data, 0x00, sizeof(this->data));
    this->check = 0x00;
    this->end = MSG_END;
}

void MsgState::updateCheck() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    this->check = check;
}

bool MsgState::calculate() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    return this->head == MSG_HEAD && this->check == check && this->end == MSG_END;
}

/*MsgReq*/
MsgReq::MsgReq() {
    this->length = sizeof(MsgReq);
    memset(this->data, 0x00, sizeof(this->data));
    this->check = 0x00;
    this->end = MSG_END;
}

void MsgReq::updateCheck() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    this->check = check;
}

bool MsgReq::calculate() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    return this->head == MSG_HEAD && this->check == check && this->end == MSG_END;
}

/*MsgLog*/
MsgLog::MsgLog() {
    this->length = sizeof(MsgLog);
    memset(this->data, 0x00, sizeof(this->data));
    this->check = 0x00;
    this->end = MSG_END;
}

void MsgLog::updateCheck() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    this->check = check;
}

bool MsgLog::calculate() {
    uint8_t check = 0x00;
    for (int i = 0; i < sizeof(this->data); i++) check += this->data[i];
    return this->head == MSG_HEAD && this->check == check && this->end == MSG_END;
}

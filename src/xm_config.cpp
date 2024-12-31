#include "xm_config.h"

ConfigerInfo configer_info;

void XM_configerInit() {
    if (configer_info.task == nullptr) {
        XM_uart1Init();
        strcpy(configer_info.name, "configer");
        configer_info.buffer = new uint8_t[CONFIGER_BUFFER_SIZE];
        configer_info.data_length = 0;
        xTaskCreate(XM_configerTask, "configer_task", 2048, &configer_info, 10, &(configer_info.task));
    }
}

void XM_configerTask(void *param) {
    ConfigerInfo *configer_info = (ConfigerInfo*)param;
    DataBlock data_block;
    while (true) {
        if (xQueueReceive(uart1_info.rx_queue, &data_block, portMAX_DELAY) == pdPASS) {
            if (configer_info->data_length + data_block.length <= CONFIGER_BUFFER_SIZE) {
                // 将数据读到缓存区
                memcpy(configer_info->buffer + configer_info->data_length, data_block.data, data_block.length);
                configer_info->data_length += data_block.length;
                while (true) {
                    // 然后从缓存区读取正确的报文
                    Message *message = XM_configerGetMessage(configer_info);
                    if (message == nullptr) break;
                    // 之后执行报文指令
                    XM_configerDoCmd(message);
                }
            }
            else Serial.printf("%s rx buffer is full.\n", configer_info->name);
            if (data_block.data != nullptr) delete[] data_block.data;
        }
        Serial.printf("%s_thread running.\n", configer_info->name);
        vTaskDelay(10);
    }
}

Message* XM_configerGetMessage(ConfigerInfo *configer_info) {
    while (configer_info->data_length > 0 && configer_info->buffer[0] != 0xBE) memmove(configer_info->buffer, configer_info->buffer + 1, --configer_info->data_length);
    if (configer_info->data_length >= 6) {
        uint16_t message_len = (configer_info->buffer[2] << 8 | configer_info->buffer[3]) + 6;
        // 包长异常
        if (message_len < 6) configer_info->buffer[0] = 0xFF;
        // 数据足够取出当前包长
        else if (configer_info->data_length >= message_len) {
            // 检验包尾和校验和
            uint8_t check = 0;
            for (int i = 0; i < message_len; i++) check += configer_info->buffer[i];
            // 包尾异常
            if (configer_info->buffer[message_len - 1] != 0xED) configer_info->buffer[0] = 0xFF;
            else if (configer_info->buffer[message_len - 2] != check) configer_info->buffer[0] = 0xFF;
            else {
                // 此时认为报文是正确的，就取出一个报文
                Message *message = new Message();
                message->head = configer_info->buffer[0];
                message->type = configer_info->buffer[1];
                message->length = configer_info->buffer[2] << 8 | configer_info->buffer[3];
                if (message->length > 0) {
                    message->data = new uint8_t[message->length];
                    memcpy(message->data, configer_info->buffer + 4, message->length);
                }
                message->check = check;
                message->end = configer_info->buffer[message_len - 1];
                memmove(configer_info->buffer, configer_info->buffer + message_len, configer_info->data_length);
                configer_info->data_length -= message_len;
                return message;
            }
        }
    }
    return nullptr;
}

bool XM_configerDoCmd(Message* message) {
    if (message == nullptr) return false;
    // 懒得写宏定义了，有需要参考报文规则文件
    switch (message->type) {
        case 0x11:
            // 重启
            break;
        case 0x21:
            // 所有配置信息
            break;
        case 0x22:
            // WIFI连接状态信息
            break;
        case 0x31:
            // 请求所有配置信息
            break;
        case 0x32:
            // 请求WIFI连接状态信息
            break;
        case 0x41:
            // 收到消息
            break;
        default:
            break;
    }
    if (message->data != nullptr) delete[] message->data;
    delete message;
    return true;
}

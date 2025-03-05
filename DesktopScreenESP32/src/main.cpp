#include <Arduino.h>
#include "thread_config.h"

void setup() {
    /* 外设初始化 */
    peripheralInit();
    DEBUG_LOG("硬件初始化完成");
    
    /* 创建所有线程 */
    createAllThread();
}

void loop() {
    // 维护 wifi 连接
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
        while (WiFi.status() != WL_CONNECTED) vTaskDelay(100);
        DEBUG_LOG("wifi 重连成功");
        DEBUG_LOG("ip 地址: %s", WiFi.localIP().toString().c_str());
    }
    delay(5000);
}

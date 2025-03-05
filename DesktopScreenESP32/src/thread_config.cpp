#include "thread_config.h"

#include "thread_led.h"
#include "thread_key.h"
#include "thread_buzzer.h"
#include "thread_sensor.h"
#include "thread_display.h"
#include "thread_uart.h"
#include "thread_udp.h"
#include "thread_console.h"
#include "thread_http.h"

/// @brief 初始化所有外设
/// @param  
void peripheralInit(void) {
    // led
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);
    pinMode(LED4_PIN, OUTPUT);
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);

    // key
    pinMode(KEY1_PIN, INPUT_PULLUP);
    pinMode(KEY2_PIN, INPUT_PULLUP);
    pinMode(KEY3_PIN, INPUT_PULLUP);
    pinMode(KEY4_PIN, INPUT_PULLUP);

    // buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, HIGH);

    // sensor
    Wire.begin(I2C0_SDA_PIN, I2C0_SCL_PIN);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    // uart
    Serial.begin(UART0_BAUD);

    // wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) vTaskDelay(100);
    DEBUG_LOG("wifi 连接成功");
    DEBUG_LOG("用户名: %s", WIFI_SSID);
    DEBUG_LOG("ip 地址: %s", WiFi.localIP().toString().c_str());
    DEBUG_LOG("udp 端口: %d", UDP_PORT);
}

/// @brief 启动所有线程
/// @param  
void createAllThread(void) {
    ledThreadCreate();          // 创建 led 线程
    keyThreadCreate();          // 创建 key 线程
    buzzerThreadCreate();       // 创建 buzzer 线程
    sensorThreadCreate();       // 创建 sensor 线程
    displayThreadCreate();      // 创建 display 线程
    uartThreadCreate();         // 创建 uart 线程
    udpThreadCreate();          // 创建 udp 线程
    consoleThreadCreate();      // 创建 console 线程
    httpThreadCreate();         // 创建 http 线程
}

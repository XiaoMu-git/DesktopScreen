#include "xm_wifi.h"

WifiInfo wifi_info[1];

void XM_wifiStart() {
    strcpy(wifi_info[0].name, "wifi");
    strcpy(wifi_info[0].ssid, WIFI_SSID);
    strcpy(wifi_info[0].password, WIFI_PASSWORD);
    wifi_info[0].wifi = &WiFi;
    wifi_info[0].udp_port = UDP_PORT;
    wifi_info[0].udp = WiFiUDP();
    xTaskCreate(XM_wifiTask, "wifi_task", STACK_SIZE_MEDIUM, &wifi_info[0], TASK_PRIORITY_MEDIUM, &(wifi_info[0].task));
}

void XM_wifiTask(void* param) {
    WifiInfo* wifi_info = (WifiInfo*)param;
    auto &wifi = wifi_info->wifi;
    auto &ssid = wifi_info->ssid;
    auto &password = wifi_info->password;
    auto &udp_port = wifi_info->udp_port;
    auto &udp = wifi_info->udp;

    while (true) {
        if (wifi->status() != WL_CONNECTED) {
            wifi->begin(ssid, password);
            for (int i = 0; i < 50; i++) {
                if (wifi->status() == WL_CONNECTED) {
                    Serial.printf("连接到wifi, IP: ");
                    Serial.println(wifi->localIP());
                    udp.begin(udp_port);
                    break;
                }
                vTaskDelay(100);
            }
        }
        vTaskDelay(1000);
    }
}

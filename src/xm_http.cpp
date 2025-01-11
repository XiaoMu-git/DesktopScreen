#include "xm_http.h"

HttpInfo http_info[1];

void XM_httpStart() {

}

void XM_httpTask(void *param) {

}

void XM_httpGet() {
    
}

bool XM_wifiConnect() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        for (int i = 0; i < 30; i++) {
            if (WiFi.status() == WL_CONNECTED) break;
            vTaskDelay(100);
        }
        return WiFi.status() == WL_CONNECTED;
    }
    return true;
}
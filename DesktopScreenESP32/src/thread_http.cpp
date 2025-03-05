#include "thread_http.h"

HttpResource https_res[1];

/// @brief http 线程任务函数
/// @param param 
void httpThreadTask(void* param) {
    HttpResource* http_res = (HttpResource*)param;
    HttpData recv_data;

    while (1) {
        // 等待 WiFi 连接
        while (http_res->wifi->status() != WL_CONNECTED) vTaskDelay(100);

        // 请求时间
        http_res->http->begin(HTTP_TIME_URL);
        if (http_res->http->GET() == HTTP_CODE_OK) {
            String timePayload = http_res->http->getString();
            JsonDocument timeDoc;
            DeserializationError error = deserializeJson(timeDoc, timePayload);
            if (!error && timeDoc["code"] == 200) {
                recv_data.time = strtoull(timeDoc["msg"], NULL, 10);
            }
        }
        http_res->http->end();
        
        // 请求位置
        http_res->http->begin(HTTP_ADDRESS_URL);
        if (http_res->http->GET() == HTTP_CODE_OK) {
            String addrPayload = http_res->http->getString();
            JsonDocument addrDoc;
            DeserializationError error = deserializeJson(addrDoc, addrPayload);
            if (!error && addrDoc["status"] == "1") {
                // 使用静态数组直接复制
                strlcpy(recv_data.province, addrDoc["province"], sizeof(recv_data.province));
                strlcpy(recv_data.city, addrDoc["city"], sizeof(recv_data.city));
                recv_data.adcode = addrDoc["adcode"];
            }
        }
        http_res->http->end();
        
        // 请求天气
        http_res->http->begin(HTTP_WEATHER_URL);
        if (http_res->http->GET() == HTTP_CODE_OK) {
            String weatherPayload = http_res->http->getString();
            JsonDocument weatherDoc;
            DeserializationError error = deserializeJson(weatherDoc, weatherPayload);
            if (!error && weatherDoc["status"] == "1") {
                JsonArray lives = weatherDoc["lives"];
                if (lives.size() > 0) {
                    JsonObject live = lives[0];
                    Weather* weather = &recv_data.weathers;

                    // 时间转换
                    time_t ts = recv_data.time;
                    struct tm* timeinfo = localtime(&ts);
                    strftime(weather->date, sizeof(weather->date), "%Y-%m-%d", timeinfo);
                    weather->week = timeinfo->tm_wday % 7;

                    // 天气数据填充
                    strlcpy(weather->dayweather, live["weather"], sizeof(weather->dayweather));
                    strlcpy(weather->nightweather, live["weather"], sizeof(weather->nightweather));
                    weather->daytemp = live["temperature"];
                    weather->nighttemp = live["temperature"];
                    strlcpy(weather->daywind, live["winddirection"], sizeof(weather->daywind));
                    strlcpy(weather->nightwind, live["winddirection"], sizeof(weather->nightwind));
                }
            }
        }
        http_res->http->end();

        // 打印接收到的数据
        DEBUG_LOG("time: %lld", recv_data.time);
        DEBUG_LOG("Location: %s/%s (adcode: %d)", recv_data.province, recv_data.city, recv_data.adcode);
        DEBUG_LOG("Weather: ");
        DEBUG_LOG("Date: %s Week: %d", recv_data.weathers.date, recv_data.weathers.week);
        DEBUG_LOG("Day: %d°C %s %s", recv_data.weathers.daytemp, recv_data.weathers.dayweather, recv_data.weathers.daywind);
        DEBUG_LOG("Night: %d°C %s %s", recv_data.weathers.nighttemp, recv_data.weathers.nightweather, recv_data.weathers.nightwind);


        vTaskDelay(HTTP_SAMPLING_RATE);
    }
}

/// @brief 初始化并创建 http 线程
/// @param  
void httpThreadCreate(void) {
    https_res[0].wifi = &WiFi;
    https_res[0].http = new HTTPClient();
    https_res[0].htask = NULL;
    xTaskCreate(httpThreadTask, "http_thread", STACK_LARGE, &https_res[0], PRIORITY_MEDIUM, &https_res[0].htask);
}

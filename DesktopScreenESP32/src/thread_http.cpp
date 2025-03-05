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
                // 清空历史数据
                recv_data.day_num = 0;
                
                // 解析预报数据
                JsonArray forecasts = weatherDoc["forecasts"];
                if (forecasts.size() > 0) {
                    JsonObject city_forecast = forecasts[0];
                    JsonArray casts = city_forecast["casts"];
                    
                    // 限制最多获取7天数据
                    recv_data.day_num = casts.size() > 7 ? 7 : casts.size();
                    
                    // 填充每日天气
                    for (int i = 0; i < recv_data.day_num; i++) {
                        JsonObject cast = casts[i];
                        Weather* weather = &recv_data.weathers[i];
                        
                        // 直接使用预报中的日期和星期
                        strlcpy(weather->date, cast["date"], sizeof(weather->date));
                        weather->week = cast["week"].as<uint8_t>() % 7; // 转换到0-6范围
                        
                        // 解析天气要素
                        strlcpy(weather->dayweather, cast["dayweather"], sizeof(weather->dayweather));
                        strlcpy(weather->nightweather, cast["nightweather"], sizeof(weather->nightweather));
                        weather->daytemp = cast["daytemp"].as<int8_t>();
                        weather->nighttemp = cast["nighttemp"].as<int8_t>();
                        strlcpy(weather->daywind, cast["daywind"], sizeof(weather->daywind));
                        strlcpy(weather->nightwind, cast["nightwind"], sizeof(weather->nightwind));
                    }
                }
            }
        }
        http_res->http->end();
        
        // 打印接收到的数据
        DEBUG_LOG("time: %lld", recv_data.time);
        DEBUG_LOG("Location: %s/%s (adcode: %d)", recv_data.province, recv_data.city, recv_data.adcode);
        DEBUG_LOG("Weather: ");
        for (int i = 0; i < recv_data.day_num; i++) {
            Weather* w = &recv_data.weathers[i];
            DEBUG_LOG("[Day %d] %s 周%d", i+1, w->date, w->week + 1); // 周数显示恢复1-7
            DEBUG_LOG("  白天: %s %d℃ %s风", w->dayweather, w->daytemp, w->daywind);
            DEBUG_LOG("  夜间: %s %d℃ %s风", w->nightweather, w->nighttemp, w->nightwind);
        }

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

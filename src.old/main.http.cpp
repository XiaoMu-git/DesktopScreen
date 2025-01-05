#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi信息
const char* ssid = "XiaoMi_WiFi";
const char* password = "123/liumx";

// HTTP API URL
const String apiURL = "https://jsonplaceholder.typicode.com/todos/1"; // 示例API

void setup() {
  Serial.begin(115200);
  Serial.println("Free Flash Space: " + String(ESP.getFreeSketchSpace()));


  // 连接到WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // 请求JSON数据
  HTTPClient http;
  http.begin(apiURL); // 传入API URL

  // 发送GET请求
  int httpCode = http.GET();

  // 检查请求是否成功
  if (httpCode > 0) {
    Serial.printf("HTTP GET request sent, code: %d\n", httpCode);
    
    // 获取响应内容
    String payload = http.getString();
    Serial.println("Response:");
    Serial.println(payload);

    // 使用 JsonDocument 来解析 JSON 数据
    JsonDocument doc;  // JsonDocument 类

    // 解析JSON数据
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    // 访问JSON字段
    int userId = doc["userId"];
    int id = doc["id"];
    String title = doc["title"];
    bool completed = doc["completed"];

    // 打印JSON字段
    Serial.println("Parsed JSON:");
    Serial.print("User ID: ");
    Serial.println(userId);
    Serial.print("ID: ");
    Serial.println(id);
    Serial.print("Title: ");
    Serial.println(title);
    Serial.print("Completed: ");
    Serial.println(completed);
  } else {
    Serial.printf("HTTP request failed, code: %d\n", httpCode);
  }

  http.end(); // 关闭HTTP连接
}

void loop() {
  // 在loop中可以进行其他操作
}

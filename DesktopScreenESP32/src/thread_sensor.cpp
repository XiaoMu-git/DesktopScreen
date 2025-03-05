#include "thread_sensor.h"

SensorResource sensors_res[1];

/// @brief sensor 线程任务函数
/// @param param 
void sensorThreadTask(void* param)  {
    SensorResource* sensor_res = (SensorResource*)param;
    // 初始化 BH1750
    sensor_res->bh1750->begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, sensor_res->i2c);
    // 初始化 AHT10
    sensor_res->aht10->begin(sensor_res->i2c);

    while (1) {
        SensorData sensor_data = {0};
        for (int i = 0; i < 10; i++) {
            // 读取传感器信息
            sensors_event_t humi, temp;
            sensor_res->aht10->getEvent(&humi, &temp);
            sensor_data.temp += temp.temperature;
            sensor_data.humi += humi.relative_humidity;
            sensor_data.light += sensor_res->bh1750->readLightLevel();
            sensor_data.volume += analogRead(sensor_res->mike_pin);
            vTaskDelay(10);
        }
        sensor_data.temp /= 10;
        sensor_data.humi /=10;
        sensor_data.light /= 10;
        sensor_data.volume /= 10;

        DEBUG_LOG("temp: %0.2f, humi: %0.2f, light: %0.2f, volume: %0.2f",
            sensor_data.temp, sensor_data.humi, sensor_data.light, sensor_data.volume);
            
        vTaskDelay(SENSOR_SAMPLING_RATE);
    }
}

/// @brief 初始化并创建 sensor 线程
/// @param  
void sensorThreadCreate(void) {
    // 初始化 sensor 资源
    sensors_res[0].mike_pin = MIKE_PIN;
    sensors_res[0].i2c = &Wire;
    sensors_res[0].aht10 = new Adafruit_AHTX0();
    sensors_res[0].bh1750 = new BH1750();
    sensors_res[0].htask = NULL;
    xTaskCreate(sensorThreadTask, "sensor_thread", STACK_SMALL, &sensors_res[0], PRIORITY_MEDIUM, &sensors_res[0].htask);
}

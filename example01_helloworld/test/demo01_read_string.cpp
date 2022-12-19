/**
 * @file demo01_read_byte.cpp
 * @author fishros@foxmail.com
 * @brief 初始化串口，当有数据过来的时候读取并将数据打印出来
 * @version 0.1
 * @date 2022-12-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <Arduino.h>

void setup()
{
    // 初始化串口
    Serial.begin(115200);
}

void loop()
{
    // 判断是否有有效数据
    if (Serial.available())
    {
        // 读取一个String字符串数据
        String str = Serial.readString();
        // 以%s的格式输出接收的数据
        Serial.printf("I receve %s\n", str.c_str());
    }
}
/**
 * @file main.cpp
 * @author fishros@foxmail.com
 * @brief 使LED灯亮1s关闭1s,持续闪烁
 * @version 0.1
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT); // 设置2号引脚模式为OUTPUT模式
}

void loop()
{
  // LED串口控制代码
  if (Serial.available())
  {
    String command = Serial.readString();
    if (command == "on")
    {
      digitalWrite(2, LOW); // 低电平，打开LED灯
    }
    else if (command == "off")
    {

      digitalWrite(2, HIGH); // 高电平，关闭LED灯
    }
  }

  /* // LED闪烁代码
    digitalWrite(2, LOW);  // 低电平，打开LED灯
    delay(1000);           // 休眠1000ms
    digitalWrite(2, HIGH); // 高电平，关闭LED灯
    delay(1000);           // 休眠1000ms
  */
}
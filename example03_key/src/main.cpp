/**
 * @file main.cpp
 * @author fishros@foxmail.com
 * @brief 使用按键控制LED灯,按着开，松开关，自锁开关
 * @version 0.1
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <Arduino.h>

bool status = false;

void setup()
{
  Serial.begin(115200);
  pinMode(0, INPUT);  // 设置2号引脚模式为OUTPUT模式
  pinMode(2, OUTPUT); // 设置2号引脚模式为OUTPUT模式
}

void loop()
{
  // 自锁开关
  if (digitalRead(0) == LOW)
  {
    delay(50); // 休眠50ms再次判断，防止误触
    if (digitalRead(0) == LOW)
    {
      status = !status;
      while (digitalRead(0) == LOW) // 死循环等待放开按键
        ;
    }
  }

  if (status == true)
  {
    digitalWrite(2, LOW); // 低电平，打开LED灯
  }
  else
  {

    digitalWrite(2, HIGH); // 低电平，打开LED灯
  }

  /*
  // 按着开，松开关
  if (digitalRead(0) == LOW)
  {
    Serial.println("LED ON");
    digitalWrite(2, LOW); // 低电平，打开LED灯
  }
  else
  {

    Serial.println("LED OFF");
    digitalWrite(2, HIGH); // 高电平，关闭LED灯
  }
  */
}
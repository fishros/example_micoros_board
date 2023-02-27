/**
 * @file main.cpp
 * @author fishros@foxmail.com
 * @brief 电机正反转控制
 * @version 0.1
 * @date 2022-12-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>

#define AIN1 23 // 电机驱动模块AIN1引脚
#define AIN2 22 // 电机驱动模块AIN2引脚
#define KEY 0   // 按键引脚

int motorStatus = 0; // 电机状态变量，0-3循环变化

void setup()
{
  Serial.begin(115200);    // 初始化串口通信
  pinMode(KEY, INPUT);     // 设置按键引脚为输入模式
  pinMode(AIN1, OUTPUT);   // 设置AIN1引脚为输出模式
  pinMode(AIN2, OUTPUT);   // 设置AIN2引脚为输出模式
}

void loop()
{
  if (digitalRead(KEY) == LOW) // 检测按键是否按下
  {
    delay(50);                 // 延迟50ms，以防止误触
    if (digitalRead(KEY) == LOW)
    {
      while (digitalRead(0) == LOW) // 等待按键松开，避免连续按下
        ;
      motorStatus++;                 // 切换电机状态
      motorStatus = motorStatus % 4; // 保持该变量值在0-4之间
    }
  }

  // 根据电机状态切换IO电平
  switch (motorStatus)
  {
    case 0:
      Serial.println("AIN1: HIGH, AIN2: LOW"); // 调试信息：AIN1为高电平，AIN2为低电平
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      break;
    case 1:
      Serial.println("AIN1: LOW, AIN2: HIGH"); // 调试信息：AIN1为低电平，AIN2为高电平
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      break;
    case 2:
      Serial.println("AIN1: HIGH, AIN2: HIGH"); // 调试信息：AIN1和AIN2均为高电平
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, HIGH);
      break;
    case 3:
      Serial.println("AIN1: LOW, AIN2: LOW"); // 调试信息：AIN1和AIN2均为低电平
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      break;
    default:
      break;
  }
}

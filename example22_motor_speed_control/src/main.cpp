/**
 * @file main.cpp
 * @author fishros@foxmail.com
 * @brief 电机速度控制
 * @version 0.1
 * @date 2022-12-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>

#define AIN1 23  // 电机驱动模块AIN1引脚
#define AIN2 22  // 电机驱动模块AIN2引脚
#define KEY 0    // 按键引脚
#define CYCLE 10 // 定义PWM信号的周期长度，单位为ms

float duty = 0.0; // 定义占空比变量，并初始化为0.0

void setup()
{
  Serial.begin(115200);   // 初始化串口通信
  pinMode(KEY, INPUT);    // 设置按键引脚为输入模式
  pinMode(AIN1, OUTPUT);  // 设置AIN1引脚为输出模式
  pinMode(AIN2, OUTPUT);  // 设置AIN2引脚为输出模式
  digitalWrite(AIN2, LOW);// 设置AIN2引脚为低电平，控制电机转向
}

void loop()
{
  // 检测按键是否按下
  if (digitalRead(KEY) == LOW) 
  {
    delay(50); // 延迟50ms，以防止误触
    // 确认按键已经按下
    if (digitalRead(KEY) == LOW)
    {
      // 等待按键松开，避免连续按下
      while (digitalRead(0) == LOW) 
        ;
      // 每次增加0.1的占空比，当占空比达到1.0时，重新从0开始计数
      duty = duty + 0.1;
      if (duty > 1.0)
        duty = 0;
    }
  }

  // 输出PWM信号控制电机转速
  digitalWrite(AIN1, HIGH);     // 将AIN1引脚设置为高电平
  delay(CYCLE * duty);           // 延迟一段时间，时间长度由占空比决定
  digitalWrite(AIN1, LOW);      // 将AIN1引脚设置为低电平
  delay(CYCLE * (1 - duty));     // 延迟一段时间，时间长度由占空比决定
}

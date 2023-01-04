/**
 * @file main.cpp
 * @author fishros@foxmail.com
 * @brief 4.电池电压测量-学会使用ADC
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright(c) fishros.com  2023
 *
 */
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  pinMode(34, INPUT);
  analogSetAttenuation(ADC_11db);
}

void loop()
{
  int analogValue = analogRead(34);                     // 读取原始值0-4096
  int analogVolts = analogReadMilliVolts(34);           // 读取模拟电压，单位毫伏
  float realVolts = 5.02 * ((float)analogVolts * 1e-3); // 计算实际电压值

  Serial.printf("ADC analog value = %d\n", analogValue);
  Serial.printf("ADC millivolts value = %d\n", analogVolts);
  Serial.printf("realVolts value = %f\n", realVolts);
  delay(100);
}
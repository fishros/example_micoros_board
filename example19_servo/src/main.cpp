#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo1; // 创建对象

void setup()
{
  servo1.setPeriodHertz(50);   // 舵机控制周期为50hz，即一个周期1000/50=20ms
  servo1.attach(4, 500, 2500); // 使用GPIO4作为舵机1信号引脚，占空比为500-2500us即 0.5-2.5ms
  servo1.write(90.0);          // 设置90度
}

void loop()
{
  for (int i = 0; i < int(180); i++)
  {
    servo1.write(i); // 设置角度
    delay(10);
  }
}
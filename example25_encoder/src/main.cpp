#include <Arduino.h>
#include <Esp32PcntEncoder.h>

Esp32PcntEncoder encoders[2]; // 创建一个数组用于存储两个编码器

void setup()
{
  // 1.初始化串口
  Serial.begin(115200); // 初始化串口通信，设置通信速率为115200

  // 2.设置编码器
  encoders[0].init(0, 32, 33); // 初始化第一个编码器，使用GPIO 32和33连接
  encoders[1].init(1, 26, 25); // 初始化第二个编码器，使用GPIO 26和25连接
}

void loop()
{
  delay(10); // 等待10毫秒

  // 读取并打印两个编码器的计数器数值
  Serial.printf("tick1=%d,tick2=%d\n", encoders[0].getTicks(), encoders[1].getTicks());
}
#include <Arduino.h>
#include <Esp32PcntEncoder.h>

Esp32PcntEncoder encoders[2]; // 创建一个数组用于存储两个编码器
int64_t last_ticks[2]; // 记录上一次读取的计数器数值
int32_t pt[2]; // 记录两次读取之间的计数器差值
int64_t last_update_time; // 记录上一次更新时间
float speeds[2]; // 记录两个电机的速度

void setup()
{
  // 1.初始化串口
  Serial.begin(115200); // 初始化串口通信，设置通信速率为115200

  // 2.设置编码器
  encoders[0].init(0, 32, 33); // 初始化第一个编码器，使用GPIO 32和33连接
  encoders[1].init(1, 26, 25); // 初始化第二个编码器，使用GPIO 26和25连接

  // 3.让电机1以最大速度转起来
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
}

void loop()
{
  delay(10); // 等待10毫秒

  // 4.计算两个电机的速度
  uint64_t dt = millis() - last_update_time; // 计算两次读取之间的时间差
  pt[0] = encoders[0].getTicks() - last_ticks[0]; // 计算第一个编码器两次读取之间的计数器差值
  pt[1] = encoders[1].getTicks() - last_ticks[1]; // 计算第二个编码器两次读取之间的计数器差值

  speeds[0] = float(pt[0] * 0.1051566) / dt; // 计算第一个电机的速度
  speeds[1] = float(pt[1] * 0.1051566) / dt; // 计算第二个电机的速度

  // 5.更新记录
  last_update_time = millis(); // 更新上一次更新时间
  last_ticks[0] = encoders[0].getTicks(); // 更新第一个编码器的计数器数值
  last_ticks[1] = encoders[1].getTicks(); // 更新第二个编码器的计数器数值

  // 6.打印信息
  Serial.printf("tick1=%d,tick2=%d\n", encoders[0].getTicks(), encoders[1].getTicks()); // 打印两个编码器的计数器数值
  Serial.printf("spped1=%f,spped2=%f\n", speeds[0], speeds[1]); // 打印两个电机的速度
}

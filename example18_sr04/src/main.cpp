#include <Arduino.h>
#define Trig 27 // 设定SR04连接的Arduino引脚
#define Echo 21

void setup()
{
  Serial.begin(115200);
  pinMode(Trig, OUTPUT); // 初始化舵机和超声波
  pinMode(Echo, INPUT);  // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
}

void loop()
{
  static double mtime;
  digitalWrite(Trig, LOW); // 测量距离
  delayMicroseconds(2);    // 延时2us
  digitalWrite(Trig, HIGH); 
  delayMicroseconds(10); // 产生一个10us的高脉冲去触发SR04
  digitalWrite(Trig, LOW);
  mtime = pulseIn(Echo, HIGH);                  // 检测脉冲宽度，注意返回值是微秒us
  float detect_distance = mtime / 58.0 / 100.0; // 计算出距离,输出的距离的单位是厘米cm
  Serial.printf("distance=%f\n", detect_distance);
  delay(500);
}
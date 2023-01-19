#include <Arduino.h>
#include "IMU.h"

MPU6050 mpu(Wire); // 初始化MPU6050对象
IMU imu(mpu);      // 初始化IMU对象

imu_t imu_data;
unsigned long timer = 0;

void setup()
{
  Serial.begin(115200);
  imu.begin(18, 19); // 初始化IMU,使用18，19引脚
}

void loop()
{
  imu.update();
  if ((millis() - timer) > 100)
  {
    imu.getImuData(imu_data); // 获取IMU数据结构体
    Serial.printf("imu:\teuler(%f,%f,%f)\n",
                  imu_data.angle_euler.x, imu_data.angle_euler.y, imu_data.angle_euler.z);
    Serial.printf("imu:\torientation(%f,%f,%f,%f)\n",
                  imu_data.orientation.w, imu_data.orientation.x, imu_data.orientation.y, imu_data.orientation.z);
    timer = millis();
  }
}
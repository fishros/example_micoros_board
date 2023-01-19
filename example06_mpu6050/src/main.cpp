/* Get all possible data from MPU6050
 * Accelerometer values are given as multiple of the gravity [1g = 9.81 m/s²]
 * Gyro values are given in deg/s
 * Angles are given in degrees
 * Note that X and Y are tilt angles and not pitch/roll.
 *
 * License: MIT
 */

#include "Wire.h"          // 导入I2C相关头文件
#include <MPU6050_light.h> // 导入MPU6050库

MPU6050 mpu(Wire); // 新建MPU6050对象mpu

unsigned long timer = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin(18, 19); // 初始化I2C，设置sda引脚为GPIO18,SCL引脚为GPIO19

  byte status = mpu.begin(); // 检测IMU模块状态
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true, true); // gyro and accelero 校准
  Serial.println("Done!\n");
}

void loop()
{
  mpu.update();

  if (millis() - timer > 1000)
  { // print data every second
    Serial.print(F("TEMPERATURE: "));
    Serial.println(mpu.getTemp()); // 温度
    Serial.print(F("ACCELERO  X: "));
    Serial.print(mpu.getAccX()); // X轴加速度
    Serial.print("\tY: ");
    Serial.print(mpu.getAccY()); // Y轴加速度
    Serial.print("\tZ: ");
    Serial.println(mpu.getAccZ()); // Z轴加速度

    Serial.print(F("GYRO      X: "));
    Serial.print(mpu.getGyroX()); // X轴 角速度
    Serial.print("\tY: ");
    Serial.print(mpu.getGyroY()); // Y轴 角速度
    Serial.print("\tZ: ");
    Serial.println(mpu.getGyroZ()); // Z轴 角速度

    Serial.print(F("ACC ANGLE X: "));
    Serial.print(mpu.getAccAngleX()); // X轴角加速度
    Serial.print("\tY: ");
    Serial.println(mpu.getAccAngleY()); // Y轴角加速度

    Serial.print(F("ANGLE     X: "));
    Serial.print(mpu.getAngleX()); // X角度
    Serial.print("\tY: ");
    Serial.print(mpu.getAngleY()); // Y角度
    Serial.print("\tZ: ");
    Serial.println(mpu.getAngleZ()); // Z角度
    Serial.println(F("=====================================================\n"));
    timer = millis();
  }
}

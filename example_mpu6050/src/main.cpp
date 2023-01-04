/* Get all possible data from MPU6050
 * Accelerometer values are given as multiple of the gravity [1g = 9.81 m/s²]
 * Gyro values are given in deg/s
 * Angles are given in degrees
 * Note that X and Y are tilt angles and not pitch/roll.
 *
 * License: MIT
 */

#include "Wire.h"
#include <MPU6050_light.h>

#include <WiFi.h>
#include <WiFiUdp.h> //引用以使用UDP

MPU6050 mpu(Wire);

long timer = 0;
WiFiUDP Udp;

void setup()
{
  Serial.begin(115200);
  Wire.begin(18, 19);

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(2000);
  mpu.calcOffsets(true, true); // gyro and accelero
  Serial.println("Done!\n");

  WiFi.mode(WIFI_AP);
  WiFi.begin("fishbot", "12345678");
  Udp.begin(8888); // 启用UDP监听以接收数据
}

void loop()
{
  mpu.update();

  if (millis() - timer > 500)
  { // print data every second
    Serial.printf("%f,%f,%f\n", (float)millis() / 1000.0, mpu.getTemp(), mpu.getAngleZ());
    Serial.print(F("TIME: "));Serial.println(millis());
    Serial.print(F("TEMPERATURE: "));Serial.println(mpu.getTemp());
    Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
    Serial.print("\tY: ");Serial.print(mpu.getAccY());
    Serial.print("\tZ: ");Serial.println(mpu.getAccZ());

    Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
    Serial.print("\tY: ");Serial.print(mpu.getGyroY());
    Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());

    Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
    Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());

    Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
    Serial.print("\tY: ");Serial.print(mpu.getAngleY());
    Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
    Serial.println(F("=====================================================\n"));
    timer = millis();
  }
}

/*

TIME: 23.847
TEMPERATURE: 41.64
ACCELERO  X: 0.01       Y: -0.00        Z: 1.01
GYRO      X: 0.04       Y: -0.05        Z: 0.13
ACC ANGLE X: -0.13      Y: -0.38
ANGLE     X: 0.03       Y: 0.01 Z: 14.16
===
TIME: 390.964
TEMPERATURE: 42.11
ACCELERO  X: 0.00       Y: 0.00 Z: 1.00
GYRO      X: -0.08      Y: -0.02        Z: 0.07
ACC ANGLE X: 0.26       Y: -0.12
ANGLE     X: 0.02       Y: 0.05 Z: 43.50
*/
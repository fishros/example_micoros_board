/* Get tilt angles on X and Y, and rotation angle on Z
 * Angles are given in degrees
 *
 * License: MIT
 */

#include "Wire.h"
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>     // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h> // 加载Adafruit_SSD1306库

Adafruit_SSD1306 display;
MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin(18, 19);
  /*========================OLED初始化====================================*/
  display = Adafruit_SSD1306(128, 64, &Wire);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 设置OLED的I2C地址
  display.clearDisplay();                    // 清空屏幕
  display.setTextSize(2);                    // 设置字体大小
  display.setCursor(0, 0);                   // 设置开始显示文字的坐标
  display.setTextColor(SSD1306_WHITE);       // 设置字体颜色
  display.println("hello oled!");            // 输出的字符
  display.display();

  /*========================IMU初始化====================================*/
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
}

void loop()
{
  mpu.update();

  if ((millis() - timer) > 100)
  { // print data every 100ms
    Serial.print("X : ");
    Serial.print(mpu.getAngleX());
    Serial.print("\tY : ");
    Serial.print(mpu.getAngleY());
    Serial.print("\tZ : ");
    Serial.println(mpu.getAngleZ());
    timer = millis();
    /*==========================OLED显示===========================*/
    display.clearDisplay();  // 清空屏幕
    display.setCursor(0, 0); // 设置开始显示文字的坐标
    display.print("X=");   // 输出X
    display.println(mpu.getAngleX());
    display.print("Y="); // 输出Y
    display.println(mpu.getAngleY());
    display.print("Z="); // 输出Z
    display.println(mpu.getAngleZ());
    display.display();
  }
}

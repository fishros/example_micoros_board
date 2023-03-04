#include <Arduino.h>
#include <Esp32McpwmMotor.h>

Esp32McpwmMotor motor; // 创建一个名为motor的对象，用于控制电机

void setup()
{
    Serial.begin(115200); // 初始化串口通信，波特率为115200

    motor.attachMotor(0, 23, 22); // 将电机0连接到引脚23和引脚22
    motor.attachMotor(1, 12, 13); // 将电机1连接到引脚12和引脚13
}

void loop()
{
    motor.updateMotorSpeed(0, -70); // 设置电机0的速度(占空比)为负70%
    motor.updateMotorSpeed(1, 70); // 设置电机1的速度(占空比)为正70%
    delay(2000); // 延迟两秒

    motor.updateMotorSpeed(0, 70); // 设置电机0的速度(占空比)为正70%
    motor.updateMotorSpeed(1, -70); // 设置电机1的速度(占空比)为负70%
    delay(2000); // 延迟两秒
}
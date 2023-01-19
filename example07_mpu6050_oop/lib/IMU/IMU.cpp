#include "IMU.h"

IMU::IMU(MPU6050 &mpu)
{
    mpu_ = &mpu;
};

bool IMU::begin(int sda, int scl)
{
    Wire.begin(sda, scl);
    byte status = mpu_->begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    if (status != 0)
    {
        return false;
    } // stop everything if could not connect to MPU6050

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
    mpu_->calcOffsets(); // gyro and accelero
    Serial.println("Done!\n");
    return true;
}

void IMU::Euler2Quaternion(float roll, float pitch, float yaw, quaternion_t &q)
{
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;
}

void IMU::getImuData(imu_t &imu)
{
    imu.angle_euler.x = mpu_->getAngleX();
    imu.angle_euler.y = -mpu_->getAngleY();
    imu.angle_euler.z = mpu_->getAngleZ();

    imu.angular_velocity.x = mpu_->getAccAngleX();
    imu.angular_velocity.y = -mpu_->getAccAngleY();
    imu.angular_velocity.z = mpu_->getGyroZ();

    imu.linear_acceleration.x = mpu_->getAccX();
    imu.linear_acceleration.y = mpu_->getAccY();
    imu.linear_acceleration.z = mpu_->getAccZ();

    IMU::Euler2Quaternion(imu.angle_euler.x, imu.angle_euler.y, imu.angle_euler.z,
                          imu.orientation);
}

void IMU::update()
{
    mpu_->update();
}
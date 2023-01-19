#ifndef __IMU_H__
#define __IMU_H__
#include "Wire.h"
#include "MPU6050_light.h"

typedef struct
{
    float w;
    float x;
    float y;
    float z;
} quaternion_t; // 四元数结构体

typedef struct
{
    float x;
    float y;
    float z;
} vector_3d_t; // 通用3D点结构体

typedef struct
{
    quaternion_t orientation;
    vector_3d_t angle_euler;
    vector_3d_t angular_velocity;
    vector_3d_t linear_acceleration;
} imu_t; // IMU数据结构体

class IMU
{
private:
    MPU6050 *mpu_; // mpu6050指针

public:
    /**
     * @brief 同u哦NPU6050构造一个新的IMU对象
     *
     * @param mpu
     */
    IMU(MPU6050 &mpu);
    
    ~IMU() = default;
    /**
     * @brief 初始化函数
     *
     * @param sda 引脚编号
     * @param scl 引脚编号
     * @return true
     * @return false
     */
    bool begin(int sda, int scl);
    /**
     * @brief 欧拉角转四元数
     *
     * @param roll 输入X
     * @param pitch 输入y
     * @param yaw 输入Z
     * @param q  返回的四元数引用
     */
    static void Euler2Quaternion(float roll, float pitch, float yaw, quaternion_t &q);
    /**
     * @brief 获取IMU数据函数
     *
     * @param imu
     */
    void getImuData(imu_t &imu);
    /**
     * @brief 更新IMU数据，同上一节中的mou.update
     *
     */
    void update();
};

#endif // __IMU_H__
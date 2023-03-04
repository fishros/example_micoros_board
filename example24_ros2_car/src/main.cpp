#include <Arduino.h>
#include <Esp32McpwmMotor.h>
#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <WiFi.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

// 定义 ROS2 执行器和支持结构
rclc_executor_t executor;
rclc_support_t support;
// 定义 ROS2 内存分配器
rcl_allocator_t allocator;
// 定义 ROS2 节点和订阅者
rcl_node_t node;
rcl_subscription_t subscriber;
// 定义接收到的消息结构体
geometry_msgs__msg__Twist sub_msg;

// 定义控制两个电机的对象
Esp32McpwmMotor motor;

// 回调函数，当接收到新的 Twist 消息时会被调用
void twist_callback(const void *msg_in)
{
  // 将接收到的消息指针转化为 geometry_msgs__msg__Twist 类型
  const geometry_msgs__msg__Twist *twist_msg = (const geometry_msgs__msg__Twist *)msg_in;
  // 从 Twist 消息中获取线速度和角速度
  float linear_x = twist_msg->linear.x;
  float angular_z = twist_msg->angular.z;
  // 打印接收到的速度信息
  Serial.printf("recv spped(%f,%f)\n", linear_x, angular_z);
  // 如果速度为零，则停止两个电机
  if (linear_x == 0 && angular_z == 0)
  {
    motor.updateMotorSpeed(0, 0);
    motor.updateMotorSpeed(1, 0);
    return;
  }

  // 根据线速度和角速度控制两个电机的转速
  if (linear_x > 0)
  {
    motor.updateMotorSpeed(0, 70);
    motor.updateMotorSpeed(1, 70);
  }

  if (linear_x < 0)
  {
    motor.updateMotorSpeed(0, -70);
    motor.updateMotorSpeed(1, -70);
  }

  if (angular_z > 0)
  {
    motor.updateMotorSpeed(0, -70);
    motor.updateMotorSpeed(1, 70);
  }

  if (angular_z < 0)
  {
    motor.updateMotorSpeed(0, 70);
    motor.updateMotorSpeed(1, -70);
  }
}

void setup()
{
  // 初始化串口
  Serial.begin(115200);

  // 初始化两个电机的引脚
  motor.attachMotor(0, 22, 23);
  motor.attachMotor(1, 12, 13);

  // 设置 micro-ROS 通信参数，连接到指定的 ROS2 代理
  IPAddress agent_ip;
  agent_ip.fromString("192.168.2.105");
  set_microros_wifi_transports("fishbot", "12345678", agent_ip, 8888);
  delay(2000);

  // 初始化 ROS2 执行器和支持结构
  allocator = rcl_get_default_allocator();
  rclc_support_init(&support, 0, NULL, &allocator);
  // 初始化 ROS2 节点
  rclc_node_init_default(&node, "esp32_car", "", &support);
  // 初始化订阅者
  rclc_subscription_init_default(
      &subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      "/cmd_vel");
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  // 设置订阅的回调函数
  rclc_executor_add_subscription(&executor, &subscriber, &sub_msg, &twist_callback, ON_NEW_DATA);
}

void loop()
{
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)); // 循环处理数据
}

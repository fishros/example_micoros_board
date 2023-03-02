#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/float32.h>

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// 声明话题发布者
rcl_publisher_t publisher;
// 声明消息文件
std_msgs__msg__Float32 pub_msg;

// 定义定时器接收回调函数
void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  RCLC_UNUSED(last_call_time);
  if (timer != NULL)
  {
    if (RCL_RET_OK != rcl_publish(&publisher, &pub_msg, NULL))
    {
      // 失败处理....
    }
  }
}

void setup()
{
  Serial.begin(115200);
  // 设置通过串口进行MicroROS通信
  set_microros_serial_transports(Serial);
  // 延时时一段时间，等待设置完成
  delay(2000);
  // 初始化内存分配器
  allocator = rcl_get_default_allocator();
  // 创建初始化选项
  rclc_support_init(&support, 0, NULL, &allocator);
  // 创建节点 topic_sub_test
  rclc_node_init_default(&node, "topic_pub_test", "", &support);
  // 发布者初始化
  rclc_publisher_init_default(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
      "battery_voltage");

  // 创建定时器，200ms发一次
  const unsigned int timer_timeout = 200;
  rclc_timer_init_default(
      &timer,
      &support,
      RCL_MS_TO_NS(timer_timeout),
      timer_callback);

  // 创建执行器
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  // 给执行器添加定时器
  rclc_executor_add_timer(&executor, &timer);
  // 初始化ADC
  pinMode(34, INPUT);
  analogSetAttenuation(ADC_11db);
}

void loop()
{
  delay(100);
  // 循环处理数据
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  // 通过ADC获取电压值
  int analogValue = analogRead(34);                     // 读取原始值0-4096
  int analogVolts = analogReadMilliVolts(34);           // 读取模拟电压，单位毫伏
  float realVolts = 5.02 * ((float)analogVolts * 1e-3); // 计算实际电压值
  pub_msg.data = realVolts;
}

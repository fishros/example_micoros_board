#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <WiFi.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

/**
 * @brief MicroROSTASK,打印ID
 *
 * @param param
 */
void microros_task(void *param)
{
  // 设置通过WIFI进行MicroROS通信
  IPAddress agent_ip;
  agent_ip.fromString("192.168.2.105");
  // 设置wifi名称，密码，电脑IP,端口号
  set_microros_wifi_transports("fishbot", "12345678", agent_ip, 8888);
  // 延时时一段时间，等待设置完成
  delay(2000);
  // 初始化内存分配器
  allocator = rcl_get_default_allocator();
  // 创建初始化选项
  rclc_support_init(&support, 0, NULL, &allocator);
  // 创建节点 microros_wifi
  rclc_node_init_default(&node, "microros_wifi", "", &support);
  // 创建执行器
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  while (true)
  {
    delay(100);
    Serial.printf("microros_task on core:%d\n", xPortGetCoreID());
    // 循环处理数据
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
  }
}

void setup()
{
  Serial.begin(115200);
  /**
   * @brief 创建一个人物在Core 0 上
   * microros_task    任务函数
   * "microros_task"  任务名称
   * 10240      任务占用内存大小
   * NULL         任务参数，为空
   * 1               任务优先级
   * NULL     任务Handle可以为空
   * 0                 内核编号
   */
  xTaskCreatePinnedToCore(microros_task, "microros_task", 10240, NULL, 1, NULL, 0);
}

void loop()
{
  delay(1000);
  Serial.printf("do some control on core:%d\n", xPortGetCoreID());
}
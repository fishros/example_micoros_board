#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <micro_ros_utilities/string_utilities.h>

#include "Wire.h"
#include <Adafruit_GFX.h>     // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h> // 加载Adafruit_SSD1306库

#include <fishbot_interfaces/srv/oled_control.h> // 添加接口

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
// 定义服务
rcl_service_t service;

// 服务请求和返回消息定义
fishbot_interfaces__srv__OledControl_Request req;
fishbot_interfaces__srv__OledControl_Response res;

Adafruit_SSD1306 display;

// 服务回调函数
void service_callback(const void *req, void *res)
{
  fishbot_interfaces__srv__OledControl_Request *req_in = (fishbot_interfaces__srv__OledControl_Request *)req;
  fishbot_interfaces__srv__OledControl_Response *res_in = (fishbot_interfaces__srv__OledControl_Response *)res;
  // 计算sum
  display.clearDisplay();                    // 清空屏幕
  display.setCursor(req_in->px, req_in->py); // 设置开始显示文字的坐标
  display.println(req_in->data.data);        // 输出的字符
  display.display();
  res_in->result = 0;
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
  // 创建节点 hello_microros
  rclc_node_init_default(&node, "example14_interface", "", &support);
  // 使用默认配置创建服务
  rclc_service_init_default(&service, &node, ROSIDL_GET_SRV_TYPE_SUPPORT(fishbot_interfaces, srv, OledControl), "/oled_control");
  // 创建执行器
  rclc_executor_init(&executor, &support.context, 1, &allocator);
  // 执行器添加服务
  rclc_executor_add_service(&executor, &service, &req, &res, service_callback);
  // 重要，为string类型消息分配空间
  req.data = micro_ros_string_utilities_init_with_size(100);

  /*========================OLED初始化====================================*/
  Wire.begin(18, 19);
  display = Adafruit_SSD1306(128, 64, &Wire);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 设置OLED的I2C地址
  display.clearDisplay();                    // 清空屏幕
  display.setTextSize(1);                    // 设置字体大小
  display.setCursor(0, 0);                   // 设置开始显示文字的坐标
  display.setTextColor(SSD1306_WHITE);       // 设置字体颜色
  display.println("hello fishros!");         // 输出的字符
  display.display();
}

void loop()
{
  delay(100);
  // 循环处理数据
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
}

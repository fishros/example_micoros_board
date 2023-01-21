#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <TimeLib.h>          // 加载时间库，提供setTime\year\month...函数
#include <Adafruit_GFX.h>     // 加载Adafruit_GFX库
#include <Adafruit_SSD1306.h> // 加载Adafruit_SSD1306库
Adafruit_SSD1306 display;     // 声明对象

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

const int timeout_ms = 1000;
static int64_t time_ms;
static time_t time_seconds;
char time_str[25];

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
  // 创建节点 example15_time_sync
  rclc_node_init_default(&node, "example15_time_sync", "", &support);
  // 创建执行器
  rclc_executor_init(&executor, &support.context, 1, &allocator);

  Wire.begin(18, 19);
  display = Adafruit_SSD1306(128, 64, &Wire);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // 设置OLED的I2C地址，默认0x3C
  display.setTextSize(2);                    // 设置字体大小，最小为1
  display.clearDisplay();                    // 清空屏幕
  display.setCursor(0, 0);                   // 设置开始显示文字的坐标
  display.setTextColor(SSD1306_WHITE);       // 设置字体颜色
  display.println("hello oled!");            // 输出的字符
}

void loop()
{
  /*=========================同步时间=====================================*/
  while (!rmw_uros_epoch_synchronized()) // 判断时间是否同步
  {
    rmw_uros_sync_session(timeout_ms); //  同步时间
    if (rmw_uros_epoch_synchronized())
    {
      time_ms = rmw_uros_epoch_millis(); // 获取当前时间
      time_seconds = time_ms / 1000;
      setTime(time_seconds + 8 * 3600); // 将当前时间+8H到北京时间然后设置到系统
    }
    delay(10);
    return;
  }

  /*========================获取时间与显示==================================*/
  sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d ", year(), month(), day(), hour(), minute(), second());

  display.clearDisplay();    // 清空屏幕
  display.setCursor(00, 0);  // 设置开始显示文字的坐标
  display.println(time_str); // 输出的字符
  display.display();
  delay(100);
}
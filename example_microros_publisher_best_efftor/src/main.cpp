#include <Arduino.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float32.h>
#include <nav_msgs/msg/odometry.h>

#include <WiFi.h>
#include <micro_ros_platformio.h>
#include "micro_ros_transport_serial.h"
#include "micro_ros_transport_wifi_udp.h"

rcl_publisher_t publisher;
std_msgs__msg__Float32 battery;
nav_msgs__msg__Odometry odom_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn)                  \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
            error_loop();            \
        }                            \
    }

#define RCSOFTCHECK(fn)                                                                 \
    {                                                                                   \
        rcl_ret_t temp_rc = fn;                                                         \
        if ((temp_rc != RCL_RET_OK))                                                    \
        {                                                                               \
            printf(                                                                     \
                "Failed status on line %d: %d. Continuing.\n", __LINE__, (int)temp_rc); \
        }                                                                               \
    }

float motor_speeds[2]{0.1, 0.2};

// Error handle loop
void error_loop()
{
    while (1)
    {
        delay(100);
    }
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    RCLC_UNUSED(last_call_time);
    if (timer != NULL)
    {
        RCSOFTCHECK(rcl_publish(&publisher, &odom_msg, NULL));
    }
}

void setup()
{
    // Configure serial transport
    Serial.begin(115200);
    IPAddress agent_ip(192, 168, 2, 105);
    size_t agent_port = 8888;
    char ssid[] = "m3";
    char psk[] = "88888888";
    set_microros_wifi_transports(ssid, psk, agent_ip, agent_port);
    // set_microros_serial_transports(Serial);
    delay(2000);

    allocator = rcl_get_default_allocator();

    // create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // create node
    RCCHECK(rclc_node_init_default(&node, "fishbot_motion_control", "", &support));

    // create publisher
    // RCCHECK(rclc_publisher_init_best_effort(
    RCCHECK(rclc_publisher_init_best_effort(
        // RCCHECK(rclc_publisher_init(
        &publisher,
        &node,
        // ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
        ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
        "fishbot_battery"));

    // create timer,
    const unsigned int timer_timeout = 100;
    RCCHECK(rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(timer_timeout),
        timer_callback));

    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
    RCCHECK(rclc_executor_add_timer(&executor, &timer));

    pinMode(34, INPUT);
}

void loop()
{
    // delay(100);
    RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}

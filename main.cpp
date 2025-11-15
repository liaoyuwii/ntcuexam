/**
 * Problem 1: EBNN MNIST Single Board
 *
 * Single ESP32 board subscribes to /mnist_input_a, performs EBNN inference,
 * and publishes predictions to /mnist_output.
 */

#include <Arduino.h>
#include <custom_msgs/msg/mnist_image.h>
#include <custom_msgs/msg/prediction.h>
#include <micro_ros_platformio.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>

#include "simple_mnist.h"

// Turn on the LED and halt when a critical ROS client error occurs
void rc_check(rcl_ret_t ret) {
  if (ret != RCL_RET_OK) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1) delay(100);
  }
}

// Blink the LED rapidly to indicate a recoverable ROS client error
void rc_check_soft(rcl_ret_t ret) {
  if (ret != RCL_RET_OK) {
    for (int i = 0; i < 5; ++i) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
    }
  }
}

rclc_support_t support;
rclc_executor_t executor;

rcl_allocator_t allocator;
rcl_init_options_t init_options;
rcl_node_t node;
rcl_publisher_t result_pub;
rcl_subscription_t input_sub;

custom_msgs__msg__MnistImage input_msg;

int ros_domain_id = 1;

// Subscription callback - convert MNIST image to float, run EBNN, publish
// prediction
void mnist_callback(const void* msgin) {
  const auto* mnist_image =
      static_cast<const custom_msgs__msg__MnistImage*>(msgin);

  // TODO: Implement the callback to perform inference and publish result
}

// Arduino setup - initialize micro-ROS node, QoS, publisher, and subscription
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();
  node = rcl_get_zero_initialized_node();

  init_options = rcl_get_zero_initialized_init_options();
  rc_check(rcl_init_options_init(&init_options, allocator));
  rc_check(rcl_init_options_set_domain_id(&init_options, ros_domain_id));

  rc_check(rclc_support_init_with_options(&support, 0, nullptr, &init_options,
                                          &allocator));

  rc_check(rclc_node_init_default(&node, "ebnn_mnist_client", "", &support));

  rc_check(rclc_executor_init(&executor, &support.context, 1, &allocator));

  custom_msgs__msg__MnistImage__init(&input_msg);

  rc_check(rclc_subscription_init_default(
      &input_sub, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(custom_msgs, msg, MnistImage),
      "/mnist_input_a"));

  rc_check(rclc_publisher_init_default(
      &result_pub, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(custom_msgs, msg, Prediction),
      "/mnist_output"));

  rc_check(rclc_executor_add_subscription(&executor, &input_sub, &input_msg,
                                          &mnist_callback, ON_NEW_DATA));
}

// Arduino loop - process micro-ROS callbacks
void loop() {
  rc_check_soft(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
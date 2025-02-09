#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/bool.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/magnetic_field.hpp>
// Your generated bindings from the Rust library:
#include "bindings.h"

using namespace std::chrono_literals;


class NavigatorNode : public rclcpp::Node
{
public:
  NavigatorNode()
  : Node("navigator_node")
  {
    // Initialize the hardware/library
    RCLCPP_INFO(this->get_logger(), "Initializing the navigator sensor library...");
    init();

    // Create publishers
    temp_pub_ = this->create_publisher<std_msgs::msg::Float32>("navigator/temp", 10);
    pressure_pub_ = this->create_publisher<std_msgs::msg::Float32>("navigator/pressure", 10);
    leak_pub_ = this->create_publisher<std_msgs::msg::Bool>("navigator/leak", 10);
    imu_pub_ = this->create_publisher<sensor_msgs::msg::Imu>("navigator/imu", 10);
    mag_pub_ = this->create_publisher<sensor_msgs::msg::MagneticField>("navigator/mag", 10);

    // Create a timer with a period of 1 ms (1000 Hz)
      timer_ = this->create_wall_timer(
        1ms,  
        std::bind(&NavigatorNode::timerCallback, this)
      );
  }

private:
  void timerCallback()
  {
    // Read from your Rust-based library:
    float temperature = read_temp();
    float pressure = read_pressure();
    bool leak = read_leak();
    AxisData accel = read_accel();
    AxisData gyro = read_gyro();
    AxisData mag = read_mag();

    // 1) Publish temperature
    std_msgs::msg::Float32 temp_msg;
    temp_msg.data = temperature;
    temp_pub_->publish(temp_msg);

    // 2) Publish pressure
    std_msgs::msg::Float32 pressure_msg;
    pressure_msg.data = pressure;
    pressure_pub_->publish(pressure_msg);

    // 3) Publish leak state
    std_msgs::msg::Bool leak_msg;
    leak_msg.data = leak;
    leak_pub_->publish(leak_msg);

    // 4) Publish IMU data
    sensor_msgs::msg::Imu imu_msg;
    // Fill linear acceleration
    imu_msg.linear_acceleration.x = accel.x;
    imu_msg.linear_acceleration.y = accel.y;
    imu_msg.linear_acceleration.z = accel.z;
    // Fill angular velocity
    imu_msg.angular_velocity.x = gyro.x;
    imu_msg.angular_velocity.y = gyro.y;
    imu_msg.angular_velocity.z = gyro.z;
    // Orientation is unknown here (unless you compute it from mag/gyro/accel),
    // so for demonstration, let's leave orientation fields as zero
    // and set orientation_covariance to indicate "no orientation estimate":
    imu_msg.orientation_covariance[0] = -1.0;

    imu_pub_->publish(imu_msg);

    // 5) Optionally publish magnetometer as a separate message
    sensor_msgs::msg::MagneticField mag_msg;
    mag_msg.magnetic_field.x = mag.x;
    mag_msg.magnetic_field.y = mag.y;
    mag_msg.magnetic_field.z = mag.z;
    mag_pub_->publish(mag_msg);

    RCLCPP_INFO_ONCE(this->get_logger(), "Publishing navigator data...");
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr temp_pub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pressure_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr leak_pub_;
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_;
  rclcpp::Publisher<sensor_msgs::msg::MagneticField>::SharedPtr mag_pub_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<NavigatorNode>();
  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}

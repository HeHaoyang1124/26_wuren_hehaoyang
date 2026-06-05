#pragma once
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"


// 先沿一个方向转圈，完成一圈后反转角速度方向，再转一圈，形成8字形。
class FigureEightController : public rclcpp::Node {
private:
    // 运动参数
    double circle_linear_speed_; // 线速度
    double circle_angular_speed_; // 角速度

    // 状态标志和跟踪变量
    bool has_pose_{false}; // 是否已接收到至少一次位姿数据
    double last_theta_{0.0}; // 上一次的角度值（用于计算角度变化）
    double accumulated_turn_{0.0}; // 累积转过的角度绝对值（用于判断是否完成一圈）

    // 当前位姿
    turtlesim::msg::Pose current_pose_;

    // ROS2通信对象
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_; // 发布器
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_; // 订阅器
    rclcpp::TimerBase::SharedPtr timer_; // 定时器

public:
    // 构造函数
    FigureEightController();

private:
    // 定时器回调函数
    void timer_callback();

    // 位姿订阅回调函数
    void subscription_callback(const turtlesim::msg::Pose::ConstSharedPtr &msg);

    // 角度归一化到 [-π, π] 范围内
    static double normalize_angle(double angle) {
        while (angle > M_PI) angle -= 2.0 * M_PI;
        while (angle < -M_PI) angle += 2.0 * M_PI;
        return angle;
    }
};

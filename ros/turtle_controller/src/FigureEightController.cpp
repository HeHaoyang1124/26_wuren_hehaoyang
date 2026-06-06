#include "FigureEightController.hpp"

// 构造函数
FigureEightController::FigureEightController() : // 节点名称: figure_eight_controller
    Node("figure_eight_controller") {
    // 从参数中获取线速度和角速度，如果未设置则使用默认值
    circle_linear_speed_ = this->declare_parameter<double>("circle_linear_speed", 2.0);
    circle_angular_speed_ = this->declare_parameter<double>("circle_angular_speed", 1.0);

    // 发布：向/turtle1/cmd_vel话题发布Twist消息
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // 订阅
    subscription_ = this->create_subscription<turtlesim::msg::Pose>(
        // 话题：/turtle1/pose
        "/turtle1/pose",
        // 队列大小：10
        10,
        // 回调函数：处理乌龟位置和朝向
        [this](const turtlesim::msg::Pose::ConstSharedPtr &msg) {
            subscription_callback(msg);
        });

    // 设置控制频率
    constexpr double control_rate_hz = 200.0;
    const auto timer_period = std::chrono::duration<double>(1.0 / control_rate_hz);

    timer_ = this->create_wall_timer(
        std::chrono::duration_cast<std::chrono::nanoseconds>(timer_period),
        [this]() { timer_callback(); });

    RCLCPP_INFO(this->get_logger(), "开始绕圈"); // 打印启动信息
}


// 定时器回调函数
void FigureEightController::timer_callback() {
    geometry_msgs::msg::Twist cmd; // 创建速度命令消息

    // 如果无乌龟的位姿信息
    if (!has_pose_) {
        // 发送零速度并返回
        publisher_->publish(cmd);
        return;
    }

    // 设置恒定的线速度和角速度
    cmd.linear.x = circle_linear_speed_; // 前进速度
    cmd.angular.z = circle_angular_speed_; // 旋转速度（正值为逆时针，负值为顺时针）

    // 更新累积转角
    const double dtheta = normalize_angle(current_pose_.theta - last_theta_);
    accumulated_turn_ += std::abs(dtheta); // 累加角度的绝对值
    last_theta_ = current_pose_.theta; // 更新参考角度

    // 判断是否完成一个完整的圆圈（转角达到360度 = 2π）
    if (accumulated_turn_ >= 2.0 * M_PI) {
        circle_angular_speed_ *= -1.0; // 反转旋转方向
        accumulated_turn_ = 0.0; // 清零累积转角
        last_theta_ = current_pose_.theta; // 更新参考角度为当前角度
        RCLCPP_INFO(this->get_logger(), "画完一圈，反转方向");
    }

    // 发布速度命令
    publisher_->publish(cmd);
}

// 位姿订阅回调函数
void FigureEightController::subscription_callback(const turtlesim::msg::Pose::ConstSharedPtr &msg) {
    // 更新当前位姿
    current_pose_ = *msg;
    // 如果是第一次接收到位姿
    if (!has_pose_) {
        last_theta_ = current_pose_.theta; // 记录初始朝向角
        has_pose_ = true; // 标记已接收到位姿
    }
}

#include <cmath>
#include <memory>
#include <string>
#include "FigureEightController.hpp"

int main(const int argc, char **argv) {
    rclcpp::init(argc, argv); // 初始化ROS2系统
    rclcpp::spin(std::make_shared<FigureEightController>()); // 运行节点（进入事件循环）
    rclcpp::shutdown(); // 关闭ROS2系统
    return 0;
}

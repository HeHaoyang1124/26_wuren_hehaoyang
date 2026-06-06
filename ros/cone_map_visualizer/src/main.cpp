#include "ConeMapVisualizer.hpp"

int main(const int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ConeMapVisualizer>());
    rclcpp::shutdown();
    return 0;
}

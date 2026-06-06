#pragma once

#include <memory>
#include <string>
#include <vector>

#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <tf2_ros/static_transform_broadcaster.h>

#include "fsd_common_msgs/msg/cone.hpp"
#include "fsd_common_msgs/msg/map.hpp"

class ConeMapVisualizer : public rclcpp::Node {
private:
    // ROS 2 话题名称
    std::string map_topic_; // 从 bag 中订阅的话题
    std::string marker_topic_; // 向 rviz2 发布的话题
    std::string fixed_frame_; // 坐标系

    // rviz2 可视化渲染参数
    double cone_marker_size_{}; // 锥桶标记大小
    double cone_alpha_{}; // 锥桶标记的不透明度

    // 通信接口
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr publisher_; // 向rviz2发送可视化标记
    rclcpp::Subscription<fsd_common_msgs::msg::Map>::SharedPtr subscription_; // 从bag中订阅锥桶地图数据
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_broadcaster_; // 用于广播静态坐标变换

public:
    ConeMapVisualizer();

private:
    // 创建颜色消息
    static std_msgs::msg::ColorRGBA makeMsgColor(double r, double g, double b, double a);

    // 清空标记
    static visualization_msgs::msg::Marker deleteAllMarker(const std_msgs::msg::Header &header);

    // 创建单个锥桶标记
    visualization_msgs::msg::Marker makeConeMarker(const std_msgs::msg::Header &header,
                                                   const fsd_common_msgs::msg::Cone &cone,
                                                   const std::string &ns,
                                                   int id,
                                                   const std_msgs::msg::ColorRGBA &color) const;

    // 添加多个锥桶标记
    void appendConeGroup(visualization_msgs::msg::MarkerArray &array,
                         const std_msgs::msg::Header &header,
                         const std::vector<fsd_common_msgs::msg::Cone> &cones,
                         const std::string &ns,
                         const std_msgs::msg::ColorRGBA &color,
                         int id_offset) const;

    // 创建静态坐标变换
    void broadcastWorldFrame();

    // 地图消息处理函数
    void mapCallback(const fsd_common_msgs::msg::Map::SharedPtr &msg) const;
};

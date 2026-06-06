#include "ConeMapVisualizer.hpp"

ConeMapVisualizer::ConeMapVisualizer() : rclcpp::Node("cone_map_visualizer"),
                                         // 从 bag 包订阅回放的 topic
                                         map_topic_(declare_parameter<std::string>(
                                             "map_topic", "/estimation/slam/map")),
                                         // 向 rviz2 发布的 topic
                                         marker_topic_(
                                             declare_parameter<std::string>("marker_topic", "/cone_map_markers")),

                                         // 固定坐标系
                                         fixed_frame_(declare_parameter<std::string>("fixed_frame", "world")),

                                         // 绘制的 cone 的大小和透明度
                                         cone_marker_size_(declare_parameter<double>("cone_marker_size", 0.25)),
                                         cone_alpha_(declare_parameter<double>("cone_alpha", 0.95)) {
    // 发布器
    publisher_ = create_publisher<visualization_msgs::msg::MarkerArray>(marker_topic_, 10);
    // 订阅器
    subscription_ = create_subscription<fsd_common_msgs::msg::Map>(
        map_topic_,
        rclcpp::QoS(rclcpp::KeepLast(10)).reliable().durability_volatile(),
        [this](const fsd_common_msgs::msg::Map::SharedPtr msg) { mapCallback(msg); });

    tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
    broadcastWorldFrame();

    RCLCPP_INFO(get_logger(), "监听 %s 并推送到 %s", map_topic_.c_str(), marker_topic_.c_str());
}

// 创建颜色消息
std_msgs::msg::ColorRGBA
ConeMapVisualizer::makeMsgColor(const double r, const double g, const double b, const double a) {
    std_msgs::msg::ColorRGBA color;
    color.r = static_cast<float>(r);
    color.g = static_cast<float>(g);
    color.b = static_cast<float>(b);
    color.a = static_cast<float>(a);
    return color;
}

// 清空标记
visualization_msgs::msg::Marker ConeMapVisualizer::deleteAllMarker(const std_msgs::msg::Header &header) {
    visualization_msgs::msg::Marker marker;
    marker.header = header;
    marker.ns = "cone_map";
    marker.id = 0;
    marker.action = visualization_msgs::msg::Marker::DELETEALL;
    return marker;
}

// 创建单个锥桶标记
visualization_msgs::msg::Marker ConeMapVisualizer::makeConeMarker(const std_msgs::msg::Header &header,
                                                                  const fsd_common_msgs::msg::Cone &cone,
                                                                  const std::string &ns, const int id,
                                                                  const std_msgs::msg::ColorRGBA &color) const {
    visualization_msgs::msg::Marker marker;
    marker.header = header;
    marker.ns = ns;
    marker.id = id;
    marker.type = visualization_msgs::msg::Marker::SPHERE;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose.position = cone.position;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = cone_marker_size_;
    marker.scale.y = cone_marker_size_;
    marker.scale.z = cone_marker_size_;
    marker.color = color;
    return marker;
}

// 添加多个锥桶标记
void ConeMapVisualizer::appendConeGroup(visualization_msgs::msg::MarkerArray &array,
                                        const std_msgs::msg::Header &header,
                                        const std::vector<fsd_common_msgs::msg::Cone> &cones, const std::string &ns,
                                        const std_msgs::msg::ColorRGBA &color, const int id_offset) const {
    for (std::size_t i = 0; i < cones.size(); i++) {
        array.markers.push_back(makeConeMarker(header, cones[i], ns, id_offset + static_cast<int>(i), color));
    }
}

// 创建静态坐标变换
void ConeMapVisualizer::broadcastWorldFrame() {
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = get_clock()->now();
    transform.header.frame_id = fixed_frame_;
    transform.child_frame_id = fixed_frame_ + "_origin";
    transform.transform.translation.x = 0.0;
    transform.transform.translation.y = 0.0;
    transform.transform.translation.z = 0.0;
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = 0.0;
    transform.transform.rotation.w = 1.0;
    tf_broadcaster_->sendTransform(transform);
}

// 地图消息处理函数
void ConeMapVisualizer::mapCallback(const fsd_common_msgs::msg::Map::SharedPtr &msg) const {
    const std::string frame_id = msg->header.frame_id;

    std_msgs::msg::Header header;
    header.stamp = msg->header.stamp;
    header.frame_id = frame_id;

    visualization_msgs::msg::MarkerArray array;
    array.markers.push_back(deleteAllMarker(header));

    const auto red = makeMsgColor(1.0, 0.15, 0.15, cone_alpha_);
    const auto blue = makeMsgColor(0.15, 0.15, 1.0, cone_alpha_);
    const auto unknown = makeMsgColor(0.7, 0.7, 0.7, cone_alpha_);
    appendConeGroup(array, header, msg->cone_red, "red_cones", red, 1000);
    appendConeGroup(array, header, msg->cone_blue, "blue_cones", blue, 2000);
    appendConeGroup(array, header, msg->cone_unknown, "unknown_cones", unknown, 3000);

    publisher_->publish(array);
}

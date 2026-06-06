cmake_minimum_required(VERSION 3.8)
project(cone_map_visualizer)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(fsd_common_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)

add_executable(cone_map_visualizer
        src/main.cpp
        src/ConeMapVisualizer.cpp)

target_include_directories(cone_map_visualizer PRIVATE include/)
target_compile_features(cone_map_visualizer PUBLIC cxx_std_17)
ament_target_dependencies(cone_map_visualizer
        rclcpp
        tf2_ros
        std_msgs
        geometry_msgs
        fsd_common_msgs
        visualization_msgs
)

install(TARGETS cone_map_visualizer DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME})

if (BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    set(ament_cmake_copyright_FOUND TRUE)
    set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
endif ()

ament_package()


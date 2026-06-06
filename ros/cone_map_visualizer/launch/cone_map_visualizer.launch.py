from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 参数名称 config_file = 'config_file'
    config_file = LaunchConfiguration('config_file')
    # 默认文件路径 default_config_file = 'cone_map_visualizer/config/cone_map_visualizer.yaml'
    default_config_file = PathJoinSubstitution(
        [FindPackageShare('cone_map_visualizer'), 'config', 'cone_map_visualizer.yaml']
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'config_file',
            default_value=default_config_file,
            description='Path to the cone map visualizer parameter file',
        ),
        # 包：package = cone_map_visualizer
        # 节点：executable = cone_map_visualizer
        # 话题：name = cone_map_visualizer
        # 参数：para = config_file
        Node(
            package='cone_map_visualizer',
            executable='cone_map_visualizer',
            name='cone_map_visualizer',
            output='screen',
            parameters=[config_file],
        ),
    ])

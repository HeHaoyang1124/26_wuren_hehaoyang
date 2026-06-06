from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 参数名称 config_file = 'config_file'
    config_file = LaunchConfiguration('config_file')
    # 默认文件路径 default_config_file = 'turtle_controller/config/turtle_controller.yaml'
    default_config_file = PathJoinSubstitution(
        [FindPackageShare('turtle_controller'), 'config', 'turtle_controller.yaml']
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'config_file',
            default_value=default_config_file,
            description='Path to the turtle_controller parameter file',
        ),
        # 包：package = turtlesim
        # 节点：executable = turtlesim_node
        # 话题：name = turtlesim
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim',
            output='screen',
        ),
        # 包：package = turtle_controller
        # 节点：executable = turtle_controller
        # 话题：name = figure_eight_controller
        # 参数：para = config_file
        Node(
            package='turtle_controller',
            executable='turtle_controller',
            name='figure_eight_controller',
            output='screen',
            parameters=[config_file],
        ),
    ])

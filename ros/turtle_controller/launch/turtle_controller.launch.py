from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    config_file = LaunchConfiguration('config_file')
    default_config_file = PathJoinSubstitution(
        [FindPackageShare('turtle_controller'), 'config', 'turtle_controller.yaml']
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'config_file',
            default_value=default_config_file,
            description='Path to the turtle_controller parameter file',
        ),
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim',
            output='screen',
        ),
        Node(
            package='turtle_controller',
            executable='turtle_controller',
            name='figure_eight_controller',
            output='screen',
            parameters=[config_file],
        ),
    ])


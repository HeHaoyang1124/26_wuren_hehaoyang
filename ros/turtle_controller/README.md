# 控制海龟8字绕圈

使用 C++ 编写控制海龟绕圈的程序

## 构建

以我的环境(conda, jazzy, 工作区在 ~/dev/ros2)为例

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
cd ~/dev/ros2
colcon build --packages-select turtle_controller
source install/setup.bash
```

## 运行

以我的环境(conda, jazzy, 工作区在 ~/dev/ros2)为例

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
source /home/he/dev/ros2/install/setup.bash
ros2 launch turtle_controller turtle_controller.launch.py
```

## 停止

```bash
pkill -SIGINT -u "$(whoami)" -f "turtlesim_node|turtle_controller|rviz2"
```

## 参数

参数文件位于 `turtle_controller/config/turtle_controller.yaml` 参数名称：`config_file`，自定义参数文件如下

```bash
ros2 launch turtle_controller turtle_controller.launch.py config_file:=/absolute/path/to/your.yaml
```

# 2026 SCUT Racing招新ROS作业

飞书笔记：https://my.feishu.cn/docx/Gweydaeu5orLPax3yX1c2RdqnMe?from=from_copylink

## 启动命令

使用 C++ 编写控制海龟绕圈的程序

### 构建

以我的环境(conda, jazzy, 工作区在 ~/dev/ros2)为例

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
cd ~/dev/ros2
colcon build --packages-select turtle_controller
source install/setup.bash
```

### 运行

以我的环境(conda, jazzy, 工作区在 ~/dev/ros2)为例

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
source /home/he/dev/ros2/install/setup.bash
ros2 launch turtle_controller turtle_controller.launch.py
```

### 停止

```bash
pkill -SIGINT -u "$(whoami)" -f "turtlesim_node|turtle_controller|rviz2"
```

### 参数

参数名称：`config_file`

```bash
ros2 launch turtle_controller turtle_controller.launch.py config_file:=/absolute/path/to/your.yaml
```

## 解决思路
1. 控制海龟说明，需要订阅海龟的信息、向海龟发布指令
2. 画完一个圆后(累计角度)，改变角速度向另一个方向画圆，模拟8字绕圈

## 节点输出信息
<img src="info.png" alt="">
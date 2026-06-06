# cone_map_visualizer

## 构建

在工作空间根目录执行：

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
cd ~/dev/ros2
colcon build --packages-select fsd_common_msgs cone_map_visualizer
source install/setup.bash
```

## 运行

### 启动可视化节点：

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
ros2 launch cone_map_visualizer cone_map_visualizer.launch.py
```

### 播放给定 bag：

```bash
conda activate ros
source /opt/ros/jazzy/setup.bash
ros2 bag play /home/he/dev/ros2/src/map_to_visualize
```

### 打开 RViz2：

```bash
rviz2
```

在 RViz2 中：

- 将 `Fixed Frame` 设为 `world`
- `MarkerArray` 订阅话题 `/cone_map_markers`

## 参数

参数文件位于 `config/cone_map_visualizer.yaml`，参数名称为 `config_file`，自定义参数文件如下

```bash
ros2 launch cone_map_visualizer cone_map_visualizer.launch.py config_file:=/absolute/path/to/your.yaml
``` 
%% 第二问：轨迹跟踪
clear; clc; close all

% 车辆参数
lfr = 2.168 + 1.907; % 轴距 L
dt = 0.01;
v = 15; 
sim_steps = 2000;

% 参考轨迹 (正弦曲线)
X_ref = 0:0.1:200; 
Y_ref = 10 * sin(X_ref / 15); 

% 初始车辆状态 
X = X_ref(1); Y = Y_ref(1) + 3; phi = 0; 
X_vec = zeros(1, sim_steps); Y_vec = zeros(1, sim_steps);


for ii = 1:sim_steps
    X_vec(ii) = X; Y_vec(ii) = Y;
    
    
    % ===============================================================
    
    % ================= TODO 2.1: 实现某种跟踪算法 =================
    % 1. 在车头正前方前方Ld的距离附近找一个点进行追踪
    Ld = 3.0;
    X_look = X + Ld * cos(phi);
    Y_look = Y + Ld * sin(phi);

    % 计算所有距离的平方，找最近的
    dist_sq = (X_ref - X_look).^2 + (Y_ref - Y_look).^2;
    [~, target_idx] = min(dist_sq);

    % 获取跟踪点坐标
    tx = X_ref(target_idx);
    ty = Y_ref(target_idx);

    % 2. 计算横向误差 e
    dx = tx - X;
    dy = ty - Y;
    e = dy * cos(phi) - dx * sin(phi);

    % 3. 计算转向角
    sigma = atan(2 * lfr * e / (Ld^2));

    % 限制最大转向角 35°
    max_steer = deg2rad(35);
    sigma = min(sigma, max_steer);
    sigma = max(sigma, -max_steer);

    % ===============================================================

    % ================= TODO 2.2: 车辆状态更新 =================
    % 提示: 将刚才求得的转向角 sigma 代入运动学模型（复用第一问代码），更新 X, Y, phi。

    phi_dot = v * tan(sigma) / lfr;
    phi = phi + phi_dot * dt;
    X = X + v * cos(phi) * dt;
    Y = Y + v * sin(phi) * dt;

    % ===============================================================
    
    % 到达终点提前结束
    if X >= X_ref(end), break; end
end

% 绘图对比
figure; hold on; grid on;
plot(X_ref, Y_ref, 'k--', 'LineWidth', 2);
plot(X_vec(1:ii), Y_vec(1:ii), 'r-', 'LineWidth', 2);
legend('参考规划轨迹', '实际行驶轨迹');
title(['Pure Pursuit 跟踪 (Ld = ', num2str(Ld), 'm)']);
xlabel('X [m]'); ylabel('Y [m]'); axis equal;
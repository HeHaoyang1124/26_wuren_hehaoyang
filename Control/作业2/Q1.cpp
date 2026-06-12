#include <iostream>
#include <Eigen/Dense>

constexpr double eta = 0.05;
constexpr double err = 1e-3;
constexpr int max_iter = 10000;

const Eigen::Vector2d S(0.0, 0.0);
const Eigen::Vector2d T(3.0, 3.0);

int main() {
    int iter_count = 0;
    Eigen::Vector2d X = S;

    for (iter_count = 1; iter_count <= max_iter && (T - X).norm() > err; iter_count++) {
        Eigen::Vector2d grad(X.x() - 3.0,
                             10.0 * (X.y() - 3.0));
        X = X - eta * grad;
    }

    std::cout << ((T - X).norm() <= err ? "已收敛" : "未收敛") << std::endl;
    std::cout << "当前位置为: (" << X.x() << ", " << X.y() << ")" << std::endl;
    std::cout << "迭代次数为: " << iter_count << std::endl;

    return 0;
}

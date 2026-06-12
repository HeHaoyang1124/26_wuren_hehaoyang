#include <OsqpEigen/OsqpEigen.h>
#include <Eigen/Sparse>
#include <iostream>

int main() {
    // 初始化求解器
    OsqpEigen::Solver solver;
    solver.settings()->setVerbosity(false);

    // 设置变量数和约束数
    solver.data()->setNumberOfVariables(2);
    solver.data()->setNumberOfConstraints(1);

    // 构建 Hessian 矩阵 P (2x2 稀疏矩阵)
    // P = [[1.0, 0.0],
    //      [0.0, 10.0]]
    Eigen::SparseMatrix<double> P(2, 2);
    P.insert(0, 0) = 1.0;
    P.insert(1, 1) = 10.0;

    // 构建线性项 q
    // q = [-3.0, -30.0]
    Eigen::VectorXd q(2);
    q << -3.0, -30.0;

    // 构建约束矩阵 A (1x2 稀疏矩阵)
    // A = [[1.0, 1.0]]
    Eigen::SparseMatrix<double> A(1, 2);
    A.insert(0, 0) = 1.0;
    A.insert(0, 1) = 1.0;

    // 构建约束下界 l = [-inf]
    Eigen::VectorXd l(1);
    l << -OsqpEigen::INFTY;

    // 构建约束上界 u = [4.0]
    Eigen::VectorXd u(1);
    u << 4.0;

    // 设置问题数据
    solver.data()->setHessianMatrix(P);
    solver.data()->setGradient(q);
    solver.data()->setLinearConstraintsMatrix(A);
    solver.data()->setLowerBound(l);
    solver.data()->setUpperBound(u);

    // 初始化求解器
    if (!solver.initSolver()) {
        std::cerr << "求解器初始化失败！" << std::endl;
        return -1;
    }

    // 求解
    if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) {
        std::cerr << "求解失败！" << std::endl;
        return -1;
    }

    // 获取结果
    Eigen::VectorXd result = solver.getSolution();

    // 输出结果
    std::cout << "终点坐标: [" << result(0) << ", " << result(1) << "]" << std::endl;

    return 0;
}
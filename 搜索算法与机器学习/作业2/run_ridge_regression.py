"""线性回归+L2正则化"""
from sklearn.datasets import fetch_california_housing
from sklearn.model_selection import train_test_split

from linear_regression_manual import LinearRegressionManual, z_score, apply_standardization, evaluate_model


def main() -> None:
    # 1. 加载加州房价数据集
    dataset = fetch_california_housing()

    # 2. 划分训练集和测试集（80%训练，20%测试）
    x_train, x_test, y_train, y_test = train_test_split(
        dataset.data,
        dataset.target,
        test_size=0.2,
        random_state=42
    )

    # 3. Z-score标准化
    x_train_std, mean, std = z_score(x_train)
    x_test_std = apply_standardization(x_test, mean, std)

    # 4. 训练线性回归模型（L2正则化，λ=100）
    l2_lambda = 100.0
    model = LinearRegressionManual(l2_lambda=l2_lambda)
    model.fit(x_train_std, y_train)
    y_pred = model.predict(x_test_std)

    # 5. 评估模型
    print(f"正则化参数 λ={l2_lambda}")
    evaluate_model(y_test, y_pred)

    # 6. 展示前10个样本的预测结果
    print("\n前10个样本预测结果:")
    for i, (true_y, pred_y) in enumerate(zip(y_test[:10], y_pred[:10]), 1):
        print(f"  {i}. 真实值={true_y:.3f}, 预测值={pred_y:.3f}, 误差={abs(true_y - pred_y):.3f}")


if __name__ == "__main__":
    main()

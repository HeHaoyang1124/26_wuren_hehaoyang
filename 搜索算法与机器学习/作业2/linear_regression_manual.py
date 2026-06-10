import numpy as np
from sklearn.metrics import root_mean_squared_error, r2_score


class LinearRegressionManual:
    """多元线性回归模型，支持可选的L2正则化
    
    使用正规方程求解: w = (X^T X + λI)^{-1} X^T y
    """

    def __init__(self, l2_lambda: float = 0.0):
        """初始化模型
        
        参数:
            l2_lambda: L2正则化系数，默认为0表示无正则化
        """
        if l2_lambda < 0:
            raise ValueError("l2_lambda必须为非负数")
        self.l2_lambda = float(l2_lambda)
        self.w = None  # 特征权重
        self.b = None  # 偏置项

    def fit(self, x: np.ndarray, y: np.ndarray) -> "LinearRegressionManual":
        """训练模型，通过正规方程求解最优参数
        
        参数:
            x: 训练集特征矩阵 (n_samples, n_features)
            y: 训练集目标向量 (n_samples,)
        返回:
            训练好的模型对象
        """
        # 输入验证
        if x.ndim != 2:
            raise ValueError(f"特征矩阵 x 必须是二维的")
        if y.ndim != 1:
            raise ValueError(f"目标向量 y 必须是一维的")
        if x.shape[0] != y.shape[0]:
            raise ValueError(f"样本数量不匹配")

        # 添加偏置列（全1）到原始特征矩阵的左侧
        x_bias = np.hstack([np.ones((x.shape[0], 1)), x])
        # 获取含偏置项的特征数量（也就是列数）
        n_features = x_bias.shape[1]

        # 构建L2正则化矩阵（偏置项不参与正则化）
        reg_matrix = np.eye(n_features)
        reg_matrix[0, 0] = 0.0

        # 计算正规方程: (X^T X + λI)w = X^T y
        xtx = x_bias.T @ x_bias
        xty = x_bias.T @ y

        if self.l2_lambda > 0:
            xtx += self.l2_lambda * reg_matrix

        # 求解特征权重（含偏置项），并保存学习参数
        weights = np.linalg.solve(xtx, xty)
        self.b = float(weights[0])
        self.w = weights[1:]
        return self

    def predict(self, x: np.ndarray) -> np.ndarray:
        """预测

        参数:
            x: 测试集特征矩阵 (n_samples, n_features)
        返回:
            预测结果数组 (n_samples,)
        """
        if self.w is None or self.b is None:
            raise RuntimeError("模型未训练，请先调用fit()")
        return x @ self.w + self.b


def z_score(x: np.ndarray) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Z-score标准化：将数据转换为均值0、标准差1的分布
    
    参数:
        x: 特征矩阵 (n_samples, n_features)
    返回:
        (标准化后的数据, 均值向量, 标准差向量)
    """
    mean = x.mean(axis=0)
    std = x.std(axis=0)
    std[std == 0] = 1.0  # 避免除以0
    return (x - mean) / std, mean, std


def apply_standardization(x: np.ndarray, mean: np.ndarray, std: np.ndarray) -> np.ndarray:
    """使用训练集的统计参数标准化新数据
    
    参数:
        x: 待标准化数据
        mean: 训练集均值
        std: 训练集标准差
    返回:
        标准化后的数据
    """
    std_safe = np.where(std == 0, 1.0, std)
    return (x - mean) / std_safe


def evaluate_model(y_true: np.ndarray, y_pred: np.ndarray) -> None:
    """评估模型性能，计算并打印RMSE和R²指标

    参数:
        y_true: 真实目标值数组
        y_pred: 模型预测值数组
    """
    rmse = root_mean_squared_error(y_true, y_pred)
    r2 = r2_score(y_true, y_pred)
    print(f"RMSE: {rmse:.4f}, R²: {r2:.4f}")

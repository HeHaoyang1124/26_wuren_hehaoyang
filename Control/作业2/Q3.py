import numpy as np
import osqp
from scipy import sparse

P = sparse.csc_matrix(np.array([[1.0, 0.0],
                                [0.0, 10.0]]))
q = np.array([-3.0, -30.0])

A = sparse.csc_matrix(np.array([[1.0, 1.0]]))
l = np.array([-np.inf])
u = np.array([4.0])

prob = osqp.OSQP()
prob.setup(P=P, q=q, A=A, l=l, u=u, verbose=False)

res = prob.solve()

print("终点坐标", res.x)
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 17 17:17:11 2019
3D机械臂展示
@author: GXU
"""

import matplotlib.pyplot as plt
import matplotlib.colors
import numpy as np
from mpl_toolkits.mplot3d import Axes3D


def midpoints(x):
   sl = ()
   for i in range(x.ndim):
       x = (x[sl + np.index_exp[:-1]] + x[sl + np.index_exp[1:]]) / 2.0
       sl += np.index_exp[:]
   return x

# prepare some coordinates, and attach rgb values to each
r, theta, z = np.mgrid[0:1:11j, 0:np.pi*2:25j, -0.5:0.5:11j]
x = r*np.cos(theta)
y = r*np.sin(theta)

rc, thetac, zc = midpoints(r), midpoints(theta), midpoints(z)

# define a wobbly torus about [0.7, *, 0]
sphere = (rc - 0.7)**2 + (zc + 0.2*np.cos(thetac*2))**2 < 0.2**2

# combine the color components
hsv = np.zeros(sphere.shape + (3,))
hsv[..., 0] = thetac / (np.pi*2)
hsv[..., 1] = rc
hsv[..., 2] = zc + 0.5
colors = matplotlib.colors.hsv_to_rgb(hsv)

# and plot everything
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.voxels(x, y, z, sphere,
         facecolors=colors,
         edgecolors=np.clip(2*colors - 0.5, 0, 1),  # brighter
         linewidth=0.5)

plt.show()





'''
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
# 三维，两个特征
fig = plt.figure(figsize=(8, 6)) #设置图标的大小
ax = fig.add_subplot(111, projection='3d') # 111的意思是把画布分为1行1列，画在第一个方格内。其实就是整个画布。

# 创建样本，注意两个特征不能线性相关，否则无法用最小二乘解参数
X1 = np.arange(-4, 4, 0.1)
m = len(X1)
X2 = np.random.rand(m)*5
# print(X2)
# print(X1)

# 堆叠全1数组和X1以及X2形成样本的矩阵，倒置，用以矩阵乘法
X = np.vstack((np.full(m, 1), X1, X2)).T

# y = 15*X1 + 3 * X2 + theta0
# 自定义样本输出
Y = X1 + 3 * X2 + 3*np.random.randn(m)

# 利用标准方程(最小二乘法求解theta)
theta = np.dot(np.dot(np.linalg.inv(np.dot(np.transpose(X), X)),
np.transpose(X)), Y)
print(theta)

# 构造网格 meshgrid函数可以通过延伸矩阵构造多维坐标中的网格坐标。
M, N = np.meshgrid(X1, X2)

# zip函数构建一个多元祖[(x1,y1),(x2,y2)...],ravel函数将一个多维数组合并成一维数组
Z = np.array([theta[1] * d + theta[2]*p + theta[0] for d, p in zip(np.ravel(M), np.ravel(N))]).reshape(M.shape)

# 根据网格和函数构建图形 suface是一个完整的面
ax.plot_surface(M, N, Z)
# scatter是散点图
ax.scatter(X1, X2, Y, c='r')
# 设置坐标轴的名称
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()
'''


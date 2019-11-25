# learn python in action

[python 代码调试软件](https://github.com/inducer/pudb)

[Python各类图像库的图片读写方式总结](https://www.cnblogs.com/skyfsm/p/8276501.html)

[Python学习手册 笔记](https://github.com/Ewenwan/python_learning_notes)

[python_tutorials 教程 较全 待整合](https://github.com/Ewenwan/python_tutorials)

[python实现的全部算法](https://github.com/Ewenwan/Python)

[教程](https://github.com/googleliyang/python)

[以撸代码的形式学习Python](https://github.com/Ewenwan/LearnPython)

[鱼C 零基础入门学习Python](http://blog.fishc.com/category/python/page/10)

[鱼C  《极客首选之Python》 ](http://blog.fishc.com/category/geekpython/page/3)

[Python抓取股票信息](http://jinjaysnow.github.io/blog/2014-10/Python%E6%8A%93%E5%8F%96%E8%82%A1%E7%A5%A8%E4%BF%A1%E6%81%AF.html)

[关于Python的面试题  Python语言特性 操作系统 数据库 网络 编程题 红黑树](https://github.com/Ewenwan/interview_python)

[HackerRank刷题之路---Python](http://www.cnblogs.com/dn-harris/p/6048255.html?utm_source=itdadao&utm_medium=referral)

[python 学习线性代数](https://github.com/Ewenwan/Play-with-Linear-Algebra)

[SWIG实现python对c++封装](http://www.cppblog.com/wanghaiguang/archive/2016/08/18/214201.html)

[TensorFlow技术内幕（四）：TF中的混合编程](https://blog.csdn.net/gaofeipaopaotang/article/details/80540721)

[Python在教育方面的资源列表](https://github.com/Ewenwan/awesome-python-in-education-zh)

[python 面向对象编程](https://github.com/PacktPublishing/Python-Object-Oriented-Programming-Cookbook)

[python cuda gpu编程](https://github.com/PacktPublishing/Hands-On-GPU-Programming-with-Python-and-CUDA)

[Python多线程编程 ](https://www.ctolib.com/docs-Python-Multithreading-c-index.html)

[python 马尔科夫模型](https://github.com/PacktPublishing/Hands-On-Markov-Models-with-Python)

[59个编写高质量Python代码](https://github.com/Ewenwan/effectivepython)

[集体智慧编程 python实现 收集并分析用户的数据和反馈信息，以便创造新的用户价值和商业价值](https://github.com/Ewenwan/Programming-Collective-Intelligence-Source-Code)

[商业爬虫项目](https://github.com/VcrTing/Scrapy_BackUp)

[清华大学 anaconda 软件包 site-page ](https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/win-64)



# 书籍
[Python Pocket Reference ](https://github.com/Ewenwan/python-pocket-reference) 
	
	内容完整精要，覆盖Python 2和Python 3。内容也与时俱进，提纲挈领的组织。是手册，也是检验对Python的体系化理解的好方法。
	
[Python Cookbook 3rd 中文版](https://github.com/Ewenwan/python3-cookbook)	
	
	Python进阶读物，包含各种常用场景下的Python代码，使得读者可以写出更加Pythonic的代码。

[《Python Parallel Programming Cookbook》中文版](https://github.com/Ewenwan/python-parallel-programming-cookbook-cn)
	
	结合Python讨论了线程、进程和异步编程三种模型，是Python并行编程不错的参考书籍。

[Effective-Python](https://github.com/Ewenwan/Effective-Python)
	
	59个编写高质量Python代码



# window python 开发环境
## 1. anaconda
[下载](https://www.anaconda.com/download/)

## 2. pycharm
[下载](https://www.jetbrains.com/pycharm/)

## 3. spider 

# Python Cookbook（Python 菜谱）
ActiveState 上有很多用户提供的 Python 代码，这些代码能够完成固定的任务；

O’Reilly 根据这些资料出版过一本《Python Cookbook》，但是这本书上并没有列出所有有用的代码。

因此，这里为用户收集一些有用的功能的 Python 代码，为用户提供方便。activestate 原地址为： http://code.activestate.com/recipes/langs/python/。

系列文章：

菜谱 1：创建一个唯一的 SESSION ID http://www.pythonpub.com/article/1502

菜谱 2：发送普通文本邮件 http://www.pythonpub.com/article/1503

菜谱 3：发送 HTML 形式的邮件 http://www.pythonpub.com/article/1504

菜谱 4：发送带图片的邮件 http://www.pythonpub.com/article/1505

菜谱 5：发送带附件的邮件 http://www.pythonpub.com/article/1506

菜谱 6：群发邮件 http://www.pythonpub.com/article/1507

菜谱 7：发送混合邮件 http://www.pythonpub.com/article/1508

菜谱 8：支持简单命令行 http://www.pythonpub.com/article/1509

菜谱 9：SOUNDEX 算法 http://www.pythonpub.com/article/1510

菜谱 10：统计单词出现的频率 http://www.pythonpub.com/article/1511

菜谱 11：使用列表实现循环数据结构 http://www.pythonpub.com/article/1512

菜谱 12：使用 UDP 数据包发送消息 http://www.pythonpub.com/article/1513

菜谱 13：在文件中搜索以及替换文本 http://www.pythonpub.com/article/1514

菜谱 14：从.ZIP文件中读取数据 http://www.pythonpub.com/article/1515

# python学习线性代数
## 1. 自定义向量Vector类

Vector.py
```python
class Vector:
    # 双下划线开头结尾的函数，定义为类的内置函数
    
    # 类构造函数
    def __init__(self, lst):
        self._values = lst
	
    # [] 下标方式会调用的函数
    def __getitem__(self, index):
        """取向量的第index个元素"""
        return self._values[index]
	
    # len(类对象) 会执行的函数
    def __len__(self):
        """返回向量长度（有多少个元素）"""
        return len(self._values)
	
    #
    def __repr__(self):
        return "Vector({})".format(self._values)
	
    # 展开成字符串
    def __str__(self):
        return "({})".format(", ".join(str(e) for e in self._values))

```

main_vector.py
```python
import Vector

if __name__ == "__main__":

    vec = Vector([5, 2]) # 构造类对象
    print(vec)           # 展开成字符串后打印??
    print("len(vec) = {}".format(len(vec))) # 向量长度
    print("vec[0] = {}, vec[1] = {}".format(vec[0], vec[1]))#获取每一个对象

```


增加向量函数操作，加减乘等运算
```python
class Vector:
    # 构造函数
    def __init__(self, lst):
        self._values = list(lst)

    def __add__(self, another):
        """向量加法，返回结果向量"""
	
	# 确保 两向量长度相等
        assert len(self) == len(another), \
            "Error in adding. Length of vectors must be same."

        # return Vector([a + b for a, b in zip(self._values, another._values)])
        return Vector([a + b for a, b in zip(self, another)]) # 注意获取 两个对象内的元素的形式!!!!!!!

    def __sub__(self, another):
        """向量减法，返回结果向量"""
        assert len(self) == len(another), \
            "Error in subtracting. Length of vectors must be same."

        return Vector([a - b for a, b in zip(self, another)])

    def __mul__(self, k):
        """返回 数量乘法 的结果向量：self * k"""
        return Vector([k * e for e in self])

    def __rmul__(self, k):
        """返回 数量乘法 的结果向量：k * self"""
        return self * k

    def __pos__(self):
        """返回 向量取正 的结果向量"""
        return 1 * self

    def __neg__(self):
        """返回 向量取负 的结果向量"""
        return -1 * self

    def __iter__(self):
        """返回向量的 迭代器"""
        return self._values.__iter__()

    def __getitem__(self, index):
        """取向量的第index个元素"""
        return self._values[index]

    def __len__(self):
        """返回向量长度（有多少个元素）"""
        return len(self._values)

    def __repr__(self):
        return "Vector({})".format(self._values)

    def __str__(self):
        return "({})".format(", ".join(str(e) for e in self._values))
	
    # classmethod 装饰器，不需要实例化类对象，即可实现该函数
    @classmethod
    def zero(cls, dim):
        """返回一个dim维的零向量"""
        return cls([0] * dim)


```

向量运算测试
```python
from playLA.Vector import Vector

if __name__ == "__main__":

    vec = Vector([5, 2]) # 构造类对象
    print(vec)
    print("len(vec) = {}".format(len(vec)))
    print("vec[0] = {}, vec[1] = {}".format(vec[0], vec[1]))

    vec2 = Vector([3, 1])
    print("{} + {} = {}".format(vec, vec2, vec + vec2))
    print("{} - {} = {}".format(vec, vec2, vec - vec2))

    print("{} * {} = {}".format(vec, 3, vec * 3))# 向量数乘
    print("{} * {} = {}".format(3, vec, 3 * vec))

    print("+{} = {}".format(vec, +vec))# 向量取正
    print("-{} = {}".format(vec, -vec))# 向量取负
    
    # 定义0向量
    zero2 = Vector.zero(2)
    print(zero2)
    print("{} + {} = {}".format(vec, zero2, vec + zero2))
```

向量归一化
```python
class Vector:
    # ...其他函数省略
    
    EPSILON = 1e-8
    
    # 默认向量 数量除法
    def __truediv__(self, k):
        """返回数量除法的结果向量：self / k"""
        return (1 / k) * self

    def norm(self):
        """返回向量的模"""
	# sqrt(x1^2 + ... + xn^2)
	# ** 为乘方标号
        return math.sqrt(sum(e**2 for e in self))

    def normalize(self):
        """返回向量的单位向量"""
        if self.norm() < EPSILON:
            raise ZeroDivisionError("Normalize error! norm is zero.")
        return Vector(self._values) / self.norm()
        # return 1 / self.norm() * Vector(self._values)
        # return Vector([e / self.norm() for e in self])
# 测试
vec = Vector([5, 2])
vec2 = Vector([3, 1]) 
zero2 = Vector.zero(2)
print("norm({}) = {}".format(vec, vec.norm()))
print("norm({}) = {}".format(vec2, vec2.norm()))
print("norm({}) = {}".format(zero2, zero2.norm()))

print("normalize {} is {}".format(vec, vec.normalize()))
print(vec.normalize().norm())

print("normalize {} is {}".format(vec2, vec2.normalize()))
print(vec2.normalize().norm())

# 会发生错误
try:
    zero2.normalize()
except ZeroDivisionError:
    print("Cannot normalize zero vector {}.".format(zero2))
```


向量点乘
```python
class Vector:
    # ...其他函数省略
    
    def dot(self, another):
        """向量点乘，返回结果标量"""
        assert len(self) == len(another), \
            "Error in dot product. Length of vectors must be same."
        # 对应位置元素相乘后，积相加
        return sum(a * b for a, b in zip(self, another))
	
# 测试
vec = Vector([5, 2])
vec2 = Vector([3, 1]) 	
print(vec.dot(vec2))


```

numpy 中的向量
```python
import numpy as np

if __name__ == "__main__":

    print(np.__version__)

    # np.array 基础
    lst = [1, 2, 3]
    lst[0] = "Linear Algebra"
    print(lst)

    vec = np.array([1, 2, 3])
    print(vec)
    # vec[0] = "Linear Algebra"
    # vec[0] = 666
    # print(vec)

    # np.array的创建
    print(np.zeros(5))
    print(np.ones(5))
    print(np.full(5, 666))

    # np.array的基本属性
    print(vec)
    print("size =", vec.size)
    print("size =", len(vec))
    print(vec[0])
    print(vec[-1])
    print(vec[0: 2])
    print(type(vec[0: 2]))

    # np.array的基本运算
    vec2 = np.array([4, 5, 6])
    print("{} + {} = {}".format(vec, vec2, vec + vec2))
    print("{} - {} = {}".format(vec, vec2, vec - vec2))
    print("{} * {} = {}".format(2, vec, 2 * vec))
    print("{} * {} = {}".format(vec, vec2, vec * vec2))
    print("{}.dot({}) = {}".format(vec, vec2, vec.dot(vec2)))

    print(np.linalg.norm(vec))
    print(vec / np.linalg.norm(vec))
    print(np.linalg.norm(vec / np.linalg.norm(vec)))

    # zero3 = np.zeros(3)
    # print(zero3 / np.linalg.norm(zero3))


```
## 2. 自定义 矩阵 Matrix 类
```python
import Vector

class Matrix:

    def __init__(self, list2d):
        # 每个元素是一个行向量
        self._values = [row[:] for row in list2d]

    def row_vector(self, index):
        """返回矩阵的第index个行向量"""
        return Vector(self._values[index])

    def col_vector(self, index):
        """返回矩阵的第index个列向量"""
        return Vector([row[index] for row in self._values])

    def __getitem__(self, pos):
        """返回矩阵pos位置的元素"""
        r, c = pos
        return self._values[r][c]

    def size(self):
        """返回矩阵的元素个数"""
        r, c = self.shape()
        return r * c

    def row_num(self):
        """返回矩阵的行数"""
        return self.shape()[0]

    __len__ = row_num

    def col_num(self):
        """返回矩阵的列数"""
        return self.shape()[1]

    def shape(self):
        """返回矩阵的形状: (行数， 列数)"""
        return len(self._values), len(self._values[0])

    def __repr__(self):
        return "Matrix({})".format(self._values)

    __str__ = __repr__

# 测试
matrix = Matrix([[1, 2], [3, 4]])
print(matrix)
print("matrix.shape = {}".format(matrix.shape()))
print("matrix.size = {}".format(matrix.size()))
print("len(matrix) = {}".format(len(matrix)))
print("matrix[0][0] = {}".format(matrix[0, 0]))

```

实现矩阵的基本操作
```python
import Vector

class Matrix:
    # 其他函数省略

    @classmethod
    def zero(cls, r, c):
        """返回一个r行c列的零矩阵"""
        return cls([[0] * c for _ in range(r)])

    @classmethod
    def identity(cls, n):
        """返回一个n行n列的单位矩阵"""
	
	# 其他部分为0
        m = [[0]*n for _ in range(n)]
	
	# 对角线上为1
        for i in range(n):
            m[i][i] = 1;
        return cls(m)

    def __add__(self, another):
        """返回两个矩阵的加法结果"""
        assert self.shape() == another.shape(), \
            "Error in adding. Shape of matrix must be same."
	# 二维矩阵，对应位置元素相加
        return Matrix([[a + b for a, b in zip(self.row_vector(i), another.row_vector(i))]
                       for i in range(self.row_num())])

    def __sub__(self, another):
        """返回两个矩阵的减法结果"""
        assert self.shape() == another.shape(), \
            "Error in subtracting. Shape of matrix must be same."
	# 二维矩阵，对应位置元素相减
        return Matrix([[a - b for a, b in zip(self.row_vector(i), another.row_vector(i))]
                       for i in range(self.row_num())])
		       
    # 矩阵数乘
    def __mul__(self, k):
        """返回矩阵的数量乘结果: self * k"""
        return Matrix([[e * k for e in self.row_vector(i)]
                       for i in range(self.row_num())])

    def __rmul__(self, k):
        """返回矩阵的数量乘结果: k * self"""
        return self * k

    def __truediv__(self, k):
        """返回数量除法的结果矩阵：self / k"""
        return (1 / k) * self

    def __pos__(self):
        """返回矩阵取正的结果"""
        return 1 * self

    def __neg__(self):
        """返回矩阵取负的结果"""
        return -1 * self


# 测试
matrix = Matrix([[1, 2], [3, 4]])
matrix2 = Matrix([[5, 6], [7, 8]])
print(matrix2)
print("add: {}".format(matrix + matrix2))
print("subtract: {}".format(matrix - matrix2))
print("scalar-mul: {}".format(2 * matrix))
print("scalar-mul: {}".format(matrix * 2))
print("zero_2_3: {}".format(Matrix.zero(2, 3)))

# 单位阵
I = Matrix.identity(2)
print(I)
print("A.dot(I) = {}".format(matrix.dot(I)))
print("I.dot(A) = {}".format(I.dot(matrix)))

```

矩阵和矩阵的乘法 以及 矩阵转置
```python
import Vector

class Matrix:
    # 其他函数省略
    
    def dot(self, another):
        """返回矩阵乘法的结果"""
        if isinstance(another, Vector):
            # 矩阵 和 向量 的乘法
	    # 矩阵 列数量 == 向量长度
            assert self.col_num() == len(another), \
                "Error in Matrix-Vector Multiplication."
	    # 矩阵每一行为向量，变成 向量*向量
            return Vector([self.row_vector(i).dot(another) for i in range(self.row_num())])

        if isinstance(another, Matrix):
            # 矩阵1 和 矩阵2 的乘法
	    # 矩阵1列数 == 矩阵2行数
            assert self.col_num() == another.row_num(), \
                "Error in Matrix-Matrix Multiplication."
	    # 矩阵1 每一行 分别*  矩阵2 每一列
	    # 矩阵2 每一列 for j in range(another.col_num())
	    # 矩阵1 每一行 for i in range(self.row_num())
            return Matrix([[self.row_vector(i).dot(another.col_vector(j)) for j in range(another.col_num())]
                           for i in range(self.row_num())])
			   
    def T(self):
        """返回矩阵的转置矩阵"""
        return Matrix([[e for e in self.col_vector(i)]
                       for i in range(self.col_num())])

# 测试
T = Matrix([[1.5, 0], [0, 2]])
p = Vector([5, 3])
print("T.dot(p) = {}".format(T.dot(p)))

P = Matrix([[0, 4, 5], [0, 0, 3]])
print("T.dot(P) = {}".format(T.dot(P)))

print("A.dot(B) = {}".format(matrix.dot(matrix2)))
print("B.dot(A) = {}".format(matrix2.dot(matrix)))

# 转置
print("P.T = {}".format(P.T()))
```

numpy矩阵
```python
import numpy as np

if __name__ == "__main__":

    # 矩阵的创建
    A = np.array([[1, 2], [3, 4]])
    print(A)

    # 矩阵的属性
    print(A.shape)
    print(A.T)

    # 获取矩阵的元素
    print(A[1, 1])
    print(A[0])
    print(A[:, 0])
    print(A[1, :])

    # 矩阵的基本运算
    B = np.array([[5, 6], [7, 8]])
    print(A + B)
    print(A - B)
    print(10 * A)
    print(A * 10)
    print(A * B)
    print(A.dot(B))

    p = np.array([10, 100])
    print(A + p)
    print(A + 1)
    
    # 点乘
    print(A.dot(p))
    
    # 单位矩阵
    I = np.identity(2)
    print(I)
    print(A.dot(I))
    print(I.dot(A))

    # 逆矩阵
    invA = np.linalg.inv(A)
    print(invA)
    print(invA.dot(A))
    print(A.dot(invA))

    # C = np.array([[1, 2, 3], [4, 5, 6]])
    # np.linalg.inv(C)
```

矩阵 平移 和旋转
```python
import matplotlib.pyplot as plt
import Matrix
import Vector
import math


if __name__ == "__main__":

    points = [[0, 0], [0, 5], [3, 5], [3, 4], [1, 4],
              [1, 3], [2, 3], [2, 2], [1, 2], [1, 0]]
    x = [point[0] for point in points]
    y = [point[1] for point in points]

    plt.figure(figsize=(5, 5))
    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.plot(x, y)
    # plt.show()

    P = Matrix(points)
    
    # 矩阵表示的是一个 空间变换
    # https://github.com/Ewenwan/Mathematics#矩阵线性变换-1
    # T = Matrix([[2, 0], [0, 1.5]])
    # T = Matrix([[1, 0], [0, -1]])
    # T = Matrix([[-1, 0], [0, 1]])
    # T = Matrix([[-1, 0], [0, -1]])
    # T = Matrix([[1, 0.5], [0, 1]])
    # T = Matrix([[1, 0], [0.5, 1]])
    
    # 纯旋转
    theta = math.pi / 3
    T = Matrix([[math.cos(theta), math.sin(theta)], [-math.sin(theta), math.cos(theta)]])

    P2 = T.dot(P.T())
    plt.plot([P2.col_vector(i)[0] for i in range(P2.col_num())],
             [P2.col_vector(i)[1] for i in range(P2.col_num())])
    plt.show()

```

## 3. 线性系统 LinearSystem 
```python
import Matrix
import Vector

# A*x = b
class LinearSystem:

    def __init__(self, A, b):

        assert A.row_num() == len(b), "row number of A must be equal to the length of b"
        self._m = A.row_num()
        self._n = A.col_num()
        assert self._m == self._n  # TODO: no this restriction

        self.Ab = [Vector(A.row_vector(i).underlying_list() + [b[i]])
                   for i in range(self._m)]

    def _max_row(self, index, n):

        best, ret = self.Ab[index][index], index
        for i in range(index + 1, n):
            if self.Ab[i][index] > best:
                best, ret = self.Ab[i][index], i
        return ret

    def _forward(self):

        n = self._m
        for i in range(n):
            # Ab[i][i]为主元
            max_row = self._max_row(i, n)
            self.Ab[i], self.Ab[max_row] = self.Ab[max_row], self.Ab[i]

            # 将主元归为一
            self.Ab[i] = self.Ab[i] / self.Ab[i][i]  # TODO: self.Ab[i][i] == 0?
            for j in range(i + 1, n):
                self.Ab[j] = self.Ab[j] - self.Ab[j][i] * self.Ab[i]

    def _backward(self):

        n = self._m
        for i in range(n - 1, -1, -1):
            # Ab[i][i]为主元
            for j in range(i - 1, -1, -1):
                self.Ab[j] = self.Ab[j] - self.Ab[j][i] * self.Ab[i]

    def gauss_jordan_elimination(self):

        self._forward()
        self._backward()

    def fancy_print(self):

        for i in range(self._m):
            print(" ".join(str(self.Ab[i][j]) for j in range(self._n)), end=" ")
            print("|", self.Ab[i][-1])


```





## Python 练习册，每天一个小程序 ##


#### 说明： ####

- Python 练习册，每天一个小程序。注：将 Python 换成其他语言，大多数题目也适用
- 不会出现诸如「打印九九乘法表」、「打印水仙花」之类的题目
- [点此链接，会看到部分题目的代码，仅供参考](https://github.com/Show-Me-the-Code/python)
- 本文本文由@史江歌（shijiangge@gmail.com  QQ:499065469）根据互联网资料收集整理而成，感谢互联网，感谢各位的分享。鸣谢！本文会不断更新。

> Talk is cheap. Show me the code.--Linus Torvalds

----------
 
**第 0000 题：**将你的 QQ 头像（或者微博头像）右上角加上红色的数字，类似于微信未读信息数量那种提示效果。
类似于图中效果

![头像](http://i.imgur.com/sg2dkuY.png?1)

**第 0001 题：**做为 Apple Store App 独立开发者，你要搞限时促销，为你的应用**生成激活码**（或者优惠券），使用 Python 如何生成 200 个激活码（或者优惠券）？

**第 0002 题**：将 0001 题生成的 200 个激活码（或者优惠券）保存到 **MySQL** 关系型数据库中。 

**第 0003 题：**将 0001 题生成的 200 个激活码（或者优惠券）保存到 **Redis** 非关系型数据库中。 

**第 0004 题：**任一个英文的纯文本文件，统计其中的单词出现的个数。

**第 0005 题：**你有一个目录，装了很多照片，把它们的尺寸变成都不大于 iPhone5 分辨率的大小。

**第 0006 题：**你有一个目录，放了你一个月的日记，都是 txt，为了避免分词的问题，假设内容都是英文，请统计出你认为每篇日记最重要的词。

**第 0007 题：**有个目录，里面是你自己写过的程序，统计一下你写过多少行代码。包括空行和注释，但是要分别列出来。

**第 0008 题：**一个HTML文件，找出里面的**正文**。

**第 0009 题：**一个HTML文件，找出里面的**链接**。

**第 0010 题：**使用 Python 生成类似于下图中的**字母验证码图片**

![字母验证码](http://i.imgur.com/aVhbegV.jpg)

- [阅读资料](http://stackoverflow.com/questions/2823316/generate-a-random-letter-in-python) 

**第 0011 题：** 敏感词文本文件 filtered_words.txt，里面的内容为以下内容，当用户输入敏感词语时，则打印出 Freedom，否则打印出 Human Rights。

    北京
    程序员
    公务员
    领导
    牛比
    牛逼
    你娘
    你妈
    love
    sex
	jiangge
	
**第 0012 题：** 敏感词文本文件 filtered_words.txt，里面的内容 和 0011题一样，当用户输入敏感词语，则用 星号 * 替换，例如当用户输入「北京是个好城市」，则变成「**是个好城市」。

**第 0013 题：** 用 Python 写一个爬图片的程序，爬 [这个链接里的日本妹子图片 :-)](http://tieba.baidu.com/p/2166231880)

- [参考代码](http://www.v2ex.com/t/61686 "参考代码")

**第 0014 题：** 纯文本文件 student.txt为学生信息, 里面的内容（包括花括号）如下所示：

    {
    	"1":["张三",150,120,100],
    	"2":["李四",90,99,95],
    	"3":["王五",60,66,68]
    }

请将上述内容写到 student.xls 文件中，如下图所示：

![student.xls](http://i.imgur.com/nPDlpme.jpg)

- [阅读资料](http://www.cnblogs.com/skynet/archive/2013/05/06/3063245.html) 腾讯游戏开发 XML 和 Excel 内容相互转换

**第 0015 题：** 纯文本文件 city.txt为城市信息, 里面的内容（包括花括号）如下所示：

    {
        "1" : "上海",
        "2" : "北京",
        "3" : "成都"
    }

请将上述内容写到 city.xls 文件中，如下图所示：

![city.xls](http://i.imgur.com/rOHbUzg.png)


**第 0016 题：** 纯文本文件 numbers.txt, 里面的内容（包括方括号）如下所示：

    [
    	[1, 82, 65535], 
    	[20, 90, 13],
    	[26, 809, 1024]
    ]

请将上述内容写到 numbers.xls 文件中，如下图所示：

![numbers.xls](http://i.imgur.com/iuz0Pbv.png)

**第 0017 题：** 将 第 0014 题中的 student.xls 文件中的内容写到 student.xml 文件中，如

下所示：

    <?xml version="1.0" encoding="UTF-8"?>
    <root>
    <students>
    <!-- 
    	学生信息表
    	"id" : [名字, 数学, 语文, 英文]
    -->
    {
    	"1" : ["张三", 150, 120, 100],
    	"2" : ["李四", 90, 99, 95],
    	"3" : ["王五", 60, 66, 68]
    }
    </students>
    </root>


**第 0018 题：** 将 第 0015 题中的 city.xls 文件中的内容写到 city.xml 文件中，如下所示：

    <?xmlversion="1.0" encoding="UTF-8"?>
    <root>
    <citys>
    <!-- 
    	城市信息
    -->
    {
    	"1" : "上海",
    	"2" : "北京",
    	"3" : "成都"
    }
    </citys>
    </root>

**第 0019 题：** 将 第 0016 题中的 numbers.xls 文件中的内容写到 numbers.xml 文件中，如下

所示：

    <?xml version="1.0" encoding="UTF-8"?>
    <root>
    <numbers>
    <!-- 
    	数字信息
    -->
    
    [
    	[1, 82, 65535],
    	[20, 90, 13],
    	[26, 809, 1024]
    ]
    
    </numbers>
    </root>

**第 0020 题：** [登陆中国联通网上营业厅](http://iservice.10010.com/index_.html) 后选择「自助服务」 --> 「详单查询」，然后选择你要查询的时间段，点击「查询」按钮，查询结果页面的最下方，点击「导出」，就会生成类似于 2014年10月01日～2014年10月31日通话详单.xls 文件。写代码，对每月通话时间做个统计。

**第 0021 题：** 通常，登陆某个网站或者 APP，需要使用用户名和密码。密码是如何加密后存储起来的呢？请使用 Python 对密码加密。

- 阅读资料 [用户密码的存储与 Python 示例](http://zhuoqiang.me/password-storage-and-python-example.html)

- 阅读资料 [Hashing Strings with Python](http://www.pythoncentral.io/hashing-strings-with-python/)

- 阅读资料 [Python's safest method to store and retrieve passwords from a database](http://stackoverflow.com/questions/2572099/pythons-safest-method-to-store-and-retrieve-passwords-from-a-database)

**第 0022 题：** iPhone 6、iPhone 6 Plus 早已上市开卖。请查看你写得 第 0005 题的代码是否可以复用。

**第 0023 题：** 使用 Python 的 Web 框架，做一个 Web 版本 留言簿 应用。

[阅读资料：Python 有哪些 Web 框架](http://v2ex.com/t/151643#reply53)

- ![留言簿参考](http://i.imgur.com/VIyCZ0i.jpg)


**第 0024 题：** 使用 Python 的 Web 框架，做一个 Web 版本 TodoList 应用。

- ![SpringSide 版TodoList](http://i.imgur.com/NEf7zHp.jpg)

**第 0025 题：** 使用 Python 实现：对着电脑吼一声,自动打开浏览器中的默认网站。


    例如，对着笔记本电脑吼一声“百度”，浏览器自动打开百度首页。
    
    关键字：Speech to Text
    
参考思路：    
1：获取电脑录音-->WAV文件
    python record wav

2：录音文件-->文本

    STT: Speech to Text
    
    STT API Google API

3:文本-->电脑命令

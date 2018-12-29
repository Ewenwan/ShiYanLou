# python数据结构和算法
[参考](https://github.com/facert/python-data-structure-cn)

      计算机科学是解决问题的研究。
      计算机科学使用抽象作为表示过程和数据的工具。
      抽象的数据类型允许程序员通过隐藏数据的细节来管理问题领域的复杂性。
      Python是一种强大但易于使用的面向对象语言。
      列表、元组和字符串都是用Python有序集合构建的。
      字典和集合是无序的数据集合。
      类允许程序员实现抽象的数据类型。
      程序员可以重写标准方法，并创建新的方法。
      类可以被组织成层次结构。
      类构造器应该总是调用其父节点的构造函数，然后继续使用自己的数据和行为。

  
算法计时 time模块，与算法复杂度 O()
```python
import time

def sumOfN2(n):
    start = time.time() # 计时开始
    
    theSum = 0 # 初始值
    for i in range(1,n+1):
        theSum = theSum + i # 叠加到累加器上 n(n+1)/2
        # n越大，计算时间越长，O(n)的 复杂度
    
    # 利用封闭方程而不是迭代来计算前n个整数的和
    # theSum =  n*(n+1)/2 # 该方法的计算时间 与 n 的大小无关
    # O(1)的复杂度
    
    end = time.time() #计时结束
    
    # 包含了执行结果和执行消耗时间的元组（tuple）
    return theSum, end-start

for i in range(5):
    print("Sum is %d required %10.7f seconds"%sumOfN(10000))
    
>>> 
Sum is 50005000 required  0.0018950 seconds
Sum is 50005000 required  0.0018620 seconds
Sum is 50005000 required  0.0019171 seconds
Sum is 50005000 required  0.0019162 seconds
Sum is 50005000 required  0.0019360 seconds

O(1)        常量
O(log(n))   对数复杂度
O(n)        线性 
O(n*log(n)) 线性 对数    
O(n^2)      平方
O(n^3)      立方
O(2^n)      指数   


```

![](https://facert.gitbooks.io/python-data-structure-cn/2.%E7%AE%97%E6%B3%95%E5%88%86%E6%9E%90/2.3.%E5%A4%A7O%E7%AC%A6%E5%8F%B7/assets/newplot.png)


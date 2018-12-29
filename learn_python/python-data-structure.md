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
      
# 基本概念
      
## 1. 时间复杂度分析

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




## 2. 乱序单词检测
1. 先排序，后比较
```python
def anagramSolution2(s1,s2):
    # 字符串转换成列表
    alist1 = list(s1)
    alist2 = list(s2)
    # 对列表字母 进行排序
    alist1.sort() // 复杂度  O(n*log(n))===
    alist2.sort()

    pos = 0
    matches = True
    
    # 比对两个有序 列表是否相同  # 复杂度O(n)====
    while pos < len(s1) and matches:
        if alist1[pos]==alist2[pos]:
            pos = pos + 1 # 相同，再查看下一个字符
        else:
            matches = False #不相同，非乱序单词

    return matches

print(anagramSolution2('abcde','edcba'))

# 总复杂度 : O(n*log(n))

```

2. 以空间换时间，生成直方图，比较直方图
```python
def anagramSolution4(s1,s2):
    
    # 小写字母 共26个
    c1 = [0]*26 # 两个单词的 字母直方图 记录
    c2 = [0]*26
    
    # 生成 字符串1 的 字母直方图  O(n)
    for i in range(len(s1)):
        pos = ord(s1[i])-ord('a') # 减去最小字母的 ascii码，之后就变成直方图bin的位置 0~25
        c1[pos] = c1[pos] + 1 # 对应bin位置 计数+1
        
    # 生成 字符串2 的 字母直方图  O(n)
    for i in range(len(s2)):
        pos = ord(s2[i])-ord('a')
        c2[pos] = c2[pos] + 1
    
    # 比对两字符串对应 的 字母直方图 是否相同  O(26)
    j = 0
    stillOK = True
    while j<26 and stillOK:
        if c1[j]==c2[j]:
            j = j + 1
        else:
            stillOK = False

    return stillOK

print(anagramSolution4('apple','pleap'))

# 总复杂度 O(n)

```

## 3. 列表 的构造速度
```python
# 拼接
def test1():
    l = []
    for i in range(1000):
        l = l + [i]

def test2():
    l = []
    for i in range(1000):
        l.append(i) # 插入

def test3():
    l = [i for i in range(1000)] #列表推导，列表生成器

def test4():
    l = list(range(1000)) # 调用 列表 构造函数 包装 range 函数

# 测试 timeit 模块 计时
t1 = Timer("test1()", "from __main__ import test1")
print("concat ",t1.timeit(number=1000), "milliseconds")

t2 = Timer("test2()", "from __main__ import test2")
print("append ",t2.timeit(number=1000), "milliseconds")
t3 = Timer("test3()", "from __main__ import test3")
print("comprehension ",t3.timeit(number=1000), "milliseconds")
t4 = Timer("test4()", "from __main__ import test4")
print("list range ",t4.timeit(number=1000), "milliseconds")

# from __main__ import test1 从 __main__ 命名空间导入到 timeit 设置的命名空间中。
# timeit 这么做是因为它想在一个干净的环境中做测试，而不会因为可能有你创建的任何杂变量，
# 以一种不可预见的方式干扰你函数的性能

>>>>>>
concat  6.54352807999 milliseconds
append  0.306292057037 milliseconds
comprehension  0.147661924362 milliseconds
list range  0.0655000209808 milliseconds   

append 操作比拼接快得多, 列表生成器的速度是 append 的两倍。

```

列表元素弹出性能记录
```python
# 从列表末尾处弹出所需时间将保持不变，即使列表不断增长。
popzero = timeit.Timer("x.pop(0)",
                       "from __main__ import x")
                       
# 从列表开始处弹出元素时间将随列表增长而增加   
popend = timeit.Timer("x.pop()",
                      "from __main__ import x")

x = list(range(2000000))
popzero.timeit(number=1000) # 传入测试次数

4.8213560581207275

x = list(range(2000000))
popend.timeit(number=1000)

0.0003161430358886719

```

## 4. 字典 相关的速度测试



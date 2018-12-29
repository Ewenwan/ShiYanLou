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
      
# 一、基本概念
      
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
    alist1.sort() # 复杂度  O(n*log(n))===
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
    l = [i for i in range(1000)] # 列表推导，列表生成器

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
字典和列表不同，你可以通过键而不是位置来访问字典中的项目。

```python
#  contains 操作 List 和 Dict 时间复杂性
import timeit
import random

for i in range(10000,1000001,20000):
    
    # i 在 x 中吗？操作，时间测试
    t = timeit.Timer("random.randrange(%d) in x"%i,
                     "from __main__ import random,x")
                     
    # 列表测试
    x = list(range(i))
    lst_time = t.timeit(number=1000)# 测试1000次??
    
    # 字典测试
    x = {j:None for j in range(i)}
    d_time = t.timeit(number=1000)
    
    # 打印两种时间
    print("%d,%10.3f,%10.3f" % (i, lst_time, d_time))


# 字典中的 contains 运算符的时间是恒定的，即使字典大小不断增长。
# 列表上的contains运算符所花费的时间与列表的大小成线性增长。

```


# 二、线性数据结构
      理解抽象数据类型的栈，队列，deque 和 列表。
      能够使用 Python 列表实现 ADT 堆栈，队列和 deque。
      了解基本线性数据结构实现的性能。
      了解前缀，中缀和后缀表达式格式。
      使用栈来实现后缀表达式。
      使用栈将表达式从中缀转换为后缀。
      使用队列进行基本时序仿真。
      能够识别问题中栈，队列和 deques 数据结构的适当使用。
      能够使用节点和引用将抽象数据类型列表实现为链表。
      能够比较我们的链表实现与 Python 的列表实现的性能。


      栈，队列，deques, 列表 是一类数据的容器，
      
      它们数据项之间的顺序由添加或删除的顺序决定。
      一旦一个数据项被添加，它相对于前后元素一直保持该位置不变。诸如此类的数据结构被称为线性数据结构。

      线性数据结构有两端，有时被称为左右，某些情况被称为前后。你也可以称为顶部和底部，名字都不重要。
      将两个线性数据结构区分开的方法是添加和移除项的方式，特别是添加和移除项的位置。
      例如一些结构允许从一端添加项，另一些允许从另一端移除项。
      这些变种的形式产生了计算机科学最有用的数据结构。
      他们出现在各种算法中，并可以用于解决很多重要的问题。

## 1. 栈
      栈（stack）又名堆栈，（有时称为“先进后出栈FILO”  后进先出LIFO ）是一个项的有序集合，其中添加移除新项总发生在同一端。
      这一端通常称为“顶部”。
      与顶部对应的端称为“底部”。
      
      先进入的数据被压入栈底，最后的数据在栈顶，需要读数据的时候从栈顶开始弹出数据（最后一个数据被第一个读出来）。
      栈具有记忆作用，对栈的插入与删除操作中，不需要改变栈底指针。
      
      向一个栈插入新元素又称作进栈、入栈或压栈，
      它是把新元素放到栈顶元素的上面，使之成为新的栈顶元素；
      从一个栈删除元素又称作 出栈 或 退栈，它是把栈顶元素删除掉，
      使其相邻的元素成为新的栈顶元素。
      
      栈的例子很常见。几乎所有的自助餐厅都有一堆托盘或盘子，你从顶部拿一个，就会有一个新的托盘给下一个客人。
      想象桌上有一堆书, 只有顶部的那本书封面可见，要看到其他书的封面，只有先移除他们上面的书。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.3.%E4%BB%80%E4%B9%88%E6%98%AF%E6%A0%88/assets/3.3.%E4%BB%80%E4%B9%88%E6%98%AF%E6%A0%88.png)
      
      
      
      
## 2. 队列




## 3. 双端队列 deques




## 4. 列表 List










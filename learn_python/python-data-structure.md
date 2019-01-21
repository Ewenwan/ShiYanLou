# python数据结构和算法
[参考](https://github.com/facert/python-data-structure-cn)

[python图解算法 选择/快速排序 哈希表 广度优先搜索算法 迪杰斯特拉算法 贪婪算法 动态规划 K-邻近算法 ](https://github.com/Ewenwan/grokking_algorithms)


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

使用字典记忆已求解，加速计算，计算最大冰雹序列长度
```python
def get_num(a,b):
    # 已求解 记录字典
    dict1 = {}
    max = 1
    num = 1
    # 在指定范围内求解  冰雹序列长度
    for x in range(a, b+1):
        i = 1 # 序列长度记录
        m = x # 序列数据x
        #===============冰雹序列
        while x > 1:
            # 先检查解集字典中是否存在
            if x in dict1:
                i += dict1[x] - 1 # 直接加上子数的 序列长度
                break #直接结束
            # 正常 冰雹序列求解过程=====
            else:
                if x%2: 
                    # 奇数==
                    x = 3*x + 1
                    i += 1      
                else:
                    # 偶数===
                    x = x//2
                    i += 1
        # 在字典中记录该解 ===
        dict1[m] = i
        # 记录最大的长度=====
        if i > max:
            max = i # 最大长度y
            num = m # 对应的x

    return (num, max)
    
lis = get_num(1,500000)
print(lis)
#(410011, 449)
[Finished in 1.9s]   
    
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
      
      栈之所以重要是因为它能反转项的顺序。
      插入跟删除顺序相反.
      
      你可以想到使用计算机的时候所碰到的例子。
      
      例如，每个 web 浏览器都有一个返回按钮。
      当你浏览网页时，这些网页被放置在一个栈中（实际是网页的网址）。
      你现在查看的网页在顶部，你第一个查看的网页在底部。
      如果按‘返回’按钮，将按相反的顺序浏览刚才的页面。
      
### 栈的抽象数据类型
    栈是有序的 LIFO 。
    栈操作如下。
      Stack()    创建一个空的新栈。 
                 它不需要参数，并返回一个空栈。
      push(item) 将一个新项添加到栈的顶部。           入栈
                 它需要 item 做参数并不返回任何内容。
      pop()      从栈中删除顶部项。                  出栈
                 它不需要参数并返回 item 。
                 栈被修改。
      peek()    从栈返回顶部项，但不会删除它。        把脉，看一眼顶端元素
                 不需要参数。 
                 不修改栈。
      isEmpty()  测试栈是否为空。
                 不需要参数，并返回布尔值。
      size()     返回栈中的 item 数量。不
                 需要参数，并返回一个整数。   
      
举例：
     
     s 是已经创建的空栈，Table1 展示了栈操作序列的结果。
     栈中，顶部项 列在最右边，栈底在左边
     
     操作              栈内容             返回
     s.isEmpty()       []                True
     s.push(4)         [4]
     s.push('dog')     [4,'dog'] 
     s.peek()          [4,'dog']         'dog'
     s.push(True)      [4,'dog',True]
     s.size()          [4,'dog',True]    3
     s.isEmpty()       [4,'dog',True]    False
     s.push(8.4)       [4,'dog',True,8.4]
     s.pop()           [4,'dog',True]    8.4
     s.pop()           [4,'dog']         True
     s.size()          [4,'dog']         2
     
### python 实现栈
      回想一下，当我们给抽象数据类型一个物理实现时，我们将实现称为数据结构。
      抽象数据类型（如栈）的选择的实现是创建一个新类。
      栈操作实现为类的方法。
      此外，为了实现作为元素集合的栈，使用由 Python 提供的原语集合的能力是有意义的。 
      我们将使用 列表 作为底层实现。
     
     
      Python 中的列表类提供了有序集合机制和一组方法。
      例如，如果我们有列表 [2,5,3,6,7,4]，我们只需要确定列表的哪一端将被认为是栈的顶部。
      一旦确定，可以使用诸如 append 和 pop 的列表方法来实现操作。
      
      以下栈实现（ActiveCode 1）假定列表的结尾将保存栈的顶部元素。
      随着栈增长（push 操作），新项将被添加到列表的末尾。 
      pop 也操作列表末尾的元素。       
      
```python
class Stack:
     # 类初始化函数
     def __init__(self):
         self.items = [] # 初始化空列表
     
     # 是否为空
     def isEmpty(self):
         return self.items == []
     
     # 入栈，直接放入 列表尾部
     def push(self, item):
         self.items.append(item)
     
     # 出栈，列表 删除并范围 尾部元素
     def pop(self):
         return self.items.pop()
     
     # 看一眼列表尾部元素
     def peek(self):
         return self.items[len(self.items)-1]
     
     # 栈大小
     def size(self):
         return len(self.items)
         
# 使用栈 
# Stack 类的定义是从 pythonds 模块导入的
# 也可以直接使用上面的

from pythonds.basic.stack import Stack

s=Stack()

print(s.isEmpty())
s.push(4)
s.push('dog')
print(s.peek())
s.push(True)
print(s.size())
print(s.isEmpty())
s.push(8.4)
print(s.pop())
print(s.pop())
print(s.size())

```

### 栈应用1：简单括号 () 单一圆括号匹配
      区分括号是否匹配的能力是识别很多编程语言结构的重要部分。
      具有挑战的是如何编写一个算法，能够从左到右读取一串符号，并决定符号是否平衡。
      为了解决这个问题，我们需要做一个重要的观察。从左到右处理符号时，
      最近开始符号必须与下一个关闭符号相匹配。
      此外，处理的第一个开始符号必须等待直到其匹配最后一个符号。
      结束符号以相反的顺序匹配开始符号。
      他们从内到外匹配。
      这是一个可以用栈解决问题的线索。
```python
from pythonds.basic.stack import Stack

def parChecker(symbolString):
    # 创建栈
    s = Stack()
    # 平衡标志
    balanced = True
    
    index = 0
    
    while index < len(symbolString) and balanced:
        # 当前 字符
        symbol = symbolString[index]
        
        # 是起始 括号
        if symbol == "(":
            s.push(symbol) # 入栈
            
        # 那就是 结尾括号 ")"    
        else:
            if s.isEmpty():
                # 无 起始括号，则，不平衡
                balanced = False
            # 已经有 起始括号了
            # 弹出最近入栈的 一个 起始括号
            else:
                s.pop()
        
        # 处理下一个字符
        index = index + 1
    
    # 最后 平衡，且 栈为空了
    if balanced and s.isEmpty():
        return True # 则是平衡的
    else:
        # 栈内还有 起始括号，或者多余 结束括号，都是不平衡的
        return False

print(parChecker('((()))'))
print(parChecker('(()'))


```

### 栈应用2：符号匹配 圆括号() 方括号[] 花括号{} 匹配  
      在 Python 中，
      方括号 [ 和 ] 用于列表，
      花括号 { 和 } 用于字典。
      圆括号 ( 和 ) 用于元组和算术表达式。
      只要每个符号都能保持自己的开始和结束关系，就可以混合符号。
      
      符号字符串如
      { { ( [ ] [ ] ) } ( ) }
      
      不仅每个开始符号都有对应的结束符号，而且符号的类型也匹配。
      上节的简单括号检查程序可以轻松的扩展处理这些新类型的符号。
      
      每个开始符号被简单的压入栈中，等待匹配的结束符号出现。
      当出现结束符号时，唯一的区别是我们必须检查确保它正确匹配栈顶部开始符号的类型。
      如果两个符号不匹配，则字符串不匹配。
      如果整个字符串都被处理完并且没有什么留在栈中，则字符串匹配。
      
```python
from pythonds.basic.stack import Stack

def parChecker(symbolString):
    # 空栈，用于存储 三种开始 字符
    s = Stack()
    balanced = True # 平衡标志
    index = 0
    
    while index < len(symbolString) and balanced:
        # 当前字符
        symbol = symbolString[index]
        
        # 是三种开始字符 中的一种
        if symbol in "([{":
            s.push(symbol)
        
        # 否者 为三种 结束字符 中的一种
        else:
            if s.isEmpty():
                # 无开始字符，来匹配结束字符，则不平衡
                balanced = False
            else:
                top = s.pop() # 弹出最近的开始字符
                # 查看是否 与 该 结束字符 匹配
                if not matches(top,symbol):
                    # 不匹配，则 不平衡
                    balanced = False
        # 检查下一个字符
        index = index + 1
        
    if balanced and s.isEmpty():
        # 平衡，且已无 开始字符
        return True
    # 不平衡，或 还有剩余开始字符未匹配到 结束字符
    else:
        return False

# 开始字符 open 与 结束字符 close 是否匹配
def matches(open,close):
    opens   = "([{"
    closers = ")]}"
    # 对应字符索引，相同的话，就匹配
    return opens.index(open) == closers.index(close)


print(parChecker('{{([][])}()}'))
print(parChecker('[{()]'))

```


### 栈应用3：十进制转换成二进制
      二进制在计算机科学中是很重要的，因为存储在计算机内的所有值都是以 0 和 1 存储的。
      如果没有能力在二进制数和普通字符串之间转换，我们与计算机之间的交互非常棘手。
      
      如何能够容易地将整数值转换为二进制呢？
          答案是“除 2”算法，它用栈来跟踪二进制结果的数字。
      “除 2” 算法假定我们从大于 0 的整数开始。不断迭代的将十进制除以 2，并跟踪余数。
      第一个除以 2 的余数说明了这个值是偶数还是奇数。偶数有 0 的余数，记为 0。
      奇数有余数 1，记为 1.我们将得到的二进制构建为数字序列，第一个余数实际上是序列中的最后一个数字。
      我们再次看到了反转的属性，表示栈可能是解决这个问题的数据结构。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.8.%E5%8D%81%E8%BF%9B%E5%88%B6%E8%BD%AC%E6%8D%A2%E6%88%90%E4%BA%8C%E8%BF%9B%E5%88%B6/assets/3.8.%E5%8D%81%E8%BF%9B%E5%88%B6%E8%BD%AC%E6%8D%A2%E6%88%90%E4%BA%8C%E8%BF%9B%E5%88%B6.figure5.png)
      
```python
from pythonds.basic.stack import Stack

def divideBy2(decNumber):
    # 空栈，记录二进制串码
    remstack = Stack()

    while decNumber > 0:
        rem = decNumber % 2 # 除以二，取余数
        remstack.push(rem)  # 余数进栈
        # 使用商，继续循环
        decNumber = decNumber // 2
    
    # 生成二进制串码
    # 栈顶，最后入栈的，为二进制串码的 高位(左边)
    binString = ""
    while not remstack.isEmpty():
        # 依次出栈，转换成字符串后，连接在一起
        binString = binString + str(remstack.pop())
    # 返回生成的 二进制串码
    return binString

print(divideBy2(42))


'''
可以修改 divideBy2 函数，使它不仅能接受十进制参数，还能接受预期转换的基数。
‘除 2’ 的概念被简单的替换成更通用的 ‘除基数’。
下面展示的是一个名为 baseConverter 函数。
采用十进制数和 2 到 16 之间的任何基数作为参数。
余数部分仍然入栈，直到被转换的值为 0。
我们创建一组数字，用来表示超过 9 的余数。
'''

# 输入十进制数 和 目标基底数串
# 也可以改进，传入一个 指定基底的子串(计算得到十进制数后，再使用 除基底法)，和目标基底
def baseConverter(decNumber,base):
    # 0~15的余数
    digits = "0123456789ABCDEF"
    
    # 空栈记录余数
    remstack = Stack()

    while decNumber > 0:
        rem = decNumber % base # 十进制数 取余 基底数 得到余数
        remstack.push(rem)     # 栈记录 余数
        decNumber = decNumber // base  # 使用 商

    newString = ""
    while not remstack.isEmpty():
        # 余数依次出栈，转换成字符串后，连接在一起
        newString = newString + digits[remstack.pop()]
        
    return newString

print(baseConverter(25,2))
print(baseConverter(25,16))


```

### 栈应用4： 中缀，前缀和后缀表达式
      算术表达式如 B*C，乘法运算符 * 出现在表达式中。
      这种类型的符号称为中缀，因为运算符在它处理的两个操作数之间。
      看另外一个中缀示例，A+B*C，运算符 + 和 * 仍然出现在操作数之间。
      这里面有个问题是，他们分别作用于哪个运算数上，
      + 作用于 A 和 B , 还是 * 作用于 B 和 C？表达式似乎有点模糊。
      
      每个运算符都有一个优先级。
      优先级较高的运算符在优先级较低的运算符之前使用。
      
      唯一改变顺序的是括号的存在。
      
      算术运算符的优先顺序是将乘法和除法置于加法和减法之上。
      
      如果出现具有相等优先级的两个运算符，则使用从左到右的顺序排序或关联。
      
      例如：
      A+B*C， B 和 C 首先相乘，然后将 A 与该结果相加。
      (A+B)*C 将强制在乘法之前执行 A 和 B 的加法。
      在表达式 A+B+C 中，最左边的 + 首先使用。
      
      计算机需要准确知道要执行的操作符和顺序。
      一种保证不会对操作顺序产生混淆的表达式的方法是创建一个称为完全括号表达式的表达式。
      这种类型的表达式对每个运算符都使用一对括号。
      括号没有歧义的指示操作的顺序。
      也没有必要记住任何优先规则。
      
      表达式 A+B*C+D 可以重写为 ((A + (B * C)) + D) ，表明先乘法，然后是左边的加法。
      A + B + C + D 可以写为 (((A + B) + C) + D)，因为加法操作从左向右相关联。
      
      中缀表达式 A+B, 如果我们移动两个操作数之间的运算符会发生什么？结果表达式变成 + A B。
      同样，我们也可以将运算符移动到结尾，得到 A B + ，这样看起来有点奇怪。
      
      改变操作符的位置得到了两种新的表达式格式，前缀和后缀。
      前缀表达式符号要求所有运算符在它们处理的两个操作数之前。
      另一个方面，后缀要求其操作符在相应的操作数之后。
                   前缀                        后缀
      A + B   ==> + A B                       A B +
      A + B * C => + A * B C                  A B C * +
      
      A + B * C + D => + + A * B C D          A B C * + D +
      
      (A + B) * C  => * + A B C               A B + C *
      (A + B) * (C + D)  => * + A B + C D     A B + C D + *
      A * B + C * D =>  + * A B * C D         A B * C D * +
      A + B + C + D =>  + + + A B C D         A B + C + D +  
      
      再次考虑表达式 A + B * C。
      如上所示，A B C * +是等价的后缀表达式。 
      我们已经注意到，操作数 A，B 和 C 保持在它们的相对位置。
      只有操作符改变位置。
      再看中缀表达式中的运算符。
      从左到右出现的第一个运算符为 +。 
      然而，在后缀表达式中，+ 在结束位置，因为下一个运算符 * 的优先级高于加法。
      原始表达式中的运算符的顺序在生成的后缀表达式中相反。（栈性质?）
      
      当我们处理表达式时，操作符必须保存在某处，因为它们相应的右操作数还没有看到。
      此外，这些保存的操作符的顺序可能由于它们的优先级而需要反转。
      这是在该示例中的加法和乘法的情况，由于加法运算符在乘法运算符之前，
      并且具有较低的优先级，因此需要在使用乘法运算符之后出现。 
      由于这种顺序的反转，考虑使用栈来保存运算符直到用到它们是有意义的。
      
      (A + B)* C的情况会是什么样呢？ 回想一下，A B + C *是等价的后缀表达式。
      从左到右处理此中缀表达式，我们先看到 +。 
      在这种情况下，当我们看到 *，+已经放置在结果表达式中，由于括号它的优先级高于*。
      我们现在可以开始看看转换算法如何工作。
      当我们看到左括号时，我们保存它，表示高优先级的另一个运算符将出现。
      该操作符需要等到相应的右括号出现以表示其位置（回忆完全括号的算法）。 
      当右括号出现时，可以从栈中弹出操作符。
      
      当我们从左到右扫描中缀表达式时，我们将使用栈来保留运算符。
      这将提供我们在第一个例子中注意到的反转。
      堆栈的顶部将始终是最近保存的运算符。
      每当我们读取一个新的运算符时，
      我们需要考虑该运算符如何与已经在栈上的运算符（如果有的话）比较优先级。
      
      假设中缀表达式是一个由空格分隔的标记字符串。 
      操作符标记是*，/，+和 - ，以及左右括号。
      操作数是单字符 A，B，C 等。 
      
      以下步骤将 生成一个 后缀顺序 字符串。
            1. 创建一个名为 opstack 的空栈以保存运算符。
               给输出创建一个空列表。
            2. 通过使用字符串方法拆分 将 输入的 中缀字符串 转换为 标记列表。
            3. 从左到右扫描标记列表。
               a. 如果标记 是 操作数，将其附加到输出 列表的末尾(操作数顺序不变)。
               b. 如果标记是左括号，将其压到 opstack 上。（起始括号入栈）
               c. 如果标记是右括号，则弹出 opstack，直到删除相应的左括号。
                  将每个运算符附加到输出列表的末尾（括号的优先级高）。
               d. 如果标记是运算符，*，/，+或 - ，将其压入 opstack。
                  但是，首先删除已经在 opstack 中具有更高 或 相等优先级的 任何运算符，并将它们加到输出列表中。

            4. 当输入表达式被完全处理时，检查 opstack。
               仍然在栈上的任何运算符都可以删除并加到输出列表的末尾。

![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.9.%E4%B8%AD%E7%BC%80%E5%89%8D%E7%BC%80%E5%92%8C%E5%90%8E%E7%BC%80%E8%A1%A8%E8%BE%BE%E5%BC%8F/assets/3.9.%E4%B8%AD%E7%BC%80%E5%90%8E%E7%BC%80%E5%92%8C%E5%90%8E%E7%BC%80%E8%A1%A8%E8%BE%BE%E5%BC%8F.figure9.png)
      
      看到操作数A,直接放入输出列表尾部 [A]。
      看到第一个 * 时，入栈，看到操作数B 放入输出列表尾部 [A B]。
      在看到 + 运算符时，先删除栈中高优先级的运算符 *号，并把删除的运算符*号 放入 输出列表尾部 [A B *]，
            同时+号入栈
      看到操作数C 放入输出列表尾部 [A B * C]。
      看到第二个 * 出现时，+ 保留在栈中，因为乘法优先级高于加法，*号入栈。
      看到操作数D 放入输出列表尾部 [A B * C D]。
      弹出*号，放入输出列表尾部 [A B * C D *]。
      弹出+号，放入输出列表尾部 [A B * C D * +]。
      在中缀表达式的末尾，栈被弹出两次，删除两个运算符，并将 + 作为后缀表达式中的最后一个运算符。
      
```python
from pythonds.basic.stack import Stack

def infixToPostfix(infixexpr):
    # 使用一个名为 prec 的字典来保存操作符的优先级。
    # 这个字典将每个 运算符 映射 到一个整数
    prec = {}
    prec["*"] = 3 # 最高优先级
    prec["/"] = 3
    prec["+"] = 2
    prec["-"] = 2
    prec["("] = 1     
    # 左括号将赋予最低的值??。这样，与其进行比较的任何运算符将具有更高的优先级，将被放置在它的顶部。
    
    # 1. 运算符 记忆 括号 栈
    opStack = Stack()
    # 输出 后缀字符串 列表
    postfixList = []
    
    # 2. 输入表达式 按空格分割成 列表===
    tokenList = infixexpr.split()
    
    # 3. 遍历 表达式 列表
    for token in tokenList:
        # 3.a 大小字母 或 数字======
        if token in "ABCDEFGHIJKLMNOPQRSTUVWXYZ" or token in "0123456789":
            # 直接放入 输出 后缀字符串 列表 尾部
            postfixList.append(token)
        # 3.b 左括号，起始括号
        elif token == '(':
            opStack.push(token) # 左括号放入栈
        # 3.c 右括号    
        elif token == ')':
            # 保存运算符与括号的栈  出栈
            topToken = opStack.pop()
            while topToken != '(': 
                # 如果不是 左括号 直接放入  输出 后缀字符串 列表 尾部
                # 是括号内部的 运算符，优先放入 
                postfixList.append(topToken)
                # 弹出下一个栈元素
                topToken = opStack.pop()
        # 3.4 运算符
        else:
            while (not opStack.isEmpty()) and \
               (prec[opStack.peek()] >= prec[token]):
                # 当前栈顶元素 比 遍历到的 运算符 优先级高
                postfixList.append(opStack.pop()) # 将优先级高的 运算符直接放入到 后缀字符串 列表 尾部
            # 将当前  遍历到的 运算符 放入到 栈中
            opStack.push(token)
    
    # 4 最后 检查 opstack
    while not opStack.isEmpty():
        # 运算符 栈不空?
        # 删除运算符，并添加到 输出后缀字符串 列表 尾部
        postfixList.append(opStack.pop()) # 未考虑 括号不匹配的情况?????
    return " ".join(postfixList)

print(infixToPostfix("A * B + C * D"))
print(infixToPostfix("( A + B ) * C - ( D - E ) * ( F + G )"))

执行结果如下 

>>> infixtopostfix("( A + B ) * ( C + D )")
'A B + C D + *'
>>> infixtopostfix("( A + B ) * C")
'A B + C *'
>>> infixtopostfix("A + B * C")
'A B C * +'
>>>


# 利用栈 后缀表达式求值
'''
作为最后栈的示例，我们考虑对后缀符号中的表达式求值。
在这种情况下，栈再次是我们选择的数据结构。
但是，在扫描后缀表达式时，它必须等待操作数，而不像上面的转换算法中的运算符。
解决问题的另一种方法是，每当在输入上看到运算符时，计算两个最近的操作数。
要详细的了解这一点，考虑后缀表达式 4 5 6 * +， 首先遇到操作数 4 和 5，此时，
你还不确定如何处理它们，直到看到下一个符号。
将它们放置到栈上，确保它们在下一个操作符出现时可用。
在这种情况下，下一个符号是另一个操作数。所以，像先前一样，压入栈中。并检查下一个符号。
现在我们看到了操作符 *，这意味着需要将两个最近的操作数相乘。
通过弹出栈两次，我们可以得到正确的两个操作数，然后执行乘法（这种情况下结果为 30）。
我们现在可以通过将其放回栈中来处理此结果，以便它可以表示为表达式后面的运算符的操作数。
当处理最后一个操作符时，栈上只有一个值，弹出并返回它作为表达式的结果。

7 8 + 3 2 + / 。
在这个例子中有两点需要注意，
首先，栈的大小增长收缩，然后再子表达式求值的时候再次增长。
第二，除法操作需要谨慎处理。回想下，后缀表达式的操作符顺序没变，仅仅改变操作符的位置。
当用于除法的操作符从栈中弹出时，它们被反转。
由于除法不是交换运算符，换句话说 15/5和 5/15 不同，
因此我们必须保证操作数的顺序不会交换。

假设后缀表达式是一个由空格分隔的标记字符串。 
运算符为*，/，+和 - ，操作数假定为单个整数值。
输出将是一个整数结果。

      1. 创建一个名为 operandStack 的空栈。
      2. 拆分字符串转换为标记列表。
      3. 从左到右扫描标记列表。
        3.1 如果标记是操作数，将其从字符串转换为整数，并将值压到operandStack。
        3.2 如果标记是运算符*，/，+或-，它将需要两个操作数。
            弹出operandStack 两次。 
            第一个弹出的是第二个操作数，第二个弹出的是第一个操作数。
            执行算术运算后，将结果压到操作数栈中。
      4. 当输入的表达式被完全处理后，结果就在栈上，弹出 operandStack 并返回值。

'''
from pythonds.basic.stack import Stack

# 计算 后缀表达式的值
def postfixEval(postfixExpr):
    #  1. 创建一个名为 operandStack 的空栈。
    operandStack = Stack()
    # 2. 拆分字符串转换为标记列表。
    tokenList = postfixExpr.split()
    # 3. 从左到右扫描标记列表。
    for token in tokenList:
        # 3.1 如果标记是操作数
        if token in "0123456789":
            # 将其从字符串转换为整数，并将值压到operandStack。
            operandStack.push(int(token))
        # 3.2 如果标记是运算符*，/，+或-
        else:
            # 第一个弹出的是第二个操作数
            operand2 = operandStack.pop()
            # 第二个弹出的是第一个操作数。
            operand1 = operandStack.pop()
            # 执行算术运算
            result = doMath(token,operand1,operand2)
            # 将结果压到操作数栈中
            operandStack.push(result)
    # 4. 当输入的表达式被完全处理后，结果就在栈上，弹出 operandStack 并返回值。
    return operandStack.pop()

# 它将获取两个操作数和运算符，执行相应的计算
def doMath(op, op1, op2):
    if op == "*":
        return op1 * op2
    elif op == "/":
        return op1 / op2
    elif op == "+":
        return op1 + op2
    else:
        return op1 - op2

print(postfixEval('7 8 + 3 2 + /'))

```
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.9.%E4%B8%AD%E7%BC%80%E5%89%8D%E7%BC%80%E5%92%8C%E5%90%8E%E7%BC%80%E8%A1%A8%E8%BE%BE%E5%BC%8F/assets/3.9.%E4%B8%AD%E7%BC%80%E5%90%8E%E7%BC%80%E5%92%8C%E5%90%8E%E7%BC%80%E8%A1%A8%E8%BE%BE%E5%BC%8F.figure10.png)  
      
      
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.9.%E4%B8%AD%E7%BC%80%E5%89%8D%E7%BC%80%E5%92%8C%E5%90%8E%E7%BC%80%E8%A1%A8%E8%BE%BE%E5%BC%8F/assets/3.9.%E4%B8%AD%E7%BC%80%E5%90%8E%E7%BC%80%E5%92%8C%E5%90%8E%E7%BC%80%E8%A1%A8%E8%BE%BE%E5%BC%8F.figure11.png)



## 2. 队列
      队列是项的有序结合，其中添加新项的一端称为队尾，移除项的一端称为队首。
      当一个元素从队尾进入队列时，一直向队首移动，直到它成为下一个需要移除的元素为止。
      最近添加的元素必须在队尾等待。
      集合中存活时间最长的元素在队首，这种排序成为 FIFO，先进先出，也被成为先到先得。
      
      队列的最简单的例子是我们平时不时会参与的列。
      排队等待电影，在杂货店的收营台等待，在自助餐厅排队等待（这样我们可以弹出托盘栈）。
      行为良好的线或队列是有限制的，因为它只有一条路，只有一条出路。
      不能插队，也不能离开。你只有等待了一定的时间才能到前面。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.10.%E4%BB%80%E4%B9%88%E6%98%AF%E9%98%9F%E5%88%97/assets/3.10.%E4%BB%80%E4%B9%88%E6%98%AF%E9%98%9F%E5%88%97.figure1.png)
      
      12306网上订票排队。。。。
      除了打印机打印队列，操作系统使用多个不同的队列来控制计算机内的进程。
      下一步做什么的调度通常基于尽可能快地执行程序和尽可能多的服务用户的排队算法。
      此外，当我们敲击键盘时，有时屏幕上出现的字符会延迟。
      这是由于计算机在那一刻做其他工作。
      按键的内容被放置在类似队列的缓冲器中，使得它们最终可以以正确的顺序显示在屏幕上。
      
      
### 抽象数据类型
     队列被构造为在队尾添加项的有序集合，并且从队首移除。
     队列保持 FIFO 排序属性。
     队列操作如下。
     
      Queue()       创建一个空的新队列。 它不需要参数，并返回一个空队列。
      enqueue(item) 将新项添加到队尾。 它需要 item 作为参数，并不返回任何内容。
      dequeue()     从队首移除项。它不需要参数并返回 item。 队列被修改。
      isEmpty()     查看队列是否为空。它不需要参数，并返回布尔值。
      size()        返回队列中的项数。它不需要参数，并返回一个整数。
      
      作为示例，我们假设 q 是已经创建并且当前为空的队列。
         操作              队列状态             返回数据
         q.isEmpty()       []                  True        队列是否为空
         q.enqueue(4)      [4]                             队尾添加元素
         q.enqueue('dog')  ['dog',4]                       队尾添加元素
         q.enqueue(True)   [True,'dog',4]                  队尾添加元素
         q.size()          [True,'dog',4]      3           队列大小
         q.enqueue(8.4)    [8.4,True,'dog',4]              队尾添加元素
         q.dequeue()       [8.4,True,'dog']    4           队首删除元素
         q.dequeue()       [8.4,True]          'dog'       队首删除元素
         q.size()          [True,'dog']        2           队列大小
      
### Python实现队列
      实现队列抽象数据类型创建一个新类。
      和前面一样，我们将使用列表集合来作为构建队列的内部表示。
      我们需要确定列表的哪一端作为队首，哪一端作为队尾。
      假定队尾在列表中的位置为 0。
      这允许我们使用列表上的插入函数向队尾添加新元素。
      弹出操作可用于删除队首的元素（列表的最后一个元素）。
      回想一下，这也意味着入队为 O(n) ( 数据依次后移 )，出队为 O(1)。
      
```python

class Queue:
    def __init__(self):
        self.items = []

    def isEmpty(self):
        return self.items == []

    def enqueue(self, item):
        self.items.insert(0,item)

    def dequeue(self):
        return self.items.pop()

    def size(self):
        return len(self.items)


# 测试
进一步的操作这个队列产生如下结果：

q = Queue()
q.enqueue(4)
q.enqueue('dog')
q.enqueue(True)
q.size()
>>> 3
 q.isEmpty()
>>>False
q.enqueue(8.4)
q.dequeue()
>>> 4
q.dequeue()
>>> 'dog'
q.size()
>>> 2
```

### 队列应用1: 模拟：烫手山芋
      在这个游戏中，孩子们围成一个圈，并尽可能快的将一个山芋递给旁边的孩子。
      在某一个时间，动作结束，有山芋的孩子从圈中移除。
      游戏继续开始直到剩下最后一个孩子。
      
      这个游戏相当于著名的约瑟夫问题，一个一世纪著名历史学家弗拉维奥·约瑟夫斯的传奇故事。
      故事讲的是，他和他的 39 个战友被罗马军队包围在洞中。
      他们决定宁愿死，也不成为罗马人的奴隶。
      他们围成一个圈，其中一人被指定为第一个人，顺时针报数到第七人，就将他杀死。
      约瑟夫斯是一个成功的数学家，他立即想出了应该坐到哪才能成为最后一人。
      最后，他加入了罗马的一方，而不是杀了自己。
      你可以找到这个故事的不同版本，有些说是每次报数 3 个人，有人说允许最后一个人逃跑。
      无论如何，思想是一样的。
      
      我们的程序将输入名称列表和一个称为 num 常量用于报数(报到该数时，淘汰)。
      它将返回以 num 为单位重复报数后剩余的最后一个人的姓名。
      
      为了模拟这个圈，我们使用队列。假设拿着山芋的孩子在队列的前面。
      当拿到山芋的时候，这个孩子将先出列再入队列，把他放在队列的最后。
      经过 num 次的出队入队后，前面的孩子将被永久移除队列。
      并且另一个周期开始，继续此过程，直到只剩下一个名字（队列的大小为 1时）。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.13.%E6%A8%A1%E6%8B%9F%EF%BC%9A%E7%83%AB%E6%89%8B%E5%B1%B1%E8%8A%8B/assets/3.13.%E6%A8%A1%E6%8B%9F%EF%BC%9A%E7%83%AB%E6%89%8B%E5%B1%B1%E8%8A%8B.figure3.png)
      
      
```python
from pythonds.basic.queue import Queue

# 传入一个名字列表，和报到的数
def hotPotato(namelist, num):
    # 1. 新建空队列====
    simqueue = Queue()
    # 2. 名字放入队列=====
    for name in namelist:
        simqueue.enqueue(name)
    
    # 3. 玩游戏，报数，淘汰，...
    while simqueue.size() > 1:
        for i in range(num):
            # 队首出队，队尾入队
            simqueue.enqueue(simqueue.dequeue())
        # 报数到 num时，只出队，被淘汰
        simqueue.dequeue()
    # 4. 还剩一个玩家时，游戏结束====
    return simqueue.dequeue()

print(hotPotato(["Bill","David","Susan","Jane","Kent","Brad"],7))
# 计数常数的值大于列表中的名称数。这不是一个问题，因为队列像一个圈，计数会重新回到开始，直到达到计数值。
# 另外，请注意，列表加载到队列中以使列表上的名字位于队列的前面。
# 在这种情况下，Bill 是列表中的第一个项，因此他在队列的前面。

```
      
      
      
###  队列应用2: 模拟： 打印机
      假设 平均 3分钟(180秒) 会有一个打印任务
      对于每一秒，我们可以通过生成 1 到 180 之间的随机数来模拟打印任务发生的机会。
      如果数字是 180，我们说一个任务已经创建。
      请注意，可能会在一下子创建许多任务，或者需要等待一段时间才有任务。
      这就是模拟的本质。
      
主要模拟步骤

      1. 创建打印任务的队列，每个任务都有个时间戳。队列启动的时候为空。
      2. 每秒（currentSecond）：
         2.1 是否创建新的打印任务？
             如果是，将 currentSecond 作为 时间戳 添加到 队列。
         2.2 如果 打印机 不忙 并且 有任务在等待（打印队列不空）
             从 打印机队列 中删除 一个任务 并将其分配给打印机
             从 currentSecond 中减去 该任务的时间戳，以计算 该任务 的 等待时间。
             将该任务的 等待时间 附加到列表中稍后处理。
             根据打印任务的页数，确定需要多少时间（所需处理时间）。
      3. 该任务的所需的等待时间 减去一秒。
      4. 如果任务已经完成，换句话说，所需的时间已经达到零，打印机空闲。
      模拟完成后，从生成的等待时间列表中计算平均等待时间。    

Python 实现

为了设计此模拟，我们将为上述三个真实世界对象创建类：

打印机(Printer), 打印任务(Task), 打印队列(PrintQueue)

```python
# 打印机(Printer)
class Printer:
    def __init__(self, ppm):
        self.pagerate = ppm     # 打印速度，页/每分钟
        self.currentTask = None # 当前是否有任务
        self.timeRemaining = 0  # 任务所需时间
    
    # 任务计时
    def tick(self):
        if self.currentTask != None:
            # 任务所需时间，每过一秒，任务时间-1
            self.timeRemaining = self.timeRemaining - 1
            if self.timeRemaining <= 0:
                # 任务已经完成，当前无任务
                self.currentTask = None

    def busy(self):
        if self.currentTask != None:
            return True # 无任务，空闲
        else:
            return False # 有任务
    
    # 添加一个任务
    def startNext(self,newtask):
        self.currentTask = newtask # 新任务
        # 根据任务的页数计算所需的时间
        self.timeRemaining = newtask.getPages() * 60/self.pagerate

# 生成打印任务(Task)====
# 创建任务时，随机数生成器将提供 1 到 20 页的长度
# 还需要保存一个时间戳用于计算等待时间
import random

class Task:
    def __init__(self,time):
        self.timestamp = time # 任务开始时间
        self.pages = random.randrange(1,21)# 任务量，打印页数

    def getStamp(self):
        return self.timestamp

    def getPages(self):
        return self.pages
    
    # 计算任务已经持续的时间
    def waitTime(self, currenttime):
        return currenttime - self.timestamp
        

#  整个仿真
from pythonds.basic.queue import Queue

import random

# 任务仿真
def simulation(numSeconds, pagesPerMinute):
    # 创建打印机 实例
    labprinter = Printer(pagesPerMinute) # 打印机速度 页/每分钟
    # 创建打印机队列实例
    printQueue = Queue()
    # 等待时间列表
    waitingtimes = []
    
    # 工作时间范围
    for currentSecond in range(numSeconds):
        # 该秒是否需要创建任务
        if newPrintTask():
            # 创建任务
            task = Task(currentSecond)
            # 把打印任务 加入打印机队列
            printQueue.enqueue(task)
            
        # 打印机不忙 并且 有打印任务
        if (not labprinter.busy()) and (not printQueue.isEmpty()):
            # 从队列中 取出一个打印任务
            nexttask = printQueue.dequeue()
            # 计算该任务 的等待时间
            waitingtimes.append(nexttask.waitTime(currentSecond))
            # 打印机开始下一个任务
            labprinter.startNext(nexttask)
            
        # 打印机工作，任务时间逐渐较少
        labprinter.tick()
    
    # 计算平均等待时间
    averageWait=sum(waitingtimes)/len(waitingtimes)
    # 大约
    print("Average Wait %6.2f secs %3d tasks remaining."%(averageWait,printQueue.size()))

# 每秒 根据生产的随机数决定是否需要产生打印任务，模拟打印任务生成
def newPrintTask():
    # 平均180秒生成一个任务
    # 每秒 生成1~180之间一个随机数，为180时，生成任务
    num = random.randrange(1,181)
    if num == 180:
        return True # 生成任务 标志为真
    else:
        return False

# 十次模型
for i in range(10):
    # 总时间 3600秒， 打印机速度 5页/每分钟 
    # 模拟 60 分钟（3,600秒）
    simulation(3600,5)
    
>>> 
Average Wait 165.38 secs 2 tasks remaining.
Average Wait  95.07 secs 1 tasks remaining.
Average Wait  65.05 secs 2 tasks remaining.
Average Wait  99.74 secs 1 tasks remaining.
Average Wait  17.27 secs 0 tasks remaining.
Average Wait 239.61 secs 5 tasks remaining.
Average Wait  75.11 secs 1 tasks remaining.
Average Wait  48.33 secs 0 tasks remaining.
Average Wait  39.31 secs 3 tasks remaining.
Average Wait 376.05 secs 1 tasks remaining.
我们可以看到，平均等待时间为 122.09 秒。 还可以看到平均等待时间有很大的变化，
最小值为 17.27 秒，最大值为 376.05 秒。 你也可能注意到，只有两种情况所有任务都完成。

# 打印速度 每分钟 10 页，再次运行 10 次测试，页面速度更快，我们希望在一小时内完成更多的任务。
>>>for i in range(10):
      simulation(3600,10)

Average Wait   1.29 secs 0 tasks remaining.
Average Wait   7.00 secs 0 tasks remaining.
Average Wait  28.96 secs 1 tasks remaining.
Average Wait  13.55 secs 0 tasks remaining.
Average Wait  12.67 secs 0 tasks remaining.
Average Wait   6.46 secs 0 tasks remaining.
Average Wait  22.33 secs 0 tasks remaining.
Average Wait  12.39 secs 0 tasks remaining.
Average Wait   7.27 secs 0 tasks remaining.
Average Wait  18.17 secs 0 tasks remaining.

```

## 3. 双端队列 deques
      deque（也称为双端队列）是与队列类似的项的有序集合。
      它有两个端部，首部和尾部，并且项在集合中保持不变。
      deque 不同的地方是添加和删除项是非限制性的。
      可以在前面或后面添加新项。
      同样，可以从任一端移除现有项。
      在某种意义上，这种混合线性结构提供了单个数据结构中的栈和队列的所有能力。 
      
      要注意，即使 deque 可以拥有栈和队列的许多特性，
      它不需要由那些数据结构强制的 LIFO 和 FIFO 排序。
      
      这取决于你如何持续添加和删除操作。

![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.15.%E4%BB%80%E4%B9%88%E6%98%AFDeque/assets/3.15.%E4%BB%80%E4%B9%88%E6%98%AFDeque.figure1-1.png)
      
      
### Deque 抽象数据类型
      
    deque 抽象数据类型由以下结构和操作定义。
    如上所述，deque 被构造为项的有序集合，其中项从首部或尾部的任一端添加和移除。
    下面给出了 deque 操作。
      Deque()        创建一个空的新 deque。它不需要参数，并返回空的 deque。
      addFront(item) 将一个新项添加到 deque 的首部。它需要 item 参数 并不返回任何内容。
      addRear(item)  将一个新项添加到 deque 的尾部。它需要 item 参数并不返回任何内容。
      removeFront()  从 deque 中删除首项。它不需要参数并返回 item。deque 被修改。
      removeRear()   从 deque 中删除尾项。它不需要参数并返回 item。deque 被修改。
      isEmpty()      测试 deque 是否为空。它不需要参数，并返回布尔值。
      size()         返回 deque 中的项数。它不需要参数，并返回一个整数。  

    假设 d 是已经创建并且当前为空的 deque:
      操作                DQ状态          返回          说明
      d.isEmpty()          []            True          是否为空
      d.addRear(4)         [4]                         尾部插入4
      d.addRear('dog')     ['dog',4]                   尾部插入'dog'
      d.addFront('cat')    ['dog',4,'cat']             前部插入'cat'
      d.addFront(True)     ['dog',4,'cat',True]        前部插入True
      d.size()             ['dog',4,'cat',True]   4    大小
      d.isEmpty()          ['dog',4,'cat',True]   False  是否为空 
      d.addRear(8.4)       [8.4,'dog',4,'cat',True]    尾部插入8.4
      d.removeRear()       ['dog',4,'cat',True]   8.4  尾部删除8.4
      d.removeFront()      ['dog',4,'cat']        True 尾部删除True
      
### python 实现 Deque
      为抽象数据类型 deque 的实现创建一个新类。
      同样，Python 列表将提供一组非常好的方法来构建 deque 的细节。
      我们的实现假定 deque 的尾部在列表中的位置为 0。
      
```python
class Deque:
    # 初始化空列表
    def __init__(self):
        self.items = []
    # 测试为空？
    def isEmpty(self):
        return self.items == []
    # 前端加入 元素，放在右面 O(1)
    def addFront(self, item):
        self.items.append(item)
    # 尾部加入元素， 在开始0位置插入，O(n)
    def addRear(self, item):
        self.items.insert(0,item)
    # 首部删除元素，删除右边的元素 O(n)
    def removeFront(self):
        return self.items.pop()
    # 尾部删除元素，删除位置0上的元素 O(n)
    def removeRear(self):
        return self.items.pop(0) # pop(0)方法必须删除列表的第一个元素。
    # 队列大小
    def size(self):
        return len(self.items)


```

### 双端队列 deques 应用1： 回文检查
      2018102
      回文是一个字符串，读取首尾相同的字符，
      例如，
        radar
        toot
        madam。 
      我们想构造一个算法输入一个字符串，并检查它是否是一个回文。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.18.%E5%9B%9E%E6%96%87%E6%A3%80%E6%9F%A5/assets/3.18.%E5%9B%9E%E6%96%87%E6%A3%80%E6%9F%A5.figure2.png)
      
      我们可以直接删除并比较首尾字符，只有当它们匹配时才继续。
      如果可以持续匹配首尾字符，我们最终要么用完字符，
      要么留出大小为 1 的deque，取决于原始字符串的长度是偶数还是奇数。
      在任一情况下，字符串都是回文。

```python

from pythonds.basic.deque import Deque

def palchecker(aString):
    # 空DQ
    chardeque = Deque()
    # 字符串创建 双端队列
    for ch in aString:
        chardeque.addRear(ch)
    # 对等
    stillEqual = True

    while chardeque.size() > 1 and stillEqual:
        # 右边元素
        first = chardeque.removeFront()
        # 左边元素
        last = chardeque.removeRear()
        # 左右两端元素应该匹配
        if first != last:
            # 不匹配，不是 回文
            stillEqual = False

    return stillEqual

print(palchecker("lsdkjfskf"))
print(palchecker("radar"))

```
      
## 4. 列表 List
      列表是一个强大但简单的收集机制，为程序员提供了各种各样的操作。
      然而，不是所有的编程语言都包括列表集合。
      在这些情况下，列表的概念必须由程序员实现。
      
      列表是项的集合，其中每个项保持相对于其他项的相对位置。
      更具体地，我们将这种类型的列表称为无序列表。
      我们可以将列表视为具有第一项，第二项，第三项等等。
      我们还可以引用列表的开头（第一个项）或列表的结尾（最后一个项）。
      为了简单起见，我们假设列表不能包含重复项。
      例如，整数 54,26,93,17,77 和 31 的集合可以表示考试分数的简单无序列表。
      请注意，我们将它们用逗号分隔，这是列表结构的常用方式。
      当然，Python 会显示这个列表为 [54,26,93,17,77,31]。
      
### 无序列表抽象数据类型
      无序列表的结构是项的集合，其中每个项保持相对于其他项的相对位置。
      下面给出了一些可能的无序列表操作。

      List()        创建一个新的空列表。它不需要参数，并返回一个空列表。
      add(item)     向列表中添加一个新项。它需要 item 作为参数，并不返回任何内容。
                    假定该 item 不在列表中。
      remove(item)  从列表中删除该项。它需要 item 作为参数并修改列表。假设项存在于列表中。
      search(item)  搜索列表中的项目。它需要 item 作为参数，并返回一个布尔值。
      isEmpty()     检查列表是否为空。它不需要参数，并返回布尔值。
      size()        返回列表中的项数。它不需要参数，并返回一个整数。
      append(item)  将一个新项添加到列表的末尾，使其成为集合中的最后一项。
                    它需要 item 作为参数，并不返回任何内容。假定该项不在列表中。
      index(item)   返回项在列表中的位置。它需要 item 作为参数并返回索引。假定该项在列表中。
      insert(pos, item) 在位置 pos 处向列表中添加一个新项。
                    它需要 item 作为参数并不返回任何内容。
                    假设该项不在列表中，并且有足够的现有项使其有 pos 的位置。
      pop()         删除并返回列表中的最后一个项。假设该列表至少有一个项。
      pop(pos)      删除并返回位置 pos 处的项。
                    它需要 pos 作为参数并返回项。假定该项在列表中。
      
### 无序列表实现：链表
      这些值已被随机放置。如果我们可以在每个项中保持一些明确的信息，
      即下一个项的位置，则每个项的相对位置可以通过简单地从一个项到下一个项的链接来表示。

![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure2.png)
      
      要注意，必须明确地指定链表的第一项的位置。
      一旦我们知道第一个项在哪里，第一个项目可以告诉我们第二个是什么，等等。
      外部引用通常被称为链表的头。
      类似地，最后一个项需要知道没有下一个项。
      
      
#### Node 节点类(值+指向)
      链表实现的基本构造块是节点。每个节点对象必须至少保存两个信息。
      首先，节点必须包含列表项本身。我们将这个称为节点的 数据字段。
      此外，每个节点必须保存 对 下一个节点的引用。 
      Node 类还包括访问，修改数据和访问下一个引用的常用方法。
```python
class Node:
    def __init__(self,initdata):
        # 数据字段
        self.data = initdata
        # 对 下一个节点的引用 
        self.next = None
        # 引用 None 代表没有下一个节点。
        # 请注意在构造函数中，最初创建的节点 next 被设置为 None。
        # 有时这被称为 接地节点，因此我们使用标准接地符号表示对 None 的引用。
        
    # 访问数据
    def getData(self):
        return self.data
        
    # 访问下一个引用
    def getNext(self):
        return self.next
        
    # 修改数据
    def setData(self,newdata):
        self.data = newdata
        
    # 修改 下一个引用
    def setNext(self,newnext):
        self.next = newnext

# 我们创建一个 Node 对象
>>> temp = Node(93)
>>> temp.getData()
93



```
      
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure4.png) 
      
      
      
#### 无序列表 Unordered List 类
      无序列表将从一组节点构建，每个节点通过显式引用链接到下一个节点。
      只要我们知道在哪里找到第一个节点（包含第一个项），
      之后的每个项可以通过连续跟随下一个链接找到。
      考虑到这一点，UnorderedList 类必须保持对第一个节点的引用。
      
```python
class UnorderedList:

    def __init__(self):
        # 记录表头
        self.head = None
        # 特殊引用 None 将再次用于表示链表的头部不引用任何内容。
        
    # 只是检查链表头是否是 None 的引用 ,判断链表是否为空
    # 显示了使用引用 None 来表示链接结构的 end 的优点
    def isEmpty(self):
        return self.head == None
    
    # 添加一个元素
    def add(self,item):
        # 新建一个节点
        temp = Node(item)
        # 新节点指向 原 链表头的指向 
        temp.setNext(self.head)
        # 在更新链表 头
        self.head = temp
        
    # 遍历链表，获取大小
    def size(self):
        # 表头后的第一个节点
        current = self.head
        count = 0
        
        # 非 接地， 未到 链表结尾
        while current != None:
            count = count + 1 # 计数+1
            current = current.getNext() # 迭代，后一个节点
        return count
        
    # 遍历链表，寻找指定元素
    def search(self,item):
        # 表头后的第一个节点
        current = self.head
        found = False # 未找到
        
        # 未到 链表结尾，且未找到
        while current != None and not found:
            # 该节点 数据项 是否为 指定元素
            if current.getData() == item:
                found = True
            # 遍历下一个节点
            else:
                current = current.getNext()
        # 返回 找到与否标志
        return found
    
    # 删除指定元素
    def remove(self,item):
        # 表头后的第一个节点
        current = self.head
        previous = None
        found = False # 寻找指定元素
        
        # 先寻找指定元素
        while not found:
            # 查看指定节点数据项 是否为 指定元素
            if current.getData() == item:
                # 找到了
                found = True
            # 还未找到
            else:
                # 记录当前节点的前一个节点
                previous = current
                # 更新 下一个节点
                current = current.getNext()
                
        # 一开始就找到了,也就是第一个节点就是要找的元素
        if previous == None:
            self.head = current.getNext()
        # 前一个节点的下一个节点 指向 当前节点的 下一个节点
        else:
            previous.setNext(current.getNext())
    
    
# 我们构建一个空的链表。赋值语句
>>> mylist = UnorderedList()

# 添加元素
>>> mylist.add(31)
>>> mylist.add(77)
>>> mylist.add(17)
>>> mylist.add(93)
>>> mylist.add(26)
>>> mylist.add(54)

# 链表大小
>>> mylist.size()
6

# 作为一个例子，试试调用 search 方法来查找 item 17
>>> mylist.search(17)
True

```

     链表的头指代列表的第一项的第一节点。反过来，
     该节点保存对下一个节点（下一个项）的引用，等等。
     重要的是注意链表类本身不包含任何节点对象。
     相反，它只包含对链接结构中第一个节点的单个引用。 

![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure6.png)
      
      
      由于该链表是无序的，所以新项相对于已经在列表中的其他项的特定位置并不重要。 
      新项可以在任何位置。考虑到这一点，将新项放在最简单的位置是有意义的。
      
      链表结构只为我们提供了一个入口点，即链表的头部。
      所有其他节点只能通过访问第一个节点，然后跟随下一个链接到达。
      这意味着添加新节点的最简单的地方就在链表的头部。 
      
      换句话说，我们将新项作为链表的第一项，现有项将需要链接到这个新项后。
      
      更改新节点的下一个引用以引用旧链表的第一个节点。
      现在，链表的其余部分已经正确地附加到新节点，我们可以修改链表的头以引用新节点。
      这两个步骤不能反，反了之后就找不到原链表了。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure7.png)
      
      
      遍历链表，获取大小
 ![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure9.png)
      
      
      我们检查存储在当前节点中的项是否是我们希望删除的项。
      如果是，found 设置为 True 。
      如果我们没有找到该项，则 previous 和 current 都必须向前移动一个节点。
      同样，这两个语句的顺序是至关重要的。
      previous 必须先将一个节点移动到 current 的位置。
      此时，才可以移动current。
      这个过程通常被称为“英寸蠕动”，因为 previous 必须赶上 current，然后 current 前进。
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure12.png)
      
      一旦 remove 的搜索步骤已经完成，我们需要从链表中删除该节点。 
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure13.png)
      
      有一个特殊情况需要解决。 如果要删除的项目恰好是链表中的第一个项，则 current 将引用链接列表中的第一个节点。
      这也意味着 previous 是 None。 我们先前说过，previous 是一个节点，它的下一个节点需要修改。
      在这种情况下，不是 previous ，而是链表的 head 需要改变
![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8/assets/3.21.%E5%AE%9E%E7%8E%B0%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8%EF%BC%9A%E9%93%BE%E8%A1%A8.figure14.png)
      
      
#### 有序列表抽象数据结构 Ordered List

      如果上面所示的整数列表是有序列表（升序），则它可以写为 17,26,31,54,77和93。
      由于 17 是最小项，它占据第一位置。同样，由于 93 是最大的，它占据最后的位置。
      
      排序通常是升序或降序，并且我们假设列表项具有已经定义的有意义的比较运算。
      许多有序列表操作与无序列表的操作相同。

      OrderedList()   创建一个新的空列表。
                      它不需要参数，并返回一个空列表。
      add(item)       向列表中添加一个新项。
                      它需要 item 作为参数，并不返回任何内容。
                      假定该 item 不在列表中。
      remove(item)    从列表中删除该项。
                      它需要 item 作为参数并修改列表。
                      假设项存在于列表中。
      search(item)    搜索列表中的项目。
                      它需要 item 作为参数，并返回一个布尔值。
      isEmpty()       检查列表是否为空。
                      它不需要参数，并返回布尔值。
      size()          返回列表中的项数。
                      它不需要参数，并返回一个整数。
      index(item)     返回项在列表中的位置。
                      它需要 item 作为参数并返回索引。
                      假定该项在列表中。
      pop()           删除并返回列表中的最后一个项。
                      假设该列表至少有一个项。
      pop(pos)        删除并返回位置 pos 处的项。
                      它需要 pos 作为参数并返回项。
                      假定该项在列表中。


```python
class OrderedList:
    def __init__(self):
        # head 的引用为 None 表示为空链表
        self.head = None
        
    # isEmpty 和size 方法可以与无序列表一样实现，因为它们只处理链表中的节点数量，而不考虑实际项值。
        
    # 只是检查链表头是否是 None 的引用 ,判断链表是否为空
    # 显示了使用引用 None 来表示链接结构的 end 的优点
    def isEmpty(self):
        return self.head == None
        
    # 遍历链表，获取大小
    def size(self):
        # 表头后的第一个节点
        current = self.head
        count = 0
        
        # 非 接地， 未到 链表结尾
        while current != None:
            count = count + 1 # 计数+1
            current = current.getNext() # 迭代，后一个节点
        return count
        
    # 同样，remove 方法将正常工作，因为我们仍然需要找到该项，然后删除它。
    # 删除指定元素
    def remove(self,item):
        # 表头后的第一个节点
        current = self.head
        previous = None
        found = False # 寻找指定元素
        
        # 先寻找指定元素
        while not found:
            # 查看指定节点数据项 是否为 指定元素
            if current.getData() == item:
                # 找到了
                found = True
            # 还未找到
            else:
                # 记录当前节点的前一个节点
                previous = current
                # 更新 下一个节点
                current = current.getNext()
                
        # 一开始就找到了,也就是第一个节点就是要找的元素
        if previous == None:
            self.head = current.getNext()
        # 前一个节点的下一个节点 指向 当前节点的 下一个节点
        else:
            previous.setNext(current.getNext())
            
    # search 和 add，将需要一些修改=====================   
            
    '''
    最重要的需要修改的方法是 add。 
    回想一下，对于无序列表，add 方法可以简单地将新节点放置在链表的头部。
    这是最简单的访问点。 不幸的是，这将不再适用于有序列表。
    需要在现有的有序列表中查找新项所属的特定位置。
    假设我们有由 17,26,54,77 和 93 组成的有序列表，并且我们要添加值31 。
    add 方法必须确定新项属于 26 到 54 之间。
    我们知道，当我们迭代完节点（ current 变为 None）
    或 current 节点的值变得大于我们希望添加的项时，我们就找到了该位置。
    在我们的例子中，看到值 54 我们停止迭代。
   
    '''
            
    # 添加一个元素，需要排序放入合适的位置
    def add(self,item):
        # 两个哥俩，一前一后双指针
        current = self.head # 
        previous = None
        # 提前结束标志
        stop = False
        
        # 遍历找到有序链表中的合适位置
        while current != None and not stop:
            if current.getEats() > item:
                # 找到合适位置了
                # current 节点的值变得大于我们希望添加的项时，我们就找到了该位置。
                # 应当停下来
                stop = True
            else:
                # 还未找到，更新两个 哥俩
                previous = current
                current  = current.getNext()
                
        # 新建一个节点
        temp = Node(item)
        if previous == None:
            # 添加的元素最小，需要放在表头
            # 新节点指向 原 链表头的指向 
            temp.setNext(self.head)
            # 更新链表 头
            self.head = temp
        else:
            # 正常的放在 current 之前，previous 节点之后 
            temp.setNext(current)  # current 之前
            previous.setNext(temp) # previous 节点之后
            
        
        
    # 遍历链表，寻找指定元素
    # 在项不在链表中的情况下，我们可以利用 有序结构 来尽快停止搜索。
    def search(self,item):
        # 表头后的第一个节点
        current = self.head
        found = False # 未找到
        
        # 添加另一个布尔变量======用于是否停止寻找==========
        stop = False
        
        # 未到 链表结尾，且未找到,且未停止
        while current != None and not found and not stop:
            # 该节点 数据项 是否为 指定元素
            if current.getData() == item:
                found = True
            # 遍历下一个节点
            else:
                # 如果发现任何节点包含大于我们正在寻找的项的数据，我们将 stop 设置为 True,不要寻找了
                if current.getData() > item:
                    stop = True # 不用找了
                else:
                    current = current.getNext()
        # 返回 找到与否标志
        return found 
'''
你可能还注意到此实现的性能与早前针对 Python 列表给出的实际性能不同。
这表明链表不是 Python 列表的实现方式。
Python 列表的实际实现基于数组的概念。
'''
```
      
      
有序表中添加新项:

![](https://facert.gitbooks.io/python-data-structure-cn/3.%E5%9F%BA%E6%9C%AC%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/3.23.%E5%AE%9E%E7%8E%B0%E6%9C%89%E5%BA%8F%E5%88%97%E8%A1%A8/assets/3.23.%E5%AE%9E%E7%8E%B0%E6%9C%89%E5%BA%8F%E5%88%97%E8%A1%A8.figure17.png)
      
#### 总结
      线性 数据结构 以 有序的方式保存它们的数据。
      栈是维持 LIFO，后进先出，排序的简单数据结构。
      栈的基本操作是 push，pop和 isEmpty。
      队列是维护 FIFO（先进先出）排序的简单数据结构。
      队列的基本操作是 enqueue，dequeue 和 isEmpty。
      前缀，中缀 和 后缀 都是写表达式的方法。
      栈 对于设计计算解析表达式算法非常有用。
      栈可以提供 反转特性。
      队列可以帮助 构建定时仿真。
      模拟使用 随机数生成器 来创建真实情况，并帮助我们回答“假设”类型的问题。
      Deques 是允许类似 栈 和 队列 的混合行为的 数据结构。
      Deque  的基本操作是 addFront，addRear，removeFront，removeRear 和 isEmpty。
      列表是 项 的集合，其中每个项目保存相对位置。
      链表 实现 保持逻辑顺序，而不需要物理存储要求。
      修改 链表头 是一种特殊情况。

      
# 二、递归算法思想---分而治之----减而治之
      要理解可能难以解决的复杂问题有一个简单的递归解决方案。
      学习如何递归地写出程序。
      理解和应用递归的三个定律。
      将递归理解为一种迭代形式。
      实现问题的递归公式化。
      了解计算机系统如何实现递归。     
      
      递归是一种解决问题的方法，
      将问题分解为更小的子问题，
      直到得到一个足够小的问题可以被很简单的解决。
      通常递归涉及函数调用自身。
      递归允许我们编写优雅的解决方案，解决可能很难编程的问题。

## 计算整数列表和
循环计算
```python
def listsum(numList):
    theSum = 0
    # for循环变量 列表中的每个元素
    for i in numList:
        theSum = theSum + i
    return theSum

print(listsum([1,3,5,7,9]))


```


递归实现
```python
def listsum(numList):
   # 问题规模最小时的情况
   if len(numList) == 1:
        return numList[0]
   # 大问题分解成多个小问题
   else:
        # 函数调用自己，递归调用
        return numList[0] + listsum(numList[1:])

print(listsum([1,3,5,7,9]))
```
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.3.%E8%AE%A1%E7%AE%97%E6%95%B4%E6%95%B0%E5%88%97%E8%A1%A8%E5%92%8C/assets/4.3.%E8%AE%A1%E7%AE%97%E6%95%B4%E6%95%B0%E5%88%97%E8%A1%A8%E5%92%8C.figure2.png)
      
##  递归的三定律
    像阿西莫夫机器人，
    所有递归算法必须服从三个重要的定律：
      1. 递归算法必须具有基本情况。
      2. 递归算法必须改变其状态并向基本情况靠近。
      3. 递归算法必须以递归方式调用自身。
      
    首先，基本情况是算法停止递归的条件。基本情况通常是足够小以直接求解的问题。
         在listsum 算法中，基本情况是长度为 1 的列表。 
    第二定律，我们必须将算法向基本情况的状态改变。状态的改变意味着该算法正在使用的一些数据被修改。
         通常，表示问题的数据在某种程度上变小。
         numList[1:],递归调用后越来越趋近于 长度为1的列表。
    最后的法则是算法必须调用自身。这是递归的定义。递归对于许多新手程序员来说是一个混乱的概念。
         我们用一个函数解决问题，但该 数通过 调用自己解决问题！
         该逻辑不是循环;递归的逻辑是通过将问题分解成 更小 和 更容易 的问题来 解决的优雅表达。
         
## 整数转换为任意进制字符串
      将整数 10 转换为十进制字符串表示为 10，或将其字符串表示为二进制 1010。
      虽然有很多算法来解决这个问题，包括在 栈 部分讨论的算法，但递归的解决方法非常优雅。
      
      不停地除以基底得到余数，将余数串联起来。
        1. 将原始数字减少为一系列单个位数字。
        2. 使用查找将单个位数字数字转换为字符串。
        3. 将单个位字符串连接在一起以形成最终结果。
      
      使用整数除法将 769 除以 10 ，我们得到 76，余数为 9。
      这给了我们两个好的结果。
      首先，余数是小于我们的基数的数字，可以通过查找立即转换为字符串。
      第二，我们得到的商小于原始数字，并让我们靠近具有小于基数的单个数字的基本情况。
      现在我们的工作是将 76 转换为其字符串表示。
      再次，我们使用商和余数分别获得 7 和 6 的结果。
      最后，我们将问题减少到转换 7，我们可以很容易地做到，
      因为它满足 n < base 的基本条件，其中 base = 10。
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.5.%E6%95%B4%E6%95%B0%E8%BD%AC%E6%8D%A2%E4%B8%BA%E4%BB%BB%E6%84%8F%E8%BF%9B%E5%88%B6%E5%AD%97%E7%AC%A6%E4%B8%B2/assets/4.5.%E6%95%B4%E6%95%B0%E8%BD%AC%E6%8D%A2%E4%B8%BA%E4%BB%BB%E6%84%8F%E8%BF%9B%E5%88%B6%E5%AD%97%E7%AC%A6%E4%B8%B2.figure3.png)
      
       其中基底可以换成其他，如2,16等。
       10进制转换到以 2 到 16 之间的任何基数的数串。
       
```python
def toStr(n,base):
   # 转换 哈希表
   convertString = "0123456789ABCDEF"
   # 最小情况
   if n < base:
      return convertString[n]
   # 计算得到的小问题结果 + 子问题
   else:
      # 子问题 原数/基底的商(帝国调用)    小问题结果
      return toStr(n//base,base) + convertString[n%base]

print(toStr(1453,16))

```
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.5.%E6%95%B4%E6%95%B0%E8%BD%AC%E6%8D%A2%E4%B8%BA%E4%BB%BB%E6%84%8F%E8%BF%9B%E5%88%B6%E5%AD%97%E7%AC%A6%E4%B8%B2/assets/4.5.%E6%95%B4%E6%95%B0%E8%BD%AC%E6%8D%A2%E4%B8%BA%E4%BB%BB%E6%84%8F%E8%BF%9B%E5%88%B6%E5%AD%97%E7%AC%A6%E4%B8%B2.figure4.png)
       
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.6.%E6%A0%88%E5%B8%A7%EF%BC%9A%E5%AE%9E%E7%8E%B0%E9%80%92%E5%BD%92/assets/4.6.%E6%A0%88%E5%B8%A7%EF%BC%9A%E5%AE%9E%E7%8E%B0%E9%80%92%E5%BD%92.figure6.png)
       
       
## 栈帧：实现递归
      假设不是将递归调用的结果与来自 convertString 的字符串拼接到 toStr，
      我们修改了算法，以便在进行递归调用之前将字符串入栈。
```python
from pythonds.basic.stack import Stack

rStack = Stack()

def toStr(n,base):
    convertString = "0123456789ABCDEF"
    # 循环 代替递归
    while n > 0:
        # 最小情况
        if n < base:
            # 直接转换原数
            rStack.push(convertString[n])
        # 其他情况，转换 原数/基数 的余数 后压入栈
        else:
            rStack.push(convertString[n % base])
        # 更新，子问题规模
        # 使用原数/基数的 商 更新目标数，
        n = n // base
    # 生成最后的字符串
    res = ""
    while not rStack.isEmpty():
        res = res + str(rStack.pop())
    return res

print(toStr(1453,16))


```
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.6.%E6%A0%88%E5%B8%A7%EF%BC%9A%E5%AE%9E%E7%8E%B0%E9%80%92%E5%BD%92/assets/4.6.%E6%A0%88%E5%B8%A7%EF%BC%9A%E5%AE%9E%E7%8E%B0%E9%80%92%E5%BD%92.figure5.png)
     
     
     
## 可视化递归
    插图的工具是 Python 的 turtle 模块称为 turtle。
    turtle 是 Python 所有版本的标准库，并且非常易于使用
    。比喻很简单。你可以创建一只乌龟，乌龟能前进，后退，左转，右转等。
    乌龟可以让它的尾巴或上或下。当乌龟的尾巴向下，它移动时会画一条线。
    为了增加乌龟的艺术价值，你可以改变尾巴的宽度以及尾巴浸入的墨水的颜色。  
    
    我们将使用 turtle 模块递归绘制螺旋。
    
    导入 turtle 模块后，我们创建一个乌龟。当乌龟被创建时，它也创建一个窗口来绘制。
    接下来我们定义 drawSpiral 函数。
    这个简单函数的基本情况是当我们想要绘制的线的长度（由 len 参数给出）减小到零或更小时。
    如果线的长度大于零，我们让乌龟以 len 单位前进，然后向右转 90 度。
    当我们再次调用 drawSpiral 并缩短长度时递归。
    结束时，会调用函数 myWin.exitonclick()，这是一个方便的缩小窗口的方法，
    使乌龟进入等待模式，直到你单击窗口，然后程序清理并退出。
      
```python

import turtle
# 创建一个乌龟
myTurtle = turtle.Turtle()
# 创建一个窗口来绘制
myWin = turtle.Screen()

# 想要绘制的线的长度
def drawSpiral(myTurtle, lineLen):
    if lineLen > 0:
        # 让乌龟以 len 单位前进
        myTurtle.forward(lineLen)
        # 然后向右转 90 度
        myTurtle.right(90)
        # 递归调用本身，并缩短长度
        drawSpiral(myTurtle,lineLen-5)
# 
drawSpiral(myTurtle,100)
# 缩小窗口的方法，使乌龟进入等待模式，直到你单击窗口，然后程序清理并退出
myWin.exitonclick()

```
      
      
    绘制一个分形树。分形来自数学的一个分支，并且与递归有很多共同之处。
    分形的定义是，当你看着它时，无论你放大多少，分形有相同的基本形状。
    大自然的一些例子是大陆的海岸线，雪花，山脉，甚至树木或灌木。
    这些自然现象中的许多的分形性质使得程序员能够为计算机生成的电影生成非常逼真的风景。
    在我们的下一个例子中，将生成一个分形树。   
       
    一棵树是树干，一棵较小的树向右走，另一棵较小的树向左走。
    如果你用递归的思想考虑这个定义，这意味着我们将树的递归定义应用到较小的左树和右树。   
    
    
```python
import turtle

def tree(branchLen,t):
    if branchLen > 5:
        # 前进 
        t.forward(branchLen)
        
        # 乌龟向右转 20 度之后立即进行递归调用,这是右树。
        t.right(20)
        tree(branchLen-15,t)
        
        # 左转 40（需要撤消原来的向右转 20 度，然后再向左转 20 度），以绘制左树。
        t.left(40)
        tree(branchLen-10,t)
        
        # 每次我们对树进行递归调用时，我们从 branchLen 参数中减去一些量; 这是为了确保递归树越来越小。
        
        # 在右转20度，摆正方向
        t.right(20)
        
        # 对应一开始的前进，这里后退
        t.backward(branchLen)
        
def main():
    # 创建一个乌龟
    t = turtle.Turtle()
    # 创建一个窗口来绘制
    myWin = turtle.Screen()
    t.left(90)
    t.up()
    t.backward(100)
    t.down()
    t.color("green")
    tree(75,t)
    myWin.exitonclick()
    
main()
    

```
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.7.%E4%BB%8B%E7%BB%8D%EF%BC%9A%E5%8F%AF%E8%A7%86%E5%8C%96%E9%80%92%E5%BD%92/assets/4.7.%E4%BB%8B%E7%BB%8D%EF%BC%9A%E5%8F%AF%E8%A7%86%E5%8C%96%E9%80%92%E5%BD%92.ac1.png)
      
      
###  谢尔宾斯基三角形
      
      谢尔宾斯基三角形阐明了三路递归算法。用手绘制谢尔宾斯基三角形的过程很简单。 
      从一个大三角形开始。通过连接每一边的中点，将这个大三角形分成四个新的三角形。
      忽略刚刚创建的中间三角形，对三个小三角形中的每一个应用相同的过程。 
      每次创建一组新的三角形时，都会将此过程递归应用于三个较小的角三角形。 
      如果你有足够的铅笔，你可以无限重复这个过程。
      
      基本情况被任意设置为我们想要将三角形划分成块的次数。
      有时我们把这个数字称为分形的“度”。 
      每次我们进行递归调用时，我们从度中减去 1，直到 0。
      当我们达到 0 度时，我们停止递归。
      
```python
import turtle

# 绘制三角形
def drawTriangle(points,color,myTurtle):
    # 填充颜色
    myTurtle.fillcolor(color)
    myTurtle.up()
    myTurtle.goto(points[0][0],points[0][1])
    myTurtle.down()
    # 使用 begin_fill 和 end_fill 方法绘制填充一个三角形
    myTurtle.begin_fill()
    myTurtle.goto(points[1][0],points[1][1])
    myTurtle.goto(points[2][0],points[2][1])
    myTurtle.goto(points[0][0],points[0][1])
    myTurtle.end_fill()

# 获取指定两点连线的 中点坐标
def getMid(p1,p2):
    return ( (p1[0] + p2[0]) / 2, (p1[1] + p2[1]) / 2)
    
# 绘制谢尔宾斯基三角形
def sierpinski(points,degree,myTurtle):
    colormap = ['blue','red','green','white','yellow',
                'violet','orange']
    # 绘制最大的带颜色的中间的三角形
    drawTriangle(points,colormap[degree],myTurtle)
    # 分割次数
    if degree > 0:
        # 绘制上三角形
        sierpinski([points[0],
                        getMid(points[0], points[1]),
                        getMid(points[0], points[2])],
                   degree-1, myTurtle)
        # 绘制左三角形
        sierpinski([points[1],
                        getMid(points[0], points[1]),
                        getMid(points[1], points[2])],
                   degree-1, myTurtle)
       # 绘制右三角形
        sierpinski([points[2],
                        getMid(points[2], points[1]),
                        getMid(points[0], points[2])],
                   degree-1, myTurtle)

def main():
    # 创建一个乌龟
    myTurtle = turtle.Turtle()
    # 创建一个窗口来绘制
    myWin = turtle.Screen()
    # 大三角形三个顶点坐标
    myPoints = [[-100,-50],[0,100],[100,-50]]
    # 绘制谢尔宾斯基三角形，中间为分割次数
    sierpinski(myPoints,3,myTurtle)
    myWin.exitonclick()

main()


```
      
### 汉诺塔游戏
      在开始的时候，牧师们被给予三根杆和一堆 64 个金碟，每个盘比它下面一个小一点。
      他们的任务是将所有 64 个盘子从三个杆中一个转移到另一个。
      有两个重要的约束，它们一次只能移动一个盘子，并且它们不能在较小的盘子顶部上放置更大的盘子。
      
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.10.%E6%B1%89%E8%AF%BA%E5%A1%94%E6%B8%B8%E6%88%8F/assets/4.10.%E6%B1%89%E8%AF%BA%E5%A1%94%E6%B8%B8%E6%88%8F.figure1.png)
      
    步骤：
      1. 借助目标杆(toPole) 将 起始杆(fromPole)上方height-1 个盘子 移动到中间杆(withPole)。
      2. 将起始杆(fromPole)上 剩余的盘子 移动到 目标杆(toPole)。
      3. 借助起始杆(fromPole) 将中间杆(withPole)上 height-1 个盘子 移动到目标杆(toPole)。
      
    只要我们遵守规则，较大的盘子保留在栈的底部，我们可以使用递归的三个步骤，处理任何更大的盘子。
    上面概要中唯一缺失的是识别基本情况。最简单的汉诺塔是一个盘子的塔。
    在这种情况下，我们只需要将一个盘子移动到其最终目的地。 
    一个盘子的塔将是我们的基本情况。 
    此外，上述步骤通过在步骤1和3中减小塔的高度，使我们趋向基本情况。   
    
```python
# 具体的执行操作，从 fp 到 tp
def moveDisk(fp,tp):
    print("moving disk from",fp,"to",tp)


def moveTower(height,fromPole, toPole, withPole):
    if height >= 1:
        # 1. 利用 toPole  从 fromPole 移动 height-1个盘子 到 withPole
        moveTower(height-1,fromPole,withPole,toPole)
        # 2. 把 fromPole 上的一个盘子 移动 到 toPole
        moveDisk(fromPole,toPole)
        # 3. 再利用fromPole 从withPole 移动 剩下的 height-1 个盘子 到 toPole 上
        moveTower(height-1,withPole,toPole,fromPole)
'''
如果要明确地跟踪盘子，需要使用三个 Stack 对象，每个杆一个。 
Python 提供了我们需要调用的隐含的栈。
'''

```
        
### 探索迷宫
      解决的问题是帮助我们的乌龟在虚拟迷宫中找到出路。

![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.11.%E6%8E%A2%E7%B4%A2%E8%BF%B7%E5%AE%AB/assets/4.11.%E6%8E%A2%E7%B4%A2%E8%BF%B7%E5%AE%AB.figure2.png)
      
      我们假设我们的迷宫被分成“正方形”。迷宫的每个正方形是开放的或被一段墙壁占据。
      乌龟只能通过迷宫的空心方块。 如果乌龟撞到墙上，它必须尝试不同的方向。
      乌龟将需要一个程序，以找到迷宫的出路。这里是过程：
       1. 从我们的起始位置，我们将首先尝试 向北(上)一格，然后从那里 递归 地尝试我们的程序。
       2. 如果我们通过尝试 向北(上) 作为 第一步 没有成功，我们将向南(下)一格，并 递归 地重复我们的程序。
       3. 如果向南(下)也不行，那么我们将尝试 向西(左) 一格，并 递归 地重复我们的程序。
       4. 如果北，南和西都没有成功，则应用程序从当前位置 递归 向东(右)。
       5. 如果这些方向都没有成功，那么没有办法离开迷宫，我们失败。  
      
      但如果北面被一堵墙阻挡，我们必须看看程序的下一步，并试着向南。
      不幸的是，向南使我们回到原来的起点。
      如果我们从那里再次应用递归过程，我们将又回到向北一格，并陷入无限循环。
      所以，我们必须有一个策略来记住我们去过哪。
      
      我们假设有一袋面包屑可以撒在我们走过的路上。
      如果我们沿某个方向迈出一步，发现那个位置上已经有面包屑，
      我们应该立即后退并尝试程序中的下一个方向。
      
      有四种基本情况要考虑：
       1. 乌龟撞到了墙。由于这一格被墙壁占据，不能进行进一步的探索。
       2. 乌龟找到一个已经探索过的格。我们不想继续从这个位置探索，否则会陷入循环。
       3. 我们发现了一个外边缘，没有被墙壁占据。换句话说，我们发现了迷宫的一个出口。
       4. 我们探索了一格在四个方向上都没有成功。
      
      地图表示：
       我们将使用 turtle 模块来绘制和探索我们的迷宫，以使我们看到这个算法的功能。
       迷宫对象 将提供以下方法让我们在编写搜索算法时使用：

        1. __init__ 读取迷宫的数据文件，初始化迷宫的内部表示，并找到乌龟的起始位置。
        2. drawMaze 在屏幕上的一个窗口中绘制迷宫。
        3. updatePosition 更新迷宫的内部表示，并更改窗口中乌龟的位置。
        4. isExit 检查当前位置是否是迷宫的退出位置。
       Maze 类还重载索引运算符 [] ，以便我们的算法可以轻松访问任何特定格的状态。
      
搜索路径算法：
```python
# 迷宫对象 maze ，起始行 startRow  和 起始列 startColumn
# 这很重要，因为作为递归函数，搜索在每次递归调用时开始。
def searchFrom(maze, startRow, startColumn):
    # 为了可视化算法，以便你可以看到乌龟如何探索通过迷宫。
    maze.updatePosition(startRow, startColumn)
    # 检查四种基本情况中的前三种
    #  1. 乌龟是否碰到墙，返回 false
    if maze[startRow][startColumn] == OBSTACLE :
        return False
    #  2. 找到一个已经探索过的格
    if maze[startRow][startColumn] == TRIED:
        return False
    # 3. 乌龟有没有到达出口，返 True
    if maze.isExit(startRow,startColumn):
        # 到达出口了，更新地图
        maze.updatePosition(startRow, startColumn, PART_OF_PATH)
        return True
    # 更新地图显示，该块格子已被探索
    maze.updatePosition(startRow, startColumn, TRIED)

    # 如果这些条件都不为真，则我们继续递归搜索。
    
    # 递归步骤中有四个对 searchFrom 的递归调用(上下左右四个方向)
    # 很难预测将有多少个递归调用，因为它们都由 or 语句连接。
    # 如果对 searchFrom 的第一次调用返回 True ，则不需要最后三个调用。
    found = searchFrom(maze, startRow-1, startColumn) or \
            searchFrom(maze, startRow+1, startColumn) or \
            searchFrom(maze, startRow, startColumn-1) or \
            searchFrom(maze, startRow, startColumn+1)
            
    # 如果其中一个方向 通过，该点是一条路
    if found:
        maze.updatePosition(startRow, startColumn, PART_OF_PATH)
        
    else:
        # 如果所有四个递归调用返回 False，那么认为是一个死胡同 DEAD_END 。
        maze.updatePosition(startRow, startColumn, DEAD_END)
    return found


```

drawMaze 方法使用这个内部表示在屏幕上绘制迷宫的初始视图。
      
     示例地图文件数据文件：
      ++++++++++++++++++++++
      +   +   ++ ++     +
      + +   +       +++ + ++
      + + +  ++  ++++   + ++
      +++ ++++++    +++ +  +
      +          ++  ++    +
      +++++ ++++++   +++++ +
      +     +   +++++++  + +
      + +++++++      S +   +
      +                + +++
      ++++++++++++++++++ +++   
      

地图内部数据 列表的列表：

      [ ['+','+','+','+',...,'+','+','+','+','+','+','+'],
        ['+',' ',' ',' ',...,' ',' ',' ','+',' ',' ',' '],
        ['+',' ','+',' ',...,'+','+',' ','+',' ','+','+'],
        ['+',' ','+',' ',...,' ',' ',' ','+',' ','+','+'],
        ['+','+','+',' ',...,'+','+',' ','+',' ',' ','+'],
        ['+',' ',' ',' ',...,'+','+',' ',' ',' ',' ','+'],
        ['+','+','+','+',...,'+','+','+','+','+',' ','+'],
        ['+',' ',' ',' ',...,'+','+',' ',' ','+',' ','+'],
        ['+',' ','+','+',...,' ',' ','+',' ',' ',' ','+'],
        ['+',' ',' ',' ',...,' ',' ','+',' ','+','+','+'],
        ['+','+','+','+',...,'+','+','+',' ','+','+','+']]
      
      
地图实现

```python
class Maze:
    # 读取 文件数据，生成 数据化地图
    def __init__(self,mazeFileName):
        # 地图总大小
        rowsInMaze = 0
        columnsInMaze = 0
        # 地图内部形式，列表的列表
        self.mazelist = []
        # 打开地图文件
        mazeFile = open(mazeFileName,'r')
        rowsInMaze = 0 # 行id=====
        # 读取每一行数据
        for line in mazeFile:
            rowList = []
            col = 0    # 列 id=====
            # 每一行 的各一个格子数据
            for ch in line[:-1]:
                rowList.append(ch) # 一行数据
                # 起点====
                # 通过使用 + 字符表示墙壁，空格表示空心方块，并使用字母 S 表示起始位置
                if ch == 'S':
                    self.startRow = rowsInMaze #行
                    self.startCol = col        #列
                # 列id ++
                col = col + 1
            # 行id++  ====
            rowsInMaze = rowsInMaze + 1
            # 行数据 列表
            self.mazelist.append(rowList)
            
            # 列数量
            columnsInMaze = len(rowList)
        # 迷宫的内部表示是列表的列表。 mazelist 实例变量的每一行也是一个列表。
        self.rowsInMaze = rowsInMaze # 总行数
        self.columnsInMaze = columnsInMaze# 总列数
        # 中点 坐标 偏移量
        self.xTranslate = -columnsInMaze/2
        self.yTranslate = rowsInMaze/2
        self.t = Turtle(shape='turtle')
        setup(width=600,height=600)
        # 世界坐标系
        setworldcoordinates(-(columnsInMaze-1)/2 - 0.5,
                            -(rowsInMaze-1)/2    - 0.5,
                            (columnsInMaze-1)/2  + 0.5,
                            (rowsInMaze-1)/2     + 0.5)
                            
    # drawMaze 方法使用这个内部表示在屏幕上绘制迷宫的初始视图 
    def drawMaze(self):
        # 每一行
        for y in range(self.rowsInMaze):
            # 每一列
            for x in range(self.columnsInMaze):
                # 数据内容为障碍物的话，显示为格子
                if self.mazelist[y][x] == OBSTACLE:
                    self.drawCenteredBox(x+self.xTranslate,
                                           -y+self.yTranslate,
                                           'tan')
        self.t.color('black','blue')

    def drawCenteredBox(self,x,y,color):
        tracer(0)
        self.t.up()
        self.t.goto(x - 0.5, y - 0.5)
        self.t.color('black',color)
        self.t.setheading(90)
        self.t.down()
        # 填充颜色==========
        self.t.begin_fill()
        for i in range(4):
            self.t.forward(1)
            self.t.right(90)
        self.t.end_fill()
        
        update()
        tracer(1)
        
    # updatePosition 方法使用两个辅助方法moveTurtle 和 dropBreadCrumb 来更新屏幕上的视图。
    def moveTurtle(self,x,y):
        self.t.up()
        # 朝向
        self.t.setheading(self.t.towards(x+self.xTranslate,
                                           -y+self.yTranslate))
        # 移动到
        self.t.goto(x+self.xTranslate,-y+self.yTranslate)

    def dropBreadcrumb(self,color):
        self.t.dot(color)
        
    # updatePosition 方法使用相同的内部表示来查看乌龟是否遇到了墙。
    # 它还用 . 或 - 更新内部表示，以表示乌龟已经访问了特定格子或者格子是死角。
    def updatePosition(self,row,col,val=None):
        if val:
            self.mazelist[row][col] = val
        # updatePosition 方法使用两个辅助方法moveTurtle 和 dropBreadCrumb 来更新屏幕上的视图。
        self.moveTurtle(col,row)
        
        # 路径
        if val == PART_OF_PATH:
            color = 'green'
        # 障碍物
        elif val == OBSTACLE:
            color = 'red'
        # 已经探索过
        elif val == TRIED:
            color = 'black'
        # 死胡同
        elif val == DEAD_END:
             color = 'red'
        else:
             color = None

        if color:
             self.dropBreadcrumb(color)
             
    # 最后，isExit 方法使用乌龟的当前位置来检测退出条件。
    # 退出条件是当乌龟已经到迷宫的边缘时，即行零或列零，或者在最右边列或底部行         
    def isExit(self,row,col):
        return (row == 0 or
                row == self.rowsInMaze-1 or
                col == 0 or
                col == self.columnsInMaze-1 )
                
    # 重载索引运算符 =======================
    # 以便我们的算法可以轻松访问任何特定格的状
    def __getitem__(self,idx):
        return self.mazelist[idx]
     
```
      
### 动态规划
      计算机科学中的许多程序是为了优化一些值而编写的。
      动态规划 是这些类型的优化问题的一个策略。
      
      找零问题：
      如果金额不匹配，我们有几个选项。
      我们想要的是 
      最低一个一分钱 加上  原始金额减去一分钱   所需的硬币数量，
      或者一个 5 美分加上  原始金额减去 5 美分  所需的硬币数量，
      或者一个 10 美分加上 原始金额减去 10 美分 所需的硬币数量，
      或者一个 250 美分加上 原始金额减去 30 美分 所需的硬币数量。
      因此，需要对原始金额找零硬币数量可以根据下式计算：
![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.12.%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92/assets/4.12.%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92.1.png)

贪婪算法，方法太低效，很多重复工作：
```python
# coinValueList 可找零 币值列表 change找零所需总钱数
def recMC(coinValueList,change):
    minCoins = change
    # 最小情况：找零总钱数 正好在 可找零 币值列表中
    if change in coinValueList:
        return 1 # 返回1个钱币
    # 其他情况
    else:
        for i in [c for c in coinValueList if c <= change]:
            # 一张各种币值 + 剩余找零所需 张数(递归)
            numCoins = 1 + recMC(coinValueList,change-i)
            # 保留 这几种 策略中 最好的，找零钱币张数最少
            if numCoins < minCoins:
                minCoins = numCoins
    return minCoins

print(recMC([1,5,10,25],63))

```

![](https://facert.gitbooks.io/python-data-structure-cn/4.%E9%80%92%E5%BD%92/4.12.%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92/assets/4.12.%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92.figure5.png)
      
      计算 找零15的 至少计算了3次，太多重复

      减少我们工作量的关键是记住一些过去的结果，这样我们可以避免重新计算我们已经知道的结果。
      一个简单的解决方案是将最小数量的硬币的结果存储在表中。
      然后在计算新的最小值之前，我们首先检查表，看看结果是否已知。
      如果表中已有结果，我们使用表中的值，而不是重新计算。
      
贪婪+查表算法(使用缓存技术，记忆)：

```python
def recDC(coinValueList,change,knownResults):
   minCoins = change
   if change in coinValueList:
      # 已知找零结果 表
      knownResults[change] = 1
      return 1
   # 查看该找零 结果 是否已经在 结果记录表中
   elif knownResults[change] > 0:
      return knownResults[change]
   #
   else:
       for i in [c for c in coinValueList if c <= change]:
         # 递归计算最小值
         numCoins = 1 + recDC(coinValueList, change-i,
                              knownResults)
         # 几种找零方案中 最好的
         if numCoins < minCoins:
            minCoins = numCoins
            # 记录该 找零方案 的结果======
            knownResults[change] = minCoins
   return minCoins

# 多传入一个，找零结果列表
print(recDC([1,5,10,25],63,[0]*64))


```
      
用一个动态规划算法来解决我们的找零问题：

```python
# 有三个参数：一个有效硬币值的列表，我们要求的找零额，以及一个包含每个找零值所需最小硬币数量的列表。 
# 当函数完成时，minCoins 将包含从 0 到找零值的所有值的解。
def dpMakeChange(coinValueList,change,minCoins):
    # 遍历每一个找零值
    for cents in range(change+1):
        coinCount = cents
        # 遍历 每一种可找零的 币值，考虑使用所有可能的硬币对指定的金额进行找零。
        for j in [c for c in coinValueList if c <= cents]:
            # 使用币值j + 找零cents-j 的最小找零数量
            if minCoins[cents-j] + 1 < coinCount:
                coinCount = minCoins[cents-j] + 1
        # 该找零 所需最小硬币数量
        minCoins[cents] = coinCount
    return minCoins[change]
```

记录找零币值：
```python
# 比上述函数 多返回一个 找零币值列表
def dpMakeChange(coinValueList,change,minCoins,coinsUsed):
    # 遍历每一个找零值
    for cents in range(change+1):
        coinCount = cents
        newCoin = 1
        # 遍历 每一种可找零的 币值，考虑使用所有可能的硬币对指定的金额进行找零。
        for j in [c for c in coinValueList if c <= cents]:
            if minCoins[cents-j] + 1 < coinCount:
                coinCount = minCoins[cents-j]+1
                # 使用该币值
                newCoin = j
        # 更新 结果方案 列表
        minCoins[cents] = coinCount
        # 记录使用的 币值
        coinsUsed[cents] = newCoin
     return minCoins[change]

# 打印使用的 币值
def printCoins(coinsUsed,change):
   coin = change
   while coin > 0:
      thisCoin = coinsUsed[coin]
      print(thisCoin)
      coin = coin - thisCoin

def main():
    # 需要找零 总数
    amnt = 63
    # 可找零币值列表
    clist = [1,5,10,21,25]
    # 找零使用列表
    coinsUsed = [0]*(amnt+1)
    # 找零结果记录
    coinCount = [0]*(amnt+1)

    print("Making change for",amnt,"requires")
    print(dpMakeChange(clist,amnt,coinCount,coinsUsed),"coins")
    print("They are:")
    printCoins(coinsUsed,amnt)
    print("The used list is as follows:")
    print(coinsUsed)

main()

```

### 总结
      在本章中，我们讨论了几个递归算法的例子。 
      选择这些算法来揭示几个不同的问题，其中递归是一种有效的问题解决技术。 
      本章要记住的要点如下：
        1. 所有递归算法都必须具有基本情况。
        2. 递归算法必须改变其状态并朝基本情况发展。
        3. 递归算法必须调用自身（递归）。
        4. 递归在某些情况下可以代替迭代。
        5. 递归算法通常可以自然地映射到你尝试解决的问题的表达式。
        6. 递归并不总是答案。有时，递归解决方案可能比迭代算法在计算上更昂贵。

    
# 排序和搜索====================================
      目标
       1. 能够解释和实现顺序查找和二分查找。
       2. 能够解释和实现选择排序，冒泡排序，归并排序，快速排序，插入排序和希尔排序。
       3. 理解哈希作为搜索技术的思想。
       4. 引入映射抽象数据类型。
       5. 使用哈希实现 Map 抽象数据类型。

## 1. 顺序查找
      从列表中的第一个项目开始，我们按照基本的顺序排序，
      简单地从一个项移动到另一个项，直到找到我们正在寻找的项或遍历完整个列表。
      如果我们遍历完整个列表，则说明正在搜索的项不存在。
      
      项列表不以任何方式排序。
      项随机放置到列表中。
      换句话说，项在列表任何位置的概率是一样的。
      
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.3.%E9%A1%BA%E5%BA%8F%E6%9F%A5%E6%89%BE/assets/5.3.%E9%A1%BA%E5%BA%8F%E6%9F%A5%E6%89%BE.figure1.png)

无序列表的顺序查找：

      最好 最坏 平均
      1    n   n/2
      1    n   n
      
```python
# 一个列表 和 我们正在寻找的项作为参数
def sequentialSearch(alist, item):
    pos = 0 # 元素位置
    found = False # 是否找到标志
    # 未遍历到最后且未找到
    while pos < len(alist) and not found:
        # 如果我们发现列表中的项，则赋值为 True。
        if alist[pos] == item:
            found = True
        # 否者检测下一个位置
        else:
            pos = pos+1
    # 返回一个是否存在的布尔值
    return found

testlist = [1, 2, 32, 8, 17, 19, 42, 13, 0]
print(sequentialSearch(testlist, 3))
print(sequentialSearch(testlist, 13))

```


有序列表的顺序查找：

      如果我们正在寻找的项存在于列表中，它在 n 个位置中的概率依旧相同。
      我们仍然会有相同数量的比较来找到该项。然而，如果该项不存在，则有一些优点。
      1   n   n/2
      1   n   n/2
      
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.3.%E9%A1%BA%E5%BA%8F%E6%9F%A5%E6%89%BE/assets/5.3.%E9%A1%BA%E5%BA%8F%E6%9F%A5%E6%89%BE.figure2.png)
      
      
```python
# 一个列表 和 我们正在寻找的项作为参数
def orderedSearch(alist, item):
    pos = 0 # 元素位置
    found = False # 是否找到标志
    stop = False  # 停止查找标志
    # 未遍历到最后且未找到
    while pos < len(alist) and not found and not stop:
        # 如果我们发现列表中的项，则赋值为 True。
        if alist[pos] == item:
            found = True
        # 否者检测下一个位置
        else:
            if alist[pos] > item:
                # 算法不必继续查看所有项,它可以立即停止.
                stop = True
            else:
                pos = pos+1
    # 返回一个是否存在的布尔值
    return found
    
testlist = [0, 1, 2, 8, 13, 17, 19, 32, 42,]
print(orderedSequentialSearch(testlist, 3))
print(orderedSequentialSearch(testlist, 13))

```


## 2. 有序列表的 二分查找\折半查找  log(n)
      分而治之，分治思想
      有序列表对于我们的比较是很有用的。
      在顺序查找中，当我们与第一个项进行比较时，如果第一个项不是我们要查找的，则最多还有 n-1 个项目。
      二分查找从中间项开始，而不是按顺序查找列表。
      如果该项是我们正在寻找的项，我们就完成了查找。
      如果它不是，我们可以使用列表的有序性质来消除剩余项的一半。
      如果我们正在查找的项大于中间项，就可以消除中间项以及比中间项小的一半元素。
      如果该项在列表中，肯定在大的那半部分。
      
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.4.%E4%BA%8C%E5%88%86%E6%9F%A5%E6%89%BE/assets/5.4.%E4%BA%8C%E5%88%86%E6%9F%A5%E6%89%BE.figure3.png)

循环版本：
```python
def binarySearch(alist, item):
    first = 0            # 起始
    last  = len(alist)-1 # 结束
    found = False        # 是否找到标志
    
    # 未遍历到最后且未找到
    while first <= last and not found:
        # 当前区域的二分区域
        midpoint = (first + last) // 2 # 整除
        # 如果我们发现列表中的项，则赋值为 True。
        if alist[midpoint] == item:
            found = True
        # 否者检测下一个位置
        else:
            if item < alist[midpoint]:
                # 查找元素较小，在左半区间
                last = midpoint-1
            else:
                # 查找元素较大，在右半区间
                first = midpoint+1
                
    # 返回一个是否存在的布尔值
    return found

testlist = [0, 1, 2, 8, 13, 17, 19, 32, 42,]
print(binarySearch(testlist, 3))
print(binarySearch(testlist, 13))

```

      这个算法是分而治之策略的一个很好的例子。
      分和治意味着我们将问题分成更小的部分，以某种方式解决更小的部分，
      然后重新组合整个问题以获得结果。 
      当我们执行列表的二分查找时，我们首先检查中间项。
      如果我们正在搜索的项小于中间项，我们可以简单地对原始列表的左半部分执行二分查找。
      同样，如果项大，我们可以执行右半部分的二分查找。
      无论哪种方式，都是递归调用二分查找函数。
      
递归版本:
```python
def binarySearch(alist, item):
    # 递归最小情况是，列表长度变为0
    if len(alist) == 0:
        return False
    else:
        midpoint = len(alist) // 2 # 长度整除
        # 如果我们发现列表中的项，则赋值为 True。
        if alist[midpoint] == item:
            return True
        # 否者检测下一个位置
        else:
            if item < alist[midpoint]:
                # 递归查找 左半区间
                return binarySearch(alist[:midpoint], item)
            else:
                # 递归查找 右半区间
                return binarySearch(alist[midpoint+1:], item)

testlist = [0, 1, 2, 8, 13, 17, 19, 32, 42,]
print(binarySearch(testlist, 3))
print(binarySearch(testlist, 13))


```
      
      
## 3. Hash查找
      哈希表 是以一种容易找到它们的方式存储的项的集合。
      哈希表的每个位置，通常称为一个槽，可以容纳一个项，并且由从 0 开始的整数值命名。
      例如，我们有一个名为 0 的槽，名为 1 的槽，名为 2 的槽，以上。
      最初，哈希表不包含项，因此每个槽都为空。
      我们可以通过使用列表来实现一个哈希表，每个元素初始化为None 。
      Figure 4 展示了大小 m = 11 的哈希表。换句话说，在表中有 m 个槽，命名为 0 到 10。
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.5.Hash%E6%9F%A5%E6%89%BE/assets/5.5.Hash%E6%9F%A5%E6%89%BE.figure4.png)

      项和该项在散列表中所属的槽之间的映射被称为 hash 函数。
      hash 函数将接收集合中的任何项，并在槽名范围内（0和 m-1之间）返回一个整数。
      假设我们有整数项 54,26,93,17,77 和 31 的集合。
      我们的第一个 hash 函数，有时被称为 余数法 ，只需要一个项并将其除以表大小，
      返回剩余部分作为其散列值（h(item) = item％11）。 
      注意，这种余数方法（模运算）通常以某种形式存在于所有散列函数中，因为结果必须在槽名的范围内。
      
      一旦计算了哈希值，我们可以将每个项插入到指定位置的哈希表中，如 Figure 5 所示。
      注意，11 个插槽中的 6 个现在已被占用。
      这被称为负载因子，通常表示为 λ=项数/表大小, 在这个例子中，λ = 6/11 。 

![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.5.Hash%E6%9F%A5%E6%89%BE/assets/5.5.Hash%E6%9F%A5%E6%89%BE.figure5.png)
      
      现在，当我们要搜索一个项时，我们只需使用哈希函数来计算项的槽名称，然后检查哈希表以查看它是否存在。
      该搜索操作是 O(1)O(1)，因为需要恒定的时间量来计算散列值，然后在该位置索引散列表。
      如果一切都正确的话，我们已经找到了一个恒定时间搜索算法。
      
      你可能已经看到，只有每个项映射到哈希表中的唯一位置，这种技术才会起作用。 
      例如，如果项 44 是我们集合中的下一个项，则它的散列值为0（44％11 == 0）。
      因为 77 的哈希值也是 0，我们会有一个问题。根据散列函数，两个或更多项将需要在同一槽中。
      
      这种现象被称为碰撞（它也可以被称为“冲突”）。
      显然，冲突使散列技术产生了问题。
      
      
      我们的目标是创建一个 散列hash函数，最大限度地减少冲突数，易于计算，并均匀分布在哈希表中的项。
      有很多常用的方法来扩展简单余数法。
      
      1. 分组求和法
        将项划分为相等大小的块（最后一块可能不是相等大小）。然后将这些块加在一起以求出散列值。
        例如，如果我们的项是电话号码 436-555-4601，我们将取出数字，并将它们分成2位数（43,65,55,46,01）。
        43 + 65 + 55 + 46 + 01，我们得到 210。
        我们假设哈希表有 11 个槽，那么我们需要除以 11 。
        在这种情况下，210％11 为 1，因此电话号码 436-555-4601 散列到槽 1 。
        
        一些分组求和法会在求和之前 每隔一个项 进行 反转。
                                65->56    46->64
        对于上述示例，我们得到 43 + 56 + 55 + 64 + 01 = 219，其给出 219％11 = 10 。
        
      2. 平方取中法
        我们首先 对该项 平方，然后提取一部分数字结果。
        例如，如果项是 44，我们将首先计算 44^2 = 1,936 。
        通过 提取 中间两个数字 93 ，我们得到 5（93％11）
        
      3. 为基于字符的项（如字符串）创建哈希函数

            词 cat 可以被认为是 ascii 值的序列。
            >>> ord('c')
            99
            >>> ord('a')
            97
            >>> ord('t')
            116
        我们可以获取这三个 ascii 值，将它们相加，并使用余数方法获取散列值.
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.5.Hash%E6%9F%A5%E6%89%BE/assets/5.5.Hash%E6%9F%A5%E6%89%BE.figure6.png)
      
```python
def hash(astring, tablesize):
    sum = 0
    for pos in range(len(astring)):
        sum = sum + ord(astring[pos])

    return sum%tablesize

```
      
      有趣的是，当使用此散列函数时，字符串总是返回相同的散列值。
      为了弥补这一点，我们可以使用字符的位置作为权重。
      下图展示了使用位置值作为加权因子的一种可能的方式。  
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.5.Hash%E6%9F%A5%E6%89%BE/assets/5.5.Hash%E6%9F%A5%E6%89%BE.figure7.png)
      
      你可以思考一些其他方法来计算集合中项的散列值。
      重要的是要记住，哈希函数必须是高效的，以便它不会成为存储和搜索过程的主要部分。
      如果哈希函数太复杂，则计算槽名称的程序要比之前所述的简单地进行基本的顺序或二分搜索更耗时。 
      这将打破散列的目的。
      
      
      
### 实现 map 抽象数据类型
      字典是一种关联数据类型，你可以在其中存储键-值对。
      该键用于查找关联的值。
      
      map 抽象数据类型定义如下。该结构是键与值之间的关联的无序集合。
      map 中的键都是唯一的，因此键和值之间存在一对一的关系。
      操作如下。
       1. Map() 创建一个新的 map 。它返回一个空的 map 集合。
       2. put(key，val) 向 map 中添加一个新的键值对。如果键已经在 map 中，那么用新值替换旧值。
       3. get(key) 给定一个键，返回存储在 map 中的值或 None。
       4. del 使用 del map[key] 形式的语句从 map 中删除键值对。
       5. len() 返回存储在 map 中的键值对的数量。
       6. in 返回 True 对于 key in map 语句，如果给定的键在 map 中，否则为False。

      我们可以使用具有顺序或二分查找的列表，
      但是使用如上所述的哈希表将更好，
      因为查找哈希表中的项可以接近 O(1) 性能.
      
实现
```python
class HashTable:
    def __init__(self):
        self.size = 11 # 哈希表的初始大小已经被选择为 11
        # 重要的是，大小是质数，使得冲突解决算法可以尽可能高效。
        self.slots = [None] * self.size # slots列表将保存键 key: 项
        # 将 键列表 视为 哈希表。
        self.data = [None] * self.size  # data 的并行列表将保存数据值 value
        # 当我们查找一个键时，data 列表中的相应位置将保存相关的数据值。

# 把 键key:值data 存入 map=====================
def put(self,key,data):
    # 对 键值 执行哈希函数，计算元素位置
    hashvalue = self.hashfunction(key,len(self.slots))
    # 该位置为空，直接放入 data值
    if self.slots[hashvalue] == None:
        self.slots[hashvalue] = key  # 键
        self.data[hashvalue] = data  # 值
    # 目标槽位已经有数据
    else:
        # 原来数据的键值 和 当前key相同，使用新数据值data替换原来的值
        if self.slots[hashvalue] == key:
            self.data[hashvalue] = data  #replace
        # 目标槽位 键key不同
        else:
            # 使用冲突函数 计算一个新的 目标位置
            nextslot = self.rehash(hashvalue,len(self.slots))
            while self.slots[nextslot] != None and \
                      self.slots[nextslot] != key:
                # 如果该槽不为空，则迭代 rehash 函数，直到出现空槽或者和当前 键key相同的槽位
                nextslot = self.rehash(nextslot,len(self.slots))
            
            # 找到的新的槽位为空，直接放入对应 键key 和 值data value
            if self.slots[nextslot] == None:
                self.slots[nextslot]=key
                self.data[nextslot]=data
            # 替换已经存在 键 的槽位内的值
            else:
                self.data[nextslot] = data #replace
        
# hash 函数实现简单的余数方法=======
def hashfunction(self,key,size):
     return key%size
     
# 冲突解决技术是 加1 rehash 函数的线性探测=====
def rehash(self,oldhash,size):
    return (oldhash+1)%size


# 按 键值key取出，值value=================================
def get(self,key):
    # 根据键值key使用 哈希函数计算出 目标槽位
    startslot = self.hashfunction(key,len(self.slots))
    data = None
    stop = False
    found = False
    position = startslot# 初始位置
    # 查找
    while self.slots[position] != None and  \
                       not found and not stop:
        # 键值是否 和 目标键值相同
        if self.slots[position] == key:
            # 找到
            found = True
            data = self.data[position]
        else:
            # 第一个可能的位置没有，使用冲突解决函数，得到下一个可能的位置
            position=self.rehash(position,len(self.slots)) #更新 position
            if position == startslot:
                # 第 15 行保证搜索将通过检查以确保我们没有返回到初始槽来终止 
                # 如果发生这种情况，我们已用尽所有可能的槽，并且项不存在。
                stop = True # 提前结束
    return data

# 使用 [] 取值的 内置实现函数
def __getitem__(self,key):
    return self.get(key)
# 使用 [] 赋值的内置实现函数
def __setitem__(self,key,data):
    self.put(key,data)

# 使用
>>> H=HashTable()
>>> H[54]="cat"
>>> H[26]="dog"
>>> H[93]="lion"
>>> H[17]="tiger"
>>> H[77]="bird"
>>> H[31]="cow"
>>> H[44]="goat"
>>> H[55]="pig"
>>> H[20]="chicken"
>>> H.slots # 键
[77, 44, 55, 20, 26, 93, 17, None, None, 31, 54]
>>> H.data  # 值(与键一一对应)
['bird', 'goat', 'pig', 'chicken', 'dog', 'lion',
       'tiger', None, None, 'cow', 'cat']

```
      
        
## 排序   
       排序是以某种顺序从集合中放置元素的过程。
       例如，单词列表可以按字母顺序或按长度排序。
       城市列表可按人口，按地区或邮政编码排序。
       我们已经看到了许多能够从排序列表中获益的算法（回忆之前的回文例子和二分查找）。 
       
       有许多开发和分析的排序算法。
       表明排序是计算机科学的一个重要研究领域。
       对大量项进行排序可能需要大量的计算资源。
       与搜索一样，排序算法的效率与正在处理的项的数量有关。
       对于小集合，复杂的排序方法可能更麻烦，开销太高。
       另一方面，对于更大的集合，我们希望利用尽可能多的改进。
       
       排序过程的操作。首先，必须比较两个值以查看哪个更小（或更大）。
       为了对集合进行排序，需要一些系统的方法来比较值，以查看是否有问题。
       比较的总数将是测量排序过程的最常见方法。
       第二，当值相对于彼此不在正确的位置时，可能需要交换它们。
       这种交换是一种昂贵的操作，并且交换的总数对于评估算法的整体效率也将是很重要的。
       
### 1. 冒泡排序
      冒泡排序需要多次遍历列表。它比较相邻的项并交换那些无序的项。
      每次遍历列表将下一个最大的值放在其正确的位置。
      实质上，每个项“冒泡”到它所属的位置。
      下图，展示了冒泡排序的第一次遍历。阴影项正在比较它们是否乱序。
      如果在列表中有 n 个项目，则第一遍有 n-1 个项需要比较。
      重要的是要注意，一旦列表中的最大值是一个对的一部分，它将不断地被移动，直到遍历完成。
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.7.%E5%86%92%E6%B3%A1%E6%8E%92%E5%BA%8F/assets/5.7.%E5%86%92%E6%B3%A1%E6%8E%92%E5%BA%8F.figure1.png)
      
      在第二次遍历的开始，现在最大的值已经在正确的位置。
      有 n-1 个项留下排序，意味着将进行 n-2 次比较。
      由于每次遍历通过将下一个最大值放置在适当位置，
      所需的遍历的总数将是 n-1。
      
```python
def bubbleSort(alist):
    for passnum in range(len(alist)-1,0,-1):
        # n-1次遍历 n-1,n-2,...,1
        for i in range(passnum):
            # 每次遍历经过 对应次相邻两次的比较
            if alist[i]>alist[i+1]:
                # 也可使用加减法/异或 实现交换
                temp = alist[i]
                alist[i] = alist[i+1]
                # 大的放在右边，升序排列
                alist[i+1] = temp

alist = [54,26,93,17,77,31,44,55,20]
bubbleSort(alist)
print(alist)

```
      
      冒泡排序通常被认为是最低效的排序方法，因为它必须在最终位置被知道之前交换项。 
      这些“浪费”的交换操作是非常昂贵的。 然而，因为冒泡排序遍历列表的整个未排序部分，
      它有能力做大多数排序算法不能做的事情。特别地，如果在遍历期间没有交换，则我们知道该列表已排序。
      如果发现列表已排序，可以修改冒泡排序提前停止。
      这意味着对于只需要遍历几次列表，冒泡排序具有识别排序列表和停止的优点。
      通常称为 短冒泡排序。 
      其次就算 该次循环中发送交换，我们也可以直接 将循环提前到交换时的位置，
      因为交换位置后的序列已经有序。
      
      
            |      /
            |  | /  直接跳过有序的部分
            |  /
            |/
```python
def shorterBubbleSort(alist):
    exchanges = True # 发生 交换标志
    changeIndex = 0  # 发送交换时的位置
    passnum = len(alist)-1
    while passnum > 0 and exchanges:
       exchanges = False
       for i in range(passnum):
           # 如果改次 循环中未发生交换，则原序列已经 有序
           if alist[i]>alist[i+1]:
               exchanges = True
               # 上次交换时的位置
               changeIndex = i
               temp = alist[i]
               alist[i] = alist[i+1]
               alist[i+1] = temp
       #
       # passnum = passnum-1 # 循环次数-1
       # 可以直接 跳转过已经有序的部分
       passnum = changeIndex

alist=[20,30,40,90,50,60,70,80,100,110,300]
shorterBubbleSort(alist)
print(alist)


```
      
### 2. 选择排序
      选择排序改进了冒泡排序，每次 遍历列表 只做 一次交换。
      为了做到这一点，一个选择排序在他遍历时寻找最大的值，并在完成遍历后，将其放置在正确的位置。
      与冒泡排序一样，在第一次遍历后，最大的项在正确的地方。
      第二遍后，下一个最大的就位。
      遍历 n-1 次排序 n 个项，因为最终项必须在第（n-1）次遍历之后。
      
      在每次遍历时，选择最大的剩余项，然后放置在其适当位置。
      第一遍放置 93，第二遍放置 77，第三遍放置 55 等。
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.8.%E9%80%89%E6%8B%A9%E6%8E%92%E5%BA%8F/assets/5.8.%E9%80%89%E6%8B%A9%E6%8E%92%E5%BA%8F.activecode1.png)
      
```python
def selectionSort(alist):
    # 还是 n-1次遍历
    for fillslot in range(len(alist)-1,0,-1):
        positionOfMax=0 #选取的最大元素的位置
        # 比较 fillslot次找剩余元素中的最大值
        for location in range(1,fillslot+1):
            if alist[location]>alist[positionOfMax]:
                # 记录最大元素的位置
                positionOfMax = location
        # 交换 右边目标位置元素  和 最大元素
        temp = alist[fillslot] # 右边 目标位置元素
        alist[fillslot] = alist[positionOfMax] # 最大元素->目标位置
        alist[positionOfMax] = temp # 目标位置元素 -> 原最大元素位置

alist = [54,26,93,17,77,31,44,55,20]
selectionSort(alist)
print(alist)

# 你可能会看到选择排序与冒泡排序有相同数量的比较，因此也是 O(n^2)。
# 然而，由于交换数量的减少，选择排序通常在基准研究中执行得更快。 
# 事实上，对于我们的列表，冒泡排序有 20 次交换，而选择排序只有 8 次。+


```
      
### 3. 插入排序
      插入排序，尽管仍然是 O(n^2)，工作方式略有不同。
      它始终在列表的较低位置维护一个排序的子列表。
      然后将每个新项 “插入” 回先前的子列表，使得排序的子列表称为较大的一个项。
      下图展示了插入排序过程。 阴影项表示算法进行每次遍历时的有序子列表。  
      
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.9.%E6%8F%92%E5%85%A5%E6%8E%92%E5%BA%8F/assets/5.9.%E6%8F%92%E5%85%A5%E6%8E%92%E5%BA%8F.figure4.png)
      
```python
def insertionSort(alist):
    # [0,n-1] 中 [1,n-1] 子项插入到前面 有序的序列中
    for index in range(1,len(alist)):
        # 当前 选取的数
        currentvalue = alist[index]
        # 对应位置
        position = index
        # 插入到前面有序的序列中，当前位置之前 到 0
        while position>0 and alist[position-1]>currentvalue:
            # 找到一个 比 当前 选取数 currentvalue 小的位置
            # 大的值依次后移一位
            alist[position]=alist[position-1]
            position = position-1
        # 找到对应位置后，把选取的数放入合适的位置
        alist[position]=currentvalue


alist = [54,26,93,17,77,31,44,55,20]
insertionSort(alist)
print(alist)


```
      
        
### 4. 希尔排序
      希尔排序（有时称为“递减递增排序”）通过将原始列表分解为
      多个较小的子列表来改进插入排序，每个子列表使用插入排序进行排序。
      选择这些子列表的方式是希尔排序的关键。不是将列表拆分为连续项的子列表，
      希尔排序使用增量i（有时称为 gap），通过选择 i 个项的所有项来创建子列表。
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.10.%E5%B8%8C%E5%B0%94%E6%8E%92%E5%BA%8F/assets/5.10.shell%E6%8E%92%E5%BA%8F.figure9.png)
      
      增量的选择方式是希尔排序的独特特征。 ActiveCode 1中展示的函数使用不同的增量集。
      在这种情况下，我们从 n/2 子列表开始。
      下一次，n/4 子列表排序。 
      最后，单个列表按照基本插入排序进行排序。
      上图 展示了我们使用此增量的示例的第一个子列表。
      
```python
def shellSort(alist):
    sublistcount = len(alist)//2 #gap 
    while sublistcount > 0:

      for startposition in range(sublistcount):
        gapInsertionSort(alist,startposition,sublistcount)

      print("After increments of size",sublistcount,
                                   "The list is",alist)
      # 
      sublistcount = sublistcount // 2

def gapInsertionSort(alist,start,gap):
    for i in range(start+gap,len(alist),gap):

        currentvalue = alist[i]
        position = i

        while position>=gap and alist[position-gap]>currentvalue:
            alist[position]=alist[position-gap]
            position = position-gap

        alist[position]=currentvalue

alist = [54,26,93,17,77,31,44,55,20]
shellSort(alist)
print(alist)

```
      
      


### 5. 归并排序
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.11.%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F/)

      我们现在将注意力转向使用分而治之策略作为提高排序算法性能的一种方法。 
      我们将研究的第一个算法是归并排序。归并排序是一种递归算法，不断将列表拆分为一半。
      如果列表为空或有一个项，则按定义（基本情况）进行排序。
      如果列表有多个项，我们分割列表，并递归调用两个半部分的合并排序。
      一旦对这两半排序完成，就执行称为合并的基本操作。
      合并是获取两个较小的排序列表并将它们组合成单个排序的新列表的过程。 
      Figure 10 展示了我们熟悉的示例列表，它被mergeSort 分割。 
      Figure 11 展示了归并后的简单排序列表。
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.11.%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F/assets/5.11.%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F.figure10.png)
      
```python
def mergeSort(alist):
    print("Splitting ",alist)
    if len(alist)>1:
        mid = len(alist)//2
        lefthalf = alist[:mid]
        righthalf = alist[mid:]

        mergeSort(lefthalf)
        mergeSort(righthalf)

        i=0
        j=0
        k=0
        while i < len(lefthalf) and j < len(righthalf):
            if lefthalf[i] < righthalf[j]:
                alist[k]=lefthalf[i]
                i=i+1
            else:
                alist[k]=righthalf[j]
                j=j+1
            k=k+1

        while i < len(lefthalf):
            alist[k]=lefthalf[i]
            i=i+1
            k=k+1

        while j < len(righthalf):
            alist[k]=righthalf[j]
            j=j+1
            k=k+1
    print("Merging ",alist)

alist = [54,26,93,17,77,31,44,55,20]
mergeSort(alist)
print(alist)

```
      
### 6.   
      快速排序首先选择一个值，该值称为 枢轴值。
      虽然有很多不同的方法来选择枢轴值，我们将使用列表中的第一项。
      枢轴值的作用是帮助拆分列表。
      枢轴值属于最终排序列表（通常称为拆分点）的实际位置，将用于将列表划分为快速排序的后续调用。
      使用首尾 两个指针，左指针 寻找大于 轴值的元素
                       右指针 寻找小于  轴值的元素
                       然后交换左右两个数
![](https://facert.gitbooks.io/python-data-structure-cn/5.%E6%8E%92%E5%BA%8F%E5%92%8C%E6%90%9C%E7%B4%A2/5.12.%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F/assets/5.12.%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F.figure13.png)
      
      
```python
def quickSort(alist):
   quickSortHelper(alist,0,len(alist)-1)

def quickSortHelper(alist,first,last):
   if first<last:

       splitpoint = partition(alist,first,last)

       quickSortHelper(alist,first,splitpoint-1)
       quickSortHelper(alist,splitpoint+1,last)


def partition(alist,first,last):
   pivotvalue = alist[first]

   leftmark = first+1
   rightmark = last

   done = False
   while not done:

       while leftmark <= rightmark and alist[leftmark] <= pivotvalue:
           leftmark = leftmark + 1

       while alist[rightmark] >= pivotvalue and rightmark >= leftmark:
           rightmark = rightmark -1

       if rightmark < leftmark:
           done = True
       else:
           temp = alist[leftmark]
           alist[leftmark] = alist[rightmark]
           alist[rightmark] = temp

   temp = alist[first]
   alist[first] = alist[rightmark]
   alist[rightmark] = temp


   return rightmark

alist = [54,26,93,17,77,31,44,55,20]
quickSort(alist)
print(alist)


````
### 总结
      对于有序和无序列表，顺序搜索是 O(n)。
      在最坏的情况下，有序列表的二分查找是 O(logn)。
      哈希表可以提供恒定时间搜索。
      冒泡排序，选择排序和插入排序是 O(n^2)算法。
      希尔排序通过排序增量子列表来改进插入排序。它落在 O(n)和 O(n^2)之间。
      归并排序是 O(nlogn)O(nlogn)，但是合并过程需要额外的空间。
      快速排序是 O(nlogn)O(nlogn)，但如果分割点不在列表中间附近，可能会降级到O(n^2)。它不需要额外的空间。
      
# 树和数算法
      
      树在计算机科学的许多领域中使用，包括操作系统，图形，数据库系统和计算机网络。
      树数据结构与他们的植物表亲有许多共同之处。
      树数据结构具有根，分支和叶。
      自然界中的树和计算机科学中的树之间的区别在于
        树数据结构的根在顶部，其叶在底部，实际数相反。
      
      你可能每天使用的树结构的另一个示例是文件系统。
      在文件系统中，目录或文件夹被构造为树。
      下图说明了 Unix文件系统层次结构的一小部分。
![](https://facert.gitbooks.io/python-data-structure-cn/6.%E6%A0%91%E5%92%8C%E6%A0%91%E7%9A%84%E7%AE%97%E6%B3%95/6.2.%E6%A0%91%E7%9A%84%E4%BE%8B%E5%AD%90/assets/6.2.%E6%A0%91%E7%9A%84%E4%BE%8B%E5%AD%90.figure2.png)
      
      
      
      节点
         节点是树的基本部分。它可以有一个名称，我们称之为“键”。
         节点也可以有附加信息。我们将这个附加信息称为“有效载荷”。
         虽然有效载荷信息不是许多树算法的核心，但在利用树的应用中通常是关键的。
      边
         边是树的另一个基本部分。边连接两个节点以显示它们之间存在关系。
         每个节点（除根之外）都恰好从另一个节点的传入连接。
         每个节点可以具有多个输出边。
      根
         树的根是树中唯一没有传入边的节点。在 Figure 2 中，/ 是树的根。
      路径
         路径是由边连接节点的有序列表。例如， Mammal \to Carnivora \to Felidae \to Felis \to   
         DomesticaMammal→Carnivora→Felidae→Felis→Domestica 是一条路径。
      子节点
         具有来自相同传入边的节点 c 的集合称为该节点的子节点。
         在 Figure 2中，节点 log/，spool/ 和 yp/ 是节点 var/ 的子节点。
      父节点
         具有和它相同传入边的所连接的节点称为父节点。
         在 Figure 2 中，节点 var/ 是节点 log/，spool/ 和 yp/ 的父节点。
      兄弟
         树中作为同一父节点的子节点的节点被称为兄弟节点。节点 etc/ 和 usr/ 是文件系统树中的兄弟节点。
      子树
         子树是由父节点和该父节点的所有后代组成的一组节点和边。
      叶节点
         叶节点是没有子节点的节点。 例如，人类和黑猩猩是 Figure 1 中的叶节点。
      层数
         节点 n 的层数为从根结点到该结点所经过的分支数目。 
         例如，图1中的Felis节点的级别为五。根据定义，根节点的层数为零。
      高度
         树的高度等于树中任何节点的最大层数。 Figure 2 中的树的高度是 2。
         现在已经定义了基本词汇，我们可以继续对树的正式定义。 
         事实上，我们将提供一个树的两个定义。 
         一个定义涉及节点和边。 
         第二个定义，将被证明是非常有用的，是一个递归定义。
         定义一：树由一组节点和一组连接节点的边组成。
         树具有以下属性：
           树的一个节点被指定为根节点。
         除了根节点之外，每个节点 n 通过一个其他节点 p 的边连接，其中 p 是 n 的父节点。
         从根路径遍历到每个节点路径唯一。
         如果树中的每个节点最多有两个子节点，我们说该树是一个二叉树。
      
      
## 列表表示
      在由列表表示的树中，我们将从 Python 的列表数据结构开始，并编写上面定义的函数。
      虽然将接口作为一组操作在列表上编写与我们实现的其他抽象数据类型有点不同，
      但这样做是有趣的，因为它为我们提供了一个简单的递归数据结构，我们可以直接查看和检查。
      在列表树的列表中，我们将根节点的值存储为列表的第一个元素。
      列表的第二个元素本身将是一个表示左子树的列表。列表的第三个元素将是表示右子树的另一个列表。
      为了说明这种存储技术，让我们看一个例子。 
      Figure 1 展示了一个简单的树和相应的列表实现。
![](https://facert.gitbooks.io/python-data-structure-cn/6.%E6%A0%91%E5%92%8C%E6%A0%91%E7%9A%84%E7%AE%97%E6%B3%95/6.4.%E5%88%97%E8%A1%A8%E8%A1%A8%E7%A4%BA/assets/6.4.%E5%88%97%E8%A1%A8%E8%A1%A8%E7%A4%BA.figure1.png)
      
```python
myTree = ['a',   #根 root
          ['b',  #左子树 left subtree
            ['d', [], []],
            ['e', [], []] ],
          ['c',  #右子树 right subtree
            ['f', [], []],
            [] ]
         ]
````

      我们可以使用标准列表索引来访问列表的子树。
      树的根是 myTree[0]，
      根的左子树是 myTree[1]，
      右子树是 myTree[2]。
      一旦树被构建，我们可以访问根和左右子树。
      该列表方法的一个非常好的属性是表示子树的列表的结构遵守树定义的结构;
      结构本身是递归的！具有根值和两个空列表的子树是叶节点。
      列表方法的另一个很好的特性是它可以推广到一个有许多子树的树。
      在树超过二叉树的情况下，另一个子树只是另一个列表。
      
```python
myTree = ['a', ['b', ['d',[],[]], ['e',[],[]] ], ['c', ['f',[],[]], []] ]
print(myTree)
print('left subtree = ', myTree[1])
print('root = ', myTree[0])
print('right subtree = ', myTree[2])

```

      
列表实现:
```python
# BinaryTree 函数简单地构造一个具有根节点和两个子列表为空的列表。
# 要将左子树添加到树的根，我们需要在根列表的第二个位置插入一个新的列表。
# 我们必须小心。如果列表已经在第二个位置有东西，我们需要跟踪它，
# 并沿着树向下把它作为我们添加的列表的左子节点。
def BinaryTree(r):
    return [r, [], []]
    
# 插入左子树
def insertLeft(root,newBranch):
    t = root.pop(1)
    if len(t) > 1:
        root.insert(1,[newBranch,t,[]])
    else:
        root.insert(1,[newBranch, [], []])
    return root
    
# 插入右子树
def insertRight(root,newBranch):
    t = root.pop(2)
    if len(t) > 1:
        root.insert(2,[newBranch,[],t])
    else:
        root.insert(2,[newBranch,[],[]])
    return root

# 获取设置值========================
def getRootVal(root):
    return root[0]

def setRootVal(root,newVal):
    root[0] = newVal

def getLeftChild(root):
    return root[1]

def getRightChild(root):
    return root[2]

    
```


节点指针实现
[](https://facert.gitbooks.io/python-data-structure-cn/6.%E6%A0%91%E5%92%8C%E6%A0%91%E7%9A%84%E7%AE%97%E6%B3%95/6.5.%E8%8A%82%E7%82%B9%E8%A1%A8%E7%A4%BA/)
      
      
      
# 图
      图是比我们在上一章中研究的树更通用的结构;实际上你可以认为树是一种特殊的图。
      图可以用来表示我们世界上许多有趣的事情，包括道路系统，从城市到城市的航空公司航班，
      互联网如何连接，甚至是完成计算机科学专业必须完成的课程顺序。
      我们将在本章中看到，一旦我们有一个问题的好的表示，
      我们可以使用一些标准图算法来解决，否则可能是一个非常困难的问题。
      
[参考](https://facert.gitbooks.io/python-data-structure-cn/7.%E5%9B%BE%E5%92%8C%E5%9B%BE%E7%9A%84%E7%AE%97%E6%B3%95/7.3.%E5%9B%BE%E6%8A%BD%E8%B1%A1%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B/)

      

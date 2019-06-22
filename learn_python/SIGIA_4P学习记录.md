# SIGIA_4P python学习 列表 字典 集合 面对对象编程 闭包 装饰器 函数式编程 作用域 异常处理
[本文连接](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_python/SIGIA_4P%E5%AD%A6%E4%B9%A0%E8%AE%B0%E5%BD%95.md)

# 简介

[SIGIA_4P 网址](https://www.sigai.cn/)

    a. 课程OKR  Objectives and Key Results 即目标与关键成果法
       目标，Objectives ： 掌握AI领域的Python开发
       成果1 KR1
  
> linux 目录结构

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_python/img/linux_file_sys.png)
  
> 为什么学习python

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_python/img/python.png)

> python 逻辑判断

![](https://github.com/Ewenwan/ShiYanLou/blob/master/learn_python/img/python_logical.png)
  
  
## 2. python

> 输入输出
```python
  # 输出
  print("XXXXX")
  print('"asc"') >>> "asc"
  print("'asc'") >>> 'asc'
  print(''' 'tpr' and "asc" ''') >>> 'tpr' and "asc"
  print('\"asc\"') >>> "asc"
  print('\"asc\\ \"') >>> "asc\ "
  
  
  print(r'\"asc\\ \"') >>> \"asc\\ \"  # raw string 正则表达式
  
  ''' 'd'
      'd'
      'r'
  '''   多行输出
  
  
  print(300)
  print(100+300)
  print('100 + 300 =', 100+300) >>> 100 + 300 = 400   # 注意,打印了空格
  
for i in range(1,21): # 遍历 1到 21-1的整数i
    print(i,end='')   # 不换行打印 i
    print(' ',end='') # 不换行打印空格
	
	
	
for i in range(1,21):    # 遍历 1到 21-1的整数i
    if i%5 == 0:         # 当 i取余5为0时，即为5 10 15 20 时
	print(i,end='')  # 不换行打印 i
	print('')        # 换行
    else:
        print(i,end='')  # 不换行打印 i
        print(' ',end='')# 不换行打印 空格	
  
  
  # 输入
  name = input()
  print(name)
  name = input('input your name: ') # 带有提示符
  
  
str = input("input a number\n") #输入一个字符
number = int(str)#字符转换成 int整数类型
for i in range(number):# for循环遍历 0到number-1
    if i%2==0:         # 偶数的话，取余2为0
        print(i)       # 符合条件打印数字
  
  
```
> 注意点
    
    大小写敏感
    4空格缩进 vs {}
    统一 tab 和 4空格，tab 不同系统代表的空格数量不同
    字符编码：
        纯英文，可用ASCII将str 编码为 bytes
        含有中文。可用utf-8将str 编码为 bytes
        从网络或磁盘上读取的字节流为 bytes
```python
s_u = 'sig'
s_b = b'sig'
type(s_b)  >>> bytes
type(s_b.decode(ascii))  >>> str  # 解码

type(s_u.encode(ascii))  >>> bytes  # 编码

s_u = 'sig中文'
type(s_u.encode(utf-8))  >>> 
b'sig\xxxxxxx'
# decode 回 'sig中文'

```
        
        sorted() 返回中间结果，对象不变
        对象.sort() 对象改变了
        对象.replace() 返回中间结果，对象不变
         
        引用、拷贝、深拷贝、浅拷贝
        可变对象   赋值   传递引用 浅拷贝
        不可变对象  赋值   深拷贝
        
        
> 数据类型
    
    整数 int
    浮点数 float
    字符串 string   "abc" 或 'dgv'  '''cdsfr'''  内容一致，id就一致
    布尔值 bool     True False   可进行 and or not
    空值 None 不同与 0
     
> 基本运算  python3
    
     加 + 
     减 - 
     乘 *
     浮点除 /
     整数除 //  得到商
     取余   %
     乘方 **

```python
a = 4   # a 指向了4 
b = a   # b 也指向了4
a = 6   # a 新指向了6
        # 而b还指向4
```
 
## 3. 列表 List  可变有序集合   []
```python
L = ['ls1','ls2','ls3']
print L
L.append('ls4')  # 在列表尾部添加一个元素， 无返回值
L.pop()          # 在尾部删除一个元素， 并且打印 删除了的元素
L2 = ['ll1','ll2','ll3']
L3 = L + L2   # 相加 ，直接在后面扩展 ['ls1','ls2','ls3','ll1','ll2','ll3']
LL = L*2      # 直接扩展  ['ls1','ls2','ls3','ls1','ls2','ls3']


# 列表索引
L =     ['ll1','ll2','ll3']
# 正索引   0     1      2
# 负索引   -3    -2     -1
L[0]  >>>> 'll1'

L =     ['ll1','ll2','ll3',[1,2,3]]
L[3][0] >>> 1
len(L)  >>> 4

L =[]  # 清空
```


## 4. Tuple  '不可变' List  ()
```python
T = ('ls1','ls2','ls3')
T[0]  # 访问

T = (1)   # T 是一个整形变量
T = (1,)  # 声明一个元素的 Tuple

T.append('ls4')   # 报错
T.pop()           # 报错
T[0] = 'TR2'      # 报错

# 但是 Tuple内的 List 可变
T = ('ls1','ls2','ls3',[1,2,3])
T[3].append(4)
 >>> 
  ('ls1','ls2','ls3',[1,2,3,4])
  
  
```

## 5. Dict 字典   key:value  查找速度快  哈系函数实现 内存换时间  无序  key必须不可变
```python
D = {'a': 1, 'b': 2, 'c': 3} # 前面的key 决定 value存在哪里
print(D['a'])  >>> 1

D['d']) = 4  # 添加

D['d']) = 7  # 修改

'b' in D >>> 返回 True
'f' in D >>> 返回 False

D.key()  >>> 
         dict_keys(['a','b','c','d'])
# get 获取元素==============
D.get('b') >>> 2

D.get('f') >>> None 无值

D.get('f', -1 ) >>> -1  返回默认值


```
## 6. Set 集合  Dict 中 Key的集合，key是唯一的，不可重复，去重复的好方法



## 7. 可变 参数函数
```python
# *参数，相当于传入一个 list  *用来拆包的
def calc_sum(*numers):
    sum = 0
    for num in numers:
        sum += num
    return sum

if __name__ == '__main__':
    # 需要测试的函数 赋值给 一个名字
    test_func = calc_sum
    # assert 测试
    assert test_func() == 0
    assert test_func(3) == 3
    assert test_func(1,3,5) == 9
    assert test_func(-1,1) == 0
    print('OK')
```

## 8. 面向对象 类 
```python
class Student(object):
    # 类构造函数 __init__   self 类名
    def __init__(self, name, score=-1):
        # __开头的参数为 私有参数
        self.__name = name
        self.__score = score
        self.say_hi()
    
    def name(self):
        # 通过函数获取私有变量
        return self.__name
    
    def say_hi(self):
        if self.__score < 0:
            print("{}: Hi, my name is {}. I'm a new student.".format(self.__name, self.__name))
        else:
            print("{}: Hi, my name is {}. My score is {}.".format(self.__name, self.__name, self.__score))
            
    def get_score(self, teacher, score):
        self.score = score
        print("{}: teacher {}. just gave me a {}.".format(self.__name, teacher, score))
        
        
class Teacher(object):
    def __init__(self, name):
        # __开头的参数为 私有参数
        self.__name = name
        self.say_hi()
        
    def say_hi(self):
        print("{}: Hi, my name is {}. I'm a teacher.".format(self.__name, self.__name)
        
    def score(self, student, score):
        student.get_score(self.__name, score)
        print("{}: I just gave {} a {}.".format(self.__name, student.name , score))
    
if __name__ == '__main__':
    studentA = Student('A')
    TeacherB = Teacher（'B'）
    # 老师给学生打分 80
    TeacherB.score(studentA, 80)
    
```

## 9. Python 代码的组织-模块
> * 包-> 模块 -> 类 或功能函数

> * 每个包里都有一个 __init__.py 文件，用以区分是普通目录函数包

> * 自建包名 不可与其他包重名 不然就覆盖了


```python
# 模块功能
'first module'

# 模块作者
__author__ = 'ewenwan'

# 内容开始

# 命令行参数  args 
import sys 

def say_hi():
    if len(args)==1:
        print('one augment')
    elif len(args)==2:
        print('two augment')
    else:
        print('more then two augment')
        
# 执行函数        
if __name__ == '__main__':
    say_hi()

```


## 10. 包管理工具 pip pip3 查找 安装 删除
```sh
# pip版本信息
pip -V
# 位置
which pip
# 查看已安装的包
pip list

# 升级 pip
pip install --upgrade pip --user
sudo pip install --upgrade pip


# python 路径  查找 添加
>>> python
>>> import sys
>>> print('\n'.join(sys.path)) # 使用 '\n' 添到 path中 回车换行打印
....
>>> pwd
>>> sys.path.append('/home/ewenwan/...')
>>> print('\n'.join(sys.path))

```



# List 可变列表
```python

>>> L = ['tensorflow', 'torch', 'caffe', 'mxnet', 'keras']
>>> 取出前三个字符串串组成新的列列表,此时便便可以使用用List的切片片功能了了

# 切片 [起始位置:终止止位置:步⻓长] (只有第一一个 : 是必须有的)
# 开头是 闭区间， 终止止位置 是开区间 

>>> newL = L[:3]
>>> newL
['tensorflow', 'torch', 'caffe']

# range 用法
>>> list(range(10)) # 从0开始到 10前面一个数
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9] 

>>> list(range(1,10)) # 从1开始到 10前面一个数
[1, 2, 3, 4, 5, 6, 7, 8, 9]

>>> list(range(1,11))# 从1开始到 11前面一个数
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

>>> list(range(1,11,2)) # 1开始，间隔2，到11之前
[1, 3, 5, 7, 9]
>>> list(range(1,11,3)) # 1开始，间隔3，到11之前
[1, 4, 7, 10]


# 切片  负值位置====================
#接下来生生成1-10的数组
>>> L = list(range(1,11))
>>> L
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

>>> L[:5]  # 从0 取到第5个之前
[1, 2, 3, 4, 5] # 0 1 2 3 4位置上的数

>>> L[:12] # 超出范围也不报错，只取到所有的
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

>>> L[:-1] # -1 表示到最后一个数之前
[1, 2, 3, 4, 5, 6, 7, 8, 9]

>>> L[:-3] # 到 倒数第3个之前
[1, 2, 3, 4, 5, 6, 7]

>>> L[:-15]# 倒数 超出范围也不会报错
[]
>>> L[:0]
[]

# 步长为负数=============方便倒顺
>>> L
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

>>> L[2:5]
[3, 4, 5]

>>> L[2:-1] # 第3个 到 最后一个之前
[3, 4, 5, 6, 7, 8, 9]

>>> L[1:5:3] # 第2个到 第6个 间隔3
[2, 5]

>>> L[-1:0:-1] # 最后一个取到 最开始 之前， -1 从右往左取====
[10, 9, 8, 7, 6, 5, 4, 3, 2]

>>> L[-1::-1] # 倒序 取出所有元素===============
[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

>>> L[::-1]
[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]  # 倒序 取出所有元素======

>>> L[::] # 切片 元素取出， 可实====现深拷贝=====
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

>>> L[:] # 切片 元素取出， 可实====现深拷贝=====
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]




```


# 多重List 的 浅拷贝 与深拷贝

```python
    import copy
    a = [[1, 2, 3], [4, 5, 6]] # 多重List，List内的List是可变对象
    b = a                 # 拷贝引用，你变换我也变换        
    c = copy.copy(a)      # 浅拷贝 拷贝一层 的 地址
    d = copy.deepcopy(a)  # 深拷贝 拷贝两层
    a.append(7)
    a[1][2] = 10
    print('原数组: ', a)
    print('引用用赋值: ', b)
    print('浅拷⻉贝: ', c)
    print('深拷⻉贝: ', d)
    sigai@8a5f47e78164:~/workspace$ python test.py
    原数组:    [[1, 2, 3], [4, 5, 10], 7]
    引用用赋值: [[1, 2, 3], [4, 5, 10], 7]
    浅拷⻉贝: [[1, 2, 3], [4, 5, 10]]
    深拷⻉贝: [[1, 2, 3], [4, 5, 6]]

```

# 序列列的加法,乘法,初始化
```python
# 加法  同类型序列 相加
>>> L
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> L + L[-2::-1]
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1] # 直接 接在 后面

>>> s = 'sigai'
>>> S = 'SIGAI'
>>> s + S
'sigaiSIGAI'

>>> s + L   # 不同类型 序列相加会出问题
Traceback (most recent call last):
File "<stdin>", line 1, in <module>
TypeError: Can't convert 'list' object to str implicitly

# 序列列的乘法
## 序列列与整数相乘,也可以快速创建包含重复元素的序列列
>>> L * 2
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> s * 5
'sigaisigaisigaisigaisigai'

# 常用用的序列列初始化方方案
>>> L = []
>>> L = [] * 10
>>> L
[]
>>> L = [0] * 10
>>> L
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
>>> L = [None] * 10
>>> L
[None, None, None, None, None, None, None, None, None, None]


# 序列列的常见内置方方法=
## 序列列的包含关系: in===================================
>>> s
'sigai'
>>> 'i' in s
True
>>> 'I' in s
False
>>> L = list(range(11))
>>> 5 in L
True
>>> 20 in L
False

## 序列列的⻓长度,最大大值,最小小值: len max min================
>>> L
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> len(L)
11
>>> s
'sigai'
>>> len(s)
5
>>> max(L)
10
>>> min(L)
0
>>> max(s)
's'
>>> min(s)
'a'

# List与str的相互转换================================
>>> s
'sigai'
>>> l = list(s)
>>> l
['s', 'i', 'g', 'a', 'i']
>>> s_from_l = ''.join(l)  # '' 为内放入 插入的内容
>>> s_from_l
'sigai'

# List的元素或切片片的赋值与删除
## 对列列表内的元素可以随意赋值和删除======================

>>> L = list(range(5))
>>> L
[0, 1, 2, 3, 4]

>>> L[2], L[4] = L[4], L[2] # 交换 第3 和 第5 个数
>>> L
[0, 1, 4, 3, 2]

>>> del L[2]
>>> L
[0, 1, 3, 2]


## 列列表内的切片片也可以随意的赋值和删除,还可以用用作插入入 ========
>>> L = [1,5]
>>> L[1:1] = list(range(2,5)) # 批量插入 修改
>>> L
[1, 2, 3, 4, 5]
>>> L[2:4] = []  # 批量删除
>>> L
[1, 2, 5]
>>> del L[1:]
>>> L
[1]



# 计数函数: count=====================================

>>> from functools import reduce
>>> L = reduce(lambda x,y: x+y, [[i]*i for i in range(1,6)])
>>> import random
>>> random.shuffle(L)
>>> L
[3, 2, 5, 5, 4, 5, 3, 1, 2, 5, 5, 4, 4, 4, 3]  # 随机数组
>>> L.count(4)
4
>>> L.count(5)
5
>>> L.count(6)
0


# 列列表的排序  sorted函数=======sort方法==============
>>> L
[3, 2, 5, 5, 4, 5, 3, 1, 2, 5, 5, 4, 4, 4, 3]
>>> L_sorted = sorted(L)  # 返回拷贝=========
>>> L_sorted
[1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5]
>>> L
[3, 2, 5, 5, 4, 5, 3, 1, 2, 5, 5, 4, 4, 4, 3]
>>> L.sort() # 直接修改自己========
>>> L
[1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5]
>>> x = L.sort() # 没有返回值   None, 
>>> print(x)
None


```


# 常见类型的常见操作 时间复杂度
[List等的内置方方法及其时间空间复杂度 ](https://wiki.python.org/moin/TimeComplexity)

```python
# list================================
操作       平均复杂度  最坏情况复杂度
Copy       O(n)       O(n)
Append[1]  O(1)       O(1)
Pop last   O(1)       O(1)
Pop intermediate
	       O(k)       O(k)
Insert     O(n)       O(n)
Get Item   O(1)       O(1)
Set Item   O(1)       O(1)
Delete Item
	       O(n)       O(n)
Iteration  O(n)       O(n)
Get Slice  O(k)       O(k)
Del Slice  O(n)       O(n)
Set Slice  O(k+n)     O(k+n)
Extend[1]  O(k)       O(k)
Sort       O(n log n) O(n log n)
Multiply   O(nk)      O(nk)    
x in s     O(n)
min(s), max(s) 
           O(n)
Get Length O(1)       O(1) 

# deque (double-ended queue) =========

Copy       O(n)       O(n)
append     O(1)       O(1)
appendleft
	       O(1)       O(1)
pop        O(1)       O(1)
popleft    O(1)       O(1)
extend     O(k)       O(k)
extendleft O(k)       O(k)
rotate     O(k)       O(k)
remove     O(n)       O(n) 

# set================================
x in s     O(1)       O(n) 
Union s|t
           O(len(s)+len(t)) 
Intersection s&t
           O(min(len(s), len(t))
	       O(len(s) * len(t)) 
# dict==============================

Copy[2]      O(n)       O(n)
Get Item     O(1)       O(n)
Set Item[1]  O(1)       O(n)
Delete Item  O(1)       O(n)
Iteration[2] O(n)       O(n) 
```


# 列表推导式 list comprehension  Python的递推式构造列表

形式1：[表达式 for 变量 in 列表]

形式2：[表达式 for 变量 in 列表 if 条件]

列表推导式较为简洁的实现 将一个函数应用于整个列表，或是删除不需要的列表项；



```python
ll=[1,2,3,4,5,6,7,8,9]

print([v*10 for v in ll if v>4])

      表达式: v*10
      变量:   v
      列表:   ll
      条件:   if v>4
# 打印输出如下的列表：
[50,60,70,80,90]

# 列表推导式构造字典

dd==dict([(v:v*10) for v in ll])
建立的字典为：
{1:10,2:20,3:30,4:40,5:50,6:60,7:70,8:80,9:90}

>>> sum([x for x in range(101) if x % 2 == 0]) # 0到100 内所有偶数的和
2550
>>> sum([1 if x % 3 == 2 else -1 if x % 3 == 1 else 0 for x in range(101)])
    # 先找 原始 range()   0到100内数 如果 除以3余数为2，就变成1; 如果除以3余数为1，就变成 -1;否则整除3，就变成0
    # [0 -1 1 0 -1 1 ...] 到 第99个数 加和刚好是0，而100 会变成-1，所以总序列求和后为 -1
-1

# List Comprehension与lambda以及map/reduce还有filter连用用功能更更强大大

# 匿名函数lambda+列表推导 在后面有介绍

[lambda 变量:表达式 for 变量 in 列表]

[lambda 变量:表达式 for 变量 in 列表 if 条件]

```



# 认识函数
	代码块
		代码块是一一组语句句,类似于其他语言言中的大大括号
		Python中的代码块靠缩进建立立,最好是4个空格,而而非非制表符
		Python中的代码块由 : 来引导,有缩进的提示终止止
		代码块里里里可以放条件语句句,循环语句句,函数定义,类定义,上下文文管理理器器等
	函数定义
		函数是一一个代码块
		用用 def 定义
		返回一一个值(任何情况下都是)
		Python中的调用用符号: ()
		Python中可调用用判断函数: callable
		函数中的 return 语句句:可省略略; 可单独出现; 可返回多个变量量(终止止与返回值)
		放在函数定义开头的字符串串称为docstring,作为函数的一一部分( __doc__ )
		交互式解释器器中的 help 函数

```python
>>> def print_sigai():
...
print("sigai")
...
>>> print_sigai()
	sigai
>>> help(print_sigai)    # 交互式解释器器中的 help 函数
	Help on function print_sigai in module __main__:
	print_sigai()
	(END)
>>> def print_sigai():
... '''JUST A FUNCTION''' # 放在函数定义开头的字符串串称为docstring, 作为函数的一一部分( __doc__ )
... print("sigai")
...
>>> print_sigai()
	sigai
>>> help(print_sigai)
	Help on function print_sigai in module __main__:
	print_sigai()
		JUST A FUNCTION
	(END)

>>> print_sigai.__doc__   # 放在函数定义开头的字符串串称为 docstring , 作为函数的一一部分( __doc__ )
'JUST A FUNCTION'
>>> print_sigai.__doc__ = 'just a function'  # docstring 可以修改
>>> print_sigai.__doc__
'just a function'
>>> help(print_sigai)
	Help on function print_sigai in module __main__:
	print_sigai() 
		just a function    # 已经被修改了====================
	(END)
	
	
#   eval(str) 把 str 当作命令来执行================
>>> def sigai_1():
... pass
...
>>> def sigai_2():
... return
...
>>> def sigai_3():
... return None
...
>>> def sigai_4():
... return 4
...
>>> def sigai_5():
... return 1,2,3,4,"5"  # 当作 一个 tuple 返回
...
>>> [eval('sigai_' + str(x) + '()') for x in range(1,6)]
[None, None, None, 4, (1, 2, 3, 4, '5')]


```

# 函数参数
## 形参与实参
	定义函数的时候是形参,使用用函数的时候传递的是实参
	内部修改实参,不不影响外部同名变量
```python
>>> def sigai(a,b):   # 传值进入，这里 分 可变对象和不可变对象，不可变 数，字符串，tuple不可变==============
... a += 1
... b += 1
... print(a,b)    
... return a + b
...
>>> a,b = 1,1
>>> sigai(a,b)
2 2
4
>>> print(a,b)  # 外部 a,b 不变
1 1
```

## 参数的传递
```c
>>> def sigai(a,b): # 可变对象 赋值传递时 传递的是引用，传递的是地址，内部修改，会影响外部
... a.append(1)
... b.append(1)
... print(a,b)
... return a + b
...
>>> a,b = [1],[1]  # 出入的是列表
>>> sigai(a,b)
[1, 1] [1, 1]  # a 和 b
[1, 1, 1, 1]   # a+b

>>> print(a,b) # a, b 已经变了
[1, 1] [1, 1]


#  传递深拷贝 的 副本=====就不会修改原对象了，通过a[:]  嵌套的 List 记得使用用 copy.deepcopy ==================
>>> a,b = [1],[1]
>>> sigai(a[:],b[:])
[1, 1] [1, 1]
[1, 1, 1, 1]
>>> print(a,b)
[1] [1]



```

## 位置参数与关键字参数
	参数位置解耦
	默认参数设置
```c
# 位置参数======
>>> def say_hi(say, name):
...print(say + ' ' + name + '!') # 第一个参数 第二个参数 
...
>>> def say_hi_2(name, say):
...print(name + ' ' + say + '!')
...
>>> say_hi('hello','sigai')
hello sigai!
>>> say_hi_2('hello','sigai')
hello sigai!

# 关键子参数==================
```python
参数名字和位置一一起记,实在记不不住呀,关键字参数了了解一一下
>>> say_hi(name='sigai',say='hello')
hello sigai!

# 关键字参数更更重要的用用途是设置默认值(大大型程序尤其重要)
>>> def say_hi(name, say='hello'):
... print(say + ' ' + name + '!')
...
>>> say_hi('sigai')
    hello sigai!


# 还可以再进一一步,所有参数都设置默认值
>>> def say_hi(name='sigai', say='hello'):
...print(say + ' ' + name + '!')
...
>>> say_hi()
hello sigai!

# 功能完整的函数:主逻辑用用位置参数,配置选项用用关键字参数
# Keras 的预处理理库 image 函数=======
# https://github.com/keras-team/keras-preprocessing/blob/master/keras_preprocessing/image.py

def __init__(self,
	featurewise_center=False,
	samplewise_center=False,
	featurewise_std_normalization=False,
	samplewise_std_normalization=False,
	zca_whitening=False,
	zca_epsilon=1e-6,
	rotation_range=0,
	width_shift_range=0.,height_shift_range=0.,
	brightness_range=None,
	shear_range=0.,
	zoom_range=0.,
	channel_shift_range=0.,
	fill_mode='nearest',
	cval=0.,
	horizontal_flip=False,
	vertical_flip=False,
	rescale=None,
	preprocessing_function=None,
	data_format='channels_last',
	validation_split=0.0,
	dtype='float32'):

# 两个建议:
#	最好不不同时使用用多个位置参数与多个关键字参数
#	如果使用用了了关键字参数,位置参数越少越好,并且集中放在最前面面

```



## 任意数量量的参数与Python中的星号
```python
>>> sum(1,2)
3
>>> sum(1,2,3)
6
>>> sum(1,2,3,4)
10
>>> sum(1,2,3,4,5,6,7)
28

# * 表示按 tuple解析======
>>> def sum(*l):
... result = 0
... for x in l: result += x
... return result
...

# 与赋值不不同,函数的参数用用星号拆包或缝合时,得到的是元组========================

>>> list(range(7))
[0, 1, 2, 3, 4, 5, 6]
>>> a,*b,c = list(range(7))  # 外部 *表示使用list 收集变量
>>> type(b)
<class 'list'>
>>> def sum(*l):
... print(type(l))
...
>>> sum(1,2)
<class 'tuple'>   # 函数中的* 表示使用 tuole 收集变量


# 还可以接受 任意 个 关键字参数,两个星号了了解一一下 ==============================

>>> def register(**kw):
... print("kw's type is",type(kw))
... print("kw is",kw)
...
>>> register(name='sigai',pi=3.1415926) # 传递字典形式，key变成字符串
kw's type is <class 'dict'>
kw is {'name': 'sigai', 'pi': 3.1415926}

# 可以一一起收集位置参数与关键字参数============
>>> def register(*list,**dict):
... print(list)
... print(dict)
...
>>> register(1,2,3,name='sigai',pi=3.14) # 传递 列表形式 和 关键字形式
(1, 2, 3)
{'name': 'sigai', 'pi': 3.14}


# 刚才的星号是用用作封包的,其实也可以用用来拆包===============
>>> def sum(a,b,c):
...
return a + b + c
...
>>> x = (1,2,3)
>>> sum(*x) # 这里的* 号 用来拆包，将x拆成了3个参数
6
>>> sum(x)
Traceback (most recent call last):
File "<stdin>", line 1, in <module>
TypeError: sum() missing 2 required positional arguments: 'b' and 'c'

# 对于函数参数中的星号拆包和封包,用用一一个即可,用用两个其实等于没用用
>>> def sum_1(*l):              # 这里是将 多个参数 包成一个参数 tuple类型
... result = 0
... for x in l: result += x
... return result
...
>>> def sum_2(l):    # 这里只能传递一个参数，但是这个参数 可以是list
... result = 0
... for x in l: result += x
... return result
...
>>> sum_1(*range(5))  # 这里的 * 的作用是拆包，传给函数的是 5个参数
10
>>> sum_2(range(5))   # 这里的 输入参数是 一个list
10


```


# 名称空间与作用用域解析(Namespace and Scope Resolution) LEGB解析规则
## 作用用域的产生生
	局部 Local:function and class method
	嵌套 Enclosed:its enclosing function, if a function is wrapped inside another function  
	全局 Global:the uppermost level of executing script itself    导入包开始
	内置 Built-in:special names that Python reserves for itself   解析器开始
##  globals() =================
```python
# globals() 字典 .key() 拿出关键字 成 list ''.join() 链接成字符串， 中间使用 '\n' 换行符链接
>>> print('\n'.join(globals().keys()))
__name__
__doc__
__package__
__loader__
__spec__
__annotations__
__builtins__
  
>>> sigai_1 = 1  # 自建了一个变量
>>> print('\n'.join(globals().keys()))
__name__
__doc__
__package__
__loader__
__spec__
__annotations__

__builtins__    # 上面是 __builtins__  内的
sigai_1         # 模块级别 自建了一个变量 也存在于 globals() 中



>>> sigai_2 = 2 
>>> import math         # 导入模块
>>> def say_hi(): pass  # 定义函数
...
>>> class Human: pass   # 定义类
...
>>> print('\n'.join(globals().keys()))
__name__
__doc__
__package__
__loader__
__spec__
__annotations__
__builtins__
sigai_1
sigai_2
math
say_hiHuman

```
## local() ========================================
```python
from copy import copy # 浅拷贝
globals_init = copy(list(globals().keys()))      # 初始全局作用域 变量  list类型

print("globals_init: ", *globals_init, sep='\n') # 中间 * 号 拆包，将list拆开，中间按 sep='\n' 填充 就是家换行 和'\n'.join() 类似

print() # 打印 一个空行

a, b = 1, 2 # 多个变量一起赋值

# 定义一个函数
def f1(c=3):
    # 
    d = 4
    # 查看 定义f1函数  使得 globals 全局 内多出来的 变量     *号拆包=====
    print("globals in f1:", *(globals().keys() - globals_init))
    
    print("locals in f1:", *(locals().keys()))  # 局部内多出来的
    # f1 内部 定义 f2函数
    def f2(e=5):
        f = None
	# 查看 定义f1 和 f2 函数  使得 globals 全局 内多出来的 变量
        print("globals in f2:", *(globals().keys() - globals_init))
	## 局部内多出来的
        print("locals in f2:", *(locals().keys()))
	
    f2() # 调用 f2
    
if __name__ == '__main__':
f1()


>>> 
	Globals_init:
	__name__
	__doc__
	__package__
	__loader__
	__spec__
	__annotations__
	__builtins__
	__file__
	__cached__
	copy             # 导入的包
	
	globals in f1: globals_init f1 a b
	locals in f1: d c
	globals in f2: globals_init f1 a b
	locals in f2: f e



```

## 作用用域间变量量的遮盖
```python
name = 'sigai_1'
print(name)
def f1():
    name = 'sigai_2'
    print(name)
    def f2():
        name = 'sigai_3'
        print(name)
    f2()
if __name__ == '__main__':
f1()

>>>   从最近的作用 域 向上 进行查找
	sigai_1
	sigai_2
	sigai_3

```

## 修改不不同作用用域里里里的变量量
```python
def f1(name):
    name = 'sigai_2'
    print(name)
if __name__ == '__main__':
    name = 'sigai_1'
    print(name)
    f1(name)
    print(name)  # 回到外面 还是 使用 外面的
>>> 
	sigai_1
	sigai_2
	sigai_1
	
# 函数内声明全局变量
def f1():
    global name      # 函数内声明全局变量 修改会 关联到 外面
    name = 'sigai_2'
    print(name)
if __name__ == '__main__':
    name = 'sigai_1'
    print(name)
    f1()
    print(name)
>>> 
	sigai_1
	sigai_2
	sigai_2


# 嵌套

def f1():
    name = 'sigai_2'
    print(name)
    def f2():
        name = 'sigai_3'
        print(name)
    f2()
    print(name)
if __name__ == '__main__':
    name = 'sigai_1'
    print(name)
    f1()
    print(name)
>>> 
	sigai_1
	sigai_2
	sigai_3
	sigai_2
	sigai_1

# nonlocal 非局部，也不是全局，嵌套函数内部变量
def f1():
    name = 'sigai_2'
    print(name)
    def f2():
        nonlocal name     # 这个变量 关联到 f1 内的 name
        name = 'sigai_3'
        print(name)
    f2()
    print(name)
if __name__ == '__main__':
    name = 'sigai_1'
    print(name)
    f1()
    print(name)
>>> 
sigai_1
sigai_2
sigai_3
sigai_3
sigai_1


	
```
## 作用用域的生生命周期
	built-in: 解释器器在则在,  解释器器亡则亡
	global:   导入入模块时创建,直到解释器器退出
	local:    函数调用用时才创建


# 函数式编程概述  线程 多线程 优化
     何为No Side Effect? 
	函数的所有功能就仅仅是返回一个新的值而而已,没有其他行为,尤其是
	不得修改外部变量 因而,各个独立的部分的执行顺序可以随意打乱,带
	来执行顺序上的自由 
	执行顺序的自由使得一系列列新的特性得以实现:
		无无锁的并发;
		惰性求值;
		编译器器级别的性能优化等.
		
     程序的状态与 命令式编程 多米诺骨牌
	程序的状态首首先包含了当前定义的全部变量
	有了程序的状态,我们的程序才能不断往前推进
	命令式编程,就是通过不断修改变量的值,来保存当前运行行的状态,来步步推进
	
     函数式编程
	通过函数来保存程序的状态(通过函数创建新的参数和返回值来保存状态)
	函数一层的叠加起来,每个函数的参数或返回值代表了一个中间状态
	
	命令式编程里  一次变量 值 的修改,
	在函数式编程里 变成了 一个函数的转换
	最自自然的方方式:递归


## 一等函数
     一等对象
     定义:
	在运行时创建
	能赋值给变量或数据结构中的元素
	能作为参数传给函数
	能作为函数的返回结果
	Python中,所有函数的都是一等对象,简称为一等函数
     
     python 中的函数 功能
     1. 在运行行行时创建
     2. 能赋值给变量量或数据结构中的元素
     3. 能作为参数传递给函数
     4. 能作为函数的返回结果

     
     
```python   
>>> def say_hi(): 
...   print("Hello SigAI")

>>> say_hi() # 1. 在运行行行时创建
Hello SigAI

>>> test = say_hi  #  2. 能赋值给变量量或数据结构中的元素
>>> test()
Hello SigAI


# 3. 能作为参数传递给函数
>>> def repeat(f,num):
... [f() for i in range(num)] # 调用函数 num次
...
>>> repeat(say_hi,3)
Hello SigAI
Hello SigAI
Hello SigAI

#  4. 能作为函数的返回结果
>>> def repeat_func(f,num):
...     def new_func():
...         [f() for i in range(num)]
        return new_func
...
>>> repeated_func = repeat_func(say_hi,3)
>>> repeated_func()
Hello SigAI
Hello SigAI
Hello SigAI


```

## 高高阶函数
定义:接受函数为参数,或把函数作为返回结果的函数

### map高高阶函数
```c
>>> def square(x): return x * x   # 行为
...
>>> xx = map(square, range(10))   # 将 行为 映射到 可迭代对象上的没一个元素   工业化生产
>>> xx
<map object at 0x7f993713bef0>
>>> xx = list(xx)
>>> xx
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
# 可用用 [x * x for x in range(10)] 替代==========================


```
	
### filter高高阶函数  过滤函数
```python
>>> x = [(), [], {}, None, '', False, 0, True, 1, 2, -3]
>>> x_result = filter(bool, x)  # 将 bool 行为 映射到 列表x 中的没一个元素
>>> x_result
<filter object at 0x7f993713beb8>
>>> x_result = list(x_result)
>>> x_result
[True, 1, 2, -3]
# 可用用 [i for i in x if bool(i)] 替代 加入了 条件判断 实现了类似 filter的功能

```

### reduce高高阶函数
```python
>>> def multiply(a,b): return a * b
...
>>> from functools import reduce
>>> reduce(multiply, range(1,5))  # 前面的变量是一个行为 1×2--->2
                                  # 2×3---->6
				  # 6×4---->24
24

```

### sorted高高阶函数
```python
>>> sorted([x * (-1) ** x for x in range(10)]) # x * (-1)^x  奇数保持不变，偶数变负数
[-9, -7, -5, -3, -1, 0, 2, 4, 6, 8] # 默认 升序排序

>>> sorted([x * (-1) ** x for x in range(10)], reverse=True)
[8, 6, 4, 2, 0, -1, -3, -5, -7, -9] # reverse=True 降序排序

>>> sorted([x * (-1) ** x for x in range(10)], key=abs) # 升序排序前，按 key行为处理一下，再排序，还按key之前的数
[0, -1, 2, -3, 4, -5, 6, -7, 8, -9]

>>> sorted([x * (-1) ** x for x in range(10)], reverse=True, key=abs)
[-9, 8, -7, 6, -5, 4, -3, 2, -1, 0] # 降序排序前，按 key行为处理一下，再排序，还按key之前的数
# max min同理理
```



### partial高高阶函数   指定函数行为 成 新的函数
```python
>>> from functools import partial

>>> abs_sorted = partial(sorted, key=abs) # 指定行为 合成函数=========

>>> abs_sorted([x * (-1) ** x for x in range(10)])
[0, -1, 2, -3, 4, -5, 6, -7, 8, -9]
>>> abs_reverse_sorted = partial(sorted, key=abs, reverse=True)
>>> abs_reverse_sorted([x * (-1) ** x for x in range(10)])
[-9, 8, -7, 6, -5, 4, -3, 2, -1, 0]

```

## 匿匿名函数 lambda

	定义:
	     使用lambda表达式创建的函数,函数本身没有名字
	特点:
	     只能使用纯表达式,不能赋值,不能使用while和try等块语句句
	语法: 
	     lambda [arg1 [,arg2 [,arg3......]]]: expression
	什么是表达式? 
	     Expressions get a value; Statements do something
	用 lambda 与用 def 的区别在哪
	写法上:
	   1. def可以用代码块,一个代码块包含多个语句句
	   2. lambda只能用单行表达式,而表达式仅仅是单个语句中的一一种
	结果上:
	   1. def语句一一定会增加一一个函数名称
	   2. lambda不会,这就降低了变量名污染的风险
	Compound Statements > Simple Statements > Expressions
	 代码块    --->             单行语句    --->     表达式
	能用一一个表达式直接放到 return 里返回的函数都可以用 lambda 速写


> 能用用一一个表达式直接放到 return 里里里返回的函数都可以用用 lambda 速写

```python
>>> def multiply(a,b): return a * b
...
>>> multiply(3,4)
12
>>> multiply_by_lambda = lambda x,y: x * y
>>> multiply_by_lambda(3,4)
12
```

> List + lambda 可以得到行为列列表 匿名函数lambda+列表推导

```python
>>> f_list = [lambda x: x + 1, lambda x: x ** 2, lambda x: x ** 3]
#           行为      x+1              x^2               x^3
# 函数式集合 

>>> [f_list[j](10) for j in range(3)]
[11, 100, 1000]


# 匿名函数lambda+列表推导

[lambda 变量:表达式 for 变量 in 列表]

[lambda 变量:表达式 for 变量 in 列表 if 条件]
```

> 问题=========
```python
fun = [lambda x: x*i for i in range(4)]  # ： 冒号前面为 lambda 函数的参数
for item in fun:
    print(item(1))
    
上述式子的输出结果：
预计结果为：0, 1, 2, 3
实际输出为：3, 3, 3, 3

分析: lambda x: x*i 为内层（嵌）函数，他的命名空间中只有 {'x': 1} 没有 i ，
     所以运行时会向外层函数（这儿是列表解析式函数 [ ]）的命名空间中请求 i 
     而当列表解析式运行时，列表解析式命名空间中的 i 经过循环依次变化为 0->1->2->3 最后固定为 3 ，
     所以当 lambda x: x*i 内层函数运行时，去外层函数取 i 每次都只能取到 3
     
解决： 给内层函数 lambda x: x*i 增加参数
给内层函数 lambda x: x*i 增加参数，命名空间中有了用来存储每次的 i ,
即改成 
[lambda x, i=i: x*i for i in range(4)] 
这样每一次，内部循环生成一个lambda 函数时，
都会把 --i--作为默认参数传入lambda的命名空间
循环4次实际lambda表达式为：
第一次：lambda x, i=0 第二次：lambda x, i=1 第三次：lambda x, i=2 第四次：lambda x, i=3

fun = [lambda x, i=i: x*i for i in range(4)]
for item in fun:
    print(item(1))

#输出结果为：
0
1
2
3   


# lambda 函数 内部没有 i
>>> f_list = [lambda x: x**i for i in range(5)]  # 最后i 都会变成4 4个函数 x^4
>>> [f_list[j](10) for j in range(5)] 
[10000, 10000, 10000, 10000, 10000]


# lambda 函数 内部 添加 i 参数
>>> f_list = [lambda x,i=i: x**i for i in range(5)] # 每次的 i 都会从  range(5) 中对应的位置取出来
                                                    # x^0 x^1 x^2 x^3 x^4
>>> [f_list[j](10) for j in range(5)]
[1, 10, 100, 1000, 10000]


```

# 变量作用域 声明 赋值 调用 global公有 nonlocal非公非私 local私有

> 3种作用域；5次调用；3次赋值；0次声明
```python

a = 1
print(a) # 全局 global

def func_enclosed():
    a = 2
    print(a) # nonlocal 非局部
    def func_local():
        a = 3
        print(a) # local 局部
    func_local()
    print(a) # nonlocal 非局部
    
func_enclosed()
print(a) # 全局 global

>>>
1
2
3
2
1
# 总结：无声明的情况下，赋值即私有，若外部有相同变量名则将其遮挡
```

> 3种作用域；5次调用；1次赋值；0次声明

```python
a = 1
print(a)
def func_enclosed():
    #a = 2
    print(a)
    def func_local():
        #a = 3
        print(a)
    func_local()
    print(a)
func_enclosed()
print(a)

>>>
1
1
1
1
1
# 总结：无赋值情况下，变量访问依据LEGB法则
```

> 3种作用域；5次调用；3次赋值；1次 global 声明(非局部nonlocal 中修改)

```python
a = 1
print(a) # 全局 global
def func_enclosed():
    global a # global声明
    a = 2
    print(a) # nonlocal 转换成 global
    def func_local():
        a = 3
        print(a) # local 局部
    func_local()
    print(a) # nonlocal 转换成 global
func_enclosed()
print(a)     #  global 已被 nonlocal  修改

>>>
1
2
3
2
2
# 总结：内层函数可以通过声明的方式直接修改外部变量
```



> 3种作用域；5次调用；3次赋值；1次 global 声明(局部local 中修改)

```python
a = 1
print(a)# 全局 global

def func_enclosed():
    a = 2
    print(a) # 非局部 nonlocal
    def func_local():
        global a # 全局声明
        a = 3
        print(a) # 局部local 转变成 全局
    func_local()
    print(a)
    
func_enclosed()
print(a)  # 全局 已被 局部local 中修改

>>> 
1
2
3
2
3
# 总结：位于最内层的函数，通过 global 声明，会越过中间层，直接修改全局变量
```

> 3种作用域；5次调用；3次赋值；2次 global 声明

```python
a = 1
print(a)# 全局 global

def func_enclosed():
    global a
    a = 2
    print(a) # nonlocal 转换成 global
    
    def func_local():
        global a
        a = 3
        print(a) # 局部local 转变成 全局
    func_local()
    print(a)
    
func_enclosed()
print(a) # 全局 已被修改 值为最近一次修改时的值
>>> 
1
2
3
3
3
# 总结： global 声明其实是一种绑定关系，意思是告诉解释器，
# 不用新创建变量了，我用的是外面那
个
```

> 3种作用域；5次调用；3次赋值；1次 nonlocal 声明

```python
a = 1
print(a) # 全局

def func_enclosed():
    a = 2
    print(a)# 非局部
    
    def func_local():
        nonlocal a # 非局部声明
        a = 3
        print(a)   # 局部转变成 非局部
    func_local()
    print(a)  # 非局部，已被 局部中修改
    
func_enclosed()
print(a) #全局

>>>
1
2
3
3
1
# 总结：位于最内层的函数，如果仅想修改中间层变量，而不是全局变量，可使用 nonlocal 关键字

```

> 3种作用域；5次调用；3次赋值；1次 nonlocal 声明；1次 global 声明

```python
a = 1
print(a)
def func_enclosed():
    global a
    a = 2
    print(a)  # nonlocal 已经转变成 global
    
    def func_local():
        nonlocal a #已经没有非局部变量a了，会报错=========!=!
        a = 3
        print(a)
	
    func_local()
    print(a)
func_enclosed()
print(a)
# 总结： nonlocal 只能绑定在中间层定义的变量，如果中间层变量被声明外全局变量，则会报错
```

## 金句：
	无声明的情况下，赋值即私有，若外部有相同变量名则将其遮挡
	想修改外部相同变量名，需要将外部变量声明
	根据外部变量的作用域级别不同，使用 global或者nonlocal
	
## Python解释器如何识别变量的作用域？
	先看出现了几层作用域
	再看变量出现的位置
	如果有对变量进行赋值操作，则再看是否声明为外部变量
	为什么会有 nonlocal 关键字？
	nonlocal 填补了 global与local 之间的空白
	nonlocal 的出现其实是一种权衡利弊的结果：私有之安全封装，全局之灵活共享
	而这也是闭包之所以出现的原因之一
	
# 闭包 简化的 面向对象编程================
	什么是闭包？
	定义：延伸了作用域的函数（能访问定义体之外定义的非全局变量）
	闭包是一种函数，它会保留定义函数时存在的外层非全局变量的绑定
	
## 求历史值的平均值的函数
```python
>>> avg(10)
10.0
>>> avg(11)  
10.5        # (10+11)/2
>>> avg(12)
11.0        # (10+11+12)/3 =11

```
## 第一个版本：用类实现
```python
class Averager():
    # 类构造函数
    def __init__(self):
        self.series = []
    # 调用执行函数
    def __call__(self, new_value):
        self.series.append(new_value) # 用列表记录历史输入值
        total = sum(self.series)      # 总和
        return total/len(self.series) # 和/数量

>>> avg = Averager()
>>> avg(10)
10.0
>>> avg(11)
10.5
>>> avg(12)
11.0
```


## 第二个版本：用闭包实现
```python
def make_averager():
    series = [] # 非局部
    
    def averager(new_value):
        series.append(new_value) # 访问上一级非局部的变量 (不用声明nonlocal即可访问可变对象)
        total = sum(series) # 总和
        return total/len(series) # 总和/数量
	
    return averager
    
>>> avg = make_averager()
>>> avg(10)
10.0
>>> avg(11)
10.5
>>> avg(12)
11.0
```

## 第三个版本：优化过的闭包 不用记录历史值
```python
def make_averager():
    count = 0 # 传入的数 的数量
    total = 0 # 当前总和
    
    def averager(new_value):
        # 上一级 非局部变量为 不可变对象 局部中不可直接访问，需要声明为 nonlocal对象
	# nonlocal count, total  # 注释掉 会报错
        count += 1 # 数量+1
        total += new_value # 总和
        return total / count # 总和/总数
	
    return averager
    
>>> from test import make_averager
>>> avg = make_averager()
>>> avg(10)
Traceback (most recent call last):
报错????
UnboundLocalError: local variable 'count' referenced before assignment

'''
思考：为什么刚才就可以修改？现在就不可以了呢？
还记得可变对象与不可变对象的区别吗？
Python 2中没有 nonlocal 关键字，只能用可变对象来临时性的解决中间层变量修改的问题
而 nonlocal 是Python 3中引入的一个官方的解决方案，以弥补内层函数无法修改中间层不可
变对象。
'''

```
## 闭包有什么用呢？
	1. 共享变量的时候避免使用了不安全的全局变量
	2. 允许将函数与某些数据关联起来，类似于简化版面向对象编程
	3. 相同代码每次生成的闭包，其延伸的作用域都彼此独立（计数器，注册表）
	4. 函数的一部分行为在编写时无法预知，需要动态实现，同时又想保持接口一致性
	5. 较低的内存开销：类的生命周期远大于闭包
	6. 实现装饰器



# Python中的 重头戏- 装饰器 ----为对象(函数... ) 化妆、装饰、打扮、粉饰
## 为什么会出现装饰器这个东西？
	名称管理
	显示调用
	就近原则
	充分复用

## 不引人多余变量名
	有时候，写一个闭包，仅仅是为了增强一个函数的功能
	功能增强完了之后，只对增强了功能的最终函数感兴趣
	装饰之前的函数引用就变得多余
	因而出现了 func = decorated_by(func) 这种即席覆盖的写法
	
> 修改一个函数的 help 命令的帮助文档

```python

def decorate(func):
    func.__doc__ += '\nDecorated by decorate.' # __doc__ 与函数帮助文档关联
    return func
    
def add(x, y):
    '''Return the sum of x and y.''' # 函数下面的 ''' ''' 注释为帮助文档
    return x + y
    
decorated_add = decorate(add) # 引入了新的变量名： decorated_add
# 不过原来的 add 函数一般情况下在后面的程序中不会再用到了

# 改进
add = decorate(add)

'''
分析：
没有新的变量名的引入，变量名使用效率提升
装饰函数的执行代码需要单独调用，可能不符合就近原则
'''
```

## 显示调用，就近原则
	装饰的次数多了，这种方式依旧显得有些多余而且会带来新的问题：在哪写这句代码比较好？
	为了编码更加优雅，保持显示调用，遵守就近原则，出现了 @ 这个语法糖
	

## 改进版二： @ 语法糖的横空出世

```python

# 装饰器函数，输入参数为 函数句柄， 对函数进行修改、装饰、化妆、粉饰
def decorate(func):
    func.__doc__ += '\nDecorated by decorate.'
    return func
    
@decorate # 表示使用 decorate函数 对 后面紧跟着的函数进行 修改、装饰
def add(x, y):
    '''Return the sum of x and y.'''
    return x + y
'''
分析：
通过语法糖，保证了装饰过程与原函数彼此之间的独立性
同时，还保证了两者代码之间的就近原则，形成一个有机的整体
问题：装饰定义函数与被装饰函数在同一个模块中实现，影响了复用效率
'''
```

## 分层封装，充分复用 改进版三：装饰器被单独封装在一个模块中


```python
# DecorateToolBox.py====

# 装饰器函数 封装在一个类中
class DecorateToolBox:

    # 类默认的装饰器，可以避免类的实例化，也就是不需要 类的构造函数 __init__
    # 就可以直接调用 类的 一个方法
    @classmethod # 内置装饰器
    def decorate(self, func):
        func.__doc__ += '\nDecorated by decorate.'
        return func

# test.py=====

from DecorateToolBox import DecorateToolBox
@DecorateToolBox.decorate
def add(x, y):
    '''Return the sum of x and y.'''
    return x + y
    
# 启动Python解释器：
>>> from test import add
>>> help(add) # help()函数打印传入函数的 帮助文档

# 解释器进入如下画面：
Help on function add in module test:

add(x, y)
    Return the sum of x and y.
    Decorated by decorate.
(END)

'''
分析：
将不同级别的功能模块封装在不同的文件中，是编写大型程序的基础
实现一个装饰器，并不一定需要写出一个闭包
类可以不依赖于一个实体而直接调用其方法，得益于 @classmethod 装饰器
'''
```
> 总结
	避免重复，充分复用：
	         装饰器的模块化使程序设计者完美的避免重复的前置和收尾代码.
	显示调用，就近原则：
	         装饰器是显式的，并且在需要装饰器的函数中即席使用.
		 

## 什么是装饰器？

	1. 装饰器是一个可调用的对象，以某种方式增强函数的功能
	2. 装饰器是一个 语法糖@ ，在源码中标记函数（此源码指编译后的源码）
	3. 解释器 解析源码 的时候将 被装饰的函数 作为 第一个位置参数 传给 装饰器
	4. 装饰器可能会直接处理被装饰函数，然后返回它（一般仅修改属性，不修改代码）
	5. 装饰器也可能用一个 新的函数 或 可调用对象替换被装饰函数（但核心功能一般不变）
	6. 装饰器仅仅看着像 闭包，其实功能的 定位 与 闭包 有重合也有很大区别
	7. 装饰器模式的本质是 元编程：在运行时 改变程序行为===
	8. 装饰器的一个不可忽视的特性：在模块加载时立即执行===
	9. 装饰器是可以堆叠的，自底向上逐个装饰===
	10.装饰器是可以带参数的，但此时至少要写两个装饰器===
	11.装饰器的更加Pythonic的实现方式其实是在类中实现 __call__() 方法
		 
### 装饰器的堆叠
```python
def deco_1(func):
    print("running deco_1...")
    return func
    
def deco_2(func):
    print("running deco_2...")
    return func
    
@deco_1 # 后装饰
@deco_2 # 先装饰 自低向上执行...
def f():
    print("running f...")
    
if __name__ == '__main__':
    f()
    #pass
    

# 运行结果：
# 两个装饰器函数 在模块加载时立即执行
running deco_2...
running deco_1...
# main函数调用的 f()函数打印
running f... 
```



## 带参数的装饰器
	既然装饰器 只能 接受一个位置参数，
	并且是 被动的接受 解释器 传过来的 函数引用====
	那么如何实现 带参数的装饰器呢？
	
> 问题分析：

	1. 限制条件一：装饰器本身只能接受一个位置参数
	2. 限制条件二：这个位置参数已经被 被装饰函数 的引用占据了
	3. 问题目标：希望装饰器 能够 使用外部传入的其他参数
	4. 推论：装饰器需要 访问 或 修改 外部参数
	
> 三种备选方案：

	1. 在装饰器内 访问 全局 不可变对象，
	   若需要修改，则使用 global 声明（不安全）
	2. 在装饰器内访问 外部 可变对象（不安全）
	3. 让装饰器成为 闭包的返回（较安全）
	
	方案：编写一个闭包，接受外部参数，返回一个装饰器(可使用闭包内的变量)

> 先来看一下参数化之前的装饰器：

```python
# 定义一个注册表，set()
registry = set()

# 装饰器，功能将传入的函数 注册到 注册表 里
def register(func):
    registry.add(func)# 加入到  注册表 里
    return func
    
# 使用 register 装饰器  装饰 f1()函数 ，注册 f1 函数
# 注意 @register() 后面不带() 
@register
def f1():
    print("running f1.") 

# 注册 f2 函数
@register
def f2():
    print("running f2.")

# 函数f3不注册
def f3():
    print("running f3.")
    
def main():
    f1()
    f2()
    f3()
    
if __name__ == '__main__':
    # 先打印注册表
    print(registry) # 注册表内 f1 和 f2 已经被注册
    main()
    
# 输出结果如下：
 # 注册表内 f1 和 f2 已经被注册
{<function f1 at 0x103906488>, <function f2 at 0x103906598>}
running f1.
running f2.
running f3.
```

> 参数化之后的装饰器，增加了开关功能：

```python
# 定义一个注册表，set()
registry = set()

# 最外层函数 负责接收参数
def register(flag=True):# 默认参数为 True
    # 连续两层函数 闭包
    # 闭包内的函数为 装饰器函数
    def decorate(func):
        if flag: # True时 注册
            registry.add(func)
        else:    # 否者删除
            registry.discard(func)
        return func
	
    # 最外层函数 返回的是装饰器函数
    return decorate

# 注意 @语法糖 装饰器 后带了 () 可传入参数后进行调用
@register()# 装饰器默认参数，进行注册
def f1():
    print("running f1.")
    
@register(False)# 不注册
def f2():
    print("running f2.")
    
@register(True)# 注册
def f3():
    print("running f3.")
    
def main():
    f1()
    f2()
    f3()
    
if __name__ == '__main__':
    print(registry)
    main()
    
# 输出结果如下：
# 注册了 f1 和 f2 函数
{<function f1 at 0x103a06620>, <function f3 at 0x103a06730>}
running f1.
running f2.
running f3.

'''
分析：
此时， register 变量被使用了两次
第一次是后面的调用： () （调用之后才变成一个装饰器）
第二次是前面的装饰： @ （装饰器符合仅能用于装饰器）
注意： register 不是装饰器； register() 或 register(False) 才是
'''

```

## 装饰器怎么用
	从模仿开始

## 装饰器的常见使用场景
	运行前处理：如确认用户授权
	运行时注册：如注册信号系统
	运行后清理：如序列化返回值

## 注册机制 或 授权机制（往往跟应用开发相关）
	函数的注册，参考上面的例子
	将某个功能注册到某个地方，比如Flask框架中URL的注册
	比如验证身份信息或加密信息，以确定是否继续后续的运算或操作
	比如查询一个身份信息是否已经被注册过，如果没有则注册，如果有则直接返回账户信息
## 参数的数据验证或清洗（往往跟数据清洗或异常处理相关）
	我们可以强行对输入参数进行特殊限制：

```python
# 装饰器，检测函数输入参数，必须全部为整数
def require_ints(func):
    def temp_func(*args):
        if not all([isinstance(arg, int) for arg in args]):
            raise TypeError("{} only accepts integers as arguments.".format(func.__name__))
        return func(*args)
    return temp_func
    
# 普通+    
def add(x,y):
    return x + y

# 使用装饰器
@require_ints
def require_ints_add(x,y):
    return x + y
    
if __name__ == '__main__':
    print(add(1.0, 2.0))# 正常
    print(require_ints_add(1.0, 2.0))

# 运行结果如下：
报错
3.0
TypeError: require_ints_add only accepts integers as arguments.


```

## 复用核心计算模块，仅改变输出方式
	让原本返回Python原生数据结构的函数变成输出JSON结构：
	
```python
import json

# 装饰器
def json_output(func):
    def temp_func(*args, **kw):
        result = func(*args, **kw)
	# 使用 json dump解析后返回 json字符串
        return json.dumps(result)
    # 返回一个新的函数
    return temp_func

# 返回字典的一个函数
def generate_a_dict(x):
    return {str(i): i**2 for i in range(x)}

# 使用装饰器，返回变成json结构
@json_output
def generate_a_dict_json_output(x):
    return {str(i): i**2 for i in range(x)}
    
if __name__ == '__main__':
    a, b = generate_a_dict(5), generate_a_dict_json_output(5)
    print(a, type(a))
    print(b, type(b))
    
# 运行结果如下：
{'0': 0, '1': 1, '2': 4, '3': 9, '4': 16} <class 'dict'> # dict key是 '' 单引号
{"0": 0, "1": 1, "2": 4, "3": 9, "4": 16} <class 'str'>  # json 字符串是 ""

```

## 总结

	装饰器作为Python四神器之一，功能非常强大（还有迭代器，生成器，上下文管理器）
	装饰器用好了能让程序复用性大大提高，代码也会变的非常优雅
	要理解装饰器，需要先理解一等函数，变量作用域，函数式编程，闭包
	要用好装饰器，要从模仿开始，多读别人的代码，多模仿，慢慢就有感觉了
	Python内置了很多的装饰器，功能都很强大，编写框架或大型程序的时候你一定会用到.
	
# 面向对象

## 什么是面向过程，什么是面向对象

### 面向过程式编程（符合人类思考方式的一种编程模式）
	1. 来了一个任务
	2. 对任务进行流程分解，得到任务的不同阶段
	3. 对不同阶段内的子任务分解，直到可以用分支语句和循环语句分解到几个表达式为止
	4. 将不同子任务连接在一起，共同完成总任务
	5. 识别重复性工作，抽象出基于任务或功能的函数，对整个程序进行重构
	6. 实现功能函数（func）与执行过程（main）分离
	7. 基础功能不变的情况下，下一个任务中可以复用的是函数
	
### 面向对象式编程
	1. 来个一个任务
	2. 找到任务中所有的利益相关方，并对利益相关方进行归类
	3. 找到每个利益相关方需要履行的职责和希望被满足的需求，并进行分类
	4. 将每个利益相关方类别的属性量化，并定义明确的行为，抽象出一个类别
	5. 将所有同一类的利益相关方用共同的类模板进行实例化
	6. 实现类别（class）与执行任务（main）的分离
	7. 识别不同类别之间的关系，梳理衍生关系
	8. 基于各种不同类别之间的衍生关系，抽象出基础类别
	9. 将所有类别重构至每个类别都基于各自的基类层层继承而来
	10. 利益相关方不变的情况下，下一个任务中可以复用的是抽象基类和各种派生类

## 举例说明
	任务：训练一个基于深度卷积神经网络的人脸识别模型，并将其封装成可调用API部署上线
### 基于过程分解出子任务：
	1. 找到合适的用于训练模型的数据库
	2. 定义模型的结构，loss函数，优化方法
	3. 根据模型的输入输出，将数据库制作为train, validation, test三个数据集
	4. 多次训练模型，寻找合适的超参数
	5. 找到训练出的最优方案，封装成 RESTful, API部署上线
	
### 基于对象分解出利益相关方：
	1. 数据：收集，验证，转换，加载，切分，增强
	2. 模型：结构，loss，优化器，超参数，训练流程，评估，使用
	3. 基础设施：模型定义环境，模型训练环境，模型部署环境
## 面向对象与面向过程的区别？
	面向过程思考的出发点是事情本身
	面向对象思考的出发点是假设所有事物都有生命，他们之间会怎么分工协作
	两种编程思路无明显优劣之分，一切只看适不适合
	评估方法：预测未来，找到复用概率最高的几个点
	将这几个点用如下原则进行初步评估：高内聚，低耦合
	按照以上方法评估之后，心中往往已经有了答案

	一般而言：
		注重复用和可维护性时，OOP多数要胜出的
		注重短期开发速度，而且是一次性的，面向过程肯定是首选
	举个例子，以下两种场景下，你的选择肯定是有区别的：
		1. 你未来一两年内都要做人脸识别研究；
		2. 你在一家创业公司，做人脸识别模型仅仅是老板或者产品经理拍脑袋想出来的……

## 比 设计模式 更重要的是 设计原则
### 面向对象设计的目标
	1. 可扩展：新特性 很容 添加 到现有系统中，基本 不影响 系统 原有功能
	2. 可修改：当修改 某一部分代码时，不会影响 到其他 不相关 的部分
	3. 可替代：用具有 相同接口 的代码去替换 系统中某一部分的代码时，系统不受影响
	
	以上三点就是用来 检测 软件设计 是否合理的要素

### 面向对象设计的SOLID原则
	1. 单一职责原则：设计出来的每一个类，只有一个引起这个类变化的原因
	2. 开闭原则：    对扩展开放，对修改封闭
	3. 替换原则：    父类能用的功能，换成子类一样可以用
	4. 接口隔离原则：接口设计要按需供给（类似微服务设计）
	5. 依赖倒置原则：抽象不依赖于细节，细节应该依赖于抽象（针对接口编程）
	
	遵循以上原则去设计面向对象程序，效果一般不会很差

### AI（场景）+ Python（语言）+ OOP（编程模式）：
	AI：    业务导向不明显，需求变动频率较低，实现和复现频率较高
	Python：虽然是一门面向对象语言，但与传统的面向对象并不相同
	OOP：   使用Python时，并不需要深入学习 OOP 或者 OOD 那些理论
	
	用Python这门面向对象语言去做 AI开发，只需要懂两句话：
		1. 高内聚，低耦合
		2. 抽象不变的接口，封装变化的细节
		
# OOP in Python 面向对象的Python实现
> 类的创建

```python
# 类实现 类名 一般大写
class Model:
    pass
    
def main():
    # 类实例化出来的 对象名字一般小写 model
    model = Model() # 调用 类 后面 加()
    print(model)

# 主函数
if __name__ == '__main__':
    main()
>>> 
    <__main__.Model object at 0x108fa05f8>
    
'''
要点：
1. 类名一般大写，实例化出来的对象，名称一般小写
2. 类在被定义时，也创建了一个局部作用域
3. 类名加上 () 生成对象，说明类被定义后便可以调用
4. 本质上讲，类本身就是一个对象
'''
```

> 类的数据绑定

```python
class Model:
    name = "CNN"
    
def main():
    print(Model.name)
    model = Model()
    print(model.name)
    model.name = "RNN"
    print(Model.name)
    print(model.name) # model 对象中 name 已经改变
    
if __name__ == '__main__':
    main()
    
# 输出：
CNN
CNN
CNN
RNN
    
'''
要点：
1. 类定义体中，可以定义自己的 属性(内定义的变量)，并通过 . 来引用
2. 类实例化出 对象 以后，创建了 新的局部作用域，也就是 对象自己的作用域
3. 对实例化出来的 对象引用属性时，先从自己的作用域找，未找到 则向上找，这里找到了类作用域
4. 实例化出来的对象是可以在 运行时绑定数据的.
'''
       
```


> 类的自定义实例化： __init__()  类构造函数

```python
class Model:
    name = "DNN"
    
    # 类构造函数，self 为类指针 c++ this
    def __init__(self, name):
        self.name = name
	
def main():
    cnnmodel = Model("CNN") # name = "CNN"
    rnnmodel = Model("RNN") # name = "RNN"
    # "DNN" "CNN" "RNN"
    print(Model.name, cnnmodel.name, rnnmodel.name)
    # 整体赋值
    cnnmodel.name, rnnmodel.name = "RNN", "CNN"
    # "DNN" "RNN" "CNN"
    print(Model.name, cnnmodel.name, rnnmodel.name)
    
if __name__ == '__main__':
    main()

'''
要点：
1. 类定义体中， self 指代实例化出来的对象
2. 没有跟在 self 后面的属性属于类属性
3. 可以使用 __init__() 函数自定义初始化方式
4. 隶属于类的方法是共享的，隶属于对象的方式是每个对象私有的
'''
```


> 对象方法, 类中定义函数, 类实例化后绑定到 对象上!!!!!

```python
class Model:
    name = "DNN"
    
    #类初始化函数 构造函数
    def __init__(self, name):
        self.name = name
	
    # 打印类内 name参数的一个函数
    def print_name(self):
        print(self.name)
	
def main():
    cnnmodel = Model("CNN")
    cnnmodel.print_name() # "CNN"
    cnnmodel.name = "RNN"
    cnnmodel.print_name() # "RNN"
    
if __name__ == '__main__':
    main()

'''
要点：
1. 类定义体中的方法默认情况下隶属于对象，而非类本身
2. 直接在类上调用方法时会报错
cnnmodel.print_name() 等价于 Model.print_name(cnnmodel)
那有没有隶属于类自己的方法呢？
'''

```

> 类方法 @classmethod 内置装饰器 语法糖

```python
class Model:
    # 类属性
    name = "DNN"
    
    # 类构造函数
    def __init__(self, name):
        self.name = name
	
    # 类实例化后才有的 方法 
    def print_name(self):
        print(self.name)
    
    # 不用实例化，类本身就有的 方法
    @classmethod # 装饰器
    def print_cls_name(cls): # 传入的为 cls 不是self
        print(cls.name)
	
def main():
    Model.print_cls_name() # 类本身的方法 "DNN"
    cnnmodel = Model("CNN")# 实例化一个对象
    cnnmodel.print_name()  # "CNN"
    cnnmodel.name = "RNN"  
    cnnmodel.print_name()  # "RNN" 
    Model.print_cls_name() # "DNN"
    
if __name__ == '__main__':
    main()
'''
要点：
1. 使用 @classmethod 与 cls 可以将方法绑定到类本身上
'''
```


> 属性封装

```python
class Model:
    # 双下划线开头，可以将数据 属性私有化，对于方法一样适用
    # 私有 属性 / 变量,需要通过公开的函数访问
    __name = "DNN"
    
    # 类构造函数
    def __init__(self, name):
        self.__name = name
	
    # 对象 方法 打印 私有属性/变量
    def print_name(self): # 传入self
        print(self.__name)
    
    # 类方法 打印 私有属性/变量
    @classmethod
    def print_cls_name(cls): # 输入cls
        print(cls.__name)
	
def main():
    Model.print_cls_name()  # "DNN"
    cnnmodel = Model("CNN") 
    cnnmodel.print_name()   # "CNN"
    # print(Model.__name)   # 报错 ，外部 不能直接访问 类内 私有变量
    # print(cnnmodel.__name)# 报错 
    
if __name__ == '__main__':
    main()
    
'''
要点：
1. 通过双下划线开头，可以将数据属性私有化，对于方法一样适用
2. 从报错信息也能看出， Model 是一个 type object ， cnnmodel 是一个 Model object
Python中的私有化是假的，本质上是做了一次名称替换，因此实际中也有为了方便调试而适
用单下划线的情况，而私有化也就全凭自觉了
'''

```


> 继承（隐式实例化）
```python
class Model:
    # 私有 属性 / 变量,需要通过公开的函数访问
    __name = "DNN"
    
    # 类构造函数，实例化
    def __init__(self, name):
        self.__name = name
	
    # 对象 方法 打印名字
    def print_name(self):
        print(self.__name)
    
    # 类 方法 打印名字
    @classmethod
    def print_cls_name(cls): # cls 属于 Model
        print(cls.__name)

# 类的继承，Model类为父类，CNNModel为子类
class CNNModel(Model):
    __name = "CNN"
    # 无 __init__(self,name):  隐式调用父类的
    
def main():
    cnnmodel = CNNModel("Lenet")# 实例化 "Lenet"
    cnnmodel.print_name()       # 
    CNNModel.print_cls_name()   # 打印 Model中的 name "DNN"
    
if __name__ == '__main__':
    main()
    
'''
要点：
1. 如果子类没有定义自己的 __init__() 函数，则隐式调用父类的
2. 子类可以使用父类中定义的所有属性和方法，但类方法的行为需要注意
使用了 @classmethod 后的方法虽然可以继承，但是方法里面的 cls 参数绑定了父类，
即使在子类中调用了类方法，但通过 cls 引用的属性依旧是父类的类属性
'''
    
```

> 继承（显示实例化）
```python
class Model:
    __name = "DNN"
    
    def __init__(self, name):
        self.__name = name
	
    def print_name(self):
        print(self.__name)
	
    @classmethod
    def print_cls_name(cls):
        print(cls.__name)
	
class CNNModel(Model):
    __name = "CNN"
    
    # 子类的 __inin__() 实例化函数，名字+层数
    def __init__(self, name, layer_num):
        # 必须显示调用 父类的 实例化函数
        Model.__init__(self, name)
	# 多传入的参数，需要执行赋值步骤
        self.__layer_num = layer_num
	
    # 子类 多出来的方法	
    def print_layer_num(self):
        print(self.__layer_num)
	
def main():
    cnnmodel = CNNModel("Lenet", 5)
    cnnmodel.print_name()      # "Lenet"
    cnnmodel.print_layer_num() # 5
    
if __name__ == '__main__':
    main()
'''
要点：
1. 如果子类中定义了 __init__() 函数，必须显示执行父类的初始化
'''
```


> 多态
```python
# 父类
class Model:

    __name = "DNN"
    
    def __init__(self, name):
        self.__name = name
	
    def print_name(self):
        print(self.__name)
	
    @classmethod
    def print_cls_name(cls):
        print(cls.__name)
#子类1	
class CNNModel(Model):

    __name = "CNN"
    
    def __init__(self, name, layer_num):
        # 子类初始化函数  需要显示调用父类的 初始化函数
        Model.__init__(self, name)
        self.__layer_num = layer_num # 层数
	
    def print_name(self):
        print(self.__name)
        self.print_layer_num() # 调用打印层数 方法
	
    def print_layer_num(self):
        print("Layer Num: ", self.__layer_num)
	
#子类2 
class RNNModel(Model):

    __name = "RNN"
    
    # 层名字 和 类型
    def __init__(self, name, nn_type):
        # 子类初始化函数 必须显示调用 父类的 实例化函数
        Model.__init__(self, name)
        self.__nn_type = nn_type # 层类型
	
    def print_name(self):
        print(self.__name)   # 名字
        self.print_nn_type() # 类型
	
    # 层类型
    def print_nn_type(self):
        print("NN Type: ", self.__nn_type)

# 传入类
def print_model(model):
    # 打印类的 print_name()方法
    model.print_name()
	
def main():
    model = Model("DNN") # 父类 实例化对象 DNN
    cnnmodel = CNNModel("CNN", 5)      # 子类 CNN 5层
    rnnmodel = RNNModel("RNN", "LSTM") # 子类 RNN LSTM类型
    
    # 打印三个 对象的 打印模型 介绍函数
    [print_model(m) for m in [model, cnnmodel, rnnmodel]]
    
    
if __name__ == '__main__':
    main()
>  输出：
DNN  # 名字
CNN  # 名字 
Layer Num: 5  # 层数
RNN  # 名字
NN Type: LSTM # 类型

'''
要点：
1. 多态的设计就是要完成对于不同类型对象使用相同的方法调用能得到各自期望的输出
2. 在数据封装，继承和多态中，多态是Python设计的核心，也叫鸭子类型
'''
```


### 面向对象的重要特性总结
	封装：只需知道我能做什么，不需要知道我怎么做的, 隐藏
	继承：纵向复用，比如：每个技能树上的节点，都有类似的特征
	多态：横向复用，比如：不同技能树上的节点，都有物理攻击和魔法伤害
	
	
	Python的设计初衷是强调多态，也就是所谓的鸭子类型，明白了Python的设计初衷后，
	我们编写的Python代码要在一定程度上避免以下三种情况（虽然写成这样也是可以的）：
		1. C语言般的过程式编程；
		2. C++或Java般的传统面向对象编程；
		3. Lisp般的函数式编程；


## Pythonic OOP
### 关于双下划线
	怎么读
	_name ：     single underscore name
	__name ：    double underscore name
	__name__ ：  dunder name     (double  ender ---> dunder)
	__init__() : dunder init method（function）
### 双下划线开头和结尾的变量或方法叫什么
	类别： special； magic； dunder
	实体： attribute(属性/变量)； method(方法/函数)
	
### 如何认识Python的special method（官档）

	special method： method with special name（dunder）
	Why use it?： A class can implement certain operations
	              that are invoked by special syntax
	original intention of design： operator overloading
	
	Allowing classes to define their own behavior with 
	     respect to language operators.

 尽可能的保证Python行为的一致性（Special Method 更像是一种协议）
 
	一切都是对象
		Python中的数据模型是 Objects
		Python程序中所有数据都是用 objects 或者 objects 之间的关系表示的
		甚至 Python代码都是 objects

	Objects 的组成:
	  1. identity （标识，身份证号码） 
	     当 objects 创建后，identity再也不会改变直到被销毁。
	     
	     a = 1.0
	     print(id(a))
	     b = 1.0
	     print(id(b))  # 和上面一致
	     
	     要点：
	       1. 变量存的是创建的 object 的identity
	       2. 创建出来的不同的 object 有不同的identity
	       3. 变量的id变了不是因为 object 的identity变了，⽽是对应的 object 变了
	       4. 对于immutable object，计算结果如果已经存在可直接返回相同的identity
                      immutable object，不可变对象
		      
	  2. type  (类型)
	  
	     a = 1.0
	     print(type(a))
		
	     当 objects 创建后，其type也是不会改变的
	     type() 函数返回一个 Object 的type
	     type决定了一个 object 支持哪些运算，可能的值在什么范围内

          3. value (值)
	     有些 Objects 的value是可以改变的： 可变对象 mutable object.
	     有些 Objects 的value是不能改变的： 不可变对象 immutable object.
	     需要注意当一个 Object 是个 container 的情况.
	     一个 Object 的type决定了它是否mutable.
	     
	     当我们聚焦于Container的values时，我们关注的是value.
	     当我们聚焦于Container的mutability时，关注的是identity.


> Special Method and Attribute __ 双下划綫 的函数和变量

	每一个class在定义的时候如果没有继承，都会隐式继承 object 这个superclass(超类)
	每一个⾃定义的class在Python中都是⼀个type object

```python
# 父类
class X:
    pass
    
# 子类    
class Y(X):
    pass
    
def main():
    x = X() # 实例化一个X 对象
    y = Y() # 实例化一个Y 对象
    print(x.__class__.__name__) # 打印 实例化该 对象 的 类的名字  X
    print(y.__class__.__name__) # Y
    
    print(X.__class__.__name__) # 实例化 类 X 的 类名字为 type
    print(Y.__class__.__name__) # type
    print(x.__class__.__base__.__name__) # 对象x 的类X 的基类的名字 object
    print(y.__class__.__base__.__name__) # 对象y 的类Y 的基类的名字 X
    print(X.__class__.__base__.__name__) # 类X 的 类type 的继承来源  object
    print(Y.__class__.__base__.__name__) #  object
    
if __name__ == "__main__":
    main()

'''
要点：
1. object.__class__  # 该对象的原型 类
2. class.__name__    # 类 的 名字
3. class.__base__    # 类 的 父亲类，继承来源 
4. 注意：链式执行
'''

```

> __ 方法
```python

class X:
    pass
    
class Y:
    # 类帮助文档  __doc__
    """Class Y"""
    
    # 类似 运算符、函数运算 重载================
    
    # print(类) 打印的函数=== 
    def __str__(self):
        return "{} object".format(self.__class__.__name__)
	
    # len(类) 执行的函数
    def __len__(self):
        return 10
    
    # 逻辑操作，执行函数  donder bool
    def __bool__(self):
        return False
	
def check_bool(x):
    if x:
        print("I'm {}. My bool value is True.".format(str(x)))
    else:
        print("I'm {}. My bool value is False.".format(str(x)))
	
def main():
    x = X() # 实例  () 需要 和c++不同
    y = Y()
    print(x)
    print(y)
    # print(len(x))
    print(len(y))
    check_bool(x) # 类实例化对象 默认为 true
    check_bool(y)
    print(X.__doc__) # 无 帮助文档 None
    print(Y.__doc__) # 有帮助文档
    
if __name__ == "__main__":
    main()

'''
要点：
1. 之所以要实现special method，是为了让⾃定义的class与Python的内置函数⽆缝衔接
2. Python有⼤量的内置函数，⽽这些函数⼤部分都是调⽤的对象⾥的special method
3. 想查看Python中到底有多少 special method：

'''

```


> 类 属性  Attribute
```python
class X:
    pass
    
if __name__ == "__main__":
    # print(X.a)
    X.a = "a" # 不用实例化，就可使随便绑定 变量
    print(X.a)  # a
    X.a = "aa"
    print(X.a)  # aa
    del X.a
    # print(X.a)# 出错
'''
说明：
1. 默认情况下， CRUD都⽀持，⽽且是在public情况下都⽀持（除了双下划线开头的）
2. 如果对象中没有这个Attribute，访问时会报错
'''

```


> Property（长得像 Attribute 的Method）（推荐使用）

	Property的设计初衷：
	代码复用
	延迟计算
	更加规范的对象属性访问管理
	场景：我要减肥，需要监控BMI指标，但是只能测量体重，每天更新体重，隔几天看一次BMI指数

```python
class X:
    def __init__(self, w, h):
        self.w = w
        self.h = h
        self.BMI = w / h ** 2 # 体重/身高的平方
	
def main():
    x = X(75, 1.83)
    print(x.BMI) # 22.395413419331717
    x.w = 74
    x.w = 73
    x.w = 72 # 体重慢慢降低===
             # 
    print(x.BMI) # 22.395413419331717  BMI只在初始化 __inin__ 时计算一次
    
if __name__ == "__main__":
    main()

```



> 
```python

class X:
    def __init__(self, w, h):
        self.__w = w
        self.__h = h
        self.BMI = w / h ** 2
	
    # 更新体重w时，需要更新 BMI指数
    def update_w(self, w):
        self.__w = w       # 更新体重w
        self._update_bmi() # 更新 BMI指数
	
    def _update_bmi(self):
        self.BMI = self.__w / self.__h ** 2
	
def main():
    x = X(75, 1.83)
    print(x.BMI)
    x.update_w(74)# 更新体重w时，也更新 BMI指数
    x.update_w(73)
    x.update_w(72)
    print(x.BMI)
    
if __name__ == "__main__":
    main()
'''
分析：
1. w变为私有，更新需要通过对象方法类执行，并将BMI的更新放于其中，实现功能逻辑
2. BMI属性依旧可以被外部访问和修改
3. 与w相关的代码全部被更改
4. 无论BMI属性是否被访问，每次w更新均会更新BMI，造成一定的计算资源浪费
'''
```


> 改进二 
```python
class X:

    def __init__(self, w, h):
        self.w = w
        self.h = h
	
    # 需要获取  bmi是 才更新BMI
    def get_bmi(self):
        return self.w / self.h ** 2
	
def main():
    x = X(75, 1.83)
    print(x.get_bmi())
    x.w = 74
    x.w = 73
    x.w = 72
    print(x.get_bmi()) # 需要获取  bmi是 才更新BMI
    
if __name__ == "__main__":
    main()
    
'''
分析：
1. 保持w和h属性可以随意更改， bmi指数仅在被访问时实时计算出结果.
2. 访问BMI的方式由 属性 改为方法，造成一定程度上的代码修改.
3. 在w更新频率高于BMI访问频率时，节省了计算资源.
4. 当w未更新，却多次调⽤BMI指数时，造成了重复计算.
'''
    
```


> 改进三：
```python

class X:

    def __init__(self, w, h):
        self._w = w
        self._h = h
        self._bmi = w / h ** 2
	
    def get_w(self):
        return self._w
    
    # 设置类内参数
    def set_w(self, value):
        if value <= 0:
            raise ValueError("Weight below 0 is not possible.")
        self._w = value
        self._bmi = self._w / self._h ** 2
    
    def get_bmi(self):
        return self._bmi
    
    # 重点============ property======需要放在后面=====
    # 通过 接口函数 修改 类内参数
    # 实例化 一个 property对象，有两个方法
    w = property(get_w, set_w)
    # 实例化 一个 property对象， 有一个获取方法
    BMI = property(get_bmi)
    
'''
说明：
1. 从改进三中我们发现，传给Property对象的其实是函数名，那么最优雅的⽅式当属Decorator(装饰器)了
property(fget=None, fset=None, fdel=None, doc=None)
'''

def main():
    x = X(75, 1.83)
    print(x.BMI) # 这里的BMI 是 一个 property对象，只有 获取 权限
    x.w = 74     # 这里的w是 property对象，调用的是 set_w()方法，会更新BMI
    x.w = 73 
    x.w = 72
    print(x.BMI) # 获取 BMI 的值
    
if __name__ == "__main__":
    main()

'''
分析:
1. Property对象显式的控制属性的访问.
2. w被更改的时候更新BMI，充分避免了重复计算.
3. 很容易的增加了异常处理，对更新属性进行预检验.
4. 完美复用原始调用代码，在调用方不知情的情况完成功能添加.
'''

```


> 改进四： 使用 @property 进行装饰
```python

class X:

    def __init__(self, w, h):
        self._w = w
        self._h = h
        self._bmi = w / h ** 2
	
    # 使用 property 进行装饰======
    # 这里 扩展访问 方法
    @property
    def w(self):
        return self._w
	
    # 使用 w.setter 对w的设置进行封装,对w的 赋值函数=====
    @w.setter
    def w(self, value):
        if value <= 0:
            raise ValueError("Weight below 0 is not possible.")
        self._w = value
	
        self._bmi = self._w / self._h ** 2
    
    # 获取BMI 的装饰器=====
    @property
    def BMI(self):
        return self._bmi
	
def main():
    x = X(75, 1.83)
    print(x.BMI) # 获取BMI的值
    x.w = 74     # 对w进行设置，赋值
    x.w = 73
    x.w = 72
    print(x.BMI)
    
if __name__ == "__main__":
    main()
    

'''
说明：
1. 从改进三中我们发现，传给Property对象的其实是函数名，那么最优雅的方式当属Decorator(装饰器)了
property(fget=None, fset=None, fdel=None, doc=None)
使用 @Property 默认实现了 可读
被   @Property 装饰过的 method 可以通过 @method.setter 继续装饰单输入参数方法实现可写
'''
```


## Cross-Cutting and Duck Typing 交叉 鸭子类型
### 单继承 vs 多态
	单继承 保证了 纵向的   复用 和 一致性
	多态   保证了 跨类型的 复用 和 一致性
	
### 传统OOP vs 鸭子类型
	传统OOP基于类别进行设计，从 类别 出发 逐步扩展
	鸭子类型仅 考虑功能，从需要满足的功能 出发 进行设计
	
### 传统OOP的多态 vs 鸭值类型的多态
	传统OOP中的多态大多基于共同的基类进行设计
	Python中的 鸭子类型 无需考虑继承关系，
	实现了某个通用的接口就可以完成 多态设计（Special Method）


> 
```python

class X:
    def f1():
        pass
	
class Y(X):
    # 有f1() 和 f2()两个方法
    def f2():
        pass
	
class A:
    def f3():
        pass
	
def do_f1(x):
    x.f1()
def do_f2(x):
    x.f2()
    
def do_f3(x):
    x.f3()

# 多重继承  MixIn  赋能 进化 转基因 基因编程
class Z(Y, A):
    # 有f1() 、 f2()、f3() 三个方法
    pass
    
```
## 总结
	1. 通过给一个类实现一个个的Special Method，
	   你就让这个类越来越像Python的Built-in Class
	2. 实现Special Method是从语⾔衔接层面为你的Class赋能   __method__
	3. 实现Decorator是从通⽤的函数功能层面为你的Class赋能  @装饰器
	4. 通过Multi-Inheritance，利用MixIn的理念，
	   你可以为你的Class批量化的赋能  多重继承

## Python语⾔设计的三个关键词（其实是一件事的三个不同视角）
	Duck Typing  鸭子类型 功能 --> 类
	Consistency          一致性
	赋能                 进化


# 迭代器 Iterator
	迭代器是一个对象
	迭代器可以被 next() 函数调用，并返回一个值
	迭代器可以被 iter() 函数调用，并返回迭代器⾃⼰
	连续被 next() 调⽤时依次返回一系列的值
	如果到了迭代的末尾，则抛出 StopIteration 异常
	迭代器也可以没有末尾，只要被 next() 调用，就一定会返回一个值
	Python中， next() 内置函数调用的是对象的 __next__() 方法
	Python中， iter() 内置函数调用的是对象的 __iter__() 方法
	一个实现了迭代器协议的的对象可以被 for 语句循环迭代直到终止

> dorner next方法
```python
class XIterator:
    # dorner next方法===
    def __next__(self):
        return "Hello World"
	
def main():
    x_it = XIterator()
    #next() 会调用  dorner next方法
    [print(next(x_it)) for i in range(3)]
    
if __name__ == "__main__":
    main()
    
>>> 
Hello World
Hello World
Hello World

# 只要一个对象实现了 __next__() 方法，就可以被 next() 函数调用
```





> dorner next方法
```python
class XIterator:
    # 类初始化
    def __init__(self):
        self.elements = list(range(5)) # 列表 0,1,2,3,4
	
    # dorner next方法
    def __next__(self):
        if self.elements:
            return self.elements.pop() # 弹出元素
	    
def main():
    x_it = XIterator()
    [print(next(x_it)) for i in range(10)]
    
if __name__ == "__main__":
    main()
>>> 
4 
3 
2 
1 
0
None
None
None
None
None 
# 此时由于没有实现 __iter__() ，若用 for 语句迭代，会报错
```

> dorner iter方法
```python
class XIterator:
    def __init__(self):
        self.elements = list(range(5))
	
    # 迭代方法
    def __next__(self):
        if self.elements:
            return self.elements.pop()
	    
    # 返回 自己    
    def __iter__(self):
        return self
	
def main():
    x_it = XIterator()
    for x in x_it:
        print(x) # 4 3 2 1 0 None ...
	
if __name__ == "__main__":
    main()
'''
说明：
实现了 __iter__() 和 __next__() 后便可以被 for 语句迭代了
但此时程序⼀旦开始执⾏便不再停止， Why？
'''

```


> for 语句的内部实现
```python

for element in iterable:
    # do something with element
    
# create an iterator object from that iterable
iter_obj = iter(iterable)

# infinite loop
while True:
    try:
        # get the next item
        element = next(iter_obj)
        # do something with element
	
    except StopIteration: # 直到 StopIteration才会结束!!!!!!
        # if StopIteration is raised, break from loop
        break
'''
说明：
for 语句里用的是 iterable ，而不是 iterator
for 语句执执行的第一个操作是从一个 iterable 生成一个 iterator
for 语句的循环体其实是靠检测 StopIteration 异常来中断的
要想被 for 语句迭代需要三个条件：
       __iter__() 
       __next__()
       StopIteration   终止条件  主动抛出=====
       
如果我们可以从一个对象⾥获得一个迭代器（Iterator），那么这个对象就是可迭代对象
（Iterable） 迭代器都是可迭代对象（因为实现了 __iter__() ），
但可迭代对象不一定是迭代器.

'''
```



> __next__ 主动  抛出 raise  StopIteration 异常
```python
class XIterator:
    def __init__(self):
        self.elements = list(range(5)) # 列表元素
	
    # 迭代方法
    def __next__(self):
        if self.elements:
            return self.elements.pop()
	else:
	    raise StopIteration # 主动 抛出 StopIteration 异常
	    
    # 返回 自己    
    def __iter__(self):
        return self
	
def main():
    x_it = XIterator()
    for x in x_it:
        print(x) # 4 3 2 1 0
	
if __name__ == "__main__":
    main()
```


# 生成器 Generator
迭代器协议很有用，但实现起来有些繁琐，没关系，生成器来帮你
生成器在保持代码简洁优雅的同时，自动实现了迭代器协议

> 实现生成器的方式1： yeild 表达式
```python

# yield实现的 生成器函数 
def f():
    yield 1
    yield 2
    yield 3
    # 相比 return 是退出， yeild 更像是暂停
    # 暂停 继续 暂停 继续 ...
    # 使用 yeild 语句可以自动实现迭代器协议
    # 自动抛出 StopIteration 异常
    
def main1():
    f_gen = f()
    [print(next(f_gen)) for i in range(5)]

def main2():
    f_gen = f()
    for x in f_gen:
        print(x)
	
if __name__ == "__main__":
    main1()
    main2()
```

> 实现生成器的方式2： Generator 表达式 生成器表达式
```python
>>> [print(x) for x in (x ** 2 for x in range(5))]
0
1
4
9
16
[None, None, None, None, None]  # ???

sum([x ** 2 for x in range(10000000)]) # 对一个list(很大) 求和, 会爆内存
sum(x ** 2 for x in range(10000000))   # 生成器 传给了 sum，一个值一个值加和
```

## 为什么需要生成器  类似 动态规划 只保存 当前 的对象??
	1. 相比迭代器协议，实现生成器的代码量小， 可读性更高
	2. 相比在 List 中操作元素，直接使用生成器能节省大量内存
	3. 有时候我们会需要写出一个方法在内存中存放的无限数据流
	4. 你可以建立生成器管道（多个生成器链式调用）
	
> 生成成器表示全部的斐波那契数列
```python
def fibonacci():
    temp = [1, 1]# 最开始的两个数
    while True:
        temp.append(sum(temp))# 加入前面的和
        yield temp.pop(0)# 弹出第一个数，保证只保留最近的两个数
```

> 通过生成器管道模块化处理数据
```python
# 生成器1
def fibonacci():
    temp = [1, 1]# 最开始的两个数
    while True:
        temp.append(sum(temp))# 加入前面的和
        yield temp.pop(0)# 弹出第一个数，保证只保留最近的两个数
	
# 数据流 
# 生成器2
def dataflow():
    for x in fibonacci():
        yield x ** 2 # 对斐波那契数列的数 求平方
	
	
if __name__ == "__main__":
    for x in dataflow(): # 生成了 两个生成器，单个数单个数进行处理====
        print(x)
        if x > 100000:   # 超过一定值后退出 
            break


```

## 总结：关于Python中的迭代思维
	Python中有两类运算：
		1. 可以并行的 矢量化的运算：      Numpy
		2. 必须一个个的操作的迭代式运算： Generator 生成器

	Python中有两类数据：
		1. 内存中放得下的：数据量较⼩的数据
		2. 内存中放不下的：数据量较⼤或者⽆穷⼤的数据

	Python中有两种思维：
		1. Eager：着急派，需要的话，必须 全都准备好.
		2. Lazy： 懒惰派， “哎，干嘛那么着急，需要的时候再说呗”.

	Python中 处处是 迭代器，缺的是发现迭代器的眼睛.



 



# 异常处理


	什么是异常？
	   错误：比如代码语法有问题，程序无法启动；比如试图在除法中除以0
	   小概率事件：比如图像识别API遇到了图像尺寸为2x2的图像
	   
	为什么程序会出现异常？
	    程序的某一部分不由程序编写者控制（使用别人的数据；数据等待外部输入；
	                                  程序运行环境一致性问题）
	    程序编写者难以考虑到全部情况并预先提供处理方式
	    
	通常如何处理？
	    条件语句： if/else
	    异常处理： try/except/else/finally
	    
	Python中的异常及相关语法
	    Exception ： Python内置的 异常类
	    raise ：     抛出异常
	    try ：       尝试运行以下语句
	    except ：    在 try 语句之后，捕获某个异常，
	                  为空则捕获全部异常（很危险，难以debug）
	    else ：在 try 语句之后，如果没有捕获到异常，则执行
	    finally ：在 try 语句之后，无论是否捕获到异常，均执行
	    
	    

> 案例：主动抛出异常
```python
>>> raise Exception
>>> raise Exception("Hello, I'm Exception.")
```

> 案例：被动遇到异常
>>> 1 / 0
ZeroDivisionError: division by zero

> 案例：异常处理语句 - 未遇到异常

```python
try:
    print("Enter try.")  # 尝试运行
except:
    print("Enter except.") # 如果出错，则运行
else:
    print("Enter else.")   # 正常执行，则运行
finally:
    print("Enter finally.")# 都执行
    
>>>
Enter try.
Enter else.
Enter finally.

```


> 案例：异常处理语句 - 捕获全部异常
```python
try:
    print("Enter try.")
    1 / 0 # 除0异常
except: # 直接接:  捕获全部异常
    print("Enter except.") # 执行异常错误后的 分支
else:
    print("Enter else.")   # 不执行
finally:
    print("Enter finally.")# 都执行
```

> 案例：异常处理语句 - 捕获指定异常
```python
try:
    print("Enter try.")
    1 / 0
except ZeroDivisionError:# 除0异常
    print("Enter except ZeroDivisionError.") # 执行
except ArithmeticError:  #  算术误差 异常
    print("Enter except ArithmeticError.")
except: # 其他异常
    print("Enter except.")
else:
    print("Enter else.")
finally:
    print("Enter finally.")


```

> 案例：异常处理语句 - 捕获异常后仍抛出
```python
MODE = "DEBUG" # 调试模式
try:
    print("Enter try.")
    1 / 0
except:
    print("Enter except.") # 除0异常
    if MODE == "DEBUG":
        raise # 发现异常，抛出异常，终止程序
    else:
        print("Enter else.")
finally:
    print("Enter finally.")


```


> 案例：异常处理语句 - 捕获异常后显示异常信息但不抛出异常
```python
MODE = "WARN"
try:
    print("Enter try.")
    1 / 0
except Exception as e:
    print("Enter except.")# 打印
    if MODE == "DEBUG":
        raise
    elif MODE == "WARN":# 进入这个分支
        print(e)# 打印异常信息，不抛出信息，程序不会结束
else:
    print("Enter else.")
finally:
    print("Enter finally.")


```


> 案例：异常语句与循环的连⽤
```python

MODE = "WARN"

def main():
    while True:
        try:
            print("Please input your Python 3 Expression.")
            exp = input() # 输入 python3 表达式
            print("The result of your expression is", eval(exp))
	    # eval 执行表达式
        except Exception as e:
            if MODE == "DEBUG":
                raise # 直接抛出异常，结束程序
            elif MODE == "WARN":
                print(e) # 打印错误信息，程序不结束
            elif MODE == "STABLE":
	        # 给用户看到，重新输入信息
                print("Something wrong, please input again.")
        else:
	    # 输入正确，直接结束 while 循环
            break
if __name__ == "__main__":
    main()

```



> 异常的传播
```python

def f1():
    try:
        return 1 / 0 # 除0异常
    except Exception as e:
        print("This is f1.") # 报错===
        print(e)             # 打印异常
	
def f2():
    try:
        f1()
    except Exception as e:
        print("This is f2.")
        print(e)
	
def f3():
    try:
        f2()
    except Exception as e:
        print("This is f3.")
        print(e)
	
def main():
    try:
        f3()
    except Exception as e:
        print("This is main.")
        print(e)
	
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("This is __main__.")
        print(e)
	
>>> 输出
This is f1.
division by zero
	
```



> raise 抛出异常 异常传播
```python
def f1():
    try:
        return 1 / 0 # 除0异常
    except Exception as e:
        print("This is f1.") # 报错===
        print(e)             # 打印异常
	raise                # 抛出异常，会被下一级 捕获
	
def f2():
    try:
        f1()
    except Exception as e:
        print("This is f2.")
        print(e)
	
def f3():
    try:
        f2()
    except Exception as e:
        print("This is f3.")
        print(e)
	
def main():
    try:
        f3()
    except Exception as e:
        print("This is main.")
        print(e)
	
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("This is __main__.")
        print(e)
	
>>> 
This is f1.
division by zero
This is f2.
division by zero

```



> 
```python
def f1():
    try:
        return 1 / 0 # 除0异常
    except Exception as e:
        print("This is f1.") # 报错===
        print(e)             # 打印异常
	raise                # 抛出异常，会被下一级 捕获
	
def f2():
    try:
        f1()
    except Exception as e:
        print("This is f2.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获
	
def f3():
    try:
        f2()
    except Exception as e:
        print("This is f3.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获
	
def main():
    try:
        f3()
    except Exception as e:
        print("This is main.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获
	
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("This is __main__.")
        print(e)
>>> 
This is f1.
division by zero
This is f2.
division by zero
This is f3.
division by zero
This is main.
division by zero
This is __main__.
division by zero
	
```



> 程序崩溃===未拦截到
```python
def f1():
    try:
        return 1 / 0 # 除0异常
    except Exception as e:
        print("This is f1.") # 报错===
        print(e)             # 打印异常
	raise                # 抛出异常，会被下一级 捕获
	
def f2():
    try:
        f1()
    except Exception as e:
        print("This is f2.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获
	
def f3():
    try:
        f2()
    except Exception as e:
        print("This is f3.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获
	
def main():
    try:
        f3()
    except Exception as e:
        print("This is main.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获
	
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("This is __main__.")
        print(e)
	raise                # 抛出异常，会被下一级 捕获，释放小偷

>>>
This is f1.
division by zero
This is f2.
division by zero
This is f3.
division by zero
This is main.
division by zero
This is __main__.
division by zero
Traceback (most recent call last):
File "Exception-2.py", line 35, in <module>  # 模块级别
main()
File "Exception-2.py", line 27, in main
f3()
File "Exception-2.py", line 19, in f3
f2()
File "Exception-2.py", line 11, in f2
f1()
File "Exception-2.py", line 3, in f1
return 1 / 0
ZeroDivisionError: division by zero  # 除0错误
```


## python 内置异常
	异常名称               描述
	BaseException     所有异常的基类
	SystemExit	  解释器请求退出
	KeyboardInterrupt 用户中断执行(通常是输入^C)
	Exception         常规错误的基类
	StopIteration     迭代器没有更多的值
	GeneratorExit     生成器(generator)发生异常来通知退出
	SystemExit        Python 解释器请求退出
	StandardError     所有的内建标准异常的基类
	ArithmeticError   所有数值计算错误的基类
	FloatingPointError浮点计算错误
	OverflowError     数值运算超出最大限制
	ZeroDivisionError 除(或取模)零 (所有数据类型)
	AssertionError    断言语句失败
	AttributeError    对象没有这个属性
	EOFError          没有内建输入,到达EOF 标记
	EnvironmentError  操作系统错误的基类
	IOError           输入/输出操作失败
	OSError           操作系统错误
	WindowsError      系统调用失败
	ImportError       导入模块/对象失败
	KeyboardInterrupt 用户中断执行(通常是输入^C)
	LookupError       无效数据查询的基类
	IndexError        序列中没有没有此索引(index)
	KeyError          映射中没有这个键
	MemoryError       内存溢出错误(对于Python 解释器不是致命的)
	NameError         未声明/初始化对象 (没有属性)
	UnboundLocalError 访问未初始化的本地变量
	ReferenceError    弱引用(Weak reference)试图访问已经垃圾回收了的对象
	RuntimeError      一般的运行时错误
	NotImplementedError 尚未实现的方法
	SyntaxError       Python 语法错误
	IndentationError  缩进错误
	TabError          Tab 和空格混用
	SystemError       一般的解释器系统错误
	TypeError         对类型无效的操作
	ValueError        传入无效的参数
	UnicodeError      Unicode 相关的错误
	UnicodeDecodeError Unicode 解码时的错误
	UnicodeEncodeError Unicode 编码时错误
	UnicodeTranslateError Unicode 转换时错误
	Warning            警告的基类
	DeprecationWarning 关于被弃用的特征的警告
	FutureWarning      关于构造将来语义会有改变的警告
	OverflowWarning    旧的关于自动提升为长整型(long)的警告
	PendingDeprecationWarning 关于特性将会被废弃的警告
	RuntimeWarning            可疑的运行时行为(runtime behavior)的警告
	SyntaxWarning             可疑的语法的警告
	UserWarning               用户代码生成的警告


# 上下文管理器
> 
```python

```


> 
```python

```




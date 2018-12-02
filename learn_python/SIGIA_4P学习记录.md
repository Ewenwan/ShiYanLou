# 简介

[网址](https://www.sigai.cn/)

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
  # 输入
  name = input()
  print(name)
  name = input('input your name: ') # 带有提示符
  
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


# 多重List 的 浅拷⻉ 与深拷⻉ 

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


# 序列列的常⻅见内置方方法=
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


# list comprehension  Python的递推式构造列表
>>> sum([x for x in range(101) if x % 2 == 0]) # 0到100 内所有偶数的和
2550
>>> sum([1 if x % 3 == 2 else -1 if x % 3 == 1 else 0 for x in range(101)])
    # 先找 原始 range()   0到100内数 如果 除以3余数为2，就变成1; 如果除以3余数为1，就变成 -1;否则整除3，就变成0
    # [0 -1 1 0 -1 1 ...] 到 第99个数 加和刚好是0，而100 会变成-1，所以总序列求和后为 -1
-1

# List Comprehension与lambda以及map/reduce还有filter连用用功能更更强大大
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

## 匿匿名函数

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

> List + lambda 可以得到行为列列表

```python
>>> f_list = [lambda x: x + 1, lambda x: x ** 2, lambda x: x ** 3]
#           行为      x+1              x^2               x^3
# 函数式集合 

>>> [f_list[j](10) for j in range(3)]
[11, 100, 1000]
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
		
# OOP in Python




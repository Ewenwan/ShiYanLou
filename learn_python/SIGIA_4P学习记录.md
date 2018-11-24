# 简介

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






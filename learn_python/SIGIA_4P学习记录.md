# 1.简介

    a. 课程OKR  Objectives and Key Results 即目标与关键成果法
       目标，Objectives ： 掌握AI领域的Python开发
       成果1 KR1
       
# 2. python

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
 
# 3. 列表 List  可变有序集合   []
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


# 4. Tuple  '不可变' List  ()
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

# 5. Dict 字典   key:value  查找速度快  哈系函数实现 内存换时间  无序  key必须不可变
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
# 6. Set 集合  Dict 中 Key的集合，key是唯一的，不可重复，去重复的好方法



# 7. 可变 参数函数
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

# 8. 面向对象 类 
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

# 9. Python 代码的组织-模块
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






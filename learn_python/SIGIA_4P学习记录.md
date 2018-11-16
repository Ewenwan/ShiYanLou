# 1.简介

    a. 课程OKR  Objectives and Key Results 即目标与关键成果法
       目标，Objectives ： 掌握AI领域的Python开发
       成果1 KR1
       
# 2. python

> 输入输出
```python
  # 输出
  print("XXXXX")
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

> 数据类型
    
    整数 int
    浮点数 float
    字符串 string   "" 或 ''  '''  '''
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
 
# 列表 List  可变有序集合   []
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


# Tuple  '不可变' List  ()
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

# Dict 字典   key:value  查找速度快
```python



```






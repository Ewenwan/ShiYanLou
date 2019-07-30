# LinuxShell命令行及脚本编程实例详解

Linux 典藏大师系列丛书

shell 脚本的构成:

    1. shell 关键字 if ...else; for ...done
    2. shell 命令 export,echo,exit,pwd,return
    3. linux 命令 data rm mkdir cd 
    4. 文本处理功能 awk cut sed grep
    5. 函数 功能语句集合 /etc/init.d/functions
    6. 控制流语句 if...then...else

bash 登录脚本 ~/.bash_profile  打开shell 会执行的脚本

bash 退出脚本 ~/bash_logout 退出shell 会执行的脚本


## shell 变量
```sh
username=wanyouwen

或者

username="wanyouwen"   注意等号=前后不能有空格

赋值数字给变量

var=2

注意shell默认赋值是字符串赋值

如 var=$var+1

echo var

>>> 1+1  看到是不是2
如果想把算术表达式的值赋给一个变量，需要用let

let var=2+3

echo $var

5


将一个变量赋值给另外一个变量
a=3
b=$a
echo $b

将命令的结果赋值给变量
var='pwd'  或者 var=$(pwd) 
echo $var

获取命令行输入 获取read命令得到的值

$ echo -n "Enter var:"; read var
Enter var: 1234
$ echo var
1234
```

## 打印变量
```sh
最简单的使用 echo

echo $变量名

var=10
echo $var
echo ${var}
echo "value: $var"
echo "value: ${var}"

也可以使用 printf

printf  "格式" $变量

var = 111
printf "%s\n" $var   类似于c语言的printf()函数

%d  %i  带符号十进制格式打印
%o      无符号八进制
%u      无符号十进制
%x      无符号小写十六进制
%X      无符号大写十六进制
%f      浮点数格式
%e      双精度浮点数  <N>e<N>
%E      双精度浮点数  <N>E<N>
#c      以字符的格式
%s      以字符串的格式
%n      指定打印字符的个数
%%      打印一个%

转义字符
\"      打印一个双引号 "
\\ 
\a      警告音
\b      删除前一个字符
\f      换页符
\r      回车，回到本行开头
\n      换行，到下一行开头
\t      一个制表符
\v      竖直制表符

### echo中 使用转移字符  需要 -e 选项使能转移字符
$ echo -e "User name: $USER\tHome Directory: $HOME\n"
>>> User name: ywy  Home Directory: /home/wyw

######字符串#####
var=shell
$printf "%s\n" $var
shell
$printf "%1s\n" $var   #指定参数长度小于实际长度，按实际长度打印
shell
$printf "%1.3s\n" $var #小数点后指示打印的字符串长度
she
$printf "%1.6s\n" $var #小数点后指示打印的字符串长度
shell
$printf "%3.6s\n" $var #小数点后指示打印的字符串长度
shell
$printf "%6.6s\n" $var #指定长度大于实际长度，左边补空格
 shell                
$printf "%10.6s\n" $var #指定长度大于实际长度，左边补空格
     shell              #左边补了5个空格
$printf "%10.2s\n" $var #指定长度大于实际长度，左边补空格
     sh                 #左边补了5个空格，同时只打印前2个字符


####小数######
$varr=123.45678
$printf "%5.1f\n" $varr #打印总长度5(包含小数点)保留小数点后1位数字
1123.5
$printf "%6.2f\n" $varr #
1123.46
$printf "%7.3f\n" $varr #
123.457
$printf "%9.3f\n" $varr #打印长度9，保留小数点后3位，只有7位长度，左侧空格
  123.457
$printf "%10.3f\n" $varr #打印长度10，保留小数点后3位，只有7位长度，左侧空格
   123.457

```

## 变量引用 双引号 "$变量名" 将变量整体化
```sh

LIST="one two three"

for var in $LIST   # 将变量的值分成3个参数传递给了for循环
do
    echo "$var"
done
>>>
one
two
three
    
for var in "$LIST"   # 将变量的值作为一个整体传递给了for循环
do
    echo "$var"
done
>>>
one

```

## export 变量名=变量值  声明定义全局变量 多bash环境可用
## readonly 变量名=变量值  声明只读变量
## unset 变量名 删除 export 定义的全局变量，不能删除 readonly 只读变量

## 检查变量是否存在 变量名? 不存在提示语 或 ${变量名:? 不存在提示语}
```sh
JAVA_HOME=/usr/local/jdk
export JAVA_HOME
echo ${JAVA_HOME:? Error variable is not defined}
>>> /usr/local/jdk

unset JAVA_HOME
echo ${JAVA_HOME:? Error variable is not defined}
>>> -bash JAVA_HOME: Error variable is not defined


还有一种更简单的检查变量是否存在的方法 使用 if语句来判断
```

## 回调 历史命令
```sh
#### 回调 历史命令
history
>>>
会显示之前输入过的命令 
id1 命令
...
idn 命令

!! 输入两个! 会执行就一条指令
!+id号 会执行历史命令中对应id号的命令
!+历史命令开头的字符  会执行历史命令中有已字符开头的命令
```


## shell中的扩展 展开 繁殖 
大括号扩展 波浪号扩展 参数和变量扩展 命令替换 算术扩展 进程替换 单词拆分 和 文件名扩展

### 大括号扩展   前缀字符{字符1,字符2,...,字符n}后缀字符
```sh
echo a{b,c,d}e
>>> abe ace ade

echo {a..z}  # 按字母表顺序显示 a~z字母
>>> a b c d ... x y z

echo {0..10} # 显示0~10的数字
>>> 0 1 2 3 ... 9 10

echo {6..-4} # 显示6~-4的数字
>>> 6 5 4 3 ... 0 -1 -2 -3 -4

echo {g..a} # 显示g~a的字母 反向
>>> g f e d c b a

echo {1..3}{a..c}
>>> 1a 1b 1c 2a 2b 2c 3a 3b 3c

## 嵌套
echo a{{b,c}1,{d,e}3,f}h
ab1h ac1h ad3h ae3h afh

## 大括号扩展 繁殖命令
在 当前目录下创建三个文件夹 son1 son2 son3
mkdir ./{son1,son2,son3}

## bash 4.0 新功能 {起点..终点..步长}
echo {0..10..2}
>>> 0 2 4 6 8 10

echo {10..3..-2}
>>> 10 8 6 4 2

echo {a..k..2}
>>> a c e g i k

echo {0001..11..5}   # 指定形式
>>> 0001 0006 0011

```
### ~ 波浪号 指代用户主目录





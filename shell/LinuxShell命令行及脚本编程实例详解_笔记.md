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

\"      打印一个双引号 "
\\ 
\a      警告音
\b      删除前一个字符
\f      换页符
\r      回车，回到本行开头
\n      换行，到下一行开头
\t      一个制表符
\v      竖直制表符


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



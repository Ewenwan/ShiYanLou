# LinuxShell命令行及脚本编程实例详解

Linux 典藏大师系列丛书

shell 脚本的构成:

    1. shell 关键字 if ...then else; for ...done; while do done
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


# 使用(())
cnt=0

cnt=$(($cnt+1))

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
```sh
cd ~
pwd
>>> /home/wyw
cd ~wyw
pwd
>>> /home/wyw

~+ 指代 shell变量 PWD的值
echo ~+
>>> /tmp
echo $PWD
>>> /tmp

~-  指代 shell变量 LODPWD 的值
echo ~1
>>> /home/wyw
echo $LODPWD
>>> /home/wyw

```

### 文件名扩展
```sh
* 匹配任意字符，包括空格,多个字符,单个字符
? 匹配任意单个字符
[xyx] 匹配阔靠内的任意单个字符

ls ./*.txt
>>> 列出 当前目录下 所有以.txt结尾的文件

ls ./[abc]*.cfg
>>> 列出 当前目录下 以a或b或c开头，以.cfg结尾的文件

ls ./image?.jpg
>>> 显示所有 image1.jpg\image2.jpg\...\image9.jpg 文件

ls ./image1[0-9]*.jpg
>>>  列出 以 image10 \ image11 \...\image19开头，以.jpg结尾的文件
```

### 命令别名

```sh
alias 别名='完整命令'

例如：
alias ll='ls -l'

是 cp\mv\rm等命令交互式地执行并解释执行了哪些操作
alias cp='cp -iv'
alias mv='mv -iv'
alias rm='rm -iv'

查看磁盘使用情况

alias dus='df -h'
查看所有定义的别名命令
alias
不带参数

```

## 第三章 常用 shell 命令
### ls 列出文件名和目录
```sh

ls
ls -l
-l 显示全部文件信息:
第一个字符：-,普通文件 d,文件夹 s,套接字文件 l,链接文件
后面9个字符: 文件权限，rwx 读r 写w 执行x 分别为 所有者、用户组、其他用户的权限

ls -lh 文件大小显示为KB MB GB 单位

ls -ls 安装文件大小顺序列出文件和文件夹

ls -a 显示所有文件

# 生成文件名变量
prototxt_file_name=$(ls *.prototxt)
#.prototxt文件数量
count=$(grep -c ".prototxt" $prototxt_file_name | wc)

```


### cat 显示文件 more  less head tail file

```sh
cat 查看文件内容 连接文件 创建一个或多个文件 重定向输出到终端或文件

# 查看文件内容   cat 正序显示   tat 倒序显示文件内容
cat wyw.txt  / cat -n wyw.txt 显示行号  / cat -b wyw.txt 显示非空白行行号 

cat -e  wyw.txt 会在每行末尾 多显示 $ 符号 （多行内容转换成一行时很有用）

#使用命令行输入创建文件 并连接多个文件

cat >test1
>>> hello!
cat >test2
>>> world!
cat test1 test2 > test3
#显示 test3
cat test3
>>> 
hello!
world!


# more 分屏显示文件内容
more /etc/inittab  / more -10 /etc/inittab 指定一次显示的行号

# less 查看文件速度快 可快速查找关键字
less /etc/inittab 
输入 /关键字  然后回车 向后搜索关键字 
     ?关键字  然后回车 向前搜索关键字 
     
Ctrl+B 向回翻一个窗口的内容 Ctrl+F 向后翻一个窗口的内容 


# 显示 文件 头 head  显示 文件尾 tail

head /etc/inittab / head -10 /etc/inittab   显示开头前10行
tail /etc/inittab / tail -20  /etc/inittab  显示末尾倒数20行


# file 文件名 显示文件类型

file /etc/inittab 
>>> /etc/inittab: ASCII English text
file -i /etc/inittab 
```


### wc 查看文件统计信息
```sh
wc 文件名
>>> 行数 单词数 字节数 文件名
wc -l 文件名  只统计行数
wc -w 文件名  只统计单词数
wc -c 文件名  只统计字节数
wc -L 文件名  统计文件中最长行的长度

```

### find 查找文件/目录
```sh
# 查找文件

find ./wanyouwen/*.txt  # 查找 ./wanyouwen 目录下的 txt文件
find ./wanyouwen -type f -name "*.txt" 
find ./wanyouwen -type f -perm 0777  查找权限是777的文件
find ./wanyouwen -type f ! -perm 777  查找权限不是777的文件
find ./wanyouwen -type f -perm /a+x  查找可执行文件

find ./wanyouwen -type f empty  查找空文件

# 查找文件夹
find ./ -type d -name wanyouwn
find ./wanyouwen -type d empty  查找空文件夹(目录)

```

### touch 创建文件  mkdir 创建文件夹  cp 复制文件/文件夹  ln -s 创建符号链接文件 mv移动/重命名文件/目录
```sh
# 创建文件
touch 文件1 文件2 文件3
touch -c 已有的文件名 只更新 文件时间戳
touch -m 已有的文件名 只更新 文件的修改时间

# 创建文件夹
mkdir 文件名1
mkdir -p 文件名2/文件夹3  递归创建 带有父目录

# 复制文件/文件夹
cp 文件1 ./xxx/ 复制

# 创建符号链接文件
ln -s  原地址  目标地址  # 在目标地址创建一个 原地址的快捷链接

# 移动/重命名文件/目录
mv 原地址文件/目录 新地址文件/目录

# 删除文件/目录
rm 文件名
rm -rf 文件夹
rm *.txt
rm *[0-9].txt
````

### 管理文件或目录权限
```sh
# 显示权限
ls -l
# 修改权限
chmod 777 文件   r(4)  w(2) x(1)
chmod a+r 文件
chmod a+w 文件
chmod a+x 文件
chmod o+r 文件
chmod o+w 文件
chmod o+x 文件
chmod u+r 文件
chmod u+w 文件
chmod u+x 文件
chmod g+r 文件
chmod g+w 文件
chmod g+x 文件
# 修改所有者 和 用户组
chown
chgrp
# 设置(用户、组)权限位
setuid
setgid

```
### 文本处理
#### 文本排序 sort
```sh

$ cat tep.txt
abc
def
ghi
def
$ sort tep.txt  # 将文件内容按字母顺序排序
abc
def
def
ghi
$ sort -u tep.txt # 排序 并去除重复的行
abc
def
ghi

$ cat digit.txt
10
30
20
100
$ sort -n digit.txt # 按数字大小排序
10
20
30
100
$ sort -n -r digit.txt # 按数字大小 倒序 排序
100
30
20
10

# 多列数字排序
$ cat  dig.txt
abc,30
def,10
ghi,20
def,50
$ sort   dig.txt # 默认按每行的第一列进行排序
abc,30
def,10
def,50
ghi,20

$ sort -t ',' -k2n,2 dig.txt # 按第二列 数字大小顺序排序 -t 指定分隔符 -k 指定列数
def,10
ghi,20
abc,30
def,50

$ sort -t ',' -k2nr,2 dig.txt # 按第二列 数字大小倒序排序 -t 指定分隔符 -k 指定列数 r倒序
def,50
abc,30
ghi,20
def,10

```
#### 文本去重 uniq   (unique)  独一无二
```sh
$ cat  tep.txt
aaa
aaa
bbb
ccc
ccc
$ uniq tep.txt  # 去除重复行
aaa
bbb
ccc
$ uniq -c  tep.txt  # 记录每行出现次数
2 aaa
1 bbb
2 ccc
$ uniq -d  tep.txt  # 只显示有重复的行，只显示一次
aaa
ccc
$ uniq -D  tep.txt  # 只显示有重复的行，显示所有
aaa
aaa
ccc
ccc
$ uniq -u  tep.txt  # 只显示不重复的行
bbb

$ uniq -w 3 tep.txt  # 限定每行的前3个字符来比较每行是否重复
$ uniq -f 1 tep.txt  # 跳过每行前1个字符来比较每行是否重复
```
#### 替换或删除字符 tr
```sh
# 大小写转换
$ echo linuxShell | tr [:lower:] [:upper:]
LINUXSHELL
$  echo linuxShell | tr a-z A-Z  # 功能同上 
LINUXSHELL

# 替换空格
$ echo "Hello world" | tr -s [:space:] '\t'
Hello   world   # 空格 换成 制表符  -s 多个连续的空格算一个

# 删除 字符 -d 选项
$ echo "Hello world" | tr -d a-z # 删除全部小写字母
$ echo "Hello world 12345" | tr -d [:digit:] # 删除全部数字
$ echo "Hello world 12345" | tr -cd [:digit:] # 删除全部数字以外的字符
```
#### 字符串查找 grep 常用 
```sh
grep  关键字 目标文件/目标文件夹/字符串
grep -r 关键字 目标文件夹   # 递归查找  或者 grep -R 关键字 目标文件夹
grep -c 关键字 目标文件/目标文件夹/字符串  # 查找出现的次数
grep -n 关键字 目标文件/目标文件夹/字符串  # 显示行号和对应出现的位置

```
#### 文件比较 diff
```sh
diff 文件1  文件2   # 比较两个文件的差异
diff -yw 文件1  文件2   # 并排显示两个文件的差异
```

### 其他常用命令
```sh
hostname  # 查看主机名
hostname 新主机名  # 临时修改主机名，下次重启恢复原主机名

who/w   # 列出系统登录用户
who -b  # 显示系统启动时间
who -l  # 显示系统登录进程时间
who -m  # 只显示与当前用户相关的信息


uptime   # 查看系统运行时间

uname    # 查看系统信息，内核名
uname -r # 内核版本信息
uname -p # 系统硬件CPU类型
uname -i # 硬件平台信息
uname -n # 打印主机名
uname -a # 显示上述所有信息

#### 获取日期信息 data 

year = $(data +%Y)
>>> 2019
year = $(data +%y)
>>> 19
mouth = $(data +%m)
>>> 08
mouth = $(data +%d)
>>> 10

week = 'data +%a' # 星期几

```

## 第四章 shell命令进阶
### 文档处理 和 归档 paste合并文件 dd备份和拷贝文件
```sh
### paste 合并多个文件的相应行到同一行
$ cat t.txt
>>>
1
2
3

$ cat d.txt
>>>
a
b
c

$ paste t.txt d.txt
>>>
1 a
2 b
3 c

$ paste -d'|' t.txt d.txt # 指定合并时的分隔符
1|a
2|b
3|c

$ paste -d':,' t.txt d.txt y.txt # 合并多个文件时指定多个分隔符


### 文档复制 dd
# dd if=输入文件的路径(默认标准输入) of=输出文件的路径(默认标准输出)

# 实例：
dd if=/dev/sda of=/dev/sdb   #将磁盘/dev/sda 数据复制到 磁盘/dev/sdb
dd if=/dev/dvd of=dvd.iso    #将磁盘/dev/dvd 数据复制变为一个iso文件
dd if=/dev/zero of=/dev/sda2 #擦除/dev/sda2分区的数据(格式化)

### 压缩和归档文件 gzip bzip2
### 解压缩文件    gunzip bunzip2
### 打包/解包文件  tar -czvf xxx（打包）  tar -xzvf xxx (解包)
```


### 监测和管理磁盘
```sh
### 挂载mount 和 卸载umount 存储介质 到 文件系统某节点

### df 报告文件系统磁盘空间利用率
df -a  # 显示所有文件系统信息
df -h  # 以GB MB 单位来显示文件大小 和 使用率等信息
df -t  # 显示文件系统类型
df -m  # 以为块大小单位显示文件系统信息

### du 评估文件空间利用率
du -h
du -sh
du -h --time # 显示大小和修改时间

```

### 后台执行命令 cron 大管家  at大将军执行者  &任务发布者
```sh
###  cron 大管家 
cron 是定时执行计划任务的守护进程，cron进程会从/etc/init.d 中自动启动。
cron进程会在目录 /var/spool/cron/crontabs/ 下搜索定时计划任务文件，并将这些定时计划任务载入内存。
cron进程还会读取 /etc/crontab 以及 /etc/cron.d 下的内容
cron进程会每分钟唤醒一次，审查所有存储的定时任务，是否按规定执行(大管家)

crontab -l
>>>60 6 * * 0  /home/xxx/backup  
# 每周日(0)的6点30分 指向该任务；前5个字段分别为 分钟 小时 日期 月份 星期; * 号表示任意，小时为24小时制。

###  at大将军执行者 在指定时间执行命令

at noon 在中午12点前 midnight 凌晨12点 teatime下午4点 ...

### &将任务放在后台执行   指令 &

### nohup  运行一个对挂起免疫的命令 

```



# 第二篇 shell 脚本编程 
## 5. shell 编程基础
```sh
#!/bin/bash  # 第一行，指示行，指定解释器，若没有，则默认使用 /bin/sh作为解释器

### 单行注释
# xxxxx代码

### 多行注释 1
:<<!
xxx 代码
xxxx
xxxxx
!

### 多行注释2
<<COMMENT
代码1
代码2
...
COMMENT

### 给脚本执行权限
chmod 777 xxx.sh

### 将脚本路径当如系统环境变量中可不加路径直接执行
export PATH=$PATH:/home/wanyouwen/shscrips
xxx.sh


### 变量进阶
# 定义变量
变量名=xxxx
# 使用变量
$变量名 或者 ${变量名}

#  字符串修改
FILENAME=linux_bash.txt

echo ${FILENAME%.*}   # 移除文件名后缀保留文件名 %.*
>>> linux_bash        

echo ${FILENAME##*.}  # 移除文件名，保留后缀 ##*.
>>> txt

FILENAMEPATH=home/wanyouwen/linux_bash.txt

echo ${FILENAME%/*}   # 移除文件名保留目录路径 %/*
>>> home/wanyouwen/
echo ${FILENAME##*/}   # 移除目录路径保留文件名 ##*/
>>> linux_bash.txt

### 字符串长度

${#变量名}  # 获取变量名指定的字符串长度
TEX="123456789a"
${#TEX}
>>> 10

### 获取子字符串
#{变量名:起点:长度}   索引从0开始
${TEX:3:4}
>>> 4567

#### bash 脚本参数
./xxx.bash  参数1  参数2 参数3 参数4

param1=$1
param2=$2
param3=$3
param4=$4


#### declare 指定变量类型
declare -r var=1   # 设定只读变量 var 其值为1
var=2              # 只读变量不能被修改，会报错
declare -i 变量    # 指定变量为整数型变量

declare -a 变量    # 声明数组变量
declare -a linux={'ubuntu','redhat'}
# 获取数组变量元素
echo ${linux[@]}  或者 echo ${linux[*]}  # 索引编号 @/* 代表所有元素
>>> ubuntu redhat
echo ${linux[0]}
>>> ubuntu
echo ${linux}    # 数组名指代第一个元素
>>> ubuntu

```

### shell 算数运算
```sh
id++  id--  变量后递增和后递减
++id  --id  变量前递增和前递减
- 单目 负号
= 单目 正号
+
! 逻辑反  ~按位取反
** 求幂
* 乘  /除 %取余 +加 -减
<< 按位左移   >> 按位右移
<=  >= <>  小于等于 大于等于 不等于
& 按位与
^ 按位异或
| 按位或
&& 逻辑与
|| 逻辑或

expr?值1:值2



10   普通数字开头 十进制数
020  0数字开头为  八进制数
0x30 0x开头为     十六进制数

2#111 #前面指定进制数  2进制数
32#20 #32进制数


##### 64 进制数   
let base64=64#@_
64进制数，0~9;a~z;A~Z;@ _
@表示62
_表示63

```


## 6. shell 的条件执行
```sh
### test 测试文件属性 字符串测试 算术测试

### if 条件语句
if [ $变量 == $变量2 ]
then
   xxxx
else 
   xxxx
fi

if [ $变量 == $变量2 ]    
then
   xxxx
elif [ $变量 -gt $变量2 ]      ### -gt大于-ge大于等于 -eq等于 -ne不等于 -lt小于 -le小于等于
then
   xxxx
else
   xxxx
fi

### 查看脚本的命令行参数数量 ###################

if [ $# -ne 1 ]; then
     echo "usage: $0 param1"
     exit 1
fi

num=$1 # 获取第一个命令行参数

if [ $num -ge 90 ] && [ $num -le 100 ]  
# 多条件 与&& （-a 也表示与 and   或|| （-o 也表示或 or） 逻辑非！
then
   echo " 90 <= num <= 100"
fi

if [ ! -f xxx.txt ]; then echo " xxx.txt not exit "; fi

-d 路径存不存在  

```
### case  多分支
```sh
if [ $# -lt 2 ]
then
   echo "usage: $0 param1 param2 "
fi

# 多case语句
case "$1" in
1)
  xxxx
;;  # 1)分支结束 必须要两个;来结束
2)
  xxxx
;;
3) 
  xxxxx
;;
4 | 5 | 6)    # 多重case 匹配
   XXXXX
;;
esac  # 结束case语句
  

```

## 7. bash 循环
### 7.1 for 循环
```sh
for 变量 in  列表
do
 ...
done

# 例如
param="1 2 3 4 5 6 7 8 9 10"
for num in $param
do
   echo $num
done


for i in 1 2 3
do
...
done

for s in aa bb cc dd
do
...
done


for (( i = 0; i < 3; i++ ))
do
   for (( j = 0; j < 5; j++ ))
   do
     ...
   done
   ...
done

```


### 7.2 while循环
```sh
while [ $var -le 3 ]
do
  var=$(( var + 1)) # 变量自加1
done


# while  和 read  读取文件

while read -r line
do
   echo $line
done < "$file_name"


# 读取文件 成 多列

while read -r col1 col2 col3   # 每一行分成三列
do
   echo $col1 $col2 $col3
done < "$file_name"




# 无限循环

while :
do
...
sleep 3
done

while true
do
...
done


```
### 7.3 until循环  与 while类似 只不过 在条件为假时执行

### 7.4 select循环


### 7.5 循环控制 break 和 continue

break  跳出整个循环，退出循环

continue 跳过当前次循环，执行下一次循环

## 8. shell 函数
```sh
# 格式1
function_name()
{
# 函数体
# 返回值 可选
return int
}

# 格式2
function function_name()
{
# 函数体
}

# 格式3
function function_name
{
# 函数体
local xxx  #函数内部变量
}

# 函数放在后台运行
function_name &


```

### 函数参数
```sh
fname()
{
# 获取函数参数
func_name=$0  # 函数名
arg1=$1
arg2=$2
# 函数参数总数
arg_num=$#
# $* / $@ 所有参数

# 以上同脚本的参数获取和统计方法
# 关于参数的语句

# 遍历所有参数
for  arg  in $* ; do 
...
done
}
```

# 调用函数 并传递参数给函数
fname foo bar
# 函数名 参数1 ($1)  参数2 ($2)


## 9. 正则表达式


## 10. 脚本输入处理



## 11. shell重定向



## 12. 管道和过滤器


## 13. 捕获


## 14. sed 和 awk 文本分析修改

## 15. 其他







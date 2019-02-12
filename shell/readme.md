# shell 编程
[参考代码1](https://github.com/Ewenwan/Linux_Code_Test/tree/master/Samples_Shell)

[参考代码2](https://github.com/keyboard-man/c_cpp/tree/master/boost)

[Linux-Shell脚本编程书籍代码](https://github.com/PacktPublishing/Linux-Shell-Scripting-Cookbook-Third-Edition)


[把平时有用的手动操作做成脚本，这样可以便捷的使用](https://github.com/Ewenwan/useful-scripts)

# 文本分析
```sh
cat demo.log | awk '
{
	for(i=1;i<=NF;i++)
        {
		if($2>="19:57:36,969") 
		print $2;
	} 
}'


```


# if 条件语句
```sh
# Shell中的条件判断语句
if []; ~ then ~fi ==

# 这种语句可以实现多重判断，注意最后一定要以一个else结尾。===========
if-then-elif-then-elif-then-...-else-fi。


#! /bin/sh #输出提示信息 
echo "Please enter a number:" 
#从键盘读取用户输入的数字 
read num 
#如果用户输入的数字大于10 
if [ "$num" -gt 10 ]; then 
#输出大于10的提示信息
	echo "The number is greater than 10." 
#否则 
else 
#输出小于或者等于10的提示信息 
	echo "The number is equal to or less than 10." 
fi

```


# case 语句
```sh
#! /bin/sh 
#输出提示信息 
echo "Hit a key,then hit return." 
#读取用户按下的键 
read keypress 
#case语句开始 
case "$keypress" in 
	#小写字母 
	[[:lower:]])  # 每个变量情况最后都要有)结尾
		echo "Lowercase letter."
	;;  # 每一个变量内容的程序段最后都需要两个分号 (;;) 来代表该程序段落的结束
	#大写字母 
	[[:upper:]]) 
		echo "Uppercase letter."
	;; 
	#单个数字 
	[0-9]) 
		echo "Digit."
	;; 
	#其他字符 
	*)      # 其余情况用*)来表示
		echo "other letter."
	;; 
esac # 最后要用esac来结束，即case反过来。

```

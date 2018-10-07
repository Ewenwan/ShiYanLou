# shell 编程
[参考代码](https://github.com/Ewenwan/Linux_Code_Test/tree/master/Samples_Shell)

[cankao](https://github.com/keyboard-man/c_cpp/tree/master/boost)
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

#########################################################################
# File Name: function.sh
# Author: He Jieting
# mail: rambo@mail.ustc.edu.cn
# Created Time: 2016年01月06日 星期三 19时50分34秒
#########################################################################
#!/bin/bash

demoFun()
{
	echo "This is a test function "
}

demoFun

funWithParam()
{
	echo "First is $1"
	echo "Second is $2"
	echo "tenth is ${10}"
	echo "number is $#"
	return $1
}

funWithParam 1 2 3 4 5 6 7 8 9 43
echo "return value is $?"

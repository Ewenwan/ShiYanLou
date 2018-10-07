#########################################################################
# File Name: expr.sh
# Author: He Jieting
# mail: rambo@mail.ustc.edu.cn
# Created Time: 2016年01月06日 星期三 19时21分29秒
#########################################################################
#!/bin/bash

a=10
b=20

val=`expr $a + $b`
echo "a + b : $val"
val=`expr $a - $b`
echo "a - b : $val"
val=`expr $a \* $b`
echo "a * b : $val"

echo "a / b : $val"
val=`expr $a % $b`
echo "a % b : $val"
if [ $a == $b ]
then
	echo "a equal b"
fi
if [ $a != $b ]
then
	echo "a not equal b"
fi

if [ $a -eq $b ]
then
	echo "$a -eq $b : a equal b"
else
	echo "$a -eq $b : a not equal b"
fi
if [ $a -ne $b ]
then
	echo "$a -ne $b : a not equal b"
else
	echo "$a -ne $b : a equal b"
fi
if [ $a -gt $b ]
then
	echo "$a -gt $b : a great b"
else
	echo "$a -gt $b : a not great b"
fi
if [ $a -lt $b ]
then
	echo "$a -lt $b : a little b"
else
	echo "$a -lt $b : a not little b"
fi
if [ $a -ge $b ]
then
	echo "$a -ge $b : a great or equal b"
else
	echo "$a -ge $b : a not great or equal b"
fi
if [ $a -le $b ]
then
	echo "$a -le $b : a little or equal b"
else
	echo "$a -le $b : a not little or equal b"
fi

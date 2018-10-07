#########################################################################
# File Name: var.sh
# Author: He Jieting
# mail: rambo@mail.ustc.edu.cn
# Created Time: 2016年01月06日 星期三 16时53分13秒
#########################################################################
#!/bin/bash

your_name="Rambo"
echo $your_name
echo ${your_name}

for skill in Ada Coffe Action Java 
do
	echo "I am good at ${skill}Script"
done

for file in `ls /home`
do
	echo $file
done

greeting="hello, $your_name !"
greeting1="hello, "$your_name" !"
echo $greeting
echo $greeting1

unset greeting
echo $greeting

#length of string & substring
string="abcde"
echo ${#string}
echo ${string:2:3}
string="alibaba is a great company"
echo `expr index "$string" "is"`

#array in shell
array_name=(value0 value1 value2)
echo ${array_name[0]}
array_name[3]=value3
array_name[5]=value5
echo ${array_name[@]}
echo ${array_name[*]}
length=${#array_name[@]}
echo $length
length=${#array_name[*]}
echo $length
length=${#array_name[0]}
echo $length

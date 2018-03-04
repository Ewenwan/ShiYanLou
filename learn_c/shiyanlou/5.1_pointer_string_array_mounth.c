/*
* 字符串数组 输出月份的英文
*/
#include<stdio.h>
int main()
{
//定义一个指针数组存储月份名称
    char *months[12]={"January","February","March","April","May","June","July","August","September","October","December"};
//声明用来存储月份号的变量
    int number; 
    printf("Please enter the month number:\n");
    scanf("%d",&number);
//实际在数组中的次序比月份号少1
    number--;
    printf("The month's name is:");
//输出月份名称
    printf("%s",*(months+number));//months为数组首地址   *( ) 取对应地址内的值 
    return 0;
}

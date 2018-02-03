#include<stdio.h>

int main(){
int a,b,c;
printf("Please enter a value: \r\n");
scanf("%d",&a);// 取变量a的地址 输入的值 存入变量a的地址中 
printf("\r\n");
printf("Please enter b value: \r\n");
scanf("%d",&b);// 取变量b的地址 输入的值 存入变量b的地址中 
c=a+b;// 整数相加
printf("\r\na+b=%d\r\n",c);
return 0;

}

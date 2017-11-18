#include<stdio.h>
/*
int a=100;
int *pointer_1;
pointer_1=&a;     取地址&a
 *pointer_1;      取值 
*/ 
int main()
{
 int a=100,b=10;
 int *pointer_1,*pointer_2; //定义指向整型数据的指针变量pionter_1,pionter_2 这里的*是定义 指针变量 
 pointer_1=&a;              //把变量a的地址赋给指针变量pointer_1
 pointer_2=&b; 
 printf("a=%d,b=%d\n",a,b);
 printf("%p,%p\n",pointer_1,pointer_2); //输出a和b在内存中的地址
 printf("*pointer_1=%d,*pointer_2=%d\n",*pointer_1,*pointer_2);//输出变量a和b的值  这里的*是取指针变量指向的值 

 return 0;

}

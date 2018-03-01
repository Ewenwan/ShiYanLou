/*
* 函数递归调用 求解n的阶乘
*/
#include<stdio.h>
int main()
{
    // 函数声明
    int fac(int n);//deaclare fac function,aim to call fac functiuion
    
    // 输入
    int n,y;
    printf("please input an integer number:");
    scanf("%d",&n);
    
    // 求解 输出
    if(n<0)
      printf("data error");   //n must great or eqqal than zero
    else
       {
           y=fac(n);
           printf("%d!=%d\n",n,y);
       }
       
    return 0;
}

int fac(int n)
{
    int f;
    if(n==0||n==1)// special case
         f=1;
    else 
         f=fac(n-1)*n;//normal case
    return f;
}

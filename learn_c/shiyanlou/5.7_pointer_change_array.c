/*
1）形参和实参都用数组名，例如：
int main()     
{
   int a[10];
   .
   f(a,10);           // a实参  实参用数组名  a为地址 &a[0] 
}
int f(int x[],int n)  // x[]形参 
{
   .
}
2）实参用数组名，形参用指针变量。例如：
int main()
{
   int a[10];
   .
   f(a，10)；
}
void f(int *x,int n)//形参  *x 指针变量
{
  .
}
3）实参形参都用指针变量。例如：
int main()
{
    int a[10],*p=a;//指针变量  ,p的值是&a[0]
    .
    f(p,10);
}
void f(int *x,int n) // x的初始值也是&a[0]
{
   .
}
4） 实参为指针变量，形参为数组名。例如：
int main（）
 {
    int a[10],*p=a;//指针变量  ,p的值是&a[0]
    .
    f(p,10);
 }
void f(int x[],int n)
{
   .
}
*/ 
#include<stdio.h>
int main()
{
   void change(int *x,int n);// 函数声明
   int i,a[10]={3,7,9,11,0,6,7,5,4,2};
   printf("the original array:\n");
   for(i=0;i<10;i++)
      printf("%d\t",a[i]);
   printf("\n");

   change(a,10);
   printf("the array has been inverted:\n");
   for(i=0;i<10;i++)
     printf("%d\t",a[i]);

   return 0;
}

void change(int *x,int n)
{
   int *p,temp,*i,*j,m=(n-1)/2;
   i=x;j=x+n-1;p=x+m;//x首地址   x+n-1尾地址  p=x+m中间的地址 
   for(;i<=p;i++,j--)
   {
      temp=*i;//地址指向的变量值  
      *i=*j;
      *j=temp;
    }
}
© 2018 GitHub, Inc.

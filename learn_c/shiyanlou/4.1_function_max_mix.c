/*
* 四个整数中的 最大最小值
* 函数调用
*/
#include<stdio.h>
int main()
{
    // 函数声明
    int max4(int a,int b,int c,int d);// declare max4 to call max4
    int min4(int a,int b,int c,int d);// declare min4 to call min4
    // 输入
    int a,b,c,d,max,min;
    printf("please enter 4 integer number:\r\n");
    scanf("%d%d%d%d",&a,&b,&c,&d);
    // 求解
    max=max4(a,b,c,d);
    min=min4(a,b,c,d);
    // 打印
    printf("max=%d\n",max);
    printf("min=%d\n",min);
    
    return 0;
}

int max4(int a,int b,int c,int d)
{
    int max2(int a,int b);      //declare max2 to call max2
    int m;
    m=max2(a,b);
    m=max2(m,c);
    m=max2(m,d);
    return(m);
}

int max2(int a,int b)// return the max one of the two numbers
{
    if(a>=b)
      return a;
    else
      return b;

}
int min4(int a,int b,int c,int d)
{
    int max2(int a,int b);      //declare min2 to call min2
    int m;
    m=min2(a,b);
    m=min2(m,c);
    m=min2(m,d);
    return(m);
}

int min2(int a,int b)// return the min one of the two numbers
{
    if(a>=b)
      return b;
    else
      return a;

}

/*
* 交换三个数 
*/
#include<stdio.h>
int main()
{
   void exchange(int * q1,int * q2,int * q3);//函数声明
   int a,b,c,*p1,*p2,*p3;
   printf("please enter 3 integer number:");
   scanf("%d%d%d",&a,&b,&c);
   p1=&a;p2=&b;p3=&c;// a的地址 b的地址  c的地址 
   exchange(p1,p2,p3);
   printf("the order is :%d,%d,%d\n",a,b,c);
   return 0;
}

void exchange(int *q1,int *q2,int *q3)
{
   void swap(int *p1,int *p2);//函数声明
   if(* q1<* q2) swap(q1,q2);// * q1=a  * q2=b   * q3=c
   if(* q1<* q3) swap(q1,q3);
   if(* q2<* q3) swap(q2,q3);
}
//交换两个指针指向的数
void swap(int *p1,int *p2)
{
  int temp;
  temp=*p1;
  *p1=*p2; //指向的内存互换 
  *p2=temp;
}

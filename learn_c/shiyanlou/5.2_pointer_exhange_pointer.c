/*
* 交换两个指针的值
*/
#include<stdio.h>
int main()
{
    int *p1,*p2,*p,a,b;
    printf("please enter two integer number:");
    scanf("%d,%d",&a,&b);
    p1=&a;
    p2=&b;
    if(a<b)
    {
        p=p1;p1=p2;p2=p;          //使p1和p2的值(指向的地址改变了)互换  a和b没有互换 即*p1和*p2未换 
    }

    printf("a=%d,b=%d\n",a,b);
    printf("max=%d,min=%d\n",*p1,*p2);
    return 0;
}

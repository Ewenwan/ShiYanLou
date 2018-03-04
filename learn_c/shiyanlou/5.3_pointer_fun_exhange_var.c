/*
* 通过参数为指针的函数 交换两个对象的值
*/
#include<stdio.h>
int main()
{
    void swap(int * point_1,int * point_2);
    int *p1,*p2,*p,a,b;
    printf("please enter two integer number:");
    scanf("%d,%d",&a,&b);
    p1=&a;//a变量的地址   指针
    p2=&b;
    if(a<b)
    swap(p1,p2);// 交换两个指针指向对象的 值

    printf("max=%d,min=%d\n",a,b);
    return 0;
}

void swap(int * point_1,int * point_2)
{
    int temp;
    temp=* point_1;         //使*p1和*p2互换  指向的值  *p1=a     *p2=b  即 a和b互换  而 p1 和 p2未换 
    * point_1=* point_2;
    * point_2=temp;
}

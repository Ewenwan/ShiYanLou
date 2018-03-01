/*
* 函数调用 求解 数组最大值 
*/
#include<stdio.h>
int main()
{   // 声明
    int max(int x,int y);//声明max函数 为了调用
    
    // 输入
    int a[10],m,n,i;
    printf("please enter 10 integer number:");
    for(i=0;i<10;i++)
       scanf("%d",&a[i]);
    printf("\n");
    
    // 函数调用求解
    for(i=1,m=a[0],n=0;i<10；i++)   
    {
        if(max(m,a[i])>m)          //若max函数返回值大于m
        {
            m=max(m,a[i]);        //max函数的返回值取代m
            n=i;                  //把数组元素的序号记下来，放到n中
        }
    }

    printf("the largest number is %d\n it is the %dth number \n",m,n+1);
}

int max(int x,int y)
{
    return(x>y?x:y);
}

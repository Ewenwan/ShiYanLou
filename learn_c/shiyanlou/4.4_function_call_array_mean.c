/*
* 函数调用 数组均值
*/
#include<stdio.h>
int main()
{
    // 函数声明
    float average(float array[10]);
    // 输入
    float score[10],aver;
    int i;
    printf("please enter 10 scores:");
    for(i=0;i<10;i++)
       scanf("%f",&score[i]);
    printf("\n");
    // 函数调用求解
    aver=average(score);   //注意这里调用函数时，向形参传递的是数组首元素的地址
    // 打印
    printf("average score is %5.2f\n ",aver);
    
    return 0;
}

//求数组平均数 
float average(float array[10])
{
    int i;
    float aver,sum=0;
    for(i=0;i<10;i++)
      sum=sum + array[i];
    aver=sum/10;
    return(aver);
}

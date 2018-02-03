#include<stdio.h>
int main()
{
    int sum=0,inter[10]={0,0,0,0,0,0,0,0,0,0};// 初始洞 观测状态
    int i,HollNum;
    for(i=1;i<=1000;i++)
    {
        sum=sum+i;//求和
        HollNum=sum%10;// 取余得到 洞编号 current holl number
//        printf("HollNum is : %d",HollNum);
        if(HollNum==0)//sum=10*n
             inter[0]=10;// holl number 10 >>> holl number 0
        else
            inter[HollNum]=HollNum;//已经到达的洞编号记录下来 already riched holl munber 
    }
    for(i=0;i<10;i++)
        if(inter[i]==0)//为初始状态的 还未到达过 not riched 
            printf("\nThe safe holl is :%d\n",i);
}

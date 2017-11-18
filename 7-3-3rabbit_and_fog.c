#include<stdio.h>
int main()
{
    int sum=0,inter[10]={0,0,0,0,0,0,0,0,0,0};
    int i,HollNum;
    for(i=1;i<=1000;i++)
    {
        sum=sum+i;
        HollNum=sum%10;// current holl number
//        printf("HollNum is : %d",HollNum);
        if(HollNum==0)//sum=10*n
             inter[0]=10;// holl number 10 >>> holl number 0
        else
            inter[HollNum]=HollNum;//already riched holl munber 
    }
    for(i=0;i<10;i++)
        if(inter[i]==0)// not riched 
            printf("\nThe safe holl is :%d\n",i);
}

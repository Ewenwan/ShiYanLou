/*
* 根据数组索引修改数组元素 
*/
#include<stdio.h>
int main()
{
   void change(int x[],int n);//函数声明
   int i,a[10]={3,7,9,11,0,6,7,5,4,2};
   // 原数组
   printf("the original array:\n");
   for(i=0;i<10;i++)
      printf("%d\t",a[i]);
   printf("\n");

   change(a,10);
   // 修改后的数组
   printf("the array has been inverted:\n");
   for(i=0;i<10;i++)
     printf("%d\t",a[i]);

   return 0;
}
// 传入要修改的数组 和 对应大小
// 交换相对位置上的两个元素
void change(int x[],int n)//±àÒëÊ±Îª (int *arr,int n); 
{
   int temp,i,j,m=(n-1)/2;
   for(i=0;i<=m;i++)
   {
      j=n-1-i;// x[0] <---> x[9]
      temp=x[i];
      x[i]=x[j];
      x[j]=temp;
    }
}

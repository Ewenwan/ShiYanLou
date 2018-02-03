#include<stdio.h>
int main()
{
    int i,a[10];
    for(i=0;i<10;i++)
    a[i]=i+1;

    for(i=9;i>=0;i--)//从数组末尾向前处理
     {
      printf("%d\t",a[i]);
      if(i==5) printf("\n");
     }
     
    return 0;

}

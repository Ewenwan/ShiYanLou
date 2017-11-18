#include<stdio.h>
int main()
{
    int i,a[10];
    for(i=0;i<10;i++)
    a[i]=i+1;

    for(i=9;i>=0;i--)
     {
      printf("%d\t",a[i]);
      if(i==5) printf("\n");
     }
    return 0;

}

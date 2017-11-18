#include<stdio.h>
int main()
{
    int i,j,a[20],sum=0;
    float mean=0.0;
    printf("please input 20 numbers:\n");
    
    for(i=0;i<20;i++){
      scanf("%d",&a[i]);
      sum += a[i];
     }
    mean = (float)sum / 20.0;
    int count=0;
    for(j=0;j<20;j++)//9 times compare 
     {
      if (a[j]>mean) count++;
     } 
    printf("The mean of the list is: %.3f\r\n",mean);
    printf("The num of the list mebber which bigger than their mean is: %d\r\n",count);
    return 0;

}

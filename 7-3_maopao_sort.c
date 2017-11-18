#include<stdio.h>
int main()
{
    int i,j,t,a[10];
    printf("please input 10 numbers:\n");
    for(i=0;i<10;i++)
    scanf("%d",&a[i]);

    for(j=0;j<9;j++)//9 times compare 
     {
        for(t=0;t<9-j;t++){
	    if(a[t]>a[t+1]){
            int b=a[t];
            a[t]=a[t+1];
            a[t+1]=b;
            }
         }
     } 
    printf("The sorted list:\r\n");
    for(int k=0;k<=9;k++) printf("%d\t",a[k]);
    return 0;

}

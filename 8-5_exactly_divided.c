#include <stdio.h>
int main(){
int fun(int m);
int m;
printf("Please input a int number:\r\n");
scanf("%d",&m);
fun(m);
return 0;
}
int fun(int m)
{
    int numbers[1000],a[1000],i,n=0;//1~m, ...
    for(i=0;i<m;i++)numbers[i]=i+1;
    for(i=0;i<m;i++)
    {
        if(numbers[i]%7==0||numbers[i]%11==0)
        {
            a[n]=numbers[i];//match condition 
            printf("%d,",a[n]);  //display
            n++;  //count
        }
    }
    putchar('\b');  // delet the last char ','
    //putchar('\n');  //
    return n;  //return the munber
}

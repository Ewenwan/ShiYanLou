#include <stdio.h>
int main(){
int fun(int a,int b);
int a,b,c;
printf("Please input two number between 10~99:\r\n");
scanf("%d%d",&a,&b);
c=fun(a,b);
printf("c=%d",c);
return 0;
}
int fun(int a,int b)
{
int g,s,bb,q;
g=b%10;
s=a/10;
bb=b/10;
q=a%10;
return g+s*10+bb*100+q*1000;
}

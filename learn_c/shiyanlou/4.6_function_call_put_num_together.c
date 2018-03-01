/*
* 函数调用 合并两个两位数 成一个四位数
*/
#include <stdio.h>

int main(){
  // 函数声明
  int fun(int a,int b);
  // 输入
  int a,b,c;
  printf("Please input two number between 10~99:\r\n");
  scanf("%d%d",&a,&b);
  // 函数调用 打印
  c=fun(a,b);
  printf("c=%d",c);
  
  return 0;
}

int fun(int a,int b)
{
  int g,s,bb,q;
  g=b%10; //b的个位数
  bb=b/10;//b的十位数
  q=a%10; //a的个位数
  s=a/10; //a的十位数

  return g + s*10 + bb*100 + q*1000;
}

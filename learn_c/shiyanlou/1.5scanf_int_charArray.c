#include<stdio.h>

int main(){

int Cnum;
  char name[20];
  printf("+++++++++++++++++++++++++++\r\n");
  printf("+++++ Welcome to my book store ++++++");
  printf("Please enter your card number;\r\n");
  scanf("%d",&Cnum);//整数输入
  printf("Please enter your name:\r\n");
  scanf("%s",name);// 字符串(字符数组)输入
  printf("\r\nWelcom.%s! Your card number is :%d\r\n",name,Cnum);

  return 0;
}

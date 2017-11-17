#include<stdio.h>
int main(){
int Cnum;
char name[20];
printf("+++++++++++++++++++++++++++\r\n");
printf("+++++ Welcome to my book store ++++++");
printf("Please enter your card number;\r\n");
scanf("%d",&Cnum);
printf("Please enter your name:\r\n");
scanf("%s",name);
printf("\r\nWelcom.%s! Your card number is :%d\r\n",name,Cnum);

return 0;
}

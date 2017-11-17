#include<stdio.h>
int main(){
char src[10]="ShiYanLou";
for(int i=0;i<9;i++){
src[i]=src[i]+1;
putchar(src[i]);
}
printf("\r\n%s",src);
}

#include<stdio.h>
int main(){
    char src[10]="ShiYanLou";
    for(int i=0;i<9;i++){
      src[i]=src[i]+1;//编码
      putchar(src[i]);//单个字符输出
    }
    printf("\r\n%s",src);//整个字符串输出
    return 0;
}

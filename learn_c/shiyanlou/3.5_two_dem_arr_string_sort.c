/*
* 两维字符数组 比较大小
*/
#include<stdio.h>
#include<string.h> //使用 string 函数 need #include<string.h>
int main()
{   char string[30];
    char ch[3][30];// store 3 strings
    int i;
    // 输入
    for(i=0;i<3;i++)
        gets(ch[i]);//inputs
    // 初始化字符串   
    strcpy(string,ch[0]);//init
    
    // 比较
    for(i=1;i<3;i++)
        if(strcmp(ch[i],string)<0)
            strcpy(string,ch[i]);
            
    printf("The result is :\n%s\n",string);

    return 0;
}

#include<stdio.h>
#include<string.h> //use string func need #include<string.h>
int main()
{    char string[30];
    char ch[3][30];// store 3 strings
    int i;
    for(i=0;i<3;i++)
        gets(ch[i]);//inputs
    strcpy(string,ch[0]);//init
    for(i=1;i<3;i++)
        if(strcmp(ch[i],string)<0)
            strcpy(string,ch[i]);
    printf("The result is :\n%s\n",string);

    return 0;
}

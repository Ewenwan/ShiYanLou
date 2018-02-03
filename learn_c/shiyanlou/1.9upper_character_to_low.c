#include<stdio.h>
int main()
{
    char letter;
    printf("Enter an upper case letter:");
    scanf("%c",&letter);// 输入一个字符

    if((letter>='A')&&(letter<='Z'))//大写字母 字符upper character
    {
        letter += 'a'-'A';   //letter = letter+'a'-'A';//upper to low '1'=31H  'A' = 41H = 65  'a'=61H =97
        printf("You entered an uppercase %c.\n",letter);
    }
    else
       printf("You did not enter an uppercase letter.\n");

    return 0;
}

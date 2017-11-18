#include<stdio.h>
int main()
{
    char letter;
    printf("Enter an upper case letter:");
    scanf("%c",&letter);

    if((letter>='A')&&(letter<='Z'))//upper character
    {
        letter+='a'-'A';   //letter=letter+'a'-'A';//upper to low  
        printf("You entered an uppercase %c.\n",letter);
    }
    else
       printf("You did not enter an uppercase letter.\n");

    return 0;
}

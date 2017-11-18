#include<stdio.h>
int main()
{
    char grade;
    printf("Please input the level of your gread(a-d):");
    scanf("%c",&grade);
    printf("you score:");
    switch(grade)
    {
         case 'a':printf("85~100\n");break;
         case 'b':printf("70~84\n");break;
         case 'c':printf("60~69\n");break;
         case 'd':printf("<60\n");break;
        default:printf("error!\n");
   }

   return 0;
}

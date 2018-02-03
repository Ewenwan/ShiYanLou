#include<stdio.h>
int main()
{
    char grade;
    printf("Please input the level of your gread(a-d):");
    scanf("%c",&grade);//输入等级
    printf("you score:");
    switch(grade)// 根据每种等级 打印相应等级的信息
    {
         case 'a':printf("85~100\n");break;
         case 'b':printf("70~84\n");break;
         case 'c':printf("60~69\n");break;
         case 'd':printf("<60\n");break;
         default:printf("error!\n");
   }

   return 0;
}

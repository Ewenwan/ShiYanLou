#include<stdio.h>
int main()
{
    int number=0;
    printf("\nPlease enter an interger between 1 and 10:\r\n");
    scanf("%d",&number);

    if(number>5)//条件成立 后面的式子会执行
      printf("You entered %d which is greater than 5\r\n",number);

    if(number<6)
      printf("You enter %d which is less than 6\r\n",number);

    return 0;

}

#include<stdio.h>
#include<math.h>

int main()
{
   double number1=0.0;// operater 1 操作数1
   double number2=0.0;// operator 2 操作数2
   char operation=0;  // operatoion 运输字符 character

   printf("\nEnter the calculation\n");
   scanf("%lf%c%lf",&number1,&operation,&number2);// 3+2 
   //long float, 字符 char , long float

   switch(operation)//不同字符对应不同 的 运算方式
   {
      case '+'://加法
          printf("=%lf\n",number1+number2);
          break;

      case '-'://减法
           printf("=%lf\n",number1-number2);
           break;

      case '*'://乘法
           printf("=%lf\n",number1*number2);
           break;

      case '/'://除法
           if(number2==0)//被除数不允许为0
             printf("\n\n\aDivision by zero error!\n");
           else
             printf("=%lf\n",number1/number2);
           break;

      case '%'://取余
           if((long)number2==0) //被除数不允许为0 
             printf("\n\n\aDivision by zero error!\n"); 
           else
             printf("=%ld\n",(long)number1%(long)number2);      
           break;
      case '^'://乘方
	         printf("=%lf\n",pow((double)number1,(double)number2));
           break;
      default:
           printf("\n\n\aDivision by zero error!\n");
           break;          
    }
    return 0;

}

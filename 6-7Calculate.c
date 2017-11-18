#include<stdio.h>
#include<math.h>
int main()
{
   double number1=0.0;// operater 1
   double number2=0.0;// operator 2
   char operation=0;  // operatoion character

   printf("\nEnter the calculation\n");
   scanf("%lf%c%lf",&number1,&operation,&number2);
   //long float, char , long float

   switch(operation)
   {
      case '+':
          printf("=%lf\n",number1+number2);
          break;

      case '-':
           printf("=%lf\n",number1-number2);
           break;

      case '*':
           printf("=%lf\n",number1*number2);
           break;

      case '/':
           if(number2==0)
             printf("\n\n\aDivision by zero error!\n");
           else
             printf("=%lf\n",number1/number2);
           break;

      case '%':
           if((long)number2==0)  
             printf("\n\n\aDivision by zero error!\n"); 
           else
             printf("=%ld\n",(long)number1%(long)number2);      
           break;
      case '^':
	  printf("=%lf\n",pow((double)number1,(double)number2));
          break;
      default:
          printf("\n\n\aDivision by zero error!\n");
          break;          
    }
    return 0;

}

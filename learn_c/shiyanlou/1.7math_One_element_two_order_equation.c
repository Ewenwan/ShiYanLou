#include<stdio.h>
#include<math.h>               //sqrt function
int main()
{
    double a,b,c,disc,x1,x2,p,q;
    printf("Please input a b c:\r\n");//a*x^2 + b*x + c = 0
    scanf("%lf%lf%lf",&a,&b,&c);// 双精度类型输入 long float
    disc=b*b - 4*a*c;// dat
    if(disc<0)
        printf("This equation hasn't real root!\n");//dat 小于0 没有根 无解
    else
       {
           p = -b/(2.0*a);
           q  =sqrt(disc)/(2.0*a);
           x1 = p+q, x2=p-q;
           printf("x1=%7.2f\n x2=%7.2f\n",x1,x2);//7位有效数字(小于7位按7位，不足空格补齐),小数位两位 
     }
    return 0;
}

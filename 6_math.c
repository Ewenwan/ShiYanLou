#include<stdio.h>
#include<math.h>
int main(){
double a,b,c,det,x1,x2,p,q;
scanf("%lf%lf%lf",&a,&b,&c);
det=b*b - 4*a*c;
p=-b/(2.0*a);
q=sqrt(det)/(2.0*a);
x1=p-q;
x2=p+q;
printf("The solve of a*x^2 + b*x + c = 0 is:\r\n");
printf("x1=%7.2f\r\nx2=%7.2f\r\n",x1,x2);

return 0;
}

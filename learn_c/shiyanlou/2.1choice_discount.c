#include<stdio.h>
int main()
{
    double unit_price=3.5;//单价
    double discount1=0.05;//最低折扣
    double discount2=0.1;//中等折扣
    double discount3=0.15;//最高折扣
    double total_price=0.0;//总价
    int quantity=0;//买的量

    printf("Enter the number that you want to buy:");
    scanf("%d",&quantity);//买的总量

    total_price=quantity*unit_price*(1.0-
                   (quantity>50?discount3:(// 总量大于50 最高折扣
                       quantity>20?discount2:(// 总量大于20 中等折扣
                            quantity>10?discount1:0.0))));// 总量大于10 最低折扣

    printf("The price for %d is %7.2f\n",quantity,total_price);
    return 0;

}

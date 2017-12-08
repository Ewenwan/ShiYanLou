#include<stdio.h>

//枚举  创建符号 (字符串标识符)  枚举名未给 匿名
typedef enum {
	COUNT, POUNDS, PINTS//使用逗号 分割  个数计 重量计 容积计
} unit_of_measure;//枚举别名  为其中的一个值


//联合 有效利用存储空间  仅有一块空间 
// 保存不同的类型 
typedef union {
	short count;// 对应 枚举类型的 COUNT
	float weight;//使用分号 分割 对应 枚举类型的 COUNT POUNDS
	float volume; // 对应 枚举类型的 PINTS
} quantity;//联合别名


// 结构体
typedef struct {
	const char *name;//字符串字面值     水果名字
	const char *country;//使用分号 分割 国家
	quantity amount;//联合 
	unit_of_measure units;//枚举
} fruit_order;// 结构体变量别名  还可以设置指针 *fruit_order;


// 结构体  位字段bitfield 存储数据 有效利用空间
typedef struct {//必须使用  unsigned int 标识 
	unsigned int low_pass_vcf:1;//该字段用一位保存  0~1 
	unsigned int two_pass_vcf:2;//该字段用2位保存	0~3
	unsigned int month_nomber:4;//4为保存 0~15 月份1~12 	
} bitfield_temp; 


void display(fruit_order order){//  fruit_order *order    &apples   order->units
	printf("水果订单\r\n");
	if(order.units==PINTS)//点操作符 读取结构中的字段  -> 符号 更新结构中的字段 
		printf("%2.2f pints of %s\n", order.amount.volume,order.name);
	else if(order.units==POUNDS)
		printf("%2.2f lbs of %s\n", order.amount.weight,order.name);	
    else
		printf("%i %s\n", order.amount.count,order.name);
}

int main(){
fruit_order apples = {"apple","England",.amount.count=144,COUNT};//苹果	
fruit_order strawberries = {"strawberries","China",.amount.weight=17.6,POUNDS};//草莓
fruit_order orangej = {"orange juice","U.S.A",.amount.volume=11.5,PINTS};//橘子汁
display(apples);
display(strawberries);
display(orangej);

return 0;
}

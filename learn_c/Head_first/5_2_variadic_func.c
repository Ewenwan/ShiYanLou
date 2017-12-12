#include <stdarg.h>

// va_list va_arg va_end  为特殊的宏  编译前 预处理时会替换为特殊的代码 
void print_ints(int args, ...){
	va_list val;//参数列表 
	va_start(val, args);//从args 开始 
	int i;
	for(i=0;i<args;i++){
	   printf("参数：%i\r\n",va_arg(val,int));//列表 参数类型
	}
	va_end(val); //结束 销毁列表 
}

//饮料类目 
typedef enum drink{
	MUDSLIDE, FUZZY_NAVEL, MONKEY_GLAND, ZOMBIE
}drink; //枚举匿名 
double price(drink d){
	switch(d){
		case MUDSLIDE:
		  return 6.79; 
		case FUZZY_NAVEL:
		  return 5.31;
		case MONKEY_GLAND:
		  return 4.82;
	    case ZOMBIE:
		  return 5.89;		  
	}
	return 0;	
} 

double total(int args, ...){
	va_list val;//参数列表 
	va_start(val, args);//从args 开始 
	int i;
	double sum=0;
	for(i=0;i<args;i++){		
	   sum += price(va_arg(val, drink)); //需要指定参数类型  
	}
	va_end(val); //结束 销毁列表 
	return  sum;
}

int main(){
    print_ints(4, 111, 222, 1212, 1111); //个数 参数... 
	printf("总消费金额：%.2f\r\n",total(1, MUDSLIDE)); 
	printf("总消费金额：%.2f\r\n",total(2, MUDSLIDE, FUZZY_NAVEL));
	printf("总消费金额：%.2f\r\n",total(3, MUDSLIDE, FUZZY_NAVEL, MONKEY_GLAND));
	printf("总消费金额：%.2f\r\n",total(4, MUDSLIDE, FUZZY_NAVEL, MONKEY_GLAND, ZOMBIE));	
	return 0;
}


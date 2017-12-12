#include <stdio.h>
#include <stdlib.h>

enum response_type{DUMP, SECOND_CHANCE, MARRIAGE};//枚举符号变量 存储时按 0 1 2 3 ... 存储 
typedef struct{
char *name;
enum response_type type; 
} response;

void dump(response r){//只是引用结构体参数 如果需要修改 需要修改为 结构体指针  response *r
	printf("Deer %s, \n",r.name);
	puts("很不幸，您没有机会了\r\n");
}
void second_chance(response r){ 
	printf("Deer %s, \n",r.name);
	puts("好消息，您还有一次机会\r\n");
}
void marriage(response r){ 
	printf("Deer %s, \n",r.name);
	puts("祝贺，您通过了!\r\n");
}

//函数指针数组
//返回类型 函数指针数组 参数类型       
void (*replies[]) (response) = {dump, second_chance, marriage};

int main(){
	response r[]={
		{"Mike", DUMP}, {"Luna",SECOND_CHANCE},
		{"Hatter",MARRIAGE}, {"Wind",SECOND_CHANCE}
	}; 
	int i;
	for(i=0; i<4; i++){
		(replies[r[i].type])(r[i]);//r[i].type = 0/1/2
	}
	
	return 0;
}


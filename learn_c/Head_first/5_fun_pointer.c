#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 需要使用函数指针的函数 的形参类型 必须为 const void* 类型
int compare_scores(const void* s_a,  const void* s_b){
	int a=*(int*)s_a;//void* 转成 int* 再取地址中的值
	int b=*(int*)s_b;
	return a-b;//升序  b-a为降序排列
}
typedef struct{
	int width;
	int height;
} rectangle;//矩形类型 结构体 匿名
int compare_areas(const void* a, const void* b){
	rectangle* ra = (rectangle*)a;//void* 转成 结构体类型指针
	rectangle* rb = (rectangle*)b;//void* 转成 结构体类型指针
return 	(ra->width*ra->height - rb->width*rb->height); 
}
int compare_name(const void* a, const void* b){
	char** sa=(char**)a;
	//字符串为字符指针，输入的为字符串指针 所以为 char** 指针的指针
	char** sb=(char**)b;
	return strcmp(*sa, *sb);
	
}

int main(){
	
	int scores[] = {534, 122, 12, 45, 345, 970, 10};
	int i;
	qsort(scores, 7, sizeof(int), compare_scores);
	puts("排序后的分数:\r\n");
	for(i=0;i<7;i++){
		printf("分数 = %i\n", scores[i]);
	}
	
	char *name[]={"Karen", "Mark", "Brett", "Molly", "Tony"};//字符串指针数组
	qsort(name, 5, sizeof(char*), compare_name);
	puts("排序后的名字:\r\n");
		for(i=0;i<5;i++){
		printf("%s\n", name[i]);
	}
	return 0;
}


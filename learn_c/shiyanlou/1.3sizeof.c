#include<stdio.h>
#include<stdbool.h>
int main(){
printf("int size:%d\r\n",sizeof(int));//整形类型 变量存储大小
printf("short size:%d\r\n",sizeof(short));//短整形类型 变量存储大小
printf("long szie:%d\r\n",sizeof(long));//长整形类型 变量存储大小
printf("float size:%d\r\n",sizeof(float));//浮点型类型 变量存储大小
printf("double size:%d\r\n",sizeof(double));//双精度类型 变量存储大小
printf("bool size:%d\r\n",sizeof(bool));//布尔类型 变量存储大小
return 0;
}

#include<stdio.h>

void printf_char(char msg[]){ //等同于  void printf_char(char *msg){
 printf("Message read: %s\r\n",msg);
 puts(msg);//puts会自动换行 
 printf("The size of msg is: %d\r\n",sizeof(msg));//是字符指针变量的大小 
 printf("Message read: %p\r\n",msg+9);//首地址 偏移 9个单元 
 printf("Message read: %s\r\n",msg+9);//首地址 偏移 9个单元 地址内存储的数据  字符 
} 

int main(){
char que[]="qwertryu qwert uy";
printf("the dress of que:%p\r\n",que);
printf_char(que);
printf("SIZE of char* :%d\r\n",sizeof(char*));
char *p_que = que;//数组首地址指针 
printf_char(p_que);

char xin[29],ming[29];
printf("Enter Your first name and last name"); 
scanf("%28s %28s",xin ,ming);//数组首地址     最大长度 + \0 为总长度 

int age;
printf("输入你的年龄:\r\n");
scanf("%d",&age);// age 变量地址 

char food[10];
printf("输入你最爱的事物:\r\n");
fgets(food,sizeof(food),stdin);//给出输入的最大长度 sizeof(food) 


return 0;
} 

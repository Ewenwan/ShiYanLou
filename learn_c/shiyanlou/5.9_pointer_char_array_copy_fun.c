/*
* 字符串拷贝 
*/
#include<stdio.h>
int main()
{
   void copy_string(char from[],char to[])；
   char a[]="I am a teacher";  //字符数组类型 
   // char *a="I an a teacher";//字符型指针 
   char b[]="You are a programmer";
   printf("string a=%s\nstring b=%s\n",a,b);
   printf("copy string a to string b:\n");
   
   copy_string(a,b);//1)用字符数组名作为函数实参数
    
  //char *from=a,*to=b;  2)用字符型指针变量作函数实参
  //copy_string(from,to);
  
   printf("\nstring a=%s\nstring b=%s\n",a,b);
   return 0;

}

// 字符数组名作函数 形参 
void copy_string(char from[],char to[])
{
   int i=0;
   while(from[i]!='\0')
   {
      to[i]=from[i];
      i++;
   }
   to[i]='\0';
}

// 字符型指针作函数 形参 
void copy_string(char *from,char *to)
{
   for(;*from!='\0';from++,to++)
   {*to=*from;}
   *to='\0';
}
© 2018 GitHub, Inc.

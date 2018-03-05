/*
* 输入文件名 新建文件
* 写入输入的信息
* 以#号键结束
*/
#include<stdio.h>
#include<stdlib.h>   // stdlib 头文件即 standard library 标准库头文件 使用 exit(0) 
int main()
{
   FILE * fp;//文件类型的指针 
   char ch,filename[10];//字符 和字符数组 
   printf("Please enter the file name:");

   scanf("%s",filename);//文件名 
   if((fp=fopen(filename,"w"))==NULL)    // 以write写入方式 打开输出文件并使 fp 指向此文件
   {
      printf("Unable to open this file\n");     // 如果打开出错，就输出“打不开”的信息
      exit(0);     // 终止程序   #include<stdlib.h> 
   }
   ch=getchar();     // 用来接收输入文件名时 最后输入的回车符
   
   printf("Please enter a string  in the disk（Ends with a #）：");
   ch=getchar();     // 接收从键盘输入的第一个字符
   while(ch!='#')     // 当输入 # 时结束循环
   {
      fputc(ch,fp);//写入一个字符 
      putchar(ch);
      ch=getchar();
   }

   fclose(fp);//关闭文件 
  // putchar(10);
   return 0;
}

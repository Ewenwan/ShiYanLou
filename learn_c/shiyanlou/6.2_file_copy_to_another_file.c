/*
* 命令行输入 文件名参数 复制一个文件的内容到 另一个文件
*/
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
 FILE *fp_src,*fp_tar;

 if(((fp_src=fopen(argv[1],"rb"))==NULL)) // 打开文件  父本
   {
      printf("error\n");    
      exit(0);
   }
//printf("%s\n",argv[1]);
//printf("%s\n",argv[2]);
fp_tar=fopen(argv[2],"wb");// 打开需要新建的文件
int ch;
while((ch=fgetc(fp_src)) != EOF)//文件结束符之前
    fputc(ch,fp_tar);//复制每一个 字符

fcloseall();// 关闭所有文件
//fclose(fp_src);
//fclose(fp_tar);
return 0;

}

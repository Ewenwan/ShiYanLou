#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
   // # Celsius degree
    int input = 0;

   // # paramter number must bu one 命令行参数必须为1个
    if( argc != 2)
        return 1;

   // # argv[0] is executive program 可执行程序名 ,argv[1] is first command line paramter 第一个命令行参数
    input = atoi(argv[1]);// 字符串转成整数 atof()转成浮点数

    float output_f=input*1.8+32.0;// 温度摄氏度 转化成 华氏度
    printf("%.1f",output_f);//保留1位有效数据输出
    return 0;
}

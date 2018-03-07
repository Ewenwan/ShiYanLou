/*
* GNU 调试命令 GDB代码调试器：GDB 可以让你看到程序在执行过程时的内部流程，并帮你明确问题的所在。
// 首先在编译时，必须要把调试信息加到可执行文件中。
// 对于c程序 gcc -g hello.c -o hello   对于c++程序 g++ -g hello.cpp -o hello
gcc –g gdb.c –o testGDB	编译时加入 调试信息
gdb testgdb <---------- 启动gdb
l           <---------- 键入l相当于 list 列出源码
break 16    <---------- 设置断点，在源程序 16行处
break func 	<---------- 设置断点，在调用函数func() 的地方 即函数入口处
info break  <---------- 查看设置的断点信息
r 			<---------- 继续运行程序 run命令 的缩写
Breakpoint 1, main () at test.c:16  <---------- 会在设置的断点1处停住。
n           <----------- 单条语句执行，next命令简写
c           <-----------继续运行程序，continue命令简写。
result[1-100] = 5050  <----------程序输出。
Breakpoint 2, func (n=250) at test.c:5 <---------- 会在设置的断点2处停住。
n
p i    <--------------------- 打印变量i的值，print命令简写。
bt     <--------------------- backtrace回溯 查看函数堆栈。通过gdb的堆栈跟踪，可以看到所有已调用的函数列表，以及每个函数在栈中的信息
finish <--------------------- 退出函数
c      <--------------------- 继续运行程序，continue命令简写。
q      <--------------------- 退出gdb。

*/
// gcc -o gdbt gnudebugger.c
// gcc -g gnudebugger.c -o gdbt
// gdb gdbt
// list    l
// break 16   # 16 line set break point
// break func_sum  # 
// info break
// r #run
// next   n
// continue  c
// print     p i
// p sum
// bt
// finish
#include <stdio.h>
int func_sum(int n)
{
    int sum=0,i;
    for(i=0; i<n; i++) {
        sum+=i;
    }
    return sum;
}

int main(void)
{
    int i;
    long result = 0;
    for(i=1; i<=100; i++) {
        result += i;
    }
    printf("result[1-100] = %ld \n", result );
    printf("result[1-250] = %d \n", func_sum(250) );
 }

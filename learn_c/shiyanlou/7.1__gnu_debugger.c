/*
* GNU 调试命令
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

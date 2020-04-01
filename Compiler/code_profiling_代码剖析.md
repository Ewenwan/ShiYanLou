# 代码剖析（Code profiling） 代码 内存、耗时、调用等情况

程序员在优化软件性能时要注意应尽量优化软件中被频繁调用的部分，这样才能对程序进行有效优化。使用真实的数据，精确的分析应用程序在时间上的花费的行为就成为_代码剖析_。

[GNU gprof](https://fooyou.github.io/document/2015/07/22/performance-tools-for-linux-cplusplus.html)

> gprof

GNU gprof 是一款linux平台上的程序分析软件（unix也有prof)。借助gprof可以获得C/C++程序运行期间的统计数据，例如每个函数耗费的时间，函数被调用的次数以及各个函数相互之间的调用关系。gprof可以帮助我们找到程序运行的瓶颈，对占据大量CPU时间的函数进行调优。

PS：gprof统计的只是用户态CPU的占用时间，不包括内核态的CPU时间。gprof对I/O瓶颈无能为力，耗时甚久的I/O操作很可能只占据极少的CPU时间。

> 如何使用gprof

gprof的使用很简单，遵循以下步骤即可：

    使用编译标志-pg编译代码。
       -pg : Generate extra code to write profile information suitable for the analysis program gprof.
    运行程序生成剖析数据。
    运行gprof分析剖析数据，得到可视结果。

测试demo
```c
#include <stdio.h>

void func() {
    printf("Inside func\n");
    int i = 0;

    for (; i < 0xffffff; ++i);
    return;
}

void a() {
    printf("Inside a()\n");
    int i = 0;
    for (; i < 0xffffff; ++i);
    func();
    return;
}

static void b() {
    printf("Inside b()\n");
    int i = 0;
    for (; i < 0xffffff; ++i);
    return;
}

int main() {
    printf("Inside main()\n");
    int i = 0;
    for (; i < 0xfffff; ++i);
    a();
    b();
    return 0;
}

```

> 1. 使用-pg 标识编译源程序

gcc -Wall -pg test.c func.c -o test

> 2. 运行加入标记的程序

./test   会生成 gmon.out文件，可以用gprof来分析

gprof test gmon.out > analysis.txt  包含个函数执行时间和函数调用过程

可使用 -a 参数频闭静态私有参数信息

gprof -a test gmon.out > analysis.txt  

使用 -b 屏蔽冗余信息

gprof -b test gmon.out > analysis.txt 











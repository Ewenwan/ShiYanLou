# 代码剖析（Code profiling） 代码 内存、耗时、调用等情况

[bcc 基于bpf 分析linux 系统性能的强大工具包 网络监控分析工具集（当然也可以分析java，ruby，python...）](https://github.com/Ewenwan/bcc)

[linux 系统性能分析](https://nova.polymtl.ca/~suchakra/)


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




## linux系统性能分析工具

### 1. vmstat--虚拟内存统计 VirtualMeomoryStatistics

vmstat(VirtualMeomoryStatistics,虚拟内存统计) 是Linux中监控内存的常用工具,可对操作系统的虚拟内存、进程、CPU等的整体情况进行监视。

vmstat的常规用法：

vmstat interval times 

即每隔interval秒采样一次，共采样times次，如果省略times,则一直采集数据，直到用户手动停止为止。

vmstat 5  每隔5s采样一次，一直采样，可以使用ctrl+c停止vmstat采集数据。

第一行显示了系统自启动以来的平均值，第二行开始显示现在正在发生的情况，接下来的行会显示每5秒间隔发生了什么，每一列的含义在头部，如下所示：


▪ procs：r这一列显示了多少进程在等待cpu，b列显示多少进程正在不可中断的休眠（等待IO）。

▪ memory：swapd列显示了多少块被换出了磁盘（页面交换），剩下的列显示了多少块是空闲的（未被使用），多少块正在被用作缓冲区，以及多少正在被用作操作系统的缓存。

▪ swap：显示交换活动：每秒有多少块正在被换入（从磁盘）和换出（到磁盘）。

▪ io：显示了多少块从块设备读取（bi）和写出（bo）,通常反映了硬盘I/O。

▪ system：显示每秒中断(in，interrupt)和上下文切换（cs，contex switch）的数量。

▪ cpu：显示所有的cpu时间花费在各类操作的百分比，包括执行用户代码（非内核），执行系统代码（内核），空闲(id)以及等待IO(wa)。

内存不足的表现：

free memory急剧减少，回收buffer和cacher也无济于事，大量使用交换分区（swpd）,页面交换（swap）频繁，读写磁盘数量（io）增多，缺页中断（in）增多，上下文切换（cs）次数增多，等待IO的进程数（b）增多，大量CPU时间用于等待IO（wa）


### 2. iostat--用于报告中央处理器统计信息

iostat用于报告中央处理器（CPU）统计信息和整个系统、适配器、tty 设备、磁盘和 CD-ROM 的输入/输出统计信息，默认显示了与vmstat相同的cpu使用信息，使用以下命令显示扩展的设备统计： 

iostat -dx 5 每5s统计采用一次

第一行显示的是自系统启动以来的平均值，然后显示增量的平均值，每个设备一行。
常见linux的磁盘IO指标的缩写习惯：rq是request,r是read,w是write,qu是queue，sz是size,a是verage,tm是time,svc是service。

    ▪rrqm/s和wrqm/s：每秒合并的读和写请求，
        “合并的”意味着操作系统从队列中拿出多个逻辑请求合并为一个请求到实际磁盘。
    ▪r/s和w/s：每秒发送到设备的读和写请求数。
    ▪rsec/s和wsec/s：每秒读和写的扇区数。
    ▪avgrq –sz：请求的扇区数。
    ▪avgqu –sz：在设备队列中等待的请求数。
    ▪await：每个IO请求花费的时间。
    ▪svctm：实际请求（服务）时间。
    ▪%util：至少有一个活跃请求所占时间的百分比。
    
### 3. dstat--系统监控工具

dstat显示了cpu使用情况，磁盘io情况，网络发包情况和换页情况，输出是彩色的，可读性较强，相对于vmstat和iostat的输入更加详细且较为直观。在使用时，直接输入命令即可，当然也可以使用特定参数。

如下：dstat –cdlmnpsy

### 4. iotop--LINUX进程实时监控工具

iotop命令是专门显示硬盘IO的命令，界面风格类似top命令，可以显示IO负载具体是由哪个进程产生的。是一个用来监视磁盘I/O使用状况的top类工具，具有与top相似的UI，其中包括PID、用户、I/O、进程等相关信息。

可以以非交互的方式使用：iotop –bod interval，查看每个进程的I/O，可以使用pidstat，pidstat –d instat。

### 5. pidstat--监控系统资源情况

pidstat主要用于监控全部或指定进程占用系统资源的情况,如CPU,内存、设备IO、任务切换、线程等。

使用方法：pidstat –d interval；pidstat还可以用以统计CPU使用信息：pidstat –u interval；统计内存信息：Pidstat –r interval。


### 6. top

top命令的汇总区域显示了五个方面的系统性能信息：

    1.负载：时间，登陆用户数，系统平均负载；
    2.进程：运行，睡眠，停止，僵尸；
    3.cpu:用户态，核心态，NICE,空闲，等待IO,中断等；
    4.内存：总量，已用，空闲（系统角度），缓冲，缓存；
    5.交换分区：总量，已用，空闲

任务区域默认显示：进程ID,有效用户，进程优先级，NICE值，进程使用的虚拟内存，物理内存和共享内存，进程状态，CPU占用率，内存占用率，累计CPU时间，进程命令行信息。

### 7. htop
htop 是Linux系统中的一个互动的进程查看器,一个文本模式的应用程序(在控制台或者X终端中),需要ncurses。

Htop可让用户交互式操作，支持颜色主题，可横向或纵向滚动浏览进程列表，并支持鼠标操作。
与top相比，htop有以下优点：

    ▪ 可以横向或者纵向滚动浏览进程列表，以便看到所有的进程和完整的命令行。
    ▪ 在启动上，比top更快。
    ▪ 杀进程时不需要输入进程号。
    ▪ htop支持鼠标操作。

### 8. mpstat

mpstat 是Multiprocessor Statistics的缩写，是实时系统监控工具。其报告与CPU的一些统计信息，这些信息存放在/proc/stat文件中。在多CPUs系统里，其不但能查看所有CPU的平均状况信息，而且能够查看特定CPU的信息。常见用法：mpstat –P ALL interval times。


### 9. ps--显示当前进程的状态 

ps参数太多，具体使用方法可以参考man ps，常用的方法：ps aux #hsserver；ps –ef |grep 

#hundsun

▪ 杀掉某一程序的方法：ps aux | grep mysqld | grep –v grep | awk ‘{print $2 }’ xargs kill -9
▪ 杀掉僵尸进程：ps –eal | awk ‘{if ($2 == “Z”){print $4}}’ | xargs kill -9

### 10. strace

跟踪程序执行过程中产生的系统调用及接收到的信号，帮助分析程序或命令执行中遇到的异常情况。
举例：查看mysqld在linux上加载哪种配置文件，可以通过运行下面的命令：strace –e stat64 mysqld –print –defaults > /dev/null


### 11. perf

perf是Linux kernel自带的系统性能优化工具。优势在于与Linux Kernel的紧密结合，它可以最先应用到加入Kernel的new feature，用于查看热点函数，查看cashe miss的比率，从而帮助开发者来优化程序性能。

性能调优工具如 perf，Oprofile 等的基本原理都是对被监测对象进行采样，最简单的情形是根据 tick 中断进行采样，即在 tick 中断内触发采样点，在采样点里判断程序当时的上下文。假如一个程序 90% 的时间都花费在函数 foo() 上，那么 90% 的采样点都应该落在函数 foo() 的上下文中。运气不可捉摸，但我想只要采样频率足够高，采样时间足够长，那么以上推论就比较可靠。因此，通过 tick 触发采样，我们便可以了解程序中哪些地方最耗时间，从而重点分析。

进入linux内核源码，cd linux/tools/perf && make  && make install

稍微扩展一下思路，就可以发现改变采样的触发条件使得我们可以获得不同的统计数据：

        以时间点 ( 如 tick) 作为事件触发采样便可以获知程序运行时间的分布。
        以 cache miss 事件触发采样便可以知道 cache miss 的分布，即 cache 失效经常发生在哪些程序代码中。如此等等。

因此让我们先来了解一下 perf 中能够触发采样的事件有哪些。

事件分为以下三种：

1）Hardware Event 是由 PMU 硬件产生的事件，比如 cache 命中，当您需要了解程序对硬件特性的使用情况时，便需要对这些事件进行采样；

   cycles instructions cache-references（命中）cache-misses（不命中） branches branche-miss bus-cycles idle-cycles ref-cycles

2）Software Event 是内核软件产生的事件，比如进程切换，tick 数等 ;

   cpu-clock  task-clock page-faults contsxt-switches

3）Tracepoint event 是内核中的静态 tracepoint 所触发的事件，这些 tracepoint 用来判断程序运行期间内核的行为细节，比如 slab 分配器的分配次数等。

上述每一个事件都可以用于采样，并生成一项统计数据，时至今日，尚没有文档对每一个 event 的含义进行详细解释。

> perf stat——概览程序的运行情况

 面对一个问题程序，最好采用自顶向下的策略。先整体看看该程序运行时各种统计事件的大概，再针对某些方向深入细节。而不要一下子扎进琐碎细节，会一叶障目的。

有些程序慢是因为计算量太大，其多数时间都应该在使用 CPU 进行计算，这叫做 CPU bound 型；有些程序慢是因为过多的 IO，这种时候其 CPU 利用率应该不高，这叫做 IO bound 型；对于 CPU bound 程序的调优和 IO bound 的调优是不同的。

如果您认同这些说法的话，Perf stat 应该是您最先使用的一个工具。它通过概括精简的方式提供被调试程序运行的整体情况和汇总数据。   




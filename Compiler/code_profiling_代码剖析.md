# 代码剖析（Code profiling） 代码 内存、耗时、调用等情况

[bcc 基于bpf 分析linux 系统性能的强大工具包 网络监控分析工具集（当然也可以分析java，ruby，python...）](https://github.com/Ewenwan/bcc)

[linux 系统性能分析](https://nova.polymtl.ca/~suchakra/)

[Perf -- Linux下的系统性能调优工具](https://www.ibm.com/developerworks/cn/linux/l-cn-perf1/)

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

现代高性能 CPU 的流水线非常复杂。 一般来说，CPU 流水线在概念上分为两部分，即前端（Front-end）和后端（Back-end）。Front-end 负责获取程序代码指令，并将其解码为一个或多个称为微操作（uOps）的底层硬件指令。uOps 被分配给 Back-end 进行执行，Back-end 负责监控 uOp 的数据何时可用，并在可用的执行单元中执行 uOp。 uOp 执行的完成称为退役（Retirement），uOp 的执行结果提交并反馈到>架构状态（CPU 寄存器或写回内存）。 通常情况下，大多数 uOps 通过流水线正常执行然后退役，但有时候**投机执行（预测出来执行的）**的 uOps 可能会在退役前被取消，例如在分支预测错误的情况下。

在最近的英特尔微体系结构上，流水线的 Front-end 每个 CPU 周期（cycle）可以分配4个 uOps ，而 Back-end 可以在每个周期中退役4个 uOps。 流水线槽（pipeline slot）代表处理一个 uOp 所需的硬件资源。在每个 CPU 周期中，pipeline slot 可以是空的或者被 uOp 填充。 如果在一个 CPU 周期内某个 pipeline slot 是空的，称之为一次停顿（stall）。如果 CPU 经常停顿，系统性能肯定是受到影响的。


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

Perf 是用来进行软件性能分析的工具。

Performance counter ---> Performance Event

通过它，应用程序可以利用 PMU，tracepoint 和内核中的特殊计数器来进行性能统计。它不但可以分析指定应用程序的性能问题 (per thread)，也可以用来分析内核的性能问题，当然也可以同时分析应用代码和内核，从而全面理解应用程序中的性能瓶颈。

perf是Linux kernel自带的系统性能优化工具。优势在于与Linux Kernel的紧密结合，它可以最先应用到加入Kernel的new feature，用于查看热点函数，查看cashe miss的比率，从而帮助开发者来优化程序性能。

使用 Perf 可以计算每个时钟周期内的指令数，称为 IPC，IPC 偏低表明代码没有很好地利用 CPU。Perf 还可以对程序进行函数级别的采样，从而了解程序的性能瓶颈究竟在哪里等等。Perf 还可以替代 strace，可以添加动态内核 probe 点，还可以做 benchmark 衡量调度器的好坏.

背景知识，比如硬件 cache；再比如操作系统内核。应用程序的行为细节往往是和这些东西互相牵扯的，这些底层的东西会以意想不到的方式影响应用程序的性能，比如某些程序无法充分利用 cache，从而导致性能下降。比如不必要地调用过多的系统调用，造成频繁的内核 / 用户切换。

性能调优工具如 perf，Oprofile 等的基本原理都是对被监测对象进行采样，最简单的情形是根据 tick 中断进行采样，即在 tick 中断内触发采样点，在采样点里判断程序当时的上下文。假如一个程序 90% 的时间都花费在函数 foo() 上，那么 90% 的采样点都应该落在函数 foo() 的上下文中。运气不可捉摸，但我想只要采样频率足够高，采样时间足够长，那么以上推论就比较可靠。因此，通过 tick 触发采样，我们便可以了解程序中哪些地方最耗时间，从而重点分析。

进入linux内核源码，cd linux/tools/perf && make  && make install

稍微扩展一下思路，就可以发现改变采样的触发条件使得我们可以获得不同的统计数据：

        以时间点 ( 如 tick) 作为事件触发采样便可以获知程序运行时间的分布。
        以 cache miss 事件触发采样便可以知道 cache miss 的分布，即 cache 失效经常发生在哪些程序代码中。如此等等。

因此让我们先来了解一下 perf 中能够触发采样的事件有哪些。

事件分为以下三种：

[perf.wiki](https://perf.wiki.kernel.org/index.php/Tutorial)

1）Hardware Event 是由 PMU performance monitor unit(性能监控单元) 硬件产生的事件，比如 cache 命中，当您需要了解程序对硬件特性的使用情况时，便需要对这些事件进行采样；

   cycles /instructions/ cache-references（命中）/cache-misses（不命中）/ branches/ branche-miss/ bus-cycles /idle-cycles /ref-cycles
   

2）Software Event 是内核软件产生的事件，比如进程切换，tick 数等 ;

   cpu-clock / task-clock/ page-faults /contsxt-switches /minor-faults  /major-faults/cpu-migrations/alignment-faults/emulation-faults

3）Tracepoint event 是内核中的静态 tracepoint 所触发的事件，这些 tracepoint 用来判断程序运行期间内核的行为细节，比如 slab 分配器的分配次数等。
   
     kvmmmu:kvm_mmu_pagetable_walk   
     sched:sched_stat_runtime                   [Tracepoint event]
     sched:sched_pi_setprio                     [Tracepoint event]
     syscalls:sys_enter_socket                  [Tracepoint event]
     syscalls:sys_exit_socket                   [Tracepoint event]
     ext4:ext4_allocate_inode                   [Tracepoint event] 
     kmem:kmalloc                               [Tracepoint event] 
     module:module_load                         [Tracepoint event] 
     workqueue:workqueue_execution              [Tracepoint event] 
     sched:sched_{wakeup,switch}                [Tracepoint event] 
     syscalls:sys_{enter,exit}_epoll_wait       [Tracepoint event] 
 
4) Hardware cache event

         L1-dcache-loads   
         L1-dcache-load-misses   
         L1-dcache-stores    
         L1-dcache-store-misses   
         L1-dcache-prefetches    
         L1-dcache-prefetch-misses 
         L1-icache-loads             
         L1-icache-load-misses            
         L1-icache-prefetches         
         L1-icache-prefetch-misses       
         LLC-loads                             
         LLC-load-misses                 
         LLC-stores                       
         LLC-store-misses       
         LLC-prefetch-misses                        [Hardware cache event]
         dTLB-loads                                 [Hardware cache event]
         dTLB-load-misses                           [Hardware cache event]
         dTLB-stores                                [Hardware cache event]
         dTLB-store-misses                          [Hardware cache event]
         dTLB-prefetches                            [Hardware cache event]
         dTLB-prefetch-misses                       [Hardware cache event]
         iTLB-loads                                 [Hardware cache event]
         iTLB-load-misses                           [Hardware cache event]
         branch-loads                               [Hardware cache event]
         branch-load-misses                         [Hardware cache event]

上述每一个事件都可以用于采样，并生成一项统计数据，时至今日，尚没有文档对每一个 event 的含义进行详细解释。

#### 基础知识 

> **硬件特性之 cache**

内存读写是很快的，但还是无法和处理器的指令执行速度相比。为了从内存中读取指令和数据，处理器需要等待，用处理器的时间来衡量，这种等待非常漫长。Cache 是一种 SRAM，它的读写速率非常快，能和处理器处理速度相匹配。因此将常用的数据保存在 cache 中，处理器便无须等待，从而提高性能。Cache 的尺寸一般都很小，充分利用 cache 是软件调优非常重要的部分。

> **硬件特性之流水线，超标量体系结构，乱序执行**

提高性能最有效的方式之一就是并行。处理器在硬件设计时也尽可能地并行，比如流水线，超标量体系结构以及乱序执行。

1. 处理器处理一条指令需要分多个步骤完成，比如先取指令，然后完成运算，最后将计算结果输出到总线上。在处理器内部，这可以看作一个**三级流水线**.
指令从左边进入处理器，上图中的流水线有三级，一个时钟周期内可以同时处理三条指令，分别被流水线的不同部分处理。

2. **超标量（superscalar）**指一个时钟周期发射多条指令的流水线机器架构，比如 Intel 的 Pentium 处理器，内部有两个执行单元，在一个时钟周期内允许执行两条指令。

3. 在处理器内部，不同指令所需要的处理步骤和时钟周期是不同的，如果严格按照程序的执行顺序执行，那么就无法充分利用处理器的流水线。因此指令有可能被**乱序执行**。

上述三种并行技术对所执行的指令有一个基本要求，即**相邻的指令相互没有依赖关系**。假如某条指令需要依赖前面一条指令的执行结果数据，那么 pipeline 便失去作用，因为第二条指令必须等待第一条指令完成。因此好的软件必须尽量避免这种代码的生成。例如load指令后，如果紧跟着使用该load数据进行计算的指令，那么会造成等待

> **硬件特性之分支预测 branche-misses 分支预测不命中（ BTB 失效）  跳转条件正误预测 可以提前载入相应分支的指令**

分支指令对软件性能有比较大的影响。尤其是当处理器采用流水线设计之后，假设流水线有三级，当前进入流水的第一条指令为分支指令。假设处理器顺序读取指令，那么如果分支的结果是跳转到其他指令，那么被处理器流水线预取的后续两条指令都将被放弃，从而影响性能。为此，很多处理器都提供了分支预测功能，根据同一条指令的历史执行记录进行预测，读取最可能的下一条指令，而并非顺序读取指令。

分支预测对软件结构有一些要求，对于重复性的分支指令序列，分支预测硬件能得到较好的预测结果，而对于类似 switch case 一类的程序结构，则往往无法得到理想的预测结果。相当于**指令不命中**.

上面介绍的几种处理器特性对软件的性能有很大的影响，然而依赖时钟进行定期采样的 profiler 模式无法揭示程序对这些处理器硬件特性的使用情况。处理器厂商针对这种情况，在硬件中加入了 PMU 单元，即 performance monitor unit(性能监控单元)。

PMU 允许软件针对某种硬件事件设置 counter，此后处理器便开始统计该事件的发生次数，当发生的次数超过 counter 内设置的值后，便产生中断。比如 cache miss 达到某个值后，PMU 便能产生相应的中断。

捕获这些中断，便可以考察程序对这些硬件特性的利用效率了。

> **Tracepoints，自行车轮胎上的 油漆斑点**

Tracepoint 是散落在内核源代码中的一些 hook，一旦使能，它们便可以在特定的代码被运行到时被触发，这一特性可以被各种 trace/debug 工具所使用。Perf 就是该特性的用户之一。

假如您想知道在应用程序运行期间，内核内存管理模块的行为，便可以利用潜伏在 slab 分配器中的 tracepoint。当内核运行到这些 tracepoint 时，便会通知 perf。

Perf 将 tracepoint 产生的事件记录下来，生成报告，通过分析这些报告，调优人员便可以了解程序运行时期内核的种种细节，对性能症状作出更准确的诊断。



####  perf stat——概览程序的运行情况

 面对一个问题程序，最好采用自顶向下的策略。先整体看看该程序运行时各种统计事件的大概，再针对某些方向深入细节。而不要一下子扎进琐碎细节，会一叶障目的。

有些程序慢是因为计算量太大，其多数时间都应该在使用 CPU 进行计算，这叫做 CPU bound 型；有些程序慢是因为过多的 IO，这种时候其 CPU 利用率应该不高，这叫做 IO bound 型；对于 CPU bound 程序的调优和 IO bound 的调优是不同的。

如果您认同这些说法的话，Perf stat 应该是您最先使用的一个工具。它通过概括精简的方式提供被调试程序运行的整体情况和汇总数据。   


> 测试

demo
```c
 //test.c 
 void longa() 
 { 
   int i,j; 
   for(i = 0; i < 1000000; i++) 
   j=i; //am I silly or crazy? I feel boring and desperate. 
 } 
 void foo2() 
 { 
   int i; 
   for(i=0 ; i < 10; i++) 
        longa(); 
 } 
 void foo1() 
 { 
   int i; 
   for(i = 0; i< 100; i++) 
      longa(); 
 } 
 int main(void) 
 { 
   foo1(); 
   foo2(); 
 } 

```

找到这个程序的性能瓶颈无需任何工具，肉眼的阅读便可以完成。Longa() 是这个程序的关键，只要提高它的速度，就可以极大地提高整个程序的运行效率。

但，因为其简单，却正好可以用来演示 perf 的基本使用。假如 perf 告诉您这个程序的瓶颈在别处，您就不必再浪费宝贵时间阅读本文了。


编译为可执行文件 test1   gcc – o test1 – g test.c  此处一定要加-g选项，加入调试和符号表信息。

perf stat ./test1   输出上下文切换数量 clock等信息

结果分析：

对 test1进行调优应该要找到热点 ( 即最耗时的代码片段 )，再看看是否能够提高热点代码的效率。

Task-clock-msecs：CPU 利用率，该值高，说明程序的多数时间花费在 CPU 计算上而非 IO。

Context-switches：cs上下文切换，进程切换次数，记录了程序运行过程中发生了多少次进程切换，频繁的进程切换是应该避免的。

CPU-migrations：表示进程 t1 运行过程中发生了多少次 CPU 迁移，即被调度器从一个 CPU 转移到另外一个 CPU 上运行。

Cycles：处理器时钟，一条机器指令可能需要多个 cycles，

Instructions: 机器指令数目。

IPC：是 Instructions/Cycles 的比值，该值越大越好，说明程序充分利用了处理器的特性。

Cache-references: cache 命中的次数

Cache-misses: cache 失效的次数。程序运行过程中总体的 cache 利用情况，如果该值过高，说明程序的 cache 利用不好

#### 精确制导——定位程序瓶颈perf record && perf report

> 查找时间上的热点函数   hot spot

     perf record – e cpu-clock -g ./test1     或者 perf record -e cpu-clock -g -p 4522    -p后面接已经启动的进程id
     使用ctrl+c中断perf进程，或者在程序执行结束后，会产生perf.data的文件，使用
     perf report 
     会产生结果分析 
     
[perf的结果可以生成火焰图。生成火焰图需要借助Flame Graph](https://github.com/Ewenwan/FlameGraph)
     
     1、使用perf script工具对perf.data进行解析
       perf script -i perf.data &> perf.unfold 
     2、将perf.unfold中的符号进行折叠：
      /data/stackcollapse-perf.pl perf.unfold &> perf.folded 
     3、最后生成svg图：
      /data/flamegraph.pl perf.folded > perf.svg
      
得到的结果有3个问题：

1）perf未能定位本地符号表对应的symbol和地址的对应关系：0x000003d4对应的什么函数？ 高版本可能已经解决了 

2）采样频率不够高，失去了一些函数的信息：显然一些内核函数没有显示在上面的结果中，因为采样频率如果不够高，那么势必会有一些函数中的采样点没有/

3）如何克服采样的随机性带来的问题：为了在测量更加逼近正确值，我们采用多次重复取平均值的方法来逼近真实值。（这里可以用-r来指定重复次数）


对于问题2），我们可以用perf record -F count 来指定采样频率加以解决：

    perf record -F 50000 -e cpu-clock ./test1
    perf report 


使用 perf 的 -g 选项便可以得到需要的信息：当然，这里符号表没有定位的问题有依然没有解决！

    perf record -g -e cpu-clock ./test1 
    perf report
    
下面我用 ls 命令来演示 sys_enter 这个 tracepoint 的使用：

perf stat -e raw_syscalls:sys_enter ls    系统调用次数

# 实验一：系统软件启动过程

## 实验目的：
    操作系统是一个软件，也需要通过某种机制加载并运行它。
    在这里我们将通过另外一个更加简单的软件-bootloader来完成这些工作。
    为此，我们需要完成一个能够切换到x86的保护模式并显示字符的bootloader，为启动操作系统ucore做准备。
    lab1提供了一个非常小的bootloader和ucore OS，
    整个bootloader执行代码小于512个字节，这样才能放到硬盘的主引导扇区中。
    通过分析和实现这个bootloader和ucore OS，
    
    读者可以了解到：
    
### 1. 计算机原理
        CPU的编址与寻址: 基于分段机制的内存管理
        CPU的中断机制
        外设：串口/并口/CGA，时钟，硬盘

### 2. Bootloader软件
        编译运行bootloader的过程
        调试bootloader的方法
        PC启动bootloader的过程
        ELF执行文件的格式和加载
        外设访问：读硬盘，在CGA上显示字符串

### 3. ucore OS软件
        编译运行ucore OS的过程
        ucore OS的启动过程
        调试ucore OS的方法
        函数调用关系：在汇编级了解函数调用栈的结构和处理过程
        中断管理：与软件相关的中断处理
        外设管理：时钟
## 实验内容：
    lab1中包含一个bootloader和一个OS。
    这个bootloader可以切换到X86保护模式，能够读磁盘并加载ELF执行文件格式，并显示字符。
    而这lab1中的OS只是一个可以处理时钟中断和显示字符的幼儿园级别OS。
    
## 项目组成
    lab1的整体目录结构如下所示：
    >>> tree
      .
      ├── bin            // 编译后生成
      │   ├── bootblock  
      │   ├── kernel     // 内核系统
      │   ├── sign       // 用于生成一个符合规范的硬盘主引导扇区
      │   └── ucore.img　// ucore.img是一个包含了bootloader或OS的硬盘镜像
      ├── boot　　　　　　// =======bootloader 代码=================================
      │   ├── asm.h      // 是bootasm.S汇编文件所需要的头文件, 是一些与X86保护模式的段访问方式相关的宏定义.
      │   ├── bootasm.S　// 0. 定义了最先执行的函数start，部分初始化，从实模式切换到保护模式，调用bootmain.c中的bootmain函数
      │   └── bootmain.c // 1. 实现了bootmain函数, 通过屏幕、串口和并口显示字符串,加载ucore操作系统到内存，然后跳转到ucore的入口处执行.
      ├── kern           // =======ucore系统部分===================================
      │   ├── debug// 内核调试部分 ==================================================
      │   │   ├── assert.h   // 保证宏 assert宏，在发现错误后调用 内核监视器kernel monitor
      │   │   ├── kdebug.c　 // 提供源码和二进制对应关系的查询功能，用于显示调用栈关系。
      │   │   ├── kdebug.h   // 其中补全print_stackframe函数是需要完成的练习。其他实现部分不必深究。
      │   │   ├── kmonitor.c // 实现提供动态分析命令的kernel monitor，便于在ucore出现bug或问题后，
      │   │   ├── kmonitor.h // 能够进入kernel monitor中，查看当前调用关系。
      │   │   ├── panic.c    // 内核错误(Kernel panic)是指操作系统在监测到内部的致命错误,
      │   │   └── stab.h
      │   ├── driver//驱动==========================================================
      │   │   ├── clock.c    // 实现了对时钟控制器8253的初始化操作 系统时钟 
      │   │   ├── clock.h   
      │   │   ├── console.c  // 实现了对串口和键盘的中断方式的处理操作 串口命令行终端
      │   │   ├── console.h
      │   │   ├── intr.c     // 实现了通过设置CPU的eflags来屏蔽和使能中断的函数
      │   │   ├── intr.h
      │   │   ├── kbdreg.h   // 
      │   │   ├── picirq.c   // 实现了对中断控制器8259A的初始化和使能操作   
      │   │   └── picirq.h
      │   ├── init// 系统初始化======================================================
      │   │   └── init.c       // ucore操作系统的初始化启动代码
      │   ├── libs
      │   │   ├── readline.c
      │   │   └── stdio.c
      │   ├── mm// 内存管理 Memory management========================================
      │   │   ├── memlayout.h  // 操作系统有关段管理（段描述符编号、段号等）的一些宏定义
      │   │   ├── mmu.h        // 内存管理单元硬件 Memory Management Unit 将线性地址映射为物理地址,包括EFLAGS寄存器等段定义
      │   │   ├── pmm.c　　　　 // 设定了ucore操作系统在段机制中要用到的全局变量
      │   │   └── pmm.h        // 任务状态段ts，全局描述符表 gdt[],加载gdt的函数lgdt，　初始化函数gdt_init
      │   └── trap// 陷阱trap 异常exception 中断interrupt 中断处理部分=================
      │       ├── trap.c       // 紧接着第二步初步处理后，继续完成具体的各种中断处理操作；
      │       ├── trapentry.S  // 紧接着第一步初步处理后，进一步完成第二步初步处理；
      |       |                // 并且有恢复中断上下文的处理，即中断处理完毕后的返回准备工作；
      │       ├── trap.h       // 紧接着第二步初步处理后，继续完成具体的各种中断处理操作；
      │       └── vectors.S    // 包括256个中断服务例程的入口地址和第一步初步处理实现。
      |                        // 此文件是由tools/vector.c在编译ucore期间动态生成的
      ├── libs// 公共库部分===========================================================
      │   ├── defs.h           // 包含一些无符号整型的缩写定义
      │   ├── elf.h
      │   ├── error.h
      │   ├── printfmt.c
      │   ├── stdarg.h　　　　　// argument 参数
      │   ├── stdio.h          // 标志输入输出 io
      │   ├── string.c
      │   ├── string.h
      │   └── x86.h            // 一些用GNU C嵌入式汇编实现的C函数
      ├── Makefile             // 指导make完成整个软件项目的编译，清除等工作。
      └── tools// 工具部分============================================================
          ├── function.mk      // mk模块　指导make完成整个软件项目的编译，清除等工作。
          ├── gdbinit　　　　   // gnu debugger 调试
          ├── grade.sh
          ├── kernel.ld
          ├── sign.c           // 一个C语言小程序，是辅助工具，用于生成一个符合规范的硬盘主引导扇区。
          └── vector.c         // 生成vectors.S　中断服务例程的入口地址和第一步初步处理实现
          
**cpu mmu 内存**
![](http://nieyong.github.io/wiki_cpu/CPU%E4%BD%93%E7%B3%BB%E6%9E%B6%E6%9E%84-image/mmu/mmu_va2pa.png)

**页地址映射**
![](http://nieyong.github.io/wiki_cpu/CPU%E4%BD%93%E7%B3%BB%E6%9E%B6%E6%9E%84-image/mmu/paging.png)
    
**中断异常陷阱**
![](https://www.crifan.com/files/doc/docbook/interrupt_related/release/htmls/images/interrput_trap_exception.png)


# 编译
    cd lab1/
    make 

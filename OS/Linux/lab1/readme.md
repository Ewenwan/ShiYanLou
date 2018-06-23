# 实验一：系统软件启动过程
[参考](https://blog.csdn.net/tiu2014/article/details/53998595)

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
      ├── bin  // =======编译后生成======================================
      │   ├── bootblock  // 是引导区
      │   ├── kernel     // 是操作系统内核
      │   ├── sign       // 用于生成一个符合规范的硬盘主引导扇区
      │   └── ucore.img　// ucore.img 通过dd指令，将上面我们生成的　bootblock　和　kernel 的ELF文件拷贝到ucore.img
      ├── boot　// =======bootloader 代码=================================
      │   ├── asm.h      // 是bootasm.S汇编文件所需要的头文件, 是一些与X86保护模式的段访问方式相关的宏定义.
      │   ├── bootasm.S　// 0. 定义了最先执行的函数start，部分初始化，从实模式切换到保护模式，调用bootmain.c中的bootmain函数
      │   └── bootmain.c // 1. 实现了bootmain函数, 通过屏幕、串口和并口显示字符串,加载ucore操作系统到内存，然后跳转到ucore的入口处执行.
      |                  // 生成 bootblock.out 
      |                  // 由 sign.c 在最后添加 0x55AA之后生成 规范的 512字节的
      ├── kern  // =======ucore系统部分===================================
      │   ├── debug// 内核调试部分 ==================================================
      │   │   ├── assert.h   // 保证宏 assert宏，在发现错误后调用 内核监视器kernel monitor
      │   │   ├── kdebug.c　 // 提供源码和二进制对应关系的查询功能，用于显示调用栈关系。
      │   │   ├── kdebug.h   // 其中补全print_stackframe函数是需要完成的练习。其他实现部分不必深究。
      │   │   ├── kmonitor.c // 实现提供动态分析命令的kernel monitor，便于在ucore出现bug或问题后，
      │   │   ├── kmonitor.h // 能够进入kernel monitor中，查看当前调用关系。
      │   │   ├── panic.c    // 内核错误(Kernel panic)是指操作系统在监测到内部的致命错误,
      │   │   └── stab.h
      │   ├── driver　//驱动==========================================================
      │   │   ├── clock.c    // 实现了对时钟控制器8253的初始化操作 系统时钟 
      │   │   ├── clock.h   
      │   │   ├── console.c  // 实现了对串口和键盘的中断方式的处理操作 串口命令行终端
      │   │   ├── console.h
      │   │   ├── intr.c     // 实现了通过设置CPU的eflags来屏蔽和使能中断的函数
      │   │   ├── intr.h
      │   │   ├── kbdreg.h   // 
      │   │   ├── picirq.c   // 实现了对中断控制器8259A的初始化和使能操作   
      │   │   └── picirq.h
      │   ├── init　// 系统初始化======================================================
      │   │   └── init.c       // ucore操作系统的初始化启动代码
      │   ├── libs
      │   │   ├── readline.c
      │   │   └── stdio.c
      │   ├── mm　// 内存管理 Memory management========================================
      │   │   ├── memlayout.h  // 操作系统有关段管理（段描述符编号、段号等）的一些宏定义
      │   │   ├── mmu.h        // 内存管理单元硬件 Memory Management Unit 将线性地址映射为物理地址,包括EFLAGS寄存器等段定义
      │   │   ├── pmm.c　　　　 // 设定了ucore操作系统在段机制中要用到的全局变量
      │   │   └── pmm.h        // 任务状态段ts，全局描述符表 gdt[],加载gdt的函数lgdt，　初始化函数gdt_init
      │   └── trap　// 陷阱trap 异常exception 中断interrupt 中断处理部分=================
      │       ├── trap.c       // 紧接着第二步初步处理后，继续完成具体的各种中断处理操作；
      │       ├── trapentry.S  // 紧接着第一步初步处理后，进一步完成第二步初步处理；
      |       |                // 并且有恢复中断上下文的处理，即中断处理完毕后的返回准备工作；
      │       ├── trap.h       // 紧接着第二步初步处理后，继续完成具体的各种中断处理操作；
      │       └── vectors.S    // 包括256个中断服务例程的入口地址和第一步初步处理实现。
      |                        // 此文件是由tools/vector.c在编译ucore期间动态生成的
      ├── libs　// 公共库部分===========================================================
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
      └── tools　// 工具部分============================================================
          ├── function.mk      // mk模块　指导make完成整个软件项目的编译，清除等工作。
          ├── gdbinit　　　　   // gnu debugger 调试
          ├── grade.sh
          ├── kernel.ld
          ├── sign.c           // 一个C语言小程序，是辅助工具，用于生成一个符合规范的硬盘主引导扇区。
          |                    // 规范的硬盘主引导扇区大小为512字节，结束符为0x55AA
          |                    // obj/bootblock.out( <= 500 )  +  0x55AA -> bootblock(512字节)
          └── vector.c         // 生成vectors.S　中断服务例程的入口地址和第一步初步处理实现
          
**cpu mmu 内存 关系**

![](http://nieyong.github.io/wiki_cpu/CPU%E4%BD%93%E7%B3%BB%E6%9E%B6%E6%9E%84-image/mmu/mmu_va2pa.png)

**页地址映射**

![](http://nieyong.github.io/wiki_cpu/CPU%E4%BD%93%E7%B3%BB%E6%9E%B6%E6%9E%84-image/mmu/paging.png)
    
**中断异常陷阱**

![](https://www.crifan.com/files/doc/docbook/interrupt_related/release/htmls/images/interrput_trap_exception.png)


# 编译
    cd lab1/
    make 
    
    # 变量定义 ( = or := )
    # 其中 = 和 := 的区别在于, 
    # := 只能使用前面定义好的变量, = 可以使用后面定义的变量
    # +=变量追加值 SRCS += programD.c   
    # +=变量追加值  
    # 命令前缀 
    # 前缀 @   :: 只输出命令执行的结果, 出错的话停止执行
    # 前缀 -   :: 命令执行有错的话, 忽略错误, 继续执行
    
[makefile文件见](https://github.com/Ewenwan/ucore_os_lab/blob/master/labcodes/lab1/Makefile)

**直接在硬件模拟器上运行**

[硬件模拟器qemu的安装](https://github.com/Ewenwan/ShiYanLou/blob/master/OS/Linux/lab0/qeum_install.md)
  
    make qemu


# 1. 从CPU加电后执行的第一条指令开始，单步跟踪BIOS的执行
    首先在CPU加电之后，CPU里面的ROM存储器会将其里面保存的初始值传给各个寄存器，
    其中CS:IP = 0Xf000 : fff0（CS：代码段寄存器；IP：指令寄存器），
    这个值决定了我们从内存中读数据的位置，PC = 16*CS + IP。
![](https://img-blog.csdn.net/20170103221505442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGl1MjAxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    此时系统处于实模式，并且截止到目前为止系统的总线还不是我们平常的32位，
    这时的地址总线只有20位，所以地址空间的总大小只有1M，
    而我们的BIOS启动固件就在这个1M的空间里面。

    BIOS启动固件需要提供以下的一些功能：
        ☆基本输入输出的程序
        ☆系统设置信息
        ☆开机后自检程序
        ☆系统自启动程序

    在此我们需要找到CPU加电之后的第一条指令的位置，然后在这里break，单步跟踪BIOS的执行，
    根据PC = 16*CS + IP，我们可以得到PC = 0xffff0，
    所以BIOS的第一条指令的位置为0xffff0（在这里因为此时我们的地址空间只有20位，所以是0xffff0）。

    在这里我们利用 make debug来观察BIOS的单步执行，
    所以我们首先通过Makefile文件来查看make debug的相关操作：

    #  Makefile 
    # 利用make debug来观察BIOS的单步执行
    # 首先是对qemu进行的操作
    # sleep 2　等待一段时间
    # 针对gdbinit文件进行的调试
    debug: $(UCOREIMG)
        $(V)$(QEMU) -S -s -parallel stdio -hda $< -serial null &
        $(V)sleep 2
        $(V)$(TERMINAL) -e "gdb -q -tui -x tools/gdbinit"

    # tools/gdbinit   
    file bin/kernel
    target remote :1234
    break kern_init
    continue
    
    可以看到，电脑在运行到kern_init是会触发break，然后又紧接着在下一步continue，
**执行调试**

    这里需要安装　cgdb：
    sudo apt-get install cgdb
    
**开始调试：**

    make debug
    
    会出现一个新的终端，分为上下两个窗口，上面的窗口显示运行到的源码，下面的窗口是gdb调试界面。
    由上面的分析可知:
       BIOS的第一条指令的位置为0xffff0
**查看　pc指针地址　对应的指令 前强制反汇编当前的指令 **

    x/i $pc    显示1行
    x/10i $pc　显示10行
   　
   
**我们查看 0xffff0地址内的信息：**
    
    　　x/i 0xffff0
     >>> 0xffff0:     ljmp   $0x3630,$0xf000e05b
     可以看到，BIOS的第一条指令是一条跳转指令 ljmp，然后程序会跳转到0xf000e05b，开始进行一系列的操作。
     在截图中我们看到pc：0xfff0，这是因为在x86的机器里面并没有pc这个寄存器，
     所谓的pc值是通过CS:IP而得到的，因此这里的PC所代表的是eip寄存器里面的值，低 16位的值。
     
     
     
**设置断点：**

     在gdb命令行中，使用b *[地址]
     便可以在指定内存地址设置断点，
     当qemu中的cpu执行到指定地址时，便会将控制权交给gdb。
     
    n/s都是C语言级的断点定位。 s会进入C函数内部,
    　　但是不会进入没有定位信息的函数
      　（比如没有加-g编译的代码，因为其没有C代码的行数标记，没办法定位），n不会。   
    ni/si都是汇编级别的断点定位。si会进入汇编和C函数内部,ni不会
    归纳:
        当要进入没有调试信息的库函数调试的时候，用si是唯一的方法。      
        当进入有调试信息的函数，用si和s都可以，但是他们不同，
        si是定位到汇编级别的第一个语句，但是s是进入到C级别的第一个语句.

**gdb的单步命令:**

    next 单步到程序源代码的下一行，不进入函数。
    nexti 单步一条机器指令，不进入函数。
    step 单步到下一个不同的源代码行（包括进入函数）。
    stepi 单步一条机器指令。
        

# 2. bootloader进入保护模式的过程

    我们最重要的是要理解三个问题：
    1、为何要开启A20，以及如何开启A20；
    2、如何初始化GDT表；
    3、如何使能和进入保护模式。

> 1、为何要开启A20，以及如何开启A20

    首先关于A20，我们通过查询资料以及说明文档可以知道早期的8086CPU所提供的地址线只有20位，
    所以可寻址空间为0~2^20(1MB)，但是8086的数据处理位宽16位，无法直接访问1M的地址空间，
    所以8086提供了段地址加偏移地址的转换机制。
    PC的寻址结构是segment:offset，segment和offset都是16位寄存器，
    最大值是0ffffh，所以换算成物理地址的计算方法是把segment左移4位，再加上offset，
    所以segment:offset所能表示的最大为10ffefh，而这个地址超过了1M，
    但是超过1M会发生“回卷”的现象不会报错，
    但是从下一代的80286开始，地址线成为了24位，所能访问的地址空间超过了1M，
    此时寻址超过1M时会报错，出现了向下不兼容，所以为了解决这个问题采用了A20机制。

    A20 Gate，将A20地址线控制器 和 键盘控制器的一个输出进行AND操作，
    这样来控制A20地址线的打开与关闭，

    所以在实模式下，需要确保A20开关处于关闭状态，这样可以防止访问大于1M的地址空间，但
    是在保护模式下，我们需要访问更大的内存空间，所以需要将A20的开关打开，

    如果在保护模式下，A20的开关未打开的话，此时我们只能访问奇数兆的内存，
    即只能访问0—1M，2—3M，4—5M……，所以如果我们要进入保护模式，首先就需要把A20开关给打开。

> 2、如何初始化GDT表

    接下来我们需要了解下GDT表（全局描述符表），在整个操作系统中我们只有一张GDT表，
    GDT可以放在内存的任意位置，但是CPU必须知道GDT的入口，
    在Intel里面有一个专门的寄存器GDTR用来存放GDT的入口地址，
    程序员将GDT设定在内存的某个位置之后，
    可以通过LGDT指令将GDT的入口地址加载到该寄存器里面，
    以后CPU就可以通过GDTR来访问GDT了。

> 3、如何使能和进入保护模式

    最后我们需要了解如何 使能 和 进入 保护模式，
    关于这一点我们需要了解一个寄存器CR0，

    首先我们来看下CR0寄存器的各个位代表什么：
![](https://img-blog.csdn.net/20170103224301537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGl1MjAxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    在这里由于我们需要进入保护模式，所以暂时可以先不用管其他的位，只需关注最低位的PE即可，
    PE是启用保护位(protection enable)，当设置该位的时候即开启了保护模式，
    系统上电复位的时候该位默认为0，于是便是实模式；
    当PE置1的时候，进入保护模式，实质上是开启了段级保护，只是进行了分段，没有开启分页机制，
    如果要开启分页机制的话我们需要同时置位PE和PG。
    
    有了初步了解之后我们便知道的开启保护模式的相关操作，
    首先开启A20 Gate，其次加载全局描述符表GDT，最后只需要将CR0寄存器的最低位置为1即可。
    
    接下来我们通过观察代码来查看UCore具体是如何实现相应的操作的：
```asm
    # Enable A20:
    #  For backwards compatibility with the earliest PCs, physical
    #  address line 20 is tied low, so that addresses higher than
    #  1MB wrap around to zero by default. This code undoes this.
seta20.1:
    inb $0x64, %al    # Wait for not busy(8042 input buffer empty).
    testb $0x2, %al   # 等待键盘控制器8042 0x64 端口 空闲，64h端口中的状态寄存器的值为0x2
    jnz seta20.1      # 忙的话一直等待

    movb $0xd1, %al  # 等到64h空闲之后我们会写入0xd1　0xd1 -> port 0x64
    outb %al, $0x64  # 表明我们要向60h里面写入数据， 0xd1 means: write data to 8042's P2 port

seta20.2:
    inb $0x64, %al   # 等待64h端口空闲　Wait for not busy(8042 input buffer empty).
    testb $0x2, %al  # 64h端口中的状态寄存器的值为0x2
    jnz seta20.2     # 忙的话一直等待

    movb $0xdf, %al  # 0xdf -> port 0x60 等到空闲之后，我们将0xdf写入60h端口，至此来打开A20开关。
    outb %al, $0x60  # 0xdf = 11011111, means set P2's A20 bit(the 1 bit) to 1 (第20位)
```

    首先是开启A20，根据上文我们知道需要将第20位为1即可，
    但是我们需要知道在UCore里是如何将A20置为1的。
    根据说明书我们可以知道，A20地址线由键盘控制器8042进行控制，
    我们的A20所对应的是8042里面的P21引脚，所以问题就变成了我们需要将P21引脚置1。
    
    对于8042芯片来说，有两个端口地址60h和64h。
    对于这两个端口来说，
    0x64用来发送一个键盘控制命令，
    0x60用来传递参数，所以将P21引脚置1的操作就变成了，
    我们首先利用0x64端口传递一个写入的指令，
    
    然后由0x60端口读进去相应的参数来将P21置1。
    由以下的资料我们可以知道，
    我们首先要先向 64h 发送 0xd1 的指令，写之前需要等待键盘控制器8042空闲，
    可以通过判断 64h端口中的状态寄存器的值0x2,来判断是否空闲。
    然后向 60h 发送 0xdf 的指令。
![](https://img-blog.csdn.net/20170103224433882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGl1MjAxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    在这里可能有人会有疑问，既然我们只需要将P21置为1就可以了，
    那么我们是不是可以传入多种不同的参数，只需要对应的位为1就好了，答案是不行的。
    
    我们传入的0xdf参数在这里也相当于一条指令，通过这条指令我们可以将A20的开关打开。
    在这里我们还需要注意一个问题就是当前端口(60h或者64h)是否空闲，
    只有当这两个端口空闲的时候我们才可以向其传入数据。

boot/bootasm.S
```asm
#include <asm.h>

# Start the CPU: switch to 32-bit protected mode, jump into C.
# The BIOS loads this code from the first sector of the hard disk into
# memory at physical address 0x7c00 and starts executing in real mode
# with %cs=0 %ip=7c00.

.set PROT_MODE_CSEG,        0x8                     # kernel code segment selector
.set PROT_MODE_DSEG,        0x10                    # kernel data segment selector
.set CR0_PE_ON,             0x1                     # protected mode enable flag

# start address should be 0:7c00, in real mode, the beginning address of the running bootloader
.globl start
start:
.code16                                             # Assemble for 16-bit mode
    cli                                             # Disable interrupts
    cld                                             # String operations increment

    # 初始化 Set up the important data segment registers (DS, ES, SS).
    xorw %ax, %ax                                   # Segment number zero
    movw %ax, %ds                                   # -> Data Segment
    movw %ax, %es                                   # -> Extra Segment
    movw %ax, %ss                                   # -> Stack Segment

# 1 Enable A20:
    #  For backwards compatibility with the earliest PCs, physical
    #  address line 20 is tied low, so that addresses higher than
    #  1MB wrap around to zero by default. This code undoes this.
seta20.1:
    inb $0x64, %al  # Wait for not busy(8042 input buffer empty).
    testb $0x2, %al # 等待键盘控制器8042 0x64 端口 空闲，64h端口中的状态寄存器的值为0x2
    jnz seta20.1    # 忙的话一直等待

    movb $0xd1, %al # 等到64h空闲之后我们会写入0xd1, 0xd1 -> port 0x64
    outb %al, $0x64 # 表明我们要向60h里面写入数据，0xd1 means: write data to 8042's P2 port

seta20.2:
    inb $0x64, %al  # 等待64h端口空闲， Wait for not busy(8042 input buffer empty).
    testb $0x2, %al # 64h端口中的状态寄存器的值为0x2
    jnz seta20.2    # 忙的话一直等待

    movb $0xdf, %al # 等到空闲之后，我们将0xdf写入60h端口，至此来打开A20开关，0xdf -> port 0x60
    outb %al, $0x60 # 0xdf = 11011111, means set P2's A20 bit(the 1 bit(第20位)) to 1

    # Switch from real to protected mode, using a bootstrap GDT
    # and segment translation that makes virtual addresses
    # identical to physical addresses, so that the
    # effective memory map does not change during the switch.

# 2 load gdt 加载GDT全局描述符  在后面可以看到
    lgdt gdtdesc
    # 使能和进入保护模式 
    movl %cr0, %eax      # 首先将cr0寄存器里面的内容取出来
    orl $CR0_PE_ON, %eax # 进行一个或操作, PE是启用保护位(protection enable),当设置该位的时候即开启了保护模式
    movl %eax, %cr0      # 最后将得到的结果再写回 cr0中 
    
    # 只是进行了分段，没有开启分页机制，如果要开启分页机制的话我们需要同时置位PE和PG。
    
    # Jump to next instruction, but in 32-bit code segment.
    # Switches processor into 32-bit mode.

# 3 最后通过一个长跳转指令正式进入保护模式。
    ljmp $PROT_MODE_CSEG, $protcseg

.code32                                             # Assemble for 32-bit mode
protcseg:
    # Set up the protected-mode data segment registers
    movw $PROT_MODE_DSEG, %ax                       # Our data segment selector
    movw %ax, %ds                                   # -> DS: Data Segment
    movw %ax, %es                                   # -> ES: Extra Segment
    movw %ax, %fs                                   # -> FS
    movw %ax, %gs                                   # -> GS
    movw %ax, %ss                                   # -> SS: Stack Segment

    # Set up the stack pointer and call into C. The stack region is from 0--start(0x7c00)
    movl $0x0, %ebp
    movl $start, %esp
    call bootmain

    # If bootmain returns (it shouldn't), loop.
spin:
    jmp spin

# Bootstrap GDT
.p2align 2                                # force 4 byte alignment
gdt:                                      # GDT表的入口地址 GDT全局描述符表由三个全局描述符组成
    SEG_NULLASM                           # null seg 第一个均为空描述符
    SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff) # code seg for bootloader and kernel 第二个为代码段描述符
    SEG_ASM(STA_W, 0x0, 0xffffffff)       # data seg for bootloader and kernel 第三个为数据段描述符

# gdt全局描述符表　 它里面有两个参数
gdtdesc:
    .word 0x17 # 表示的是GDT表的大小                 # sizeof(gdt) - 1
    .long gdt  # 表示的是GDT表的入口地址             # address gdt
```




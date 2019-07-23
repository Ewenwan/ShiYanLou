# linux 系统
[linux下编程代码实验 CMake/Make/Shell/Multi-thread/socket/stl/awk](https://github.com/Ewenwan/Linux_Code_Test)

[linux 软硬件驱动 ](https://github.com/Ewenwan/linux-study)

[Linux程序设计 中文版 第四版](https://github.com/Ewenwan/Beginning-Linux-Programming)

[Linux中国翻译项目](https://github.com/Ewenwan/TranslateProject)

[Vim Practice ](https://github.com/oldratlee/vim-practice)

[Linux命令必知必会](https://github.com/mylxsw/growing-up/blob/master/doc/Linux%E5%91%BD%E4%BB%A4%E5%BF%85%E7%9F%A5%E5%BF%85%E4%BC%9A.md)

[Unix环境高级编程 代码&学习笔记](https://github.com/Ewenwan/APUE)

[ubuntu deb软件包搜索下载](https://packages.ubuntu.com/)


# 操作系统启动过程
	1. bootloader bootasm.S 使能A20 加载全局描述符表GDT 使能并进入保护模式　调用bootmain
	2. bootmain.c 从硬盘读取ELF格式的os，以程序块格式存入内存,跳转到 操作系统入口函数处, kern/init.c中的kern_init函数
	3. ucore就接管了整个控制权, kern/init.c中的kern_init函数 ucore主要完成的工作包括：

	    a. 初始化终端；
	    b. 显示字符串；
	    c. 显示堆栈中的多层函数调用关系；
	    d. 切换到保护模式，启用分段机制；
	    e. 初始化中断控制器，设置中断描述符表(IDT)，初始化时钟中断，使能整个系统的中断机制；
	    f. 执行while（1）死循环。



## 基础知识
### c内联汇编

    // lab0_ex1.c
    int count=1;
    int value=1;
    int buf[10];
    void main()
    {
    // 内联汇编-----扩展内联汇编	
       asm(
    // __asm__( 也可以，避免冲突
      "cld \n\t"
            "rep \n\t"					     
            "stosl"
      :　　　　　　　　　　　　　　　# 输出　output operands 
                            # 无内容　表示没有输出结果的数据操作 . 
      : "c" (count), "a" (value) , "D" (buf[0]) # 输入 input operands
                            # 每个操作数（括号里C语言的变量）
                            # 都有一个限制符（"" 中的内容）加以描述.  a代表使用   寄存器%eax存储
                            #                                      c代表使用   寄存器%ecx存储 
                            #                                      d代表使用   寄存器%edx存储
                            #                                      D代表使用   寄存器%edi存储
                            # 冒号用来分割输入的操作和输出的操作.
                            # 如果每组内有多个操作数,用逗号分割它们.  
                            # 操作数最多为10个, 或者依照具体机器而异 .
                            #  括号里的若干操作数，依次对应%0，%1。。。等
      :               # 寄存器变化表　list of clobbered registers  
                            # "%ebx", "%edi"   
          );
    }
    // 这段内嵌汇编把 value, count装入寄存器
    // 双引号内的“限制符”是一个规定的格式。
    // 在输出操作中，这个限制符会额外多一个符号（=）。
    // 限制符主要用来决定操作数的寻址方式。
    // 同时还可指定使用某一个寄存器。
    // 参考博客　https://www.cnblogs.com/whutzhou/articles/2638498.html
    
**寄存器名称限定符**

    +---+--------------------+  
    | r |    Register(s)     |  
    +---+--------------------+  
    | a |   %eax, %ax, %al   |  
    | b |   %ebx, %bx, %bl   |  
    | c |   %ecx, %cx, %cl   |  
    | d |   %edx, %dx, %dl   |  
    | S |   %esi, %si        |  
    | D |   %edi, %di        |  
    +---+--------------------+  
**其他一些限定符**

    1. "m"：对内存的操作被允许，用一个合法内存空间来做操作数。
    2. "o"：对内存的操作被允许，但是必须支持地址偏移值，
            即，对于给出的地址，加上一个偏移量，此时也是一个合法的地址。
    3. "V"：对内存的操作被允许，但是不支持偏移量。
            也就是说，支持“m”限制符，但不支持“o”的那些地址。
    4. "i"：对立即整数（const，常值）的操作被允许，
            这个常值可以是运行到汇编内才被赋值。
    5. "n"：对立即整数的操作被允许。
            许多系统不支持汇编中的操作数小于一个字宽，对于这些操作数，应该使用“n”而非“i”。
    6. "g"：任意寄存器，内存，立即数都被允许。
            除了非通用寄存器。
            eax,ebx,ecx,edx或内存变量
    7. "q"：从eax, ebx, ecx, edx分配寄存器。 
    8. "r"：从eax, ebx, ecx, edx, esi, edi分配寄存器。
            
**最常用的修饰符有：“=”，“&”**

    1. “=”，表示此操作数类型是只写，即输出寄存器。
    　　之前的值会被输出数据值替代。
    2. “&”表示此操作数是一个很早更变的（earlyclobber）操作数。
       在指令执行过程中，输出操作数产生之前，输入操作数还未使用完成，
       所以输出操作数不能与该指令的任何输入操作数公用同一寄存器。
       这个声明就为防止这种合并寄存器的优化。
       因此，这个输入操作数可能没有被保存到寄存器。


### 编译成汇编代码

      gcc -S lab0_ex1.c　　// 默认和注意类型相同
      gcc -S -m32 lab0_ex1.c　　// 编译成32位机型
      
      默认编译输出　lab0_ex1.s
      
      打印内容
      cat lab0_ex1.s
      
      >>>>>>>>>>
      
        .file　"lab0_ex1.c"
        .globl　count
        .data                   # 数据段
        .align 4　　　　　　　　　　　　　　　　# int 类型　4位
        .type count, @object
        .size count, 4
      count:
        .long 1               # value  = 1
        .globl value
        .align 4
        .type value, @object
        .size value, 4
      value:
        .long 1               # count = 1
        .comm buf,40,32       # buf int类型的指针　4*10＝40位
        .text                   # 代码段          
        .globl main
        .type main, @function
      main:
      .LFB0:
        .cfi_startproc
        pushl %ebp　　　　　　　　　　  # 保存栈底指针
        .cfi_def_cfa_offset 8
        .cfi_offset 5, -8
        movl %esp, %ebp      # 栈顶指针　%esp ->
        .cfi_def_cfa_register 5
        pushl %edi　　　　　　　　　　  #　保存　%edi  %ebi  下面要用到　以免修改
        pushl %ebx
        .cfi_offset 7, -12
        .cfi_offset 3, -16
        movl count, %edx     #  %edx = count = 1
        movl value, %eax     #  %eax = value = 1
        movl buf, %ebx       #  %ebx = buf 为地址指针
        movl %edx, %ecx　　　　  #  %ecx = %edx = count = 1     # 要求使用%ecx存储count
        movl %ebx, %edi　　　   #  %edi = %ebx = buf 为地址指针 # 要求使用%edi存储buf
      #APP
      # 6 "lab0_ex1.c" 1
        cld 
        rep 
        stosl
      # 0 "" 2
      #NO_APP
        popl %ebx　　　　　　　　　　　# 退出时恢复现场　之前push进堆栈的，现在pop出来还原
        .cfi_restore 3
        popl %edi
        .cfi_restore 7
        popl %ebp
        .cfi_restore 5
        .cfi_def_cfa 4, 4
        ret
        .cfi_endproc
      .LFE0:
        .size main, .-main
        .ident "GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4"
        .section .note.GNU-stack,"",@progbits

### 两个数做加法的内联汇编

    int main(void)  
    {  
            int foo = 10, bar = 15;  　
            __asm__ __volatile__("addl  %%ebx,%%eax"  # 求和指令!!! 注意要使用两个%,因为一个%的语法已经被%n用掉了
                                 :"=a"(foo)           # 输出  foo = (%eax += %ebx)
                                                      # 这个“=”表示这是输出结果的寄存器
                                 :"a"(foo), "b"(bar)  # 输入　%eax = foo; %ebx = bar
                                 );  
            printf("foo+bar=%d\n", foo);  
            return 0;  
    }  

## make和Makefile
    GNU make(简称make)是一种代码维护工具，在大中型项目中，
    它将根据程序各个模块的更新情况，自动的维护和生成目标代码。
    
    make命令执行时，需要一个 makefile （或Makefile）文件，
    以告诉make命令需要怎么样的去编译和链接程序。
    
### makefile的规则

    target ... : prerequisites ...
        command
        ...
        ...
    解释:
    target:
        也就是一个目标文件，
        可以是object file，
        也可以是执行文件,
        还可以是一个标签（label）。
    prerequisites:
        就是，要生成那个target所需要的文件或是目标.
    command:
        也就是make需要执行的命令（任意的shell命令）.

     这是一个文件的依赖关系，也就是说，
     target这一个或多个的目标文件依赖于prerequisites中的文件，
     其生成规则定义在 command中。   

     如果prerequisites中有一个以上的文件比target文件要新，
     那么command所定义的命令就会被执行。
     这就是makefile的规则。
     也就是makefile中最核心的内容。

### gdb调试工具的使用
    gdb 是功能强大的调试程序，可完成如下的调试任务：
        1. 设置断点
        2. 监视程序变量的值
        3. 程序的单步(step in/step over)执行
        4. 显示/修改变量的值
        5. 显示/修改寄存器
        6. 查看程序的堆栈情况
        7. 远程调试
        8. 调试线程
**步骤**

    在可以使用 gdb 调试程序之前，必须使用 -g 或 –ggdb编译选项编译源文件。
    gcc -g hello.c -o hello
    运行 gdb 调试程序时通常使用如下的命令：
    gdb hello
    
**在 gdb 提示符处键入help，将列出命令的分类，主要的分类有**

    1. aliases：    命令别名
    2. breakpoints：断点定义；
    3. data：       数据查看；
    4. files：      指定并查看文件；
    5. internals：  维护命令；
    6. running：    程序执行；
    7. stack：      调用栈查看；
    8. status：     状态查看；
    9. tracepoints：跟踪程序执行。
    键入 help 后跟命令的分类名，可获得该类命令的详细清单。

**gdb的常用命令如下表所示**

    break FILENAME:NUM  在特定源文件 特定行(NUM指定) 上设置断点
    clear FILENAME:NUM  删除设置在特定源文件特定行上的断点
    run                 运行调试程序
    step                单步执行调试程序，不会直接执行函数
    next                单步执行调试程序，会直接执行函数
    backtrace           显示所有的调用栈帧。该命令可用来显示函数的调用顺序
    where continue      继续执行正在调试的程序
    display EXPR        每次程序停止后显示表达式的值,表达式由程序定义的变量组成
    file FILENAME       装载指定的可执行文件进行调试
    help CMDNAME        显示指定调试命令的帮助信息
    info break          显示当前断点列表，包括到达断点处的次数等
    info files          显示被调试文件的详细信息
    info func           显示被调试程序的所有函数名称
    info prog           显示被调试程序的执行状态
    info local          显示被调试程序当前函数中的局部变量信息
    info var            显示被调试程序的所有全局和静态变量名称
    kill                终止正在被调试的程序
    list                显示被调试程序的源代码
    quit                退出 gdb
### 示例查找一个错误程序发生错误的地方
**错误程序输入一个字符串，指针未赋初值，野指针**
```c
/*bugging.c*/
#include <stdio.h>
#include <stdlib.h>

static char buff [256];
static char* string;
int main ()
{
    printf ("Please input a string: ");
    gets (string);
    printf ("\nYour string is: %s\n", string);
}
```
#### 编译　运行
    gcc -o bugging -g bugging.c
    ./bugging >>>>
    Please input a string: asdf
    Segmentation fault (core dumped)
    将出现 "Segment Fault" 段错误　野指针指飞了。

#### gbd调试查找问题
    1. 加载 bugging 可执行文件
       gdb bugging 
    2. 运行程序
       run
       >>> 会提示输入一个string
       >>> trer 随便输入
       发生段错误
    3. 查看程序出错的地方
      where
       >>> 
       #0  _IO_gets (buf=0x0) at iogets.c:54   　　　　　　　#显示在gets附近出错
       #1  0x000000000040059f in main () at bugging.c:10   
    4. 利用 list 命令查看调用 gets 函数附近的代码
       list
       >>> in iogets.c   
    5. 在源程序中　gets附近添加断点 在第 11 行处设置断点
      break 11  
    6. next 继续运行　结束
    7. run 重新运行
       程序重新运行到第 11 行处停止
    8. 打印 string 的值 
      print string
      >>> 
       $1 = 0x0
       显示为空指针
    9. kill & quit  结束并退出  
    
###  a. 修改程序
        问题在于string指向的是一个无效指针，
        修改程序，在10行和11行之间增加一条语句 “string=buff; ”，
        重新编译程序，然后继续运行，将看到正确的程序运行结果
```c
/*bugging.c*/
#include <stdio.h>
#include <stdlib.h>

static char buff [256];
static char* string;
int main ()
{
    printf ("Please input a string: ");
    string = buff;//　指定地址
    gets (string);
    printf ("\nYour string is: %s\n", string);
}
```
# c 函数调用
```c
// isone.c
void X(int b) {
  if (b==1){
    printf("X:: b is 1!\n");
  }else{
    printf("X:: b is not 1!\n");
  }
}

int main(int argc, char * argv){
    int a=2;
    X(a);
}
```
**编译流程**
[目标文件 ELF文件结构](https://www.jianshu.com/p/863b279c941e)

[文件编译过程](https://img-blog.csdn.net/20161022190123798)

	1. 预处理　
		gcc -E isone.c -o isone.i
	2. 编译 编译成汇编语言
		gcc -s isone.i -o isone.s
	3. 汇编 编译成机器语言(目标文件 ELF文件结构)
	     	gcc -c isone.s -o isone.o
		
		objdump　反汇编查看：
	    	objdump -d isone.o  得到汇编格式
		objdump -s isone.o  得到elf格式
		
		readelf 分析目标文件:
		readelf -a isone.o
			ELF 头：
			  Magic：   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
			  类别:                              ELF64
			  数据:                              2 补码，小端序 (little endian)
			  版本:                              1 (current)
			  OS/ABI:                            UNIX - System V
			  ABI 版本:                          0
			  类型:                              EXEC (可执行文件)
			  系统架构:                          Advanced Micro Devices X86-64
			  版本:                              0x1
			  入口点地址：               0x400440
			  程序头起点：          64 (bytes into file)
			  Start of section headers:          4512 (bytes into file)
			  标志：             0x0
			  本头的大小：       64 (字节)
			  程序头大小：       56 (字节)
			  Number of program headers:         9
			  节头大小：         64 (字节)
			  节头数量：         30
			  字符串表索引节头： 27
			  
		nm分析 目标文件 用于显示二进制目标文件的符号表
		nm isone.o
		
	4. 链接
		gcc isone.o -o isone.out




# ｃ语言 结构体指针　指针和类型转换相关的Ｃ编程
```c
#if 0
#include <stdio.h>

#define STS_IG32        0xE            // 32-bit Interrupt Gate
#define STS_TG32        0xF            // 32-bit Trap Gate

typedef unsigned uint32_t;

//设置结构体参数
#define SETGATE(gate, istrap, sel, off, dpl) {          \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;      \
    (gate).gd_ss = (sel);                               \
    (gate).gd_args = 0;                                 \
    (gate).gd_rsv1 = 0;                                 \
    (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = (dpl);                              \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}

 /* Gate descriptors for interrupts and traps */
// 数据结构　门
 struct gatedesc {
    unsigned gd_off_15_0 : 16;   // 偏置　   low 16 bits of offset in segment
    unsigned gd_ss : 16;         // 段选择器 segment selector
    unsigned gd_args : 5;        // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;        // reserved(should be zero I guess)
    unsigned gd_type : 4;        // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;           // must be 0 (system)
    unsigned gd_dpl : 2;         // descriptor(meaning new) privilege level
    unsigned gd_p : 1;           // Present
    unsigned gd_off_31_16 : 16;  // high bits of offset in segment
 };
 
int
main(void)
{
    unsigned before;
    unsigned intr;
    unsigned after;
    struct gatedesc gintr;//结构体变量
    intr=8;
    before=after=0;
// 取地址　&intr　强制类型转换　(struct gatedesc *)
// *() 再取地址指针指向的值
    gintr=*((struct gatedesc *)&intr);
    printf("befer gintr is 0x%llx\n",gintr);// 0x8
    printf("befer intr is 0x%x\n",intr);// 0x8
    SETGATE(gintr, 0,1,2,3);
    printf("after gintr is 0x%llx\n",gintr);// 0xee0000010002
    printf("after intr is 0x%x\n",intr);// 0x8
// 取地址　&intr　强制类型转换　(unsigned *)
// *() 再取地址指针指向的值
    intr=*(unsigned *)&(gintr);//低位
    printf("after 22  intr is 0x%x\n",intr);//　0x10002
    return 0;
}
#endif
/*
befer gintr is 0x8
befer intr is 0x8
after gintr is 0xee0000010002
after intr is 0x8
after 22  intr is 0x10002

*/
// other examples
//ex1
#if 0
#include <stdlib.h>
#include <stdio.h>
//#include <iostream>
//#include <cstring>
typedef unsigned uint32_t;
#define STS_TG32 0xF
#define STS_IG32 0xE
//　结构体对象访问成员 .访问
#define SETGATE( gate, istrap, sel, off, dpl) {         \
    (gate).gd_off_15_0 = (uint32_t)(off) & 0xffff;      \
    (gate).gd_ss = (sel);                               \
    (gate).gd_args = 0;                                 \
    (gate).gd_rsv1 = 0;                                 \
    (gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;    \
    (gate).gd_s = 0;                                    \
    (gate).gd_dpl = (dpl);                              \
    (gate).gd_p = 1;                                    \
    (gate).gd_off_31_16 = (uint32_t)(off) >> 16;        \
}
//using namespace std;

struct gatedesc {
    unsigned gd_off_15_0 : 16;    // low 16 bits of offset in segment
    unsigned gd_ss : 16;          // segment selector
    unsigned gd_args : 5;          // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;          // reserved(should be zero I guess)
    unsigned gd_type : 4;          // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;             // must be 0 (system)
    unsigned gd_dpl : 2;           // descriptor(meaning new) privilege level
    unsigned gd_p : 1;             // Present
    unsigned gd_off_31_16 : 16;    // high bits of offset in segment
};
int main()
{  
// 定义结构体变量 
    struct gatedesc intr;
// 直接对结构体变量中的对象赋值 
    intr.gd_off_15_0 = 8;
    intr.gd_ss = 0;
    intr.gd_args = 0;
    intr.gd_rsv1 = 0;
    intr.gd_type = 0;
    intr.gd_s = 0;
    intr.gd_dpl = 0;
    intr.gd_p = 0;
    intr.gd_off_31_16 = 0;
    SETGATE( intr, 0,1,2,3);// 0x02 & 0xffff = 0x0002
    printf( "unsigned intr :%u\r\n",  intr);//无符号 65538
    printf( "xxxx intr :%x\r\n", intr);//　16进制格式 0x10002
    return 0;
}
#endif

//ex2
#if 1
#include "stdlib.h"

struct gatedesc {
    unsigned gd_off_15_0 : 16;        // low 16 bits of offset in segment
    unsigned gd_ss : 16;            // segment selector
    unsigned gd_args : 5;            // # args, 0 for interrupt/trap gates
    unsigned gd_rsv1 : 3;            // reserved(should be zero I guess)
    unsigned gd_type : 4;            // type(STS_{TG,IG32,TG32})
    unsigned gd_s : 1;                // must be 0 (system)
    unsigned gd_dpl : 2;            // descriptor(meaning new) privilege level
    unsigned gd_p : 1;                // Present
    unsigned gd_off_31_16 : 16;        // high bits of offset in segment
};

typedef struct gatedesc gatedesc;
typedef unsigned int uint32_t;

#define STS_IG32    0xE     // 32-bit Interrupt Gate
#define STS_TG32    0xF     // 32-bit Trap Gate
// 结构体对象指针　访问成员 ->访问  
#define SETGATE(gate, istrap, sel, off, dpl) {                        \
    ((gatedesc*)(&gate))->gd_off_15_0 = (uint32_t)(off) & 0xffff;     \
    ((gatedesc*)(&gate))->gd_ss = (sel);                              \
    ((gatedesc*)(&gate))->gd_args = 0;                                \
    ((gatedesc*)(&gate))->gd_rsv1 = 0;                                \
    ((gatedesc*)(&gate))->gd_type = (istrap) ? STS_TG32 : STS_IG32;   \
    ((gatedesc*)(&gate))->gd_s = 0;                                   \
    ((gatedesc*)(&gate))->gd_dpl = (dpl);                             \
    ((gatedesc*)(&gate))->gd_p = 1;                                   \
    ((gatedesc*)(&gate))->gd_off_31_16 = (uint32_t)(off) >> 16;       \
}

int main()
{
	unsigned intr;
	intr=8;
	SETGATE(intr, 0,1,2,3);// 取地址进行修改　会改变内容 
                               // 0x02 & 0xffff = 0x10002
	printf("d intr :%d\r\n", intr);// 65538
	printf("x intr :%x\r\n", intr);// 0x10002
	return 0;
}
#endif
```
# fock() 子进程
```c
/*
 fock复制子进程 execXXX 接力新进程
参考　https://github.com/Ewenwan/ShiYanLou/blob/master/learn_c/Head_first/7_2_execXXX_fock.c
*/
#include <stdio.h>
#include <unistd.h>
void main(void){
  int child_status, exec_status;
  int pid = fork(); //创建一个新的进程
  if(pid == -1) {
     //fprintf(stderr, "不能复制进程：%s", strerror(errno));
     fprintf(stderr, "不能复制进程");
     return; 
  }
  else if (pid==0) 
   { //pid=0为子进程
    printf("Child: EXEC lec7_1\n");
    // execXXX 接力新进程
    // l list 参数列表 ； v vector 参数数组/向量 ；
    // p 路径 path ; e 环境变量 environment 
    exec_status=execve("lec7_1",NULL,NULL);
    printf("Child: Why would I execute?\n");
  } 
  else
   {  // pid =1 是父进程
    printf("Parent: Whose your daddy?\n");//先打印
    child_status=wait(pid);// 等待 子进程结束
    // 上面的子程序结束后　执行后面的
    printf("Parent: the child %d exit with %d\n",pid, child_status);
  }
}
/*
打印输出:
Parent: Whose your daddy?
Child: EXEC lec7_1
Child: Why would I execute?
Parent: the child 15757 exit with -1
*/
```

# 通用链表结构相关的Ｃ编程
```c
// 双向链表 无内容值
struct list_entry {
    struct list_entry *prev, *next;
};
typedef struct list_entry list_entry_t;
//　链表节点+值
struct entry {

    list_entry_t node;//之前前后的节点
    int num;// 值
};
```

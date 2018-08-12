#include <defs.h>
#include <stdio.h>
#include <string.h>
#include <console.h>
#include <kdebug.h>
#include <picirq.h>
#include <trap.h>
#include <clock.h>
#include <intr.h>
#include <pmm.h>
#include <kmonitor.h>
int kern_init(void) __attribute__((noreturn));
void grade_backtrace(void);
static void lab1_switch_test(void);

int
kern_init(void) {
    extern char edata[], end[];
    memset(edata, 0, end - edata);

// 1. 初始化终端
    cons_init();  // driver/conole.c init the console
                  //  包含:
                  //  显示器(Color Graphics Adapter)初始化, cga_init()，6845芯片是IBM PC中的视频控制器；
                  //  串口的初始化,                         serial_init()，com1，波特率等设置，串口中断；
                  //  键盘(keyboard)初始化,                 kbd_init(), 字符映射 和 键盘中断；
// 2. 显示字符串
    const char *message = "(THU.CST) os is loading ... su wyw ==============";
    cprintf("%s\n\n", message);//　欢迎信息
    print_kerninfo();//内核信息
// 3. 显示堆栈中的多层函数调用关系 切换到保护模式，启用分段机制；
    grade_backtrace();// 本文件内
// 4. 初始化物理内存管理
    pmm_init();        // mm/pmm.c  init physical memory management
                           // 这里仅包含 gdt_init()： initialize the default GDT and TSS
                           // 全局描述符表（Global Descriptor Table），GDT表项指向LDT, 记录OS使用的内存区域的段描述符.
                             // 局部描述符表（Local Descriptor Table）, 
                             // 每个进程的局部空间的段的所有段描述符集中存放在一张表中，这张表为该进程的局部描述符表（LDT）。
                             // 当前正在执行的进程，其在GDT中的LDT描述符位置，由CPU的LDTR寄存器指出.
                           // 任务状态段（Task State Segment）,
                             // 每个任务都有一个任务状态段TSS，描述任务状态段的描述符称为TSS描述符（类型 = 1、3、9、 B）.
                             // 所有任务的TSS描述符均被存放在GDT中。
                             // 系统可通过TR(任务)寄存器在GDT中找到正在执行的任务的TSS描述符，从而找到相应任务的TSS。
                             // 任务状态段TSS中包含：
                                // 1. 任务的CPU现场（通用寄存器eax-edx、段选择寄存器dsr、指令指针eip、标志寄存器FR等）；
                                // 2. 特权级分别为0、1、2时的 堆栈段选择符 和 栈顶指针 esp ；
                                // 3. 该任务被调用时，前一个任务TSS 的 返回连接选择符；
                                // 4. I/O允许位图等。

// 5. 初始化中断控制器，设置中断描述符表，初始化时钟中断，使能整个系统的中断机制；
    pic_init();        // driver/picirq.c 8259A中断控制器初始化 init interrupt controller
    idt_init();        // trap/trap.c  中断描述符表初始化 init interrupt descriptor table
                          // 各类中断的中断处理程序的相关信息，是通过一个控制段描述符来描述的，
                          // 该描述符叫中断门，其描述符类型为E.
                          // 所有中断门集中保存在一张表中，该表即中断描述符表IDT，其作用类似于实模式下的中断向量表IVT。
                          // CPU寄存器IDTR中存放有IDT的限长和物理基址，从而可以找到系统使用的IDT。
    clock_init();      // driver/clock.c 时钟中断初始化 init clock interrupt
                          // 8253 定时器，设置1秒中断100次
                          // 定时器中断使能
    intr_enable();     // driver/intr.c 使能整个系统的中断机制 enable irq interrupt
                          // sti() ----> x86.h : asm volatile ("sti");
                          // STI(Set Interrupt) 中断标志置1指令 使 IF = 1
                          // CLI(Clear Interrupt) 中断标志置0指令 使 IF = 0

    //LAB1: CAHLLENGE 1 If you try to do it, uncomment lab1_switch_test()
    // user/kernel mode switch test
    lab1_switch_test();
// 执6. 行while（1）死循环
    /* do nothing */
    while (1);
}

void __attribute__((noinline))
grade_backtrace2(int arg0, int arg1, int arg2, int arg3) {
    mon_backtrace(0, NULL, NULL);
}

void __attribute__((noinline))
grade_backtrace1(int arg0, int arg1) {
    grade_backtrace2(arg0, (int)&arg0, arg1, (int)&arg1);
}

void __attribute__((noinline))
grade_backtrace0(int arg0, int arg1, int arg2) {
    grade_backtrace1(arg0, arg2);
}

void
grade_backtrace(void) {
    grade_backtrace0(0, (int)kern_init, 0xffff0000);
}

// 打印当前系统用户属性：用户态/内核态
static void
lab1_print_cur_status(void) {
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;
    asm volatile (
            "mov %%cs, %0;"
            "mov %%ds, %1;"
            "mov %%es, %2;"
            "mov %%ss, %3;"
            : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));// 输出，=m 输出寄存器

    cprintf("%d: @ring %d\n", round, reg1 & 3);
    cprintf("%d:  cs = %x\n", round, reg1);
    cprintf("%d:  ds = %x\n", round, reg2);
    cprintf("%d:  es = %x\n", round, reg3);
    cprintf("%d:  ss = %x\n", round, reg4);
    round ++;
}

// 我们首先要知道内核态和用户态指的究竟是什么。
// 它们即ring0和ring3，指的是当前代码运行的特权等级。这个特权级由cs中标识特权级的几位声明。
// 访问数据段时，数据段的特权级（DPL）必须大于等于代码段的特权级（CPL），
// 同时还有一个东西叫做i/o特权级（IOPL），其作用方式同上。
static void
lab1_switch_to_user(void) {
    //LAB1 CHALLENGE 1 : TODO
asm volatile (
   "sub $0x8, %%esp \n"  // 保存内核态 栈指针
   "int %0 \n"           // 调用 T_SWITCH_TOU 中断  在 trap.c 内 需要修改
   "movl %%ebp, %%esp"   // 恢复栈指针
   :
   : "i"(T_SWITCH_TOU)   // 最后作为参数传送给trap函数, 转到用户态 .
);
}

static void
lab1_switch_to_kernel(void) {
    //LAB1 CHALLENGE 1 :  TODO
  	asm volatile (
	    "int %0 \n"           // 保存内核态 栈指针
	    "movl %%ebp, %%esp \n"// 恢复栈指针
	    : 
	    : "i"(T_SWITCH_TOK)   // 最后作为参数传送给trap函数, 转到内核态 .
	);
}
// 系统用户等级 切换 测试
static void
lab1_switch_test(void) {
    lab1_print_cur_status();
    cprintf("+++ switch to  user  mode +++\n");
    lab1_switch_to_user();
    lab1_print_cur_status();
    cprintf("+++ switch to kernel mode +++\n");
    lab1_switch_to_kernel();
    lab1_print_cur_status();
}

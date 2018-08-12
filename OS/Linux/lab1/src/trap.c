// 中断 异常 陷阱 
#include <defs.h>
#include <mmu.h>//　内存管理单元
#include <memlayout.h>//内存布局
#include <clock.h>//时钟
#include <trap.h>//中断
#include <x86.h>
#include <stdio.h>
#include <assert.h>//　确保 
#include <console.h>
#include <kdebug.h>// 函数堆栈调试信息

#define TICK_NUM 100 //　每隔多长时间打印时间信息

// 打印 时钟信息
static void print_ticks() {
    cprintf("hello linux, times %d ticks\n",TICK_NUM);
#ifdef DEBUG_GRADE
    cprintf("End of Test.\n");
    panic("EOT: kernel seems ok.");
#endif
}

/* *
 * Interrupt descriptor table:
 *
 * Must be built at run time because shifted function addresses can't
 * be represented in relocation records.
 * 中端描述符表 , 内容指向中断服务例程
 * 中断（异常）号乘以8做为 IDT的索引
 * */
static struct gatedesc idt[256] = {{0}};

static struct pseudodesc idt_pd = {
    sizeof(idt) - 1, (uintptr_t)idt
};

/* idt_init - initialize IDT to each of the entry points in kern/trap/vectors.S */
void
idt_init(void) {
     /* LAB1 YOUR CODE : STEP 2 */
     /* (1) Where are the entry addrs of each Interrupt Service Routine (ISR)?
      *     All ISR's entry addrs are stored in __vectors. where is uintptr_t __vectors[] ?
      *     __vectors[] is in kern/trap/vector.S which is produced by tools/vector.c
      *     (try "make" command in lab1, then you will find vector.S in kern/trap DIR)
      *     You can use  "extern uintptr_t __vectors[];" to define this extern variable which will be used later.
      * (2) Now you should setup the entries of ISR in Interrupt Description Table (IDT).
      *     Can you see idt[256] in this file? Yes, it's IDT! you can use SETGATE macro to setup each item of IDT
      * (3) After setup the contents of IDT, you will let CPU know where is the IDT by using 'lidt' instruction.
      *     You don't know the meaning of this instruction? just google it! and check the libs/x86.h to know more.
      *     Notice: the argument of lidt is idt_pd. try to find it!
      */
// 1. 声明__vectors[] 来对应中断描述符表中的256个中断符  tools/vector.c中
    extern uintptr_t __vectors[];// 代码段偏移量
// 2. 通过for循环运用SETGATE宏定义函数(类似c++ inline内连函数)  进行 中断门idt[i] 的初始化
    // 在kernel/mm/mmu.h中　#define SETGATE(gate, istrap, sel, off, dpl) {}
    int i;
    for(i = 0; i<sizeof(idt)/sizeof(struct gatedesc); i++)
    {// 0 中断门 1 陷阱门  G
     // D_KTEXT 内核　代码段起始地址 在kernel/mm/memlayout.h中
     // __vectors[i]　偏移地址
     // DPL_KERNEL 内核权限  DPL_USER 用户权限  在kernel/mm/memlayout.h中
      SETGATE(idt[i], 0, GD_KTEXT, __vectors[i], DPL_KERNEL);
    }
// 需要对 用户态 转 内核态 的中断表进行初始化了，
// 和上面的不同之处只是在于特权值的不同，所以我们的操作如下：
// T_SWITCH_TOK 121  trap.h 中
   SETGATE(idt[T_SWITCH_TOK], 0, GD_KTEXT, __vectors[T_SWITCH_TOK], DPL_USER);

// 3.　最后加载idt中断描述符表　 libs/x86.h
// 将　&idt_pd 首地址 加载到 中断描述符表寄存器 (IDTR)
    lidt(&idt_pd);

}

static const char *
trapname(int trapno) {
    static const char * const excnames[] = {
        "Divide error",
        "Debug",
        "Non-Maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "BOUND Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection",
        "Page Fault",
        "(unknown trap)",
        "x87 FPU Floating-Point Error",
        "Alignment Check",
        "Machine-Check",
        "SIMD Floating-Point Exception"
    };

    if (trapno < sizeof(excnames)/sizeof(const char * const)) {
        return excnames[trapno];
    }
    if (trapno >= IRQ_OFFSET && trapno < IRQ_OFFSET + 16) {
        return "Hardware Interrupt";
    }
    return "(unknown trap)";
}

/* trap_in_kernel - test if trap happened in kernel */
bool
trap_in_kernel(struct trapframe *tf) {
    return (tf->tf_cs == (uint16_t)KERNEL_CS);
}

static const char *IA32flags[] = {
    "CF", NULL, "PF", NULL, "AF", NULL, "ZF", "SF",
    "TF", "IF", "DF", "OF", NULL, NULL, "NT", NULL,
    "RF", "VM", "AC", "VIF", "VIP", "ID", NULL, NULL,
};

void
print_trapframe(struct trapframe *tf) {
    cprintf("trapframe at %p\n", tf);
    print_regs(&tf->tf_regs);
    cprintf("  ds   0x----%04x\n", tf->tf_ds);
    cprintf("  es   0x----%04x\n", tf->tf_es);
    cprintf("  fs   0x----%04x\n", tf->tf_fs);
    cprintf("  gs   0x----%04x\n", tf->tf_gs);
    cprintf("  trap 0x%08x %s\n", tf->tf_trapno, trapname(tf->tf_trapno));
    cprintf("  err  0x%08x\n", tf->tf_err);
    cprintf("  eip  0x%08x\n", tf->tf_eip);
    cprintf("  cs   0x----%04x\n", tf->tf_cs);
    cprintf("  flag 0x%08x ", tf->tf_eflags);

    int i, j;
    for (i = 0, j = 1; i < sizeof(IA32flags) / sizeof(IA32flags[0]); i ++, j <<= 1) {
        if ((tf->tf_eflags & j) && IA32flags[i] != NULL) {
            cprintf("%s,", IA32flags[i]);
        }
    }
    cprintf("IOPL=%d\n", (tf->tf_eflags & FL_IOPL_MASK) >> 12);

    if (!trap_in_kernel(tf)) {
        cprintf("  esp  0x%08x\n", tf->tf_esp);
        cprintf("  ss   0x----%04x\n", tf->tf_ss);
    }
}

void
print_regs(struct pushregs *regs) {
    cprintf("  edi  0x%08x\n", regs->reg_edi);
    cprintf("  esi  0x%08x\n", regs->reg_esi);
    cprintf("  ebp  0x%08x\n", regs->reg_ebp);
    cprintf("  oesp 0x%08x\n", regs->reg_oesp);
    cprintf("  ebx  0x%08x\n", regs->reg_ebx);
    cprintf("  edx  0x%08x\n", regs->reg_edx);
    cprintf("  ecx  0x%08x\n", regs->reg_ecx);
    cprintf("  eax  0x%08x\n", regs->reg_eax);
}

static void
lab1_print_cur_status(void) {
    static int round = 0;
    uint16_t reg1, reg2, reg3, reg4;
    asm volatile (
            "mov %%cs, %0;"
            "mov %%ds, %1;"
            "mov %%es, %2;"
            "mov %%ss, %3;"
            : "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));// =m  结合上面的mov语句，输出寄存器的值
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
   : "i"(T_SWITCH_TOU)   //最后作为参数传送给trap函数, 转到用户态 .
);
}

static void
lab1_switch_to_kernel(void) {
    //LAB1 CHALLENGE 1 :  TODO
  	asm volatile (
	    "int %0 \n"  // 保存内核态 栈指针
	    "movl %%ebp, %%esp \n"// 恢复栈指针
	    : 
	    : "i"(T_SWITCH_TOK) //最后作为参数传送给trap函数, 转到内核态 .
	);
}


/* temporary trapframe or pointer to trapframe */
struct trapframe switchk2u, *switchu2k;

/* trap_dispatch - dispatch based on what type of trap occurred */
// 各种中断响应处理
static void
trap_dispatch(struct trapframe *tf) {
    char c;

    switch (tf->tf_trapno) {
    case IRQ_OFFSET + IRQ_TIMER:
        /* LAB1 YOUR CODE : STEP 3 */
        /* handle the timer interrupt */
        /* (1) After a timer interrupt, you should record this event using a global variable (increase it), such as ticks in kern/driver/clock.c
         * (2) Every TICK_NUM cycle, you can print some info using a funciton, such as print_ticks().
         * (3) Too Simple? Yes, I think so!
         */
        // 1.记录时钟中断
        ticks++; // 定义在 kern/driver/clock.c 中
        // 2. 判断 ticks的状态, 执行相应的操作
        if(ticks% TICK_NUM == 0)// TICK_NUM 本文件最上面　为100 
         {// 每当ticks计数达到100时，即出发了100次时钟中断后（1s），时钟中断会print“100 ticks”。
           print_ticks();//向终端打印时间信息  
         }

        break;
    case IRQ_OFFSET + IRQ_COM1://　串口1 中断
        c = cons_getc();
        cprintf("serial [%03d] %c\n", c, c);
        break;
    case IRQ_OFFSET + IRQ_KBD://键盘中断
        c = cons_getc();
        cprintf("kbd [%03d] %c\n", c, c);
        cprintf("keyboard interrupt\n");
	if(c == 'k') {
	   cprintf("+++ switch to  kernel  mode +++\n");
	  lab1_switch_to_kernel();
          lab1_print_cur_status();
	}
	else if(c == 'u'){
	      cprintf("+++ switch to  user  mode +++\n");
	      lab1_switch_to_user();
              lab1_print_cur_status();
	     cprintf("+++ switch to  kernel  mode +++\n");
	     lab1_switch_to_kernel();
             lab1_print_cur_status();
	}
        break;
    //LAB1 CHALLENGE 1 : YOUR CODE you should modify below codes.
    case T_SWITCH_TOU://用户切换 用户到内核
       //  panic("T_SWITCH_ to user\n");
      // Panic()函数（实际上是User::Panic()）是当系统发现无法继续运行下去的故障时将调用它，会导致程序中止，然后由系统显示错误号。 
       // 在 trap_dispatch 当中对应部分加入:
	if (tf->tf_cs != USER_CS){// 如果是非用户态 则 切换访问级别
// 从内核态切换到用户态只能通过iret指令修改cs的值, 这个特权级由cs中标识特权级的几位声明
	  switchk2u = *tf;
	  /*
	   tf->tf_cs = USER_CS;
	   tf->tf_ss = USER_DS;
	   tf->tf_ds = USER_DS;
	   tf->tf_es = USER_DS;
	  // tf->tf_fs = USER_DS;
	  // tf->tf_gs = USER_DS;
	   
	   tf->tf_esp = (uint32_t)tf + sizeof(struct trapframe) - 8;
	   
	   tf->tf_eflags |= FL_IOPL_MASK;  // i/o特权级（IOPL）
	   
	   *((uint32_t *)tf - 1) = (uint32_t)tf;//设置临时堆栈？？
	   */
            switchk2u.tf_cs = USER_CS;
            switchk2u.tf_ds = switchk2u.tf_es = switchk2u.tf_ss = USER_DS;
            switchk2u.tf_esp = (uint32_t)tf + sizeof(struct trapframe) - 8;
		
            // set eflags, make sure ucore can use io under user mode.
            // if CPL > IOPL, then cpu will generate a general protection.
            switchk2u.tf_eflags |= FL_IOPL_MASK;
		
            // set temporary stack
            // then iret will jump to the right stack
            *((uint32_t *)tf - 1) = (uint32_t)&switchk2u;
	}
        break;

    case T_SWITCH_TOK://切换到内核态
       // panic("T_SWITCH_** ??\n");
	if (tf->tf_cs != KERNEL_CS) {//如果当前不是内核态
            tf->tf_cs = KERNEL_CS;
            tf->tf_ds = tf->tf_es = KERNEL_DS;
            tf->tf_eflags &= ~FL_IOPL_MASK;
            switchu2k = (struct trapframe *)(tf->tf_esp - (sizeof(struct trapframe) - 8));
            memmove(switchu2k, tf, sizeof(struct trapframe) - 8);
            *((uint32_t *)tf - 1) = (uint32_t)switchu2k;
        }
        break;
    case IRQ_OFFSET + IRQ_IDE1:
    case IRQ_OFFSET + IRQ_IDE2:
        /* do nothing */
        break;
    default:
        // in kernel, it must be a mistake
        if ((tf->tf_cs & 3) == 0) {
            print_trapframe(tf);
            panic("unexpected trap in kernel.\n");
        }
    }
}

/* *
 * trap - handles or dispatches an exception/interrupt. if and when trap() returns,
 * the code in kern/trap/trapentry.S restores the old CPU state saved in the
 * trapframe and then uses the iret instruction to return from the exception.
 * */
void
trap(struct trapframe *tf) {
    // dispatch based on what type of trap occurred
    trap_dispatch(tf);
}

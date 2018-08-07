# 重要文件
# 调用顺序
```c
  1. boot/bootasm.S  | bootasm.asm（修改了名字，以便于彩色显示）
     a. 开启A20   16位地址线 实现 20位地址访问  芯片版本兼容
        通过写 键盘控制器8042  的 64h端口 与 60h端口。
        
     b. 加载GDT全局描述符 lgdt gdtdesc
     c. 使能和进入保护模式 置位 cr0寄存器的 PE位 (内存分段访问) PE+PG（分页机制）
        movl %cr0, %eax 
        orl $CR0_PE_ON, %eax  或操作，置位 PE位 
        movl %eax, %cr0
     d. 调用载入系统的函数 call bootmain  # 转而调用 bootmain.c 
     
  2. boot/bootmain.c -> bootmain 函数
     a. 调用readseg函数从ELFHDR处读取8个扇区大小的 os 数据。
     b. 将输入读入 到 内存中以 进程(程序)块 proghdr 的方式存储
     c. 跳到ucore操作系统在内存中的入口位置（kern/init.c中的kern_init函数的起始地址）
     
  3. kern/init.c
     a. 初始化终端 cons_init(); init the console   kernel/driver/consore.c
         显示器初始化       cga_init();    
         串口初始化         serial_init(); 
         keyboard键盘初始化 kbd_init();
         
     b. 打印内核信息 & 欢迎信息 
        print_kerninfo();          //  内核信息  kernel/debug/kdebug.c
        cprintf("%s\n\n", message);//　欢迎信息 const char *message = “qwert”
     
     c. 显示堆栈中的多层函数调用关系 切换到保护模式，启用分段机制
        grade_backtrace();
        
     d. 初始化物理内存管理
        pmm_init();        // init physical memory management   kernel/mm/ppm.c
        --->gdt_init();    // 初始化默认的全局描述符表
        
     e. 初始化中断控制器，
        pic_init();        // 初始化 8259A 中断控制器   kernel/driver/picirq.c
        
     f. 设置中断描述符表
        idt_init();        // kernel/trap/trap.c 
        // __vectors[] 来对应中断描述符表中的256个中断符  tools/vector.c中
        
     g. 初始化时钟中断，使能整个系统的中断机制  8253定时器 
        clock_init();      // 10ms 时钟中断(1s中断100次)   kernel/driver/clock.c
        ----> pic_enable(IRQ_TIMER);// 使能定时器中断 
        
     h. 使能整个系统的中断机制 enable irq interrupt
        intr_enable();     // kernel/driver/intr.c
        // sti();          // set interrupt // x86.h
        
     i. trap中断(陷阱)处理函数
        trap() ---> trap_dispatch()   // kernel/trap/trap.c 
        
        1. 10ms 时钟中断处理 case IRQ_TIMER：
           if((ticks++)%100==0) print_ticks();//向终端打印时间信息（1s打印一次）
           
        2. 串口1 中断    case IRQ_COM1: 
           获取串口字符后打印
           
        3. 键盘中断      case IRQ_KBD: 
           获取键盘字符后打印
           
        4. 用户切换中断
```    
           

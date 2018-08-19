# lab2 系统内存的探测
[博客](https://segmentfault.com/a/1190000009450840)

    主要涉及操作系统的物理内存管理。
    操作系统为了使用内存，还需高效地管理内存资源。
    这里我们会了解并且自己动手完成一个简单的物理内存管理系统。

# 实验目的
    理解基于段页式内存地址的转换机制
    理解页表的建立和使用方法
    理解物理内存的管理方法

# 实验内容 
    1. 首先了解如何发现系统中的物理内存；
    2. 然后了解如何建立对物理内存的初步管理，即了解连续物理内存管理；
    3. 最后了解页表相关的操作，即如何建立页表来实现虚拟内存到物理内存之间的映射，对段页式内存管理机制有一个比较全面的了解。
    本实验里面实现的内存管理还是非常基本的，并没有涉及到对实际机器的优化，比如针对 cache(缓冲区)的优化等。
    如果大家有余力，尝试完成扩展练习。
![](https://img-blog.csdn.net/20170903193711721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZm91eV95dW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 程序执行顺序
    1. boot/bootasm.S  | bootasm.asm（修改了名字，以便于彩色显示）
     a. 开启A20   16位地址线 实现 20位地址访问  芯片版本兼容
        通过写 键盘控制器8042  的 64h端口 与 60h端口。
         
     ab.物理内存探测  通过 BIOS 中断获取内存布局 ==========比lab1多的部分===========
     
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
        ---> page_init();// 内存管理等函数  ===============比lab1多的部分=================

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

     i. lab1_switch_test();// 用户切换函数 会 触发中断用户切换中断

    4. kernel/trap/trap.c 
       trap中断(陷阱)处理函数
        trap() ---> trap_dispatch()   // kernel/trap/trap.c 

        a. 10ms 时钟中断处理 case IRQ_TIMER：
           if((ticks++)%100==0) print_ticks();//向终端打印时间信息（1s打印一次）

        b. 串口1 中断    case IRQ_COM1: 
           获取串口字符后打印

        c. 键盘中断      case IRQ_KBD: 
           获取键盘字符后打印

        d. 用户切换中断
# 新添加的代码分析

    # 机器启动后有2种方式探测物理内存：直接探测、通过BIOS中断探测。 
    # 这里我们关注如何通过BIOS的0x15中断探测物理内存。
    # 由于BIOS中断需要在实模式下调用，所以我们在bootloader中探测物理内存比较合适(切换至 保护模式 之前)。
    ## BIOS的0x15中断有3个子功能：e820h、e801h、88h。这三个子功能区别是：
    ### e820h返回内存布局，信息量大，这是功能最强大的子功能，使用也最复杂。
    ### e801h返回内存容量
    ### 88h最简单，功能也最弱
    ### e820将物理内存探测的结果以 地址范围描述符 的格式放在内存中。
    # 地址范围描述符共计20字节，格式是：
    #     其具体表示如下：
    #       Offset      Size    Description
    #        00h 0~7    8字节   base address               # 内存块基地址
    #        08h 8~15   8字节   length in bytes            # 这块内存的大小
    #        10h 16~20  4字节   type of address range      # 这块内存的类型（共有4种类型）
    # 
    
## 通过中断获取的一个一个内存块的信息会存入一个缓冲区中 e820map结构体
    
```c
    /* memlayout.h */
    #define E820MAX 20
    struct e820map {
    int nr_map; // 表示数组元素个数，该字段是为了方便后续OS，不是BIOS访问的
    struct {
        long long addr;
        long long size;
        long type;
    } map[E820MAX];
    };
```
# e820h的调用参数
    显然在保护模式下用int $0x15来调用15h中断。
    
    但在这之前我们要将参数放置在寄存器中：
    
        eax：子功能编号，这里我们填入0xe820
        edx：534D4150h(ascii字符”SMAP”)，签名，约定填”SMAP”
        ebx：每调用一次int $0x15，ebx会加1。当ebx为0表示所有内存块检测完毕。（重要！看后面的案例会明白如何使用）
        ecx：存放地址范围描述符的内存大小，至少要设置为20。
        es:di：告诉BIOS要把地址描述符写到这个地址。
        
    中断的返回值如下：
    
        CF标志位：若中断执行失败，则置为1。
        eax：     值是534D4150h(“SMAP”)
        es:di：   中断不改变该值，值与参数传入的值一致
        ebx：     下一个中断描述符的计数值（见后面的案例）
        ecx：     返回BIOS写到cs:di处的地址描述符的大小（应该就是20吧？）
        ah：      若发生错误，表示错误码

## boot/bootasm.S ab.物理内存探测  通过 BIOS 中断获取内存布局 ==========比lab1多的部分===========
```asm
 .set SMAP,                  0x534d4150    # 设置变量(即4个ASCII字符“SMAP”)
 
# 第一步： 设置一个存放内存映射地址描述符的物理地址(这里是0x8000)
probe_memory:
    # 约定在bootloader中将内存探测结果放到0x8000地址处。
    # 在0x8000处存放struct e820map, 并清除 e820map 中的 nr_map 记录了内存块的个数
    movl $0, 0x8000 # 对0x8000处的32位单元清零,即给位于0x8000处的struct e820map的成员变量 nr_map 清零
    xorl %ebx, %ebx # 清理 %ebx, 异或,相同为0，不同为1
    
    # 0x8004 处将用于存放第一个内存映射地址描述符 
    movw $0x8004, %di # 表示设置调用INT 15h BIOS中断后，BIOS返回的映射地址描述符的起始地址
                      # 中断前需要传递的参数， es:di：告诉BIOS要把地址描述符写到这个地址。
                      
# 第二步： 将e820作为参数传递给INT 15h中断
start_probe:
  # 传入0xe820 作为INT 15h中断的参数 
    movl $0xE820, %eax  #  INT 15的中断调用参数 eax：子功能编号，这里我们填入0xe820
  # 内存映射地址描述符的大小 
    movl $20, %ecx      # 设置地址范围描述符的大小为20字节，其大小等于struct e820map的成员变量map的大小
                        # 存放地址范围描述符的内存大小，至少要设置为20
    movl $SMAP, %edx    # 设置edx为534D4150h (即4个ASCII字符“SMAP”)，这是一个约定
  # 调用INT 15h中断 
    int $0x15 # 中断参数0xe820，要求BIOS返回一个用地址范围描述符表示的内存段信息
    
# 通过检测 eflags 的CF位来判断探测是否结束。
# 如果没有结束， 设置存放下一个内存映射地址描述符的物理地址，然后跳到步骤2；如果结束，则程序结束
    # 如果eflags的CF位为0，则表示还有内存段需要探测 
    jnc cont # 如果发生错误，CF位为1。那么可以尝试使用其它子功能进行探测，或者就直接关机（连内存容量都没探测肯定无法启动OS了）
    movw $12345, 0x8000 # 在ucore中表示出错，与BIOS无关
    jmp finish_probe
cont:
  # 继续探测 设置下一个内存映射地址描述符的起始地址 
    addw $20, %di   # 设置下一个BIOS返回的映射地址描述符的起始地址
                    # 控制BIOS该将“地址描述符”写到哪里
  # e820map中的nr_map加1 
    incl 0x8000     # 递增struct e820map的成员变量nr_map # nr_map成员自增1，该变量与BIOS无关
  # 如果还有内存段需要探测则继续探测, 否则结束探测 
    cmpl $0, %ebx   # 每调用一次int $0x15，ebx会加1。当ebx为0表示所有内存块检测完毕。
    jnz start_probe
finish_probe:
``` 
# 程序流程图
![](https://github.com/Ewenwan/ShiYanLou/blob/master/OS/Linux/lab2/BIOS_INT0x15_mem_layout.png)

# 练习1：实现 first-fit 连续物理内存分配算法（需要编程）
    在实现first fit 内存分配算法的回收函数时，要考虑地址连续的空闲块之间的合并操作。
    提示:在建立空闲页块链表时，需要按照空闲页块起始地址来排序，形成一个有序的链表。
    可能会修改default_pmm.c 中的 default_init，default_init_memmap，default_alloc_pages， default_free_pages等相关函数。
    请仔细查看和理解default_pmm.c中的注释。
    
## 软件了解硬件实际物理内存(物理内存如何分布的，哪些可用，哪些不可用)

    探测物理内存分布和大小的方法, 基本方法是通过 BIOS 中断调用 来帮助完成的。
    
    其中BIOS中断调用必须在实模式下进行，所以在bootloader进入保护模式前完成这部分工作相对比较合适。
    
    所以需要修改bootloader软件。这些部分由boot/bootasm.S 中从 probe_memory 处到 finish_probe 处的代码部分完成。
    
    通过BIOS中断获取内存可调用参数为e820h的INT 15h BIOS中断。
    
    BIOS通过 系统内存映射地址描述符（Address Range Descriptor）格式来 表示 系统物理 内存布局。
    
    其具体表示如下：
        Offset  Size    Description
        00h     8字节   base address               # 系统内存块基地址
        08h     8字节   length in bytes            # 系统内存大小
        10h     4字节   type of address range      # 内存类型
    
    看下面的 系统内存映射地址类型
        Values for System Memory Map address type:
        type = 1 01h    memory, available to OS，是可以使用的物理内存空间
        type = 2 02h    reserved, not available (e.g. system ROM, memory-mapped device)
                        保留，不能使用的物理内存空间，
                        这些地址不能映射到物理内存上， 但它们可以映射到ROM或者映射到其他设备，比如各种外设等。
        type = 3 03h    ACPI Reclaim Memory (usable by OS after reading ACPI tables)
        type = 4 04h    ACPI NVS Memory (OS is required to save this memory between NVS sessions)
                 other  not defined yet -- treat as Reserved
    
    INT15h BIOS中断的详细调用参数:
        eax：e820h：INT 15的中断调用参数；
        edx：534D4150h (即4个ASCII字符“SMAP”) ，这只是一个签名而已；
        ebx：如果是第一次调用或内存区域扫描完毕，则为0。 如果不是，则存放上次调用之后的计数值；
        ecx：保存地址范围描述符的内存大小,应该大于等于20字节；
        es:di：指向保存地址范围描述符结构的缓冲区，BIOS把信息写入这个结构的起始地址。

    此中断的返回值为:
        eflags 的CF位：若INT 15中断执行成功，则不置位，否则置位；
        eax：  534D4150h ('SMAP') ；
        es:di：指向保存地址范围描述符的缓冲区,此时缓冲区内的数据已由BIOS填写完毕
        ebx：  下一个地址范围描述符的计数地址
        ecx：  返回BIOS往ES:DI处写的地址范围描述符的字节大小
        ah：   失败时保存出错代码
        
    这样，我们通过调用INT 15h BIOS中断，递增di的值（20的倍数），
    让BIOS帮我们查找出一个一个的内存布局 entry，
    并放入到一个保存地址范围描述符结构的 缓冲区 中，
    供后续的ucore进一步进行物理内存管理。
    
    这个缓冲区结构定义在memlayout.h中：
    
```c
    /* memlayout.h */
    #define E820MAX 20
    struct e820map {
    int nr_map; // 表示数组元素个数，该字段是为了方便后续OS，不是BIOS访问的
    struct {
        long long addr;
        long long size;
        long type;
    } map[E820MAX];
    };
```
    
    bootasm.S 需要做的修改，再使能A20之后，线进行物理内存的探测保存一些信息后，再加载全局描述符表后进入保护模式。
    
```asm
# bootasm.S
# 2. 物理内存探测  通过 BIOS 中断获取内存布局
# 第一步： 设置一个存放内存映射地址描述符的物理地址(这里是0x8000)
probe_memory:
    # 在0x8000处存放struct e820map, 并清除e820map中的nr_map 
    movl $0, 0x8000 # 对0x8000处的32位单元清零,即给位于0x8000处的struct e820map的成员变量nr_map清零
    xorl %ebx, %ebx
    # 0x8004处将用于存放第一个内存映射地址描述符 
    movw $0x8004, %di # 表示设置调用INT 15h BIOS中断后，BIOS返回的映射地址描述符的起始地址
# 第二步： 将e820作为参数传递给INT 15h中断
start_probe:
    # 传入0xe820作为INT 15h中断的参数 
    movl $0xE820, %eax  #  INT 15的中断调用参数
    # 内存映射地址描述符的大小 
    movl $20, %ecx   # 设置地址范围描述符的大小为20字节，其大小等于struct e820map的成员变量map的大小
    movl $SMAP, %edx # 设置edx为534D4150h (即4个ASCII字符“SMAP”)，这是一个约定
    # 调用INT 15h中断 
    int $0x15 # 要求BIOS返回一个用地址范围描述符表示的内存段信息
# 通过检测eflags的CF位来判断探测是否结束。
# 如果没有结束， 设置存放下一个内存映射地址描述符的物理地址，然后跳到步骤2；如果结束，则程序结束
    # 如果eflags的CF位为0，则表示还有内存段需要探测 
    jnc cont
    movw $12345, 0x8000
    jmp finish_probe
cont:
    # 继续探测 设置下一个内存映射地址描述符的起始地址 
    addw $20, %di   # 设置下一个BIOS返回的映射地址描述符的起始地址
    # e820map中的nr_map加1 
    incl 0x8000 # 递增struct e820map的成员变量nr_map
    # 如果还有内存段需要探测则继续探测, 否则结束探测 
    cmpl $0, %ebx
    jnz start_probe
finish_probe:
```
    
    上述代码正常执行完毕后，
    在0x8000地址处保存了从BIOS中获得的内存分布信息，此信息按照struct e820map的设置来进行填充。
    这部分信息将在bootloader启动ucore后，
    由ucore的 page_init函数 来根据 struct e820map的memmap（定义了起始地址为0x8000）
    来完成对整个机器中的物理内存的总体管理。
    
## 物理内存空间管理的初始化  mm/pmmc --->  pmm_init();
    当我们在bootloader 中完成对物理内存空间的 探测后（知己知彼，百战不殆）， 
    我们就可以根据得到的信息来对可用的内存空间进行管理。
    
    在ucore中， 我们将物理内存空间按照 页page 的大小(4KB，  PGSIZE =4 kb)进行管理， 
    页的信息用Page这个结构体来保存。
    对整个计算机的每一个物理页的属性用结构Page.
    Page的定义在kern/mm/memlayout.h中。以页为单位的物理内存分配管理的实现在kern/default_pmm.[ch].
```c
// 每一个物理页的属性结构===============
struct Page { // kern/mm/memlayout.h
    int ref;        // 映射此 物理页的 虚拟页个数，表示该 物理页 被 页表 的引用记数 page frame's reference counter
                    // 一旦某页表中有一个页表项设置了虚拟页到这个Page管理的物理页的映射关系，就会把Page的ref加一。
                    // 反之，若是解除，那就减一。
    uint32_t flags; // 描述 物理页 属性的标志flags数组， array of flags that describe the status of the page frame
      // 表示此物理页的状态标记，有两个标志位，第一个表示是否被保留，如果被保留了则设为1（比如内核代码占用的空间）。
      // 第二个表示此页是否是free的。
      // 如果设置为1，表示这页是free的，可以被分配；
      // 如果设置为0，表示这页已经被分配出去了，不能被再二次分配。
      
    unsigned int property; //  the num of free block, used in first fit pm manager
                           // 用来记录某连续内存空闲块的大小，
                           // 这里需要注意的是用到此成员变量的这个Page一定是连续内存块的开始地址（第一页的地址）。
                           
    list_entry_t page_link;// 双向链接 各个Page结构的 page_link 双向链表  free list link
    // list_entry_t 是便于把多个连续内存空闲块链接在一起的 双向链表指针，
    // 连续内存空闲块利用这个页的 成员变量 page_link 来链接比它地址小和大的其他连续内存空闲块.
};

// 然后是下面这个结构。
// 一个双向链表，负责管理所有的 连续内存 空闲块，便于分配和释放==============
typedef struct {// kern/mm/memlayout.h
    list_entry_t free_list;         //  是一个list_entry结构的双向链表指针 the list header
    unsigned int nr_free;           //  记录当前空闲页的个数, of free pages in this free list
} free_area_t;

```
## 物理内存空间管理的初始化的过程 mm/pmmc --->  pmm_init() ---> page_init() 
物理内存空间的初始化可以分为以下4步：

    1. 根据物理内存空间探测的结果， 找到最后一个可用空间的结束地址(或者Kernel的结束地址，选一个小的)，
       根据这个 结束地址 计算出整个 可用的物理内存 空间一共有多少个页 npage。
       
    2. 找到Kernel的结束地址(end)，这个地址是在kernel.ld 中定义的， 
       我们从这个地址所在的 下一个页开始(pages = roundup(end/PGSIZE) )写入系统页的信息(将所有的Page写入这个地址，信息页)。
       
    3. 从pages开始，将所有页的信息的flag 都设置为reserved(不可用)。
    
    4. 找到free页的开始地址， 并初始化所有free页的信息(free页就是除了kernel 和 页信息外的可用空间，
       初始化的过程会reset flag中的 reserved位)。
       
     上面这几部中提到了很多地址空间， 下面我用一幅图来说明：   
![](https://sfault-image.b0.upaiyun.com/295/946/295946239-592131eae426e_articlex)
 
    end   指的就是 BSS 的结束处；
    pages 指的是 BSS结束处 - 空闲内存空间的起始地址；
    free 页是从 空闲内存空间的起始地址 - 实际物理内存空间结束地址。

    有了这幅图，这些地址就很容易理解了。 

    物理页数 数量：
        我们首先根据 bootloader 给出的 内存布局信息 找出 最大的物理内存地址 maxpa
        （定义在 page_init() (函数中的局部变量，maxpa = end;），
        
        由于x86 的 起始物理内存地址为0，
        每个页的大小为 PGSIZE = 4KB， 
        所以可以得知需要管理的物理页个数为：
        npage = maxpa / PGSIZE
        
    每个物理也都需要一个 结构体 Page 来进行记录，
    所以内存管理结构体 Page 需要的内存数量：
        这样，我们就可以预估出 管理 页级 物理内存空间 所需的 Page结构 的 内存空间 所需的 内存大小为：
        
        sizeof(struct Page) * npage

    内存管理处 的 起始管理结构体page：
        由于 bootloader 加载 ucore的 结束地址（用 全局指针变量end记录）以上的空间 没有被使用，
        所以我们可以把end 按页大小为 边界取整 后，作为 管理页级物理内存空间所需的Page结构的内存空间，记为：
     
        pages = (struct Page *)ROUNDUP((void *)end, PGSIZE);//  除去操作系统后的需要管理的内存的起始页地址
        
    空闲物理空间起始地址 free：
        内存管理处的起始地址page + 内存管理所需的内存数量，
        之后为空闲地址。
        uintptr_t freemem = PADDR((uintptr_t)pages + sizeof(struct Page) * npage);

    标记内存管理page：
        for (i = 0; i < npage; i ++) {
             SetPageReserved(pages + i);
      // 只需把物理地址对应的Page结构中的flags标志设置为PG_reserved ，表示这些页已经被使用了，将来不能被用于分配。
        }

    标记空闲物理空间：
        //获得空闲空间的起始地址begin和结束地址end
        ...
        init_memmap(pa2page(begin), (end - begin) / PGSIZE);
        // 是把空闲物理页对应的Page结构中的flags和引用计数ref清零，
        // 并加到 free_area.free_list 指向的双向列表中，为将来的空闲页管理做好初始化准备工作。
        // 从pages开始保存了所有物理页的信息(严格来讲， 在pages处保存的npage个页的信息并不一定是所有的物理页信息，
        // 它还包括各种外设地址，ROM地址等。不过因为它包含了所有可用free页的信息，我们就可以使用 pages来找到任何free页的信息)。 
        // 那如何将free页的信息和free页联系起来呢？很简单， 我们用地址的物理页号(pa 的高 20bit)作为index来定位free页的信息。
        // 因为pages处保存了系统中的第一个物理页的页信息，只要我们知道某个页的物理地址， 我们就可以很容易的找到它的页号(pa >> 12)。 
        // 有了页号，我们就可以通过pages[页号]来定位其页的信息了。在本lab中， 获取页的信息是由 pa2page() 来完成的。
        // 在初始化free页的信息时， 我们只将连续多个free页中第一个页的信息连入free_list中， 
        // 并且只将这个页的property设置为连续多个free页的个数。 其他所有页的信息我们只是简单的设置为0。

        相应的实现在 default_pmm.c 中的 default_alloc_pages 函数和 default_free_pages() 函数，
        相关实现很简单，这里就不具体分析了，直接看源码，应该很好理解。
    物理内存页管理器框架: 

        在内存分配和释放方面最主要的作用是建立了一个物理内存页管理器框架，这实际上是一个函数指针列表，定义如下：
```c
struct pmm_manager {
            const char *name;                                 // 物理内存页管理器的名字
            void (*init)(void);                               // 初始化内存管理器
            void (*init_memmap)(struct Page *base, size_t n); // 初始化管理空闲内存页的数据结构
            struct Page *(*alloc_pages)(size_t n);            // 分配n个物理内存页
            void (*free_pages)(struct Page *base, size_t n);  // 释放n个物理内存页
            size_t (*nr_free_pages)(void);                    // 返回当前剩余的空闲页数
            void (*check)(void);                              // 用于检测分配/释放实现是否正确的辅助函数
};
```
    
    重点是实现 init_memmap/ alloc_pages / free_pages这三个函数。


## 内存段页式管理
    这个lab中最重要的一个知识点就是内存的段页式管理。 
    下图是段页式内存管理的示意图：
![](https://sfault-image.b0.upaiyun.com/419/087/4190879442-592131eaad948_articlex)
    
    我们可以看到，在这种模式下，
    逻辑地址 先通过 段机制 转化成 线性地址， 
    然后通过两种 页表(页目录和页表) 来实现 线性地址 到 物理地址的转换。 
    有一点需要注意，在 页目录 和 页表中 存放的地址都是 物理地址。
    
    下面是页目录表项：
![](https://sfault-image.b0.upaiyun.com/122/930/1229303217-5921324e8c4ef_articlex)
    
    下面是页表表项:
![](https://sfault-image.b0.upaiyun.com/386/565/3865659503-5921324e5d6d9_articlex)
    
    在X86系统中，页目录表 的起始物理地址存放在 cr3 寄存器中， 
    这个地址必须是一个 页对齐 的地址，也就是低 12 位必须为0。
    在ucore 用boot_cr3（mm/pmm.c）记录这个值。
    
    在ucore中，线性地址的的高10位作为页目录表的索引，之后的10位作为页表的的索引，
    所以页目录表和页表中各有1024个项，每个项占4B，所以页目录表和页表刚好可以用一个物理的页来存放。
  
# 实验1
    我们第一个实验需要完成的主要是default_pmm.c中的
    1. default_init()       ， 空闲内存页记录初始化，nr_free=0，总的空闲内存块先初始化为0；
    2. default_init_memmap()， 初始化空闲页链表，初始化每一个空闲页，并计算空闲页的总数；
    3. default_alloc_pages()， 分配内存大小为n的内存，遍历内容空闲页链表，找到第一块内存大小大于n的块，
                               然后分配出来，把它从空闲页链表中除去，
                               然后如果有多余的内存，把分完剩下的部分再次加入会空闲页链表中即可。
    4. default_free_pages() ， 释放大小为n的内存
    
##  1. default_init() 
    kern/init.c ---> 内核初始化kern_init() ---> pmm_init()( mm/pmm.c);
    
    pmm_init() ---> a. 内存页管理器初始化 init_pmm_manager() ----> b. 内存页初始化page_init()
    
    
### a. 内存页管理器初始化 init_pmm_manager()
init_pmm_manager() ----> default_pmm_manager.init() ----> default_init() ---> list_init();    

#### 内存管理器 结构体

```c
// 结构体定义==============
struct pmm_manager {
            const char *name;                                 // 物理内存页管理器的名字
            void (*init)(void);                               // 初始化内存管理器
            void (*init_memmap)(struct Page *base, size_t n); // 初始化管理空闲内存页的数据结构
            struct Page *(*alloc_pages)(size_t n);            // 分配n个物理内存页
            void (*free_pages)(struct Page *base, size_t n);  // 释放n个物理内存页
            size_t (*nr_free_pages)(void);                    // 返回当前剩余的空闲页数
            void (*check)(void);                              // 用于检测分配/释放实现是否正确的辅助函数
};
// 结构体实例==========
const struct pmm_manager default_pmm_manager = {
    .name = "default_pmm_manager",       // 名字
    .init = default_init,                // 管理器初始化函数
    .init_memmap = default_init_memmap,  // 内存映射初始化
    .alloc_pages = default_alloc_pages,  // 分配内存页
    .free_pages = default_free_pages,    // 空闲内存页
    .nr_free_pages = default_nr_free_pages,// 空闲内存页数量
    .check = default_check,                // 内存页检查
};
```
#### 管理器初始化函数 default_init()
```c
// kern/mm/default_pmm.c
free_area_t free_area;
#define free_list (free_area.free_list)   // 双向链表表头 header
#define nr_free (free_area.nr_free)       // 该 空闲区域链表 free list 中存储的空闲页的数量
static void
default_init(void) {
    list_init(&free_list); // 双向链表节点初始化 前节点指针 和 后节点指针 初始化为指向自己
    nr_free = 0;           // 空闲也数量清零 
} 

// 空闲区域链表 结构体 kern/mm/memlayout.h
typedef struct {
    list_entry_t free_list; //  双向链表表头 header  节点
    unsigned int nr_free;   //  该 空闲区域链表 free list 中存储的空闲页的数量
} free_area_t;

// 双向链表节点 结构体   libs/list.h
struct list_entry {
    struct list_entry *prev, *next;// 指向 前节点 和 后节点 的 节点指针
};
typedef struct list_entry list_entry_t;

// 双向链表节点初始化 ============
static inline void
list_init(list_entry_t *elm) {
    elm->prev = elm->next = elm;// 前节点指针 和 后节点指针 初始化为指向自己
}


```

###  b. 内存页初始化page_init() 来自 pmm_init()( mm/pmm.c);
    page_init() -> 获取最大物理内存地址 maxpa
                   从Kernel的结束地址(end-->取整pages)开始初始化 内存信息页
                   从 真正空闲页 freemem 开始 初始化每一个可用的内存页 init_memmap()
    init_memmap() -> pmm_manager->init_memmap() --> default_init_memmap() (kern/mm/default_pmm.c)
```c
// page_init() 
/* pmm_init - initialize the physical memory management */
static void
page_init(void) {
    struct e820map *memmap = (struct e820map *)(0x8000 + KERNBASE);
    uint64_t maxpa = 0;
    
// 1. 获取物理内存空间的最大地址 maxpa =======================
    cprintf("e820map:\n");
    int i;
    for (i = 0; i < memmap->nr_map; i ++) // 地址范围描述符 数组元素 个数 nr_map
    {
      // 这里内存块的信息来源于 bootloder中使用 BIOS INT5h中断探测到的 内存信息
        uint64_t begin = memmap->map[i].addr,   end = begin + memmap->map[i].size;// 该块内存的地址范围
        cprintf("  memory: %08llx, [%08llx, %08llx], type = %d.\n",
                memmap->map[i].size, begin, end - 1, memmap->map[i].type);// 内存块，大小，起始地址，借宿地址，类型(是否可用)
	       
        if (memmap->map[i].type == E820_ARM) 
	{
            if (maxpa < end && begin < KMEMSIZE) 
	    {
                maxpa = end;// 所有内存块中的最大地址， 实际物理内存空间结束地址 4GB  8GB 16GB ...
            }
        }
        
    }
    if (maxpa > KMEMSIZE) 
    {
        maxpa = KMEMSIZE;
    }
    
// 2. 计算内存信息页的起始页 地址(在系统数据之后，用来记录所有内存页信息的)=====
    extern char end[];// 全局变量 end， Kernel的结束地址(end)，这个地址是在tools/kernel.ld 中定义的(ucoreBSS段结束处)
    npage = maxpa / PGSIZE;// 计算总内存空间 按页分组(4K)，需要多少页
    pages = (struct Page *)ROUNDUP((void *)end, PGSIZE);// 系统结束处  空闲内存空间的起始地址，page是4k的帧数倍
    // 把end 按页大小为 边界取整 后，作为 管理页级物理内存空间所需的Page结构的内存空间
    //  除去操作系统后的需要管理的内存的起始页地址
    
// 3. 在 内存信息页区域(内存页管理区域)写入内存页信息======
    for (i = 0; i < npage; i ++) 
    {// 从pages开始，将所有页的信息的flag 都设置为reserved(不可用)。
        SetPageReserved(pages + i);// 该内存页 首先设置为 保留，不可用
    }
    
//4.  最终空闲物理空间起始地址 free , free页就是除了kernel 和 页信息外的可用空间
// 内存管理处的起始地址page + 内存管理所需的内存数量 ，之后为空闲地址。
    uintptr_t freemem = PADDR((uintptr_t)pages + sizeof(struct Page) * npage);
    
// 5. 初始化每一块内存空间中的 空闲内存页==========================
    for (i = 0; i < memmap->nr_map; i ++)
    {
        uint64_t begin = memmap->map[i].addr, end = begin + memmap->map[i].size;// 该块内存的地址范围
        if (memmap->map[i].type == E820_ARM)
	   {
            if (begin < freemem) 
	        {
                begin = freemem;// 将起始地址设置为空闲页空间的起始地址 freemem
            }
            if (end > KMEMSIZE)
	        {
                end = KMEMSIZE;// 最大地址
            }
            if (begin < end) 
	        {
                begin = ROUNDUP(begin, PGSIZE);// 内存起始地址 按页大小取整，还是地址，只不过地址是 页大小的整数倍
                end = ROUNDDOWN(end, PGSIZE);// 内存结束地址 按页大小取整
                if (begin < end) 
		        {
		  // 初始化改块内存空间的 free页的信息，重置页标志 flag中的 reserved位
		      // pa2page() 物理内存地址转换成 页id ，pmm.h --> mmu.h   左移12位, 相当于除以 4096(4k), 每个内存页大小4K
		      // 改块内存空间包含的空闲页数量 (end - begin) / PGSIZE
                    init_memmap(pa2page(begin), (end - begin) / PGSIZE);
   // init_memmap() -> pmm_manager->init_memmap() --> default_init_memmap() (kern/mm/default_pmm.c)
                }
            }
        }
    }
}

```
#### 初始化空闲内存页 default_init_memmap() (kern/mm/default_pmm.c)


    

#include <defs.h>
#include <x86.h>
#include <elf.h>// elf文件格式定义
/*
通过bootmain函数从硬盘中 加载elf格式的 操作系统os 到内存中使用程序块方式存储

 1. 将一些OS的ELF文件从硬盘中读到内存的ELFHDR里面 格式在elf.h中定义
 2. 在加载操作开始之前我们需要对ELFHDR进行判断，观察是否是一个合法的ELF头
 3. 通过循环读取每个段，并且将每个段读入相应的虚存p_va 程序块中
 4. 最后调用ELF header表头中的内核入口地址, 实现 内核链接地址 转化为 加载地址，无返回值。
    就转跳到ucore操作系统在内存中的入口位置（kern/init.c中的kern_init函数的起始地址）   
*/
#define SECTSIZE        512   // 一个扇区的大小
#define ELFHDR          ((struct elfhdr *)0x10000)// scratch space 虚拟地址va(virtual address)

/* 等待磁盘准备好
waitdisk - wait for disk ready */
static void
waitdisk(void) {
    while ((inb(0x1F7) & 0xC0) != 0x40)//判断磁盘寄存器的状态为标志
// 0x1F7 0号硬盘　读时状态寄存器
// 0xC0 = 0x11000000 最高两位为1
// 0x40 = 0x01000000 01表示空闲
// 检查0x1F7的最高两位，如果是01，那么证明磁盘准备就绪，跳出循环，否则继续等待。
        /* do nothing */;
}

/* 读取一整块扇区
readsect - read a single sector at @secno into @dst */
static void
readsect(void *dst, uint32_t secno) {
    // wait for disk to be ready
    waitdisk();// 1、等待磁盘准备好

    outb(0x1F2, 1);           // 设置需要读取得参数 扇区计数 count = 1
    outb(0x1F3, secno & 0xFF);// 即读取相应的内容到寄存器里面，
    outb(0x1F4, (secno >> 8) & 0xFF);
    outb(0x1F5, (secno >> 16) & 0xFF);
    outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    outb(0x1F7, 0x20); // 2、发出读取磁盘命令 cmd 0x20 - read sectors

    // wait for disk to be ready
    waitdisk();// 3、等待磁盘准备好； 

    // read a sector
    insl(0x1F0, dst, SECTSIZE / 4);// 4、把磁盘扇区数据读到指定内存。 
}

/* *
 * readseg - read @count bytes at @offset from kernel into virtual address @va,
 * might copy more than asked.
 * 从0号硬盘上读入os文件
 * 第一个参数是一个虚拟地址va (virtual address)，起始地址
 * 第二个是count（我们所要读取的数据的大小 512*8），
 *            SECTSIZE的定义我们通过追踪可以看到是512，即一个扇区的大小
 * 第三个是offset（偏移量）
 * */
static void
readseg(uintptr_t va, uint32_t count, uint32_t offset) {

    uintptr_t end_va = va + count;//结束地址

    // round down to sector boundary
    va -= offset % SECTSIZE;//　SECTSIZE＝512扇区单位长度　起始地址减去偏移　块首地址

    // translate from bytes to sectors; kernel starts at sector 1
    uint32_t secno = (offset / SECTSIZE) + 1;// 存储我们需要读取的磁盘的位置

// 通过一个for循环一次从磁盘中读取一个整块
    for (; va < end_va; va += SECTSIZE, secno ++) {
        // 继续对虚存va和secno进行自加操作，直到读完所需读的东西为止。
        readsect((void *)va, secno);// 磁盘中读取一个整块 存到相应的虚存va中

    }
}

/* bootmain - the entry of bootloader 
bootmain 函数 加载　elf格式的os
操作系统文件是存在0号硬盘上的
读取扇区的流程我们通过查询指导书可以看到：
1、等待磁盘准备好；
2、发出读取扇区的命令；
3、等待磁盘准备好；
4、把磁盘扇区数据读到指定内存。 
*/
void
bootmain(void) {
    // read the 1st page off disk
    // 从0号硬盘上读入os文件
    // 第一个参数是一个虚拟地址va(virtual address)，
    // 第二个是count（我们所要读取的数据的大小 512*8），
    // 第三个是offset（偏移量）
    // SECTSIZE的定义我们通过追踪可以看到是512，即一个扇区的大小
// 1. 将一些OS的ELF文件从硬盘中读到内存的ELFHDR里面 格式在elf.h中定义
    readseg((uintptr_t)ELFHDR, SECTSIZE * 8, 0);//　调用readseg函数从ELFHDR处读取8个扇区的大小。

    // is this a valid ELF?
// 2. 在加载操作开始之前我们需要对ELFHDR进行判断，观察是否是一个合法的ELF头
    if (ELFHDR->e_magic != ELF_MAGIC) {
        goto bad;//加载到错误得操作系统, 跳转到
    }

    struct proghdr *ph, *eph;//内存中 进程(程序)块 的存储方式, 在elf.h中定义
    // ph表示ELF段表首地址 ,  eph表示ELF段表末地址
    // load each program segment (ignores ph flags)
    ph = (struct proghdr *)((uintptr_t)ELFHDR + ELFHDR->e_phoff);//首地址=基地址+偏移量
    eph = ph + ELFHDR->e_phnum;// 末地址 = 首地址+elf大小

//　3. 通过循环读取每个段，并且将每个段读入相应的虚存p_va 程序块中
    for (; ph < eph; ph ++) {
        readseg(ph->p_va & 0xFFFFFF, ph->p_memsz, ph->p_offset);
    }

    // call the entry point from the ELF header
// 4. 最后调用ELF header表头中的内核入口地址, 实现 内核链接地址 转化为 加载地址，无返回值。
// 就转跳到ucore操作系统在内存中的入口位置（kern/init.c中的kern_init函数的起始地址）
    ((void (*)(void))(ELFHDR->e_entry & 0xFFFFFF))();

bad:
    outw(0x8A00, 0x8A00);
    outw(0x8A00, 0x8E00);

    /* do nothing */
    while (1);
}

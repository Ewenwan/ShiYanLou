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

# 0 初始化 Set up the important data segment registers (DS, ES, SS).
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
# 3 使能和进入保护模式 
    movl %cr0, %eax      # 首先将cr0寄存器里面的内容取出来
    orl $CR0_PE_ON, %eax # 进行一个或操作, PE是启用保护位(protection enable),当设置该位的时候即开启了保护模式
    movl %eax, %cr0      # 最后将得到的结果再写回 cr0中 
    
    # 只是进行了分段，没有开启分页机制，如果要开启分页机制的话我们需要同时置位PE和PG。
    
    # Jump to next instruction, but in 32-bit code segment.
    # Switches processor into 32-bit mode.

    # 最后通过一个长跳转指令正式进入保护模式。
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

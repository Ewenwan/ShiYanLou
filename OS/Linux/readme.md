# linux 系统

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
    
**寄存器名称限定符

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
**其他一些限定符

    1. “m”：对内存的操作被允许，用一个合法内存空间来做操作数。
    2. “o”：对内存的操作被允许，但是必须支持地址偏移值，
            即，对于给出的地址，加上一个偏移量，此时也是一个合法的地址。
    3. “V”：对内存的操作被允许，但是不支持偏移量。
            也就是说，支持“m”限制符，但不支持“o”的那些地址。
    4. “i”：对立即整数（const，常值）的操作被允许，
            这个常值可以是运行到汇编内才被赋值。
    5. “n”：对立即整数的操作被允许。
            许多系统不支持汇编中的操作数小于一个字宽，对于这些操作数，应该使用“n”而非“i”。
    6. “g”：任意寄存器，内存，立即数都被允许。
            除了非通用寄存器。
            
**最常用的修饰符有：“=”，“&”。

    1. “=”表示此操作数类型是只写。之前的值会被输出数据值替代。
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
            __asm__ __volatile__("addl  %%ebx,%%eax"  # 求和指令 
                                 :"=a"(foo)           # 输出  foo = (%eax += %ebx)
                                                      # 这个“=”表示这是输出结果的寄存器
                                 :"a"(foo), "b"(bar)  # 输入　%eax = foo; %ebx = bar
                                 );  
            printf("foo+bar=%d\n", foo);  
            return 0;  
    }  



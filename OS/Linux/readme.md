# linux 系统

## 基础知识
### 汇编程序
**c内联汇编

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

**编译成汇编代码

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

  

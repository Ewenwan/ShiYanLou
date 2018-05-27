# 操作系统

[深入理解计算机系统-笔记-第一章-计算机系统漫游](https://blog.csdn.net/wzmsltw/article/details/52894071)

[uCore OS实验指导书和源码网址 (2017)](https://chyyuu.gitbooks.io/ucore_os_docs/content/)

# ucore实验中的常用工具

在ucore实验中，一些基本的常用工具如下：
  - 命令行shell: bash shell -- 有对文件和目录操作的各种命令，如ls、cd、rm、pwd...
  - 系统维护工具：apt、git
    - apt：安装管理各种软件，主要在debian, ubuntu linux系统中
    - git：开发软件的版本维护工具
  - 源码阅读与编辑工具：eclipse-CDT、understand、gedit、vim
    - Eclipse-CDT：基于Eclipse的C/C++集成开发环境、跨平台、丰富的分析理解代码的功能，可与qemu结合，联机源码级Debug uCore OS。
    - Understand：商业软件、跨平台、丰富的分析理解代码的功能，Windows上有类似的sourceinsight软件
    - gedit：Linux中的常用文本编辑，Windows上有类似的notepad
    - vim: Linux/unix中的传统编辑器，类似有emacs等，可通过exuberant-ctags、cscope等实现代码定位
  - 源码比较和打补丁工具：diff、meld，用于比较不同目录或不同文件的区别, patch是打补丁工具
    - diff, patch是命令行工具，使用简单
    - meld是图形界面的工具，功能相对直观和方便，类似的工具还有 kdiff3、diffmerge、P4merge
  - 开发编译调试工具：gcc 、gdb 、make
    - gcc：C语言编译器
    - gdb：执行程序调试器
    - ld：链接器
    - objdump：对ELF格式执行程序文件进行反编译、转换执行格式等操作的工具
    - nm：查看执行文件中的变量、函数的地址
    - readelf：分析ELF格式的执行程序文件
    - make：软件工程管理工具， make命令执行时，需要一个 makefile 文件，以告诉make命令如何去编译和链接程序
    - dd：读写数据到文件和设备中的工具
  - 硬件模拟器：qemu -- qemu可模拟多种CPU硬件环境，本实验中，用于模拟一台 intel x86-32的计算机系统。类似的工具还有BOCHS, SkyEye等
  - markdown文本格式的编写和阅读工具(比如阅读ucore_docs)
    - 编写工具 haroopad 
    - 阅读工具 gitbook


# 上述工具的使用方法在线信息
- apt-get
  - http://wiki.ubuntu.org.cn/Apt-get%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97
- git github
  - http://www.cnblogs.com/cspku/articles/Git_cmds.html
  - http://www.worldhello.net/gotgithub/index.html
- diff patch
  - http://www.ibm.com/developerworks/cn/linux/l-diffp/index.html
  - http://www.cnblogs.com/itech/archive/2009/08/19/1549729.html  
- gcc
  - http://wiki.ubuntu.org.cn/Gcchowto
  - http://wiki.ubuntu.org.cn/Compiling_Cpp
  - http://wiki.ubuntu.org.cn/C_Cpp_IDE
  - http://wiki.ubuntu.org.cn/C%E8%AF%AD%E8%A8%80%E7%AE%80%E8%A6%81%E8%AF%AD%E6%B3%95%E6%8C%87%E5%8D%97
- gdb
  - http://wiki.ubuntu.org.cn/%E7%94%A8GDB%E8%B0%83%E8%AF%95%E7%A8%8B%E5%BA%8F
- make & makefile
  - http://wiki.ubuntu.com.cn/index.php?title=%E8%B7%9F%E6%88%91%E4%B8%80%E8%B5%B7%E5%86%99Makefile&variant=zh-cn
  - http://blog.csdn.net/a_ran/article/details/43937041
- shell
   - http://wiki.ubuntu.org.cn/Shell%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80
   - http://wiki.ubuntu.org.cn/%E9%AB%98%E7%BA%A7Bash%E8%84%9A%E6%9C%AC%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97
- understand
   - http://blog.csdn.net/qwang24/article/details/4064975
- vim
   - http://www.httpy.com/html/wangluobiancheng/Perljiaocheng/2014/0613/93894.html
   - http://wenku.baidu.com/view/4b004dd5360cba1aa811da77.html
- meld
   - https://linuxtoy.org/archives/meld-2.html
- qemu
   - http://wenku.baidu.com/view/04c0116aa45177232f60a2eb.html
- Eclipse-CDT
   - http://blog.csdn.net/anzhu_111/article/details/5946634  
- haroopad
   - http://pad.haroopress.com/
- gitbook
   - https://github.com/GitbookIO/gitbook  https://www.gitbook.com/


# 汇编指令概述　AT&T汇编基本语法 

    x86汇编的两种语法：intel语法和AT&T语法
    x86汇编一直存在两种不同的语法，在intel的官方文档中使
    用intel语法，Windows也使用intel语法，而UNIX平台的汇编器一
    直使用AT&T语法，所以本书使用AT&T语法。

    AT&T格式的汇编，与Intel格式的汇编有一些不同。二者语法上主要有以下几个不同：
```c 
    1. 寄存器命名原则
        AT&T格式: %eax                      Intel格式: eax
        寄存器前面加"%"，以便跟符号名区分开。
    2. 源/目的操作数顺序    -->
        AT&T格式: movl %eax, %ebx           Intel格式: mov ebx, eax
        
    3. 常数/立即数的格式　
        AT&T格式: movl $_value, %ebx        Intel格式: mov eax, _value
        CPU内部产生的数称为立即数，在汇编程序中立即数前面加"$" 美元符号
        movl $1, %eax
          CPU内部产生一个数字1, 然后传送到eax寄存器中。
          mov后边的l表示long,说明是32位的传送指令。
        movl $4, %ebx
          与上条指令类似，生成一个立即数4，传送到ebx寄存器中。
          
    4. 把value的地址放入eax寄存器
        AT&T格式: movl $0xd00d, %ebx        Intel格式: mov ebx, 0xd00d
        
    5. 操作数长度标识 
        AT&T格式: movw %ax, %bx             Intel格式: mov bx, ax
    6. 寻址方式 
        AT&T格式:   immed32(basepointer, indexpointer, indexscale)
        Intel格式:  [basepointer + indexpointer × indexscale + imm32)
    
    7.中断　系统调用
    　movl $1, %eax     // 成一个立即数1，传送到eax寄存器中。
                        // eax寄存器的值是系统调用号,1表示_exit系统调用
      movl $4, %ebx     // 生成一个立即数4，传送到ebx寄存器中。
      　　　　　　　　  　// ebx的值则是传给_exit系统调用的参数,还可以有ecx、edx做参数，也就是退出状态。
      int $0x80　　　　  // interupt 
      // int指令称为软中断指令, CPU从用户模式切换到特权模式，然后跳转到内核代码中执行异常处理程序
      // int指令中的立即数0x80是一个参数,在linux内核中,int $0x80这种异常称系统调用（System Call）。
      // 一般在调用结束后CPU再切换回用户模式，继续执行int指令后面的指令，在用户程序看来就像函数的调用和返回一样.
      // _exit这个系统调用会终止掉当前进程,而不会返回它继续执行.
```
# 


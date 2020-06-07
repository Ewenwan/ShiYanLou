# linux 系统

[linux 基础环境配置](http://robot.czxy.com/docs/linux/other/03-vim-quick-short/)


[linux下编程代码实验 CMake/Make/Shell/Multi-thread/socket/stl/awk](https://github.com/Ewenwan/Linux_Code_Test)

[linux 软硬件驱动 ](https://github.com/Ewenwan/linux-study)

[Linux程序设计 中文版 第四版](https://github.com/Ewenwan/Beginning-Linux-Programming)

[Linux中国翻译项目](https://github.com/Ewenwan/TranslateProject)

[Vim Practice ](https://github.com/oldratlee/vim-practice)

[Linux命令必知必会](https://github.com/mylxsw/growing-up/blob/master/doc/Linux%E5%91%BD%E4%BB%A4%E5%BF%85%E7%9F%A5%E5%BF%85%E4%BC%9A.md)

[Unix环境高级编程 代码&学习笔记](https://github.com/Ewenwan/APUE)

[ubuntu deb软件包搜索下载](https://packages.ubuntu.com/)

[系统编程：Makefile的编写、进程线程区别以及控制、生产者消费者模型、System V标准和POSIX标准中的IPC间通信、简单Shell脚本的编写；网络编程：基于TCP/UDP的客户端/服务器、socket、高级I/O、select/poll/epoll、Http协议](https://github.com/Ewenwan/_Linux-OS-Network_)


开机启动软件&执行命令
```sh
将要执行的命令添加到/etc/rc.local中即可

sudo vi /etc/rc.local
注意命令要写在exit 0之前
```

开发工具 
```sh
必备IDE 
clion：C++开发平台
pycharm：Python开发平台
```

apt-fast：更快速的软件管理

安装过程会进入一个图形界面，配置线程数等信息，全部默认即可

```sh
sudo add-apt-repository ppa:apt-fast/stable
sudo apt-get update
sudo apt-get -y install apt-fast
以后安装应用，把apt-get直接替换成apt-fast即可，例如安装vlc

sudo apt-fast install vlc
```

vlc万能播放器：支持倍速播放

sudo apt-get install vlc

terminator多tab终端：黑客必备

sudo apt-get install terminator

chrome浏览器：最好的浏览器，没有之一

wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb

sudo dpkg -i google-chrome*.deb; sudo apt-get -f install

vim：编辑器之神

sudo apt-get install vim

常用软件 

stardict：星际译王，翻译软件

chrome - Google浏览器

typora - Markdown编辑器

git & smartgit 界面版本管理工具

zsh：更强大的Shell

sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"

eek - GIF录制工具

inkscape - 矢量图工具

WPS

环境工具

anaconda - Python集成环境

Anaconda自带的conda环境工具可以用来的创建或删除虚拟环境，方便我们切换不同版本的虚拟python运行环境，更重要的是可以加快我们加载环境的速度，方便切换不同版本的pip依赖库。



ROS - 机器人操作系统

ursim - UR模拟器

nvm - Node版本管理

SimpleScreenRecorder - 屏幕录制工具





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
    8. "r"：从eax, ebx, ecx, edx, esi, edi

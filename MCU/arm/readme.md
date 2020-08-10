# arm 学习
[移动端盒子滤波算法优化笔记 arm neon 汇编优化](https://github.com/Ewenwan/ArmNeonOptimization)

[arm-gcc-inline-assembler  ARM GCC 内联汇编参考手册 - 中文版](https://github.com/tidyjiang8/arm-gcc-inline-assembler)

[Arm-neon-intrinsics 制指令集](https://github.com/Ewenwan/Arm-neon-intrinsics)

[arm官方资料](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.subset.swdev.rvds/index.html)

[ARM](https://www.cnblogs.com/lvdongjie/category/975651.html)

[ARMv8 架构与指令集.学习笔记](https://www.cnblogs.com/lvdongjie/p/6644821.html)

[ARMv8 内存管理架构.学习笔记](https://www.cnblogs.com/lvdongjie/p/6644848.html)

[嵌入式 arm linux c编程](https://github.com/PacktPublishing/Mastering-Embedded-Linux-Programming-Second-Edition)

[linux 设备驱动开发](https://github.com/PacktPublishing/Linux-Device-Drivers-Development)

[ARM内嵌汇编示例](https://wenku.baidu.com/view/72c12e4133687e21af45a990.html)

[YUV转RGB（NV21-ARGB）的Neon优化代码](https://blog.csdn.net/jxt1234and2010/article/details/46620001)

[使用ARM NEON Intrinsics加速Video Codec](https://www.jianshu.com/p/70601b36540f)

# android 开发


linux adb 设被配置:

            新建文件 /etc/udev/rules.d/80-android.rules  写入 设备usb编号(lusb查看得到)

            增加内容:

            SUBSYSTEM=="usb", ATTR{idVendor}=="4255", MODE="0666", GROUP="plugdev"

            将设备usb编号替换到 ATTR{idVendor}=="4255"中，其他不用修改。

            保存，sudo chmod 777 80-android.rules

            执行以下命令：

            sudo usermod -a -G plugdev $(id -u -n)

            sudo udevadm control --reload-rules

            sudo service udev restart

            sudo udevadm trigger

            第四步： 重启

            adb adb kill-server
            adb start-server 
            adb devices
            
            adb -s 序列号 shell 登录指定设备
             



# ARM与单片机到底有啥区别

## 1、软件方面
    这应该是最大的区别了。
    引入了操作系统。为什么引入操作系统？有什么好处？
    1）方便。主要体现在后期的开发，即在操作系统上直接开发应用程序。
       不像单片机一样一切都要重新写。前期的操作系统移植工作，还是要专业人士来做。
    2）安全。这是LINUX的一个特点。
       LINUX的内核与用户空间的内存管理分开，不会因为用户的单个程序错误而引起系统死掉。
       这在单片机的软件开发中没见到过。
    3）高效。引入进程的管理调度系统，使系统运行更加高效。
       在传统的单片机开发中大多是基于中断的前后台技术，对多任务的管理有局限性。
       
## 2、硬件方面
    现在的 8位单片机技术 硬件发展的也非常得快，也出现了许多功能非常强大的单片机。
    但是与32arm相比还是有些差距吧。
    arm芯片大多把SDRAM,LCD等控制器集成到片子当中。在8位机，大多要进行外扩。
    总的来说，
    单片机是个微控制器，
    arm显然已经是个微处理器了。
    引入嵌入式操作系统之后，可以实现许多单片机系统不能完成的功能。
    比如：嵌入式web服务器，java虚拟机等。
    也就是说，有很多免费的资源可以利用，上述两种服务就是例子。
    如果在单片机上开发这些功能可以想象其中的难度。

## 初学者如何选择ARM开发硬件？
    1． 如果你有做硬件和单片机的经验,
        建议自己做个最小系统板：假如你从没有做过ARM的开发，
        建议你一开始不要贪大求全，把所有的应用都做好，
        因为ARM的启动方式和dsp或单片机有所不同，往往会碰到各种问题，
        所以建议先布一个仅有Flash,SRAM或SDRAM、CPU、JTAG、和复位信号的小系统板，留出扩展接口。
        使最小系统能够正常运行，你的任务就完成了一半，好在arm的外围接口基本都是标准接口，
        假如你已有这些硬件的布线经验，这对你来讲是一件很轻易的事情。
    2． 动手写启动代码，根据硬件地址先写一个能够启动的小代码，包括以下部分：
        初始化端口，屏蔽中断，把程序拷贝到SRAM中;
        完成代码的重映射；
        配置中断句柄，连接到C语言入口。
        也许你看到给你的一些示例程序当中，bootloader会有很多东西，
        但是不要被这些复杂的程序所困扰，因为你不是做开发板的，
        你的任务就是做段小程序，让你的应用程序能够运行下去
    3.假如你是作硬件，每个厂家基本上都有针对该芯片的DEMO板原理图。
        先将原理图消化。这样你以后做设计时，对资源的分配心中有数。
        器件的DATSHEET一定要好好消化。
    4． 仔细研究你所用的芯片的资料，
        尽管arm在内核上兼容，但每家芯片都有自己的特色，编写程序时必须考虑这些问题.
        尤其是女孩子，在这儿千万别有依靠心理，总想拿别人的示例程序修改，却越改越乱。
    5． 多看一些操作系统程序，
        在arm的应用开放源代码的程序很多，要想提高自己，
        就要多看别人的程序，linux,uc/os-II等等这些都是很好的原码。
    6.假如做软件最好对操作系统的机理要有所了解。
        当然这对软件工程师来说是小菜一碟。
        但假如是硬件出身的就有点费劲。
        
# 知识点

## android绑核
[参考](https://blog.csdn.net/h176nhx7/article/details/86520060)
```c

#include <jni.h>
#include <android/log.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>

#include <sys/syscall.h>

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>

#define __USE_GNU
#include<sched.h>
#include<ctype.h>

#ifndef CPU_ZERO
#define CPU_SETSIZE 1024
#define __NCPUBITS  (8 * sizeof (unsigned long))
typedef struct
{
    unsigned long __bits[CPU_SETSIZE / __NCPUBITS];
} cpu_set_t;

#define CPU_SET(cpu, cpusetp) \
  ((cpusetp)->__bits[(cpu)/__NCPUBITS] |= (1UL << ((cpu) % __NCPUBITS)))
#define CPU_ZERO(cpusetp) \
  memset((cpusetp), 0, sizeof(cpu_set_t))
#else
#define CPU_SET(cpu,cpustep) ((void)0)
#define CPU_ZERO(cpu,cpustep) ((void)0)
#endif

#ifdef DEBUG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#else
#define LOGD(...) ((void)0)
#define LOGE(...) ((void)0)
#endif

void bind_core(int cpu)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu,&mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)//设置线程CPU亲和力
    {
        LOGD("warning: could not set CPU affinity, continuing...\n");
    }else
    {
        LOGD("set affinity to %d success",cpu);
    }
    
    // linux 等绑核
    // pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &mask);
}



```

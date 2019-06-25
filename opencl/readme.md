# opencl

OpenCL(全称为Open Computing Langugae，开放运算语言)是第一个面向异构系统(此系统中可由CPU，GPU或其它类型的处理器架构组成)的并行编程的开放式标准。
它是跨平台的。
OpenCL由两部分组成，一是用于编写kernels（在OpenCL设备上运行的函数）的语言，二是用于定义并控制平台的API(函数)。
OpenCL提供了基于任务和基于数据两种并行计算机制，它极大地扩展了GPU的应用范围，使之不再局限于图形领域。
OpenCL是一种标准，intel、Nvidia、ARM、AMD、QUALCOMM、Apple都有其对应的OpenCL实现。
像NVDIA将OpenCL实现集成到它的CUDA SDK中，而AMD则将其实现后放在AMD APP （Accelerated Paral Processing）SDK中…

## 框架组成

OpenCL平台API：平台API定义了宿主机程序发现OpenCL设备所用的函数以及这些函数的功能，另外还定义为OpenCL应用创建上下文(上下文表示的是程序运行时所拥有的所有软硬件资源+内存+处理器)的函数。这里的平台指的是宿主机、OpenCL设备和OpenCL框架的组合。

OpenCL运行时API：平台API主要用来创建上下文，运行时API则强调使用这个上下文满足应用需求的函数集，用来管理上下文来创建命令队列以及运行时发生的其它操作。例如，将命令提交到命令队列的函数。

OpenCL编程语言：用来编写内核代码的编程语言，基于ISO C99标准的一个扩展子集，通常称为OpenCL C编程语言。

## 基本概念

OpenCL程序同CUDA程序一样，也是分为两部分，一部分是在主机（以CPU为核心）上运行，一部分是在设备（以GPU为核心）上运行。设备有一个或多个计算单元，计算单元又包含一个或多个处理单元。在设备上运行的程序被称为核函数。但是对于核函数的编写，CUDA一般直接写在程序内，OpenCL是写在一个独立的文件中，并且文件后缀是.cl，由主机代码读入后执行，这一点OpenCL跟OpenGL中的渲染程序很像。

OpenCL平台由两部分组成，宿主机和OpenCL设备:

宿主机Host：宿主机一般为CPU，扮演组织者的角色。其作用包括定义kernel、为kernel指定上下文、定义NDRange和队列等；队列控制着kernel如何执行以及何时执行等细节。

设备Device：通常称为计算设备，可以是CPU、GPU、DSP或硬件提供以及OpenCL开发商支持的任何其他处理器。

SIMT（Single Instruction Multi Thread）： 单指令多线程，GPU并行运算的主要方式，很多个多线程同时执行相同的运算指令，当然可能每个线程的数据有所不同，但执行的操作一致。

核函数（Kernel）： 是在设备程序上执行运算的入口函数，在主机上调用。

工作项（Work-item）： 跟CUDA中的线程（Threads）是同一个概念，N多个工作项（线程）执行同样的核函数，每个Work-item都有一个唯一固定的ID号，一般通过这个ID号来区分需要处理的数据。

工作组（Work-group）：跟CUDA中的线程块（Block）是同一个概念，N多个工作项组成一个工作组，Work-group内的这些Work-item之间可以通信和协作。

ND-Range： 跟CUDA中的网格是同一个概念，定义了Work-group的组织形式。

上下文（Context）： 定义了整个OpenCL的运行环境，包括Kernel、Device、程序对象和内存对象：

设备：OpenCL程序调用的计算设备。

内核：在计算设备上执行的并行程序。

程序对象：内核程序的源代码(.cl文件)和可执行文件。

内存对象：计算设备执行OpenCL程序所需的变量。

## 编写OpenCL程序的基本步骤

      1）获取平台–>clGetPlatformIDs
      2）从平台中获取设备–>clGetDeviceIDs
      3）创建上下文–>clCreateContext
      4）创建命令队列–>clCreateCommandQueue
      前面为 平台层，后面为 运行时层
      5）创建缓存->clCreateBuffer
      6）读取程序文件，创建程序–>clCreateProgramWithSource
      7）编译程序–>clBuildProgram
      8）创建内核–>clCreateKernel
      9）为内核设置参数–>clSetKernelArg
      10）将内核发送给命令队列，执行内核–>clEnqueueNDRangeKernel
      11）获取计算结果–>clEnqueueReadBuffer
      12）释放资源–>clReleaseXX**


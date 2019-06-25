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
Device（设备）：通过cl_device来表现，使用下面的代码：
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

## 示例 
> 向量相加
```c
// c 语言版本
void vector_add_cpu (const float* src_a, // 向量a, 常量指针
                     const float* src_b,
                     float*  res,        // 结果向量
                     const int num)      // 数据数量
{
   for (int i = 0; i < num; i++)
      res[i] = src_a[i] + src_b[i];      // 一个线程在做
}

// gpu kernel版本
// 在GPU上，逻辑就会有一些不同。我们使每个线程计算一个元素的方法来代替cpu程序中的循环计算。每个线程的index与要计算的向量的index相同。
__kernel void vector_add_gpu (__global const float* src_a, // gpu 上的 常量指针 数据
                              __global const float* src_b,
                              __global float* res,
                              const int num)
{
   /* get_global_id(0) 返回正在执行的这个线程的ID。
   许多线程会在同一时间开始执行同一个kernel，
   每个线程都会收到一个不同的ID，所以必然会执行一个不同的计算。*/
   const int idx = get_global_id(0);

   /* 每个work-item都会检查自己的id是否在向量数组的区间内[0~num)。
   如果在，work-item就会执行相应的计算。*/
   if (idx < num)
      res[idx] = src_a[idx] + src_b[idx];
}
```

> 主机 准备工作 建立基本OpenCL运行环境

Plantform（平台）：主机加上OpenCL框架管理下的若干设备构成了这个平台，通过这个平台，应用程序可以与设备共享资源并在设备上执行kernel。平台通过cl_plantform来展现，可以使用下面的代码来初始化平台：

```c
// Returns the error code 
// 获取平台ID
cl_int oclGetPlatformID (cl_platform_id *platforms) // Pointer to the platform object

```

Device（设备）：通过cl_device来表现，使用下面的代码：

```C
// Returns the error code
// 获取设备ID
cl_int clGetDeviceIDs (cl_platform_id platform,
	cl_device_type device_type, // Bitfield identifying the type. For the GPU we use CL_DEVICE_TYPE_GPU
	cl_uint num_entries, // Number of devices, typically 1
	cl_device_id *devices, // Pointer to the device object
	cl_uint *num_devices) // Puts here the number of devices matching the device_type

```

Context（上下文）：定义了整个OpenCL化境，包括OpenCL kernel、设备、内存管理、命令队列等。上下文使用cl_context来表现。使用以下代码初始化：

```c
// Returs the context
// 创建上下文 环境
cl_context clCreateContext (const cl_context_properties *properties, // Bitwise with the properties (see specification)

	cl_uint num_devices, // Number of devices
	const cl_device_id *devices, // Pointer to the devices object
	void (*pfn_notify)(const char *errinfo, const void *private_info, size_t cb, void *user_data), // (don't worry about this)
	void *user_data, // (don't worry about this)
	cl_int *errcode_ret) // error code result
```

Command-Queue（指令队列）：就像它的名字一样，他是一个存储需要在设备上执行的OpenCL指令的队列。“指令队列建立在一个上下文中的指定设备上。多个指令队列允许应用程序在不需要同步的情况下执行多条无关联的指令。”

```c
// 创建指令队列
cl_command_queue clCreateCommandQueue (cl_context context,
	cl_device_id device,
	cl_command_queue_properties properties, // Bitwise with the properties
	cl_int *errcode_ret) // error code result
```

> 综合示例

```c
cl_int error = 0;          // int 类型的错误码，作为函数返回值，表示函数执行正确与否(执行状态)
cl_platform_id platform;   // 平台id
cl_device_id device;       // 设别id
cl_context context;        // 上下文环境
cl_command_queue queue;    // 指令队列

// Platform 获取平台id
error = oclGetPlatformID(&platform);
if (error != CL_SUCCESS) {
   cout << "Error getting platform id: " << errorMessage(error) << endl;
   exit(error);
}
// Device 获取设备ID
error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
if (err != CL_SUCCESS) {
   cout << "Error getting device ids: " << errorMessage(error) << endl;
   exit(error);
}
// Context 创建上下文 环境
context = clCreateContext(0, 1, &device, NULL, NULL, &error);
if (error != CL_SUCCESS) {
   cout << "Error creating context: " << errorMessage(error) << endl;
   exit(error);
}
// Command-queue  创建指令队列
queue = clCreateCommandQueue(context, device, 0, &error);
if (error != CL_SUCCESS) {
   cout << "Error creating command queue: " << errorMessage(error) << endl;
   exit(error);
}


```

### 变量内存分配

```c
// 主机 端  内存分配
// 主机的基本环境已经配置好了，为了可以执行我们的写的小kernel，我们需要分配3个向量的内存空间，然后至少初始化它们其中的两个。
const int size = 1234567
float* src_a_h = new float[size]; 
float* src_b_h = new float[size];
float* res_h   = new float[size]; // 结果向量
// Initialize both vectors 初始化两个加数向量
for (int i = 0; i < size; i++) 
{
   src_a_h = src_b_h = (float) i;  
}

// 设备上分配内存，我们需要使用cl_mem类型
// Returns the cl_mem object referencing the memory allocated on the device
// 调用接口
cl_mem clCreateBuffer ( cl_context context, // The context where the memory will be allocated
			cl_mem_flags flags,
			size_t size, // The size in bytes
			void *host_ptr,
			cl_int *errcode_ret)
/*
flags是逐位的，选项如下：
CL_MEM_READ_WRITE    // 可读可写
CL_MEM_WRITE_ONLY    // 可写
CL_MEM_READ_ONLY     // 可读
CL_MEM_USE_HOST_PTR  // 使用主机指针 创建
CL_MEM_ALLOC_HOST_PTR// 分配
CL_MEM_COPY_HOST_PTR // 从 host_ptr处拷贝数据
*/

// 使用

const int mem_size = sizeof(float)*size; // 总子节数量
// 只读 变量 从主机的 指针 拷贝数据过来 进行创建
cl_mem src_a_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_a_h, &error);
cl_mem src_b_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_b_h, &error);、
// 可写变量，保存向量加法结果
cl_mem res_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_size, NULL, &error);

```


### 程序和kernel

到现在为止，你可能会问自己一些问题，比如：我们怎么调用kernel？编译器怎么知道如何将代码放到设备上？我们怎么编译kernel？
下面是我们在对比OpenCL程序和OpenCL kernel时的一些容易混乱的概念：

Kernel：你应该已经知道了，像在上文中描述的一样，kernel本质上是一个我们可以从主机上调用的，运行在设备上的函数。你或许不知道kernel是在运行的时候编译的！更一般的讲，所有运行在设备上的代码，包括kernel和kernel调用的其他的函数，都是在运行的时候编译的。这涉及到下一个概念，Program。

Program：OpenCL Program由kernel函数、其他函数和声明组成。它通过cl_program表示。当创建一个program时，你必须指定它是由哪些文件组成的，然后编译它。

你需要用到下面的函数来建立一个Program：

```c
// Returns the OpenCL program
cl_program clCreateProgramWithSource (cl_context context,
				     cl_uint count, // number of files
				     const char **strings, // array of strings, each one is a file
				     const size_t *lengths, // array specifying the file lengths
				     cl_int *errcode_ret) // error code to be returned

```

当我们创建了Program我们可以用下面的函数执行 编译 操作：

```c
cl_int clBuildProgram (cl_program program,
    cl_uint num_devices,
    const cl_device_id *device_list,
    const char *options, // Compiler options, see the specifications for more details
    void (*pfn_notify)(cl_program, void *user_data),
    void *user_data);
```

查看编译log，必须使用下面的函数：

```c
cl_int clGetProgramBuildInfo (cl_program program,
		    cl_device_id device,
		    cl_program_build_info param_name, // The parameter we want to know
		    size_t param_value_size,
		    void *param_value, // The answer
		    size_t *param_value_size_ret);
```

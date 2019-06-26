# opencl

OpenCL(全称为Open Computing Langugae，开放运算语言)是第一个面向异构系统(此系统中可由CPU，GPU或其它类型的处理器架构组成)的并行编程的开放式标准。
它是跨平台的。
OpenCL由两部分组成，一是用于编写kernels（在OpenCL设备上运行的函数）的语言，二是用于定义并控制平台的API(函数)。
OpenCL提供了基于任务和基于数据两种并行计算机制，它极大地扩展了GPU的应用范围，使之不再局限于图形领域。
OpenCL是一种标准，intel、Nvidia、ARM、AMD、QUALCOMM、Apple都有其对应的OpenCL实现。
像NVDIA将OpenCL实现集成到它的CUDA SDK中，而AMD则将其实现后放在AMD APP （Accelerated Paral Processing）SDK中…

CUDA只支持NVIDIA自家的GPU。

CUDA C语言与OpenCL的定位不同，或者说是使用人群不同。CUDA C是一种高级语言，那些对硬件了解不多的非专业人士也能轻松上手；而OpenCL则是针对硬件的应用程序开发接口，它能给程序员更多对硬件的控制权，相应的上手及开发会比较难一些。


CUDA 与 opencl 名词比较：

	Block: 相当于 opencl 中的work-group
	Thread：相当于opencl 中的work-item
	SP: 相当于opencl 中的PE
	SM: 相当于opencl 中的CU
	warp: 相当于opencl 中的wavefront(简称wave)，基本的调试单位



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
    1. 检测申请计算资源
      1）获取平台–>clGetPlatformIDs
      2）从平台中获取设备–>clGetDeviceIDs
      3）创建上下文–>clCreateContext
      4）创建命令队列–>clCreateCommandQueue
      前面为 平台层，后面为 运行时层
      5）创建缓存->clCreateBuffer
    2. 拷贝主机数据到设备device  clCreateBuffer
    3. opencl程序编译
      6）读取程序文件，创建程序–>clCreateProgramWithSource
      7）编译程序–>clBuildProgram
      8）创建内核–>clCreateKernel
      9）为内核设置参数–>clSetKernelArg
         设置 NDRange 
    4. 运行kernel函数
      10）将内核发送给命令队列，执行内核–>clEnqueueNDRangeKernel
    5. 拷贝设备上的计算结果到主机
      11）获取计算结果–>clEnqueueReadBuffer
    6.释放计算资源
      12）释放资源–>clReleaseXX**
      
	释放kernel核,              clReleaseKernel
	释放program编译程序,       clReleaseProgram
	释放device memory设备内存, clReleaseMemObject
	释放command queue指令队列, clReleaseCommandQueue
	释放context上下文环境,     clReleaseContext

CUDA C加速步骤：

	1) 在device (也就是GPU) 上申请内存
	2) 将host (也就是CPU) 上的数据拷贝到device
	3) 执行CUDA kernel function
	4) 将device上的计算结果传回host
	5) 释放device上的内存

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
CL_MEM_READ_WRITE    // kernel 可读可写
CL_MEM_WRITE_ONLY    // kernel 可写
CL_MEM_READ_ONLY     // kernel 可读
CL_MEM_USE_HOST_PTR  // 使用主机指针 创建 device端会对host_ptr位置内存进行缓存
CL_MEM_ALLOC_HOST_PTR// 分配 新开辟一段host端可以访问的内存
CL_MEM_COPY_HOST_PTR // 从 host_ptr处拷贝数据 在devices新开辟一段内存供device使用，并将host上的一段内存内容copy到新内存上

host权限，默认为可读写:
CL_MEM_HOST_WRITE_ONLY：host 只写
CL_MEM_HOST_READ_ONLY： host只读
CL_MEM_HOST_NO_ACCESS: host没有访问权限

size是buffer的大小
host_ptr只有在CL_MEM_USE_HOST_PTR， CL_MEM_COPY_HOST_PTR时才有效。

一般对于kernel函数的输入参数，使用CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR可以将host memory拷贝到device memory，表示device只读，位置在device上并进行内存复制，host权限为可读写；
对于输出参数，使用CL_MEM_WRITE_ONLY表示device只写，位置在device上，host权限为可读可写。
如果进行host与device之间的内存传递，可以使用clEnqueueReadBuffer读取device上的内存到host上, clEnqueueWriteBuffer可以将host上内存写到device上。

*/

// 使用

const int mem_size = sizeof(float)*size; // 总子节数量
// 只读 变量 从主机的 指针 拷贝数据过来 进行创建
cl_mem src_a_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_a_h, &error);
cl_mem src_b_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_b_h, &error);、
// 可写变量，保存向量加法结果
cl_mem res_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_size, NULL, &error);

```


### 编译 程序和kernel

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

最后，我们需要“提取”program的入口点。使用cl_kernel：

```c
cl_kernel clCreateKernel (cl_program program, // The program where the kernel is

const char *kernel_name, // The name of the kernel, i.e. the name of the kernel function as it's declared in the code

cl_int *errcode_ret);
```


注意我们可以创建多个OpenCL program，每个program可以拥有多个kernel。

```c
// 创建程序
// Uses NVIDIA helper functions to get the code string and it's size (in bytes)
size_t src_size = 0;
const char* path = shrFindFilePath("vector_add_gpu.cl", NULL);
const char* source = oclLoadProgSource(path, "", &src_size);
cl_program program = clCreateProgramWithSource(context, 1, &source, &src_size, &error);
assert(error == CL_SUCCESS);

// 编译程序
error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
assert(error == CL_SUCCESS);

// 查看编译日志
char* build_log;
size_t log_size;
// First call to know the proper size
clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
build_log = new char[log_size+1];
// Second call to get the log
clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
build_log[log_size] = '\0';
cout << build_log << endl;
delete[] build_log;

// 提取编译好的kernel
cl_kernel vector_add_kernel = clCreateKernel(program, "vector_add_gpu", &error);
assert(error == CL_SUCCESS);

```


### 运行 kernel

一旦我们的kernel建立好，我们就可以运行它。

首先，我们必须设置kernel的参数

```c
cl_int clSetKernelArg (cl_kernel kernel, // Which kernel
    cl_uint arg_index, // Which argument
    size_t arg_size, // Size of the next argument (not of the value pointed by it!)
    const void *arg_value);// Value
 ```
 
每个参数都需要调用一次这个函数。

当所有参数设置完毕，我们就可以调用这个kernel：

```c
cl_int  clEnqueueNDRangeKernel (
	cl_command_queue command_queue, 
	cl_kernel kernel, 
	cl_uint  work_dim,    // Choose if we are using 1D, 2D or 3D work-items and work-groups
	const size_t *global_work_offset,
	const size_t *global_work_size,   // The total number of work-items (must have work_dim dimensions)
	const size_t *local_work_size,     // The number of work-items per work-group (must have work_dim dimensions)
	cl_uint num_events_in_wait_list, 
	const cl_event *event_wait_list, 
	cl_event *event);
/*
参数：
command_queue,
kernel,
work_dim，使用多少维的NDRange，可以设为1, 2, 3, ..., CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS。
global_work_offset(GWO), 每个维度的偏移，如果不设置默认为0
global_work_size(GWS)，每个维度的索引长度，GWS(1) * GWS(2) * ... * GWS(N) 应该大于等于需要处理的数据量
local_work_size(LWS), 每个维度work-group的大小，如果不设置，系统会自己选择一个合适的大小
num_events_in_wait_list: 执行kernel前需要等待的event个数
event_wait_list: 需要等待的event列表
event: 当前这个命令会返回一个event，以供后面的命令进行同步
返回：
函数返回执行状态。如果成功， 返回CL_SUCCESS
*/

```
运行实例：

```c
// 设置kernel的参数 Enqueuing parameters
// Note that we inform the size of the cl_mem object, not the size of the memory pointed by it
error = clSetKernelArg(vector_add_k, 0, sizeof(cl_mem), &src_a_d);
error |= clSetKernelArg(vector_add_k, 1, sizeof(cl_mem), &src_b_d);
error |= clSetKernelArg(vector_add_k, 2, sizeof(cl_mem), &res_d);
error |= clSetKernelArg(vector_add_k, 3, sizeof(size_t), &size);
assert(error == CL_SUCCESS);

// 启动kernel Launching kernel
const size_t local_ws = 512;    // Number of work-items per work-group
// shrRoundUp returns the smallest multiple of local_ws bigger than size
const size_t global_ws = shrRoundUp(local_ws, size);    // Total number of work-items
error = clEnqueueNDRangeKernel(queue, vector_add_k, 1, NULL, &global_ws, &local_ws, 0, NULL, NULL);
assert(error == CL_SUCCESS);


```


一个主机要使得内核运行在设备上，必须要有一个上下文来与设备进行交互。 一个上下文就是一个抽象的容器，管理在设备上的内存对象，跟踪在设备上 创建的程序和内核。

主机程序使用命令队列向设备提交命令，一个设备有一个命令队列，且与上下文 相关。命令队列对在设备上执行的命令进行调度。这些命令在主机程序和设备上 异步执行。执行时，命令间的关系有两种模式：(1)顺序执行，(2)乱序执行。

内核的执行和提交给一个队列的内存命令会生成事件对象，可以用来控制命令的执行、协调宿主机和设备的运行。

有3种命令类型：

	• Kernel-enqueue commands: Enqueue a kernel for execution on a device.（执行kernel函数）
	• Memory commands: Transfer data between the host and device memory, between memory objects, or map and unmap memory objects from the host address space.（内存传输）
	• Synchronization commands: Explicit synchronization points that define order constraints between commands.（同步点）

命令执行经历6个状态：

	Queued: 将command放到CommandQueue
	Submitted: 将command从CommandQueue提交到Device
	Ready: 当所有运行条件满足，放到Device的WorkPool里
	Running: 命令开始执行
	Ended: 命令执行结束
	Complete: command以及其子command都结束执行，并设置相关的事件状态为CL_COMPLETE


Mapping work-items onto an NDRange：

与CUDA里的grid, block, thread类似，OpenCL也有自己的work组织方式NDRange。NDRange是一个N维的索引空间(N为1, 2, 3...)，一个NDRange由三个长度为N的数组定义，与clEnqueueNDRangeKernel几个参数对应：

	global_work_size(GWS)，每个维度的索引长度，GWS(1) * GWS(2) * ... * GWS(N) 应该大于等于需要处理的数据量
	global_work_offset(GWO), 每个维度的偏移，如果不设置默认为0
	local_work_size(LWS), 每个维度work-group的大小，如果不设置，系统会自己选择较好的结果

如下图所示，整个索引空间的大小为
./cudavectoraddSUCCESScopyinputtime:15438.000000CUDAtime:23.000000copyoutputtime:17053.000000CPUtime:16259.000000resultisright!guruge@dl: /opencl/test

，每个work-group大小为

./cudavectoraddSUCCESScopyinputtime:59825.000000CUDAtime:36.000000copyoutputtime:67750.000000CPUtime:64550.000000resultisright!guruge@dl: /opencl/test

，全局偏移为。

对于一个work-item，有两种方式可以索引：

	直接使用global id 

	或者使用work-group进行相关计算，设当前group索引为，group里的local id分别为(s_x, s_y)，那么便有

### 读取结果


读取结果非常简单。与之前讲到的写入内存（设备内存）的操作相似，现在我们需要存入队列一个读取缓冲区的操作：
```c
cl_int  clEnqueueReadBuffer (cl_command_queue command_queue, 
                      cl_mem buffer,   // from which buffer
                      cl_bool blocking_read,   // whether is a blocking or non-blocking read
                      size_t offset,   // offset from the beginning
                      size_t cb,   // size to be read (in bytes)
                      void *ptr,   // pointer to the host memory
                      cl_uint num_events_in_wait_list,
                      const cl_event *event_wait_list, 
                      cl_event *event);

cl_int  clEnqueueWriteBuffer()
参数
command_queue,
buffer, 将内存写到的位置
blocking_write, 是否阻塞
offset, 从buffer的多少偏移处开始写
size, 写入buffer大小
ptr, host端buffer地址
num_events_in_wait_list, 等待事件个数
event_wait_list, 等待事件列表
event, 返回的事件


创建一个ImageBuffer:
cl_mem clCreatImage()
参数
context
flags, 同clCreateBuffer里的flags
image_format, 图像的属性，包含两个变量: image_channel_order, 指定通道数和形式，通常为RGBA；image_channel_data_type, 定义数据类型， CL_UNORM_INT8表示为unsigned规一化的INT8，CL_UNSIGNED_INT8
表示 为非规一化的unsigned int8
image_desc, 定义图像的维度大小，
host_ptr, 输入图像地址
errorce_ret, 返回状态


写一个ImageBuffer:
cl_mem clEnqueueWriteImage()
参数：
command_queue
image, 目标图像
block_writing, 是否阻塞，如果TRUE，则阻塞
origin, 图像的偏移，通常为（0, 0, 0）
region, 图像的区域，(width, height, depth)
input_row_pitch，每行字节数，可能有对齐；如果设为0,则程序根据每个像素的字节数 乘以 width 计算
input_slice_pitch，3D图像的2D slice块，如果是1D或2D图像，这个值必须为0
ptr, host端输入源图像地址
num_events_in_wait_list, 需等待事件个数
evnet_wait_list, 需要等待的事件列表
event, 返回这个命令的事件，用于后续使用


映射 buffer
void clEnqueueMapBuffer()
参数：
command_queue
buffer, cl_mem映射的源地址
blocking_map, 是否阻塞
map_flags, CL_MAP_READ，映射的地址为只读；CL_MAP_WRITE，向映射的地址写东西；CL_MAP_WRITE_INVALIDATE_REGION, 向映射的地址为写东西，host不会使用这段地址的内容，这时返回的地址处的内容不保证是最新的
offset, cl_mem的偏移
size, 映射的内存大小
num_events_in_wait_list, 等待事件个数
event_wait_list, 等待事件列表
event, 返回事件
errorcode_ret, 返回状态
返回值是CPU可访问的指针。

注意：
当flag为CL_MAP_WRITE时，如果不使用unmap进行解映射，device端无法保证可以获取到最新写的值。
如果不用unmap，那么device端无法释放这部分内存
所以写完内容后，要立马解映射。


buffer
clEnqueueCopyBuffer() //  从一个cl buffer拷贝到另一个cl buffer

事件：

cl_int clWaitForEvents(cl_uint num_events, const cl_event *event_list)
等待事件执行完成才返回，否则会阻塞

cl_int clEnqueueWaitForEvents(cl_command_queue command_queue, cl_uint num_events, const cl_event *event_list)
和 clWaitForEvents 不同的是该命令执行后会立即返回，线程可以在不阻塞的情况下接着执行其它任务。而 clWaitForEvents 会进入阻塞状态，直到事件列表 event_list 中对应的事件处于 CL_COMPLETE 状态。

cl_int clFlush(cl_command_queue command_queue)
只保证command_queue中的command被commit到相应的device上，不保证当clFlush返回时这些command已经执行完。

cl_int clFinish(cl_command_queue command_queue)
clFinish直到之前的队列命令都执行完才返回。clFinish is also a synchronization point.

cl_int clEnqueueBarrier(cl_command_queue command_queue)
屏障命令保证在后面的命令执行之前，它前面提交到命令队列的命令已经执行完成。
和 clFinish 不同的是该命令会异步执行，在 clEnqueueBarrier 返回后，线程可以执行其它任务，例如分配内存、创建内核等。而 clFinish 会阻塞当前线程，直到命令队列为空（所有的内核执行/数据对象操作已完成）。

cl_int clEnqueueMarker(cl_command_queue command_queue, cl_event *event)
将标记命令提交到命令队列 command_queue 中。当标记命令执行后，在它之前提交到命令队列的命令也执行完成。该函数返回一个事件对象 event，在它后面提交到命令队列的命令可以等待该事件。例如，随后的命令可以等待该事件以确保标记之前的命令已经执行完成。如果函数成功执行返回 CL_SUCCESS。

 ```
 
使用方法如下：

```c
// Reading back
float* check = new float[size];
clEnqueueReadBuffer(queue, res_d, CL_TRUE, 0, mem_size, check, 0, NULL, NULL);
```





清理
作为一名牛X的程序员我们肯定要考虑如何清理内存！
你需要知道最基本东西：使用clCreate申请的（缓冲区、kernel、队列）必须使用clRelease释放。
代码如下：
```c
// cpu 上内存清理
delete[] src_a_h;
delete[] src_b_h;
delete[] res_h;
delete[] check;
// gpu上内存清理
clReleaseKernel(vector_add_k);
clReleaseCommandQueue(queue);
clReleaseContext(context);
clReleaseMemObject(src_a_d);
clReleaseMemObject(src_b_d);
clReleaseMemObject(res_d);
````


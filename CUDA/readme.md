# CUDA编程
[参考](https://www.cnblogs.com/cuancuancuanhao/category/1236273.html)

[参考2](https://blog.csdn.net/fishseeker/article/details/75093166)

[参考3](https://bbs.csdn.net/topics/390798229)

[The CMake version of cuda_by_example ](https://github.com/Ewenwan/cuda_by_example)

[CUDA Program：CUDA image rgb to gray；CUDA KLT](https://github.com/canteen-man/CUDA-Program)


[CUDA 编程 加速 计算机视觉 ！！！推荐](https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA)

# CUDA存储器类型：

	每个线程拥有自己的 register寄存器 and loacal memory 局部内存
	每个线程块拥有一块 shared memory 共享内存
	所有线程都可以访问 global memory 全局内存
	
	还有，可以被所有线程访问的
	     只读存储器：
	     constant memory (常量内容) and texture memory
        
	a. 寄存器Register
	   寄存器是GPU上的高速缓存器，其基本单元是寄存器文件，每个寄存器文件大小为32bit.
	   Kernel中的局部(简单类型)变量第一选择是被分配到Register中。
           特点：每个线程私有，速度快。
	
	b. 局部存储器 local memory
	
	   当register耗尽时，数据将被存储到local memory。
	   如果每个线程中使用了过多的寄存器，或声明了大型结构体或数组，
	   或编译器无法确定数组大小，线程的私有数据就会被分配到local memory中。
	   
           特点：每个线程私有；没有缓存，慢。
           注：在声明局部变量时，尽量使变量可以分配到register。如：
           unsigned int mt[3];
           改为：　unsigned int mt0, mt1, mt2;
	
	c. 共享存储器 shared memory
           可以被同一block中的所有线程读写
           特点：block中的线程共有；访问共享存储器几乎与register一样快.
	
	d. 全局存储器 global memory
　         特点：所有线程都可以访问；没有缓存
	
	e. 常数存储器constant memory
	   用于存储访问频繁的只读参数
	   特点：只读；有缓存；空间小(64KB)
	   注：定义常数存储器时，需要将其定义在所有函数之外，作用于整个文件
	
	f. 纹理存储器 texture memory
           是一种只读存储器，其中的数据以一维、二维或者三维数组的形式存储在显存中。
	   在通用计算中，其适合实现图像处理和查找，对大量数据的随机访问和非对齐访问也有良好的加速效果。
           特点：具有纹理缓存，只读。

# 1、hello wold
```c
// 输出hello world
#include<stdio.h>

__global__ void kernel() {
  printf("hello world");
}
int main() {
  kernel<<<1, 1>>>();// 第一个1，代表线程格里只有一个线程块；第二个1，代表一个线程块里只有一个线程。
  return 0;
}
//这个程序和普通的C程序的区别值得注意
// 函数的定义带有了__global__这个标签，表示这个函数是在GPU上运行
// 函数的调用除了常规的参数之外，还增加了<<<>>>修饰。
// 调用通过<<<参数1,参数2>>>，用于说明内核函数中的线程数量，以及线程是如何组织的。
// 以线程格（Grid）的形式组织，每个 线程格 由若干个 线程块（block）组成，
// 而每个线程块又由若干个线程（thread）组成。
// 是以block为单位执行的。

```
```c
dim3结构类型
        1. dim3是基亍uint3定义的矢量类型，相当亍由3个unsigned int型组成的结构体。
           uint3类型有三个数据成员unsigned int x; unsigned int y; unsigned int z;
        2. 可使用亍一维、二维或三维的索引来标识线程，构成一维、二维或三维线程块。
        3. dim3结构类型变量用在核函数调用的<<<,>>>中。
        4. 相关的几个内置变量
        4.1. threadIdx，顾名思义获取线程thread的ID索引；
              如果线程是一维的那么就取threadIdx.x，
              二维的还可以多取到一个值threadIdx.y，
              以此类推到三维threadIdx.z。
        4.2. blockIdx，线程块的ID索引；同样有blockIdx.x，blockIdx.y，blockIdx.z。
        4.3. blockDim，线程块的维度，同样有blockDim.x，blockDim.y，blockDim.z。
        4.4. gridDim，线程格的维度，同样有gridDim.x，gridDim.y，gridDim.z。
        5. 对于一维的block，线程的threadID=threadIdx.x。
        6. 对于大小为（blockDim.x, blockDim.y）的 二维 block，
            线程的threadID=threadIdx.x+threadIdx.y*blockDim.x。
        7. 对于大小为（blockDim.x, blockDim.y, blockDim.z）的 三维 block，
           线程的threadID=threadIdx.x+threadIdx.y*blockDim.x+threadIdx.z*blockDim.x*blockDim.y。
        8. 对于计算线程索引偏移增量为已启动线程的总数。
            如stride = blockDim.x * gridDim.x; threadId += stride。
函数修饰符
        1. __global__，表明被修饰的函数在设备上执行，但在主机上调用。
        2. __device__，表明被修饰的函数在设备上执行，但只能在其他__device__函数或者__global__函数中调用。

        
```
      



# 2、CPU、GPU内存数据交换
```c

#include<stdio.h>

__global__ void add(int a,int b,int *c){
  *c = a + b;
}
int main(){
  int c;
  int *dev_c;
  
  cudaMalloc((void**)&dev_c,sizeof(int));// 在CUDA中申请内存
  
  add<<<1,1>>>(2,7,dev_c);
  cudaMemcpy(&c,dev_c,sizeof(int),cudaMemcpyDeviceToHost);
  printf("2 + 7 = %d",c);
  return 0;
}


这里就涉及了GPU和主机之间的内存交换了，cudaMalloc是在GPU的内存里开辟一片空间，
然后通过操作之后，这个内存里有了计算出来内容，再通过cudaMemcpy这个函数把内容从GPU复制出来。

常用的GPU内存函数
cudaMalloc()
        1. 函数原型： cudaError_t cudaMalloc (void **devPtr, size_t size)。
        2. 函数用处：与C语言中的malloc函数一样，只是此函数在GPU的内存你分配内存。
        3. 注意事项：
        3.1. 可以将cudaMalloc()分配的指针传递给在设备上执行的函数；
        3.2. 可以在设备代码中使用cudaMalloc()分配的指针进行设备内存读写操作；
        3.3. 可以将cudaMalloc()分配的指针传递给在主机上执行的函数；
        3.4. 不可以在主机代码中使用cudaMalloc()分配的指针进行主机内存读写操作（即不能进行解引用）。
cudaMemcpy()
        1. 函数原型：cudaError_t cudaMemcpy (void *dst, const void *src, size_t count, cudaMemcpyKind kind)。
        2. 函数作用：与c语言中的memcpy函数一样，只是此函数可以在主机内存和GPU内存之间互相拷贝数据。
        3. 函数参数：cudaMemcpyKind kind表示数据拷贝方向，如果kind赋值为cudaMemcpyDeviceToHost表示数据从设备内存拷贝到主机内存。
        4. 与C中的memcpy()一样，以同步方式执行，即当函数返回时，复制操作就已经完成了，并且在输出缓冲区中包含了复制进去的内容。
        5. 相应的有个异步方式执行的函数cudaMemcpyAsync()，这个函数详解请看下面的流一节有关内容。
cudaFree()
        1. 函数原型：cudaError_t cudaFree ( void* devPtr )。
        2. 函数作用：与c语言中的free()函数一样，只是此函数释放的是cudaMalloc()分配的内存。
        下面实例用于解释上面三个函数
        
```



# 3、CUDA 设备 
```c
#include <stdio.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "D:\Code\CUDA\book\common\book.h" 

int main(void)
{
    cudaDeviceProp  prop;                                   //放置设备属性参数的结构体
    int dev_count, dev_id;
    
    HANDLE_ERROR(cudaGetDeviceCount(&dev_count));           //获取设备数量
    printf("Count of CUDA devices:\n\t%d\n",dev_count);

    HANDLE_ERROR(cudaGetDevice(&dev_id));                   //获取当前设备编号
    printf("ID of current CUDA device:\n\t%d\n", dev_id);

    HANDLE_ERROR(cudaGetDeviceProperties(&prop, dev_id));   //将指定变红的设备的属性放入prop中
    printf("Name the CUDA device:\n\t%s\n", prop.name);     //调用结构成员访问prop的内容

    memset(&prop, 0, sizeof(cudaDeviceProp));               //清空prop内容
    prop.major = 6;                                         //指定prop中特定项目的值
    HANDLE_ERROR(cudaChooseDevice(&dev_id, &prop));         //依照指定寻找符合条件的首个设备
    printf("ID of CUDA device with Major compute capability 6.X:\n\t%d\n", dev_id);

    HANDLE_ERROR(cudaSetDevice(dev_id));                    //设置使用特定编号的设备
    
    getchar();
    return 0;
}


// 结构体 cudaDeviceProp 定义于 driver_types.h 中，包含以下66个参数
```

# 4、 教程

## a 基本
### 1）单个数 加法
```c
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>

// kernel function 对两个变量进行相加  传递变量=====================
__global__ void gpuAdd(int d_a, int d_b, int *d_c) 
{
	*d_c = d_a + d_b;
}

//main function
int main(void) 
{
	//Defining host variable to store answer
	int h_c; // CPU 变量
  
	//Defining device pointer
	int *d_c;// 指针指向 GPU内存地址
  
	// 分配gpu 内存存储一个 int
	cudaMalloc((void**)&d_c, sizeof(int));
	// 输入 1 and 4 使用gpu内存变量 存储结果 d_c
	//<< <1,1> >> means 1 block is executed with 1 thread per block
  // 1线程块，1个线程
	gpuAdd << <1, 1 >> > (1, 4, d_c);
  
	// 内存数据移动  GPU数据d_c 到CPU数据h_c
	cudaMemcpy(&h_c, d_c, sizeof(int), cudaMemcpyDeviceToHost);
	printf("1 + 4 = %d\n", h_c);
  
	// 清理GPU内存
	cudaFree(d_c); 
	return 0;
}


// 输入变量 以指针方式传递===================================
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>

// 输入变量 全部为指针  类似数据的引用
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c) 
{
	*d_c = *d_a + *d_b;
}

int main(void) 
{
	// CPU变量
	int h_a,h_b, h_c;
	// CPU  指针 变量 指向 GPU数据地址
	int *d_a,*d_b,*d_c;
	// 初始化CPU变量
	h_a = 1;
	h_b = 4;
  
	// 分配GPU 变量内存
	cudaMalloc((void**)&d_a, sizeof(int));
	cudaMalloc((void**)&d_b, sizeof(int));
	cudaMalloc((void**)&d_c, sizeof(int));
  
	// 输入变量 CPU 拷贝到 GPU   右 到 左
	cudaMemcpy(d_a, &h_a, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, &h_b, sizeof(int), cudaMemcpyHostToDevice);
  
	// 调用核函数
	gpuAdd << <1, 1 >> > (d_a, d_b, d_c);
  
	// 拷贝GPU数据结果 d_c 到 CPU变量
	cudaMemcpy(&h_c, d_c, sizeof(int), cudaMemcpyDeviceToHost);
	printf("Passing Parameter by Reference Output: %d + %d = %d\n", h_a, h_b, h_c);
  
	// 清理GPU内存 Free up memory 
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	return 0;
}

```

### 2）多线程 块 id  GPU设备
```c
// 多线程块，线程块id==============
#include <iostream>

#include <stdio.h>
__global__ void myfirstkernel(void) 
{
	//blockIdx.x gives the block number of current kernel
  // 打印当前所属 线程块id
	printf("Hello!!!I'm thread in block: %d\n", blockIdx.x);
}

int main(void) {
	// 16个线程块，每个线程块1个线程
	myfirstkernel << <16,1>> >();
	// 等待所有线程完成工作
	cudaDeviceSynchronize();
	printf("All threads are finished!\n");
	return 0;
}

// 多GPU设备===============================
#include <memory>
#include <iostream>
#include <cuda_runtime.h>
#include <stdio.h>

// Main Program 

int main(void)
{
	int device_Count = 0;
	cudaGetDeviceCount(&device_Count);// 获取CUDA设备数量====
	// This function returns count of number of CUDA enable devices and 0 if there are no CUDA capable devices.
	if (device_Count == 0)
	{
		printf("There are no available device(s) that support CUDA\n");
	}
	else
	{
		printf("Detected %d CUDA Capable device(s)\n", device_Count);
	}

	
}

```
### 3） CPU数组数据 加法
```c
#include "stdio.h"
#include<iostream>
// 数组元素数量
#define N	5
// 向量加法 CPU
void cpuAdd(int *h_a, int *h_b, int *h_c) 
{
	int tid = 0;	
	while (tid < N)
	{
		h_c[tid] = h_a[tid] + h_b[tid];
		tid += 1;// 数组元素index
	}
}

int main(void) 
{
	int h_a[N], h_b[N], h_c[N];
		//Initializing two arrays for addition
  // 两个加数================
	for (int i = 0; i < N; i++) 
  {
		h_a[i] = 2 * i*i;
		h_b[i] = i;
	}
	// 使用cpu函数进行相加
	cpuAdd (h_a, h_b, h_c);
	// 打印结果
	printf("Vector addition on CPU\n");
	for (int i = 0; i < N; i++) {
		printf("The sum of %d element is %d + %d = %d\n", i, h_a[i], h_b[i], h_c[i]);
	}
	return 0;
}

```

### 3） GPU 数组 数据 加法   多线程块，单线程===================

```c
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>
// 数组元素数量
#define N	5
//Defining Kernel function for vector addition
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c) {
	// Getting block index of current kernel
	int tid = blockIdx.x;	// 本线程块的id 
  // 不需要循环体 核函数本身就是被多个线程块共同执行=======
	if (tid < N)
		d_c[tid] = d_a[tid] + d_b[tid];
    
  //  使用N个线程块，每个线程块执行一个加法==============
  // 多线程块，单线程
}

int main(void)
{
	// cpu数组
	int h_a[N], h_b[N], h_c[N];
	// CPU指针数据 指向 gpu内存数据
	int *d_a, *d_b, *d_c;
  
	// 分配GPU数据
	cudaMalloc((void**)&d_a, N * sizeof(int));
	cudaMalloc((void**)&d_b, N * sizeof(int));
	cudaMalloc((void**)&d_c, N * sizeof(int));
  
	//初始化 cpu数组数据
	for (int i = 0; i < N; i++) 
  {
		h_a[i] = 2*i*i;
		h_b[i] = i ;
	}
	// CPU数据拷贝到 GPU内存
	cudaMemcpy(d_a, h_a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, N * sizeof(int), cudaMemcpyHostToDevice);
  
	// 在GPU中执行加法 使用N个线程块，每个线程块执行一个加法
	gpuAdd << <N, 1 >> >(d_a, d_b, d_c);
  
	// 从GPU中拷贝结果 到 cpu
	cudaMemcpy(h_c, d_c, N * sizeof(int), cudaMemcpyDeviceToHost);

	printf("Vector addition on GPU \n");
	// 打印结果信息
	for (int i = 0; i < N; i++) 
  {
		printf("The sum of %d element is %d + %d = %d\n", i, h_a[i], h_b[i], h_c[i]);
	}
	// 清空GPU内存
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	return 0;
}

```

### 4）数组数据 平方 GPU     多线程 ，单线程块===============

```C
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>
// 数组元素数量
#define N	5
// 执行平方
__global__ void gpuSquare(float *d_in, float *d_out) {
	//Getting thread index for current kernel
	int tid = threadIdx.x;	// 当前 线程id
	float temp = d_in[tid]; // 每个线程分配的 数组 id
	d_out[tid] = temp*temp;
  // 多线程 ，单线程块
}

int main(void) {
	// cpu数组数据
	float h_in[N], h_out[N];
	// 执行GPU内存数据
	float *d_in, *d_out;

	// 分配gpu数据
	cudaMalloc((void**)&d_in, N * sizeof(float));
	cudaMalloc((void**)&d_out, N * sizeof(float));
  
	// 初始化CPU数据
	for (int i = 0; i < N; i++) {
		h_in[i] = i;
	}
	// 拷贝CPU数据到 GPU
	cudaMemcpy(d_in, h_in, N * sizeof(float), cudaMemcpyHostToDevice);
  
	// 1个线程块，中N个线程，执行函数
	gpuSquare << <1, N >> >(d_in, d_out);
  
	// 拷贝gpu 结果 到cpu
	cudaMemcpy(h_out, d_out, N * sizeof(float), cudaMemcpyDeviceToHost);
  
	// 打印结果信息
	printf("Square of Number on GPU \n");
	for (int i = 0; i < N; i++) {
		printf("The square of %f is %f\n", h_in[i], h_out[i]);
	}
	// 情况Gpu内存===
	cudaFree(d_in);
	cudaFree(d_out);
	return 0;
}


```

### 5） 大数据量 多数据块，多线程并发执行
```c
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>

// 大数据量，大数组
#define N 50000

//Defining Kernel function for vector addition
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c)
{

	// 当前核的id 当前线程id + 块id * 每块线程数
	int tid = threadIdx.x + blockIdx.x * blockDim.x;	
	while (tid < N)
	{
		d_c[tid] = d_a[tid] + d_b[tid];
		// 更新 tid 
		// 线程格维度 gridDim.x ====== 线程块数量
	        // 每个线程块 维度blockDim.x 为单块线程数数量
		// 一次执行 一个格子 512线程块 * 512线程数量
		tid += blockDim.x * gridDim.x;
	}
		
}

int main(void) 
{
	// cpu数组
	int h_a[N], h_b[N], h_c[N];
	
	// 执行GPU内存数据
	int *d_a, *d_b, *d_c;
	
	// 分配GPU内存数据
	cudaMalloc((void**)&d_a, N * sizeof(int));
	cudaMalloc((void**)&d_b, N * sizeof(int));
	cudaMalloc((void**)&d_c, N * sizeof(int));
	
	// 初始化CPU数据
	for (int i = 0; i < N; i++) {
		h_a[i] = 2 * i*i;
		h_b[i] = i;
	}
	// cpu数据 到 GPU
	cudaMemcpy(d_a, h_a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, N * sizeof(int), cudaMemcpyHostToDevice);
	// 512个线性块，每个线程块512个线程
	gpuAdd << <512, 512 >> >(d_a, d_b, d_c);
	
	// 拷贝GPU结果 到 cpu
	cudaMemcpy(h_c, d_c, N * sizeof(int), cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();// 等待 所有线程结束====
	
	int Correct = 1;
	printf("Vector addition on GPU \n");
	
	//Printing result on console
	for (int i = 0; i < N; i++)
	{
		if ((h_a[i] + h_b[i] != h_c[i]))// 计算出错
		{
			Correct = 0;// 发生错误
		}
		
	}
	if (Correct == 1)
	{
		printf("GPU has computed Sum Correctly\n");
	}
	else
	{
		printf("There is an Error in GPU Computation\n");// 发生错误
	}
	
	// 清空GPU内存=====
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	return 0;
}


```

### 6） 多线程数据共享 核函数内部 共享数据 __shared__  可以被同一block中的所有线程读写
```c
#include <stdio.h>

__global__ void gpu_shared_memory(float *d_a)
{
	// 当前线程id    局部数据，线程直接不共享
	int i, index = threadIdx.x;
	float average, sum = 0.0f;
	
	// 共享数据 ，多线程之间 贡献
	// 可以被同一block中的所有线程读写
	__shared__ float sh_arr[10];

	// 共享数据，记录数组数据
	sh_arr[index] = d_a[index];

	__syncthreads();    // 确保共享数据写入完成

	for (i = 0; i<= index; i++) 
	{ 
		sum += sh_arr[i]; // 数组累加
	}
	average = sum / (index + 1.0f);//求平均值

	d_a[index] = average; // 输出值 记录平均值

	sh_arr[index] = average;// 共享数据也被替换
}

int main(int argc, char **argv)
{
	// CPU数据
	float h_a[10];   
	// 指向GPU内存数据
	float *d_a;       
        // 赋值
	for (int i = 0; i < 10; i++)
	{
		h_a[i] = i;
	}
	// 分配gpu内存
	cudaMalloc((void **)&d_a, sizeof(float) * 10);
	// cpu数据到gpu数据
	cudaMemcpy((void *)d_a, (void *)h_a, sizeof(float) * 10, cudaMemcpyHostToDevice);
	
        // 一个线程块，10个线程 执行
	gpu_shared_memory << <1, 10 >> >(d_a);
	// 拷贝结果 gpu  到 cpu
	cudaMemcpy((void *)h_a, (void *)d_a, sizeof(float) * 10, cudaMemcpyDeviceToHost);
	printf("Use of Shared Memory on GPU:  \n");
	//Printing result on console
	for (int i = 0; i < 10; i++) {
		printf("The running average after %d element is %f \n", i, h_a[i]);
	}
	// gpu内存清理
	return 0;
}

```

### 7) 多线程块 多线程计算 未使用 原子操作 atomic 加法
```c
#include <stdio.h>


#define NUM_THREADS 10000 // 总线程数
#define SIZE  10// 数组大小

#define BLOCK_WIDTH 100   // 每个线程块的线程数

__global__ void gpu_increment_without_atomic(int *d_a)
{
	// 计算当前总线程id  当前块id*块维度 + 当前线程id
	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	// each thread increments elements wrapping at SIZE variable
	tid = tid % SIZE;// 取余数组大小
	d_a[tid] += 1;   // 对应index元素自增1，可记录每个对应的线程总共执行了多少次
}

int main(int argc, char **argv)
{

	printf("%d total threads in %d blocks writing into %d array elements\n",
		NUM_THREADS, NUM_THREADS / BLOCK_WIDTH, SIZE);

	// 定义CPU数据
	int h_a[SIZE]; // 数组
	const int ARRAY_BYTES = SIZE * sizeof(int);// 总字节数

	// 定义GPU数据，并分配内存，进行初始化
	int * d_a;//cpu指针 指向GPU数据地址
	cudaMalloc((void **)&d_a, ARRAY_BYTES);// GPU上分配内存
	cudaMemset((void *)d_a, 0, ARRAY_BYTES);// 数据初始化为0
        
	// 无原子操作 自加法                线程块数量                 每块线程数数量  
	gpu_increment_without_atomic << <NUM_THREADS / BLOCK_WIDTH, BLOCK_WIDTH >> >(d_a);

	// 拷贝GPU结果到 CPU数据
	cudaMemcpy(h_a, d_a, ARRAY_BYTES, cudaMemcpyDeviceToHost);

	printf("Number of times a particular Array index has been incremented without atomic add is: \n");
	for (int i = 0; i < SIZE; i++)
	{
		printf("index: %d --> %d times\n ", i, h_a[i]);// 每个数组元素被执行 自增运算次数，无原子操作
	}
        // 清空GPU内存
	cudaFree(d_a);
	return 0;
}


```


### 8) 多线程块 多线程计算 使用 原子操作 atomic  加法  atomicAdd()
```c
#include <stdio.h>


#define NUM_THREADS 10000 // 总线程数
#define SIZE  10// 数组大小

#define BLOCK_WIDTH 100   // 每个线程块的线程数

__global__ void gpu_increment_without_atomic(int *d_a)
{
	// 计算当前总线程id  当前块id*块维度 + 当前线程id
	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	// each thread increments elements wrapping at SIZE variable
	tid = tid % SIZE;// 取余数组大小
	// d_a[tid] += 1;   // 对应index元素自增1，可记录每个对应的线程总共执行了多少次
	atomicAdd(&d_a[tid], 1); // 原子add操作 每个元素 加上第二个参数 1 ------------------
}

int main(int argc, char **argv)
{

	printf("%d total threads in %d blocks writing into %d array elements\n",
		NUM_THREADS, NUM_THREADS / BLOCK_WIDTH, SIZE);

	// 定义CPU数据
	int h_a[SIZE]; // 数组
	const int ARRAY_BYTES = SIZE * sizeof(int);// 总字节数

	// 定义GPU数据，并分配内存，进行初始化
	int * d_a;//cpu指针 指向GPU数据地址
	cudaMalloc((void **)&d_a, ARRAY_BYTES);// GPU上分配内存
	cudaMemset((void *)d_a, 0, ARRAY_BYTES);// 数据初始化为0
        
	// 无原子操作 自加法                线程块数量                 每块线程数数量  
	gpu_increment_without_atomic << <NUM_THREADS / BLOCK_WIDTH, BLOCK_WIDTH >> >(d_a);

	// 拷贝GPU结果到 CPU数据
	cudaMemcpy(h_a, d_a, ARRAY_BYTES, cudaMemcpyDeviceToHost);

	printf("Number of times a particular Array index has been incremented with atomic add is: \n");
	for (int i = 0; i < SIZE; i++)
	{
		printf("index: %d --> %d times\n ", i, h_a[i]);// 每个数组元素被执行 自增运算次数，无原子操作
	}
        // 清空GPU内存
	cudaFree(d_a);
	return 0;
}


```

### 9) GPU常量只读  __constant__ ，缩放+平移操作 ，单线程块，多线程,
```c
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>

// 定义两个 GPU int常量 
__constant__ int constant_f;
__constant__ int constant_g;

// 数组大小
#define N	5

// 使用GPU常量的 缩放+平移操作 核函数
__global__ void gpu_constant_memory(float *d_in, float *d_out) 
{
	// 单线程块，多线程，当前线程id ==== 总线程id
	int tid = threadIdx.x;	
	//  使用GPU常量的 缩放+平移操作 核函数
	d_out[tid] = constant_f*d_in[tid] + constant_g;
}

int main(void) 
{
	// cpu 数组变量
	float h_in[N], h_out[N];
	
	// 准备GPU 数据
	float *d_in, *d_out; // cpu指针数据，指向GPU数据数据地址
	int h_f = 2;   // 为GPU 长亮数据准备的 对应的CPU数据
	int h_g = 20;
	
	// 分配GPU 数组数据
	cudaMalloc((void**)&d_in, N * sizeof(float));
	cudaMalloc((void**)&d_out, N * sizeof(float));
	
	// 初始化CPU数组数据
	for (int i = 0; i < N; i++) {
		h_in[i] = i;
	}
	// 拷贝cpu数组变量h_in 到 gpu数组数据 d_in
	cudaMemcpy(d_in, h_in, N * sizeof(float), cudaMemcpyHostToDevice);
	
	// 拷贝cpu变量 h_f,  到   GPU常量 constant_f
	cudaMemcpyToSymbol(constant_f, &h_f, sizeof(int),0,cudaMemcpyHostToDevice);
	cudaMemcpyToSymbol(constant_g, &h_g, sizeof(int));

	// 调用核函数，使用1个线程块，内部执行N个线程
	gpu_constant_memory << <1, N >> >(d_in, d_out);
	
	// 拷贝GPU中的结果 到 CPU
	cudaMemcpy(h_out, d_out, N * sizeof(float), cudaMemcpyDeviceToHost);
	
	// 打印结果
	printf("Use of Constant memory on GPU \n");
	for (int i = 0; i < N; i++) 
	{
		printf("The expression for input %f is %f\n", h_in[i], h_out[i]);
	}
	
	// 清空GPU内存 。。。。常量会自动清理???
	cudaFree(d_in);
	cudaFree(d_out);
	return 0;
}


```

### 10) 纹理存储器 texture memory，只读，常数组数据，适合实现图像处理和查找
```c
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#define NUM_THREADS 10  // 一个块具有的线程数
#define N 10            // 数组元素数量，总需要计算的次数

// 纹理内存数据 只读，一维 纹理数据，类似 寄存器功能，加强运算速度
texture <float, 1, cudaReadModeElementType> textureRef;

__global__ void gpu_texture_memory(int n, float *d_out)
{
        // 当前总线程id  = 块id*块维度 + 块线程id
	int idx = blockIdx.x*blockDim.x + threadIdx.x;
	
	if (idx < n) 
	{
	// tex1D ,一维 纹理数据
		float temp = tex1D(textureRef, float(idx));// 取出 纹理数据中的 数据
		d_out[idx] = temp;
	}
}

int main()
{
	// 计算需要的线程块 数量
	int num_blocks = N / NUM_THREADS + ((N % NUM_THREADS) ? 1 : 0);
	
	// 准备GPU数据
	float *d_out; //cpu 指针数据，指向 GPU数据内存
	// 分配GPU内存 存储输出
	cudaMalloc((void**)&d_out, sizeof(float) * N);
	
	// 分配CPU数据存储 输出
	float *h_out = (float*)malloc(sizeof(float)*N);
	
	// 初始化CPU数据，存储输入数据
	float h_in[N];
	for (int i = 0; i < N; i++) {
		h_in[i] = float(i);
	}
	
	// 定义GPU 数组变量 
	cudaArray *cu_Array;
        // GPU分配数组数据
	cudaMallocArray(&cu_Array, &textureRef.channelDesc, N, 1);
	
	// 拷贝CPU数组数据 到 GPU数组类型数据 
	cudaMemcpyToArray(cu_Array, 0, 0, h_in, sizeof(float)*N, cudaMemcpyHostToDevice);
	
	// 将CPU数组数据 绑定到 纹理内存数据类型
	cudaBindTextureToArray(textureRef, cu_Array);
	
	// 调用核函数  线程块，单位线程数
  	gpu_texture_memory << <num_blocks, NUM_THREADS >> >(N, d_out);
	
	// 拷贝GPU结果数据 到 CPU
	cudaMemcpy(h_out, d_out, sizeof(float)*N, cudaMemcpyDeviceToHost);
	
	// 打印
	printf("Use of Texture memory on GPU: \n");
	for (int i = 0; i < N; i++) {
		printf("Texture element at %d is : %f\n",i, h_out[i]);
	}
	
	// 清理内存
	free(h_out);      // 清理cpu内存
	cudaFree(d_out);  // 清理Gpu内存，输出
	cudaFreeArray(cu_Array);// 清理Gpu 数组内存 输入
	cudaUnbindTexture(textureRef);// 取消 纹理数据内存 绑定
	
}


```

### 10)
```c



```

### 11)
```c



```

### 9)
```c



```

### 9)
```c



```

### 9)
```c



```

### 9)
```c



```

### 9)
```c



```

### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```

### 9)
```c



```

### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```


### 9)
```c



```

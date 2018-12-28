# CUDA编程
[参考](https://www.cnblogs.com/cuancuancuanhao/category/1236273.html)

[参考2](https://blog.csdn.net/fishseeker/article/details/75093166)

[参考3](https://bbs.csdn.net/topics/390798229)

[The CMake version of cuda_by_example ](https://github.com/Ewenwan/cuda_by_example)

[CUDA Program：CUDA image rgb to gray；CUDA KLT](https://github.com/canteen-man/CUDA-Program)

[CUDA 编程 加速 计算机视觉 ！！！推荐](https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA)

[并行编程 CUDA openMP 等](http://heather.cs.ucdavis.edu/~matloff/158/PLN/ParProcBook.pdf)

[并行编程 CUDA openMP 等 中文版](https://github.com/thirdwing/ParaBook)

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

# threadIdx，blockIdx, blockDim, gridDim之间的区别与联系

     在启动kernel的时候，要通过指定gridsize和blocksize才行
     dim3 gridsize(2,2);   // 2行*2列*1页 形状的线程格，也就是说 4个线程块
        gridDim.x，gridDim.y，gridDim.z相当于这个dim3的x，y，z方向的维度，这里是2*2*1。
	序号从0到3，且是从上到下的顺序，就是说是下面的情况:
	 具体到 线程格 中每一个 线程块的 id索引为：
	 
        grid 中的 blockidx 序号标注情况为：      0     2 
                                               1     3
					       
       dim3 blocksize(4,4);  // 线程块的形状，4行*4列*1页，一个线程块内部共有 16个线程
     
       blockDim.x，blockDim.y，blockDim.z相当于这个dim3的x，y，z方向的维度，
       这里是4*4*1.序号是0-15，也是从上到下的标注：

	block中的 threadidx 序号标注情况   0       4       8      12 
					  1       5       9      13
					  2       6       10     14
					  3       7       11     15
	1.  1维格子，1维线程块，N个线程======

	 实际的线程id tid =  blockidx.x * blockDim.x + threadidx.x
	 块id   0 1 2 3 
	 线程id 0 1 2 3 4
	2. 1维格子，2D维线程块
	 块id   1 2 3 
	 线程id  0  2
	        1  3
			       块id           块线程总数
	 实际的线程id tid =  blockidx.x * blockDim.x * blockDim.y + 
			       当前线程行数    每行线程数
			    threadidx.y * blockDim.x   + 
			       当前线程列数
			    threadidx.x



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

### 10) 向量点乘 ，使用共享内存实现
```c
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>

#define N 1024 //  向量/数组 元素数量
#define threadsPerBlock 512 // 每个线程块，分配的线程数


__global__ void gpu_dot(float *d_a, float *d_b, float *d_c) {

	// 定义 共享内存变量，每个线程格 共享====================
	__shared__ float partial_sum[threadsPerBlock];
	
	// 当前 总线程id
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	
	// 当前线程格内 线程id
	int index = threadIdx.x;
	//Calculate Partial Sum
	float sum = 0;
	while (tid < N) 
	{
		sum += d_a[tid] * d_b[tid];// 相乘后加和
		tid += blockDim.x * gridDim.x;//一次执行一个线程格
		// 所有线程格中 某一个对应位置的线程计算的和
		// 但是不同线程格，该 共享数据不共享
		// 所以上面 得到的 也只是 每个线程格内 每个线程 计算得到的结果=====
	}

	// 每个对应位置的线程，在所有线程格中的计算和
	partial_sum[index] = sum;

	// 等待所有线程计算结束
	__syncthreads();


	// 计算一个线程块内所有线程的和=================
	int i = blockDim.x / 2; // 线程块维度/2
	while (i != 0) 
	{
	        // 右半部分 对应位置 叠加到 左半部分
		if (index < i)
			partial_sum[index] += partial_sum[index + i];
			
		__syncthreads();
		i /= 2;// 折半
	}
	
	// 在每个线程块一开始时，上述 partial_sum 刚好计算完成，partial_sum[0]保存了一个线程块内所有线程计算的和
	if (index == 0)
		d_c[blockIdx.x] = partial_sum[0]; //  每个线性块，计算得到结果
}


int main(void) 
{
	// 定义cpu变量数组 等
	float *h_a, *h_b, h_c, *partial_sum;
	
	// 定义GPU变量
	float *d_a, *d_b, *d_partial_sum;
	
	// 计算总线程块数量
	int block_calc = (N + threadsPerBlock - 1) / threadsPerBlock;
	
	// 每个线程格具有的线程块数量，线程格数
	int blocksPerGrid = (32 < block_calc ? 32 : block_calc);
	
	// 在CPU上分配 内存
	h_a = (float*)malloc(N * sizeof(float)); // 1*N 浮点型数组
	h_b = (float*)malloc(N * sizeof(float)); // 1*N 浮点型数组
	
	// 是以线程块为执行单位的
	partial_sum = (float*)malloc(blocksPerGrid * sizeof(float));// 每个线程格执行的 结果

	// 在GPU上分配内存
	cudaMalloc((void**)&d_a, N * sizeof(float));// 1*N 浮点型数组
	cudaMalloc((void**)&d_b, N * sizeof(float));
	cudaMalloc((void**)&d_partial_sum, blocksPerGrid * sizeof(float));// 每个线程格执行的 结果

	// 填充CPU数据，两个向量
	for (int i = 0; i<N; i++) {
		h_a[i] = i;
		h_b[i] = 2;  // 扩大两倍
	}
        
	// cpu输入数组 复制到 GPU数据
	cudaMemcpy(d_a, h_a, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, N * sizeof(float), cudaMemcpyHostToDevice);
	
	// 线程格，线程数    
	gpu_dot << <blocksPerGrid, threadsPerBlock >> >(d_a, d_b, d_partial_sum);

	// 拷贝GPU数据 到 cpu
	cudaMemcpy(partial_sum, d_partial_sum, blocksPerGrid * sizeof(float), cudaMemcpyDeviceToHost);

	// 对每个线程格计算的结果在此求和
	h_c = 0;
	for (int i = 0; i<blocksPerGrid; i++) 
	{
		h_c += partial_sum[i]; // 求和
	}
	
	// 打印最终的结果
	printf("The computed dot product is: %f\n", h_c);
	
// 验证计算结果是否正确 
#define cpu_sum(x) (x*(x+1))
	if (h_c == cpu_sum((float)(N - 1)))
	{
	// 计算过程为 (0+2+...+ N-1)*2 = (N-1)*N
	// 可以尝试 1+ 2^2 + 3^2 + ... + N^2 = N(N+1)(2*N+1)/6
	// 1+ 2^3 + 3^3 + ... + N^3 = (N(N+1)/2)^2
		printf("The dot product computed by GPU is correct\n");
	}
	else
	{
		printf("Error in dot product computation");
	}
	
	// 清理gpu内存
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_partial_sum);
	// 清理cpu内存
	free(h_a);
	free(h_b);
	free(partial_sum);
}


```

### 11)  Matrix multiplication 矩阵乘法， 有/无 共享内存 实现
```c
//Matrix multiplication using shared and non shared kernal
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <math.h>
#define TILE_SIZE 2 // 线程块维度  2*2
                    // 矩阵维度 4*4
		    // 线程格 维度 2*2

// 不使用共享内存实现 Matrix multiplication 矩阵乘法====
__global__ void gpu_Matrix_Mul_nonshared(float *d_a, float *d_b, float *d_c, const int size)
{
	int row, col;
	// 目标矩阵位置=========
	//  块一行线程数 * 块所述列id  + 当前块内 线程 列id
	col = TILE_SIZE * blockIdx.x + threadIdx.x; // 线程 列id
	row = TILE_SIZE * blockIdx.y + threadIdx.y; // 线程 行id

	for (int k = 0; k< size; k++)// 4次 1*4  与 4*1 向量共四次 乘+运算
	{
	//  ??????   结果矩阵位置              A                B
		d_c[row*size + col] += d_a[row * size + k] * d_b[k * size + col];
	}
	
//                      A              *             B                =       C
//                                                                          col 
//    [row*size+0 ][.][.][row*size+3]     [0*size+col ][ ][ ][ ]         [row*size+col][ ][ ][ ] 
//    [ ][ ][ ][ ]                        [1*size+col ][ ][ ][ ]    row  [ ][ ][ ][ ] 
//    [ ][ ][ ][ ]                        [2*size+col ][ ][ ][ ]         [ ][ ][ ][ ] 
//    [ ][ ][ ][ ]                        [3*size+col ][ ][ ][ ]         [ ][ ][ ][ ] 

	
}

// 不使用共享内存实现 Matrix multiplication 矩阵乘法=====
__global__ void gpu_Matrix_Mul_shared(float *d_a, float *d_b, float *d_c, const int size)
{
	int row, col;
	// 线程块共享内存=====存储矩阵A和矩阵B的数据==
	__shared__ float shared_a[TILE_SIZE][TILE_SIZE]; // 2*2
	__shared__ float shared_b[TILE_SIZE][TILE_SIZE]; // 2*2
	
	// 目标矩阵位置=========
	// 块一行线程数 * 块所述列id  + 当前块内 线程 列id
	col = TILE_SIZE * blockIdx.x + threadIdx.x;
	row = TILE_SIZE * blockIdx.y + threadIdx.y;

	for (int i = 0; i< size / TILE_SIZE; i++) // 4/2=2 一个线程块内 2次
	{
		shared_a[threadIdx.y][threadIdx.x] = d_a[row* size + (i*TILE_SIZE + threadIdx.x)];   
		                                                      // 一行元素 k 计算不同了 
		shared_b[threadIdx.y][threadIdx.x] = d_b[(i*TILE_SIZE + threadIdx.y) * size + col];  
		                                                  // 一列元素
		
		__syncthreads(); 
		
		for (int j = 0; j<TILE_SIZE; j++)// 每个线程求和  2次
			d_c[row*size + col] += shared_a[threadIdx.x][j] * shared_b[j][threadIdx.y];
			
		__syncthreads(); 

	}
}

// main routine
int main()
{
	const int size = 4; //矩阵维度
	
	// 定义二维矩阵 cpu==============
	float h_a[size][size], h_b[size][size],h_result[size][size];
	
	// 定义GPU 数据 指针=============
	float *d_a, *d_b, *d_result; // cpu指针变量 指向 GPU数据地址
	
	// 初始化 cpu 两个二维数组变量====
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			h_a[i][j] = i;
			h_b[i][j] = j;
		}
	}
        
	// 分配GPU变量
	cudaMalloc((void **)&d_a,      size*size * sizeof(int));
	cudaMalloc((void **)&d_b,      size*size * sizeof(int));
	cudaMalloc((void **)&d_result, size*size * sizeof(int));


	// 从CPU 拷贝两个输入矩阵到 GPU
	cudaMemcpy(d_a, h_a, size*size* sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, size*size* sizeof(int), cudaMemcpyHostToDevice);
	
	// 定义GPU 使用情况
	dim3 dimGrid(size / TILE_SIZE, size / TILE_SIZE, 1); // 2D 线程格 每格 有 (size / TILE_SIZE)^2 个线程块
	dim3 dimBlock(TILE_SIZE, TILE_SIZE, 1);              // 2D 线程块 每块有 TILE_SIZE*TILE_SIZE 个线程 2*2
	
	// 不使用共享内存实现
	//gpu_Matrix_Mul_nonshared << <dimGrid, dimBlock >> > (d_a, d_b, d_result, size);
	
        // 使用共享内存实现
	gpu_Matrix_Mul_shared << <dimGrid, dimBlock >> > (d_a, d_b, d_result, size);
        
	// 从 GPU 拷贝 结果数据 到 CPU
	cudaMemcpy(h_result, d_result, size*size * sizeof(int),	cudaMemcpyDeviceToHost);
	
	// 打印结果
	printf("The result of Matrix multiplication is: \n");
	for (int i = 0; i< size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%f   ", h_result[i][j]);
		}
		printf("\n");
	}
	
	// 清空GPU内存====
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_result);
	return 0;
}


```


## b  Event事件记录、错误处理、stream异步、rank_sort排列排序算法、histogram直方图计算

### 1) CUDA使用 Event 进行程序计时
```c
#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>
// 向量元素数量
#define N	50000

// 向量加法
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c) 
{
	// 1D线程格  1D线程块
	int tid = threadIdx.x + blockIdx.x * blockDim.x;// 总线程id
	while (tid < N)
	{
		d_c[tid] = d_a[tid] + d_b[tid]; // 指向加法
		tid += blockDim.x * gridDim.x;  // 一次执行一个线程格子??
	}

}

int main(void)
{
	// 定义CPU向量 1维数组
	int h_a[N], h_b[N], h_c[N];
	// 定义CPU指针变量 指向GPU数据地址
	int *d_a, *d_b, *d_c;
	
	// cudaEvent 变量，计时====
	cudaEvent_t e_start, e_stop;
	cudaEventCreate(&e_start);// 创建变量
	cudaEventCreate(&e_stop);
	cudaEventRecord(e_start, 0); // 开始记录
	
	// 分配GPU内存
	cudaMalloc((void**)&d_a, N * sizeof(int));
	cudaMalloc((void**)&d_b, N * sizeof(int));
	cudaMalloc((void**)&d_c, N * sizeof(int));
	
	// 初始化CPU输入向量
	for (int i = 0; i < N; i++)
	{
		h_a[i] = 2 * i*i;
		h_b[i] = i;
	}
	
	//  拷贝 CPU输入向量 到GPU
	cudaMemcpy(d_a, h_a, N * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, N * sizeof(int), cudaMemcpyHostToDevice);
	
	// 512个线程格，每个线程格512个线程，来执行核函数
	gpuAdd << <512, 512 >> >(d_a, d_b, d_c);
	
	// 拷贝GPU结果 到 CPU
	cudaMemcpy(h_c, d_c, N * sizeof(int), cudaMemcpyDeviceToHost);
	
	//  事件记录 
	cudaDeviceSynchronize();// 线程同步
	cudaEventRecord(e_stop, 0);// 事件记录
	cudaEventSynchronize(e_stop);// 事件记录 同步
	
	// 计算 时间间隔
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, e_start, e_stop);// 计算 时间间隔
	printf("Time to add %d numbers: %3.1f ms\n",N, elapsedTime);// 打印 时间间隔

        // 验证计算是否出错=========================
	int Correct = 1;
	printf("Vector addition on GPU \n");
	for (int i = 0; i < N; i++) 
	{
		if ((h_a[i] + h_b[i] != h_c[i]))
		{
			Correct = 0;
		}

	}
	if (Correct == 1)
	{
		printf("GPU has computed Sum Correctly\n");
	}
	else
	{
		printf("There is an Error in GPU Computation\n");
	}
	
	
	// 清空GPU内存
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	return 0;
}


```

### 2) 错误处理 error_handling  
```c
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
// 两个数相乘====
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c)
{
	*d_c = *d_a + *d_b;
}

int main()
{
    // 定义CPU变量 host variables
    int h_a, h_b, h_c;
    // 定义cpu指针变量 指向 gpu数据地址
    int *d_a, *d_b, *d_c;
    // 初始话cpu数据
    h_a = 1;
    h_b = 4;
    
    // cuda 错误变量
    cudaError_t cudaStatus;
	  
    //分配输出变量 的GPU内存  .
    cudaStatus = cudaMalloc((void**)&d_c, sizeof(int));
    if (cudaStatus != cudaSuccess) 
    {
        fprintf(stderr, "cudaMalloc failed!");// 内存分配错误
        goto Error;
    }
    // 分配两个输入变量 的GPU内存 
    cudaStatus = cudaMalloc((void**)&d_a, sizeof(int));
    if (cudaStatus != cudaSuccess) 
    {
        fprintf(stderr, "cudaMalloc failed!");// 内存分配错误
        goto Error;
    }
    cudaStatus = cudaMalloc((void**)&d_b, sizeof(int));
    if (cudaStatus != cudaSuccess) 
    {
        fprintf(stderr, "cudaMalloc failed!");// 内存分配错误
        goto Error;
    }

    // 拷贝两个CPU变量 到 GPU
    cudaStatus = cudaMemcpy(d_a,&h_a, sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) 
    {
        fprintf(stderr, "cudaMemcpy failed!");// 内存拷贝错误
        goto Error;
    }
    cudaStatus = cudaMemcpy(d_b, &h_b, sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");// 内存拷贝错误
        goto Error;
    }

    // 调用核函数计算
    gpuAdd<<<1, 1>>>(d_a, d_b, d_c);

    // 检测核函数执行后的状态
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) 
    {
        // 核函数执行 错误======
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // 从GPU中 拷贝结果 到CPU中
    cudaStatus = cudaMemcpy(&h_c, d_c, sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy failed!");//  内存拷贝错误
        goto Error;
    }
    
    printf("Passing Parameter by Reference Output: %d + %d = %d\n", h_a, h_b, h_c);
    
// 错误分支
Error:
    // 清空GPU内存
    cudaFree(d_c);
    cudaFree(d_a);
    cudaFree(d_b);
    
    return 0;
}

```

### 3) Cuda stream 是指一堆异步的cuda操作
	Cuda stream是指一堆异步的cuda操作，他们按照host代码调用的顺序执行在device上。
	Stream维护了这些操作的顺序，并在所有预处理完成后允许这些操作进入工作队列，
	同时也可以对这些操作进行一些查询操作。这些操作包括host到device的数据传输，
	launch kernel以及其他的host发起由device执行的动作。这些操作的执行总是异步的，
	cuda runtime会决定这些操作合适的执行时机。我们则可以使用相应的
	cuda api来保证所取得结果是在所有操作完成后获得的。
	同一个stream里的操作有严格的执行顺序，不同的stream则没有此限制。

	由于不同stream的操作是异步执行的，就可以利用相互之间的协调来充分发挥资源的利用率。
	
	https://www.cnblogs.com/1024incn/p/5891051.html
	
```c
// https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA/blob/master/Chapter4/03_cuda_streams.cu

#include "stdio.h"
#include<iostream>
#include <cuda.h>
#include <cuda_runtime.h>

// 向量大小
#define N    50000

// 向量加法核函数
__global__ void gpuAdd(int *d_a, int *d_b, int *d_c)
{
	//总线程id = 当前块线程id.x + 块id*块维度x
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	while (tid < N)
	{
		d_c[tid] = d_a[tid] + d_b[tid];// 加法
		tid += blockDim.x * gridDim.x; // 一次执行一个格子 块维度x*格子维度x
	}

}

int main(void) 
{
	// 定义CPU数据
	int *h_a, *h_b, *h_c;
	
	// GPU stream 0 变量数据指针
	int *d_a0, *d_b0, *d_c0;
	// GPU stream 1 变量数据指针
	int *d_a1, *d_b1, *d_c1;
	
        // cudaStream 变量
	cudaStream_t stream0, stream1;
	cudaStreamCreate(&stream0);
	cudaStreamCreate(&stream1);
	
	// cudaEvent 变量 计时
	cudaEvent_t e_start, e_stop;
	cudaEventCreate(&e_start);
	cudaEventCreate(&e_stop);
	cudaEventRecord(e_start, 0);// 启动计时

        // 分配cpu 内存?? 对应两个 cudaStream   大小为两倍
	cudaHostAlloc((void**)&h_a, N *2* sizeof(int), cudaHostAllocDefault);
	cudaHostAlloc((void**)&h_b, N *2* sizeof(int), cudaHostAllocDefault);
	cudaHostAlloc((void**)&h_c, N *2*sizeof(int),  cudaHostAllocDefault);


	// 分配两个 cudaStream  对应的变量的 gpu中的内存
	cudaMalloc((void**)&d_a0, N * sizeof(int));
	cudaMalloc((void**)&d_b0, N * sizeof(int));
	cudaMalloc((void**)&d_c0, N * sizeof(int));
	cudaMalloc((void**)&d_a1, N * sizeof(int));
	cudaMalloc((void**)&d_b1, N * sizeof(int));
	cudaMalloc((void**)&d_c1, N * sizeof(int));
	
	// 初始化cpu数据
	for (int i = 0; i < N*2; i++) 
	{
		h_a[i] = 2 * i*i;
		h_b[i] = i;
	}
	
	// cpu数据 复制到 指定stream的 gpu中，
	cudaMemcpyAsync(d_a0, h_a , N * sizeof(int), cudaMemcpyHostToDevice, stream0);
	cudaMemcpyAsync(d_a1, h_a+ N, N * sizeof(int), cudaMemcpyHostToDevice, stream1);
	cudaMemcpyAsync(d_b0, h_b , N * sizeof(int), cudaMemcpyHostToDevice, stream0);
	cudaMemcpyAsync(d_b1, h_b + N, N * sizeof(int), cudaMemcpyHostToDevice, stream1);
	
	//调用核函数，512块，*512线程，传入 stream 指针
	gpuAdd << <512, 512, 0, stream0 >> > (d_a0, d_b0, d_c0);
	gpuAdd << <512, 512, 0, stream1 >> > (d_a1, d_b1, d_c1);
	
	// 拷贝GPU结果到 cpu
	cudaMemcpyAsync(h_c , d_c0, N * sizeof(int), cudaMemcpyDeviceToHost, stream0);
	cudaMemcpyAsync(h_c + N, d_c1, N * sizeof(int), cudaMemcpyDeviceToHost, stream0);
	
	// 设备同步 和 stream 同步，等待执行结束
	cudaDeviceSynchronize();
	cudaStreamSynchronize(stream0);
	cudaStreamSynchronize(stream1);
	
	// EventRecord 计时
	cudaEventRecord(e_stop, 0);
	// 等待 EventRecord结束
	cudaEventSynchronize(e_stop);
	
	// 计算处理时间
	float elapsedTime;
	cudaEventElapsedTime(&elapsedTime, e_start, e_stop);
	printf("Time to add %d numbers: %3.1f ms\n",2* N, elapsedTime);

        // 验证计算结果
	int Correct = 1;
	printf("Vector addition on GPU \n");
	//Printing result on console
	for (int i = 0; i < 2*N; i++) 
	{
		if ((h_a[i] + h_b[i] != h_c[i]))
		{
			Correct = 0;
		}

	}
	if (Correct == 1)
	{
		printf("GPU has computed Sum Correctly\n");
	}
	else
	{
		printf("There is an Error in GPU Computation\n");
	}
	
	// 清空GPU内存
	cudaFree(d_a0);
	cudaFree(d_b0);
	cudaFree(d_c0);
	cudaFree(d_a0);
	cudaFree(d_b0);
	cudaFree(d_c0);
	
	// 清空cuda分配的cpu内存
	cudaFreeHost(h_a);
	cudaFreeHost(h_b);
	cudaFreeHost(h_c);
	return 0;
}

```

### 4) rank_sort  排列排序算法
```c
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

#define arraySize 5
#define threadPerBlock 5

// 和函数，对a数组进行排序后 放入b数组
__global__ void addKernel(int *d_a, int *d_b)
{
	int count = 0;
	int tid = threadIdx.x; // 当前块中线程id
	int ttid = blockIdx.x * threadPerBlock + tid;// 总线程id
	
	int val = d_a[ttid];// 当前 数组元素
	
        // 共享内存，一个线程块共享
	__shared__ int cache[threadPerBlock];
	
	for (int i = tid; i < arraySize; i += threadPerBlock) // 多线程块中对应的线程执行的元素
	{
		cache[tid] = d_a[i];// 
		__syncthreads();
		for (int j = 0; j < threadPerBlock; ++j)
			if (val > cache[j])
				count++;// 位置指针
		__syncthreads();
	}
	d_b[count] = val;// 按大小放入对应位置
}

int main()
{
    // cpu数据
    int h_a[arraySize] = { 5, 9, 3, 4, 8 };
    int h_b[arraySize];
    int *d_a, *d_b; // 指针，指向gpu数据地址
	 
    // 分配GPU内存
    cudaMalloc((void**)&d_b, arraySize * sizeof(int));
    cudaMalloc((void**)&d_a, arraySize * sizeof(int));
    
    // 赋值CPU数据 到 GPU
    cudaMemcpy(d_a, h_a, arraySize * sizeof(int), cudaMemcpyHostToDevice);
    
    // 每个线程计算一个元素
    addKernel<<<arraySize/threadPerBlock, threadPerBlock>>>(d_a, d_b);
    // 设备同步，等待GPU计算完成
    cudaDeviceSynchronize();
    // 赋值GPU结果到 CPU
    cudaMemcpy(h_b, d_b, arraySize * sizeof(int), cudaMemcpyDeviceToHost);
    
    // 打印结果
    printf("The Enumeration sorted Array is: \n");
    for (int i = 0; i < arraySize; i++) 
    {
	printf("%d\n", h_b[i]);
    }
    
    // 清空GPU内存
    cudaFree(d_a);
    cudaFree(d_b);
    
    return 0;
}
	 
```

### 5) histogram 直方图计算  有/无 元组操作
```c
#include <stdio.h>
#include <cuda_runtime.h>
 
#define SIZE 1000  // 数组元素数量， 按大小分配到 16个格子的直方图中
#define NUM_BIN 16 // 直方图 bin数量

// 无 原子操作
__global__ void histogram_without_atomic(int *d_b, int *d_a)
{       
        // 当前总线程id
	int tid = threadIdx.x + blockDim.x * blockIdx.x;
	int item = d_a[tid];// 该数组元素
	if (tid < SIZE)
	{
		d_b[item]++;// 数据为整形，直接以数据为直方图索引，对应直方图bin计数++
	}
	
}
// 原子操作加法
__global__ void histogram_atomic(int *d_b, int *d_a)
{
	int tid = threadIdx.x + blockDim.x * blockIdx.x; // 当前总线程id
	int item = d_a[tid];// 该数组元素
	if (tid < SIZE)
	{
		atomicAdd(&(d_b[item]), 1);// 原子操作+1
	}
}

int main()
{
	// 待计算直方图的 一维数组向量
	int h_a[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		
		h_a[i] = i % NUM_BIN;
	}
	
	// 初始化直方图======
	int h_b[NUM_BIN];
	for (int i = 0; i < NUM_BIN; i++)
	{
		h_b[i] = 0;
	}

	// 定义GPU数据指针
	int * d_a;
	int * d_b;

	// 分配 GPU数据内存
	cudaMalloc((void **)&d_a, SIZE * sizeof(int));
	cudaMalloc((void **)&d_b, NUM_BIN * sizeof(int));

	// 拷贝cpu数据 到 gpu中
	cudaMemcpy(d_a, h_a, SIZE * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, NUM_BIN * sizeof(int), cudaMemcpyHostToDevice);

	
	// 启动核函数，多个线程格子，每个线程格子内共bin个线程，每个线程分配一个bin
	histogram_without_atomic << <((SIZE+NUM_BIN-1) / NUM_BIN), NUM_BIN >> >(d_b, d_a);
	//histogram_atomic << <((SIZE+NUM_BIN-1) / NUM_BIN), NUM_BIN >> >(d_b, d_a);
		

	// 从GPU 拷贝结果 到CPU 
	cudaMemcpy(h_b, d_b, NUM_BIN * sizeof(int), cudaMemcpyDeviceToHost);
	printf("Histogram using 16 bin without shared Memory is: \n");
	// 打印结果
	for (int i = 0; i < NUM_BIN; i++) 
	{
		printf("bin %d: count %d\n", i, h_b[i]);
	}

	// 清空GPU内存
	cudaFree(d_a);
	cudaFree(d_b);
	return 0;
}


```

### 6) histogram 直方图计算 共享内存
```c
// https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA/blob/master/Chapter4/06_histogram_shared_memory.cu

#include <stdio.h>
#include <cuda_runtime.h>
#define SIZE 1000   // 向量大小
#define NUM_BIN 256 // 直方图数量

// 使用共享内存 进行直方图计算
__global__ void histogram_shared_memory(int *d_b, int *d_a)
{
	int tid = threadIdx.x + blockDim.x * blockIdx.x;// 当前总线程id
	int offset = blockDim.x * gridDim.x;// 一个线程格，线程数量，
	
	__shared__ int cache[256];// 一个线程块内共享数据
	cache[threadIdx.x] = 0;   // 一开始初始化为0
	__syncthreads();
	
	while (tid < SIZE)
	{
		atomicAdd(&(cache[d_a[tid]]), 1);// 对应元素位置的 直方图bin中计数+1
		tid += offset; // 跳过 步长
	}
	__syncthreads();//线程同步
	
	atomicAdd(&(d_b[threadIdx.x]), cache[threadIdx.x]);// 累加到 对应的直方图bin中
}

int main()
{
	// 生成待分配 数组，CPU中
	int h_a[SIZE];
	for (int i = 0; i < SIZE; i++) 
	{
		//h_a[i] = bit_reverse(i, log2(SIZE));
		h_a[i] = i % NUM_BIN;
	}
	// 初始化直方图数组
	int h_b[NUM_BIN];
	for (int i = 0; i < NUM_BIN; i++)
	{
		h_b[i] = 0;
	}

	// 定义 GPU数据指针
	int * d_a;
	int * d_b;

	// 分配 GPU 内存
	cudaMalloc((void **)&d_a, SIZE * sizeof(int)); // 原数组
	cudaMalloc((void **)&d_b, NUM_BIN * sizeof(int));// 直方图数组

	//拷贝CPU数据到GPU
	cudaMemcpy(d_a, h_a, SIZE * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, NUM_BIN * sizeof(int), cudaMemcpyHostToDevice);


	// 启动核函数，现场格子内 NUM_BIN个线程
	histogram_shared_memory << <SIZE / NUM_BIN, NUM_BIN >> >(d_b, d_a);


	// 拷贝GPU结果 到CPU
	cudaMemcpy(h_b, d_b, NUM_BIN * sizeof(int), cudaMemcpyDeviceToHost);
	// 打印结果
	printf("Histogram using 16 bin is: ");
	for (int i = 0; i < NUM_BIN; i++) 
	{
		printf("bin %d: count %d\n", i, h_b[i]);
	}

	// 清空gpu内存
	cudaFree(d_a);
	cudaFree(d_b);

	return 0;
}


```




## c opencv图像读取、显示、视频播放等 opencvGPU接口图像add、sub、颜色空间转换、阈值操作等

### 1) opencv 图像读取
```c
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 // Read the image 
 Mat img = imread("images/cameraman.tif",0);

 // Check for failure in reading an Image
 if (img.empty()) 
 {
  cout << "Could not open an image" << endl;
  return -1;
 }
// 可视化窗口名
 String win_name = "My First Opencv Program"; 

 // Create a window
 namedWindow(win_name); 

 // Show our image inside the created window.
imshow(win_name, img); 

// Wait for any keystroke in the window 
waitKey(0); 

//destroy the created window
 destroyWindow(win_name); 

 return 0;
}
```


### 2) 创建单通道 256x256 0值 矩阵，黑色图
```c
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 // 创建单通道 256x256 0值 矩阵，黑色图
 Mat img(256, 256, CV_8UC1, Scalar(0)); 
 String win_name = "Blank Image"; 
 namedWindow(win_name); 
 imshow(win_name, img); 
 waitKey(0); 
 destroyWindow(win_name); 
 return 0;
}


```


### 3) 创建三通道 256x256 blue color 图
```c
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 
 // 通道顺序 bgr
 Mat img(256, 256, CV_8UC3, Scalar(255,0,0)); 
 
 String win_name = "Blank Blue Color Image"; 
 namedWindow(win_name); 
 imshow(win_name, img); 

 waitKey(0); 
 destroyWindow(win_name); 
 return 0;
}


```


### 4) opencv显示图形 
```c
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 // 3通道 512*512图像
 Mat img(512, 512, CV_8UC3, Scalar(0,0,0)); 
 // 画线条，起点，终点，颜色，粗细
 line(img,Point(0,0),Point(511,511),Scalar(0,255,0),7);
 // 画长方形
 rectangle(img,Point(384,0),Point(510,128),Scalar(255,255,0),5);
 // 画圆
 circle(img,Point(447,63), 63, Scalar(0,0,255), -1);
 // 画椭圆
 ellipse(img,Point(256,256),Point(100,100),0,0,180,255,-1);
 
 // 显示文字
 putText( img, "OpenCV!", Point(10,500), FONT_HERSHEY_SIMPLEX, 3,
           Scalar(255, 255, 255), 5, 8 );
	   
 String win_name = "Blank Blue Color Image"; //Name of the window
 namedWindow(win_name); // Create a window
 imshow(win_name, img); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window
 destroyWindow(win_name); //destroy the created window

 return 0;
}



```


### 5) 播放视频文件
```c
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
 // 打开视频文件
 VideoCapture cap("images/rhinos.avi"); 
 // if not success, exit program
 if (cap.isOpened() == false) 
 {
  cout << "Cannot open the video file" << endl;
  return -1;
 }
cout<<"Press Q to Quit" << endl;
String win_name = "First Video";
namedWindow(win_name); 
 while (true)
 {
  Mat frame;
  // read a frame
  bool flag = cap.read(frame); 

  //Breaking the while loop at the end of the video
  if (flag == false) 
  {
   break;
  }
  //display the frame 
  imshow(win_name, frame);// 显示该帧图像
  //Wait for 100 ms and key 'q' for exit
  if (waitKey(100) == 'q')
  {
    break;
  }
 }
destroyWindow(win_name);
return 0;
}


```


### 6) 打开摄像头，播放拍摄到的图像
```c
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
 //open the Webcam
 VideoCapture cap(0); // 打开0号摄像头
 // if not success, exit program
 if (cap.isOpened() == false)  
 {
  cout << "Cannot open Webcam" << endl;
  return -1;
 }
 //get the frames rate of the video
 double fps = cap.get(CAP_PROP_FPS); 
 cout << "Frames per seconds : " << fps << endl;
cout<<"Press Q to Quit" <<endl;
 String win_name = "Webcam Video";
 namedWindow(win_name); //create a window
 while (true)
 {
  Mat frame;
  bool flag = cap.read(frame); // read a new frame from video 
  //show the frame in the created window
  imshow(win_name, frame);
  if (waitKey(1) == 'q')
  {
      break;
  }
 }
return 0;
}


```


### 7) opencv GPU 接口 图像相加
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
    // 读取图像，存储在cpu上
    cv::Mat h_img1 = cv::imread("images/cameraman.tif");
    cv::Mat h_img2 = cv::imread("images/circles.png");
    cv::Mat h_result1;
    
    // 定义GPU mat数据
    cv::cuda::GpuMat d_result1,d_img1, d_img2;
    // cpu上的图像 上传到 GPU中   
    d_img1.upload(h_img1);
    d_img2.upload(h_img2);
    
    // 调用GPU接执行 mat add
    cv::cuda::add(d_img1,d_img2, d_result1);
    
    // 下载结果，GPU结果 到 CPU中
    d_result1.download(h_result1);
    
    //显示结果
    cv::imshow("Image1 ", h_img1);
    cv::imshow("Image2 ", h_img2);
    cv::imshow("Result addition ", h_result1);
    cv::imwrite("images/result_add.png", h_result1);
    cv::waitKey();
    return 0;
}


```
### 8) opencv GPU 接口 图像相减
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
    //Read Two Images 
    cv::Mat h_img1 = cv::imread("images/cameraman.tif");
    cv::Mat h_img2 = cv::imread("images/circles.png");
    cv::Mat h_result1;
    
    // 定义GPU数据
    cv::cuda::GpuMat d_result1,d_img1, d_img2;
    
    // CPU 到 GPU   
    d_img1.upload(h_img1);
    d_img2.upload(h_img2);
    
    // 调用GPU接执行 mat substract
    cv::cuda::subtract(d_img1, d_img2,d_result1);
    
    // gpu 结果 到 cpu
    d_result1.download(h_result1);
    
    // 显示，保存
    cv::imshow("Image1 ", h_img1);
    cv::imshow("Image2 ", h_img2);
    cv::imshow("Result Subtraction ", h_result1);
    cv::imwrite("images/result_add.png", h_result1);
    cv::waitKey();
    return 0;
}


```


### 9) opencv GPU 接口 图像 加权叠加
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
    //Read Two Images 
    cv::Mat h_img1 = cv::imread("images/cameraman.tif");
    cv::Mat h_img2 = cv::imread("images/circles.png");
    cv::Mat h_result1;
    
    //定义GPU数据
    cv::cuda::GpuMat d_result1,d_img1, d_img2;
    
    // 上传 输入
    d_img1.upload(h_img1);
    d_img2.upload(h_img2);
    
    // 图像叠加  d_result1 = 0.7*d_img1 + d_img2*0.3 + 0.0
    cv::cuda::addWeighted(d_img1,0.7,d_img2,0.3,0,d_result1);
    
    // 下载结果
    d_result1.download(h_result1);
    cv::imshow("Image1 ", h_img1);
    cv::imshow("Image2 ", h_img2);
    cv::imshow("Result blending ", h_result1);
    cv::imwrite("images/result_add.png", h_result1);
    cv::waitKey();
    return 0;
}


```


### 10)  opencv GPU 接口 图像 二进制操作 与 或 异或 非
```c
// bitwise_and是对二进制数据进行“与”操作，即对图像（灰度图像或彩色图像均可）每个像素值进行二进制“与”操作，1&1=1，1&0=0，0&1=0，0&0=0
//bitwise_or是对二进制数据进行“或”操作，即对图像（灰度图像或彩色图像均可）每个像素值进行二进制“或”操作，1|1=1，1|0=0，0|1=0，0|0=0
//bitwise_xor是对二进制数据进行“异或”操作，即对图像（灰度图像或彩色图像均可）每个像素值进行二进制“异或”操作，1^1=0,1^0=1,0^1=1,0^0=0
//bitwise_not是对二进制数据进行“非”操作，即对图像（灰度图像或彩色图像均可）每个像素值进行二进制“非”操作，~1=0，~0=1


#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
    cv::Mat h_img1 = cv::imread("images/circles.png");
    //Create Device variables
    cv::cuda::GpuMat d_result1,d_img1;
    cv::Mat h_result1;     
    //Upload Image to device
    d_img1.upload(h_img1);

    cv::cuda::bitwise_not(d_img1,d_result1);// 图像非操作 d_result1 = ~d_img1
    
    //Download result back  to host
    d_result1.download(h_result1);
    cv::imshow("Result inversion ", h_result1);
    cv::imwrite("images/result_inversion.png", h_result1);
    cv::waitKey();
    return 0;
}

```


### 11) opencv GPU 接口 图像 颜色空间转换
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
        cv::Mat h_img1 = cv::imread("images/autumn.tif");
        //Define device variables
        cv::cuda::GpuMat d_result1,d_result2,d_result3,d_result4,d_img1;
        //Upload Image to device
        d_img1.upload(h_img1);

        //  GPU 接口 图像 颜色空间转换
        cv::cuda::cvtColor(d_img1, d_result1,cv::COLOR_BGR2GRAY);// 彩色到 灰度
        cv::cuda::cvtColor(d_img1, d_result2,cv::COLOR_BGR2RGB); // bgr彩色 到 RGB
        cv::cuda::cvtColor(d_img1, d_result3,cv::COLOR_BGR2HSV); // bgr彩色 到 HSV
        cv::cuda::cvtColor(d_img1, d_result4,cv::COLOR_BGR2YCrCb);//  bgr彩色 到 YCrCb
        
        cv::Mat h_result1,h_result2,h_result3,h_result4;
        // 下载结果到 cpu
        d_result1.download(h_result1);
        d_result2.download(h_result2);
        d_result3.download(h_result3);
        d_result4.download(h_result4);
        
        // 下载
        cv::imshow("Result in Gray ", h_result1);
        cv::imshow("Result in RGB", h_result2);
        cv::imshow("Result in HSV ", h_result3);
        cv::imshow("Result in YCrCb ", h_result4);
        
        cv::waitKey();
        return 0;
}


```


### 12) opencv GPU 接口 图像 阈值操作
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
        cv::Mat h_img1 = cv::imread("images/cameraman.tif", 0);
        //Define device variables
        cv::cuda::GpuMat d_result1,d_result2,d_result3,d_result4,d_result5, d_img1;
        //Upload image on device
        d_img1.upload(h_img1);

        // GPU图像阈值操作，                     阈值， 最大值，
        cv::cuda::threshold(d_img1, d_result1, 128.0, 255.0, cv::THRESH_BINARY);    // 二值化，大于阈值为1
        cv::cuda::threshold(d_img1, d_result2, 128.0, 255.0, cv::THRESH_BINARY_INV);// 二值化，大于阈值为0 
        cv::cuda::threshold(d_img1, d_result3, 128.0, 255.0, cv::THRESH_TRUNC);     // 上截断，大于阈值，截断为阈值
        cv::cuda::threshold(d_img1, d_result4, 128.0, 255.0, cv::THRESH_TOZERO);    // 下截断到0，小于阈值，截断为0
        cv::cuda::threshold(d_img1, d_result5, 128.0, 255.0, cv::THRESH_TOZERO_INV);// 上截断到0,大于阈值，截断为0

        cv::Mat h_result1,h_result2,h_result3,h_result4,h_result5;
        //Copy results back to host
        d_result1.download(h_result1);
        d_result2.download(h_result2);
        d_result3.download(h_result3);
        d_result4.download(h_result4);
        d_result5.download(h_result5);
        cv::imshow("Result Threshhold binary ", h_result1);
        cv::imshow("Result Threshhold binary inverse ", h_result2);
        cv::imshow("Result Threshhold truncated ", h_result3);
        cv::imshow("Result Threshhold truncated to zero ", h_result4);
        cv::imshow("Result Threshhold truncated to zero inverse ", h_result5);
        cv::waitKey();

    return 0;
}


```


### 13) opencv cpu 阈值操作性能
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main (int argc, char* argv[])
{
cv::Mat src = cv::imread("images/cameraman.tif", 0);
cv::Mat result_host1,result_host2,result_host3,result_host4,result_host5;
//Get initial time in miliseconds
int64 work_begin = cv::getTickCount(); // 计时
cv::threshold(src, result_host1, 128.0, 255.0, cv::THRESH_BINARY);
cv::threshold(src, result_host2, 128.0, 255.0, cv::THRESH_BINARY_INV);
cv::threshold(src, result_host3, 128.0, 255.0, cv::THRESH_TRUNC);
cv::threshold(src, result_host4, 128.0, 255.0, cv::THRESH_TOZERO);
cv::threshold(src, result_host5, 128.0, 255.0, cv::THRESH_TOZERO_INV);
//Get time after work has finished     
int64 delta = cv::getTickCount() - work_begin;
//Frequency of timer
double freq = cv::getTickFrequency();
double work_fps = freq / delta;
std::cout<<"Performance of Thresholding on CPU: " <<std::endl;
std::cout <<"Time: " << (1/work_fps) <<std::endl;
std::cout <<"FPS: " <<work_fps <<std::endl;
return 0;
}


```

### 14) opencv Gpu 阈值操作性能
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main (int argc, char* argv[])
{
 cv::Mat h_img1 = cv::imread("images/cameraman.tif", 0);
cv::cuda::GpuMat d_result1,d_result2,d_result3,d_result4,d_result5, d_img1;
//Measure initial time ticks
int64 work_begin = cv::getTickCount(); // 计时
d_img1.upload(h_img1);
cv::cuda::threshold(d_img1, d_result1, 128.0, 255.0, cv::THRESH_BINARY);
cv::cuda::threshold(d_img1, d_result2, 128.0, 255.0, cv::THRESH_BINARY_INV);
cv::cuda::threshold(d_img1, d_result3, 128.0, 255.0, cv::THRESH_TRUNC);
cv::cuda::threshold(d_img1, d_result4, 128.0, 255.0, cv::THRESH_TOZERO);
cv::cuda::threshold(d_img1, d_result5, 128.0, 255.0, cv::THRESH_TOZERO_INV);

cv::Mat h_result1,h_result2,h_result3,h_result4,h_result5;
d_result1.download(h_result1);
d_result2.download(h_result2);
d_result3.download(h_result3);
d_result4.download(h_result4);
d_result5.download(h_result5);
//Measure difference in time ticks
int64 delta = cv::getTickCount() - work_begin;
double freq = cv::getTickFrequency();
//Measure frames per second
double work_fps = freq / delta;
std::cout <<"Performance of Thresholding on GPU: " <<std::endl;
std::cout <<"Time: " << (1/work_fps) <<std::endl;
std::cout <<"FPS: " <<work_fps <<std::endl;
 return 0;
}


```


## d opencv GPU接口 直方图均衡化  变形 仿射变换 均值滤波器 高斯核滤波器 索贝尔 拉普拉斯 腐蚀膨胀
### 1)opencv GPU接口 直方图均衡化 
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
    cv::Mat h_img1 = cv::imread("images/cameraman.tif",0);
    cv::cuda::GpuMat d_img1,d_result1;
    d_img1.upload(h_img1);
    
    //  GPU接口 直方图均衡化 
    cv::cuda::equalizeHist(d_img1, d_result1);
    
    cv::Mat h_result1;
    d_result1.download(h_result1);
    cv::imshow("Original Image ", h_img1);
	cv::imshow("Histogram Equalized Image", h_result1);
	cv::imwrite("images/result_inversion.png", h_img1);
	cv::imwrite("images/result_inversion.png", h_result1);
    cv::waitKey();
    return 0;
}


```


### 2) opencv 转成HSV空间 单通道 直方图均衡化 再合并
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
    cv::Mat h_img1 = cv::imread("images/autumn.tif");
    cv::Mat h_img2,h_result1;
    cvtColor(h_img1, h_img2, cv::COLOR_BGR2HSV);// 转成HSV空间 
    
    // split分割成 多个 单通道
    std::vector< cv::Mat > vec_channels;
    cv::split(h_img2, vec_channels); 
    
    // 对单个通道进行直方图均衡化
    cv::equalizeHist(vec_channels[2], vec_channels[2]);
    
    //M那个通道 合并
    cv::merge(vec_channels, h_img2); 
      
    //Convert the histogram equalized image from HSV to BGR color space again
    cv::cvtColor(h_img2,h_result1, cv::COLOR_HSV2BGR);
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Histogram Equalized Image", h_result1);
    cv::waitKey();
    return 0;
}


```


### 3) opencv GPU接口  图像 变形 
```c
#include <iostream>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/opencv.hpp"

int main ()
{
    cv::Mat h_img1 = cv::imread("images/cameraman.tif",0);
    cv::cuda::GpuMat d_img1,d_result1,d_result2;
    d_img1.upload(h_img1);
    // 原图像尺寸
    int width= d_img1.cols;
    int height = d_img1.size().height;
    
    // gpu cuda接口 图像变形 
    cv::cuda::resize(d_img1,d_result1,cv::Size(200, 200), cv::INTER_CUBIC);// 变形到固定尺寸，
    cv::cuda::resize(d_img1,d_result2,cv::Size(0.5*width, 0.5*height), cv::INTER_LINEAR);// 缩小一半 
// CV_INTER_NN - 最近-邻居插补
// CV_INTER_LINEAR - 双线性插值（默认方法）
// CV_INTER_AREA - 像素面积相关重采样。当缩小图像时，该方法可以避免波纹的出现。当放大图像时，类似于方法CV_INTER_NN。
// CV_INTER_CUBIC - 双三次插值。

    cv::Mat h_result1,h_result2;
    d_result1.download(h_result1);
    d_result2.download(h_result2);
    
    cv::imshow("Original Image ", h_img1);
    cv::imshow("Resized Image", h_result1);
    cv::imshow("Resized Image 2", h_result2);
    cv::imwrite("Resized1.png", h_result1);
    cv::imwrite("Resized2.png", h_result2);
    cv::waitKey();
    return 0;
}

```

### 4) opencv GPU接口  图像 仿射变换
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main ()
{
	cv::Mat h_img1 = cv::imread("images/cameraman.tif",0);
	cv::cuda::GpuMat d_img1,d_result1,d_result2;
	d_img1.upload(h_img1);
	int cols= d_img1.cols;
	int rows = d_img1.size().height;
	
	// 平移变换，x，水平方向平移70，y垂直方向平移50
	cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, 70, 
	                                              0, 1, 50);		      
	cv::cuda::warpAffine(d_img1,d_result1,trans_mat,d_img1.size());
	
	// 旋转变换
	cv::Point2f pt(d_img1.cols/2., d_img1.rows/2.); // 中心点   
	cv::Mat r = cv::getRotationMatrix2D(pt, 45, 1.0);// 旋转45度
	cv::cuda::warpAffine(d_img1, d_result2, r, cv::Size(d_img1.cols, d_img1.rows));
	
	cv::Mat h_result1,h_result2;
	d_result1.download(h_result1);
	d_result2.download(h_result2);
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Translated Image", h_result1);
	cv::imshow("Rotated Image", h_result2);
	cv::imwrite("Translated.png", h_result1);
	cv::imwrite("Rotated.png", h_result2);
	cv::waitKey();
	return 0;
}
```


### 5) opencv GPU接口  图像 均值滤波器
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
	cv::Mat h_img1 = cv::imread("images/cameraman.tif",0);
	cv::cuda::GpuMat d_img1,d_result3x3,d_result5x5,d_result7x7;

	d_img1.upload(h_img1);
	// cuda滤波器指针
	cv::Ptr<cv::cuda::Filter> filter3x3,filter5x5,filter7x7;
	// 创建cuda滤波器 BoxFilter 
	filter3x3 = cv::cuda::createBoxFilter(CV_8UC1,CV_8UC1,cv::Size(3,3));
	// 执行滤波器
	filter3x3->apply(d_img1, d_result3x3);
	filter5x5 = cv::cuda::createBoxFilter(CV_8UC1,CV_8UC1,cv::Size(5,5));
	filter5x5->apply(d_img1, d_result5x5);
	filter7x7 = cv::cuda::createBoxFilter(CV_8UC1,CV_8UC1,cv::Size(7,7));
	filter7x7->apply(d_img1, d_result7x7);

	cv::Mat h_result3x3,h_result5x5,h_result7x7;
	d_result3x3.download(h_result3x3);
	d_result5x5.download(h_result5x5);
	d_result7x7.download(h_result7x7);


	cv::imshow("Original Image ", h_img1);
	cv::imshow("Blurred with kernel size 3x3", h_result3x3);
	cv::imshow("Blurred with kernel size 5x5", h_result5x5);
	cv::imshow("Blurred with kernel size 7x7", h_result7x7);
	cv::imwrite("Blurred3x3.png", h_result3x3);
	cv::imwrite("Blurred5x5.png", h_result5x5);
	cv::imwrite("Blurred7x7.png", h_result7x7);

	cv::waitKey();
	return 0;
	}


```

### 6) opencv GPU接口  图像 高斯核 滤波器
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
	cv::Mat h_img1 = cv::imread("images/cameraman.tif",0);
	cv::cuda::GpuMat d_img1,d_result3x3,d_result5x5,d_result7x7;

	d_img1.upload(h_img1);
	// cuda滤波器指针
	cv::Ptr<cv::cuda::Filter> filter3x3,filter5x5,filter7x7;
	// 创建cuda滤波器 GaussianFilter 
	filter3x3 = cv::cuda::createGaussianFilter(CV_8UC1,CV_8UC1,cv::Size(3,3),1);
	// 执行滤波器
	filter3x3->apply(d_img1, d_result3x3);
	filter5x5 = cv::cuda::createGaussianFilter(CV_8UC1,CV_8UC1,cv::Size(5,5),1);
	filter5x5->apply(d_img1, d_result5x5);
	filter7x7 = cv::cuda::createGaussianFilter(CV_8UC1,CV_8UC1,cv::Size(7,7),1);
	filter7x7->apply(d_img1, d_result7x7);
	
	cv::Mat h_result3x3,h_result5x5,h_result7x7;
	d_result3x3.download(h_result3x3);
	d_result5x5.download(h_result5x5);
	d_result7x7.download(h_result7x7);
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Blurred with kernel size 3x3", h_result3x3);
	cv::imshow("Blurred with kernel size 5x5", h_result5x5);
	cv::imshow("Blurred with kernel size 7x7", h_result7x7);
	cv::imwrite("gBlurred3x3.png", h_result3x3);
	cv::imwrite("gBlurred5x5.png", h_result5x5);
	cv::imwrite("gBlurred7x7.png", h_result7x7);
	cv::waitKey();
	return 0;
}



```


### 7) cpu 均值滤波器
```c
#include <iostream>
#include "opencv2/opencv.hpp"

int main ()
{
    cv::Mat h_img1 = cv::imread("images/saltpepper.png",0);
	cv::Mat h_result;
	cv::medianBlur(h_img1,h_result,3);
    cv::imshow("Original Image ", h_img1);
	cv::imshow("Median Blur Result", h_result);
	cv::waitKey();
    return 0;
}


```

 
### 8) GPU 索贝尔滤波器
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
	cv::Mat h_img1 = cv::imread("images/blobs.png",0);
	cv::cuda::GpuMat d_img1,d_resultx,d_resulty,d_resultxy;
	d_img1.upload(h_img1);
	
	cv::Ptr<cv::cuda::Filter> filterx,filtery,filterxy;
	// 索贝尔滤波器
	filterx = cv::cuda::createSobelFilter(CV_8UC1,CV_8UC1,1,0);// x水平方向
	filterx->apply(d_img1, d_resultx);
	
	filtery = cv::cuda::createSobelFilter(CV_8UC1,CV_8UC1,0,1);// y垂直方向
	filtery->apply(d_img1, d_resulty);
	
	cv::cuda::add(d_resultx,d_resulty,d_resultxy);    // 叠加
	
	cv::Mat h_resultx,h_resulty,h_resultxy;
	d_resultx.download(h_resultx);
	d_resulty.download(h_resulty);
	d_resultxy.download(h_resultxy);
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Sobel-x derivative", h_resultx);
	cv::imshow("Sobel-y derivative", h_resulty);
	cv::imshow("Sobel-xy derivative", h_resultxy);
	cv::imwrite("sobelx.png", h_resultx);
	cv::imwrite("sobely.png", h_resulty);
	cv::imwrite("sobelxy.png", h_resultxy);
	cv::waitKey();
	return 0;
}


```


### 9) GPU ScharrFilter 滤波器 类似 索贝尔 Sobel
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
	cv::Mat h_img1 = cv::imread("images/blobs.png",0);
	cv::cuda::GpuMat d_img1,d_resultx,d_resulty,d_resultxy;
	d_img1.upload(h_img1);

	cv::Ptr<cv::cuda::Filter> filterx,filtery;
	// 创建滤波器
	filterx = cv::cuda::createScharrFilter(CV_8UC1,CV_8UC1,1,0);
	filterx->apply(d_img1, d_resultx);
	filtery = cv::cuda::createScharrFilter(CV_8UC1,CV_8UC1,0,1);
	filtery->apply(d_img1, d_resulty);

	cv::cuda::add(d_resultx,d_resulty,d_resultxy);   

	cv::Mat h_resultx,h_resulty,h_resultxy;
	d_resultx.download(h_resultx);
	d_resulty.download(h_resulty);
	d_resultxy.download(h_resultxy);
	
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Scharr-x derivative", h_resultx);
	cv::imshow("Scharr-y derivative", h_resulty);
	cv::imshow("Scharr-xy derivative", h_resultxy);
	
	cv::imwrite("scharrx.png", h_resultx);
	cv::imwrite("scharry.png", h_resulty);
	cv::imwrite("scharrxy.png", h_resultxy);
	cv::waitKey();
	return 0;
}




```


### 9) GPU 拉普拉斯滤波器
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
	cv::Mat h_img1 = cv::imread("images/blobs.png",0);
	cv::cuda::GpuMat d_img1,d_result1,d_result3;
	d_img1.upload(h_img1);
	cv::Ptr<cv::cuda::Filter> filter1,filter3;
	
	// 创建滤波器
	filter1 = cv::cuda::createLaplacianFilter(CV_8UC1,CV_8UC1,1);
	filter1->apply(d_img1, d_result1);
	filter3 = cv::cuda::createLaplacianFilter(CV_8UC1,CV_8UC1,3);
	filter3->apply(d_img1, d_result3);
	
	cv::Mat h_result1,h_result3;
	d_result1.download(h_result1);
	d_result3.download(h_result3);
	
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Laplacian Filter 1", h_result1);
	cv::imshow("Laplacian Filter 3", h_result3);
	
	cv::imwrite("laplacian1.png", h_result1);
	cv::imwrite("laplacian3.png", h_result3);
	cv::waitKey();
	return 0;
}


```


### 9) GPU 腐蚀膨胀操作
```c
#include <iostream>
#include "opencv2/opencv.hpp"


int main ()
{
	cv::Mat h_img1 = cv::imread("images/blobs.png",0);
	cv::cuda::GpuMat d_img1,d_resulte,d_resultd,d_resulto, d_resultc;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));// 长方形核	
	d_img1.upload(h_img1);
	
	cv::Ptr<cv::cuda::Filter> filtere,filterd,filtero,filterc;
	
	filtere = cv::cuda::createMorphologyFilter(cv::MORPH_ERODE,CV_8UC1,element);// 腐蚀操作
	filtere->apply(d_img1, d_resulte);
	filterd = cv::cuda::createMorphologyFilter(cv::MORPH_DILATE,CV_8UC1,element);//膨胀操作
	filterd->apply(d_img1, d_resultd);
	filtero = cv::cuda::createMorphologyFilter(cv::MORPH_OPEN,CV_8UC1,element);// 开运算
	filtero->apply(d_img1, d_resulto);
	filterc = cv::cuda::createMorphologyFilter(cv::MORPH_CLOSE,CV_8UC1,element);// 闭运算
	filterc->apply(d_img1, d_resultc);

	cv::Mat h_resulte,h_resultd,h_resulto,h_resultc;
	d_resulte.download(h_resulte);
	d_resultd.download(h_resultd);
	d_resulto.download(h_resulto);
	d_resultc.download(h_resultc);
	cv::imshow("Original Image ", h_img1);
	cv::imshow("Erosion", h_resulte);
	cv::imshow("Dilation", h_resultd);
	cv::imshow("Opening", h_resulto);
	cv::imshow("closing", h_resultc);
	cv::imwrite("erosion7.png", h_resulte);
	cv::imwrite("dilation7.png", h_resultd);
	cv::imwrite("opening7.png", h_resulto);
	cv::imwrite("closing7.png", h_resultc);
	cv::waitKey();
	return 0;
}


```

## e opencv GPU接口 边缘检测 orb sift 特征 candy hough mog 级联回归人脸检测
### 1) 蓝色通道图像 边缘检测
```c
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
 // 打开摄像头
    VideoCapture cap(0); //capture the video from web cam
    if ( !cap.isOpened() )  
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }
    while (true)
    {
        Mat frame;
// 读取一帧图像
        bool flag = cap.read(frame); 
         if (!flag) 
        {
             cout << "Cannot read a frame from webcam" << endl;
             break;
        }
// 定义GPU数据等
cuda::GpuMat d_frame, d_frame_hsv,d_intermediate,d_result;
cuda::GpuMat d_frame_shsv[3];
cuda::GpuMat d_thresc[3];
Mat h_result;
d_frame.upload(frame);

// GPU brg 转HSV空间
cuda::cvtColor(d_frame, d_frame_hsv, COLOR_BGR2HSV);

// HSV空间 通道分割
cuda::split(d_frame_hsv, d_frame_shsv);

// 阈值，最大值
// 三通道，阈值二值化，大于阈值为1
cuda::threshold(d_frame_shsv[0], d_thresc[0], 110, 130, THRESH_BINARY);
cuda::threshold(d_frame_shsv[1], d_thresc[1], 50, 255, THRESH_BINARY);
cuda::threshold(d_frame_shsv[2], d_thresc[2], 50, 255, THRESH_BINARY);

// 二值化三通通道与操作
cv::cuda::bitwise_and(d_thresc[0], d_thresc[1],d_intermediate);
cv::cuda::bitwise_and(d_intermediate, d_thresc[2], d_result);

d_result.download(h_result);
imshow("Thresholded Image", h_result); 
imshow("Original", frame); 

        if (waitKey(1) == 'q') 
       {
            break; 
       }
    }
   return 0;
}


```

### 2) GPU canny边缘检测 
```c
#include <cmath>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;


int main()
{
	Mat h_image = imread("images/drawing.JPG",0);
	if (h_image.empty())
	{
	    cout << "can not open image"<< endl;
	    return -1;
	}
	GpuMat d_edge,d_image;
	Mat h_edge;
	d_image.upload(h_image);
	
	// 边缘检测指针
	cv::Ptr<cv::cuda::CannyEdgeDetector> canny_edge = cv::cuda::createCannyEdgeDetector(2.0, 100.0, 3, false);
	// 创建边缘检测器
	canny_edge->detect(d_image, d_edge);
	
	d_edge.download(h_edge);
	imshow("source", h_image);
	imshow("detected edges", h_edge);
	 waitKey(0);

	return 0;
}


```

### 3) 霍夫线变换  cpu  与 gpu 函数 运行时间对比
```c
#include <cmath>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;


int main()
{
    Mat h_image = imread("images/drawing.JPG",0);
    if (h_image.empty())
    {
        cout << "can not open image"<< endl;
        return -1;
    }

    Mat h_edge;
    // 首先需要进行边缘检测
    cv::Canny(h_image, h_edge, 100, 200, 3);

    Mat h_imagec;
    cv::cvtColor(h_edge, h_imagec, COLOR_GRAY2BGR);// 变成3通道，可以显示彩色线条
    
    Mat h_imageg = h_imagec.clone();
    vector<Vec4i> h_lines;
    
    // CPU 霍夫线变换计时 HoughLinesP()===================================
    {
        const int64 start = getTickCount(); // 计时=====
        HoughLinesP(h_edge, h_lines, 1, CV_PI / 180, 50, 60, 5);
        const double time_elapsed = (getTickCount() - start) / getTickFrequency();
        cout << "CPU Time : " << time_elapsed * 1000 << " ms" << endl;
        cout << "CPU FPS : " << (1/time_elapsed) << endl;
    }
    for (size_t i = 0; i < h_lines.size(); ++i)
    {
        // 在图像上 画上检测出来的 直线========
        Vec4i line_point = h_lines[i];
        line(h_imagec, Point(line_point[0], line_point[1]), Point(line_point[2], line_point[3]), Scalar(0, 0, 255), 2, LINE_AA);
    }

    GpuMat d_edge, d_lines;// GPU 中 检测结果
    
    d_edge.upload(h_edge);// 边缘图像
    {
	const int64 start = getTickCount();
        Ptr<cuda::HoughSegmentDetector> hough = cuda::createHoughSegmentDetector(1.0f, (float) (CV_PI / 180.0f), 50, 5);
        hough->detect(d_edge, d_lines);

        const double time_elapsed = (getTickCount() - start) / getTickFrequency();
        cout << "GPU Time : " << time_elapsed * 1000 << " ms" << endl;
       cout << "GPU FPS : " << (1/time_elapsed) << endl;
    }
    vector<Vec4i> lines_g;
    if (!d_lines.empty())
    {
        lines_g.resize(d_lines.cols);
        Mat h_lines(1, d_lines.cols, CV_32SC4, &lines_g[0]);
        d_lines.download(h_lines);// 拷贝检测结果==========
    }
    for (size_t i = 0; i < lines_g.size(); ++i)
    {
        // 在图像上 画上检测出来的 直线========
        Vec4i line_point = lines_g[i];
        line(h_imageg, Point(line_point[0], line_point[1]), Point(line_point[2], line_point[3]), Scalar(0, 0, 255), 2, LINE_AA);
    }

    imshow("source", h_image);
    imshow("detected lines [CPU]", h_imagec);
    imshow("detected lines [GPU]", h_imageg);
    
    imwrite("hough_source.png", h_image);
    imwrite("hough_cpu_line.png", h_imagec);
    imwrite("hough_gpu_line.png", h_imageg);
    
    waitKey(0);

    return 0;
}


```


### 4) OPENCV GPU faster特征点检测
```c
#include <iostream>
#include "opencv2/opencv.hpp"
 
using namespace cv;
using namespace std;
 
int main()
{
	Mat h_image = imread( "images/drawing.JPG", 0 );

	// 创建OPENCV GPU faster特征点检测器
	cv::Ptr<cv::cuda::FastFeatureDetector> detector = cv::cuda::FastFeatureDetector::create(100,true,2);
	
	std::vector<cv::KeyPoint> keypoints;// 特征点不用拷贝????
	
	// 上传图像
	cv::cuda::GpuMat d_image;
	d_image.upload(h_image);
	
	// 执行检测
	detector->detect(d_image, keypoints);
	
	// 绘制特征点
	cv::drawKeypoints(h_image,keypoints,h_image);
	//Show detected keypoints
	imshow("Final Result", h_image );
	waitKey(0);
	return 0;
}


```


### 5) OPENCV GPU orb特征点检测
```c
#include <iostream>
#include "opencv2/opencv.hpp"
 
using namespace cv;
using namespace std;
 
int main()
{
	Mat h_image = imread( "images/drawing.JPG", 0 );

	//  OPENCV GPU orb特征点检测
	cv::Ptr<cv::cuda::ORB> detector = cv::cuda::ORB::create();
	std::vector<cv::KeyPoint> keypoints;

	cv::cuda::GpuMat d_image;
	d_image.upload(h_image);

	detector->detect(d_image, keypoints);

	cv::drawKeypoints(h_image,keypoints,h_image);
	imshow("Final Result", h_image );
	waitKey(0);
	return 0;

}


```


### 6)  OPENCV GPU SURF特征 检测 描述子匹配 单应变换目标图像位置显示
```c
#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudafeatures2d.hpp"
#include "opencv2/xfeatures2d/cuda.hpp"


using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main( int argc, char** argv )
{
        // cpu 数据
	Mat h_object_image = imread( "images/object1.jpg", 0 ); // 带皮牌目标图像
	Mat h_scene_image = imread( "images/scene1.jpg", 0 ); // 场景图像，
	
	// gpu 数据
	cuda::GpuMat d_object_image;
	cuda::GpuMat d_scene_image;
	
	cuda::GpuMat d_keypoints_scene, d_keypoints_object;       // GPU 关键点
	vector< KeyPoint > h_keypoints_scene, h_keypoints_object; // CPU 关键点
	
	cuda::GpuMat d_descriptors_scene, d_descriptors_object;   // GPU 描述子
	// 图像 CPU 上传到 GPU
	d_object_image.upload(h_object_image);
	d_scene_image.upload(h_scene_image);

	// SURF_CUDA 检测器
	cuda::SURF_CUDA surf(100);
	// 检测 特征点并提取 对应的 描述子
	surf( d_object_image, cuda::GpuMat(), d_keypoints_object, d_descriptors_object );
	surf( d_scene_image, cuda::GpuMat(), d_keypoints_scene, d_descriptors_scene );
        
	// Brute Force 暴力匹配器
	Ptr< cuda::DescriptorMatcher > matcher = cuda::DescriptorMatcher::createBFMatcher();
	vector< vector< DMatch> > d_matches;
	matcher->knnMatch(d_descriptors_object, d_descriptors_scene, d_matches, 2);// 最近邻 2个

	// 下载关键点
	surf.downloadKeypoints(d_keypoints_scene, h_keypoints_scene);
	surf.downloadKeypoints(d_keypoints_object, h_keypoints_object);
	
	
	std::vector< DMatch > good_matches;
	for (int k = 0; k < std::min(h_keypoints_object.size()-1, d_matches.size()); k++)
	{
		if ( (d_matches[k][0].distance < 0.6*(d_matches[k][1].distance)) &&
				((int)d_matches[k].size() <= 2 && (int)d_matches[k].size()>0) )
		{
		// 最近匹配 < 0.6*次近匹配
			good_matches.push_back(d_matches[k][0]);
		}
	}
        std::cout << "size:" <<good_matches.size();
	
	// 绘制匹配点对
	Mat h_image_result;
	drawMatches( h_object_image, h_keypoints_object, h_scene_image, h_keypoints_scene,
			good_matches, h_image_result, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::DEFAULT );

	
	// 找到对应匹配点对 的 图像像素 2d坐标
	std::vector<Point2f> object;
	std::vector<Point2f> scene;
	for (int i = 0; i < good_matches.size(); i++) 
	{
		object.push_back(h_keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(h_keypoints_scene[good_matches[i].trainIdx].pt);
	}
	// 计算匹配平面的 单元变换矩阵
	Mat Homo = findHomography(object, scene, RANSAC);
	std::vector<Point2f> corners(4); // 图像四个角点
	std::vector<Point2f> scene_corners(4);
	
	corners[0] = Point(0, 0);// 最上面
	corners[1] = Point(h_object_image.cols, 0);// 目标图像 四点 单应变换后 计算在 场景图像中的位置
	corners[2] = Point(h_object_image.cols, h_object_image.rows);
	corners[3] = Point(0, h_object_image.rows);
	perspectiveTransform(corners, scene_corners, Homo);// 目标点 ====> 单应变换 ====> 场景中的位置
	
	// 画出四条边界线=====
	line(h_image_result, scene_corners[0] + Point2f(h_object_image.cols, 0),scene_corners[1] + Point2f(h_object_image.cols, 0),	Scalar(255, 0, 0), 4);
	line(h_image_result, scene_corners[1] + Point2f(h_object_image.cols, 0),scene_corners[2] + Point2f(h_object_image.cols, 0),Scalar(255, 0, 0), 4);
	line(h_image_result, scene_corners[2] + Point2f(h_object_image.cols, 0),scene_corners[3] + Point2f(h_object_image.cols, 0),Scalar(255, 0, 0), 4);
	line(h_image_result, scene_corners[3] + Point2f(h_object_image.cols, 0),scene_corners[0] + Point2f(h_object_image.cols, 0),Scalar(255, 0, 0), 4);
	
	imshow("Good Matches & Object detection", h_image_result);
	
	waitKey(0);

	
	return 0;
}



```


### 7) OPENCV gpu 级联回归 人脸检测
```c
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp" 
#include <iostream>
#include <stdio.h>
 
using namespace std;
using namespace cv;
 
int main( )
{
    Mat h_image;
    h_image = imread("images/lena_color_512.tif", 0);  
    
    // OPENCV gpu 级联回归 人脸检测
    Ptr<cuda::CascadeClassifier> cascade = cuda::CascadeClassifier::create("haarcascade_frontalface_alt2.xml");
    
    cuda::GpuMat d_image;
    cuda::GpuMat d_buf;
    d_image.upload(h_image);
    
    //cascadeGPU->setMinNeighbors(0);
    //cascadeGPU->setScaleFactor(1.01);
    cascade->detectMultiScale(d_image, d_buf);// 多尺度检测
    
    // 转换检测结果
    std::vector<Rect> detections;
    cascade->convert(d_buf, detections);
    if (detections.empty())
        std::cout << "No detection." << std::endl;
    
    //转换成彩色图
    cvtColor(h_image,h_image,COLOR_GRAY2BGR);
    for(int i = 0; i < detections.size(); ++i)
    {
      // 画矩形框===== 
      rectangle(h_image, detections[i], Scalar(0,255,255), 5);
    }

    imshow("Result image", h_image);
     
waitKey(0);   
return 0;
}


```


### 8) OPENCV gpu 级联回归 人脸检测 处理视频
```c
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap(0);// 打开摄像头
    if (!cap.isOpened())
    {
        cerr << "Can not open video source";
        return -1;
    }
    
    std::vector<cv::Rect> h_found;// cpu 目标框
    // OPENCV gpu 级联回归 人脸检测
    cv::Ptr<cv::cuda::CascadeClassifier> cascade = 
                  cv::cuda::CascadeClassifier::create("haarcascade_frontalface_alt2.xml");
		  
    cv::cuda::GpuMat d_frame, d_gray, d_found;
    
    while(1)
    {
        Mat frame;
        if ( !cap.read(frame) ) 
	{
            cerr << "Can not read frame from webcam";
            return -1;
        }
        d_frame.upload(frame);// 上传帧图像到GPU
	// 转换成灰度图
        cv::cuda::cvtColor(d_frame, d_gray, cv::COLOR_BGR2GRAY);
        
	// 多尺度人脸检测
        cascade->detectMultiScale(d_gray, d_found);
	
	// 转换结果
        cascade->convert(d_found, h_found);
        
	for(int i = 0; i < h_found.size(); ++i)
	{
	  // 画矩目标形框===== 
          rectangle(frame, h_found[i], Scalar(0,255,255), 5);
	}

        imshow("Result", frame);
        if (waitKey(1) == 'q') {
            break;
        }
    }

    return 0;
}


```


### 9) OPENCV gpu 级联回归 人眼检测
```c
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp" 
#include <iostream>
#include <stdio.h>
 
using namespace std;
using namespace cv;
 
int main( )
{
    Mat h_image;
    h_image = imread("images/lena_color_512.tif", 0);  // 灰度图格式
    
    // OPENCV gpu 级联回归 人脸检测
    Ptr<cuda::CascadeClassifier> cascade = cuda::CascadeClassifier::create("haarcascade_eye.xml");
    
    cuda::GpuMat d_image;
    cuda::GpuMat d_buf;
    d_image.upload(h_image);
    
    //cascadeGPU->setMinNeighbors(0);
    //cascadeGPU->setScaleFactor(1.01);
    cascade->detectMultiScale(d_image, d_buf);
    
    std::vector<Rect> detections;
    cascade->convert(d_buf, detections);// 转换结果
    
    if (detections.empty())
        std::cout << "No detection." << std::endl;
	
    // 转换成彩色图格式，方便绘制彩色框
    cvtColor(h_image,h_image,COLOR_GRAY2BGR);
    for(int i = 0; i < detections.size(); ++i)
    {
      // 画矩目标形框===== 
      rectangle(h_image, detections[i], Scalar(0,255,255), 5);
    }

    imshow("Result image", h_image);
     
waitKey(0);   
return 0;
}


```


### 10) gpu 背景减除进行目标检测  背景/前景 检测
```c
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;
int main()
{
    VideoCapture cap("abc.avi");
    if (!cap.isOpened())
    {
        cerr << "can not open camera or video file" << endl;
        return -1;
    }
    Mat frame;
    cap.read(frame);
    GpuMat d_frame;
    d_frame.upload(frame);
	
    // MOG算法，即高斯混合模型分离算法，全称Gaussian Mixture-based Background/Foreground Segmentation Algorithm
    Ptr<BackgroundSubtractor> mog = cuda::createBackgroundSubtractorMOG();
    GpuMat d_fgmask,d_fgimage,d_bgimage;
    Mat h_fgmask,h_fgimage,h_bgimage;
    mog->apply(d_frame, d_fgmask, 0.01);
    while(1)
    {
        cap.read(frame);
        if (frame.empty())
            break;
        d_frame.upload(frame);// 上传到GPU
	
        int64 start = cv::getTickCount();// 计时
	
        mog->apply(d_frame, d_fgmask, 0.01);// 运动检测
        mog->getBackgroundImage(d_bgimage);// 获取静态背景
	
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
        std::cout << "FPS : " << fps << std::endl;
	
        d_fgimage.create(d_frame.size(), d_frame.type());
        d_fgimage.setTo(Scalar::all(0));
        d_frame.copyTo(d_fgimage, d_fgmask);// 按mask拷贝
	
        d_fgmask.download(h_fgmask);
        d_fgimage.download(h_fgimage);
        d_bgimage.download(h_bgimage);
	
        imshow("image", frame);
	
        imshow("foreground mask", h_fgmask);// 前景
        imshow("foreground image", h_fgimage);
        imshow("mean background image", h_bgimage);// 背景
        if (waitKey(1) == 'q')
            break;
    }

    return 0;
}


```


### 11) GMG 统计背景图像估计和每像素贝叶斯分割
```c
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/cudabgsegm.hpp"
#include "opencv2/cudalegacy.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;

int main(
)
{
    VideoCapture cap("abc.avi");
    if (!cap.isOpened())
    {
        cerr << "can not open video file" << endl;
        return -1;
    }
    Mat frame;
    cap.read(frame);
    GpuMat d_frame;
    d_frame.upload(frame);
    
    // 背景去除
    Ptr<BackgroundSubtractor> gmg = cuda::createBackgroundSubtractorGMG(40);
    GpuMat d_fgmask,d_fgimage,d_bgimage;
    Mat h_fgmask,h_fgimage,h_bgimage;
    
    gmg->apply(d_frame, d_fgmask);
    
    while(1)
    {
        cap.read(frame);
        if (frame.empty())
            break;
        d_frame.upload(frame);
        int64 start = cv::getTickCount();
	
        gmg->apply(d_frame, d_fgmask, 0.01);
	
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
        std::cout << "FPS : " << fps << std::endl;
	
        d_fgimage.create(d_frame.size(), d_frame.type());
        d_fgimage.setTo(Scalar::all(0));
        d_frame.copyTo(d_fgimage, d_fgmask);
        d_fgmask.download(h_fgmask);
        d_fgimage.download(h_fgimage);
	
        imshow("image", frame);
        imshow("foreground mask", h_fgmask);
        imshow("foreground image", h_fgimage);
        if (waitKey(30) == 'q')
            break;
    }
    return 0;
}


```

##  f 
[pycuda ](https://github.com/PacktPublishing/Hands-On-GPU-Accelerated-Computer-Vision-with-OpenCV-and-CUDA/tree/master/Chapter11)

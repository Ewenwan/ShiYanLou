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




## c 图像读取、显示形状、播放视频、add、sub、颜色空间转换、阈值操作等

### 1)
```c



```


### 2)
```c



```


### 3)
```c



```


### 4)
```c



```


### 5)
```c



```


### 6)
```c



```


### 7)
```c



```

## c 图像读取、显示形状、播放视频、add、sub、颜色空间转换、阈值操作等
### 1)
```c



```


### 2)
```c



```


### 3)
```c



```


### 4)
```c



```


### 5)
```c



```


### 6)
```c



```


### 7)
```c



```


### 8)
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

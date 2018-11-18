# CUDA编程
[参考](https://www.cnblogs.com/cuancuancuanhao/category/1236273.html)

[参考2](https://blog.csdn.net/fishseeker/article/details/75093166)

[参考3](https://bbs.csdn.net/topics/390798229)

[The CMake version of cuda_by_example ](https://github.com/Ewenwan/cuda_by_example)

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
// 以线程格（Grid）的形式组织，每个线程格由若干个线程块（block）组成，
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



# CUDA 设备 
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


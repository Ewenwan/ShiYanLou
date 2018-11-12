# CUDA编程
[参考](https://www.cnblogs.com/cuancuancuanhao/category/1236273.html)


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


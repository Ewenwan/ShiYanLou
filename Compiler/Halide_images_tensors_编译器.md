# Halide
a language for fast, portable computation on images and tensors

Halide是用C++作为宿主语言的一个图像处理相关的DSL(Domain Specified Language)语言，全称领域专用语言。主要的作用为在软硬层面上(与算法本身的设计无关)实现对算法的底层加速，我们有必要对其有一定的了解。因为不论是 传统的图像处理方法亦或是深度学习应用 都使用到了halide的思想。

其中，在OpenCV(传统图像处理库)中部分算法使用了Halide后端，而TVM(神经网络编译器)也是用了Halide的思想去优化神经网络算子。

同样的一个算法处理(局部拉普拉斯变换)，使用直接的C++语言写出来算法速度很慢，Adobe公司使用3个月对这个算法进行了优化(手工优化)使这个算法的速度快了10倍，但是如果你使用了Halide，只需要几行代码，就可以使这个算法比之前普通直接的算法快上20倍。

一句话来说，Halide大大节省了我们手动优化底层算法的时间，让我们只需要关心算法的设计。


[官网](https://halide-lang.org/)

[Halide_CVPR_intro pdf](https://halide-lang.org/assets/lectures/Halide_CVPR_intro.pdf)

[github](https://github.com/halide/Halide)

Halide is a programming language designed to make it easier to write high-performance image and array processing code on modern machines. Halide currently targets: 
* CPU architectures: X86, ARM, MIPS, Hexagon, PowerPC
* Operating systems: Linux, Windows, macOS, Android, iOS, Qualcomm QuRT
* GPU Compute APIs: CUDA, OpenCL, OpenGL, OpenGL Compute Shaders, Apple Metal, Microsoft Direct X 12


Halide’s answer: decouple algorithm from schedule 分离计算和调度

Algorithm: what is computed  

Schedule: where and when it’s computed 
 
* Easy for programmers to build pipelines simpliﬁes algorithm codeimproves modularity
* Easy for programmers to specify & explore optimizations fusion, tiling, parallelism, vectorization can’t break the algorithm
* Easy for the compiler to generate fast code

## Halide为什么可以优化算法

Halide的特点是其图像算法的计算的实现（Function和Expression）和这些计算在计算硬件单元上的调度（Schedule）是分离的，其调度以Function为单位。最终将整个图像算法转换为高效率的多层for循环，for循环的分部数据范围划分和数据加载都是由Halide来完成的，而且可以实现数据的加载和算法计算的Overlay，掩盖数据加载导致的延迟。Halide的Schedule可以由程序员来指定一些策略，指定硬件的buffer大小，缓冲线的相关设置，这样可以根据不同的计算硬件的特性来实现高效率的计算单元的调度，而图像算法的计算实现却不需要修改。


决定算法在某个硬件平台上执行时性能的“三角力量”如下：

其中，算法本身的设计是一方面，一个好的算法往往效率会高很多。而另外一个方面就是算法中计算顺序的组织，而Halide可以改变的就是我们算法在某个硬件平台上的计算顺序：

## 例子
首先我们设计一个可以对图像进行模糊的操作函数：

https://oldpan.me/archives/learn-a-little-halide

```c

// in为输入原始图像 blury为输出模糊后的图像
void box_filter_3x3(const Mat &in, Mat &blury)
{
    Mat blurx(in.size(), in.type());

    for(int x = 1; x < in.cols-1; x ++)
        for(int y = 0 ; y < in.rows; y ++)
            blurx.at<uint8_t >(y, x) = static_cast<uint8_t>(
                    (in.at<uint8_t >(y, x-1) + in.at<uint8_t >(y, x) + in.at<uint8_t >(y, x+1)) / 3); // 每一行相邻三个像素均值

    for(int x = 0; x < in.cols; x ++)
        for(int y = 1 ; y < in.rows-1; y ++)
            blury.at<uint8_t >(y, x) = static_cast<uint8_t>(
                    (blurx.at<uint8_t >(y-1, x) + blurx.at<uint8_t >(y, x) + blurx.at<uint8_t >(y+1, x)) / 3); // 上下三行位置像素再求均值

}

```
对图像模糊操作很简单，我们首先在x轴上对每个像素点以及周围的两个点进行求和平均，然后再到y轴上进行同样的操作，这样相当于一个3×3平均卷积核对整个图像进行操作，这里就不进行详细描述了。

Adobe工程师对上述的算法在硬件层面上极致优化结果，比之前的算法快了10倍，其中用到了SIMD(单指令多数据流)、以及平铺（Tiling）、展开（Unrolling）和向量化（Vectorization）等常用技术。充分利用了硬件的性能，从而不改变算法本身设计的前提下最大化提升程序执行的速度。

* 首先我们引用Halide头文件以及其他的文件。
```c
#include "Halide.h"
#include <stdio.h>
#include <algorithm>

using namespace Halide;
```


* halide中的一些语法
  * 定义变量 
    * Var x("x"), y("y"); // 用字符串x和y为两个变量起了名字
  * 定义函数
    * 然后利用Func 定义一个待执行的function，并起名为gradient。
    * Func gradient("gradient");
  * 定义实现
  
  这时我们定义function中每个点的执行逻辑，对于(x,y)这个点执行的逻辑为x + y。
其中x和y都是Var，而x + y这个操作在赋予给gradient的时候会自动转化为Expr类型，这里可以理解为将x + y这个代数表达式的逻辑赋予了gradient，最后，通过realize函数来执行整个逻辑：
```c
gradient(x, y) = x + y;
// realize 即为实现这个操作 到了这一步才会对上述的操作进行编译并执行
Buffer<int> output = gradient.realize(4, 4);
```
这个逻辑我们用C++来表示即为：
```c
for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
        printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
    }
}
```
    
而上述实现的Halide伪代码为:

```py
produce gradient:
  for y:
    for x:
      gradient(...) = ...
None
```
Halide默认的计算顺序是行优先的，也就是x代表每一行的元素位置，y代表每一列的元素位置：
如果我们将其中y和x的计算顺序换一下：
```
// 将y的顺序提到x之前
gradient.reorder(y, x); // 列优先

```

相应的伪代码为：

```c
produce gradient_col_major:
  for x:
    for y:
      gradient_col_major(...) = ...
```
### 调度

* **循环轴 拆分 Split**

我们可以对每个维度进行拆分，假如我们依然是行优先计算，但是我们对x轴进行拆分，将其拆成一个外循环一个里循环，y轴不进行变动：
```c
Var x_outer, x_inner;
gradient.split(x, x_outer, x_inner, 2);
```
这时对应的C++代码实现为:
```c
for (int y = 0; y < 4; y++) {
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        for (int x_inner = 0; x_inner < 2; x_inner++) {
            int x = x_outer * 2 + x_inner;
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
    }
}
```
* **融合 fuse**

或者我们不进行拆分，对x和y两个轴进行融合：
```c
Var fused;
gradient.fuse(x, y, fused);
```
此时对应的C++实现代码为:
```c
for (int fused = 0; fused < 4*4; fused++) {
    int y = fused / 4;
    int x = fused % 4;
    printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
}
```









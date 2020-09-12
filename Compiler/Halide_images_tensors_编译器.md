# Halide
a language for fast, portable computation on images and tensors

Halide是用C++作为宿主语言的一个图像处理相关的DSL(Domain Specified Language)语言，全称领域专用语言。主要的作用为在软硬层面上(与算法本身的设计无关)实现对算法的底层加速，我们有必要对其有一定的了解。因为不论是 传统的图像处理方法亦或是深度学习应用 都使用到了halide的思想。

其中，在OpenCV(传统图像处理库)中部分算法使用了Halide后端，而TVM(神经网络编译器)也是用了Halide的思想去优化神经网络算子。

同样的一个算法处理(局部拉普拉斯变换)，使用直接的C++语言写出来算法速度很慢，Adobe公司使用3个月对这个算法进行了优化(手工优化)使这个算法的速度快了10倍，但是如果你使用了Halide，只需要几行代码，就可以使这个算法比之前普通直接的算法快上20倍。

一句话来说，Halide大大节省了我们手动优化底层算法的时间，让我们只需要关心算法的设计。

我们正处于一个数据密集的时代，4D广场相机、图形渲染、3D打印、图像传感器、高质量医学图像等，每天生产大量的图像数据，面对这样一个图像时代，急需要针对图像处理算法设计的高性能图像处理编程语言，在这样的需求下，Halide应运而生。 

应用当前存在的编程工具编写高性能的图像处理程序需要牺牲可读性、可移植性和模块性。这可能是由于算法描述、数据存储和计算顺序杂糅在一起所致。Halide语言将算法描述与算法中数据存储与计算执行等调度方面的内容解耦合。从而算法的描述和性能的优化可以分开进行，可以在不改动算法的情况下对算法进行调优。 

另一个方面需要回答的是：在同样的计算机下，从什么地方去挖掘和提升算法的计算性能？


算术逻辑单元（ALU）进行一次操作在能量和时间上的消耗都是最小的，然而数据之间的访问通信占用了计算花费的大部分时间和能耗。因此，在计算的过程中减少数据的移动是一个很好的优化方向。另一方面，随着摩尔定律的逐渐失效，当今已经进入多核时代，将计算任务拆分，送到多个核心中并行计算，同一时刻进行计算，成倍缩短计算时间。然而，最小的数据移动并不利于计算任务的并行，考虑到算数逻辑单元的操作相对于数据移动来说小一个甚至多个数量级，因此考虑牺牲一部分数据移动性，冗余一部分计算，通过数据移动最小、计算冗余性和并行度之间的折中来达到提高性能计算性能的目的。 


[官网](https://halide-lang.org/)

[Halide_CVPR_intro pdf](https://halide-lang.org/assets/lectures/Halide_CVPR_intro.pdf)

[github](https://github.com/halide/Halide)

[源码学习笔记](https://blog.csdn.net/luzhanbo207/category_7314346.html)

[Halide_Tutorial demo 中文注释 源码仓库 ](https://github.com/Ewenwan/Halide_Tutorial)

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

Halide的特点是其图像算法的计算的实现（Function和Expression）和这些计算在计算硬件单元上的调度（Schedule）是分离的，其调度以Function为单位。最终将整个图像算法转换为高效率的多层for循环，for循环的分部数据范围划分和数据加载都是由Halide来完成的，而且可以实现数据的加载和算法计算的Overlay，掩盖数据加载导致的延迟。Halide的Schedule可以由程序员来指定一些策略，指定硬件的buffer大小，缓冲线的相关设置，这样可以根据不同的计算硬件的特性来实现高效率的计算单元的调度，而图像算法的计算实现却不需要修改。


## Halide为什么可以优化算法

Halide的特点是其图像算法的计算的实现（Function和Expression）和这些计算在计算硬件单元上的调度（Schedule）是分离的，其调度以Function为单位。最终将整个图像算法转换为高效率的多层for循环，for循环的分部数据范围划分和数据加载都是由Halide来完成的，而且可以实现数据的加载和算法计算的Overlay，掩盖数据加载导致的延迟。Halide的Schedule可以由程序员来指定一些策略，指定硬件的buffer大小，缓冲线的相关设置，这样可以根据不同的计算硬件的特性来实现高效率的计算单元的调度，而图像算法的计算实现却不需要修改。


决定算法在某个硬件平台上执行时性能的“三角力量”如下：

其中，算法本身的设计是一方面，一个好的算法往往效率会高很多。而另外一个方面就是算法中计算顺序的组织，而Halide可以改变的就是我们算法在某个硬件平台上的计算顺序：

## Halide的特点
Halide这个底层优化库有几个比较亮眼的特点：

* 1. Explicit programmer control

      The compiler does exactly what you say.
      Schedules cannot influence correctness.
      Exploration is fast and easy.

      明确的程序控制，也就是说，我们如何按照这个计算的顺序(与算法本身无关)是确定的，一旦我们已经设定好就不会再改变。

* 2. Stochastic search (autotuning)

    Pick your favorite high-dimensional search.
    而自动搜索则是每个具有搜索空间的优化器都可以使用的，因为每次进行优化操作的时候，优化的因子都是不确定的，对于不同的硬件来说，不同的配置可能导致的执行速度也不一样。因此自动随机搜索空间因子是有必要的。


* 3. 元编程

Halide的思想与元编程有着密切的关系，不仅是其设计思路或者是其执行思路，都遵循了元编程的思想，也就是代码在编译之前并没有明确的执行逻辑，只有编译过后，才会形成执行逻辑。


Halide的运行有两种方式，一种是JIT的模式，另一种是AOT的模式。JIT模式使用起来比较方便，可以直接将算法和Halide的代码生成generator封装成一个类，在程序的其他部分调用这个类即可。在嵌入式环境和交叉编译环境下一般使用AOT模式，此时需要调用compiler函数将算法代码和Halide的代码生成generator编译位目标机器的代码，生成一个.o目标文件和.h头文件。然后在独立的目标机器的应用的工程的源代码中通过头文件调用算法实现的计算函数，并在build的时候链接上.o文件，这样就得到一个可以在目标机器上运行的用Halide实现算法的程序了。一般DSP上都是这种方式来做的。

Halide的利用范围很广，我之所以想要深入了解Halide是因为使用了TVM库，TVM借助了Halide的思想去实现神经网络算子的优化并且取得了不错的效果。


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
但是要知道，上述拆分和融合操作只是对Halide所能进行的操作进行一下演示而是，这种操作方式并没有实际用处，也就是说实际中的计算顺序并没有改变。



* **平铺 tile**  相当于分块？  提高数据重复使用率 减少数据加载次数

这一步中就要进入Halide中比较重要的部分了，这一步中我们将x和y轴以4为因子间隔进行划分，并且重新对计算的路径进行重排序：
```c
Var x_outer, x_inner, y_outer, y_inner;
gradient.split(x, x_outer, x_inner, 4);
gradient.split(y, y_outer, y_inner, 4);
gradient.reorder(x_inner, y_inner, x_outer, y_outer);

// 上面的步骤其实可以简化成
gradient.tile(x, y, x_outer, y_outer, x_inner, y_inner, 4, 4);
```

对应的C++计算代码为：
```c
for (int y_outer = 0; y_outer < 2; y_outer++) {              // 两行两列个 4x4大小的快
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        for (int y_inner = 0; y_inner < 4; y_inner++) {      // 分成 4x4大小的快为单位计算
            for (int x_inner = 0; x_inner < 4; x_inner++) {
                int x = x_outer * 4 + x_inner;
                int y = y_outer * 4 + y_inner;
                printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
            }
        }
    }
}

```

这种平铺的好处是可以充分利用相邻的像素，例如在模糊中，我们会使用重叠的输入数据(也就是存在一个元素使用两次的情况)，如果采用这种计算方式，可以大大加快计算性能。

* **向量化 vector**

向量化即我们使用cpu中的SIMD技术，一次性计算多个数据，充分利用硬件的特点，例如在x86中我们可以利用SSE技术来实现这个功能。

在Halide中，我们首先将x轴的循环嵌套按照，内侧循环因子4的方式，拆分为两个(也就是内侧循环x执行四次，外侧根据总数进行计算，下例是2*4=8)，然后将内侧的x循环转化为向量的形式：
```c
Var x_outer, x_inner;
gradient.split(x, x_outer, x_inner, 4); // x轴拆分成两个轴， 内层轴以4位循环因子
gradient.vectorize(x_inner);            // 内层轴向量化， 相当于循环平铺写出来， 编译器可以识别到，会自动进行向量化

````


用C++来表示即为:

```c
for (int y = 0; y < 4; y++) {
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        // The loop over x_inner has gone away, and has been
        // replaced by a vectorized version of the
        // expression. On x86 processors, Halide generates SSE
        // for all of this.
        int x_vec[] = {x_outer * 4 + 0,
                        x_outer * 4 + 1,
                        x_outer * 4 + 2,
                        x_outer * 4 + 3};  // 生成输入向量 x坐标偏移
                        
        int val[] = {x_vec[0] + y,         // x坐标偏移 + 行偏移
                        x_vec[1] + y,
                        x_vec[2] + y,
                        x_vec[3] + y};
        printf("Evaluating at <%d, %d, %d, %d>, <%d, %d, %d, %d>:"
                " <%d, %d, %d, %d>\n",
                x_vec[0], x_vec[1], x_vec[2], x_vec[3],
                y, y, y, y,
                val[0], val[1], val[2], val[3]);
    }
}

```
可视化后就比较明显了，外部x每一行执行两次，内侧x变为向量的形式，一个指令集就可以执行完成：


* **展开 unrolling**

如果在图像中多个像素同时共享有重叠的数据，这个时候我们就可以将循环展开，从而使那些可以共享使用的数据只计算一次亦或是只加载一次。

在下面中我们将x轴拆分为内侧和外侧，因为每次内侧的数值增长都是从0到1，如果我们将内测循环的x轴展开，就不需要每次循环到这里再读取内测循环的x的值了：
```c
Var x_outer, x_inner;
gradient.split(x, x_outer, x_inner, 2);
gradient.unroll(x_inner);
```
相应的C++代码为：
```c
printf("Equivalent C:\n");
for (int y = 0; y < 4; y++) {
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        // Instead of a for loop over x_inner, we get two
        // copies of the innermost statement.
        {
            int x_inner = 0;
            int x = x_outer * 2 + x_inner;
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
        {
            int x_inner = 1;
            int x = x_outer * 2 + x_inner;
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
    }
}
```



* **融合、平铺、并行 Fusing, tiling, and parallelizing**
这一步中，我们将融合、平铺和并行操作都融合到一起，来对一个8×8的图像进行操作。首先，我们将x轴和y轴都按照4因子进行平铺操作。随后我们将外侧的y和外侧的x轴循环进行融合(2+2=4)，再将这个融合后的操作进行并行操作，也就是同时执行这四个(2+2=4)操作：
```c
Var x_outer, y_outer, x_inner, y_inner, tile_index;
gradient.tile(x, y, x_outer, y_outer, x_inner, y_inner, 4, 4);
gradient.fuse(x_outer, y_outer, tile_index); // 外层两层循环合并成 一个循环 tile_index
gradient.parallel(tile_index);
```
相应的C++代码为：
```c
// This outermost loop should be a parallel for loop, but that's hard in C.
for (int tile_index = 0; tile_index < 4; tile_index++) {
    int y_outer = tile_index / 2; // 合成循环 分别计算 合成前的两层循环 因子 
    int x_outer = tile_index % 2;
    for (int y_inner = 0; y_inner < 4; y_inner++) {
        for (int x_inner = 0; x_inner < 4; x_inner++) {
            int y = y_outer * 4 + y_inner;
            int x = x_outer * 4 + x_inner;
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
    }
}
```
可视化后的结果，可以看到8×8中左上、左下、右上、右下四个区域是几乎同时进行的(tile_index)，而每个区域和之前tile那一节的计算方式是一样的，只不过这次换成了并行计算。

* **整合**

这次来点大点的图像，我们输入的图像大小为350 x 250，对其进行最优化的操作：

首先我们将其按照64 x 64的因子进行平铺，其次融合y轴和x轴外侧的循环操作数，最后对其进行并行操作

（这里注意下，我们可以看到350或者250并不能被64整除，这个不用担心，Halide会自动处理多余或者不够的部分）。
```c
Var x_outer, y_outer, x_inner, y_inner, tile_index;
gradient_fast
    .tile(x, y, x_outer, y_outer, x_inner, y_inner, 64, 64)
    .fuse(x_outer, y_outer, tile_index)
    .parallel(tile_index);
// 可以这样连续使用.写，因为对象函数返回的是对象本身的引用
```

这样还不够，上面我们已经将整个图像平铺为6*4个部分，而这一步中对每个平铺后的部分再进行一次平铺操作，这次将每个小块按照4×2的形式平铺为，

其中y_inner_outer分成两个(每个为y_pairs)，x_inner_outer分成四个(每个为x_vectors)，然后将每个x_vectors并行化，将y_pairs展开。
```c
Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;
gradient_fast
    .tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
    .vectorize(x_vectors)
    .unroll(y_pairs);
```

对应的c++展示代码为：
```c
for (int tile_index = 0; tile_index < 6 * 4; tile_index++) {
    int y_outer = tile_index / 4;
    int x_outer = tile_index % 4;
    for (int y_inner_outer = 0; y_inner_outer < 64/2; y_inner_outer++) {
        for (int x_inner_outer = 0; x_inner_outer < 64/4; x_inner_outer++) {
            // We're vectorized across x
            int x = std::min(x_outer * 64, 350-64) + x_inner_outer*4;
            int x_vec[4] = {x + 0,
                            x + 1,
                            x + 2,
                            x + 3};

            // And we unrolled across y
            int y_base = std::min(y_outer * 64, 250-64) + y_inner_outer*2;
            {
                // y_pairs = 0
                int y = y_base + 0;
                int y_vec[4] = {y, y, y, y};
                int val[4] = {x_vec[0] + y_vec[0],
                                x_vec[1] + y_vec[1],
                                x_vec[2] + y_vec[2],
                                x_vec[3] + y_vec[3]};

                // Check the result.
                for (int i = 0; i < 4; i++) {
                    if (result(x_vec[i], y_vec[i]) != val[i]) {
                        printf("There was an error at %d %d!\n",
                                x_vec[i], y_vec[i]);
                        return -1;
                    }
                }
            }
            {
                // y_pairs = 1
                int y = y_base + 1;
                int y_vec[4] = {y, y, y, y};
                int val[4] = {x_vec[0] + y_vec[0],
                                x_vec[1] + y_vec[1],
                                x_vec[2] + y_vec[2],
                                x_vec[3] + y_vec[3]};

                // Check the result.
                for (int i = 0; i < 4; i++) {
                    if (result(x_vec[i], y_vec[i]) != val[i]) {
                        printf("There was an error at %d %d!\n",
                                x_vec[i], y_vec[i]);
                        return -1;
                    }
                }
            }
        }
    }
}
```


哦，对了，如果用Halide来写文章一开头描述的模糊(blur)算法的话，会是这个样子：

```c
Func blur_3x3(Func input) {
  Func blur_x, blur_y;
  Var x, y, xi, yi;

  // The algorithm - no storage or order
  blur_x(x, y) = (input(x-1, y) + input(x, y) + input(x+1, y))/3;
  blur_y(x, y) = (blur_x(x, y-1) + blur_x(x, y) + blur_x(x, y+1))/3;

  // The schedule - defines order, locality; implies storage
  blur_y.tile(x, y, xi, yi, 256, 32)
        .vectorize(xi, 8).parallel(y);
  blur_x.compute_at(blur_y, x).vectorize(x, 8);

  return blur_y;
}
```




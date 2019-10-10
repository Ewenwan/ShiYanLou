 # Halide 高效的图像处理语言 简化图像编程
 
 [github源码](https://github.com/halide/Halide)

Halide是用C++作为宿主语言的一个图像处理相关的DSL(Domain Specified Language)语言，全称领域专用语言。
主要的作用为在软硬层面上(与算法本身的设计无关)实现对算法的底层加速，我们有必要对其有一定的了解。
因为不论是 传统的图像处理方法亦或是深度学习应用 都使用到了halide的思想。

在OpenCV(传统图像处理库)中部分算法使用了Halide后端，而TVM(神经网络编译器)也是用了Halide的思想去优化神经网络算子。

只要我们稍微留意一下Instagram便不难发现，图像处理软件有着的广阔的市场前景。
Facebook正在试图用10亿元收购这家从事图像处理业务的公司。以往人们主要通过计算机来向网络上传照片；
但现在，越来越多的人却选择直接通过手机发送。因此图像处理也逐渐在移动终端变得流行。
同时，现在的数码图片普遍非常大，不借助好的软件工具，即使是在台式机上也需要花费很长的时间业处理。
工程师可以通过一定的技巧来加速图像处理算法，但不幸的是，这些技巧会使代码几乎没有可读性，也很难被复用。
想要向图像处理程序中增加一个函数，或者将算法移植到一个不同的平台上，往往需要彻底地修改整个代码。


MIT计算机科学和人工智能实验室（CSAIL）的研究人员开发了一门名为“Hlaide”的新的编程语言，旨在改变图像处理编程的现状。
比起用传统语言编写的代码，用Halide编写的程序不仅更容易阅读、编写和修改，
而且，由于Halide能够自动优化代码（对常规的编程语言，这个过程需要花费数小时来手工完成），因此，程序的运行速度也显著提高。
在测试中，MIT的研究人员用Halide重写了几个常用的图像处理算法（这些算法都事先经过熟练的程序员优化）。
Halide版本的算法在代码量上大体要多出1/3，但性能却有极大的提升，加速比能够加速达2倍、3倍甚至6倍。
在一个算法中，Halide程序虽然比原程序长，但却取得是了70倍的加速比。
Jonathan Ragan-Kelley（电子工程与计算机科学技术学院研究生）和Andrew Adams（CSAIL博士后）领导了Halide的开发，
并且还将原代码在网上公布。在这个月的Siggraph（最重要的图形会议）上，他们提交了一篇关于Halide的文章，
这篇文章由他俩与MIT计算工教授Saman Amarasingh、Fredo Durand，以及他们在Adobe和斯坦福的同事共同写的。

## 并行流水线

图像处理之所以需要如此大的计算量的原因之一是，它通常需要一系列离散操作。当光打到手机摄像头的传感器上之后，手机扫描图像数据，查找可能导致异常像素点的数据并加以纠正。之后，将传感器信息转化为像素的颜色信息，并进行颜色校正以和比对度高速高速，以使图像更接近人眼的观看效果。至此，手机已经进行了大量的处理工作，最后还要再对所有数据进行一次清理处理。

这么工作也仅仅是使图像在手机屏幕上得以显示。之后，我们还需要更多的处理步骤，以便用软件完成诸去除红眼、柔化阴影或增加饱和度（甚至是使图像看起来更有老式一次性照片的感觉）等操作。此外，高级的修改操作常常需要软件能够撤销现有操作，恢复到上一阶段的状态。

在今天的多核心芯片上，将图像的不同区域分配到不同的计算核心上并行执行，能够使图像处理更加高效。但是，通常各个核心在完成计算任务后，会将结果返回给主存。由于数据传输要比计算慢很多，所以这会抵销并行化带来的性能提升。

因此，软件工程师试图使每个核心在将它们的计算结果返回到主存前，尽可能的保持忙碌。这意味着，在最终汇总所有数据前，每个计算核心都必须在它所分得的数据上，执行图像处理流水线中的若干个步骤。高效的图像处理代码之所以难以编写，原因在于我们必须跟踪不同核心上处理的像素点之间的所有依赖关系。计算核心数量、计算核心的处理能力、分配给每个核心的局部内存大小以及核心何时将数据移出芯片外，在这些因素的选择之间必须作出折衷，而折衷方案则会因机器而异。因此一台设备而言优化的程序，换在另一台机器上可能起不到加速效果。


## 分而治之

使用Halide，程序员也还得自己考虑在特定机器上高效实现并行化的方案，但是仅就描述图像处理算法而言，程序员则完全不用担心这些。一个Halide程序有两部分组成：一部分是算法，另部分是“调度“（schedule）。调度指定了每个计算核心在各个处理步骤中需要处理的图像块的大小和形状。同时，它还指定的数据的依赖性——例如，某个核心上的某个处理步骤需要用到在另外的核心上之前步骤的计算结果。一旦调度确定，Halide将自动处理调度中描述的操作。

程序员如果想要将程序移植到一台不同的机器上，他不需要改变算法描述，只需变更调度。程序员如果想要向流水线中增加一个新的处理步骤，也只需要插入一个关于这个操作过程的新的描述，而无需修改已有的代码。（但是，流水线中的新步骤需要在调度中有一个与之对应的设定）

“当你有一个想法，想要将某个操作步骤而并行化，或者需要将某些步骤作某种修改时，如果你需要手动编码实现，那么你会发现，想要正确地表达自己的这个想法是件很困难的事情。”Ragan_Kelley说，“如果你有一个最的优化思路想要应用，很可能你必须花上三天时间去调度，因为你必须将原算法的操作步骤打散。而利用Halide，你只需理性一行代码就能正确地表达出你的想法。”

虽然比起普通的画像处理程序，Halide程序更易于编写和阅读，但是，由于对并行化的调度是自动完成的，因此，相当比大多数精心手工编写的代码，它们仍然能够经常获得性能上的提升。更重要的是，Halide代码非常容易修改，它使程序员能够轻松地通过实验去验证自己初步的想法能否提升性能。

“你可以随心所欲的尝试不同的整改，并且你总会发现一些好的东西，”Adams说，“之后，当你经过深思熟虑之后，你将会发现它们为什么好的道理。”

“使用一个领域相关的语言——像他们现在选择去研究开发的这门语言——能够带来许多激动人心的事情，但是，成功的例子屈指可数，”John Owens，加州理工大学戴维斯分校的一位电气与计算机工程助理教授如是说，“我认为这将会是一个美妙的成功故事。它具备你期望从一个完整系统上找到的所有的部件，而且它所针对的确实一个非常重要的领域。” 

## Halide为什么可以优化算法

Halide的特点是其图像算法的计算的实现（Function和Expression）和这些计算在计算硬件单元上的调度（Schedule）是分离的，其调度以Function为单位。
最终将整个图像算法转换为高效率的多层for循环，for循环的分部数据范围划分和数据加载都是由Halide来完成的，
而且可以实现数据的加载和算法计算的Overlay，掩盖数据加载导致的延迟。Halide的Schedule可以由程序员来指定一些策略，
指定硬件的buffer大小，缓冲线的相关设置，这样可以根据不同的计算硬件的特性来实现高效率的计算单元的调度，而图像算法的计算实现却不需要修改。


> **元编程**

Halide的思想与元编程有着密切的关系，不仅是其设计思路或者是其执行思路，都遵循了元编程的思想，也就是代码在编译之前并没有明确的执行逻辑，只有编译过后，才会形成执行逻辑。


## 使用简介

[参考](https://oldpan.me/archives/learn-a-little-halide)

```c

Halide 基本语法


Halide 关键字

Algorithm部分：

Halide::func 对应图像流水线处理中的一个步骤。这个func定义了一个图像中每一个像素应该是什么值。这里只是一个定义。
Halide::Var 用于定义func的变量。
Halide::Expr 用于定义一个表达式。
Halide::cast 强制类型转换。
Halide::min 
Halide::Buffer<T>用于定义缓存

Schedule部分：Vectorize，parallelize，unroll
Halide在定义func的时候没有真正计算每个像素点的值，只有在调用func的realize方法时才会真正执行。
Halide Debug手段

_.compile_to_lowered_stmt("_.html", {}, HTML)可以将中间结果生成html预览。
trace_stores用于跟踪执行过程。
在func中直接print部分内容。
cout可以打印Expr的具体表达式内容。
_.print_loop_nest()可以将调度的伪代码打印出来。



```


```c
// 定义函数
Func gradient("gradient"); // 利用Func 定义一个待执行的function，并起名为gradient

// 定义变量
Var x("x"), y("y"); // 利用Halide定义两个变量，这两个变量单独使用时没有任何意义，同时我们用字符串x和y为两个变量起了名字

// 定义表达式  Expr

// 这时我们定义function中每个点的执行逻辑，对于(x,y)这个点执行的逻辑为x + y。
// 其中x和y都是Var，而x + y这个操作在赋予给gradient的时候会自动转化为Expr类型，这里可以理解为将x + y这个代数表达式的逻辑赋予了gradient，
// 最后，通过realize函数来执行整个逻辑：

gradient(x, y) = x + y;
// realize 即为实现这个操作 到了这一步才会对上述的操作进行编译并执行
Buffer<int> output = gradient.realize(4, 4);

// 这个逻辑我们用C++来表示即为：
for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
        printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
    }
}

// 而上述实现的Halide伪代码为:
produce gradient:
  for y:
    for x:
      gradient(...) = ...
      
// Halide默认的计算顺序是行优先的，也就是x代表每一行的元素位置，y代表每一列的元素位置

// 如果我们将其中y和x的计算顺序换一下：
// 将y的顺序提到x之前
gradient.reorder(y, x);
//最终的计算过程就为列优先：

//相应的伪代码为：
produce gradient_col_major:
  for x:
    for y:
      gradient_col_major(...) = ...
      
      
// 定义图像  Image


```

> **demo示例**

```c

// Halide.h包含了整个Halide, 只需要include这个头文件即可
#include "Halide.h"
//c头文件是为了使用c函数
#include <stdio.h>

int main(int argc, char **argv) {

    //定义函数
    Halide::Func gradient;
    //定义变量
    Halide::Var x, y;
    //定义计算
    Halide::Expr e = x + y;

    gradient(x, y) = e;
    //获取计算结果
    Halide::Buffer<int32_t> output = gradient.realize(800, 600);

    //验证计算结果
    for (int j = 0; j < output.height(); j++) {
        for (int i = 0; i < output.width(); i++) {
            if (output(i, j) != i + j) {
                printf("Something went wrong!\n"
                       "Pixel %d, %d was supposed to be %d, but instead it's %d\n",
                       i, j, i+j, output(i, j));
                return -1;
            }
        }
    }
    printf("Success!\n");
    return 0;
}

```



> **拆分 Split**

//我们可以对每个维度进行拆分，假如我们依然是行优先计算，但是我们对x轴进行拆分，将其拆成一个外循环一个里循环，y轴不进行变动：

```c
Var x_outer, x_inner;

gradient.split(x, x_outer, x_inner, 2);

这时对应的C++代码实现为:

for (int y = 0; y < 4; y++) {
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        for (int x_inner = 0; x_inner < 2; x_inner++) {
            int x = x_outer * 2 + x_inner;
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
    }
}
```

> **融合 fuse**

```c
或者我们不进行拆分，对x和y两个轴进行融合：

Var fused;

gradient.fuse(x, y, fused);

此时对应的C++实现代码为:

for (int fused = 0; fused < 4*4; fused++) {
    int y = fused / 4;
    int x = fused % 4;
    printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
}

````


> **平铺 tile 分块计算**   

这一步中就要进入Halide中比较重要的部分了，这一步中我们将x和y轴以4为因子间隔进行划分，并且重新对计算的路径进行重排序：

```c
Var x_outer, x_inner, y_outer, y_inner;
gradient.split(x, x_outer, x_inner, 4);
gradient.split(y, y_outer, y_inner, 4);
gradient.reorder(x_inner, y_inner, x_outer, y_outer);

// 上面的步骤其实可以简化成
gradient.tile(x, y, x_outer, y_outer, x_inner, y_inner, 4, 4);

// 对应的C++计算代码为：
for (int y_outer = 0; y_outer < 2; y_outer++) {
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        for (int y_inner = 0; y_inner < 4; y_inner++) {
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



> **向量化 vector**

向量化即我们使用cpu中的SIMD技术，一次性计算多个数据，充分利用硬件的特点，例如在x86中我们可以利用SSE技术来实现这个功能。

在Halide中，我们首先将x轴的循环嵌套按照，内侧循环因子4的方式，拆分为两个(也就是内侧循环x执行四次，外侧根据总数进行计算，下例是2*4=8)，然后将内侧的x循环转化为向量的形式：

```c
Var x_outer, x_inner;
gradient.split(x, x_outer, x_inner, 4);
gradient.vectorize(x_inner);
// 用C++来表示即为:
for (int y = 0; y < 4; y++) {
    for (int x_outer = 0; x_outer < 2; x_outer++) {
        // The loop over x_inner has gone away, and has been
        // replaced by a vectorized version of the
        // expression. On x86 processors, Halide generates SSE
        // for all of this.
        int x_vec[] = {x_outer * 4 + 0,
                        x_outer * 4 + 1,
                        x_outer * 4 + 2,
                        x_outer * 4 + 3};
        int val[] = {x_vec[0] + y,
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



````

可视化后就比较明显了，外部x每一行执行两次，内侧x变为向量的形式，一个指令集就可以执行完成：


> **展开 unrolling**

如果在图像中多个像素同时共享有重叠的数据，这个时候我们就可以将循环展开，从而使那些可以共享使用的数据只计算一次亦或是只加载一次。

在下面中我们将x轴拆分为内侧和外侧，因为每次内侧的数值增长都是从0到1，如果我们将内测循环的x轴展开，就不需要每次循环到这里再读取内测循环的x的值了：

```c

Var x_outer, x_inner;
gradient.split(x, x_outer, x_inner, 2);
gradient.unroll(x_inner);
相应的C++代码为：
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



> **融合、平铺、并行 Fusing, tiling, and parallelizing**

这一步中，我们将融合、平铺和并行操作都融合到一起，来对一个8×8的图像进行操作。首先，我们将x轴和y轴都按照4因子进行平铺操作。随后我们将外侧的y和外侧的x轴循环进行融合(2+2=4)，再将这个融合后的操作进行并行操作，也就是同时执行这四个(2+2=4)操作：

```c
Var x_outer, y_outer, x_inner, y_inner, tile_index;
gradient.tile(x, y, x_outer, y_outer, x_inner, y_inner, 4, 4);
gradient.fuse(x_outer, y_outer, tile_index);
gradient.parallel(tile_index);

相应的C++代码为：

// This outermost loop should be a parallel for loop, but that's hard in C.
for (int tile_index = 0; tile_index < 4; tile_index++) {
    int y_outer = tile_index / 2;
    int x_outer = tile_index % 2;
    for (int y_inner = 0; y_inner < 4; y_inner++) {
        for (int x_inner = 0; x_inner < 4; x_inner++) {
            int y = y_outer * 4 + y_inner;
            int x = x_outer * 4 + x_inner;
            printf("Evaluating at x = %d, y = %d: %d\n", x, y, x + y);
        }
    }



```
可视化后的结果，可以看到8×8中左上、左下、右上、右下四个区域是几乎同时进行的(tile_index)，而每个区域和之前tile那一节的计算方式是一样的，只不过这次换成了并行计算：


> **整合**

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

这样还不够，上面我们已经将整个图像平铺为6*4个部分，而这一步中对每个平铺后的部分再进行一次平铺操作，这次将每个小块按照4×2的形式平铺为，其中y_inner_outer分成两个(每个为y_pairs)，x_inner_outer分成四个(每个为x_vectors)，然后将每个x_vectors并行化，将y_pairs展开。
```c
Var x_inner_outer, y_inner_outer, x_vectors, y_pairs;
gradient_fast
    .tile(x_inner, y_inner, x_inner_outer, y_inner_outer, x_vectors, y_pairs, 4, 2)
    .vectorize(x_vectors)
    .unroll(y_pairs);


对应的c++展示代码为：
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


> **模糊(blur)**


```c
如果用Halide来写文章一开头描述的模糊(blur)算法的话，会是这个样子：
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

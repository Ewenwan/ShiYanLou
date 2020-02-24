# 编译器 编译原理

[甲骨文公司编辑器Oracle Solaris Studio 12.4 Information Library (简体中文) c/cpp用户指南 数值计算指南 代码分析器 性能分析器 线程分析器](https://docs.oracle.com/cd/E57201_01/)

[哈工大 编译原理 ](http://www.icourse163.org/course/HIT-1002123007)

[编译原理——词法分析器实现](https://www.cnblogs.com/zyrblog/p/6885922.html)

[编译技术 西电](http://www.xuetangx.com/courses/course-v1:XIYOU+20180208+sp/about)

[LLVM_proj](https://github.com/Ewenwan/llvm-project/blob/master/README.md)

[gcc五分钟系列 介绍 gcc 的基本用法 ](https://github.com/Ewenwan/gcc_five_minute)

[方舟编译器的Runtime参考实现-中科院软件研究所智能软件研究中心](https://github.com/isrc-cas/pacific)

[PL/0 语言编译系统 HomePage](https://github.com/gdut-yy/PL0)

[文言文編程語言](https://github.com/LingDong-/wenyan-lang)

[pycparser C代码解析器 AST树 ](https://github.com/Ewenwan/pycparser)

[方舟编译器开源代码 ](https://gitee.com/harmonyos/OpenArkCompiler)

[Shedskin 引擎系统 Python转 C/C++](https://github.com/Ewenwan/shedskin)

[oclint 静态代码检查 ](https://github.com/Ewenwan/oclint)

[CodeChecker 静态代码检查 ](https://github.com/Ewenwan/codechecker)

[LLVM  cmu 教案](http://www.cs.cmu.edu/afs/cs.cmu.edu/academic/class/15745-s14/public/lectures/)

[收集一些如何使用 clang 库的例子](https://github.com/loarabia/Clang-tutorial)

[代码质量分析 圈复杂度 ](https://github.com/Ewenwan/Code-Quality-Analyzer)

# 编译器
```
词法分析; 
自顶向下的语法分析; 
符号表 (symbol table); 
基于堆栈 (stack-based) 的虚拟机; 
代码生成; 
数组和对象的实现.


预处理阶段 预处理器 (cpp) 根据以字符 # 开头的命令, 修改原始的 C 程序. 
          比如 hello.c 中第一行的 #include <stdio.h> 命令告诉预处理器读取系统头文件 stdio.h 的内容, 
          并把它直接插入到程序文本中. 结果就得到另一个 C 程序, 通常是以 .i 作为文件扩展名.
          
编译阶段 编译器 (cc1) 将文本文件 hello.i 翻译成文本文件 hello.s, 它包含一个汇编语言程序. 
        汇编语言程序中的每条语句都以一种标准的文本格式确切地描述了一条低级机器语言指令.
        汇编语言是非常有用的, 因为它为不同高级语言的不同编译器提供了通用的输出语言. 
        例如, C 编译器和 Fortran 编译器产生的输出文件用的都是一样的汇编语言.
        
汇编阶段 接下来, 汇编器 (as) 将 hello.s 翻译成机器语言指令,
        把这些指令打包成一种叫做可重定位的目标程序 (relocatable object program) 的格式, 
        并将结果保存在目标文件 hello.o 中. hello.o 文件是一个二进制文件, 
        它的字节编码是机器语言指令而不是字符. 如果我们在文件编辑器中打开 hello.o 文件, 看到的将是一堆乱码.
        
链接阶段 请注意, hello 程序调用了 printf() 函数, 它是每个 C 编译器都会提供的标准 C 库中的一个函数. 
        printf() 存在于一个名为 printf.o 的单独的预编译好的目标文件中, 
        而这个文件必须以某种方式合并到我们的 hello.o 程序中. 连接器 (ld) 就负责处理这种合并. 
        结果就得到 hello 文件, 它是一个可执行目标文件 (或简称为可执行文件), 
        可以被加载到内存中, 由系统执行.

```

## GCC, G++, CPP, GPP
    随着开源运动的兴起，自由软件基金会开发了自己的开源免费的C语言编译器GNU C Compiler，简称GCC。
    GCC中提供了C Preprocessor这个C语言的预处理器，简称CPP。后来GCC又加入了对C++等其它语言的支持，
    所以他的名字也改为GNU Compiler Collection。G++则是专门用来处理C++语言的。在GNU的官方手册中，
    有一个章节叫做G++ and GCC介绍了这二者的区别。G++是GCC编译器集合的一个前端。
    关于前端、后端的概念下面有更详细的介绍。而GPP呢，这个名字比较特殊，如果你用的是Linux系统，
    可能并没有这个命令。但是在某些特殊的系统下，例如DOS，是无法创建G++这样带有特殊符号的文件名的。
    所以按照DJGPP编译器的做法，GPP其实就是G++。

LLVM与GCC
    回顾GCC的历史，虽然它取得了巨大的成功，但开发GCC的初衷是提供一款免费的开源的编译器，仅此而已。
    可后来随着GCC支持了越来越多的语言，GCC架构的问题也逐渐暴露出来。
    但GCC到底有什么问题呢？
    我们一起看看这篇文章：The Architecture of Open Source Applications: LLVM。
    http://www.aosabook.org/en/llvm.html
    LLVM的优点也正是GCC的缺点。

## 传统编译器
    传统编译器的工作原理基本上都是三段式的，可以分为前端（Frontend）、优化器（Optimizer）、后端（Backend）。
    前端负责解析源代码，检查语法错误，并将其翻译为 抽象的 语法树（Abstract Syntax Tree）。
    优化器对这一中间代码进行优化，试图使代码更高效。
    后端则负责将优化器优化后的中间代码转换为目标机器的代码，
    这一过程后端会最大化的利用目标机器的特殊指令，以提高代码的性能。

    这种三段式的结构还有一个好处，开发前端的人只需要知道如何将源代码转换为优化器能够理解的中间代码就可以了，
    他不需要知道优化器的工作原理，也不需要了解目标机器的知识。这大大降低了编译器的开发难度，使更多的开发人员可以参与进来。

    虽然这种三段式的编译器有很多有点，并且被写到了教科书上，但是在实际中这一结构却从来没有被完美实现过。
    做的比较好的应该属Java和.NET虚拟机。虚拟机可以将目标语言翻译为bytecode，
    所以理论上讲我们可以将任何语言翻译为bytecode，然后输入虚拟机中运行。
    但是这一动态语言的模型并不太适合C语言，所以硬将C语言翻译为bytecode并实现垃圾回收机制的效率是非常低的。

    GCC也将三段式做的比较好，并且实现了很多前端，支持了很多语言。
    但是上述这些编译器的致命缺陷是，他们是一个完整的可执行文件，
    没有给其它语言的开发者提供代码重用的接口。即使GCC是开源的，
    但是源代码重用的难度也比较大。


    LLVM最初是Low Level Virtual Machine的缩写，定位是一个虚拟机，但是是比较底层的虚拟机。
    它的出现正是为了解决编译器代码重用的问题，LLVM一上来就站在比较高的角度，
    制定了LLVM IR这一中间代码表示语言。LLVM IR充分考虑了各种应用场景，
    例如在IDE中调用LLVM进行实时的代码语法检查，对静态语言、动态语言的编译、优化等。

```
传统编译器架构
Frontend:前端
    词法分析、语法分析、语义分析、生成中间代码
Optimizer:优化器
    中间代码优化
Backend:后端
    生成机器码
```
    
## LLVM Compiler

[用LLVM开发新语言---官方教程](https://llvm-tutorial-cn.readthedocs.io/en/latest/)



    从上面这个图中我们发现LLVM与GCC在三段式架构上并没有本质区别。
    LLVM与其它编译器最大的差别是，它不仅仅是Compiler Collection，也是Libraries Collection。
    举个例子，假如说我要写一个XYZ语言的优化器，我自己实现了PassXYZ算法，
    用以处理XYZ语言与其它语言差别最大的地方。而LLVM优化器提供的PassA和
    PassB算法则提供了XYZ语言与其它语言共性的优化算法。
    那么我可以选择XYZ优化器在链接的时候把LLVM提供的算法链接进来。
    LLVM不仅仅是编译器，也是一个SDK。
    
```   
LLVM架构:

不同的前端后端使用统一的中间代码LLVM Intermediate Representation (LLVM IR)
如果需要支持一种新的编程语言，那么只需要实现一个新的前端
如果需要支持一种新的硬件设备，那么只需要实现一个新的后端
优化阶段是一个通用的阶段，它针对的是统一的LLVM IR，不论是支持新的编程语言，还是支持新的硬件设备，都不需要对优化阶段做修改
相比之下，GCC的前端和后端没分得太开，前端后端耦合在了一起。所以GCC为了支持一门新的语言，或者为了支持一个新的目标平台，就 变得特别困难
LLVM现在被作为实现各种静态和运行时编译语言的通用基础结构(GCC家族、Java、.NET、Python、Ruby、Scheme、Haskell、D等)

```


什么是Clang :

LLVM项目的一个子项目，基于LLVM架构的C/C++/Objective-C编译器前端。
相比于GCC，Clang具有如下优点:

          编译速度快:在某些平台上，Clang的编译速度显著的快过GCC(Debug模式下编译OC速度比GGC快3倍)
          占用内存小:Clang生成的AST所占用的内存是GCC的五分之一左右
          模块化设计:Clang采用基于库的模块化设计，易于 IDE 集成及其他用途的重用
          诊断信息可读性强:在编译过程中，Clang 创建并保留了大量详细的元数据 (metadata)，有利于调试和错误报告
          设计清晰简单，容易理解，易于扩展增强

编译过程：

          0.找到main.m文件
          1.预处理器，处理include、import、宏定义
          2.编译器编译，编译成ir中间代码
          3.后端，生成目标代码
          4.汇编
          5.链接其他动态库静态库
          6.编译成适合某个架构的代码

词法分析：

          词法分析，生成Token: $ clang -fmodules -E -Xclang -dump-tokens main.m
          将代码分成一个个小单元（token）

语法树-AST：

          语法分析，生成语法树(AST，Abstract Syntax Tree): $ clang -fmodules -fsyntax-only -Xclang -ast-dump main.m
          通过语法树，我们能知道这个代码是做什么的。

LLVM IR：

          LLVM IR有3种表示形式（本质是等价的）
                    text:便于阅读的文本格式，类似于汇编语言，拓展名.ll， $ clang -S -emit-llvm main.m
                    memory:内存格式
                    bitcode:二进制格式，拓展名.bc， $ clang -c -emit-llvm main.m
                    我们以text形式编译查看：
```asm
; Function Attrs: noinline nounwind optnone ssp uwtable
define void @test(i32, i32) #2 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %6 = load i32, i32* %3, align 4
  %7 = load i32, i32* %4, align 4
  %8 = add nsw i32 %6, %7
  %9 = sub nsw i32 %8, 3
  store i32 %9, i32* %5, align 4
  ret void
}
```

IR基本语法:

          注释以分号 ; 开头
          全局标识符以@开头，局部标识符以%开头
          alloca，在当前函数栈帧中分配内存
          i32，32bit，4个字节的意思
          align，内存对齐
          store，写入数据
          load，读取数据
          
[官方语法参考](https://llvm.org/docs/LangRef.html)



## Pass Linkage




    Apple LLVM compiler 4.2和LLVM GCC 4.2
    现在我们可以回答本文最前面我遇到的那个问题了。Apple LLVM compiler 4.2是一个真正的LLVM编译器，
    前端使用的是Clang，基于最新的LLVM 3.2编译的。LLVM GCC 4.2编译器的核心仍然是LLVM，
    但是前端使用的是GCC 4.2编译器。从LLVM的下载页面可以看出，
    LLVM从1.0到2.5使用的都是GCC作为前端，直到2.6开始才提供了Clang前端。


## 编译器中的优化技术
编译器被允许在不改变单线程程序执行结果的前提下，进行各种优化

* 公共子表达式删除(Common Subexpression Elimination)
```c
a = b * c + g; //‐‐‐‐‐‐‐‐‐‐> tmp = b * c; //公共子表达式为 b * c
d = b * c * e; // rewrite    a = tmp + g;
               //            d = tmp * e;

```

* 死代码删除(Dead Code Elimination)
```c
while (!flag); //‐‐‐‐‐‐‐‐‐‐‐> loop:
               //             jmp loop

```

* 寄存器分配(Register Allocation) ：下例将对g的2次读取优化成了1次
```c
a = g;  //‐‐‐‐‐‐‐‐‐‐‐> load %r1, 0($mem1)  // 载入g到r1寄存器为a
b += a; // rewrite     add %r2, %r2, %r1   // r2寄存器存储b，a+b后赋值给b
a = g;  //
c += a; //             add %r3, %r3, %r1   // r3寄存器存储c，a+c后赋值给c

```

* 指令调度(Instruction Scheduling) ：下例重排的后一条指令不必等待前一条的结果，减少了停顿(这里的静态流水线调度缓解了RAW Hazard )
```c
load %r0, 0($mem0)  //        load %r0, 0($mem0)
mul %r1, %r1, %r0   //‐-----> load %r2, 0($mem2)
store 0($mem1), %r1 //rewrite mul %r1, %r1, %r0
load %r2, 0($mem2)  //        mul %r3, %r3, %r2
mul %r3, %r3, %r2   //        store 0($mem1), %r1
store 0($mem3), %r3 //        store 0($mem3), %r3

```


* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```

* 
```c


```

* 
```c


```

* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```

* 
```c


```

* 
```c


```

* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```

* 
```c


```

* 
```c


```

* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```

* 
```c


```

* 
```c


```

* 
```c


```


* 
```c


```


* 
```c


```


* 
```c




* 
```c


```

* 
```c


```

* 
```c


```

* 
```c


```


* 
```c


```


* 
```c


```


* 
```c


```
```

# 编译器 编译原理

[一步步掌握 LLVM](https://www.zhihu.com/column/c_1250484713606819840)

[定点数优化：性能成倍提升  llvm-mca代码分析 时钟 流水线 ](https://zhuanlan.zhihu.com/p/149517485)

[基于python写的 简单的编译器 ](https://github.com/Phantom1003/MyCompiler)

[spa-StaticProgramAnalysis程序静态分析-书籍](https://cs.au.dk/~amoeller/spa/spa.pdf)

[北京大学 件分析技术（Software Analysis）](https://xiongyingfei.github.io/SA/2017/main.htm)

[模糊测试工具   基于llvm  在qume上仿真测试](https://github.com/Ewenwan/afl)

[使用自动代码生成技术TVM优化深度学习算子的一些思考](https://zhuanlan.zhihu.com/p/101026192)

[基于clang static analyzer的源码漏洞检测插件](https://github.com/GoSSIP-SJTU/TripleDoggy)

[编译器书籍 现代体系结构的优化编译器 高级编译器设计与实现](https://github.com/Ewenwan/compilerbook)

[甲骨文公司编辑器Oracle Solaris Studio 12.4 Information Library (简体中文) c/cpp用户指南 数值计算指南 代码分析器 性能分析器 线程分析器](https://docs.oracle.com/cd/E57201_01/)

[哈工大 编译原理 ](http://www.icourse163.org/course/HIT-1002123007)

[编译原理——词法分析器实现](https://www.cnblogs.com/zyrblog/p/6885922.html)

[编译技术 西电](http://www.xuetangx.com/courses/course-v1:XIYOU+20180208+sp/about)

[LLVM_proj](https://github.com/Ewenwan/llvm-project/blob/master/README.md)

[TVM_proj](https://github.com/Ewenwan/tvm)

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

[编译优化算法](https://www.jianshu.com/u/8866e26199f3)

[初识 LLVM ](https://xiaozhuanlan.com/topic/0542687193)

[利用LLVM，Clang制作自己的编译器   示例代码  强烈推荐](https://github.com/Ewenwan/llvm-clang-samples)

[LLVM 每日谈](https://blog.csdn.net/snsn1984/article/details/8036032)

[博客llvm-clang](https://eli.thegreenplace.net/tag/llvm-clang)

[在线编译器 汇编码](https://godbolt.org/)

[UFMG的DCC888课程 LLVM介绍 程序分析和优化 ](https://homepages.dcc.ufmg.br/~fernando/classes/dcc888/ementa/)


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
        
汇编阶段 接


## 插桩技术

插桩技术是指将额外的代码注入程序中以收集运行时的信息，可分为两种：

(1)源代码插桩[Source Code Instrumentation(SCI)]：额外代码注入到程序源代码中。

●静态二进制插桩[Static Binary Instrumentation(SBI)]：在程序执行前插入额外的代码和数据，生成一个永久改变的可执行文件。

(2)二进制插桩（Binary Instrumentation）：额外代码注入到二进制可执行文件中。

●动态二进制插桩[Dynamic Binary Instrumentation(DBI)]：在程序运行时实时地插入额外代码和数据，对可执行文件没有任何永久改变。

DBI做些什么呢

（1）访问进程的内存

（2）在应用程序运行时覆盖一些功能

（3）从导入的类中调用函数

（4）在堆上查找对象实例并使用这些对象实例

（5）Hook，跟踪和拦截函数等等


# GCC

## 编译选项

### 常用选项

* [常用指令](https://gcc.gnu.org/onlinedocs/gcc-9.2.0/gcc/Overall-Options.html#Overall-Options)
* 编译选项
    * `-c`  Compile or assemble the source files, but do not link. 
    * `-S`  Stop after the stage of compilation proper; do not assemble.
    * `-o file`  This applies regardless to whatever sort of output is being produced, whether it be an executable file, an object file, an assembler file or preprocessed C code. 
* 优化选项
    * `-O` == -O1
    * `-O2`已经为较激进优化
    * `-O3`更为激进
    * `-Os`和-O2有一定的差异性，主要表现在控制生成目标的size（为android默认优化选项）
    * `-ffast-math`
        * `-ffast-math` does a lot more than just break strict IEEE compliance. First of all, of course, it does **break strict IEEE compliance**, allowing e.g. the reordering of instructions to something which is mathematically the same (ideally) but not exactly the same in floating point. Second,  it **disables setting errno** after single-instruction math functions, which means avoiding a write to a thread-local variable (this can make a 100% difference for those functions on some architectures). Third, it makes the **assumption that all math is finite**, which means that **no checks for NaN (or zero)** are made in place where they would have detrimental effects. It is simply assumed that this isn't going to happen. Fourth, it **enables reciprocal approximations for division and reciprocal square root**. Further, it **disables signed zero** (code assumes signed zero does not exist, even if the target supports it) and **rounding math**, which enables among other things constant folding at compile-time. Last, it generates code that **assumes that no hardware interrupts can happen due to signalling/trapping math** (that is, if these cannot be disabled on the target architecture and consequently do happen, they will not be handled).
        * it includes `-fno-math-errno`, `-funsafe-math-optimizations`, `-ffinite-math-only`, `-fno-rounding-math`, `-fno-signaling-nans`, `-fcx-limited-range` and `-fexcess-precision=fast`
        * 参考：[What does gcc's ffast-math actually do?](https://stackoverflow.com/questions/7420665/what-does-gccs-ffast-math-actually-do)
* 特殊属性(attribute)
    * [属性的语法范式](https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html)

### 特殊选项

* fomit-frame-point
    * fp record the history stack of outstanding calls. Most smaller functions don't need a frame-pointer
    * larger functions MAY need one. this option allows one extra register to be available for general-purpose use. In thumb it is R7
    * 查看：[Trying to understand gcc option -fomit-frame-pointer](https://stackoverflow.com/questions/14666665/trying-to-understand-gcc-option-fomit-frame-pointer)
* fvisibility(链接选项)
    * 将库中的symbol隐藏
        * `-fvisibility=default|internal|hidden|protected`
    * 可以设置所有符号全部隐藏，但暴露部分符号：
        * 暴露的符号： `__attribute__ ((visibility ("default"))) ` and pass `-fvisibility=hidden` to the compiler
    * 也可以设置所有符号全部暴露，但部分隐藏：
        * 隐藏的符号： `__attribute__ ((visibility ("hidden"))) `
    * 参考 [how-to-hide-the-exported-symbols-name-within-a-shared-library](https://stackoverflow.com/questions/9648655/how-to-hide-the-exported-symbols-name-within-a-shared-library)
* fpic
    * Generate position independent code
    * All objects in a shared library should be fpic or not fpic(keep the same) ([GCC -fPIC option](https://stackoverflow.com/questions/5311515/gcc-fpic-option))
    * diff between `fpic` & `fPIC`
    * fpic and fPIC区别
        * Use -fPIC or -fpic to generate position independent code. Whether to use -fPIC or -fpic to generate position independent code is target-dependent. **The -fPIC choice always works, but may produce larger code than -fpic** (mnenomic to remember this is that PIC is in a larger case, so it may produce larger amounts of code). Using **-fpic option usually generates smaller and faster code, but will have platform-dependent limitations**, such as the number of globally visible symbols or the size of the code. The linker will tell you whether it fits when you create the shared library. When in doubt, I choose -fPIC, because it always works.
	    * `-fpic` Generate position-independent code (PIC) suitable for use in a shared library, if supported for the target machine. Such code accesses all constant addresses through a global offset table (GOT). The dynamic loader resolves the GOT entries when the program starts (the dynamic loader is not part of GCC; it is part of the operating system). If the GOT size for the linked executable exceeds a machine-specific maximum size, you get an error message from the linker indicating that -fpic does not work; in that case, recompile with -fPIC instead. (These maximums are 8k on the SPARC, 28k on AArch64 and 32k on the m68k and RS/6000. The x86 has no such limit.) Position-independent code requires special support, and therefore works only on certain machines. For the x86, GCC supports PIC for System V but not for the Sun 386i. Code generated for the IBM RS/6000 is always position-independent.
* `-rpath`
    * 链接选项，运行期生效
    * Add a directory to the runtime library search path. This is used when linking an ELF executable with shared objects. All -rpath arguments are concatenated and passed to the runtime linker, which uses them to locate shared objects at runtime.
* `-L`
    * 链接选项，编译期链接生效
    * `--library-path=searchdir`
    * Add path searchdir to the list of paths that ld will search for archive libraries and ld control scripts.
    * So, `-L` tells ld where to look for libraries to **link against when linking**. You use this (for example) when you're building against libraries in your build tree, which will be put in the normal system library paths by make install. `--rpath`, on the other hand, stores that path inside the executable, so that the **runtime dynamic linker can find the libraries**. You use this when your libraries are outside the system library search path.
    * 参考：[What's the difference between -rpath and -L?](https://stackoverflow.com/questions/8482152/whats-the-difference-between-rpath-and-l)
* `-Wl`
    * The `-Wl,xxx` option for gcc passes a comma-separated list of tokens as a space-separated list of arguments to the linker. So `gcc -Wl,aaa,bbb,ccc` eventually becomes a linker call `ld aaa bbb ccc`
    * 参考：[I don't understand -Wl,-rpath -Wl,](https://stackoverflow.com/questions/6562403/i-dont-understand-wl-rpath-wl)

## GDB调试

* 状态查看
    * 查看函数堆栈`bt`
    * 查看一次寄存器状态`info registers`
    * 打印变量
        * `p your_variable`
* 控制执行
    * 单步执行(step into)`step & s`
    * 单条语句(step over)`n`
    * 单步汇编指令执行`si` & `stepi`
    * 继续运行`c`
    * 添加断点`b xxx.c:line_num`，比如`b main.c:97`
    * 运行`r`
* 调试脚本
    * ` ./gdbtest  -command=gdbtest.sh`

# transpiler

* all python to cpp transpiler projects
    * [py14](https://github.com/lukasmartinelli/py14)
    * [pythran](https://github.com/serge-sans-paille/pythran)
    * [py2c](https://github.com/pradyun/py2c)
    * [transpyle](https://pypi.org/project/transpyle/#id2)
    * [cython](http://docs.cython.org/en/latest/src/tutorial/cython_tutorial.html)
    * [pyrex](http://www.cosc.canterbury.ac.nz/greg.ewing/python/Pyrex/version/Doc/About.html)
    * [nuikta](https://nuitka.net/doc/user-manual.html)
    * [shedskin](https://github.com/shedskin/shedskin)
* cpython based方案分析
    * **难验证**：基于PyObject以及PyObject衍生出来的系列function缺乏可读性，难以验证生成代码的正确性
    * **兼容性差**： 基于PyObject类型的c代码风格，很难整合其他编译器功能，包括热点分析，热点函数抓取或替换，以及基于模型的代码块分类等。
    * **性能差**： 任何对象，哪怕是简单序列操作都被PyObject化，转化的c代码本身的性能堪忧
* shedskin
    * 框架部署分析
        * 执行环境：需要在嵌入式平台中加入libgc和libpcre3库（[distributing binaries](https://shedskin.readthedocs.io/en/latest/documentation.html#distributing-binaries)）
        * [libgc](https://github.com/ivmai/bdwgc)库，license: FSF(成分较为复杂，需要深入分析)，依赖库：无
        * [libpcre3](http://www.pcre.org/)库，license：BSD，可直接商用，依赖库：无
# TVM

* 图表示
    * NNVM
        * NNVM相当于深度学习领域的LLVM，是一个神经网络中比较高级的中间表示模块，通常称为计算图。前端你框架只需要将其计算表达成NNVM中间表示，之后NNVM则统一的对图做与具体硬件和框架无关的优化。包括内存分配，数据类型和形状的推导，算子融合等。
    * Relay
        * Relay解决了静态图和动态图的矛盾。是一种专用于自动微分编程领域的特定域语言。
    * 图优化方法
        * OpFusion:算子融合
        * FoldConstant:常量折叠
        * CombineParallelConv2D:结合并行的卷积与运算
        * FoldScaleAxis:折叠缩放轴
        * AlterOpLayout:改变算子排布
        * CanonicalizeOps:规范化算子
        * EliminateCommonSubexpr:消除公共子表达式
* 算子优化
    * TVM低层次中间表达的特点
        * Halide & HalideIR
        * Auto-Tuning
        * loopy循环变换工具，多面体模型分析
        * python作为宿主语言

# LLVM源码分析

[LLVM  CMU 教案 ](www.cs.cmu.edu/afs/cs.cmu.edu/academic/class/15745-s14/public/lectures/)

[深入剖析-iOS-编译-Clang---LLVM](https://github.com/ming1016/study/wiki/深入剖析-iOS-编译-Clang---LLVM)

[LLVM_proj](https://github.com/Ewenwan/llvm-project/blob/master/README.md)

[LLVM编程索引](http://llvm.org/docs/ProgrammersManual.html)

[llvm源码浏览带跳转](http://androidxref.com/4.2.2_r1/xref/external/llvm/)

[llvm-clang-samples ](https://github.com/Ewenwan/llvm-clang-samples/tree/master/src_clang)

[llvm-tutor  A collection of LLVM passes (with tests and build scripts)](https://github.com/Ewenwan/llvm-tutor)

[Clang 中 AST 相关类简介（不定时更新）](https://blog.csdn.net/qq_23599965/article/details/94595735)

[设计文档 —— Clang CFE 内部手册](https://blog.csdn.net/qq_23599965/article/details/91046137)

[关于LLVM，这些东西你必须要知道！](https://blog.csdn.net/hypercode/article/details/53815894?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

[LLVM学习笔记——目录（含7.0更新）](https://blog.csdn.net/wuhui_gdnt/article/details/61194221)

[CLANG 分析](https://github.com/Ewenwan/llvm-project/blob/master/clang/lib/readme.md)

[使用libclang来处理c++代码用来做c++反射代码自动生成 ](https://github.com/Ewenwan/meta)

[清华大学--开源机器学习框架——计图（Jittor）--Op编译器和调优器](https://cg.cs.tsinghua.edu.cn/jittor/)

[Instrumentation Pass for LLVM    函数打桩 良心推荐](https://github.com/Ewenwan/LLVM_Instrumentation_Pass)

## LLVM开发者手册

[英文参考](http://llvm.org/docs/ProgrammersManual.html)

[中文翻译](https://blog.csdn.net/qq_23599965/article/details/88538590?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

[LLVM/Clang 学习笔记](https://github.com/Enna1/LLVM-Clang-Study-Notes)

其中IR(intermediate representation)是前端语言生成的中间代码表示，也是Pass操作的对象，它主要包含四个部分：

（1）Module：比如一个.c或者.cpp文件。

（2）Function：代表文件中的一个函数。

（3）BasicBlock：每个函数会被划分为一些block，它的划分标准是：一个block只有一个入口和一个出口。

（4）Instruction：具体的指令。

# 代码目录结构分析

LLVM的主要优化流程和主流编译器教材并无大的差异，
Analysis和Transformation是主要的部分，

1. Analysis主要用于分析优化目标获取信息，

2. Transformation则是实际去做优化变换。

3. AsmParser,IR,IRReader,BitCode都是为中间llvm汇编的操作提供相关的支持。

4. ExecutionEngine提供了解释器和即时编译的相关支持。

5. **Fuzzer是提供关于测试代码时自动生成函数参数以增加代码覆盖率测试的工具，在最新的代码中已经从llvm核心移动了编译器运行时库compiler-rt中。**

6. LTO提供连接时优化支持。

剩下的一大块主要都与提供目录机器指令集支持，操作系统平台的目标文件类型支持等功能。

7. Target目录是目标平台指令集相关内容，内部代码量很大，具体芯片指令，版本等相关信息都在其中，最新的专有功能，比如AMD,NVIDIA相关的显卡计算目标代码生成也在其中。

8. Object中包含了很多目标格式，如XCOFF/COFF/ELF/MACHO等相关数据结构的内容，其中当然也有关于动态库的数据结构。

9. ObjectYAML是关于将各种目标格式数据结构转化为YAML格式的内容，我不知道这个东西为什么会在这里也觉得没什么大的用处，估计逆向工程软件会调用到这块。

10. MC/MCA两个目录是关于机器码，机器指令相关的内容(Machine Code)。

11. **XRay是Google贡献的关于动态运行时动态插桩与解除插桩配置的工具，是个非常实用的运行时日志收集工具，集体请参考https://www.llvm.org/docs/XRay.html **

12. DebugInfo是关于在目标中生成调试信息和符号相关的内容。

13. 没有处在llvm的lib中的目录有些也和优化关系巨大，只是不会做为库被调用，比如polly目录中用于高级循环优化和数据局部性优化的polly项目。polly项目主要利用高维整数多面体的抽象代数结构来对像矩阵乘积等线性远算进行优化，效果非常显著甚至可以与专门的优化库做同一量级性能比较。更多请参考 http://polly.llvm.org/index.html 


在 IR 的加持上，llvm 提供了各种功能的 Pass，例如应用在 libfuzzer 上SanitizerCoverage 。

这是一个 llvm 自带的统计代码覆盖率的插桩方法。Libfuzzerlibfuzzer 是一个 coverage-based fuzzer，想要获得程序的覆盖率信息，就必须利用某种技术来获取程序的执行信息，

可从硬件层面实现，如 Intel pt ；

也可从 IR 层面实现插桩，即在程序中插入一些小代码段，来实现一些搜集程序执行信息、改变程序行为的功能。

插桩是程序分析一个重要的技术，llvm 自带了插桩功能的库，即 llvm pass framework，也是体现 llvm 的强大之处之一。

llvm pass 提供了各种功能的“插桩”方式：ModulePass , CallGraphSCCPass, FunctionPass , LoopPass, RegionPass。

其中 SanitizerCoverage 使用的是 ModulePass，原理是在程序控制流图的每条边都加上 

__sanitizer_cov_trace_pc_guard 这么一个小函数来记录触发次数。


## qemu 二进制程序反向分析

参考 程序安全分析平台初探：IR 与应用 https://zhuanlan.zhihu.com/p/127174468

Qemu 是一个功能齐全的虚拟机，可执行全系统仿真和用户模式仿真，包含各种架构和硬件设备的模拟执行和虚拟化。

这里只提他的一部分功能，即利用了 IR 思想，涉及到指令执行的 TCG 模块。

原理也很容易理解，为了模拟执行已经编译好的某种架构的程序，qemu 把目标代码转化为IR中间语言，
再把 IR 翻译为本机架构可执行的机器语言，实现跨架构模拟执行。

与上述 llvm 一样，在这个翻译的过程中，又可以做很多有意思的程序分析工作；

而且与 llvm 是互补的关系，llvm 需要从源码翻译为 IR ，而 qemu 多用于没有源码的二进制程序分析。

另外一点不同在于，clang 涉及的是前半部分，从源代码翻译到IR的过程，

因此它的使用方式往往只需要用抽象的高级语言，因为无论 llvm 前端对程序做了多复杂的变化，
对后端来说只需要照常把IR翻译成机器语言即可；

而使用 qemu 分析二进制程序，例如做类似插桩的工作时，失去了高级语言的抽象表达，

则不得不与更底层的部分打交道，即 CPU、寄存器、虚拟内存等。

来源，星阑科技

VEX IR又是一套中间语言。使用它的是 Valgrind 插桩框架工具；上面提到的 llvm/qemu 其实本身并不是以安全分析为出发点的平台，只是因为他们过于完善和强大，所以有很多基于他们的改进工作来做程序安全分析。而 Valgrind 则是以安全为出发点开发的插桩框架，也相对成熟流行；同类的产品还有 pin、DynamoRIO、dyninst ，这里篇幅有限，就不再详加赘述了。


## 关键字 字典
```c
1. ADCE  Aggressive Dead Code Elimination      

积极的死代码消除,是一个 LLVM transform pass，用于消除冗余代码。该 ADCE 本质上是 liveness analysis(生存性分析) 的应用，是一个 backward dataflow analysis。Aggressive 的意思是对于每一条指令，该 pass 假设该指令是 dead 除非该指令被证明是 live 的，在分析结束后所有的被认为是 dead 的指令都会被消除。
llvm/src/lib/Transforms/Scalar/ADCE.cpp 

大体上来说，该分析的流程如下：
该算法的起点是所有 terminator 指令 ( 例如 ReturnInst ), may side effecting 指令 ( 例如 StoreInst )，这些认为是 live 的
然后，利用 SSA 形式的 use-def 信息，从上述起点出发迭代，把所有能通过 use-def 链到达的指令都标记为 live
最后，没有被标记为 live 的指令就是 dead，遍历一次所有指令，把没有被标记为 live 的指令删除，DCE就完成了
该分析可以看作是一个使用了只有 2 个元素的 lattice ( bottom 是 live，top 是 dead ) 的 backward 数据流分析。


2. Alias Analysis 别名分析（又称指针分析）
是程序分析中的一类技术，试图确定两个指针是否指向内存中的同一个对象。别名分析有很多中算法，也有很多种分类的方法：flow-sensitive( 流敏感 ) vs. flow-insensitive( 流不敏感 ), context-sensitive( 上下文敏感 ) vs. context-insensitive( 上下文不敏感 ), field-sensitive ( 域敏感 ) vs. field-insensitive( 域不敏感 ), unification-based vs. subset-based, etc.

LLVM 中实现了很多别名分析算法：
llvm/src/lib/Analysis/AliasAnalysisEvaluator.cpp

basicaa - Basic Alias Analysis (stateless AA impl)
cfl-anders-aa - Inclusion-Based CFL Alias Analysis
cfl-steens-aa - Unification-Based CFL Alias Analysis
external-aa - External Alias Analysis
globals-aa - Globals Alias Analysis
scev-aa - ScalarEvolution-based Alias Analysis
scoped-noalias - Scoped NoAlias Alias Analysis
tbaa - Type-Based Alias Analysis

 LLVM 中别名分析的结果有四种：

NoAlias，两个指针之前没有直接依赖的关系时就是 NoAlias 。
   比如：两个指针指向非重叠的内存区域；两个指针只被用于读取内存 ( ? )；有一段内存空间，存在一个指针用于访问该段内存，该段内存空间被 free（释放），然后被 realloc（重新分配），另外一个指针用于访问该段内存空间，这两个指针之间为NoAlias;
   
MayAlias，两个指针可能指向同一个对象，MayAlias 是最不精确（保守）的分析结果;

PartialAlias，两个内存对象以某种方式存在重叠的部分，注意：不管两个内存对象起始地址是否相同，只要有重叠的部分，它们之间就是 PartialAlias;

MustAlias，两个内存对象互为别名;

3. AST   Abstract Syntax Tree                  抽象语法树 一个 “AST” 可能更像一个通用图而不是一棵“树”

BB Vectorization   Basic-Block Vectorization   基本块向量化

BURS   Bottom Up Rewriting System — A method of instruction selection for code generation.
                                              自底向上重写系统－一种代码生成中的指令选择方法。
CFG control flow graph  控制流程图
                                              
CSE    Common Subexpression Elimination.     共同子表达式消除。
     一种移除共同子表达式的优化。例如 (a+b)*(a+b) 有两个相同的子表达式: (a+b)。这个优化使得只做一次加法然后执行乘法（但是只针对正确且安全的计算）。
     
DAG   Directed Acyclic Graph                 有向不循环图

Derived Pointer                              一个指向对象内部的指针，使得垃圾回收器不能使用此指针进行可达性分析。
     只要一个 derived 指针存在，对应的对象的指针必须保留在 root 内，否则垃圾回收器可能会释放引用的对象。对于拷贝收集器，derived 指针具有额外的风险使得它们可能在任何安全点处无效化。这个机制被用来对抗对象指针。

DFA 数据流分析  Data Flow  Analysis
    数据流分析是一种用于在计算在某个程序点的程序状态（数据流值）的技术。基于数据流分析的典型例子有常量传播、到达定值等。
    llvm/src/include/llvm/Analysis/SparsePropagation.h
在标准的数据流分析框架中，应该有如下的组成部分:

D: 数据流分析方向，forward 还是 backward，即是前向的数据流分析还是后向的数据流分析
V, ^ : 即数据流值和交汇运算。(V, ^)需要满足半格的定义，即(V, ^)是一个半格
F: V 到 V 的传递函数族。

CalledValuePropagation 是一个 transform pass，基于 SparsePropagation 实现了对间接调用点 (indirect call sites)的被调函数的可能取值进行分析。
llvm/src/lib/Transforms/IPO/CalledValuePropagation.cpp


DSA   数据结构分析

DSE   Dead Store Elimination  可不达存储消除

FCA  First Class Aggregate    第一类集合
GC   垃圾回收。使用可达性分析替代显式的内存管理来回首不再使用的内存。  
Heap 堆 在垃圾回收中，使用可达性分析管理的内存区域

IPA   Inter-Procedural Analysis.  过程间分析。引用到过程、函数和编译单元（模块）之间发生的大量代码分析。
IPO   Inter-Procedural Opti. 过程间优化。引用到过程、函数和编译单元（模块）之间发生的大量代码优化。
ISel  instruction selection  指令选择
LCSSA Loop-Closed Static Single Assignment Form  闭环静态单赋值形式
LICM  Loop Invariant Code Motion 循环不变量
Load-VN 负荷值编号
LTO   link time opt 链接时优化
MC    机器码
Object Pointer  指向对象的指针，使得垃圾回收器可以追踪对象内的引用。这个机制用来对抗derived 指针。
PRE   Partial Redundancy Elimination  部分冗余消除
SCC   Strongly Connected Component 强链接组件
SCCP  Sparse Conditional Constant Propagation  稀疏的条件常量传播

TBAA Type-Based Alias Analysis  基于类型的别名分析

支配 Dominator：

https://blog.csdn.net/dashuniuniu/article/details/52224882

N dominates M (N dom M) <==> 在 CFG 上，从 entry node（入口节点） 到 M 的所有路径都经过 N；
真支配 (strictly dominate, sdom)，如果 N dom M 并且 N != M，则 N sdom M；
直接支配 (immediate dominate, idom)，如果N dom M 并且不存在 N’，使 N dom N’，N’ dom M，则 N idom M，独一无二的支配；

Dominator Tree 支配树，父节点是子节点的直接支配节点，支配树（dominator tree）用来表示支配信息，在树中，入口结点，并且每个结点只支配它在树中的后代结点。

支配边界（dominance frontier），当前结点所能支配的边界（区域）（并不包括该边界）。

llvm/src/lib/Analysis/IteratedDominanceFrontier.cpp


```
### 重要且有用的LLVM APIs  函数

> cast类型相关

[源码](http://llvm.org/doxygen/Casting_8h_source.html)

**RTTI(Run-Time Type Identification,运行时类型辨认)**

**1. isa<>：  isa<具体类型>(变量名)  判断该变量是否是某个具体类型**

isa<>操作符的工作原理与Java “instanceof”操作符完全相同。它返回true或false，这取决于引用或指针是否指向指定类的实例。这对于各种类型的约束检查非常有用。

实现机理：

调用 isa<Argument>(V) 时，会返回 Argument::classof(V) 的值，而前面我们看到，Argument::classof(V) 的值就是 return V->getValueID() == ArgumentVal; ，因为在构造该 Argument 对象时，已经将其基类 Value 的 SubclassID 设置为 ArgumentVal ，所以最后会返回true，即指针 V 指向的对象是一个 Argument 类型的对象。


**2.cast<>： 已知类型的强制转换**

cast<>操作符是一个“已检查的强制转换”操作。它将指针或引用从基类转换为派生类，如果不是正确类型的实例，则会导致断言失败。当你有一些信息使你相信某样东西是正确的类型时，应该使用这种方法。isa<> 和 cast<> 模板的一个例子是：

```c
static bool isLoopInvariant(const Value *V, const Loop *L) {
  if (isa<Constant>(V) || isa<Argument>(V) || isa<GlobalValue>(V))  // V是常量/参数/全局变量?
    return true;

  // Otherwise, it must be an instruction...  否则V是指令
  return !L->contains(cast<Instruction>(V)->getParent());  // 将基类Value类型的V转换cast成派生类类型的 Instruction
}
```

实现机理：

先使用 C++ const_cast 然后对 const_cast 的结果进行C风格的强制类型转换。

**3.dyn_cast<>：未知类型的强制转换**

dyn_cast<>操作符是一个“检查转换”操作。它检查操作数是否属于指定的类型，如果是，则返回指向它的指针(该操作符不与引用一起工作)。如果操作数类型不正确，则返回空指针。因此，这与c++中的dynamic_cast<>操作符非常相似，应该在相同的环境中使用。通常，dyn_cast<>操作符用于if语句或其他类似的流控制语句中：
```c
if (auto *AI = dyn_cast<AllocationInst>(Val)) { // 如果Val可以转换成 AllocationInst类型则if语句成立，否则返回NULL，if语句不成立
  // ...
}
```
这种形式的if语句有效地将对 isa<> 的调用和对 cast<> 的调用组合到一个语句中，这非常方便。
注意，dyn_cast<>操作符可以被滥用，就像c++的dynamic_cast<>或Java的instanceof操作符一样。特别是，不应该使用大的if/then/else块来检查类的许多不同变体。如果您发现自己想这样做，那么使用 InstVisitor 类直接分派指令类型会更清晰、更有效。


实现机理：

return isa<X>(Val) ? cast<X>(Val) : nullptr;
  
通过三元运算符实现的，如果 isa<X>(val) 返回 true (val是 X 类的一个对象)，则将 val cast 为 X 类后返回，否则返回空指针 nullptr 。

**4.cast_or_null<>**

cast_or_null<>操作符的工作原理与 cast<>操作符类似，只是它允许一个空指针作为参数(然后将其传播)。这有时很有用，允许您将多个null检查合并到一个检查中。

**5.dyn_cast_or_null<>**

dyn_cast_or_null<>操作符的工作原理与 dyn_cast<> 操作符类似，只是它允许一个空指针作为参数(然后将其传播)。这有时很有用，允许您将多个null检查合并到一个检查中。


了解了 LLVM 中 RTTI 的实现，我们想要让其支持自己编写的类，模仿 Value 类 和 Argument 类的写法，声明一个枚举变量，在子类构造函数中显示调用父类构造函数并传递给父类构造函数一个表示子类类型的枚举常量，还有需要定义 classof 函数。

> **传递字符串(StringRef和Twine类)**

StringRef 类的定义位于  src/include/llvm/ADT/StringRef.h

虽然LLVM通常不做太多字符串操作，但是我们有几个重要的APIs接受字符串。两个重要的例子是 Value 类（它有指令、函数等的名称）和 StringMap 类（在 LLVM 和 Clang 中广泛使用）。

这些是泛型类，它们需要能够接受可能包含空字符的字符串。因此，它们不能简单地接受const char *，而接受const std::string&要求客户机执行堆分配，这通常是不必要的。代替的是，许多LLVM APIs使用StringRef或const twine&来有效地传递字符串。

**1.StringRef类 llvm字符串引用类**

StringRef数据类型表示对常量字符串（一个字符数组和一个长度）的引用，并支持std::string上可用的公共操作，但不需要堆分配。
它可以使用一个C风格的以null结尾的字符串、一个std::string隐式地被造，也可以使用一个字符指针和长度显式地构造。例如，StringRef find函数声明为：

 iterator find(StringRef Key);

client可以用以下任意一种方式调用这个函数：Map是  StringMap 类对象
```c
Map.find("foo");                 // Lookup "foo"    C风格的以null结尾的字符串构造 StringRef 
Map.find(std::string("bar"));    // Lookup "bar"    C风格的以null结尾的字符串构造 std::string 再构造成 StringRef 
Map.find(StringRef("\0baz", 4)); // Lookup "\0baz"  一个字符指针 和 长度   显式地构造 StringRef 
```

类似地，需要返回string的APIs可能会返回一个StringRef实例，该实例可以直接使用，也可以使用str成员函数将其转换为std::string。有关更多信息，请查看 [llvm/ADT/StringRef.h](http://llvm.org/doxygen/StringRef_8h_source.html)

您应该很少直接使用StringRef类，因为它包含指向外部内存的指针，所以存储该类的实例通常是不安全的（除非您知道不会释放外部存储）。StringRef在 LLVM 中足够小和普遍，因此它应该总是通过值传递。


可以看到，StringRef 类有两个成员变量：Data 和 Length，Data是一个指向 const char 的指针，Length 用于存储字符串的长度。

与 std::string 类似，StringRef 支持 data, empty, size, startswith, endswith 等常用的函数；当然 StringRef 还支持一些 std::string 中没有的成员函数，如 equals , split, trim 等。

**2. Twine 字符串连接**

Twine类是 APIs 接受连接字符串的有效方法。例如，一个常见的LLVM范型是根据带有后缀的另一条指令的名称来命名一条指令。

Twine类实际上是一个轻量级的rope，它指向临时(分配给栈的)对象。Twine可以隐式地构造为加运算符应用于字符串的结果（即，一个C字符串，一个std::string，或者一个StringRef）。Twine会延迟字符串的实际连接，直到实际需要它时，才会有效地将其直接呈现到字符数组中。这避免了在构造字符串连接的临时结果时涉及的不必要的堆分配。有关更多信息，请查看[llvm/ADT/Twine.h](http://llvm.org/doxygen/Twine_8h_source.html)和这里。
与StringRef一样，Twine对象指向外部内存，并且几乎不应该直接存储或提及。它们仅用于在定义一个应该能够有效接受连接字符串的函数时使用。

> **格式化字符串(formatv函数)**

虽然LLVM不一定要做很多字符串操作和解析，但它确实做了很多字符串格式化。从诊断消息，到llvm工具输出(如llvm-readobj)，再到打印详细的分解清单和LLDB运行时日志，字符串格式化的需求无处不在。

formatv在本质上类似于printf，但是使用了另一种语法，这种语法大量借鉴了Python和c#。与printf不同，它推断要在编译时格式化的类型，因此不需要%d之类的格式说明符。这减少了构造可移植格式字符串的脑力开销，特别是对于size_t或指针类型等特定于平台的类型。与printf和Python不同的是，如果LLVM不知道如何格式化类型，它还不能编译。这两个属性确保函数比传统的格式化方法（如printf函数族）更安全，使用起来也更简单。

**简单格式化**

formatv调用涉及一个由0个或多个替换序列组成的格式字符串，然后是替换值的一个可变长度列表。一个替换序列是一个形式为{N[[,align]:style]}的字符串。 {列表中的第一个元素,可选对齐:风格}

N表示替换值列表中参数的基于0的索引。注意，这意味着可以以任何顺序多次引用相同的参数，可能使用不同的样式和/或对齐选项。
align是一个可选字符串，指定要将值格式化为的字段的宽度，以及字段内值的对齐方式。它被指定为一个可选的对齐样式，后跟一个正整数字段宽度。对齐样式可以是字符-(左对齐)、=(中对齐)或+(右对齐)中的一个。默认值是右对齐的。

style是一个可选字符串，由控制值格式的特定类型组成。例如，要将浮点值格式化为百分比，可以使用样式选项P。

```c
std::string S;
// 基本类型的简单格式化和隐式字符串转换。
S = formatv("{0} ({1:P})", 7, 0.35);  // S == "7 (35.00%)"  // P 是格式 percent 百分比格式  N十进制  X十六进制 E科学计数法

// 无序引用和多引用
outs() << formatv("{0} {2} {1} {0}", 1, "test", 3); // prints "1 3 test 1"

// 左、右、中对齐  align
S = formatv("{0,7}",  'a');  // S == "      a";
S = formatv("{0,-7}", 'a');  // S == "a      "; // -号左对齐
S = formatv("{0,=7}", 'a');  // S == "   a   ";
S = formatv("{0,+7}", 'a');  // S == "      a";

// 自定义样式  style
S = formatv("{0:N} - {0:x} - {1:E}", 12345, 123908342); // S == "12,345 - 0x3039 - 1.24E8"

// Adapters
S = formatv("{0}", fmt_align(42, AlignStyle::Center, 7));  // S == "  42   "  对齐格式fmt_align函数
S = formatv("{0}", fmt_repeat("hi", 3)); // S == "hihihi"                     重复函数 fmt_repeat
S = formatv("{0}", fmt_pad("hi", 2, 6)); // S == "  hi      "                 填充函数 fmt_pad  

// Ranges 范围填充
std::vector<int> V = {8, 9, 10};
S = formatv("{0}", make_range(V.begin(), V.end())); // S == "8, 9, 10"
S = formatv("{0:$[+]}", make_range(V.begin(), V.end())); // S == "8+9+10"
S = formatv("{0:$[ + ]@[x]}", make_range(V.begin(), V.end())); // S == "0x8 + 0x9 + 0xA"

```

> **错误处理**

正确的错误处理帮助我们识别代码中的错误，并帮助最终用户理解他们的工具使用中的错误。错误可以分为两大类：编程错误和可恢复性错误，它们具有不同的处理和报告策略

**编程错误 断言assert llvm_unaccessible**

1. 断言用于表示不变条件，并且应该包含描述不变条件的消息：
```c
assert(isPhysReg(R) && "All virt regs should have been allocated already.");
```
llvm_unaccessible 函数可用于记录控制流的区域，如果程序不变量保持：
```c
enum { Foo, Bar, Baz } X = foo();

switch (X) {
  case Foo: /* Handle Foo */; break;
  case Bar: /* Handle Bar */; break;
  default:
    llvm_unreachable("X should be Foo or Bar here");
}
```

**可恢复性错误**

可恢复错误表示程序环境中的错误，例如资源故障(丢失的文件、丢失的网络连接等)或格式错误的输入。应该检测这些错误，并将其传达给程序的某个级别，以便对其进行适当处理。处理错误可能与向用户报告问题一样简单，也可能涉及尝试恢复。

可恢复错误使用LLVM的错误模式建模。这个方案使用函数返回值表示错误，类似于经典的C整数错误代码，或者c++的std::error_code。然而，Error类实际上是用户定义错误类型的轻量级包装器，允许附加任意信息来描述错误。这类似于c++异常允许抛出用户定义类型的方式。
成功值是通过调用Error:: Success()创建的


> **传递函数和其他可调用对象**

> **为任务选择正确的数据结构**

LLVM /ADT/目录中有大量的数据结构，我们通常使用STL数据结构。本节描述在选择时应该考虑的权衡。
第一步是选择您自己的冒险:您想要顺序容器、类似于集合的容器还是类似于映射的容器?在选择容器时，最重要的是计划如何访问容器的算法属性。基于此，你应该使用:

  如果需要基于另一个值高效地查找值，则使用类似于映射的容器。类映射容器还支持有效的包含查询(无论键是否在映射中)。类映射容器通常不支持有效的反向映射(值到键)。如果需要，可以使用两个映射。一些类似于映射的容器还支持按顺序高效地遍历键。类映射容器是最昂贵的一种，只有在需要这些功能之一时才使用它们。
  如果您需要将一堆东西放入一个容器中，这个容器可以自动消除重复。一些类似集合的容器支持按排序顺序对元素进行有效的迭代。类集合容器比顺序容器更昂贵。
  顺序容器提供了最有效的方法来添加元素，并跟踪它们添加到集合中的顺序。它们允许复制并支持有效的迭代，但不支持基于键的高效查找。
  字符串容器是用于字符或字节数组的专用顺序容器或引用结构。

位容器提供了一种有效的方法来存储和执行数字id集上的set操作，同时自动消除重复。要存储的每个标识符，位容器最多需要1位。
一旦确定了容器的适当类别，您就可以通过明智地选择类别中的成员来微调内存使用、常量因素和缓存访问行为。请注意，常量因素和缓存行为可能很重要。例如，如果您有一个向量，它通常只包含几个元素(但是可以包含许多元素)，那么使用SmallVector比使用vector要好得多。这样做可以避免(相对)昂贵的malloc/free调用，这大大降低了向容器添加元素的成本。

> **顺序容器Sequential Containers (std::vector, std::list, etc)**
根据您的需要，可以使用各种顺序容器。在本节中选择第一个可以做您想做的事情。

**1.数据引用lvm/ADT/ArrayRef.h**
llvm::ArrayRef类是接口中使用的首选类，该接口接受内存中元素的顺序列表并从其中读取数据。通过使用ArrayRef，可以向API传递一个固定大小的数组、一个std::vector、一个llvm::SmallVector以及内存中任何相邻的内容。

**2.Fixed Size Arrays固定大小数组**
固定大小的数组非常简单和快速。如果您确切地知道您有多少个元素，或者您对您有多少个元素有一个(低)上限，那么它们是很好的。

**3.Heap Allocated Arrays堆分配数组**
堆分配数组(new[] + delete[])也很简单。如果元素的数量是可变的，如果您知道在分配数组之前需要多少元素，如果数组通常很大(如果不是，请考虑一个小向量)，那么它们是很有用的。堆分配数组的成本是new/delete(又名malloc/free)的成本。还请注意，如果使用构造函数分配类型的数组，则将为数组中的每个元素运行构造函数和析构函数(重新调整大小的向量只构造实际使用的元素)。

**4.llvm/ADT/SmallVector.h简化vecot 小向量**
是一个看起来和闻起来都像vector的简单类:它支持高效的迭代，以内存顺序排列元素(这样您就可以在元素之间执行指针算术)，支持高效的push_back/pop_back操作，支持对其元素的高效随机访问，等等。

SmallVector的主要优点是它为对象本身中的一些元素(N)分配了空间。因此，如果小向量动态地小于N，则不执行malloc。如果malloc/free调用比处理元素的代码昂贵得多，那么这将是一个巨大的胜利。

这是有利于向量”通常小”(如前辈的数量/继任者的一块通常小于8)。另一方面,这使得SmallVector本身的尺寸大,所以你不想分配很多(这样做会浪费很多空间)。因此，在堆栈上使用小向量是最有用的。

SmallVector还为alloca提供了一个很好的可移植性和高效的替代品。

SmallString是SmallVector的子类，它添加了一些方便的api，比如+=，它接受StringRef的api。SmallString避免在预分配的空间足够容纳其数据时分配内存，并且在需要时回调一般堆分配。因为它拥有自己的数据，所以使用它非常安全，并且支持字符串的完全变异。

和SmallVector一样，SmallString的最大缺点是它们的sizeof。虽然它们针对小字符串进行了优化，但它们本身并不特别小。这意味着它们对于堆栈上的临时刮擦缓冲区非常有效，但通常不应该放到堆中:很少看到SmallString作为频繁分配的堆数据结构的成员或按值返回。

> **Set-Like Containers 集合类容器 (std::set, SmallSet, SetVector, etc)**

> **Map-Like Containers (std::map, DenseMap, etc) 字典映射类容器**

> **Bit storage containers 位存储容器(BitVector, SparseBitVector)**

## 遍历程序
### module类简介 
Module类表示LLVM程序中的顶层结构。
一个LLVM Module实际上要么是原始程序的一个翻译单元，要么是链接器合并的几个翻译单元的一个组合。Module 模块包含 Functions列表 + GlobalVariables全局变量列 + SymbolTable符号表

1.一个Functions列表

Module::FunctionListType &getFunctionList()
返回Function列表。当您需要更新列表或执行没有转发方法的复杂操作时，这是必需的。

2.一个GlobalVariables全局变量列表
```c
Module::global_iterator —— 全局变量列表iterator的类型定义
Module::const_global_iterator —— const_iterator的类型定义。
global_begin(), global_end(), global_size(), global_empty()
Module::GlobalListType &getGlobalList()
返回GlobalVariables列表。当您需要更新列表或执行没有转发方法的复杂操作时，这是必需的。

```

3.一个SymbolTable符号表。此外，它还包含一些有用的成员函数，这些函数试图简化常见操作。
```c
SymbolTable *getSymbolTable()
返回对这个Module的SymbolTable的一个引用。

Function *getFunction(StringRef Name) const
在Module SymbolTable中查找指定的函数。如果不存在，返回null。

FunctionCallee getOrInsertFunction(const std::string &Name, const FunctionType *T)
在Module SymbolTable中查找指定的函数。如果它不存在，则为函数添加一个外部声明并返回它。注意，已经存在的函数签名可能与请求的签名不匹配。因此，为了支持将结果直接传递给EmitCall的常见用法，
返回类型是{FunctionType *T, Constant *FunctionPtr}的一个结构体，而不是具有潜在的意外签名的简单Function*。

std::string getTypeName(const Type *Ty)
如果指定Type的SymbolTable中至少有一个条目，则返回它。否则返回空字符串。

bool addTypeName(const std::string &Name, const Type *Ty)
在将Name映射到Ty的SymbolTable中插入一个条目。如果已经有该名称的条目，则返回true，并且不修改SymbolTable。
BasicBlock

```

```c
// 迭代器 iterator

Module::iterator     遍历模块内的函数 Function

Function::iterator   遍历函数内的基本块 BasicBlocks

BasicBlock::iterator 遍历基本块BB内的 指令 Instruction (又名 使用者 User) 
                     子类: 载入指令 存储指令 分配指令 调用指令 
                     可使用 dyn_cast<AllocationInst>(val) 进行类别转换

User::op_iterator    遍历使用者(User  指令 Instruction) 内使用的操作数op(值 Value)

Value::use_iterator  遍历值的使用者(指令等)


```

**a. 函数类 FunctionType**

Function类表示LLVM中的一个过程。它实际上是LLVM层次结构中比较复杂的类之一，因为它必须跟踪大量数据。Function类跟踪基本块列表BBtable、形式参数列表param list和符号表SymbolTable。

基本块basic block table列表是函数对象中最常用的部分。该列表强制函数中块的隐式排序，这指示代码将如何由后端布局。此外，第一个基本块是函数的隐式入口节点。在LLVM中显式地分支到这个初始块是不合法的。不存在隐式的退出节点，实际上一个函数可能有多个退出节点。

如果BasicBlock列表是空的，这表明函数实际上是一个函数声明:函数的实际主体还没有被链接进来。

除了基本块列表之外，函数类还跟踪函数接收到的形式参数列表。这个容器管理参数节点的生存期，就像BasicBlock列表管理BasicBlock一样。

SymbolTable是一个很少使用的LLVM特性，只在必须**按名称查找值**时才使用。除此之外，符号表还用于内部，以确保函数体中指令、基本块或参数的名称之间没有冲突。

注意，函数是一个全局值，因此也是一个常量。函数的值是它的地址(链接后)，它保证是常量。

```c
DerivedTypes 的子类，用于function类型。
bool isVarArg() cons：                  如果它是一个vararg函数，则返回true。
const Type * getReturnType() const：    返回函数的返回类型。
const Type * getParamType (unsigned i)：返回第i个参数的类型。  参数名字在哪里??
const unsigned getNumParams() const：   返回形式参数的数量。
FunctionType *getFunctionType()：       返回函数类型

Function(const FunctionType *Ty, LinkageTypes Linkage, const std::string &N = "", Module* Parent = 0)
//构造函数，用于在需要创建新函数来添加程序时使用。构造函数必须指定要创建的函数的类型以及函数应该具有哪种类型的链接。FunctionType参数指定函数的形式参数和返回值。同一个FunctionType值可用于创建多个函数。父参数指定定义函数的模块。如果提供了这个参数，函数将自动插入该模块的函数列表中。

bool isDeclaration ()
//返回函数是否定义了主体。如果函数是“外部的”，那么它就没有主体，因此必须通过与在不同翻译单元中定义的函数链接来解决。

Function::iterator —— 基本块列表迭代器的类型定义
Function::const_iterator —— const_iterator的类型定义。
begin(), end(), size(), empty()
这些转发方法使访问函数对象的BasicBlock列表的内容变得很容易。

Function::BasicBlockListType &getBasicBlockList()
//返回BasicBlock列表。当您需要更新列表或执行没有转发方法的复杂操作时，这是必需的。

Function::arg_iterator —— 参数列表iterator的类型定义
Function::const_arg_iterator —— const_iterator的类型定义。
arg_begin(), arg_end(), arg_size(), arg_empty()
//这些转发方法使访问函数对象的参数列表的内容变得很容易。

Function::ArgumentListType &getArgumentList()
//返回参数列表。当您需要更新列表或执行没有转发方法的复杂操作时，这是必需的。

BasicBlock &getEntryBlock()
//返回函数的入口BasicBlock。因为函数的入口块总是第一个块，所以它返回Function的第一个块。

Type *getReturnType()
FunctionType *getFunctionType()
//它遍历Function的Type并返回函数的返回类型，或实际函数的FunctionType。

SymbolTable *getSymbolTable()
//返回指向此函数的SymbolTable的指针。


```

**b. BasicBlock类**

该类表示代码的单个入口和单个出口部分，编译器社区通常将其称为基本块。

BasicBlock类维护一个Instructions指令列表，这些指令构成了块的主体。与语言定义匹配，此指令列表的最后一个元素始终是一个终止符指令。

除了跟踪组成块的指令列表外，BasicBlock类还跟踪它所嵌入的Function（调用的外部函数 函数调用指令）。

注意，BasicBlocks本身是Values，因为它们由branches之类的指令引用，所以可以放在switch表中。BasicBlocks有类型label。

BasicBlock类的重要Public成员

```c

BasicBlock(const std::string &Name = "", Function *Parent = 0)

//BasicBlock构造函数用于创建用于插入函数的新基本块。构造函数可选地接受新块的一个名称和将其插入其中的一个Function。
//如果指定了Parent参数，则在指定Function的末尾自动插入新的BasicBlock；如果没有指定，则必须手动将BasicBlock插入Function。

BasicBlock::iterator —— 指令列表iterator的类型定义
BasicBlock::const_iterator —— const_iterator的类型定义。
用于访问指令列表的begin(), end(), front(), back(), size(), empty() STL样式函数。

这些方法和typedefs是转发函数，它们具有与相同名称的标准库方法相同的语义。这些方法以易于操作的方式公开基本块的底层指令列表。要获得完整的容器操作(包括更新列表的操作)，必须使用getInstList()方法。

BasicBlock::InstListType &getInstList()
此方法用于访问实际包含指令的底层容器。当BasicBlock类中没有要执行的操作的转发函数时，必须使用此方法。因为没有用于“更新”操作的转发函数，所以如果想更新BasicBlock的内容，就需要使用这个函数。

Function *getParent()
返回一个指针，它指向这个块所嵌套的Function，或者返回一个空指针(如果它是无家可归的)。

Instruction *getTerminator()
返回一个指向出现在BasicBlock末尾的终止符指令的指针。如果没有终止符指令，或者如果块中的最后一条指令不是终止符，则返回一个空指针。

```

**c. Argument类 函数参数类**

这个Value的子类为函数的传入形式参数定义接口。一个函数维护其一个形式参数的列表。一个参数有一个指向父Function的指针。

**d. Value类 包含了我被哪些指令（LLVM User）使用**

Value类是LLVM源库中最重要的类。它表示一个类型化值，可以(除其他外)用作一条指令的操作数。有许多不同类型的Values，比如常量、参数。甚至指令和函数也是Values。

一个特定的Value可以在程序的LLVM表示中多次使用。例如，一个函数的一个传入参数(用Argument类的一个实例表示)被引用该参数的函数中的每条指令“使用”。为了跟踪这种关系，Value类保存了使用它的所有Users的一个列表(User类是LLVM图中所有可以引用Values的节点的基类)。这个use列表是LLVM在程序中表示def-use信息的方式，并且可以通过use_*方法访问，如下所示。

因为LLVM是一个类型化表示，所以每个LLVM Value都是类型化的，并且这种Type可以通过 getType() 方法获得。此外，所有LLVM values都可以被命名。Value的“name”是可在LLVM代码中打印的一个符号字符串：
%foo = add i32 1, 2

这个指令的名称是“foo”。注意，任何值的名称都可能丢失(一个空字符串)，所以名称应该只用于调试(使源代码更容易阅读，调试打印输出)，不应该用于跟踪值或在它们之间映射。为此，使用指向这个Value本身的一个std::map代替。

LLVM的一个重要方面是，SSA变量(静态单赋值 三地址)和生成它的操作之间没有区别。因此，任何对指令生成的值的引用(例如，作为传入参数可用的值)都表示为指向表示该值的类实例的直接指针。虽然这可能需要一些时间来适应，但它简化了表示，使操作更容易。

```c
Value::use_iterator       —— use-list上的iterator的类型定义   遍历使用者的迭代器
Value::const_use_iterator —— use-list上的const_iterator的类型定义
unsigned use_size()       —— 返回这个value的users数量。
bool use_empty()          —— 如果没有users，返回true。
use_iterator use_begin()  —— 获取指向use-list的开始的一个迭代器。
use_iterator use_end()    —— 获取指向use-list的结尾的一个迭代器。
User *use_back()          —— 返回列表中的最后一个元素。

这些方法是访问LLVM中的def-use信息的接口。与LLVM中的所有其他iterators一样，命名约定遵循STL定义的约定。

Type *getType() const 这个方法返回Value的Type。
bool hasName() const
std::string getName() const
void setName(const std::string &Name)

void replaceAllUsesWith(Value *V)

此方法遍历一个Value的use列表，它更改当前value的所有Users以引用“V”。例如，如果您检测到一条指令总是产生一个常量值(例如通过常量折叠)，您可以像这样用常量替换该指令的所有用法：
Inst->replaceAllUsesWith(ConstVal);


```


**e. User类 包含了我使用了哪些操作数（LLVM Value）**

是所有可能 引用Values的LLVM节点 的公共基类。它公开了一个“操作数”列表，这些“操作数”是User引用的所有Values。User类本身是Value的子类。

User的操作数直接指向它引用的LLVM Value。因为LLVM使用静态单赋值(SSA)表单，所以只能引用一个定义，从而允许这种直接连接。这个连接在LLVM中提供use-def信息。

User类以两种方式公开操作数列表：通过一个索引访问接口和一个基于iterator的接口。
```C
Value *getOperand(unsigned i);  // 通过一个索引访问接口
unsigned getNumOperands();
//一个基于iterator的接口访问
User::op_iterator       —— 操作数列表上的iterator的类型定义
op_iterator op_begin()  —— 获取指向操作数列表的开始的一个迭代器。
op_iterator op_end()    —— 获取指向操作数列表的末尾的一个迭代器。
这些方法一起组成了一个User操作数的基于iterator的接口。
```

**f. Instruction 指令类**
Instruction类是所有LLVM指令的公共基类。它只提供了几个方法，但是是一个非常常用的类。

Instruction类本身跟踪的主要数据是操作码(指令类型)和嵌入Instruction的 父BasicBlock。

为了表示一个特定类型的指令，使用了众多Instruction子类中的一个。

因为Instruction类是User类的子类，所以可以像访问其他Users一样访问它的操作数
使用 getOperand() / getNumOperands() 和

op_begin() / op_end()方法)。

Instruction类的一个重要文件是llvm/Instruction.def文件。

这个文件包含一些关于LLVM中各种不同类型指令的元数据。

它描述了用作操作码的enum值(例如，Instruction::Add和Instruction::ICmp)，以及实现该指令的具体Instruction子类(例如，BinaryOperator和CmpInst)。不幸的是，这个文件中宏的使用混淆了doxygen，所以这些enum值没有正确地显示在doxygen输出中。

**g. Instruction类的重要子类**

BinaryOperator:这个子类表示所有两个操作数指令，它们的操作数必须是相同的类型，比较指令除外。

CastInst:这个子类是12个casting指令的父类。它提供了对cast指令的通用操作。

CmpInst:这个子类表示两个比较指令，ICmpInst(整数操作数)和FCmpInst(浮点操作数)。

**h. Instruction类的重要Public成员**
```c

BasicBlock *getParent()：返回嵌入该 Instruction 的BasicBlock，返回该指令所属的 基本块。
bool mayWriteToMemory()：如果指令（即call、free、invoke或store）写入内存，则返回true。
unsigned getOpcode()   ：返回该 Instruction 的操作码。
Instruction *clone() const
返回指定指令的另一个实例，该实例在所有方面与原始指令相同，只是该指令没有parent(即没有嵌入到BasicBlock中)，而且没有名称。
```
**i. Constant类和子类**

Constant表示不同类型常量的基类。它由 ConstantInt、ConstantArray 等构成子类，用于表示各种类型的Constants。GlobalValue也是一个子类，它表示全局变量或函数的地址。

**j. Constant类的重要子类**
```c
ConstantInt                  ：Constant的子类表示任意宽度的一个整数常量。
const APInt& getValue() const：返回这个常量的底层值，一个APInt值。

int64_t getSExtValue() const ：通过符号扩展将底层APInt值转换为int64_t。如果APInt的值(而不是位宽)太大，无法容纳int64_t，则会生成一个断言。由于这个原因，不鼓励使用这种方法。

uint64_t getZExtValue() const：通过zero扩展将底层APInt值转换为uint64_t。如果APInt的值(而不是位宽)太大，无法放入uint64_t中，则会生成一个断言。由于这个原因，不鼓励使用这种方法。

static ConstantInt* get(const APInt& Val)：返回代表Val提供的值的ConstantInt对象。该类型被暗示为与Val的位宽相对应的整数类型。

static ConstantInt* get(const Type *Ty, uint64_t Val)：返回代表Val为整数类型Ty提供的值的ConstantInt对象。
ConstantFP：这个类表示一个浮点常量。
double getValue() const：返回这个常量的基础值。
ConstantArray          ：这表示一个常量数组。
const std::vector<Use> &getValues() const：返回组成这个数组的一个组件常量向量。
ConstantStruct         ：这表示一个常量Struct。
const std::vector<Use> &getValues() const：返回组成这个struct的一个组件常量向量。
GlobalValue：它表示一个全局变量或函数。在这两种情况下，值都是一个常量固定地址(链接之后)。
```
**k. GlobalValue类 全局变量**

GlobalValue（GlobalVariables 或 Functions）是所有函数体中唯一可见的LLVM values。因为它们在全局范围内是可见的，所以它们还受制于与其他在不同翻译单元中定义的全局变量的链接。为了控制链接过程，GlobalValues知道它们的 linkage 规则。具体地说，GlobalValues知道它们是否具有internal或external linkage，这是由LinkageTypes枚举定义的。

如果一个GlobalValue有internal linkage(相当于C语言中的static链接)，那么它对于当前翻译单元之外的代码是不可见的，并且不参与链接。如果它有external linkage，那么它对外部代码是可见的，并且确实参与了链接。除了linkage信息，GlobalValues还跟踪它们当前属于哪个Module。

因为GlobalValues是内存对象，所以它们总是由它们的地址来引用。因此，一个全局的Type始终是指向其内容的一个指针。在使用GetElementPtrInst指令时，一定要记住这一点，因为必须首先取消对该指针的引用。例如，如果您有一个GlobalVariable (GlobalValue的子类)，它是一个24 int的数组，类型为[24xi32]，那么GlobalVariable是指向该数组的指针。虽然这个数组的第一个元素的地址和GlobalVariable的值是相同的，但是它们有不同的类型。全局变量的类型是[24xi32]。第一个元素的类型是i32。


### 遍历一个Function中的BasicBlock 遍历一个函数中的基本块
有一个你想要以某种方式转换的函数实例是很常见的；特别是，您希望操作它的基本块。为了实现这一点，您需要遍历构成该Function的所有BasicBlocks。下面是打印一个BasicBlock的名称和它包含的Instructions数的例子：
```c
Function &Func = ...
for (BasicBlock &BB : Func)
  // 如果有BasicBlock，则打印它的名称，然后打印它包含的Instructions数
  errs() << "Basic block (name=" << BB.getName() << ") has "
             << BB.size() << " instructions.\n";
```

### 遍历一个BasicBlock中的Instruction 遍历一个基本块中的指令
就像在函数中处理基本块一样，很容易遍历组成基本块的各个指令。这是一个代码片段，打印出在一个基本块的每个指令：
```c
BasicBlock& BB = ...
for (Instruction &I : BB)
   // 由于操作符<<(ostream&，…)为Instruction&重载，所以下一条指令可用
   errs() << I << "\n";
```
然而，这并不是打印BasicBlock内容的最佳方式！由于ostream操作符实际上重载了您所关心的所有内容，所以您可以调用基本块本身上的打印例程：errs() << BB << "\n";。

### 直接遍历一个Function中的Instruction
如果您发现您通常遍历函数的基本块，然后遍历基本块的指令，那么应该使用InstIterator。您需要include llvm/IR/InstIterator.h(doxygen)，然后在代码中显式实例化InstIterator。下面是一个小例子，展示了如何将函数中的所有指令转储到标准错误流：
```c
#include "llvm/IR/InstIterator.h"
// F是指向函数实例的指针 Function &F = ...
for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)  // 直接遍历函数中的所有指令
  errs() << *I << "\n";
```
很容易，不是吗？您还可以使用InstIterator来用工作列表的初始内容填充工作列表。例如，如果你想初始化一个工作列表来包含函数F中的所有指令，你需要做的就是：
```c
std::set<Instruction*> worklist; // 指令集合(顺序不变)
// 或者更好的是：SmallPtrSet<Instruction*, 64> worklist;
for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
  worklist.insert(&*I); // STL set工作列表现在将包含F指向的函数中的所有指令。
```

### 综合应用 编写函数pass 记录目标函数在函数中被调用的次数
记住，因为我们在编写FunctionPass，我们的FunctionPass派生类只需要重载 runOnFunction 方法
```c
Function* targetFunc = ...; // 目标函数指针
class OurFunctionPass : public FunctionPass { // 我们自己的函数pass 继承于 父类 FunctionPass
  public:
    OurFunctionPass(): callCounter(0) { } // 类构造函数 初始化 计数变量 callCounter 为 0
    virtual runOnFunction(Function& F) {  // 重载 runOnFunction 方法, 赋予新的灵魂 
      for (BasicBlock &B : F) {           // 遍历该函数的每一个基本块
        for (Instruction &I: B) {         // 遍历该基本块的每一个指令
          if (auto *CallInst = dyn_cast<CallInst>(&I)) { // 如果该指令是一个 调用指令 则强制转换 为 调用指令
            // 我们知道我们已经遇到了一个调用指令，所以我们需要确定它是否是m_func指向的函数的调用。
            if (CallInst->getCalledFunction() == targetFunc)
              // 该调用指令指向的函数 是 目标函数 则 将调用计数变量+1
              ++callCounter;
          }
        }
      }
    }
  private:
    unsigned callCounter;
};


```

### 遍历 def-use(哪些用户使用了我) 和 use-def（我使用了哪些东西（值）） 链

通常，我们可能有一个Value类的实例(doxygen)，我们希望确定哪些Users使用这个值。具有特定Value的所有Users的列表称为def-use链。例如，我们有一个 Function* F 指向一个特定的函数 foo。找到所有使用 foo 的指令就像遍历 F 的def-use链一样简单：
```c
Function *F = ...;
for (User *U : F->users()) { // 遍历具有特定值F的所有Users的列表 
  if (Instruction *Inst = dyn_cast<Instruction>(U)) { // 如果该使用者是 一条指令 则强制转换为指令
    errs() << "F is used in instruction:\n";
    errs() << *Inst << "\n";
  }
```

或者，通常有一个User类的实例(doxygen)，并且需要知道它使用什么Values。一个User使用的所有Values的列表称为use-def链。类Instruction的实例是常见的User，所以我们可能需要遍历特定Instruction使用的所有values(即特定Instruction的操作数)：
```c
Instruction *pi = ...;
for (Use &U : pi->operands()) { // 遍历该条指令 所使用的东西(值)
  Value *v = U.get();
  // ...
}
```

### 遍历块的前置和后继
使用“llvm/IR/CFG.h”中定义的例程，遍历块的前置和后继是非常容易的。只需使用这样的代码来遍历所有BB的前置：
```c
#include "llvm/IR/CFG.h"   // 控制流程图
BasicBlock *BB = ...;
for (BasicBlock *Pred : predecessors(BB)) { // predecessors(B) 基本块B的前继基本块
  // ...                                    // successors(B)   基本块B的后继基本块
}
````

## 改变IR
LLVM基础架构中有一些基本的转换操作值得了解。在执行转换时，操作基本块的内容是相当常见的。本节描述了一些常用的方法，并给出了示例代码。

###  创建和插入新 Instructions
实例化 Instructions
创建Instructions非常简单：只需调用该类指令的构造函数来实例化并提供必要的参数。例如，AllocaInst 只需要提供一个(const-ptr-to) Type。因此：

```C
auto *ai = new AllocaInst(Type::Int32Ty);

```
这将在运行时创建一个 AllocaInst 实例，该实例表示当前堆栈帧中一个整数的分配。每个 Instruction 子类都可能有不同的默认参数，这些参数会改变这个指令的语义，所以请参考 Instruction子类的doxygen文档，以获得您感兴趣的要实例化的子类的内容。


**命名值**

如果可以的话，命名指令的值是非常有用的，因为这有助于调试您的转换。如果您最终查看生成的LLVM机器码，那么您肯定希望有与指令结果关联的逻辑名称！通过为Instruction构造函数的Name(缺省)参数提供一个值，您可以将逻辑名称与运行时指令执行的结果关联起来。例如，假设我正在编写一个转换，它动态地为堆栈上的一个整数分配空间，这个整数将被其他代码用作某种索引。为此，我将AllocaInst放在某个 Function 的第一个 BasicBlock 的第一个 point 上，并打算在同一个 Function 中使用它。我可能会做：
```C
auto *pa = new AllocaInst(Type::Int32Ty, 0, "indexLoc");
```
其中indexLoc现在是指令执行值的逻辑名称，它是指向运行时堆栈上整数的指针。

**插入 Instructions**

从本质上讲，有三种方法可以将一条 Instruction 插入到构成一个 BasicBlock 的现有指令序列中：
插入到显式指令列表中
给定一个BasicBlock* pb，该 BasicBlock 中的一个Instruction* pi，以及我们希望在 *pi 之前插入的一条新创建的instruction，我们执行以下操作：
```
BasicBlock *pb = ...;
Instruction *pi = ...;
auto *newInst = new Instruction(...);
pb->getInstList().insert(pi, newInst); // Inserts newInst before pi in 
```

**删除 Instructions**
从构成一个 BasicBlock 的现有指令序列中删除一条指令非常简单：只需调用该指令的eraseFromParent()方法。例如：
```C
Instruction *I = .. ;
I->eraseFromParent();
```
这将从其包含的基本块中断开指令的链接并删除它。如果只是想从包含基本块的指令中断开链接，而不是删除它，可以使用removeFromParent()方法。

## 线程和LLVM

本节描述LLVM APIs与多线程的交互，包括客户端应用程序的交互和JIT中的交互，以及托管应用程序中的交互。
注意，LLVM对多线程的支持仍然相对较年轻。在2.5版之前，支持线程托管应用程序的执行，但不支持线程客户机访问APIs。虽然现在支持这个用例，但是客户端必须遵守下面指定的指导原则，以确保在多线程模式下正确操作。
注意，在类unix平台上，为了支持线程操作，LLVM需要GCC的原子内部特性。如果需要在没有合适的现代系统编译器的平台上使用支持多线程的LLVM，可以考虑在单线程模式下编译LLVM和LLVM-GCC，并使用生成的编译器构建支持多线程的LLVM副本。

7.1 使用llvm_shutdown()结束执行

使用LLVM api之后，应该调用llvm_shutdown()来释放用于内部结构的内存。

7.2 使用ManagedStatic延迟初始化

ManagedStatic是LLVM中的一个实用程序类，用于实现静态资源的静态初始化，比如全局类型表。在单线程环境中，它实现了一个简单的延迟初始化方案。然而，在编译支持多线程的LLVM时，它使用双重检查锁定来实现线程安全的延迟初始化。

7.3 使用LLVMContext实现隔离

LLVMContext是LLVM API中的一个不透明类，客户端可以使用它在同一个地址空间内并发地操作多个独立的LLVM实例。例如，在假设的编译服务器中，单个翻译单元的编译在概念上独立于所有其他单元，并且希望能够在独立的服务器线程上同时编译传入的翻译单元。幸运的是，LLVMContext只支持这种场景!

从概念上讲，LLVMContext提供了隔离。LLVM内存IR中的每个LLVM实体(模块、值、类型、常量等)都属于一个LLVMContext。不同上下文中的实体不能相互交互:不同上下文中的模块不能链接在一起，不同上下文中的函数不能添加到模块中，等等。这意味着在多个线程上同时编译是安全的，只要没有两个线程对同一上下文中的实体进行操作。

实际上，除了类型创建/查找API之外，API中很少有地方需要LLVMContext的显式规范。因为每种类型都带有对其所属上下文的引用，所以大多数其他实体可以通过查看自己的类型来确定它们属于哪个上下文。如果您正在向LLVM IR添加新实体，请尝试维护此接口设计。

7.4 线程和JIT

LLVM的“eager”JIT编译器在线程程序中使用是安全的。多个线程可以并发地调用ExecutionEngine::getPointerToFunction()或ExecutionEngine::runFunction()，多个线程可以并发地运行JIT输出的代码。用户仍然必须确保只有一个线程访问给定LLVMContext中的IR，而另一个线程可能正在修改它。一种方法是在访问JIT外部的IR时始终保持JIT锁(JIT通过添加CallbackVHs来修改IR)。另一种方法是只从LLVMContext的线程调用getPointerToFunction()。
当JIT被配置为延迟编译(使用ExecutionEngine:: disablelazycompile (false))时，当前在延迟jated函数之后更新调用站点时存在竞争条件。如果您确保每次只有一个线程可以调用任何特定的延迟存根，并且JIT锁保护任何IR访问，那么仍然可以在线程程序中使用延迟JIT，但是我们建议只在线程程序中使用即时JIT。


## 实战

 LLVM Pass是什么呢？ Pass就是“遍历一遍IR，可以同时对它做一些操作”的意思。翻译成中文应该叫“趟”。 在实现上，LLVM的核心库中会给你一些 Pass类 去继承。你需要实现它的一些方法。 最后使用LLVM的编译器会把它翻译得到的IR传入Pass里，给你遍历和修改。

那LLVM Pass有什么用呢？

显然它的一个用处就是插桩，毕竟这是我本来想利用它做的事情： 在Pass遍历LLVM IR的同时，自然就可以往里面插入新的代码。 

机器无关的代码优化：大家如果还记得编译原理的知识的话，应该知道IR在被翻译成机器码前会做一些机器无关的优化。 但是不同的优化方法之间需要解耦，所以自然要各自遍历一遍IR，实现成了一个个LLVM Pass。 最终，基于LLVM的编译器会在前端生成LLVM IR后调用一些LLVM Pass做机器无关优化， 然后再调用LLVM后端生成目标平台代码。

静态分析： 像VSCode的C/C++插件就会用LLVM Pass来分析代码，提示可能的错误 (无用的变量、无法到达的代码等等)。
…… (自行发挥想象)


### 1. 编写pass 记录函数中每种操作op的数量 

```c++

#define DEBUG_TYPE "opCounter"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
using namespace llvm;
namespace {
struct CountOp : public FunctionPass {
std::map<std::string, int> opCounter;
static char ID;
CountOp() : FunctionPass(ID) {}
virtual bool runOnFunction(Function &F) {
errs() << "Function " << F.getName() << '\n';
// for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F);        // 模块迭代器 得到函数列表
for (Function::iterator bb = F.begin(), e = F.end(); bb != e; ++bb) {     // 函数迭代器 得到 基本快BB 列表
for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {  // 基本快迭代器 得到指令列表
// for (User::op_iterator O = I.op_begin(), E = I.op_end(); O != E; ++O); // 指令是使用者User 迭代它 可以得到 操作op列表
if(opCounter.find(i->getOpcodeName()) == opCounter.end()) {
opCounter[i->getOpcodeName()] = 1; // 第一次遇到这个op，赋值为1
} else {
opCounter[i->getOpcodeName()] += 1; // 之后没遇到一次，+1
}
}
}

// 打印
std::map <std::string, int>::iterator i = opCounter.begin();
std::map <std::string, int>::iterator e = opCounter.end();
while (i != e) {
errs() << i->first << ": " << i->second << "\n";
i++;
}
errs() << "\n";
opCounter.clear();
return false;
}
};
}
char CountOp::ID = 0;
static RegisterPass<CountOp> X("opCounter", "Counts opcodes per functions");

```

### 2. 记录循环内的bb数量

```c
namespace {
struct BBinLoops : public Func@onPass {
staAc char ID;
BBinLoops() : FuncAonPass(ID) {}

// An LLVM pass must declare which other passes it requires to execute properly.
void getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<LoopInfo>();
    AU.setPreservesAll ();
}

void countBlocksInLoop(Loop *L, unsigned nesAng) {
unsigned numBlocks = 0;
Loop::block_iterator bb;
for(bb = L‐>block_begin(); bb != L‐>block_end();++bb)
numBlocks++;
errs() << "Loop level " << nesAng << " has " << numBlocks << " blocks\n";
vector<Loop*> subLoops = L‐>getSubLoops();
Loop::iterator j, f;
for (j = subLoops.begin(), f = subLoops.end(); j != f; ++j)
    countBlocksInLoop(*j, nesAng + 1);
}

virtual bool runOnFuncAon(FuncAon &F) {
LoopInfo &LI = getAnalysis<LoopInfo>();
errs() << "FuncAon " << F.getName() + "\n";
for (LoopInfo::iterator i = LI.begin(), e = LI.end(); i != e; ++i)
    countBlocksInLoop(*i, 0);
return(false);
}
};
}
char BBinLoops::ID = 0;
staAc RegisterPass<BBinLoops> X("bbloop",
"Count the number of BBs inside each loop");

```
### 3.各种分支类型计数
```c
bool BranchCounter::runOnFunction(Function &F) {
for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    if (BranchInst* BI = dyn_cast<BranchInst>(&*I)) {
        // Count this branch in the total.
        TotalBranches++;
        // Count unconditional branches.
        if (!BI->isConditional())
          UnconditionalBranches++;
        // Count the other types os branches
        else if (ICmpInst* CI = dyn_cast<ICmpInst>(BI->getCondition())) {
          bool const_op0 = dyn_cast<ConstantInt>(CI->getOperand(0)) != 0;
          bool const_op1 = dyn_cast<ConstantInt>(CI->getOperand(1)) != 0;
          // Both operands are constants.
          if (const_op0 && const_op1)
            ConstantAndConstantBranches++;
          // Both operands are variables.
          else if (!const_op0 && !const_op1)
            VarAndVarBranches++;
          // A variable and a constant operands.
          else
            ConstantAndVarBranches++;
        // Count other types of branches.
        } 
        else
          OtherBranches++;
        }
    }
    return false;
}

```

### 4. 控制流程图中的 分支会和节点 统计
```c
struct Count_Phis : public FunctionPass
{
  static char ID;
  Count_Phis(): FunctionPass(ID) {}
  virtual bool runOnFunction(Function &F) {
  errs() << "FuncFon " << F.getName() << '\n';
  for(inst_iterator I=inst_begin(F), E=inst_end(F); I !=E; ++I)
  // 直接使用 inst_iterator迭代 函数中的 指令
  // 也可以使用 Function::iterator 迭代函数中的BB,再使用BasicBlock::iterator迭代BB中的指令
  {
    if(isa<PHINode>(*I))
    // if(PHINode *PN = dyn_cast<PHINode>(&*I)) // 动态判断类型
    {
       errs() << *I << "\n";
       errs() <<" has" <<  cast<PHINode>(*I).getNumIncomingValues() <<" arguments.\n";
       for(int arg=0; arg<numArgs; arg++)
       {
          errs() << " Argument"<< arg << ": \n";
          errs() << PN‐>getIncomingBlock(arg)‐>getName()<<": "<< *(PN‐>getIncomingValue(arg))<< "\n";
       }
    }
      
  }
  return false;
}

```

### 5. 优化分支会和节点的 常量分支
```c
virtual bool runOnFunction(Function &F)
{
bool cutInstrucFon = false;
errs() << "FuncFon " << F.getName() << '\n';
SmallVector<PHINode*,16> Worklist;
for(Function::iterator B = F.begin(), EB = F.end(); B != EB; ++B)
{
  for (BasicBlock::iterator I = B‐>begin(), EI = B‐>end(); I != EI; ++I)
  {
    if (PHINode *PN = dyn_cast<PHINode>(I))
    {
      if (PN‐>hasConstantValue()) // 常量
      {
        Worklist.push_back(PN); // 记录优化点
        cutInstrucFon = true;
      }
    }
    else
    {
      break;
    }
  }
}

// 优化处理
while (!Worklist.empty())
{
  PHINode* PN = Worklist.pop_back_val();
  PN‐>replaceAllUsesWith(PN‐>getIncomingValue(0));
  PN‐>eraseFromParent();
}
return cutInstrucFon;
}

```

### 6. 处理函数参数的pass
```c
namespace {
    struct Add_No_Alias : public ModulePass {
        staFc char ID;
        Add_No_Alias() : ModulePass(ID) {}
        virtual bool runOnModule(Module &M) {
            for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
                if (!F‐>isDeclaration()) {
                    FuncFon::arg_iterator Arg = F‐>arg_begin(), ArgEnd = F‐>arg_end(); // 遍历函数参数
                    // for (inst_iterator I = inst_begin(&*F), E = inst_end(&*F); I != E; ++I) // 遍历指令
                    while (Arg != ArgEnd) {
                        if (Arg‐>getType()‐>isPointerTy()) {
                            // 处理指针参数
                            AjrBuilder noalias(Ajribute::get(Arg‐>getContext(), Ajribute::NoAlias));
                            int argNo = Arg‐>getArgNo() + 1;
                            Arg‐>addAjr(AjributeSet::get(Arg‐>getContext(), argNo, noalias));
                        }
                        ++Arg;
                    }
                }
            }
            return true;
        }
    };
}

// 注册pass
char Add_No_Alias::ID = 0;
staFc RegisterPass<Add_No_Alias> X
("addnoalias", "Add no alias to funcFon ajributes");
```

### 利用深度遍历来剔除死代码 的pass   Dead Blocks Elimination
[参考](https://monetaphilis.github.io/2019/04/21/DeadBlock/)

```c
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DepthFirstIterator.h"  // LLVM的数据结构  深度优先遍历
#include "llvm/IR/CFG.h" // 控制流程图
#include <set>           // STL  SET集合

using namespace llvm;

namespace {
struct DeadBlock : public FunctionPass   // 死代码块去除pass 继承 FunctionPass
                                         // 这个pass将会在每个函数上运行一次
{
    static char ID;

    DeadBlock() : FunctionPass(ID) {}
    
    virtual bool runOnFunction(llvm::Function& F) override {
        // pass的入口
        bool changed = false;  // 是否改变了函数
         // 用来指示这个pass是否改变了目标function，如果改变了，则要标记为true。这个值将是我们的返回值

        // visitedSet 用于存放已经被访问过的BaseBlock
        // 从函数的root block开始，遍历这个root block可能会达到的block，被遍历到的block将会存放到这个 visitedSet 中
        // unreachableSet 则在最后用于存放无法被访问到的block
        // 在得到visitedSet之后，我们可以将其和这个函数中所有block做比较，如果有不在visitedSet中的block，就将其添加到unreachableSet
        
        std::set<BasicBlock*> visitedSet;
        std::set<BasicBlock*> unreachableSet;

        // 从EntryBlock开始深度优先遍历整个函数内可以访问的BaseBlock
        // 将已被访问过的BaseBlock存放在visitedSet中
        for (auto i = df_ext_begin<BasicBlock*,std::set<BasicBlock*>>(&F.getEntryBlock(), visitedSet),
            e = df_ext_end<BasicBlock*,std::set<BasicBlock*>>(&F.getEntryBlock(), visitedSet);
            i != e; i++);
        // 我们无需自己手动实现深度优先遍历，
        // 只需调用DepthFirstIterator.h里的df_ext_begin和df_ext_end两个模板，
        // 就能轻松遍历整个函数，并将访问过的block添加到visitedSet之中。
        
        // 遍历函数内所有BaseBlock，将不在 vistitedSet 中的BaseBlock添加到unreachableSet中
        for (BasicBlock & BB : F) {
            if (visitedSet.find(&BB) == visitedSet.end()) { // 在 vistitedSet中未找到
                unreachableSet.insert(&BB);
            }
        }

        // 标记目标函数是否会被修改
        if (!unreachableSet.empty()) {
            changed = true;
        }

        // 遍历unreachableSet，通知其successor移除多余的phi node
        for (BasicBlock* BB : unreachableSet) {
            for (auto i = succ_begin(BB); i != succ_end(BB); i++) {
                i->removePredecessor(BB); 
                // removePredecessor()函数会通知该block有predecessor已被移除，
                // 随后这个block会检查自己是否有会受到影响的phi node并自动做出修改。
            }
            BB->eraseFromParent();  // 删除掉不想要的block
        }
    
        return changed;
    };
};
}

// LLVM会利用pass的地址来为这个id赋值，所以初始值并不重要
char DeadBlock::ID = 0;
// 注册pass，这个pass可能会改变CFG，所以将第三个参数设为true
static RegisterPass<DeadBlock> X("deadblock", "Dead blocks elimination pass", true, false);



```

示例: IR
```C
; ModuleID = 'test.bc'

define i32 @main() {
b1:
  br label %b2

dead:                                             ; No predecessors!
  br label %b3

b2:                                               ; preds = %b1
  br label %b3

b3:                                               ; preds = %dead, %b2
  %b = phi i32 [ 1, %b2 ], [ 2, %dead ]
  ret i32 %b
}
```
名为dead的这个base block实际是无法被访问到的！b1中的无条件br永远只会跳转到b2，而不是dead。我们现在要实现的DeadBlock pass正是要消去这样的base block。

不过，并不是直接删掉dead就完事了，在b3之中，我们有一个phi node。为了让原来的代码保持正确，我们还需要调整或者删除受影响的phi node。


名为dead的这个base block实际是无法被访问到的！b1中的无条件br永远只会跳转到b2，而不是dead。我们现在要实现的DeadBlock pass正是要消去这样的base block。

不过，并不是直接删掉dead就完事了，在b3之中，我们有一个phi node。为了让原来的代码保持正确，我们还需要调整或者删除受影响的phi node。

>  执行 

opt -load ../../../Debug+Asserts/lib/DeadBlock.so -deadblock < test.bc > optimized.bc

优化后的IR
```C
; ModuleID = 'optimized.bc'

define i32 @main() {
b1:
  br label %b2

b2:                                               ; preds = %b1
  br label %b3

b3:                                               ; preds = %b2
  ret i32 1
}

```
dead已经被移除，多余的phi node也被去除，替换成了常量。


# llvm内置的众多 Passes
Passes主要分为三类

llvm内置的pass特别多，基本上分为三类，Analysis Passes、Transform Passes、Utility Passes。

Analysis主要用于分析优化目标获取信息，而Transformation则是实际去做优化变换。

1. 分析 Analysis Passes类别的Pass主要用于计算分析和输出一些IR信息，用于调试用途、可视化用途等等。

2. 转化 Transform Passes类别的Pass就是我们的重点，可以彻底将现有的IR“更改成”另外一种形式的IR，具体如何更改，全看代码怎么写。可以在Transform Passes里调用Analysis Passes。

3. Utility Passes则完全就是因为有些Pass既不是Analysis类型，也不是Transform类型，比如某些Pass的作用就是将函数释放成字节码，或者将某个模块生成字节码本质上既不是分析也不是转化，所以就归类成Utility Passes。


       Analysis Passes
          -aa-eval: Exhaustive Alias Analysis Precision Evaluator
          -basicaa: Basic Alias Analysis (stateless AA impl)
          -basiccg: Basic CallGraph Construction
          -count-aa: Count Alias Analysis Query Responses
          -da: Dependence Analysis
          -debug-aa: AA use debugger
          -domfrontier: Dominance Frontier Construction
          -domtree: Dominator Tree Construction
          -dot-callgraph: Print Call Graph to “dot” file
          -dot-cfg: Print CFG of function to “dot” file
          -dot-cfg-only: Print CFG of function to “dot” file (with no function bodies)
          -dot-dom: Print dominance tree of function to “dot” file
          -dot-dom-only: Print dominance tree of function to “dot” file (with no function bodies)
          -dot-postdom: Print postdominance tree of function to “dot” file
          -dot-postdom-only: Print postdominance tree of function to “dot” file (with no function bodies)
          -globalsmodref-aa: Simple mod/ref analysis for globals
          -instcount: Counts the various types of Instructions
          -intervals: Interval Partition Construction
          -iv-users: Induction Variable Users
          -lazy-value-info: Lazy Value Information Analysis
          -libcall-aa: LibCall Alias Analysis
          -lint: Statically lint-checks LLVM IR
          -loops: Natural Loop Information
          -memdep: Memory Dependence Analysis
          -module-debuginfo: Decodes module-level debug info
          -postdomfrontier: Post-Dominance Frontier Construction
          -postdomtree: Post-Dominator Tree Construction
          -print-alias-sets: Alias Set Printer
          -print-callgraph: Print a call graph
          -print-callgraph-sccs: Print SCCs of the Call Graph
          -print-cfg-sccs: Print SCCs of each function CFG
          -print-dom-info: Dominator Info Printer
          -print-externalfnconstants: Print external fn callsites passed constants
          -print-function: Print function to stderr
          -print-module: Print module to stderr
          -print-used-types: Find Used Types
          -regions: Detect single entry single exit regions
          -scalar-evolution: Scalar Evolution Analysis
          -scev-aa: ScalarEvolution-based Alias Analysis
          -stack-safety: Stack Safety Analysis
          -targetdata: Target Data Layout
       Transform Passes
          -adce: Aggressive Dead Code Elimination
          -always-inline: Inliner for always_inline functions
          -argpromotion: Promote ‘by reference’ arguments to scalars
          -bb-vectorize: Basic-Block Vectorization
          -block-placement: Profile Guided Basic Block Placement
          -break-crit-edges: Break critical edges in CFG
          -codegenprepare: Optimize for code generation
          -constmerge: Merge Duplicate Global Constants
          -constprop: Simple constant propagation
          -dce: Dead Code Elimination
          -deadargelim: Dead Argument Elimination
          -deadtypeelim: Dead Type Elimination
          -die: Dead Instruction Elimination
          -dse: Dead Store Elimination
          -functionattrs: Deduce function attributes
          -globaldce: Dead Global Elimination
          -globalopt: Global Variable Optimizer
          -gvn: Global Value Numbering
          -indvars: Canonicalize Induction Variables
          -inline: Function Integration/Inlining
          -instcombine: Combine redundant instructions
          -aggressive-instcombine: Combine expression patterns
          -internalize: Internalize Global Symbols
          -ipconstprop: Interprocedural constant propagation
          -ipsccp: Interprocedural Sparse Conditional Constant Propagation
          -jump-threading: Jump Threading
          -lcssa: Loop-Closed SSA Form Pass
          -licm: Loop Invariant Code Motion
          -loop-deletion: Delete dead loops
          -loop-extract: Extract loops into new functions
          -loop-extract-single: Extract at most one loop into a new function
          -loop-reduce: Loop Strength Reduction
          -loop-rotate: Rotate Loops
          -loop-simplify: Canonicalize natural loops
          -loop-unroll: Unroll loops
          -loop-unroll-and-jam: Unroll and Jam loops
          -loop-unswitch: Unswitch loops
          -loweratomic: Lower atomic intrinsics to non-atomic form
          -lowerinvoke: Lower invokes to calls, for unwindless code generators
          -lowerswitch: Lower SwitchInsts to branches
          -mem2reg: Promote Memory to Register
          -memcpyopt: MemCpy Optimization
          -mergefunc: Merge Functions
          -mergereturn: Unify function exit nodes
          -partial-inliner: Partial Inliner
          -prune-eh: Remove unused exception handling info
          -reassociate: Reassociate expressions
          -reg2mem: Demote all values to stack slots
          -sroa: Scalar Replacement of Aggregates
          -sccp: Sparse Conditional Constant Propagation
          -simplifycfg: Simplify the CFG
          -sink: Code sinking
          -strip: Strip all symbols from a module
          -strip-dead-debug-info: Strip debug info for unused symbols
          -strip-dead-prototypes: Strip Unused Function Prototypes
          -strip-debug-declare: Strip all llvm.dbg.declare intrinsics
          -strip-nondebug: Strip all symbols, except dbg symbols, from a module
          -tailcallelim: Tail Call Elimination
       Utility Passes
          -deadarghaX0r: Dead Argument Hacking (BUGPOINT USE ONLY; DO NOT USE)
          -extract-blocks: Extract Basic Blocks From Module (for bugpoint use)
          -instnamer: Assign names to anonymous instructions
          -verify: Module Verifier
          -view-cfg: View CFG of function
          -view-cfg-only: View CFG of function (with no function bodies)
          -view-dom: View dominance tree of function
          -view-dom-only: View dominance tree of function (with no function bodies)
          -view-postdom: View postdominance tree of function
          -view-postdom-only: View postdominance tree of function (with no function bodies)
          -transform-warning: Report missed forced transformations


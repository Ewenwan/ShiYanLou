# Getting Started with LLVM Core Libraries
[参考1](https://www.cnblogs.com/pocean/p/4021129.html)

## 工具和设计

追溯llvm的历史要追溯到10年前（2014），起初它着眼于编译器的后端算法和中间表述（今天依然是llvm的重点），当时其他部分还要依赖gcc。其中最重要的是它的IR，llvm在整个编译的过程中有很多种中间表示，如AST(C/C++转换成IR的过程)、DAG(IR转换成特定机器的汇编语言的过程)、某种特定的中间表示(汇编器和链接器使用)等其他中间表示依然起到很重要的作用，但是，真正让llvm与众不同的还是它的IR，甚至llvm这个名字的由来还和IR有很大的关系。

> 首先看IR(intermediate representation)的特点：

    SSA  signal static assignment 静态单赋值   三地址码
    无限多个寄存器
    
  其存在方式有两种，一种在内存中，一种是在磁盘上，SSA能够更方便的优化，再加上保存在磁盘上的这种编码方式能够达到几近全时优化的效果，这也是llvm的作者在最早介绍llvm的论文中提出的目标，但是IR的描述能力毕竟有限，要想实现全时优化，就要求对诸如程序分布这种行为做出描述，这也意味着实现了此功能的llvm要像jvm那样能够成为一个运行平台，类似jvm的名字，llvm这个名字就是这么来的。

  随着llvm项目的发展，其设计目标已经重点放在了通过磁盘上的IR操作实现链接时优化等功能，而不是全时优化，最终只保留了llvm这个名字，但是llvm已经成了一个强大的编译器框架，而不是一个类似jvm的那样的平台。

  说到这，说一下个人理解吧，感觉是llvm起初提出了一个比较精妙的编码表示方案，有利于一些编译优化，进而发现可以憧憬的的目标——全时优化，但是又发现实现全时优化比较困难，在llvm的mail list上有人专门就此发起了一个很详细的讨论，详细阐述了llvm不能像java那样成为一个platform或者vm的原因，重点还是要做编译器。
  
不管怎样，llvm这么名字还是保留下来了，现在如果有人跟你提起“LLVM”，可能是指以下内容：

    整个LLVM Project/框架
    由LLVM框架构造的编译器
    LLVM框架的可重用代码：库
    LLVM IR

  LLVM框架主要包括三个方面的核心内容：

    前端(frontend):将源码转换成中间代码(IR),包括词法分析、语法分析、语义分析、中间代码生成

    IR：中间表示 两种方式，readable和binary-encoded

    后端(backend):将IR转换成汇编代码，包括寄存器分配

  每个步骤中间交互有两种介质：1) in-memory; 2) by-file，前者变现出来通过一些高层的工具如clang，自动进行指定的动作；后者可以每一步每一步分离开来，通过一个个独立的工具完成。
  
  
## External Projects 附加工具

    Clang extra tools 前段附加工具
    Compiler-RT       编译器运行时
    DragonEgg         利用GCC(不仅支持类c语言解析) 得到 IR
    LLVM test suite   测试
    LLDB              类 gdb 调试器
    libc++            底层运行时库 系统动态链接库
  
## Frontend  前段  Clang
    1. 使用 libclang
    2. 理解Clang diagnostics
    3. the frontend phases：
       词法分析 Lexical analysis
       语法分析 Syntactic analysis（AST） 抽象语法树
               TranslationUnitDecl 翻译单元
               TypedefDecl   类型定义
               FunctionDecl  函数定义
               CFG
       语义分析 Semantic analysis


## LLVM IR  中间表达
    1. 语法syntax
        Module
        Function
        BasicBlock
        Instruction
        use-def与def-use链
            Value
            User
    2. 生成器 generator
    3. IR级优化：pass
        -Ox
        -print-stats
        理解pass间依赖：
            e.g. Loop Info and Dominator Tree
        理解pass API
        定制pass


## Backend 后端
    1. TableGen
    2. SelectionDAG
    3. Scheduler           调度器
    4. MachineInstr
    5. Register allocation 寄存器分配
    6. Prologue and epilogue
    7. MCInst



## 运行时 JIT


## 交叉编译   仿真平台 QEMU



## Clang静态分析
    1. （后端指令生成的）性能优化 & （基于前端AST的）静态分析，编译器技术的两大法宝！
    2. 竞争对手：HP Fortify and Synopsis Coverity
    3. exponential-time complexity，不支持inter-module analysis
    4. forward data flow analysis  数据流分析
        给变量符号关联一些属性，然后在后面用到的地方检查约束是否满足
        False positives：往往导致程序员忽略所有的警告信息







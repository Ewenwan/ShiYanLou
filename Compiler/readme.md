# 编译器 编译原理

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




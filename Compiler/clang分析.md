# clang 编译器前端 分析

[clang static analyzer源码分析](https://blog.csdn.net/dashuniuniu/article/details/50773316)

AnalysisASTConsumer 继承自ASTConsumer，是一个虚基类，只提供了一个添加报错机制的纯虚函数。

clang静态分析checker提供了两种方法，一种是遍历AST进行语法层级的报错，例如位运算符的操作数是有符号整数等，这些直接从AST树上拿到相关信息就可以直接报错，另外一种是需要构建CFG并在其上进行符号执行实现路径敏感的代码分析，关于这个，就有人曾经问过我，clang静态分析中CFG和AST的关系是什么。现在我们从这个类的继承体系就可以看出，clang静态代码分析是继承自于ASTConsumer，也就是说无论是基于AST的检查还是基于CFG的检查都是在AST上实现的，因为构建CFG也需要AST的协助。

## 前端clang分析

Clang是LLVM的C/C++前端，从原理上他会产生用于后端的IR指令。但实际上Clang会有两种执行方式： 我们可以使用”-###”观察Clang的执行过程

    1. 以Driver的方式执行： 
       会自动调用相关后端程序，并生成可执行文件，这也是之所以Clang虽然只是前端，却可以直接产生目标代码的原因.
       在驱动模式下，clang实质只是一个调度管理程序.
       
       
    2. 作为编译器-cc1前端方式运行：
       最后仅生成LLVM IR 

Clang执行初期是作为driver执行的，因此，程序的入口是：tools/driver/driver.cpp；

[代码位置](https://github.com/Ewenwan/llvm-project/blob/63ab2f559ba1a4619fdba079ea85e3bea0aaa51a/clang/tools/driver/driver.cpp#L337)

如果不是-cc1，则进行相关命令解释，生成相容的命令行

通过Driver建立与GCC相容的编译过程，并由TheDriver.ExecuteCompilation执行该相容的

注意因为clang两种工作模式下，驱动模式实际是在补足参数后再通过-cc1的方式执行;

在Driver方式下，只是为clang补齐相关执行的各参数，如类库的名字，然后是通过“系统”执行clang -cc1命令，而并没有在“内部”继续clang的其余的操作；此时，clang会等待相关的执行操作完成后执行下一个命令（如ld）

驱动方式过程：

    1。Parse：Option Parsing 编译参数解析

       在这个阶段完成后，命令行将被分解为具有适当参数定义好的选项。

    2. Pipeline：编译动作构造（Compilation Action Construction） 编译流水线构造

   子进程作业树需要确认构造编译序列。这包含确认输入文件及其类型、对他们进行什么样的工作（预处理、编译、汇编、链接等）并为每个人物构造动作实例链表。这样的结构是一个或更多的顶层动作列表，每个通常对应一个单一的输出（例如，对象或链接的可执行文件）多数的动作（Actions）对应一个实际的任务，但是这里有两个特殊的任务（Actions），第一个是InputActions，他只是简单将输入参数匹配到另一个Actions的输入。第二个是BindArchAction，从概念上给所有使用输入的动作替换架构Clang驱动可以使用命令“-ccc-print-phases”转印这一阶段的结果。

    2.5 Action
    
每次的 option 都会完整的走一遍从预处理，静态分析，backend 再到汇编的过程。

下面列下一些编译器的前端 Action，大家可以一个个用着玩。

    InitOnlyAction - 只做前端初始化，编译器 option 是    -init-only
    PreprocessOnlyAction - 只做预处理，不输出，编译器的 option 是 -Eonly
    PrintPreprocessedAction - 做预处理，子选项还包括-P、-C、-dM、-dD 具体可以查看PreprocessorOutputOptions 这个类，编译器 option 是 -E
    RewriteIncludesAction - 预处理
    DumpTokensAction - 打印token，option 是 -dump-tokens
    DumpRawTokensAction - 输出原始tokens，包括空格符，option 是 -dump-raw-tokens
    RewriteMacrosAction - 处理并扩展宏定义，对应的 option 是 -rewrite-macros
    HTMLPrintAction - 生成高亮的代码网页，对应的 option 是 - emit-html
    DeclContextPrintAction - 打印声明，option 对应的是 -print-decl-contexts
    ASTDeclListAction - 打印 AST 节点，option 是 -ast-list
    ASTDumpAction - 打印 AST 详细信息，对应 option 是 -ast-dump
    ASTViewAction - 生成 AST dot 文件，能够通过 Graphviz 来查看图形语法树。 option 是 -ast-view
    AnalysisAction - 运行静态分析引擎，option 是 -analyze
    EmitLLVMAction - 生成可读的 IR 中间语言文件，对应的 option 是      -emit-llvm
    EmitBCAction - 生成 IR Bitcode 文件，option 是                   -emit-llvm-bc
    MigrateSourceAction - 代码迁移，option 是 -migrate


    3. Bind：Tool & Filename Selection 工具、文件绑定
    
Bind 主要是与工具链 ToolChain 交互
根据创建的那些 Action，在 Action 执行时 Bind 来提供使用哪些工具，比如生成汇编时是使用内嵌的还是 GNU 的，还是其它的呢，这个就是由 Bind 来决定的，具体使用的工具有各个架构，平台，系统的 ToolChain 来决定

驱动与工具链互动去执行工具绑定（The driver interacts with a ToolChain to perform the Tool bindings）。每个工具链包含特定架构、平台和操作系统等编译需要的所有信息；一个单一的工具在编译期间需要提取很多工具链，为了与不同体系架构的工具进行交互。

这一阶段不会直接计算，但是驱动器可以使用”-ccc-print-bindings”参数打印这一结果，会显示绑定到编译序列的工具链，工具，输入和输出。


    4. Translate：Tool Specific Argument Translation 工具参数翻译

当工具被选择来执行特定的动作，工具必须为之后运行的编译过程构造具体的命令。主要的工作是翻译gcc格式的命令到子进程所期望的格式。

    5.Execute  工具执行
    
其执行过程大致如下：Driver::ExecuteCompilation -> Compilation::ExecuteJobs -> Compilation::ExecuteCommand-> Command::Execute -> llvm::sys::ExecuteAndWait；此时执行的ExecuteAndWait为Support/Program.cpp中的程序，其调用相关操作系统，执行其系统相关的执行程序，并等待执行过程完成。

### clang cc1前端运行  真正有意义的前端操作
```c
    auto FirstArg = std::find_if(argv.begin() + 1, argv.end(),
                               [](const char *A) { return A != nullptr; });
  if (FirstArg != argv.end() && StringRef(*FirstArg).startswith("-cc1")) {
    // If -cc1 came from a response file, remove the EOL sentinels.
    if (MarkEOLs) {
      auto newEnd = std::remove(argv.begin(), argv.end(), nullptr);
      argv.resize(newEnd - argv.begin());
    }
    return ExecuteCC1Tool(argv);
  }  
 ```
如果是 -cc1 的话会调用 ExecuteCC1Tool 这个函数，先看看这个函数

```c
static int ExecuteCC1Tool(ArrayRef<const char *> argv) {
  llvm::cl::ResetAllOptionOccurrences();
  StringRef Tool = argv[1];
  void *GetExecutablePathVP = (void *)(intptr_t) GetExecutablePath;
  if (Tool == "-cc1")
    return cc1_main(argv.slice(2), argv[0], GetExecutablePathVP);
  if (Tool == "-cc1as")
    return cc1as_main(argv.slice(2), argv[0], GetExecutablePathVP);
  if (Tool == "-cc1gen-reproducer")
    return cc1gen_reproducer_main(argv.slice(2), argv[0], GetExecutablePathVP);
  // Reject unknown tools.
  return 1;
}
```
最终的执行会执行 cc1-main 、cc1as_main 、cc1gen_reproducer_main。这三个函数分别在 driver.cpp 同级目录里的 cc1_main.cpp 、cc1as_main.cpp 、cc1gen_reproducer_main.cpp中。

依照关于Driver的示意图，clang将藉由Action完成具体的操作，在clang中所有action定义在include/clang/Drivers名字域：clang::driver下，其Action即其派生的Actions定义如下：

这一阶段完成，编译过程被分为一组需要执行并产生中间或最终输出（某些情况下，类似-fsyntax-only，不会有“真实”的最终输出）的Action。阶段是我们熟知的编译步骤，类似：预处理、编译、汇编、链接等等。

所有相关Action的定义在FrontendOptions.h(clang/include/clang/Frontend/FrontendOptions.h )中；

[代码](https://github.com/Ewenwan/llvm-project/blob/f9665a93fa96c464d7b6e911e111fbbbc0fbfa64/clang/include/clang/Frontend/FrontendOptions.h#L34)

在clang中允许通过FrontendAction编写自己的Action，使用FrontendPluginRegistry（clang/frontend/FrontendRegistry.h）注册自己的Action:其核心是通过继承clang::FrontendAction来实现，详细示例参考：clang/examples/AnnotateFunctions/AnnotateFunctions.cpp，该示例通过继承PluginASTAction，并使用FrontendPluginRegistry::Add将其注册

最初的C/C++源码经过：词法分析（Lexical analysis）、语法分析（Syntactic analysis）、语义分析（Semantic analysis）最后输出与平台无关的IR（LLVM IR generator）

###  词法分析（Lexical analysis） libclangLex 

编译器第一个步骤是词法分析（Lexical analysis）。词法分析器读入组成源程序的字节流，并将他们组成有意义的词素（Lexeme）序列。对于每个词素，词法分析器产生**词单元（token）**作为输出，并生成相关符号表。词法库包含了几个紧密相连的类，他们涉及到词法和C源码预处理。

源码      clang/lib/Lex/
相关诊断  DiagnosticLexKinds.td



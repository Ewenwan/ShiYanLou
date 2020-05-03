# clang 编译器前端 分析

[clang static analyzer源码分析](https://blog.csdn.net/dashuniuniu/article/details/50773316)

clang静态代码分析是clang相对于gcc一个比较能够引起关注的点，特别是clang静态代码分析基于checker的架构和大部分的静态分析工具都不相同。clang静态代码分析使用符号执行的技术执行路径敏感的代码分析，符号执行引擎并不实际进行报错，而是使用挂载在引擎上的checker对程序状态进行检查并报错。这种方式方便用户对代码检查规则或者bug类型进行扩展，但是这种架构也有其缺陷，符号执行完成一条语句后，clang静态分析引擎会遍历checker列表中的回调函数进行报错，也就是说checker的数量越多，clang静态分析扫描代码的速度越慢（clang静态分析引擎的速度是不变的）。

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

## 细节分析

[clang 在线文档 ](http://clang.llvm.org/docs/)

最初的C/C++源码经过：词法分析（Lexical analysis）-> 语法分析（Syntactic analysis）-> 语义分析（Semantic analysis）-> 与平台无关的IR（LLVM IR generator）

从词法分析开始——将C语言 源码分解成token流，每个token可表示标识符、字面量、运算符等；
token流会传递给语法分析器，语法分析器会在语言的CFG（Context Free Grammar，上下文无关文法）的指导下将token流组织成AST（抽 象语法树）；接下来会进行语义分析，检查语义正确性，然后生成IR。

libclang  clang-c/Index.h   c接口调用clang [示例](https://github.com/Ewenwan/screader/blob/master/scread.c)

Parser部分涉及到的目录有 tools/clang/lib/AST语法树定义，Sema语义分析，Lex词法分析器，Parse语法分析器。

## 1. 词法分析（Lexical analysis） clang/lib/Lex/ 

### token

词法分析器读入组成源程序的字节流，并将他们组成有意义的词素（Lexeme）序列。对于每个词素，词法分析器产生词单元（token）作为输出，并生成相关符号表。词法库包含了几个紧密相连的类，他们涉及到词法和C源码预处理。

相关诊断: clang/include/clang/Basic/DiagnosticLexKinds.td

词法单元（Token）的定义：TokenKinds.def（clang/include/clang/Basic/）

Clang的保留字定义在 TokenKinds.def，如常见的if或for关键字

说明TokenKinds.def中定义了许多诸如TOK和KEYWORD等宏，实际相关宏只有在外部引用程序引用TokenKinds.def前定义才会真正有意义；而且引用位置不同，其意义可能不同


### Preprocessor类 ----- 返回下一个Token

Preprocessor是词法分析中的一个主要接口,可以看到，词法分析时在预处理过程中初始化的即Preprocessor.

在词法分析中，预处理程序被初始调用的主要程序是[CompilerInstance::createPreprocessor（lib/Frontend/CompilerInstance.cpp）](https://github.com/Ewenwan/llvm-project/blob/3745623ba574effe6c157fde7b556734bb7456b5/clang/lib/Frontend/CompilerInstance.cpp#L379)

之后是 FrontEnd Action -> /clang/lib/Lex/Preprocessor.cpp

其核心程序是[Preprocessor::Lex](https://github.com/Ewenwan/llvm-project/blob/3745623ba574effe6c157fde7b556734bb7456b5/clang/lib/Lex/Preprocessor.cpp#L886),该程序返回下一个Token


### Token类  clang/include/clang/Lex/Token.h 

Token类用于表述电仪的词法单元。Token被用于词法/预处理单元，但并不会在这些库以外存在（如，Token不会存在于AST中）.
[Token.h](https://github.com/Ewenwan/llvm-project/blob/master/clang/include/clang/Lex/Token.h)


## 2. 语法分析（Syntactic analysis） libclangParse libclangAST

语法分析主要是解析词法分析产生的词法单元（token）并生成抽象语法树（ATS）。如同自然语言一样，语法分析并不检查语法的上下文的合理性，其仅仅是从语法角度确认是否正确。 

语法分析的核心数据结构：Decl、Stmt、Type对应于声明、指令、类型。所有的被Clang描述的C/C++类都是继承于这三个类 

源码  clang/lib/Parse/ 和clang/lib/AST/

诊断信息  DiagnosticParseKinds.td

语法分析器是使用递归下降（recursive-descent）的语法分析。

clang的Parser是由clang::ParseAST执行的。

libclangAST：

        提供了类用于：表示C AST、C类型、内建函数和一些用于分析和操作AST的功能（visitors、漂亮的打印输出等）
        源码定义：lib/AST
        重要的AST节点：Type、Decl、DeclContext、Stmt
        
### Type类和他的派生类 clang/lib/AST/Type.cpp 

Type类及其派生类是AST中非常重要的一部分。通过ASTContext访问Type（clang/ast/ASTContext.h），在需要时他隐式的唯一的创建他们。Type有一些不明显的特征：1）他们不捕获类似const或volatile类型修饰符（See QualType）；2）他们隐含的捕获typedef信息。一旦创建，type将是不可变的。


### 声明，Decl类 clang/AST/DeclBase.h

表示一个声明（declaration）或定义（definition）。例如：变量、typedef、函数、结构等


### 声明上下文，DeclContext类  clang/AST/DeclBase.h

程序中每个声明都存在于某一个声明上下文中，类似翻译单元、名字空间、类或函数。Clang中的声明上下文是由类DeclContext类进行描述：各种AST节点声明上下文均派生于此（TranslationUnitDecl、NamespaceDecl、RecordDecl、FunctionDecl等）

DeclContext类对于每个声明上下文提供了一些公用功能：

1. 与源码为中心和语义为中心的声明视图
   
   DeclContext提供了两种声明视图：源码为中心视图准确表示源程序代码，包括多个实体的声明（参见再宣告和重载）。而语义为中心的视图表示了程序语义。这两个视图在AST构造时与语义分析保持同步。
   
2.在上下文中保存声明

  每个声明上下文中都包含若干的声明。例如：C++类（被表示为RecordDecl）包含了一些成员函数、域、嵌套类型等等。所有这些声明都保存在DeclContext中：即可以由容器迭代操作获得 
   
  这个机制提供了基于源码视图的声明上下文视图。

3. 在上下文中查找声明

由DeclarationName类型指定的声明在声明上下文中查找声明   lookup()

该机制提供了基于语义为中心的声明上下文视图

4. 声明所有者

   DeclContext包含了所有在其中声明的声明上下文，并负责管理他们并以及序列化（反）他们所有声明都保存在声明上下文中，并可以查询每个保存在其中的声明信息。关于声明上下文可以查看词法和语义分析一节.
   
5. 再宣告和重载
   
   在翻译单元中，公共的实体可能会被声明多次。
   
   表达式”f”在以源码为中心的和以语义为中心的上下文中的视图有所不同，在以源码为中心的声明上下文中，再宣告与他在源码中声明的位置有关。在语义为中心的视图中，会使用最近的视图替换最初的声明。而基于DeclContext::look操作将返回基于语义视图的上下文。
   
6. 词法和语义上下文

对于每个声明可能存在两个不同的声明上下文：词法上下文，对应于源码视图的声明上下文。语义上下文对应于语法视图的。Decl::getLexicalDeclContext（clang/AST/DeclBase.h）返回词法声明上下文。而Decl::getDeclContext返回基于语义上下文，返回的两个值都是指向DeclContext的指针。

7.透明声明上下文（TransparentDeclaration Contexts）

现于枚举类型，Red是Color中的成员，但是，我们在Color外引用Red时并不需要限定名：Color；

8.多重定义的声明上下文（Multiply-DefinedDeclaration Contexts）

可以由DeclContext::isTransparentContext确认是否是透明声明。

###  Stmt 指令 类  clang/AST/Stmt.h   for do goto return ...

### QualType类  查询类

QualType被设计为一个微不足道的、微小的通过传值用于高效查询的类。QualType的思想是将类型修饰符（const、volatile、restrict以及语言扩展所需的修饰符）与他们自己的类型分开保存。QualType概念上是一对“Type *”和他们的类型修饰符。类型限定符只是占用指针的低位。


### 声明名字（Declarationnames）

DeclarationName（clang/AST/DeclarationName.h）用来描述clang中的声明名字。声明在C族语言中有一些不同的形式。多数的声明被命名为简单的标识，例如：f(int x)中的声明f和x。在C++中，声明可以构造类的构造函数、类的析构函数、重载操作符合转换函数。


### CFG类 控制流程图？ （Context Free Grammar，上下文无关文法）
CFG是用于描述单个指令（Stmt *）的源码级控制流程图？。典型的CFG实例为构造函数体（典型的是一个CompoundStmt实例），但是也可以表示任何Stmt派生类的控制流。控制流图通常对给定函数执行流-或路径-敏感的分析特别有用。


## 3.语义分析（Semantic Analysis）

libclangSema



## 4. 中间代码生成（IR Generator）  libclangCodeGen

libclangCodeGen

## 5. 其他库介绍

libclangAnaylysis：用于进行静态分析用的


libclangRewrite：编辑文本缓冲区（代码重写转换非常重要，如重构）


libclangBasic：诊断、源码定位、源码缓冲区抽象化、输入源文件的文件缓冲区

Clang诊断子系统是一个编译器与人交互的重要部分。诊断是当代码不正确或可疑时产生警告和错误。在clang中，每个诊断产生（最少）一个唯一标识ID、一个相关的英文、SourceLocation源码位置“放置一个^”和一个严重性（例如：WARNING或ERROR）。他可以选择包含一些参数给争端（如使用%0填充字串）以及相关源码区域。

Clang diagnostics   Diagnostic::Level  enum  [ NOTE  WARNING  EXTENSION  EXTWARN ERROR ]


SourceLocation：表示源代码的位置。See：clang/Basic/SourceLocation.h。SourceLocation因为被嵌入到许多AST中，因此，该类必须足够小。

SourceLocation：通常是和SourceManager一同使用，用于对一个位置信息的两条信息进行编码。见：clang/Basic/SourceManager.h

SourceRange：是SourceLocation.h中类，表示源码的范围：【first，last】。First和last都是SourceLocation





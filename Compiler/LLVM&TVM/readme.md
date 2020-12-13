# LLVM &TVM 相关

[A Tour to LLVM IR（上）](https://zhuanlan.zhihu.com/p/66793637)

[LLVM Language Reference Manual](https://releases.llvm.org/2.7/docs/LangRef.html#i_br)


##  下载LLVM 8.0.1源码并编译

* Step 1) 首先是一些必要依赖的安装
  ``` bash
  sudo apt-get install -y cmake xz-utils build-essential wget
  ```
* Step 2) 在仓库之外,选择好你的工作目录,下载源码,最后得到名为llvm的目录
  ``` bash
  # 下载
  wget https://github.com/llvm/llvm-project/releases/download/llvmorg-8.0.1/llvm-8.0.1.src.tar.xz
  wget https://github.com/llvm/llvm-project/releases/download/llvmorg-8.0.1/cfe-8.0.1.src.tar.xz
  # 解压缩
  tar xvf llvm-8.0.1.src.tar.xz
  mv llvm-8.0.1.src llvm
  tar xvf cfe-8.0.1.src.tar.xz
  mv cfe-8.0.1.src llvm/tools/clang
  ```
* Step 3) 编译并安装.这里在内存及硬盘充足的条件下,推荐`Debug`配置的编译,这更能让你体验"较大的项目"的编译过程.否则请采用`Release`配置的编译
  ``` bash
  mkdir llvm-build && cd llvm-build
  # Release
  cmake ../llvm -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`cd .. && pwd`/llvm-install
  # Debug
  cmake ../llvm -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=`cd .. && pwd`/llvm-install
  # 编译安装,下面的N表示的是你采取的同时执行的编译任务的数目.你需要将其替换为具体的数值,如8,4,1
  # 如果内存充足,一般推荐将N设置为cpu核数的2倍,如果未指明N,将会尽可能多地并行编译
  make -jN install
  # 这一过程可能有半小时至两小时不等,如果内存不足,可以尝试减小N并重新make,已经编译好的部分不会重新编译
  ```
* Step 4) 配置PATH,使得生成的二进制文件方便使用.配置PATH在以后的工作中也会是经常用到的,希望大家熟练掌握(或者至少熟练如何搜索)
  ``` bash
  # 将llvm-install/bin目录的完整路径,加入到环境变量PATH中
  # 假设该完整路径为the_path_to_your_llvm-install_bin,
  # 如果你的默认shell是zsh,可以在~/.zshrc中加入一行:
  export PATH=the_path_to_your_llvm-install_bin:$PATH
  # 然后执行
  source ~/.zshrc
  # 如果你的默认shell是bash,欢迎自行google/baidu

  # 最后,检查PATH配置是否成功,执行命令:
  llvm-config --version
  # 成功标志:
  8.0.1 #或类似
  # 失败标志:
  zsh: command not found: llvm-config #或类似
  ```
  
  
## 函数构建 赋值语句构建 assign_generator


```c
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <memory>

#ifdef DEBUG  // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl;  // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

using namespace llvm;

#define CONST(num) ConstantInt::get(context, APInt(32, num))

int main()
{
    LLVMContext context;
    Type *TYPE32 = Type::getInt32Ty(context);
    IRBuilder<> builder(context);
    auto module = new Module("assign", context);

    auto mainFun = Function::Create(FunctionType::get(TYPE32, false), GlobalValue::ExternalLinkage, "main", module);

    auto bb = BasicBlock::Create(context, "entry", mainFun); //代码中只有 entry 一个 BasicBlock。
    // Begin: BasicBlock entry
    builder.SetInsertPoint(bb);

    auto a = builder.CreateAlloca(TYPE32);
    a->setAlignment(4);                                //Allocate a.
    builder.CreateStore(CONST(1), a)->setAlignment(4); // Store the value to a.

    auto retval = builder.CreateLoad(a);
    retval->setAlignment(4); // Load the value to retval.

    builder.CreateRet(retval); // Return.
    // End: BasicBlock entry
    module->print(outs(),nullptr);
    delete module;
    return 0;
}

// 对于的 llvm IR
/*
; ModuleID = 'assign.c'
source_filename = "assign.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
    %a = alloca i32, align 4        ; 在栈上给 a 分配内存
    store i32 1, i32* %a, align 4   ; 在 a 的地址里写入 1
    %retval = load i32, i32* %a, align 4 ; 从 a 的地址里读出返回值
    ret i32 %retval                 ; 返回
}
*/

```


## 函数调用语句构建
代码中共有两个BasicBlock，都叫做entry，但是存在于不同的函数中，对应关系见注释。

```c

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <memory>

#ifdef DEBUG                                             // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl; // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

using namespace llvm;

#define CONST(num) ConstantInt::get(context, APInt(32, num))

int main()
{
    LLVMContext context;
    Type *TYPE32 = Type::getInt32Ty(context);
    IRBuilder<> builder(context);
    auto module = new Module("call", context);

    std::vector<Type *> Ints(1, TYPE32); // Declare type of function args.
    auto calleeFunction = Function::Create(FunctionType::get(TYPE32, Ints, false),
                                           GlobalValue::LinkageTypes::ExternalLinkage,
                                           "callee", module);

    auto entry = BasicBlock::Create(context, "entry", calleeFunction);
    // Begin: BasicBlock entry in calleeFunction.
    builder.SetInsertPoint(entry);

    // Get args.
    std::vector<Value *> args;
    for (auto arg = calleeFunction->arg_begin(); arg != calleeFunction->arg_end(); arg++)
    {
        args.push_back(arg);
    }

    auto mul = builder.CreateMul(CONST(2), args[0]);
    builder.CreateRet(mul); // End of calleeFunction.
    // End: BasicBlock entry in calleeFunction.

    auto mainFun = Function::Create(FunctionType::get(TYPE32, false), GlobalValue::LinkageTypes::ExternalLinkage, "main", module);
    entry = BasicBlock::Create(context, "entry", mainFun);
    // Begin: BasicBlock entry in mainFun.
    builder.SetInsertPoint(entry);

    auto call = builder.CreateCall(calleeFunction, {CONST(10)}); // Call the function.
    builder.CreateRet(call);
    // End: BasicBlock entry in mainFun.

    module->print(outs(), nullptr);
    delete module;
    return 0;
}

/*

; ModuleID = 'call.c'
source_filename = "call.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @callee(i32 %a) {    ; 声明 callee 函数.
    %mul = mul i32 2, %a
    ret i32 %mul                ; 返回 2 * a.
}

define i32 @main() {
    %retval = call i32 @callee(i32 10)  ; 调用函数.
    ret i32 %retval                     ; 返回.
}

*/


```


## GCD 生成


```c

  
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <memory>

#ifdef DEBUG  // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl;  // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

using namespace llvm;  // 指明命名空间为llvm
#define CONST(num) \
  ConstantInt::get(context, APInt(32, num))  //得到常数值的表示,方便后面多次用到

int main() {
  LLVMContext context;
  Type *TYPE32 = Type::getInt32Ty(context);
  IRBuilder<> builder(context);
  auto module = new Module("gcd", context);  // module name是什么无关紧要

  // 函数参数类型的vector
  std::vector<Type *> Ints(2, TYPE32);
  auto gcdFun = Function::Create(FunctionType::get(TYPE32, Ints, false),
                                 GlobalValue::LinkageTypes::ExternalLinkage,
                                 "gcd", module);

  // BB的名字在生成中无所谓,但是可以方便阅读
  auto bb = BasicBlock::Create(context, "entry", gcdFun);
  builder.SetInsertPoint(bb);                     // 一个BB的开始
  auto retAlloca = builder.CreateAlloca(TYPE32);  // 返回值的空间分配
  auto uAlloca = builder.CreateAlloca(TYPE32);    // 参数u的空间分配
  auto vAlloca = builder.CreateAlloca(TYPE32);    // 参数v的空间分配

  std::vector<Value *> args;  //获取gcd函数的参数,通过iterator
  for (auto arg = gcdFun->arg_begin(); arg != gcdFun->arg_end(); arg++) {
    args.push_back(arg);
  }

  builder.CreateStore(args[0], uAlloca);  //将参数u store下来
  builder.CreateStore(args[1], vAlloca);  //将参数v store下来

  auto vLoad = builder.CreateLoad(vAlloca);           //将参数v load上来
  auto icmp = builder.CreateICmpEQ(vLoad, CONST(0));  // v和0的比较,注意ICMPEQ

  auto trueBB = BasicBlock::Create(context, "trueBB", gcdFun);    // true分支
  auto falseBB = BasicBlock::Create(context, "falseBB", gcdFun);  // false分支
  auto retBB = BasicBlock::Create(
      context, "", gcdFun);  // return分支,提前create,以便true分支可以br

  auto br = builder.CreateCondBr(icmp, trueBB, falseBB);  // 条件BR
  DEBUG_OUTPUT // 我调试的时候故意留下来的,以醒目地提醒你这个调试用的宏定义方法
  builder.SetInsertPoint(trueBB);  // if true; 分支的开始需要SetInsertPoint设置
  auto uLoad = builder.CreateLoad(uAlloca);
  builder.CreateStore(uLoad, retAlloca);
  builder.CreateBr(retBB);  // br retBB

  builder.SetInsertPoint(falseBB);  // if false
  uLoad = builder.CreateLoad(uAlloca);
  vLoad = builder.CreateLoad(vAlloca);
  auto div = builder.CreateSDiv(uLoad, vLoad);  // SDIV - div with S flag
  auto mul = builder.CreateNSWMul(div, vLoad);  // NSWMUL - mul with NSW flags
  auto sub = builder.CreateNSWSub(uLoad, mul);  // the same
  auto call = builder.CreateCall(gcdFun, {vLoad, sub});
  // {vLoad, sub} - 参数array
  builder.CreateStore(call, retAlloca);
  builder.CreateBr(retBB);  // br retBB

  builder.SetInsertPoint(retBB);  // ret分支
  auto retLoad = builder.CreateLoad(retAlloca);
  builder.CreateRet(retLoad);

  // main函数
  auto mainFun = Function::Create(FunctionType::get(TYPE32, false),
                                  GlobalValue::LinkageTypes::ExternalLinkage,
                                  "main", module);
  bb = BasicBlock::Create(context, "entry", mainFun);
  // BasicBlock的名字在生成中无所谓,但是可以方便阅读
  builder.SetInsertPoint(bb);

  auto xAlloca = builder.CreateAlloca(TYPE32);
  auto yAlloca = builder.CreateAlloca(TYPE32);
  auto tempAlloca = builder.CreateAlloca(TYPE32);

  builder.CreateStore(CONST(0), tempAlloca);
  builder.CreateStore(CONST(72), xAlloca);
  builder.CreateStore(CONST(18), yAlloca);

  auto xLoad = builder.CreateLoad(xAlloca);
  auto yLoad = builder.CreateLoad(yAlloca);
  icmp = builder.CreateICmpSLT(xLoad, yLoad);  // ICMPLT with S

  trueBB = BasicBlock::Create(context, "trueBB", mainFun);
  falseBB = BasicBlock::Create(context, "falseBB", mainFun);
  builder.CreateCondBr(icmp, trueBB, falseBB);

  builder.SetInsertPoint(trueBB);
  builder.CreateStore(xLoad, tempAlloca);
  builder.CreateStore(yLoad, xAlloca);
  auto tempLoad = builder.CreateLoad(tempAlloca);
  builder.CreateStore(tempLoad, yAlloca);
  builder.CreateBr(falseBB);  // 注意在下一个BB之前要Br一下

  builder.SetInsertPoint(falseBB);
  xLoad = builder.CreateLoad(xAlloca);
  yLoad = builder.CreateLoad(yAlloca);
  call = builder.CreateCall(gcdFun, {xLoad, yLoad});
  builder.CreateRet(call);
  builder.ClearInsertionPoint();
  // 给这么多注释了,但是可能你们还是会弄很多bug
  // 所以强烈建议配置AutoComplete,效率会大大提高!
  // 别人配了AutoComplete,只花1小时coding
  // 你没有配AutoComplete,找method花5小时,debug花5小时,肯定哭唧唧!
  // 最后,如果猜不到某个IR指令对应的C++的函数,建议把指令翻译成英语然后在method列表中搜索一下
  // 最后的最后,这个例子只涉及到了一点基本的指令生成,
  // 对于额外的指令,包括数组,在之后的实验中可能需要大家好好搜索一下思考一下,
  // 还有涉及到的C++语法,可以在gitlab上发issue提问或者向大家提供指导,会有bonus哦!
  // 对于这个例子里的代码风格/用法,如果有好的建议也欢迎提出!
  module->print(outs(), nullptr);
  delete module;
  return 0;
}


/*


*/
```

## IF 语句构建

 代码中共有 3 个 BasicBlock，分别是 entry, truebb, falsebb，对应关系在注释中给出。
 
```c
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <memory>

#ifdef DEBUG                                             // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl; // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

using namespace llvm;

#define CONST(num) ConstantInt::get(context, APInt(32, num))

int main()
{
    LLVMContext context;
    Type *TYPE32 = Type::getInt32Ty(context);
    IRBuilder<> builder(context);
    auto module = new Module("if", context);

    auto mainFun = Function::Create(FunctionType::get(TYPE32, false), GlobalValue::ExternalLinkage, "main", module);

    auto bb = BasicBlock::Create(context, "entry", mainFun);
    // Begin: BasicBlock entry
    builder.SetInsertPoint(bb);

    auto truebb = BasicBlock::Create(context, "truebb", mainFun);
    auto falsebb = BasicBlock::Create(context, "falsebb", mainFun);

    auto icmp = builder.CreateICmpSGT(CONST(2), CONST(1)); // ? 2>1.

    auto br = builder.CreateCondBr(icmp, truebb, falsebb); // Create conditional branch.
    // End: BasicBlock entry

    // Begin: BasicBlock truebb
    builder.SetInsertPoint(truebb);
    builder.CreateRet(CONST(1)); // Return 1 in true branch.
    // End: BasicBlock truebb

    // Begin: BasicBlock falsebb
    builder.SetInsertPoint(falsebb);
    builder.CreateRet(CONST(0)); // Return 0 in false branch.
    // End: BasicBlock falsebb

    module->print(outs(), nullptr);
    delete module;
    return 0;
}

/*
; ModuleID = 'if.c'
source_filename = "if.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
    %cmp = icmp sgt i32 2, 1           ; 比较 2 是否大于 1，结果赋给 %cmp
    br i1 %cmp, label %if.then, label %if.end  ; 分支指令，如果 %cmp 为真，则跳转 %if.then 否则 %if.end

if.then:
    ret i32 1               ;返回
if.end:
    ret i32 0               ;返回

}
*/
```



## while 语句构建

代码中共有 4 个basicBlock，分别是entry, loop, truebb, ret，对应关系见注释。

```c
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <memory>

#ifdef DEBUG                                             // 用于调试信息,大家可以在编译过程中通过" -DDEBUG"来开启这一选项
#define DEBUG_OUTPUT std::cout << __LINE__ << std::endl; // 输出行号的简单示例
#else
#define DEBUG_OUTPUT
#endif

using namespace llvm;

#define CONST(num) ConstantInt::get(context, APInt(32, num))

int main()
{
    LLVMContext context;
    Type *TYPE32 = Type::getInt32Ty(context);
    IRBuilder<> builder(context);
    auto module = new Module("while", context);

    auto mainFun = Function::Create(FunctionType::get(TYPE32, false), GlobalValue::ExternalLinkage, "main", module);

    auto bb = BasicBlock::Create(context, "entry", mainFun);
    builder.SetInsertPoint(bb);             // Entry.
    // Begin: BasicBlock entry
    auto a = builder.CreateAlloca(TYPE32);
    auto i = builder.CreateAlloca(TYPE32);

    builder.CreateStore(CONST(10), a);
    builder.CreateStore(CONST(0), i);

    auto loop = BasicBlock::Create(context, "loop", mainFun);
    auto truebb =BasicBlock::Create(context, "truebb", mainFun);
    auto ret = BasicBlock::Create(context, "ret", mainFun);
    builder.CreateBr(loop);                 // Jump to loop.
    // End: BasicBlock entry

    // BasicBlock: BasicBlock loop
    builder.SetInsertPoint(loop);

    auto iLoad = builder.CreateLoad(i);
    auto aLoad = builder.CreateLoad(a);
    auto icmp = builder.CreateICmpSLT(iLoad, CONST(10)); // See if should break.
    builder.CreateCondBr(icmp,truebb,ret);
    // End: BasicBlock loop

    // Begin: BasicBlock truebb
    builder.SetInsertPoint(truebb);
    auto inc = builder.CreateAdd(iLoad, CONST(1));
    auto tempa = builder.CreateAdd(inc, aLoad);
    builder.CreateStore(inc, i);
    builder.CreateStore(tempa, a); // i = i + 1; a = a + i;
    builder.CreateBr(loop);
    // End: BasicBlock truebb

    // Begin: BasicBlock ret
    builder.SetInsertPoint(ret);
    auto retval = builder.CreateLoad(a);
    builder.CreateRet(retval); // Return a
    // End: BasicBlock ret

    module->print(outs(), nullptr);
    delete module;
    return 0;
}

/*

; ModuleID = 'while.c'
source_filename = "while.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
    %a = alloca i32, align 4
    %i = alloca i32, align 4      ; 定义 a, i

    store i32 10, i32* %a, align 4
    store i32 0, i32* %i, align 4   ; 赋初值

    %0 = load i32, i32* %i, align 4

    %cmp = icmp slt i32 %0, 10     ; i<10 ?
    br i1 %cmp, label %loop, label %return
    
loop:
    %1 = load i32, i32* %i, align 4
    %2 = load i32, i32* %a, align 4
    %inc = add i32 %1, 1                
    store i32 %inc, i32* %i, align 4    ;i = i + 1
    %add = add i32 %inc, %2             
    store i32 %add, i32* %a, align 4    ; a = a + i

    %3 = icmp slt i32 %inc, 10          ; i<10?
    br i1 %3, label %loop, label %return

return:
    %retval = load i32, i32* %a, align 4;return a
    ret i32 %retval
}

*/

```



## 
```c


/*

*/
```


## 
```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c

/*

*/

```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c


/*

*/
```


##

```c

/*

*/

```


##

```c

/*

*/

```






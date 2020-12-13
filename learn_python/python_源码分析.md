# python_源码分析

使用 c语言 构建了一套 描述python 变量的 面向对象的 架构，以PyObject为中心，构建对象类型系统、内存分配以及运行时状态。

## 架构：语言前端+解释器中端+后端运行时系统
```

语言前端        核心解释器                    运行时环境

python模块      词法语法解析编译执行           对象类型系统，内存分配，运行时状态
                Scanner 词法分析器得到token
core modules    Parser 语法分析器得到 AST      对象/类型系统(Object/Type structures)（int对象、list对象等）
library         Compiler编译器得到字节码       内存分配器（Memory Allocator）（对象创建释放（malloc/free上层接口））
user-defined    Evaluator代码解释执行引擎      运行时状态（Current State of Python）有穷状态机

```

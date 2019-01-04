# Boost C++ 库
[参考](https://python.ctolib.com/docs/sfile/the-boost-cpp-libraries/0.html)

# 简介
    Boost C++ 库 是一组基于C++标准的现代库。
    要增强C++项目的生产力，除了C++标准以外，Boost C++ 库是一个不错的选择。 
    由于当前版本的C++标准在2003年修订之后，C++又有了新的发展，所以 Boost C++ 库提供了许多新的特性。 
    由于有了 Boost C++ 库，我们无需等待下一个版本的C++标准，就可以立即享用C++演化中取得的最新进展。
    Boost C++ 库具有良好的声誉，这基于它们的使用已被证实是非常有价值的。 
    在面试中询问关于 Boost C++ 库的知识是不常见的，
    因为知道这些库的开发人员通常也清楚C++的最新创新，并且能够编写和理解现代的C++代码。
    
    Boost C++ 库均带有源代码。
    其中大多数库只包含头文件，可以直接使用，但也有一些库需要编译。 
    为了尽可能容易安装，可以使用 Boost Jam 进行自动安装。
    无需逐个库进行检查和编译，Boost Jam 自动安装整个库集。
    它支持许多操作系统和编译器，并且知道如何基于适当的配置文件来编译单个库。
    
## 内部各种库介绍
    1. Boost.Any 
      提供了一个名为 boost::any 的数据类型，可以存放任意的类型。
      例如，一个类型为 boost::any 的变量可以先 存放一个 int 类型的值，
      然后替换为一个 std::string 类型的字符串。
      
    2.Boost.Array 
      可以把 C++ 数组视同 C++ 标准的容器。
      
    3. Boost.Asio 
      可用于开发异步处理数据的应用，如网络应用。
      
    4. Boost.Bimap 
       提供了一个名为 boost::bimap 的类，它类似于 std::map. 
       主要的差别在于 boost::bimap 可以 同时 从 键 和 值 进行搜索。
       
    5. Boost.Bind   
       绑定???
       是一种适配器，可以将函数作为模板参数，即使该函数的签名与模板参数不兼容。
       
    6. Boost.Conversion 
       提供了三个转型操作符，分别执行向下转型、交叉转型，以及不同数字类型间的值转换。
       
    7. Boost.DateTime 
       可用于以灵活的格式处理、读入和写出日期及时间值。
       
    8. Boost.Exception 
       可以在抛出的异常中加入额外的数据，以便在 catch 处理中提供更多的信息。 
       这有助于更容易地调试，以及对异常情况更好地作出反应。
    
    9. Boost.Filesystem 
       提供了一个类来处理路径信息，还包含了几个访问文件和目录的函数。
    
    10.Boost.Format 
       以一个类型安全且可扩展的 boost::format 类替代了 std::printf() 函数。
    
    11.Boost.Function 
       简化了函数指针的定义。
    
    12.Boost.Interprocess 
       允许多个应用通过共享内存以快速、高效的方式进行通信。
    
    13.Boost.Lambda 
       可以定义匿名的函数。 
       代码被内联地声明和执行，避免了单独的函数调用。
       
    14.Boost.Multiindex
       多索引
       定义了一些新的容器，它们可以同时支持多个接口，
       如 std::vector 和 std::map 的接口。
    
    15.Boost.NumericConversion 
       提供了一个转型操作符，
       可以安全地在不同的数字类型间进行值转换，
       不会生成上溢出或下溢出的条件。
    
    16.Boost.PointerContainer 
       提供了专门为动态分配对象进行优化的容器。
    
    17.Boost.Ref 
       的适配器可以将不可复制对象的引用传给需要复制的函数。
    
    18.Boost.Regex
       提供了通过正则表达式进行文本搜索的函数。
    
    19.Boost.Serialization
       通过 Boost.Serialization，对象可以被序列化，
       如保存在文件中，并在以后重新导入。
    
    20.Boost.Signal 
       是一个事件处理的框架，基于所谓的 signal/slot 概念。
       函数与信号相关联 并在 信号被触发 时 自动被调用。
    
    21.Boost.SmartPoiners 
       提供了多个智能指针，简化了动态分配对象的管理。
    
    22.Boost.Spirit 
       可以用类似于 EBNF (扩展巴科斯范式)的语法生成词法分析器。
    
    23.Boost.StringAlgorithms 
       提供了多个独立的函数，以方便处理字符串。
    
    24.Boost.System 
       提供了一个处理系统相关或应用相关错误代码的框架。
    
    25.Boost.Thread 
       可用于开发多线程应用。
    
    26.Boost.Tokenizer 
       可以对一个字符串的各个组件进行迭代。
    
    27.Boost.Tuple 
       提供了泛化版的 std::pair，可以将任意数量的数据组在一起。
    
    28.Boost.Unordered 
       扩展了 C++ 标准的容器，增加了boost::unordered_set 和 boost::unordered_map.
    
    29.Boost.Variant 
       可以定义多个数据类型，类似于 union, 将多个数据类型组在一起。
       Boost.Variant 比 union 优胜的地方在于它可以使用类。

# 1. 智能指针 SmartPoiners


# 2. 函数对象  Boost.Bind Boost.Ref Boost.Function Boost.Lambda
## 2.1 概述
## 2.2 Boost.Bind
## 2.3 Boost.Ref
## 2.4 Boost.Function
## 2.5 Boost.Lambda

# 3. 事件处理 信号Boost.Signal  连接Connections  



# 4. 

# 5. 

# 6. 

# 7. 

# 8. 

# 9. 


# 10. 

# 11. 

# 12.


# 13.


# 14.


# 15.


# 16.


# 17. 


# 18. 

    

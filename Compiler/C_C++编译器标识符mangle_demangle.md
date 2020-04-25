# C_C++编译器标识符mangle_demangle

[参考](https://www.cnblogs.com/robinex/p/7892795.html)

C/C++语言在编译以后，函数的名字会被编译器修改，改成编译器内部的名字，这个名字会在链接的时候用到。

将C++源程序标识符(original C++ source identifier)转换成C++ ABI标识符(C++ ABI identifier)的过程称为mangle；相反的过程称为demangle。

## ABI
ABI是Application Binary Interface的简称。

C/C++发展的过程中，二进制兼容一直是个问题。不同编译器厂商编译的二进制代码之间兼容性不好，甚至同一个编译器的不同版本之间兼容性也不好。

之后，C拥有了统一的ABI，而C++由于其特性的复杂性以及ABI标准推进不力，一直没有自己的ABI。

这就涉及到标识符的mangle问题。比如，C++源码中的同一个函数名，不同的编译器或不同的编译器版本，编译后的名称可能会有不同。

每个编译器都有一套自己内部的名字，比如对于linux下g++而言。以下是基本的方法：

　　每个方法都是以_Z开头，对于嵌套的名字（比如名字空间中的名字或者是类中间的名字,比如Class::Func）后面紧跟N ， 然后是各个名字空间和类的名字，每个名字前是名字字符的长度，再以E结尾。(如果不是嵌套名字则不需要以E结尾)。

比如对于_Z3foov 就是函数foo() , v 表示参数类型为void。又如N:C:Func 经过修饰后就是 _ZN1N1C4FuncE，这个函数名后面跟参数类型。 如果跟一个整型，那就是_ZN1N1C4FuncEi。 其中，数字为相关字符串的长度。

## RTTI与type_info

　　C++在编译时开启RTTI（Run-Time Type Identification，通过运行时类型识别）特性时，可以在代码中使用typeid操作符（当然还需要包含<typeinfo>），此符号可以对一个变量或者一个类名使用，返回一个type_info对象的引用。编译时会为每种使用到RTTI的特性的C++类都建立一个唯一的type_info对象，并且会包含继承关系，dynamic_cast便是根据这个对象来判断某个基类对象的指针能否向下转换成子类对象的指针。
  
### demangle

将C++ ABI标识符(C++ ABI identifier)转换成C++源程序标识符(original C++ source identifier)的过程称为demangle。更简单的说，识别C++编译以后的函数名的过程，就叫demangle。

在libstdc++里关于abi命名空间的文档中(https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/namespaces.html)，介绍了GCC所使用的跨厂商(cross-vendor) C++ ABI，其中暴露的一个函数abi::__cxa_demangle就是用于demangling。

```c
  static const auto& name = *(new std::string(demangle(typeid(T).name())));
  return name.c_str();
```


地球人都知道C++里有一个typeid操作符可以用来获取一个类型/表达式的名称：
```c
std::cout << typeid(int).name() << std::endl;
```
但是这个name()的返回值是取决于编译器的，在vc和gcc中打印出来的结果如下：
```c
int // vc      demangle 了的
i   // gcc     mangle 了的  
```

```c
// gcc
char* name = abi::__cxa_demangle(typeid(const int&).name(), nullptr, nullptr, nullptr);
std::cout << name << std::endl;
free(name)
```



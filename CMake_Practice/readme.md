# CMake 学习笔记


# CMake 特点

      cmake 的特点主要有:
      1,开放源代码,使用类 BSD 许可发布。http://cmake.org/HTML/Copyright.html
      2,跨平台,并可生成 native 编译配置文件,在 Linux/Unix 平台,生成 makefile,在
      苹果平台,可以生成 xcode,在 Windows 平台,可以生成 MSVC 的工程文件。
      3,能够管理大型项目,KDE4 就是最好的证明。
      4,简化编译构建过程和编译过程。Cmake 的工具链非常简单:cmake+make。
      5,高效虑,按照 KDE 官方说法,CMake 构建 KDE4 的 kdelibs 要比使用 autotools 来
      构建 KDE3.5.6 的 kdelibs 快 40% ,主要是因为 Cmake 在工具链中没有 libtool。
      6,可扩展,可以为 cmake 编写特定功能的模块,扩充 cmake 功能。
      
# helloword
### main.c 文件内容:
    //main.c
    #include <stdio.h>
    int main()
    {
    printf(“Hello World from t1 Main!\n”);
    return 0;
    }
    
### CmakeLists.txt 文件内容:
         # PROJECT 指令的语法是 PROJECT(projectname [CXX] [C] [Java])  默认情况表示支持所有语言
         # 这个指令隐式的定义了两个 cmake 变量: <projectname>_BINARY_DIR 以及<projectname>_SOURCE_DIR
         # HELLO_BINARY_DIR 以及 HELLO_SOURCE_DIR  调用变量的值需要使用 ${变量名} 得到变量的值
         # IF 语句中直接使用变量名
         # 同时 cmake 系统也帮助我们预定义了 PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR 变量
    PROJECT (HELLO)#工程名
    
         # 也可以写成 SET(SRC_LIST “main.c”)  
         # 为了防止 源文件名 中间带有空格 fu nc.c 写成 SET(SRC_LIST “fu nc.c”)不会出错
    SET(SRC_LIST main.c)#代替  显示定义变量 多个源文件可定义 SET(SRC_LIST main.c t1.c t2.c)
    
        # MESSAGE 指令的语法是:
        # MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] "message to display"...)
        # SEND_ERROR,产生错误,生成过程被跳过。 
        # SATUS ,输出前缀为 -- 的信息。
        # FATAL_ERROR,立即终止所有 cmake 过程.
    
    MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})#输出信息 编译后的二进制 文件 路径
    MESSAGE(STATUS "This is SOURCE dir " ${HELLO_SOURCE_DIR})#源文件路径
    
        # 添加可执行文件 最前面为 可执行文件名
        # ADD_EXECUTABLE(hello main.c func.c)或者 ADD_EXECUTABLE(hello main.c;func.c)
        # 这里也可以 忽略掉 source 列表中的源文件后缀 ADD_EXECUTABLE(t1 main) 也可以 但是最好加上后缀
    ADD_EXECUTABLE(hello ${SRC_LIST} )#添加可执行文件 使用变量  
    
  # 外部编译
> mkdir build   cd build    cmake ..    make     make clean 清理编译文件
> 变量PROJECT_SOURCE_DIR 指代工程路径 变量PROJECT_BINARY_DIR  指代编译路径 ../build 




    

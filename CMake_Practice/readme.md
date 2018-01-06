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

> 变量PROJECT_SOURCE_DIR 指代工程路径 变量PROJECT_BINARY_DIR  指代编译路径 工程名/build 

# 工程文件
      1,为工程添加一个子目录 src,用来放置工程源代码;         mv main.c src 
      2,添加一个子目录 doc,用来放置这个工程的文档 hello.txt
      3,在工程目录添加文本文件 COPYRIGHT, README;
      4,在工程目录添加一个 runhello.sh 脚本,用来调用 hello 二进制可执行文件
      4,将构建后的目标文件放入构建目录的 bin 子目录;      CMakeList.txt 中添加ADD_SUBDIRECTORY(src bin)
      5,最终安装这些文件:将 hello 二进制与 runhello.sh 安装至/usr/bin,将 doc 目录
      的内容以及 COPYRIGHT/README 安装到/usr/share/doc/cmake/t2
      
      注:
      CMakeList.txt 中添加 ADD_SUBDIRECTORY(src bin)
      ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
      向当前工程添加存放源文件的子目录,并可以指定中间二进制和目标二进制存放的位置。如果不指定bin文件目录，则放入 src中
      
      换个地方保存目标二进制：
      SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)#可执行文件 输出保存地址 工程名/build/bin
      SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)#共享库 输出保存地址        工程名/build/lib

      安装库/可执行文件
      CMAKE 命令行 变量 cmake -DCMAKE_INSTALL_PREFIX=/usr  指定安装路径   默认定义是/usr/local
      cmake 指令 
      INSTALL(TARGETS myrun mylib mystaticlib  # 可执行文件myrun  动态库 mylib   静态库 mystaticlib 
      RUNTIME DESTINATION bin       # 可执行文件 安装路径 ${CMAKE_INSTALL_PREFIX}/bin
      LIBRARY DESTINATION lib       # 动态库文件 安装路径 ${CMAKE_INSTALL_PREFIX}/lib
      ARCHIVE DESTINATION libstatic # 静态库文件 安装路径 ${CMAKE_INSTALL_PREFIX}/libstatic
      )

    
# 静态库与动态库构建
      mkdir -p t3/lib
       
      在 t3 目录下建立 CMakeLists.txt,内容如下:
      PROJECT(HELLOLIB)
      ADD_SUBDIRECTORY(lib)
      
      在 lib 目录下建立两个源文件 hello.c 与 hello.h
      
      hello.c 内容如下:
      
      #include “hello.h”
      void HelloFunc()
      {
      printf(“Hello World\n”);
      }
      
      hello.h 内容如下:
      
      #ifndef HELLO_H
      #define HELLO_H
      #include <stdio.h>
      void HelloFunc();
      #endif

      在 lib 目录下建立 CMakeLists.txt,内容如下:
      SET(LIBHELLO_SRC hello.c)
      ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})# SHARED 表示 创建共享库 对于libXXX.so  STATIC,静态库 对应 libXXX.a
      
      编译共享库 
      mkdir build 
      cd build
      cmake ..
      make
      
      在lib下 得到一个 共享库 libhello.so 
      
      






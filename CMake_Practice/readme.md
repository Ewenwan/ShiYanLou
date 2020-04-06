# CMake 学习笔记

[CMake 示例用法](https://github.com/Ewenwan/Linux_Code_Test/tree/master/Samples_CMake)

[cmake-examples](https://github.com/Ewenwan/cmake-examples)

```C
PROJECT(CppBaseTest) # 项目名称
CMAKE_MINIMUM_REQUIRED(VERSION 3.0) # cmakelists版本要求

#  SET设置变量 支持C++11    -O2  优化等级
SET(CMAKE_C_FLAGS "${CMAK_C_FLAGS} -g -Wall -O2 -std=c11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -Wall -O2 -std=c++11")
# 支持C++14, when gcc version > 5.1, use -std=c++14 instead of c++1y
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -Wall -O2 -std=c++1y")

# MESSAGE 编译时打印信息
MESSAGE(STATUS "project source dir: ${PROJECT_SOURCE_DIR}")

# SET设置变量
SET(PATH_SRC_FILES ${PROJECT_SOURCE_DIR}/./../../demo/CppBaseTest)
MESSAGE(STATUS "path src files: ${PATH_SRC_FILES}")

# INCLUDE_DIRECTORIES 指定头文件的搜索路径
INCLUDE_DIRECTORIES(${PATH_SRC_FILES})

# 递归查询所有匹配的文件:*.cpp
FILE(GLOB_RECURSE CPP_LIST ${PATH_SRC_FILES}/*.cpp)
FILE(GLOB_RECURSE C_LIST ${PATH_SRC_FILES}/*.c)
#MESSAGE(STATUS "cpp list: ${C_LIST}")

# 编译可执行程序
ADD_EXECUTABLE(CppBaseTest ${CPP_LIST} ${C_LIST})
# 用来为target添加需要链接的共享库，指定工程所用的依赖库，包括动态库和静态库
TARGET_LINK_LIBRARIES(CppBaseTest pthread)

# 查找库
# 查找OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
IF(OpenCV_FOUND)
	MESSAGE("===== support OpenCV =====")
	MESSAGE(STATUS "OpenCV library status:")
	MESSAGE(STATUS "	version: ${OpenCV_VERSION}")
	MESSAGE(STATUS "	include path: ${OpenCV_INCLUDE_DIRS}")
	MESSAGE(STATUS "	install path: ${OpenCV_INSTALL_PATH}")
	MESSAGE(STATUS "	libraries: ${OpenCV_LIBS}")
	INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
ELSE()
	MESSAGE("##### not support OpenCV #####")
ENDIF()

# 查找OpenMP
FIND_PACKAGE(OpenMP REQUIRED)
IF(OpenMP_FOUND)
	MESSAGE("===== support OpenMP =====")
	MESSAGE(STATUS "OpenMP library status:")
	MESSAGE(STATUS "	version: ${OpenMP_VERSION}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ELSE()
	MESSAGE("##### not support OpenMP #####")
ENDIF()

# 查找OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
	MESSAGE("===== support OpenGL =====")
	MESSAGE(STATUS "OpenGL library status:")
	MESSAGE(STATUS "	include path: ${OPENGL_INCLUDE_DIR}")
	MESSAGE(STATUS "	libraries: ${OPENGL_LIBRARIES}")
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
ELSE()
	MESSAGE("##### not support OpenGL #####")
ENDIF()

```



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
      5,最终安装这些文件:将 hello 二进制与 runhello.sh 安装

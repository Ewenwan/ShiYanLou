# CMake 学习笔记
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
      CMAKE 命令行 变量 cmake -DCMAKE_INSTALL_PREFIX=/usr .. 指定安装路径   默认定义是/usr/local
      cmake -DCMAKE_INSTALL_PREFIX=/usr ..
      make
      make install 
      
      cmake 指令 
      INSTALL(TARGETS myrun mylib mystaticlib  # 可执行文件myrun  动态库 mylib   静态库 mystaticlib 
      RUNTIME DESTINATION bin       # 可执行文件 安装路径 ${CMAKE_INSTALL_PREFIX}/bin
      LIBRARY DESTINATION lib       # 动态库文件 安装路径 ${CMAKE_INSTALL_PREFIX}/lib
      ARCHIVE DESTINATION libstatic # 静态库文件 安装路径 ${CMAKE_INSTALL_PREFIX}/libstatic
      )
      
      安装 头文件
      INSTALL(FILES hello.h DESTINATION include/hello) #头文件安装到 ${CMAKE_INSTALL_PREFIX}/include/hello

    
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
      
      SET(LIBHELLO_SRC hello.c)# 显示 指定变量
      ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})# SHARED 表示 创建共享库 对于libXXX.so  STATIC,静态库 对应 libXXX.a
      
      编译共享库 
      mkdir build 
      cd build
      cmake ..
      make
      
      在lib下 得到一个 共享库 libhello.so 
      
      静态库构建  同时生成 同名的 动态库和静态库
      向 lib/CMakeLists.txt 中添加:
      ADD_LIBRARY(hello_static STATIC ${LIBHELLO_SRC})
      SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")# 重命名
      SET_TARGET_PROPERTIES(hello PROPERTIES CLEAN_DIRECT_OUTPUT 1) # 动态库 不清除
      SET_TARGET_PROPERTIES(hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)#静态库 不清除
      
      动态库版本号
      SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)
      VERSION 指代动态库版本,SOVERSION 指代 API 版本。
      
      
# 使用 库文件 和头文件

      重复以前的步骤,建立 t4/src 目录,  mkdir -p t4/src
      编写源文件 main.c,内容如下:
      #include <hello.h>  //使用头文件
      int main()
      {
      HelloFunc();
      return 0;
      }
      
      编写工程主文件 CMakeLists.txt
      PROJECT(NEWHELLO)
      ADD_SUBDIRECTORY(src)
      
      编写 src/CMakeLists.txt
      ADD_EXECUTABLE(main main.c)
      
      在 src/CMakeLists.txt 中添加一个头文件搜索路径,方式很简单,加入:
      INCLUDE_DIRECTORIES(/usr/include/hello)#头文件 直接指定了地址   LINK_DIRECTORIES 指令加入非标准的库文件搜索路径
      TARGET_LINK_LIBRARIES(main hello)#链接动态库    TARGET_LINK_LIBRARIES(main libhello.a) 链接 静态库


      为了将程序更智能一点,我们可以使用 CMAKE_INCLUDE_PATH 来进行,使用 bash 的方法
      如下:
      
      设置环境变量
      export CMAKE_INCLUDE_PATH=/usr/include/hello 
      
      然后在头文件中将 INCLUDE_DIRECTORIES(/usr/include/hello)替换为:
      
      FIND_PATH(myHeader hello.h)#找头文件
      IF(myHeader)
      INCLUDE_DIRECTORIES(${myHeader})#包含头文件
      ENDIF(myHeader)


# cmake 常用变量和常用环境变量

    一,cmake 变量引用的方式:
      前面我们已经提到了,使用${}进行变量的引用。在 IF 等语句中,是直接使用变量名而不通过${}取值
      
    二,cmake 自定义变量的方式:
      主要有隐式定义和显式定义两种,前面举了一个隐式定义的例子,就是 PROJECT 指令,他会隐式的定义
      <projectname>_BINARY_DIR  和  <projectname>_SOURCE_DIR 两个变量。
      
      显式定义的例子我们前面也提到了,使用 SET 指令,就可以构建一个自定义变量了。
      比如:
      SET(HELLO_SRC main.SOURCE_PATHc)  就 PROJECT_BINARY_DIR 可以通过
      ${HELLO_SRC}来引用这个自定义变量了.
      
    三,cmake 常用变量:
     1,CMAKE_BINARY_DIR
        PROJECT_BINARY_DIR
        <projectname>_BINARY_DIR
      这三个变量指代的内容是一致的,如果是 in source 编译,指得就是工程顶层目录,如果
      是 out-of-source 外部编译  编译,指的是工程编译发生的目录。PROJECT_BINARY_DIR 跟其他
      指令稍有区别,现在,你可以理解为他们是一致的。
      
     2,CMAKE_SOURCE_DIR
       PROJECT_SOURCE_DIR
       <projectname>_SOURCE_DIR
      这三个变量指代的内容是一致的,不论采用何种编译方式,都是工程顶层目录。
      也就是在 in source 编译时,他跟 CMAKE_BINARY_DIR 等变量一致。
      PROJECT_SOURCE_DIR 跟其他指令稍有区别,现在,你可以理解为他们是一致的。 
      
     3,CMAKE_CURRENT_SOURCE_DIR
      指的是当前处理的 CMakeLists.txt 所在的路径,比如上面我们提到的 src 子目录。
      
    4,CMAKE_CURRRENT_BINARY_DIR
      如果是 in-source 编译,它跟 CMAKE_CURRENT_SOURCE_DIR 一致,如果是 out-of-
      source 编译,他指的是 target 编译目录。
      使用我们上面提到的 ADD_SUBDIRECTORY(src bin)可以更改这个变量的值。
      使用 SET(EXECUTABLE_OUTPUT_PATH <新路径>)并不会对这个变量造成影响,它仅仅
      修改了最终目标文件存放的路径。
      
    5,CMAKE_CURRENT_LIST_FILE
      输出调用这个变量的 CMakeLists.txt 的完整路径
      
    6,CMAKE_CURRENT_LIST_LINE
      输出这个变量所在的行

    7,CMAKE_MODULE_PATH
      这个变量用来定义自己的 cmake 模块所在的路径。如果你的工程比较复杂,有可能会自己
      编写一些 cmake 模块,这些 cmake 模块是随你的工程发布的,为了让 cmake 在处理
      CMakeLists.txt 时找到这些模块,你需要通过 SET 指令,将自己的 cmake 模块路径设
      置一下。
      比如
      SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
      这时候你就可以通过 INCLUDE 指令来调用自己的模块了。
      
    8,EXECUTABLE_OUTPUT_PATH 和 LIBRARY_OUTPUT_PATH
      分别用来重新定义最终结果 可执行文件、库文件 的存放目录,前面我们已经提到了这两个变量。
      
    9,PROJECT_NAME
      返回通过 PROJECT 指令定义的项目名称。
      
      
    四,cmake 调用环境变量的方式
      使用$ENV{NAME}指令就可以调用系统的环境变量了。
      比如
      MESSAGE(STATUS “HOME dir: $ENV{HOME}”)
      设置环境变量的方式是:
      SET(ENV{变量名} 值)
      
     1,CMAKE_INCLUDE_CURRENT_DIR
      自动添加 CMAKE_CURRENT_BINARY_DIR 和 CMAKE_CURRENT_SOURCE_DIR 到当前处理
      的 CMakeLists.txt。相当于在每个 CMakeLists.txt 加入:
      INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR})
      
     2,CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE
      将工程提供的头文件目录始终至于系统头文件目录的前面,当你定义的头文件确实跟系统发 生冲突时可以提供一些帮助。
      
     3,CMAKE_INCLUDE_PATH 和 CMAKE_LIBRARY_PATH 上面已经提及。  
      
    五,系统信息
      1,CMAKE_MAJOR_VERSION,CMAKE 主版本号,比如 2.4.6 中的 2
      2,CMAKE_MINOR_VERSION,CMAKE 次版本号,比如 2.4.6 中的 4
      3,CMAKE_PATCH_VERSION,CMAKE 补丁等级,比如 2.4.6 中的 6
      4,CMAKE_SYSTEM,系统名称,比如 Linux-2.6.22
      5,CMAKE_SYSTEM_NAME,不包含版本的系统名,比如 Linux
      6,CMAKE_SYSTEM_VERSION,系统版本,比如 2.6.22
      7,CMAKE_SYSTEM_PROCESSOR,处理器名称,比如 i686.
      8,UNIX,在所有的类 UNIX 平台为 TRUE,包括 OS X 和 cygwin   
      9,WIN32,在所有的 win32 平台为 TRUE,包括 cygwin
      
    六,主要的开关选项:
      1,CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS,用来控制 IF ELSE 语句的书写方式,在
      下一节语法部分会讲到。
      2,BUILD_SHARED_LIBS
      这个开关用来控制默认的库编译方式,如果不进行设置,使用 ADD_LIBRARY 并没有指定库
      类型的情况下,默认编译生成的库都是静态库。
      如果 SET(BUILD_SHARED_LIBS ON)后,默认生成的为动态库。
      3,CMAKE_C_FLAGS
      设置 C 编译选项,也可以通过指令 ADD_DEFINITIONS()添加。
      4,CMAKE_CXX_FLAGS
      设置 C++编译选项,也可以通过指令 ADD_DEFINITIONS()添加。




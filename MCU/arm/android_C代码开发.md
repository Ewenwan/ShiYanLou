# android_C代码开发

使用NDK编译一个用C写的小demo，放在Android平台上通过命令行调用。

**（如果要生成可以通过Java代码直接调用的so库，c代码的入口不能是main()，需要按包名改，
如:JNIEXPORT jint JNICALL Java_com_example_jnitest_MainActivity_main() )**


步骤：

          配置 NDK 编译环境
          配置 adb android调试环境
          测试
          打开Android虚拟机或者连接上开发板
          用adb push把helloworld传到Android中；
          用adb shell进入Android的shell；
          找到刚刚传的helloworld，执行#./helloworld就可以看到输出啦！

[Android.mk 解读](https://blog.csdn.net/zhonglunshun/article/details/78247342)

[自己动手编译C程序并运行于Android系统](https://blog.csdn.net/zhonglunshun/article/details/82850993)

[android studio ndk-build 编译C生成.so文件（ndk基础篇）](https://blog.csdn.net/cheng2290/article/details/77717164)

> **明确一些理论知识：**

1. Android是个基于Linux的操作系统，所以可以把它当作一个Linux(这句话我不知道说的是否过于绝对，若有错误，希望指正)；
2. 如果需要程序不执行在虚拟机上，而是执行在Linux操作系统里，那么这个程序的就必须是由一个“针对‘该Linux所执行的’特定硬件平台的”编译器编译得到的。例如，我们普通发行版中的gcc就是针对你的pc机的编译的，这个可执行程序放到有着同样硬件平台上也是可以用的。但是如果放在类似arm的嵌入式平台上，显然是不能执行的(因为arm和你pc的指令集都不一样)。如果你想用同一份源代码编译出arm上可以运行的程序，就要用针对arm的编译器（例如linux-arm-gcc）来编译。这就是所谓的交叉编译。学过嵌入式开发的同学一定懂得。
3. NDK的本质是什么？如果你用编辑器打开ndk-build，就会惊奇的发现，它不是二进制代码，而是个shell脚本，并且很简单，最后会调用本机的make。ndk-build的工作就是：解释jni/Android.mk文件里的语法，把它转化成类似于“linux-arm-gcc xxx.c -shared -o -Ixx -Lxx libxxx.so”。所以，我理解的NDK的本质类似make，解读类似Makefile的Android.mk。可惜的是，NDK做的包装让我们只可以编译出lib（它有连个选项）。
4. 既然ndk-build只是make而不是编译器，那么真正的编译器一定也在NDK包里面。我们就可以利用这些交叉编译的工具链来进行编译了。
5. 重新看题目，“NDK编译Android字符界面的可执行程序”，我们要做的其实不是用NDK来编译，而是用NDK中的交叉编译的工具链来编译，编译出来的程序也不是运行在什么“Android字符界面”中的，确切地说，是运行在“‘Andorid执行的硬件’上的Linux”上的。

**NDK可以生成lib,让java程序通过jni来调用,其实,NDK也可以生成C/C++的可执行程序.不过这个程序要被执行的话还有要求：**

1.可执行文件的名字必须是lib*.so. 否则apk安装时不会安装上去,因为目前apk的安装只支持安装lib文件,即lib*.so文件,如果不是此文件格式的,安装时不会拷到lib目录里.也可以考虑把可执行文件放assets里,java程序运行后把它拷贝到其它目录或系统目录.
2.这个文件的执行必须由java程序通过Runtime.getRuntime().exec()来执行.

**注意事项**

1,在项目根目录下建立文件夹libs/armeabi文件夹。最好文件夹名是armeabi，换了别的名字的话，试了一下，装上之后在data/data/程序/lib下是没有的

 2,将so库放入libs/armeabi文件夹注意事项： 1,如果采用静态注册的方式请注意C文件中严格按照命名规则Java_packageName_className_method()的方式命名 2,在Android项目中建立同上述命名规则中packageName中相同的包名,在此包名下建立同上述命名规则中className相同的类名 3,在className声明native方法 4,程序中加载so库System.loadLibrary(data/data/xxx.xxx.xxx/lib/xx.so)或者System.loadLibrary(xx)。这里的xx一定是方法名，即method（），不是类名或者文件名。




## NDK 使用
> 下载

[官方下载地址](https://developer.android.google.cn/ndk/downloads/)

win7下 android-ndk-r13b-windows-x86_64，把android-ndk-r13b-windows-x86_64解压在目当中就可，不用安装

配置系统环境变量

D:\android-ndk\android-ndk-r13b\platforms-tools


> 配置


> 编译

[参考](https://www.jianshu.com/p/5cbddbdac211)

          ~/Desktop/test/
          └── jni/
              ├── Android.mk
              ├── Application.mk
              └── test.c
              
其中Android.mk和Application.mk 是辅助ndk-build进行编译的，test.c是我们的c测试用例

Application.mk：主要指定生成哪些cpu架构的动态库，这里只生成armeabi 和armeabi-v7a架构的库
Android.mk

具体Android.mk的更详细的配置自行Google

[Android.mk参考](https://blog.csdn.net/the_Sunshine_of_King/article/details/78405149)

```mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
# 生成的so库的名字为libdemo.so
LOCAL_MODULE    := demo
# 用于编译的c文件
LOCAL_SRC_FILES := test.c
# 生成库的类型，是动态库，静态库还是可执行文件，这里先来生成一个动态库
include $(BUILD_SHARED_LIBRARY)
# 下面这么写是生成可执行文件
#include $(BUILD_EXECUTABLE)
```

APP_ABI      := armeabi armeabi-v7a
APP_PLATFORM := android-14
ndk-build NDK_PROJECT_PATH=/path/to/project(XXXX/Android.mk)

> 3.test.c

很简单就是打印一句 hello world!
```c
#include <stdio.h>

int main(const int argc,const char* args[]){

const char* str  = "hello world!";
printf("%s\n",str);
return 0;

}
```

> 


## ADB 调试工具使用

[ADB工具 android usb驱动 等 ](http://adbshell.com/downloads)


上述得到的可执行文件在电脑上是没法执行的，只能放到手机里面执行，执行如下命令，我们把刚生成的文件放到手机上：

push 文件到手机：

$ adb push libs/armeabi-v7a/demo /data/local/tmp

为手机中的demo文件添加可执行权限(为了方便直接给予所有权限了)：

$ adb shell 'chmod 777 /data/local/tmp/demo'

这里有问题，手机需要root???

用手机环境运行demo这个文件：

$ adb shell '/data/local/tmp/demo'






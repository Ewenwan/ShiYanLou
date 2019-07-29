# android_C代码开发
[NDK版本 下载地址 ](https://blog.csdn.net/zmzsoftware/article/details/80350955)

[官方 NDK 入门指南](https://developer.android.google.cn/ndk/guides)

[Android NDK开发(一)C语言基础语法](https://blog.csdn.net/weixin_42580207/article/details/80901614)

[JNI开发系列①JNI概念及开发流程](https://www.jianshu.com/p/68bca86a84ce)

[NDK开发基础①使用Android Studio编写NDK](https://www.jianshu.com/p/f1b8b97d2ef8)

[C语言基础及指针 结构体与指针 结构体数组 结构体嵌套 结构体内函数指针 结构体动态内存分配 ](https://www.jianshu.com/p/36cc18151e87)

[C语言基础及指针⑨联合体与枚举](https://www.jianshu.com/p/0bd682066106)

[C语言基础及指针⑩预编译 宏函数](https://www.jianshu.com/p/569f968bcdce)

[linux安装android NDK](https://blog.csdn.net/zmlovelx/article/details/80262979)

[Windows 下编译ncnn的android端的库](http://www.voidcn.com/article/p-cyxkeoaj-bpp.html)

使用NDK(原生开发集：Native Development Kit)编译一个用C写的小demo，放在Android平台上通过命令行调用。

NDK是一个包含API、构建工具、交叉编译、调试器、文档示例等一系列的工具集，可以帮助开发者快速开发C（或C++）的动态库，并能自动将so和java应用一起打包成APK。

JNI：Java Native Interface 也就是java本地接口，它是一个协议，这个协议用来沟通java代码和本地代码(c/c++)。通过这个协议，Java类的某些方法可以使用原生实现，同时让它们可以像普通的Java方法一样被调用和使用，而原生方法也可以使用Java对象，调用和使用Java方法。也就是说，使用JNI这种协议可以实现：java代码调用c/c++代码，而c/c++代码也可以调用java代码。


为什么要使用NDK开发呢？

1.我们都知道，java是半解释型语言，很容易被反汇编后拿到源代码文件，在开发一些重要协议时，我们为了安全起见，使用C语言来编写这些重要的部分，来增大系统的安全性。
2.在一些复杂性的计算中，要求高性能的场景中，C/C++更加的有效率，代码也更便于复用。


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

```
Android adb命令

1. 查看设备
adb devices
这个命令是查看当前连接的设备, 连接到计算机的android设备或者模拟器将会列出显示

2. 安装软件
adb install
这个命令将指定的apk文件安装到设备上

3. 卸载软件
adb uninstall <软件名>
adb uninstall -k <软件名>
如果加 -k 参数,为卸载软件但是保留配置和缓存文件.

4. 登录设备shell
adb shell
adb shell
这个命令将登录设备的shell.
后面加将是直接运行设备命令, 相当于执行远程命令

5. 从电脑上发送文件到设备
adb push <本地路径> <远程路径>
用push命令可以把本机电脑上的文件或者文件夹复制到设备(手机)

6. 从设备上下载文件到电脑
adb pull <远程路径> <本地路径>
用pull命令可以把设备(手机)上的文件或者文件夹复制到本机电脑

7. 显示帮助信息
adb help
这个命令将显示帮助信息

8.删除文件和文件夹
tools>adb remount 
tools>adb shell 
#su 
#cd system/sd/data //进入系统内指定文件夹 
#ls //列表显示当前文件夹内容 
#rm -r xxx //删除名字为xxx的文件夹及其里面的所有文件 
#rm xxx //删除文件xxx 
#rmdir xxx //删除xxx的文件夹

9.使用mkdir命令新建文件夹。例如：mkdir images
到此，文件夹已经新建完成，到eclipse中的DDMS中查看，打开新建文件夹的位置，可以看到，新建的文件夹已经存在该目录中。

10.通过adb命令 发送坐标给模拟器

（1）在DDMS下通过设置经纬度并send，则在该模拟器中可以得到该经纬度，并且该模拟器下所有应用都以之为本机的经纬度 
（2）有些 情况下不支持在DDMS中设置经纬度，因此需要通过cmd下telnet localhost 5444来进入5444这个模拟器（这个数字可能会变化）——注意在win7下telnet服务是没有打开的，于是要进入控制面板/程序/打开或关闭 windows功能/勾选telnet服务端和客户端来启动telnet服务。然后再使用geo fix 经度 纬度的形式设置经纬度达到同样效果。
```

## NDK编程的一个坑—Arm平台下的类型转换

最近在做DNN定点化相关的工作，DNN定点化就是把float表示的模型压缩成char表示，虽然会损失精度，但是由于DNN训练的模型值比较接近且范围较小，实际上带来的性能损失非常小。DNN定点化的好处是可以以4倍的效率压缩模型，这个在移动端会具有比较大的优势。 
做完定点化之后，在x86服务器上验证没有问题，但是利用NDK移植到arm移动端却一直得不到正确结果，真是一时头大。通过仔细调试，最终发现问题所在—所有值为负的float值定点化成char之后都变为0！在网上搜了很久终于知道这个居然是arm平台本身的问题。Arm平台下，char默认其实是unsigned char，正是由于char是无符号的，导致一个不在范围内的数被强转时会产生未定义的行为，在Stack Overflow上有一个很好的解释。 
找到问题所在，解决方法就非常简单，强制让arm平台下的char是有符号的，具体就是在编译的时候加-fsigned-char选项。


## ARM 优化

[利用ARM NEON intrinsic优化常用数学运算 向量点积 + 指数运算](https://blog.csdn.net/yutianzuijin/article/details/79944292)


# android_C代码开发

使用NDK编译一个用C写的小demo，放在Android平台上通过命令行调用。

（如果要生成可以通过Java代码直接调用的so库，c代码的入口不能是main()，需要按包名改，

如:JNIEXPORT jint JNICALL Java_com_example_jnitest_MainActivity_main() )

          配置 NDK 编译环境
          配置 adb android调试环境
          测试
          打开Android虚拟机或者连接上开发板
          用adb push把helloworld传到Android中；
          用adb shell进入Android的shell；
          找到刚刚传的helloworld，执行#./helloworld就可以看到输出啦！

明确一些理论知识：

1. Android是个基于Linux的操作系统，所以可以把它当作一个Linux(这句话我不知道说的是否过于绝对，若有错误，希望指正)；
2. 如果需要程序不执行在虚拟机上，而是执行在Linux操作系统里，那么这个程序的就必须是由一个“针对‘该Linux所执行的’特定硬件平台的”编译器编译得到的。例如，我们普通发行版中的gcc就是针对你的pc机的编译的，这个可执行程序放到有着同样硬件平台上也是可以用的。但是如果放在类似arm的嵌入式平台上，显然是不能执行的(因为arm和你pc的指令集都不一样)。如果你想用同一份源代码编译出arm上可以运行的程序，就要用针对arm的编译器（例如linux-arm-gcc）来编译。这就是所谓的交叉编译。学过嵌入式开发的同学一定懂得。
3. NDK的本质是什么？如果你用编辑器打开ndk-build，就会惊奇的发现，它不是二进制代码，而是个shell脚本，并且很简单，最后会调用本机的make。ndk-build的工作就是：解释jni/Android.mk文件里的语法，把它转化成类似于“linux-arm-gcc xxx.c -shared -o -Ixx -Lxx libxxx.so”。所以，我理解的NDK的本质类似make，解读类似Makefile的Android.mk。可惜的是，NDK做的包装让我们只可以编译出lib（它有连个选项）。
4. 既然ndk-build只是make而不是编译器，那么真正的编译器一定也在NDK包里面。我们就可以利用这些交叉编译的工具链来进行编译了。
5. 重新看题目，“NDK编译Android字符界面的可执行程序”，我们要做的其实不是用NDK来编译，而是用NDK中的交叉编译的工具链来编译，编译出来的程序也不是运行在什么“Android字符界面”中的，确切地说，是运行在“‘Andorid执行的硬件’上的Linux”上的。

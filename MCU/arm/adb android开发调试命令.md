# android 系统 adb 调试

[android_C代码开发](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/arm/android_C代码开发.md)

[Android Skia2D引擎库 深度优化的算法、完善的渲染体系和精炼的代码框架](https://blog.csdn.net/jxt1234and2010/article/list/3?)

[Android图形显示系统](https://blog.csdn.net/jxt1234and2010/article/details/44503019)

[AndroidlibJpeg库解码OpenCL优化](https://blog.csdn.net/jxt1234and2010/article/details/45200441)

[adb 等工具](https://blog.csdn.net/wbdwsqwwn/article/details/25201779)

# adb执行多条连续cmd命令

```sh

#echo root > temp.txt
adb -s $adb_device_id root

echo mkdir -p $device_mount_dir >> temp.sh
echo busybox umount $device_mount_dir >> temp.sh
echo busybox mount -t nfs $serv_mount_dir $device_mount_dir -o nolock >> temp.sh
echo cd $device_mount_dir/$demo_dir >> temp.sh
echo chmod 777 ./run.sh >> temp.sh
echo ./run.sh >> temp.sh
adb -s $adb_device_id shell < temp.sh

echo $?

rm temp.txt


```

# android 挂载远程服务器
```sh
adb -s F8RZ6XQC6U shell  adb 登录设备之后：可挂载 远程服务器上的文件：

busybox mount -t nfs 远程服务器ip地址:/远程文件地址 /mnt/本地地址-o nolock
需要系统内核支持 nfs
```

# Android ADB linux命令集合

A.用adb局域网功能连接设备 

     1，先用usb连接运行adb命令，将连接方式改为tcpip 
          adb tcpip {port}     port为端口号
          adb connect xxx.xxx.xxx.xxx(设备ip):port(刚才设置的端口号) 
          
     3，正常运行adb命令 
     
  adb的工作方式比较特殊采用监听Socket TCP 5554等端口的方式
  
     2，拔掉usb线，运行adb命令连接设备 
     
     让IDE和Qemu通讯，默认情况下adb会daemon相关的网络端口，所以当我们运行Eclipse时adb进程就会自动运行。 ADB是一个 客户端-服务器端 程序, 其中客户端是你用来操作的电脑, 服务器端是android设备. 

B.adb shell的一些常见命令 

　　1.adb shell 

　　      a.通过上面的命令，就可以进入设备或模拟器的shell环境中，在这个Linux Shell中，你可以执行各种Linux 的命令,如果只想执行一条命令，可以输入adb shell cmd 
　　         eg: adb shell dmesg会打印出内核的调试信息  
           
adb shell logcat v会打印出log信息 

　　   b.adb shell ls列出设备的目录列表 
     
    eg: adb连接设备操作 
    
    adb shell 
    adb -s xxxx shell 

　　2.上传文件: adb push 
  
　　  下载文件: adb pull 
    
　　   /tmp/...指的是在设备linux环境中要操作文件的路径 
     
　　     eg: adb push key data/app 就是将key文件上传到用户目录中 
       
  a.将文件放入设备 
  
         eg: adb push xxx.* /directory 
     adb -s xxxx(设备编号) xxx.* /directory 
     
     b.将文件拉出设备 
     
        eg: adb pull xxx.* /directory  
        
# android 开发


linux adb 设被配置:

            新建文件 /etc/udev/rules.d/80-android.rules  写入 设备usb编号(lusb查看得到)

            增加内容:

            SUBSYSTEM=="usb", ATTR{idVendor}=="4255", MODE="0666", GROUP="plugdev"

            将设备usb编号替换到 ATTR{idVendor}=="4255"中，其他不用修改。

            保存，sudo chmod 777 80-android.rules

            执行以下命令：

            sudo usermod -a -G plugdev $(id -u -n)

            sudo udevadm control --reload-rules

            sudo service udev restart

            sudo udevadm trigger

            第四步： 重启

            adb adb kill-server
            adb start-server 
            adb devices
            
            adb -s 序列号 shell 登录指定设备
             
                adb -s xxxx(设备编号) xxx.* /directory 

　　3.安装程序: adb install <*.apk> 
  
　　  卸载软件: adb unistall apk(注意卸载的时候和安装的时候的文件名是不一样的，例如安装的时候adb shell GPSStatus2b2.apk,这个apk文件就被安装在data/app目录下，但是使用uninstall的时候，首先要到、data/app目录下查看安装的apk文件在linux目录下的文件名，发现是com.eclipsim.gpsstatus.apk，使用adb uninstall com.eclipsim.gpsstatus.注意不要加apk后缀。返回success结果证明文件卸载成功)。 
    
  a.用adb安装apk 
  
            eg: adb install xxx.apk 
                adb install -s xxxx(设备编号) xxx.apk     多个设备 
                
          b.用adb卸载apk 
          
         eg: adb uinstall xxx.apk(通常要写明详细的包名和activity名) 
         
     adb uinstall -s xxxx(设备编号) xxx.apk     多个设备 
     
　　补充一点，通过adb安装的软件(*.apk)都在"/data/app/"目录下，所以安装时不必制定路径。 
  
　　卸载的时候当然也可以直接到目录下使用rm命令也可。 

          如果有多个设备在运行的话，发送命令时必须用上-s,-e或-d这几个参数指定目标设备。 
                adb -e  发送命令到模拟器。 
                adb -d  发送命令到到USB设备，比如手机。 
                adb -s  指定一个目标。adb -s <serialNumber> <command>install <path-to-apk> 
          例如：adb -s emulator-5554 install helloWorld.apk 

　　4.显示android模拟器状态:


　　 adb devices 列出所有连接的设备 
   
          例如： 
                ~$ sudo /opt/android/android-sdk/tools/adb devices 
                List of devices       attached 
                emulator-5554      device 
                HT95LKF00945    device 
                这里就列出了两个设备，第一个是模拟器，第二个是手机。 
          注意：这里是用root用户来启动adb服务器和执行adb命令，不然就会 出现“no permissions” 
          
　　 adb get-serialno 打印设备序列号 
   
　 adb version 列出ADB的版本号 
  
　　 adb get-state 打印出的结果一般是offline | bootloader | device 
   
                adb help  查看adb所支持的所有命令 
                adb version    查看adb的版本序列号 
                adb logcat  打印日志到屏幕 
                adb bugreport 打印dumpsys,dumpstate和logcat数据到屏幕 
                adb jdwp       查看指定的设施的可用的JDWP信息. 
                adb forward    forward <local> <remote> 
                adb get-serialno 查看adb实例的序列号. 
                adb get-state 查看模拟器/设施的当前状态. 
                adb ppp 通过use设备运行PPP 
                adb wait-for-device  如果设备不联机就不让执行。 

　　5.等待正在运行的设备: adb wait-for-device 

　　6.adb start-server 

　　 adb kill-server 一般在键入adb shell命令后显示device offline或者是显示有多个设备的情况下使用 

 　　7.adb remount 重新挂载系统分区，就是将系统分区重新挂载为可写。 

　　8. adb root使用管理员权限 

　　9. adb bugreport打印除所有的bug信息 

　　10.adb shell logcat -b radio 记录无线通讯日志：一般来说，无线通讯的日志非常多，在运行时没必要去记录，但我们还是可以通过命令，设置记录： 

　　11.adb emu 

　　12.端口转发: adb forward adb forward tcp:5555 tcp:1234 

　　(将默认端口TCP 5555转发到1234端口上) 

# adb  app 相关操作

1启动命令很简单：

adb shell am start -n package/launch activity
2例如以下命令可以打开模拟器上的设置app。

adb shell am start -n com.android.settings/com.android.settings.Settings

3. 所以命令的关键是要获取package和launch activity。有多种方法可以获得。

第一种，有代码的，打开AndroidManifest.xml，“package=”之后的就是命令需要的package，“android.intent.action.MAIN” action 所在的activity就是命令需要的launch activity，也就是图中蓝线的部分。这种方法对所有的APP都适用。

第二种，打开App，在cmd窗口中输入以下命令。
adb shell dumpsys window windows | grep Current

如下图所示，/之前的就是package，之后的就是当前的activity，如下图红线的部分。对于大多数的App，这个当前的activity就是am命令需要的launch activity， 但也有些应用启动时包含了欢迎页之类的，在你打开App的时候就自动切换了activity，那用这种方法就获取不了需要的package。


adb命令查看apk信息, adb启动你的apk 

1.用adb获得手机里面某个apk的应用信息、版本信息

　　adb shell dumpsys package com.sy.a268
  
2.列出所有

　　adb shell dumpsys 
  
3.用adb启动apk

　　adb shell am start -n com.sy.a268/com.sy.a268.MainActivity
  
  
4.随机产生事件给某个应用程序, 随机产生500个事件给程序。你会发现你的程序不断的被点击，旋转！  随机测试

　　adb  shell  monkey -p  com.sy.a268  -v 500
  
  
  
  
# android 开机启动脚本

          android 开机启动脚本/init.rc是在ramdisk.img中的，每次开机启动会解出来。
          所以直接修改/init.rc是行不通的，修改后，重启就恢复了。
          但是/init.rc里面调用了/system/etc/install-recovery.sh （其实是 /system/bin/install-recovery.sh  可查看 /init.rc）
          可以修改/system/etc/install-recovery.sh，来执行启动脚本
          下面脚本自动启动wifi adb和vncserver
          #!/system/bin/sh
          setprop service.adb.tcp.port 5555
          stop adbd
          start adbd
          /data/androidvncserver &

androidvncserver来自https://code.google.com/p/android-vnc-server/


TCP/IP方式：

     setprop service.adb.tcp.port 5555
     stop adbd
     start adbd

usb方式：

     setprop service.adb.tcp.port -1
     stop adbd
     start adbd









# adb 命令大全

# adb命令大全

[参考](http://www.mamicode.com/info-detail-2175361.html)

基本用法
命令语法
adb 命令的基本语法如下：

adb [-d|-e|-s ] 

如果只有一个设备/模拟器连接时，可以省略掉 [-d|-e|-s ] 这一部分，直接使用 adb 。

为命令指定目标设备

如果有多个设备/模拟器连接，则需要为命令指定目标设备。

参数 含义
-d 指定当前唯一通过 USB 连接的 Android 设备为命令目标
-e 指定当前唯一运行的模拟器为命令目标
-s 指定相应 serialNumber 号的设备/模拟器为命令目标
在多个设备/模拟器连接的情况下较常用的是 -s 参数，serialNumber 可以通过 adb devices 命令获取。如：

$ adb devicesList of devices attachedcf264b8f    deviceemulator-5554   device

输出里的 cf264b8f 和 emulator-5554 即为 serialNumber。比如这时想指定 cf264b8f 这个设备来运行 adb 命令获取屏幕分辨率：

adb -s cf264b8f shell wm size

遇到多设备/模拟器的情况均使用这几个参数为命令指定目标设备，下文中为简化描述，不再重复。

启动/停止

启动 adb server 命令：

adb start-server

（一般无需手动执行此命令，在运行 adb 命令时若发现 adb server 没有启动会自动调起。）

停止 adb server 命令：

adb kill-server

查看 adb 版本

命令：

adb version

示例输出：

Android Debug Bridge version 1.0.32Revision 09a0d98bebce-android

以 root 权限运行 adbd

adb 的运行原理是 PC 端的 adb server 与手机端的守护进程 adbd 建立连接，然后 PC 端的 adb client 通过 adb server 转发命令，adbd 接收命令后解析运行。

所以如果 adbd 以普通权限执行，有些需要 root 权限才能执行的命令无法直接用 adb xxx 执行。这时可以 adb shell 然后 su 后执行命令，也可以让 adbd 以 root 权限执行，这个就能随意执行高权限命令了。

命令：

adb root

正常输出：

restarting adbd as root

现在再运行 adb shell，看看命令行提示符是不是变成 # 了？

有些手机 root 后也无法通过 adb root 命令让 adbd 以 root 权限执行，比如三星的部分机型，会提示 adbd cannot run as root in production builds，此时可以先安装 adbd Insecure，然后 adb root 试试。

相应地，如果要恢复 adbd 为非 root 权限的话，可以使用 adb unroot 命令。

指定 adb server 的网络端口

命令：

adb -P  start-server

默认端口为 5037。

设备连接管理

查询已连接设备/模拟器

命令：

adb devices

输出示例：

List of devices attachedcf264b8f    deviceemulator-5554   device

输出格式为 [serialNumber] [state]，serialNumber 即我们常说的 SN，state 有如下几种：

    offline —— 表示设备未连接成功或无响应。
    device —— 设备已连接。注意这个状态并不能标识 Android 系统已经完全启动和可操作，在设备启动过程中设备实例就可连接到 adb，但启动完毕后系统才处于可操作状态。
    no device —— 没有设备/模拟器连接。
    以上输出显示当前已经连接了两台设备/模拟器，cf264b8f 与 emulator-5554 分别是它们的 SN。从 emulator-5554 这个名字可以看出它是一个 Android 模拟器


常见异常输出：

    没有设备/模拟器连接成功。
    List of devices attached
    设备/模拟器未连接到 adb 或无响应。
    List of devices attachedcf264b8f offline

USB 连接

通过 USB 连接来正常使用 adb 需要保证几点：

     硬件状态正常。
     包括 Android 设备处于正常开机状态，USB 连接线和各种接口完好。
     Android 设备的开发者选项和 USB 调试模式已开启。
     可以到「设置」-「开发者选项」-「Android 调试」查看。
     如果在设置里找不到开发者选项，那需要通过一个彩蛋来让它显示出来：在「设置」-「关于手机」连续点击「版本号」7 次。
     设备驱动状态正常。

这一点貌似在 Linux 和 Mac OS X 下不用操心，在 Windows 下有可能遇到需要安装驱动的情况，确认这一点可以右键「计算机」-「属性」，到「设备管理器」里查看相关设备上是否有黄色感叹号或问号，如果没有就说明驱动状态已经好了。否则可以下载一个手机助手类程序来安装驱动先。

通过 USB 线连接好电脑和设备后确认状态。

adb devices
如果能看到

xxxxxx device
说明连接成功。

* 无线连接

除了可以通过 USB 连接设备与电脑来使用 adb，也可以通过无线连接——虽然连接过程中也有需要使用 USB 的步骤，但是连接成功之后你的设备就可以在一定范围内摆脱 USB 连接线的限制啦！

操作步骤：

将 Android 设备与将运行 adb 的电脑连接到同一个局域网，比如连到同一个 WiFi。

将设备与电脑通过 USB 线连接。

应确保连接成功（可运行 adb devices 看是否能列出该设备）。

让设备在 5555 端口监听 TCP/IP 连接：

adb tcpip 5555

断开 USB 连接。

找到设备的 IP 地址。

一般能在「设置」-「关于手机」-「状态信息」-「IP地址」找到。

通过 IP 地址连接设备。

adb connect 

这里的 就是上一步中找到的设备 IP 地址。

确认连接状态。

adb devices

如果能看到 :5555 device 说明连接成功。
如果连接不了，请确认 Android 设备与电脑是连接到了同一个 WiFi，然后再次执行 adb connect 那一步；
如果还是不行的话，通过 adb kill-server 重新启动 adb 然后从头再来一次试试。

断开无线连接
命令：
adb disconnect 
应用管理
查看应用列表
查看应用列表的基本命令格式是
adb shell pm list packages [-f] [-d] [-e] [-s] [-3] [-i] [-u] [--user USER_ID] [FILTER]
即在 adb shell pm list packages 的基础上可以加一些参数进行过滤查看不同的列表，支持的过滤参数如下：
参数
显示列表
无
所有应用

    -f  显示应用关联的 apk 文件
    -d  只显示 disabled 的应用
    -e  只显示 enabled 的应用
    -s  只显示系统应用
    -3  只显示第三方应用
    -i  显示应用的 installer
    -u  包含已卸载应用

包名包含 字符串
所有应用

命令：
adb shell pm list packages

输出示例：

package:com.android.smoketestpackage:com.example.android.livecubespackage:com.android.providers.telephonypackage:com.google.android.googlequicksearchboxpackage:com.android.providers.calendarpackage:com.android.providers.mediapackage:com.android.protipspackage:com.android.documentsuipackage:com.android.gallerypackage:com.android.externalstorage...// other packages here...

系统应用

命令：
adb shell pm list packages -s
第三方应用
命令：
adb shell pm list packages -3
包名包含某字符串的应用
比如要查看包名包含字符串 mazhuang 的应用列表，命令：
adb shell pm list packages mazhuang
当然也可以使用 grep 来过滤：
adb shell pm list packages | grep mazhuang
安装 APK
命令：
adb install 
参数：
adb install 后面可以跟一些参数来控制安装 APK 的行为，常用参数及含义如下：
参数
含义
-r
允许覆盖安装。
-s
将应用安装到 sdcard。
-d
允许降级覆盖安装。
完整参数列表及含义可以直接运行 adb 命令然后查看 adb install [-lrtsdg] 一节。
如果见到类似如下输出（状态为 Success）代表安装成功：
12040 KB/s (22205609 bytes in 1.801s)        pkg: /data/local/tmp/SogouInput_android_v8.3_sweb.apkSuccess
而如果状态为 Failure 则表示安装失败。常见安装失败输出代码、含义及可能的解决办法如下：
输出
含义
解决办法
INSTALL_FAILED_ALREADY_EXISTS
应用已经存在
使用 -r 参数
INSTALL_FAILED_INVALID_APK
无效的 APK 文件

INSTALL_FAILED_INVALID_URI
无效的 APK 文件名
确保 APK 文件名里无中文
INSTALL_FAILED_INSUFFICIENT_STORAGE
空间不足
清理空间
INSTALL_FAILED_DUPLICATE_PACKAGE
已经存在同名程序

INSTALL_FAILED_NO_SHARED_USER
请求的共享用户不存在

INSTALL_FAILED_UPDATE_INCOMPATIBLE
已经安装过签名不一样的同名应用，且数据没有移除

INSTALL_FAILED_SHARED_USER_INCOMPATIBLE
请求的共享用户存在但签名不一致

INSTALL_FAILED_MISSING_SHARED_LIBRARY
安装包使用了设备上不可用的共享库

INSTALL_FAILED_REPLACE_COULDNT_DELETE
替换时无法删除

INSTALL_FAILED_DEXOPT
dex 优化验证失败或空间不足

INSTALL_FAILED_OLDER_SDK
设备系统版本低于应用要求

INSTALL_FAILED_CONFLICTING_PROVIDER
设备里已经存在与应用里同名的 content provider

INSTALL_FAILED_NEWER_SDK
设备系统版本高于应用要求

INSTALL_FAILED_TEST_ONLY
应用是 test-only 的，但安装时没有指定 -t 参数

INSTALL_FAILED_CPU_ABI_INCOMPATIBLE
包含不兼容设备 CPU 应用程序二进制接口的 native code

INSTALL_FAILED_MISSING_FEATURE
应用使用了设备不可用的功能

INSTALL_FAILED_CONTAINER_ERROR
sdcard 访问失败
确认 sdcard 可用，或者安装到内置存储
INSTALL_FAILED_INVALID_INSTALL_LOCATION
不能安装到指定位置
切换安装位置，添加或删除 -s 参数
INSTALL_FAILED_MEDIA_UNAVAILABLE
安装位置不可用
一般为 sdcard，确认 sdcard 可用或安装到内置存储
INSTALL_FAILED_VERIFICATION_TIMEOUT
验证安装包超时

INSTALL_FAILED_VERIFICATION_FAILURE
验证安装包失败

INSTALL_FAILED_PACKAGE_CHANGED
应用与调用程序期望的不一致

INSTALL_FAILED_UID_CHANGED
以前安装过该应用，与本次分配的 UID 不一致
清除以前安装过的残留文件
INSTALL_FAILED_VERSION_DOWNGRADE
已经安装了该应用更高版本
使用 -d 参数
INSTALL_FAILED_PERMISSION_MODEL_DOWNGRADE
已安装 target SDK 支持运行时权限的同名应用，要安装的版本不支持运行时权限

INSTALL_PARSE_FAILED_NOT_APK
指定路径不是文件，或不是以 .apk 结尾

INSTALL_PARSE_FAILED_BAD_MANIFEST
无法解析的 AndroidManifest.xml 文件

INSTALL_PARSE_FAILED_UNEXPECTED_EXCEPTION
解析器遇到异常

INSTALL_PARSE_FAILED_NO_CERTIFICATES
安装包没有签名

INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES
已安装该应用，且签名与 APK 文件不一致
先卸载设备上的该应用，再安装
INSTALL_PARSE_FAILED_CERTIFICATE_ENCODING
解析 APK 文件时遇到 CertificateEncodingException

INSTALL_PARSE_FAILED_BAD_PACKAGE_NAME
manifest 文件里没有或者使用了无效的包名

INSTALL_PARSE_FAILED_BAD_SHARED_USER_ID
manifest 文件里指定了无效的共享用户 ID

INSTALL_PARSE_FAILED_MANIFEST_MALFORMED
解析 manifest 文件时遇到结构性错误

INSTALL_PARSE_FAILED_MANIFEST_EMPTY
在 manifest 文件里找不到找可操作标签（instrumentation 或 application）

INSTALL_FAILED_INTERNAL_ERROR
因系统问题安装失败

INSTALL_FAILED_USER_RESTRICTED
用户被限制安装应用

INSTALL_FAILED_DUPLICATE_PERMISSION
应用尝试定义一个已经存在的权限名称

INSTALL_FAILED_NO_MATCHING_ABIS
应用包含设备的应用程序二进制接口不支持的 native code

INSTALL_CANCELED_BY_USER
应用安装需要在设备上确认，但未操作设备或点了取消
在设备上同意安装
INSTALL_FAILED_ACWF_INCOMPATIBLE
应用程序与设备不兼容

does not contain AndroidManifest.xml
无效的 APK 文件

is not a valid zip file
无效的 APK 文件

Offline
设备未连接成功
先将设备与 adb 连接成功
unauthorized
设备未授权允许调试

error: device not found
没有连接成功的设备
先将设备与 adb 连接成功
protocol failure
设备已断开连接
先将设备与 adb 连接成功
Unknown option: -s
Android 2.2 以下不支持安装到 sdcard
不使用 -s 参数
No space left on devicerm
空间不足
清理空间
Permission denied ... sdcard ...
sdcard 不可用


参考：PackageManager.java

卸载应用
命令：
adb uninstall [-k] 
表示应用的包名，-k 参数可选，表示卸载应用但保留数据和缓存目录。
命令示例：
adb uninstall com.qihoo360.mobilesafe
表示卸载 360 手机卫士。
清除应用数据与缓存
命令：
adb shell pm clear 
表示应用名包，这条命令的效果相当于在设置里的应用信息界面点击了「清除缓存」和「清除数据」。
命令示例：
adb shell pm clear com.qihoo360.mobilesafe
表示清除 360 手机卫士的数据和缓存。
查看前台 Activity
命令：
adb shell dumpsys activity activities | grep mFocusedActivity
输出示例：
mFocusedActivity: ActivityRecord{8079d7e u0 com.cyanogenmod.trebuchet/com.android.launcher3.Launcher t42}
其中的 com.cyanogenmod.trebuchet/com.android.launcher3.Launcher 就是当前处于前台的 Activity。
与应用交互
主要是使用 am 命令，常用的 如下：
command
用途
start [options] 
启动 指定的 Activity
startservice [options] 
启动 指定的 Service
broadcast [options] 
发送 指定的广播
force-stop 
停止 相关的进程
参数很灵活，和写 Android 程序时代码里的 Intent 相对应。
用于决定 intent 对象的选项如下：
参数
含义
-a 
指定 action，比如 android.intent.action.VIEW
-c 
指定 category，比如 android.intent.category.APP_CONTACTS
-n 
指定完整 component 名，用于明确指定启动哪个 Activity，如 com.example.app/.ExampleActivity
里还能带数据，就像写代码时的 Bundle 一样：
参数
含义
--esn 
null 值（只有 key 名）
-e|--es 
string 值
--ez 
boolean 值
--ei 
integer 值
--el 
long 值
--ef 
float 值
--eu 
URI
--ecn 
component name
--eia [,
integer 数组
--ela [,
long 数组
调起 Activity
命令格式：
adb shell am start [options] 
例如：
adb shell am start -n com.tencent.mm/.ui.LauncherUI
表示调起微信主界面。
adb shell am start -n org.mazhuang.boottimemeasure/.MainActivity --es "toast" "hello, world"
表示调起 org.mazhuang.boottimemeasure/.MainActivity 并传给它 string 数据键值对 toast - hello, world。
调起 Service
命令格式：
adb shell am startservice [options] 
例如：
adb shell am startservice -n com.tencent.mm/.plugin.accountsync.model.AccountAuthenticatorService
表示调起微信的某 Service。
发送广播
命令格式：
adb shell am broadcast [options] 
例如：
adb shell am broadcast -a android.intent.action.BOOT_COMPLETED -n org.mazhuang.boottimemeasure/.BootCompletedReceiver
表示向 org.mazhuang.boottimemeasure/.BootCompletedReceiver 发送一个 BOOT_COMPLETED 广播，这类用法在测试的时候很实用，比如某个广播的场景很难制造，可以考虑通过这种方式来发送广播。
强制停止应用
命令：
adb shell am force-stop 
命令示例：
adb shell am force-stop com.qihoo360.mobilesafe
表示停止 360 安全卫士的一切进程与服务。
文件管理
复制设备里的文件到电脑
命令：
adb pull <设备里的文件路径> [电脑上的目录] 
其中 电脑上的目录 参数可以省略，默认复制到当前目录。
例：
adb pull /sdcard/sr.mp4 ~/tmp/
小技巧：设备上的文件路径可能需要 root 权限才能访问，如果你的设备已经 root 过，可以先使用 adb shell 和 su 命令在 adb shell 里获取 root 权限后，先 cp /path/on/device /sdcard/filename 将文件复制到 sdcard，然后 adb pull /sdcard/filename /path/on/pc。
复制电脑里的文件到设备
命令：
adb push <电脑上的文件路径> <设备里的目录> 
例：
adb push ~/sr.mp4 /sdcard/
小技巧：设备上的文件路径普通权限可能无法直接写入，如果你的设备已经 root 过，可以先 adb push /path/on/pc /sdcard/filename，然后 adb shell 和 su 在 adb shell 里获取 root 权限后，cp /sdcard/filename /path/on/device。
模拟按键/输入
在 adb shell 里有个很实用的命令叫 input，通过它可以做一些有趣的事情。
input 命令的完整 help 信息如下：
Usage: input []  [...]The sources are:      mouse      keyboard      joystick      touchnavigation      touchpad      trackball      stylus      dpad      gesture      touchscreen      gamepadThe commands and default sources are:      text  (Default: touchscreen)      keyevent [--longpress]  ... (Default: keyboard)      tap   (Default: touchscreen)      swipe     [duration(ms)] (Default: touchscreen)      press (Default: trackball)      roll   (Default: trackball)
比如使用 adb shell input keyevent 命令，不同的 keycode 能实现不同的功能，完整的 keycode 列表详见 KeyEvent，摘引部分我觉得有意思的如下：
keycode
含义
3
HOME 键
4
返回键
5
打开拨号应用
6
挂断电话
24
增加音量
25
降低音量
26
电源键
27
拍照（需要在相机应用里）
64
打开浏览器
82
菜单键
85
播放/暂停
86
停止播放
87
播放下一首
88
播放上一首
122
移动光标到行首或列表顶部
123
移动光标到行末或列表底部
126
恢复播放
127
暂停播放
164
静音
176
打开系统设置
187
切换应用
207
打开联系人
208
打开日历
209
打开音乐
210
打开计算器
220
降低屏幕亮度
221
提高屏幕亮度
223
系统休眠
224
点亮屏幕
231
打开语音助手
276
如果没有 wakelock 则让系统休眠
下面是 input 命令的一些用法举例。
电源键
命令：
adb shell input keyevent 26
执行效果相当于按电源键。
菜单键
命令：
adb shell input keyevent 82
HOME 键
命令：
adb shell input keyevent 3
返回键
命令：
adb shell input keyevent 4
音量控制
增加音量：
adb shell input keyevent 24
降低音量：
adb shell input keyevent 25
静音：
adb shell input keyevent 164
媒体控制
播放/暂停：
adb shell input keyevent 85
停止播放：
adb shell input keyevent 86
播放下一首：
adb shell input keyevent 87
播放上一首：
adb shell input keyevent 88
恢复播放：
adb shell input keyevent 126
暂停播放：
adb shell input keyevent 127
点亮/熄灭屏幕
可以通过上文讲述过的模拟电源键来切换点亮和熄灭屏幕，但如果明确地想要点亮或者熄灭屏幕，那可以使用如下方法。
点亮屏幕：
adb shell input keyevent 224
熄灭屏幕：
adb shell input keyevent 223
滑动解锁
如果锁屏没有密码，是通过滑动手势解锁，那么可以通过 input swipe 来解锁。
命令（参数以机型 Nexus 5，向上滑动手势解锁举例）：
adb shell input swipe 300 1000 300 500
参数 300 1000 300 500 分别表示起始点x坐标 起始点y坐标 结束点x坐标 结束点y坐标。
输入文本
在焦点处于某文本框时，可以通过 input 命令来输入文本。
命令：
adb shell input text hello
现在 hello 出现在文本框了。
查看日志
Android 系统的日志分为两部分，底层的 Linux 内核日志输出到 /proc/kmsg，Android 的日志输出到 /dev/log。
Android 日志
命令格式：
[adb] logcat [
] ... [
常用用法列举如下：
按级别过滤日志
Android 的日志分为如下几个级别：
V —— Verbose（最低，输出得最多）
D —— Debug
I —— Info
W —— Warning
E —— Error
F —— Fatal
S —— Silent（最高，啥也不输出）
按某级别过滤日志则会将该级别及以上的日志输出。
比如，命令：
adb logcat *:W
会将 Warning、Error、Fatal 和 Silent 日志输出。
按 tag 和级别过滤日志
比如，命令：
adb logcat ActivityManager:I MyApp:D *:S
表示输出 tag ActivityManager 的 Info 以上级别日志，输出 tag MyApp 的 Debug 以上级别日志，及其它 tag 的 Silent 级别日志（即屏蔽其它 tag 日志）。
日志格式
可以用 adb logcat -v 选项指定日志输出格式。
日志支持按以下几种 ：
brief
默认格式。格式为：
/(): 
示例：
D/HeadsetStateMachine( 1785): Disconnected process message: 10, size: 0
process
格式为：
() 
示例：
D( 1785) Disconnected process message: 10, size: 0  (HeadsetStateMachine)
tag
格式为：
/: 
示例：
D/HeadsetStateMachine: Disconnected process message: 10, size: 0
raw
格式为：

示例：
Disconnected process message: 10, size: 0
time
格式为：
 /(): 
示例：
08-28 22:39:39.974 D/HeadsetStateMachine( 1785): Disconnected process message: 10, size: 0
threadtime
格式为：
    : 
示例：
08-28 22:39:39.974  1785  1832 D HeadsetStateMachine: Disconnected process message: 10, size: 0
long
格式为：
[  : / ]
示例：
[ 08-28 22:39:39.974  1785: 1832 D/HeadsetStateMachine ]Disconnected process message: 10, size: 0
指定格式可与上面的过滤同时使用。比如：
adb logcat -v long ActivityManager:I *:S
清空日志
adb logcat -c
内核日志
命令：
adb shell dmesg
输出示例：
<6>[14201.684016] PM: noirq resume of devices complete after 0.982 msecs<6>[14201.685525] PM: early resume of devices complete after 0.838 msecs<6>[14201.753642] PM: resume of devices complete after 68.106 msecs<4>[14201.755954] Restarting tasks ... done.<6>[14201.771229] PM: suspend exit 2016-08-28 13:31:32.679217193 UTC<6>[14201.872373] PM: suspend entry 2016-08-28 13:31:32.780363596 UTC<6>[14201.872498] PM: Syncing filesystems ... done.
中括号里的 [14201.684016] 代表内核开始启动后的时间，单位为秒。
通过内核日志我们可以做一些事情，比如衡量内核启动时间，在系统启动完毕后的内核日志里找到 Freeing init memory 那一行前面的时间就是。
查看设备信息
型号
命令：
adb shell getprop ro.product.model
输出示例：
Nexus 5
电池状况
命令：
adb shell dumpsys battery
输入示例：
Current Battery Service state:  AC powered: false  USB powered: true  Wireless powered: false  status: 2  health: 2  present: true  level: 44  scale: 100  voltage: 3872  temperature: 280  technology: Li-poly
其中 scale 代表最大电量，level 代表当前电量。上面的输出表示还剩下 44% 的电量。
屏幕分辨率
命令：
adb shell wm size
输出示例：
Physical size: 1080x1920
该设备屏幕分辨率为 1080px * 1920px。
屏幕密度
命令：
adb shell wm density
输出示例：
Physical density: 420
该设备屏幕密度为 420dpi。
显示屏参数
命令：
adb shell dumpsys window displays
输出示例：
WINDOW MANAGER DISPLAY CONTENTS (dumpsys window displays)  Display: mDisplayId=0    init=1080x1920 420dpi cur=1080x1920 app=1080x1794 rng=1080x1017-1810x1731    deferred=false layoutNeeded=false
其中 mDisplayId 为 显示屏编号，init 是初始分辨率和屏幕密度，app 的高度比 init 里的要小，表示屏幕底部有虚拟按键，高度为 1920 - 1794 = 126px 合 42dp。
android_id
命令：
adb shell settings get secure android_id
输出示例：
51b6be48bac8c569
IMEI
在 Android 4.4 及以下版本可通过如下命令获取 IMEI：
adb shell dumpsys iphonesubinfo
输出示例：
Phone Subscriber Info:  Phone Type = GSM  Device ID = 860955027785041
其中的 Device ID 就是 IMEI。
而在 Android 5.0 及以上版本里这个命令输出为空，得通过其它方式获取了（需要 root 权限）：
adb shellsuservice call iphonesubinfo 1
输出示例：
Result: Parcel(  0x00000000: 00000000 0000000f 00360038 00390030 ‘........8.6.0.9.‘  0x00000010: 00350035 00320030 00370037 00350038 ‘5.5.0.2.7.7.8.5.‘  0x00000020: 00340030 00000031                   ‘0.4.1...        ‘)
把里面的有效内容提取出来就是 IMEI 了，比如这里的是 860955027785041。
参考：adb shell dumpsys iphonesubinfo not working since Android 5.0 Lollipop
Android 系统版本
命令：
adb shell getprop ro.build.version.release
输出示例：
5.0.2
Mac 地址
命令：
adb shell cat /sys/class/net/wlan0/address
输出示例：
f8:a9:d0:17:42:4d
CPU 信息
命令：
adb shell cat /proc/cpuinfo
输出示例：
Processor       : ARMv7 Processor rev 0 (v7l)processor       : 0BogoMIPS        : 38.40processor       : 1BogoMIPS        : 38.40processor       : 2BogoMIPS        : 38.40processor       : 3BogoMIPS        : 38.40Features        : swp half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivtCPU implementer : 0x51CPU architecture: 7CPU variant     : 0x2CPU part        : 0x06fCPU revision    : 0Hardware        : Qualcomm MSM 8974 HAMMERHEAD (Flattened Device Tree)Revision        : 000bSerial          : 0000000000000000
这是 Nexus 5 的 CPU 信息，我们从输出里可以看到使用的硬件是 Qualcomm MSM 8974，processor 的编号是 0 到 3，所以它是四核的，采用的架构是 ARMv7 Processor rev 0 (v71)。
更多硬件与系统属性
设备的更多硬件与系统属性可以通过如下命令查看：
adb shell cat /system/build.prop
这会输出很多信息，包括前面几个小节提到的「型号」和「Android 系统版本」等。
输出里还包括一些其它有用的信息，它们也可通过 adb shell getprop <属性名> 命令单独查看，列举一部分属性如下：
属性名
含义
ro.build.version.sdk
SDK 版本
ro.build.version.release
Android 系统版本
ro.build.version.security_patch
Android 安全补丁程序级别
ro.product.model
型号
ro.product.brand
品牌
ro.product.name
设备名
ro.product.board
处理器型号
ro.product.cpu.abilist
CPU 支持的 abi 列表
persist.sys.isUsbOtgEnabled
是否支持 OTG
dalvik.vm.heapsize
每个应用程序的内存上限
ro.sf.lcd_density
屏幕密度
实用功能
屏幕截图
命令：
adb shell screencap -p /sdcard/sc.png
然后将 png 文件导出到电脑：
adb pull /sdcard/sc.png
可以使用 adb shell screencap -h 查看 screencap 命令的帮助信息，下面是两个有意义的参数及含义：
参数
含义
-p
指定保存文件为 png 格式
-d display-id
指定截图的显示屏编号（有多显示屏的情况下）
实测如果指定文件名以 .png 结尾时可以省略 -p 参数；否则需要使用 -p 参数。如果不指定文件名，截图文件的内容将直接输出到 stdout。
录制屏幕
录制屏幕以 mp4 格式保存到 /sdcard：
adb shell screenrecord /sdcard/filename.mp4
需要停止时按 Ctrl-C，默认录制时间和最长录制时间都是 180 秒。
如果需要导出到电脑：
adb pull /sdcard/filename.mp4
可以使用 adb shell screenrecord --help 查看 screenrecord 命令的帮助信息，下面是常见参数及含义：
参数
含义
--size WIDTHxHEIGHT
视频的尺寸，比如 1280x720，默认是屏幕分辨率。
--bit-rate RATE
视频的比特率，默认是 4Mbps。
--time-limit TIME
录制时长，单位秒。
--verbose
输出更多信息。
重新挂载 system 分区为可写
注：需要 root 权限。
/system 分区默认挂载为只读，但有些操作比如给 Android 系统添加命令、删除自带应用等需要对 /system 进行写操作，所以需要重新挂载它为可读写。
步骤：
进入 shell 并切换到 root 用户权限。
命令：
adb shellsu
查看当前分区挂载情况。
命令：
mount
输出示例：
rootfs / rootfs ro,relatime 0 0tmpfs /dev tmpfs rw,seclabel,nosuid,relatime,mode=755 0 0devpts /dev/pts devpts rw,seclabel,relatime,mode=600 0 0proc /proc proc rw,relatime 0 0sysfs /sys sysfs rw,seclabel,relatime 0 0selinuxfs /sys/fs/selinux selinuxfs rw,relatime 0 0debugfs /sys/kernel/debug debugfs rw,relatime 0 0none /var tmpfs rw,seclabel,relatime,mode=770,gid=1000 0 0none /acct cgroup rw,relatime,cpuacct 0 0none /sys/fs/cgroup tmpfs rw,seclabel,relatime,mode=750,gid=1000 0 0none /sys/fs/cgroup/memory cgroup rw,relatime,memory 0 0tmpfs /mnt/asec tmpfs rw,seclabel,relatime,mode=755,gid=1000 0 0tmpfs /mnt/obb tmpfs rw,seclabel,relatime,mode=755,gid=1000 0 0none /dev/memcg cgroup rw,relatime,memory 0 0none /dev/cpuctl cgroup rw,relatime,cpu 0 0none /sys/fs/cgroup tmpfs rw,seclabel,relatime,mode=750,gid=1000 0 0none /sys/fs/cgroup/memory cgroup rw,relatime,memory 0 0none /sys/fs/cgroup/freezer cgroup rw,relatime,freezer 0 0/dev/block/platform/msm_sdcc.1/by-name/system /system ext4 ro,seclabel,relatime,data=ordered 0 0/dev/block/platform/msm_sdcc.1/by-name/userdata /data ext4 rw,seclabel,nosuid,nodev,relatime,noauto_da_alloc,data=ordered 0 0/dev/block/platform/msm_sdcc.1/by-name/cache /cache ext4 rw,seclabel,nosuid,nodev,relatime,data=ordered 0 0/dev/block/platform/msm_sdcc.1/by-name/persist /persist ext4 rw,seclabel,nosuid,nodev,relatime,data=ordered 0 0/dev/block/platform/msm_sdcc.1/by-name/modem /firmware vfat ro,context=u:object_r:firmware_file:s0,relatime,uid=1000,gid=1000,fmask=0337,dmask=0227,codepage=cp437,iocharset=iso8859-1,shortname=lower,errors=remount-ro 0 0/dev/fuse /mnt/shell/emulated fuse rw,nosuid,nodev,relatime,user_id=1023,group_id=1023,default_permissions,allow_other 0 0/dev/fuse /mnt/shell/emulated/0 fuse rw,nosuid,nodev,relatime,user_id=1023,group_id=1023,default_permissions,allow_other 0 0
找到其中我们关注的带 /system 的那一行：
/dev/block/platform/msm_sdcc.1/by-name/system /system ext4 ro,seclabel,relatime,data=ordered 0 0
重新挂载。
命令：
mount -o remount,rw -t yaffs2 /dev/block/platform/msm_sdcc.1/by-name/system /system
这里的 /dev/block/platform/msm_sdcc.1/by-name/system 就是我们从上一步的输出里得到的文件路径。
如果输出没有提示错误的话，操作就成功了，可以对 /system 下的文件为所欲为了。
查看连接过的 WiFi 密码
注：需要 root 权限。
命令：
adb shellsucat /data/misc/wifi/*.conf
输出示例：
network={    ssid="TP-LINK_9DFC"    scan_ssid=1    psk="123456789"    key_mgmt=WPA-PSK    group=CCMP TKIP    auth_alg=OPEN    sim_num=1    priority=13893}network={    ssid="TP-LINK_F11E"    psk="987654321"    key_mgmt=WPA-PSK    sim_num=1    priority=17293}
ssid 即为我们在 WLAN 设置里看到的名称，psk 为密码，key_mgmt 为安全加密方式。
设置系统日期和时间
注：需要 root 权限。
命令：
adb shellsudate -s 20160823.131500
表示将系统日期和时间更改为 2016 年 08 月 23 日 13 点 15 分 00 秒。
重启手机
命令：
adb reboot
检测设备是否已 root
命令：
adb shellsu
此时命令行提示符是 $ 则表示没有 root 权限，是 # 则表示已 root。
使用 Monkey 进行压力测试
Monkey 可以生成伪随机用户事件来模拟单击、触摸、手势等操作，可以对正在开发中的程序进行随机压力测试。
简单用法：
adb shell monkey -p  -v 500
表示向 指定的应用程序发送 500 个伪随机事件。
Monkey 的详细用法参考 官方文档。
刷机相关命令
重启到 Recovery 模式
命令：
adb reboot recovery
从 Recovery 重启到 Android
命令：
adb reboot
重启到 Fastboot 模式
命令：
adb reboot bootloader
通过 sideload 更新系统
如果我们下载了 Android 设备对应的系统更新包到电脑上，那么也可以通过 adb 来完成更新。
以 Recovery 模式下更新为例：
重启到 Recovery 模式。
命令：
adb reboot recovery
在设备的 Recovery 界面上操作进入 Apply update-Apply from ADB。
注：不同的 Recovery 菜单可能与此有差异，有的是一级菜单就有 Apply update from ADB。
通过 adb 上传和更新系统。
命令：
adb sideload 
更多 adb shell 命令
Android 系统是基于 Linux 内核的，所以 Linux 里的很多命令在 Android 里也有相同或类似的实现，在 adb shell 里可以调用。本文档前面的部分内容已经用到了 adb shell 命令。
查看进程
命令：
adb shell ps
输出示例：
USER     PID   PPID  VSIZE  RSS     WCHAN    PC        NAMEroot      1     0     8904   788   ffffffff 00000000 S /initroot      2     0     0      0     ffffffff 00000000 S kthreadd...u0_a71    7779  5926  1538748 48896 ffffffff 00000000 S com.sohu.inputmethod.sogou:classicu0_a58    7963  5926  1561916 59568 ffffffff 00000000 S org.mazhuang.boottimemeasure...shell     8750  217   10640  740   00000000 b6f28340 R ps
各列含义：
列名
含义
USER
所属用户
PID
进程 ID
PPID
父进程 ID
NAME
进程名
查看实时资源占用情况
命令：
adb shell top
输出示例：
User 0%, System 6%, IOW 0%, IRQ 0%User 3 + Nice 0 + Sys 21 + Idle 280 + IOW 0 + IRQ 0 + SIRQ 3 = 307  PID PR CPU% S  #THR     VSS     RSS PCY UID      Name 8763  0   3% R     1  10640K   1064K  fg shell    top  131  0   3% S     1      0K      0K  fg root     dhd_dpc 6144  0   0% S   115 1682004K 115916K  fg system   system_server  132  0   0% S     1      0K      0K  fg root     dhd_rxf 1731  0   0% S     6  20288K    788K  fg root     /system/bin/mpdecision  217  0   0% S     6  18008K    356K  fg shell    /sbin/adbd ... 7779  2   0% S    19 1538748K  48896K  bg u0_a71   com.sohu.inputmethod.sogou:classic 7963  0   0% S    18 1561916K  59568K  fg u0_a58   org.mazhuang.boottimemeasure ...
各列含义：


PID 进程 ID
PR 优先级
CPU% 当前瞬间占用 CPU 百分比
S 进程状态（R=运行，S=睡眠，T=跟踪/停止，Z=僵尸进程）

#THR

线程数
VSS Virtual Set Size 虚拟耗用内存（包含共享库占用的内存）
RSS Resident Set Size 实际使用物理内存（包含共享库占用的内存）
PCY 调度策略优先级，SP_BACKGROUND/SPFOREGROUND
UID 进程所有者的用户 ID
NAME
进程名

top 命令还支持一些命令行参数，详细用法如下：

Usage: top [ -m max_procs ] [ -n iterations ] [ -d delay ] [ -s sort_column ] [ -t ] [ -h ]    -m num  最多显示多少个进程    -n num  刷新多少次后退出    -d num  刷新时间间隔（单位秒，默认值 5）    -s col  按某列排序（可用 col 值：cpu, vss, rss, thr）    -t      显示线程信息    -h      显示帮助文档
其它

如下是其它常用命令的简单描述，前文已经专门讲过的命令不再额外说明：


cat 显示文件内容

cd 切换目录

chmod 改变文件的存取模式/访问权限

df 查看磁盘空间使用情况

grep 查找过滤输出

kill 杀死指定 PID 的进程

ls列举目录内容

mount挂载目录的查看和管理

mv 移动或重命名文件

ps 查看正在运行的进程

rm 删除文件

top 查看进程的资源占用情况

参考链接

Android Debug Bridge

ADB Shell Commands

logcat Command-line Tool

Android ADB命令大全

adb 命令行的使用记录

Android ADB命令大全(通过ADB命令查看wifi密码、MAC地址、设备信息、操作文件、查看文件、日志信息、卸载、启动和安装APK等)
那些做Android开发必须知道的ADB命令

adb shell top




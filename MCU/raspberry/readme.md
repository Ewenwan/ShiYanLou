# 摘要
[playWithRaspberryPi 实战笔记](https://github.com/summer2009/playWithRaspberryPi/tree/master/posts)

[raspberrypi_cookbook 树莓派python代码本](https://github.com/Ewenwan/raspberrypi_cookbook_ed2)

    树莓派(Raspberry Pi)是一种小型的单片机计算机；
    它可以通过其引脚和接口与外部硬件通信。
    Pi(树莓派的简称)展示了我们可以如何分解大型的Realbox机器，
    使Dynamicland成为许多计算机、传感器和执行器的生态系统，而不是服务器上的软件。
    “......Pi之于Dynamicland正如USB接口之于笔记本电脑：这就是我将东西插入Dynamicland的方式。”
    使用Realtalk来打开/关闭Pi上LED的最小示例。
    解释Realtalk的“反应数据库”(reactive database)编程模型; 
    它如何使动感桌(dynamic table)中的Pi编程变得异常简单。
    Pi的编程与Dynamicland中的任何其他编程没什么区别。
    通过向Pi发送完整程序来进行远程控制，而不是用一些动力不足的控制语言与之通信。
    关于Pi如何在未来发挥作用的猜想。

[基于树莓派的智能小车 python](https://github.com/Ewenwan/robot_car)

# 树莓派
## 树莓派简介
Raspberry Pi，中文名：树莓派。树莓派由注册于英国的慈善组织“Raspberry Pi 基金会”开发，旨在于学生计算机编程教育，树莓派官方的关键词就是："Teach, Learn, and Make with Raspberry Pi"。在2006年，树莓派早期概念是基于[Atmel](http://www.atmel.com/)(Atmel公司会在单片机一章中有介绍)的 ATmega644单片机。2012年2月29日，树莓派基金会发布了256MB内存的树莓派B型。四年后，也就是2016年2月29号，基金会宣布树莓派3 B型将以35美元的价格发售。
## 树莓参数对比
与前三款比较（包括树莓派 B+），树莓派3在硬件配置上有什么不一样呢？
以下是主要几款树莓派（不包括树莓派A+及Zero）的主要参数对比：

| 硬件           | A型 (一代）    | B型 (二代)    | B+版     | 树莓派3 B型 |
| ------------- |-------------  | -------------|----------| ----------|
| CPU           | ARM1176JZF-S核心（ARM11系列）700MHz | 900MHz的四核  Cortex-A7 | 同左 | 1.2GHz 四核 64 位 ARMv8 处理器|
| SoC（系统级芯片 )| Broadcom BCM2835|  Broadcom BCM2835 | 同左 | Broadcom BCM2837 |
| GPU（图形处理器）| Broadcom VideoCore IV, OpenGL ES 2.0, 1080p 30 h.264/MPEG-4 AVC 高清解码器 |  同左 | 同左 | 同左 |
| 内存           |  256MB         |    512MB    |    512MB  |    1GB  |

据说，树莓派3外观相对前几款树莓派并没有太多变化，但性能提升了50%。而且，就在树莓派3发布几天之后，windows 10 iot就提供了支持对树莓派3的支持，可以看出大家对这款卡片机的青睐。于是，我特意查了一下BCM2835以及BCM2837这两款系统级芯片，但里面主要介绍了树莓的外围扩展，定时器，中断，GPIO,PWM等，有更高的项目需求可以看看。如果想看具体的芯片书册，可到datasheet进行下载，附上[BCM2835链接。](http://www.alldatasheet.com/datasheet-pdf/pdf/502533/BOARDCOM/BCM2835.html)

# 树莓派应用
[Game Utility library for the Raspberry PI 树莓派游戏库](https://github.com/progschj/pigu)

[C/C++ Library for Raspberry Pi. ](https://github.com/Ewenwan/raspberryX)

[树莓派(Raspberry Pi )资源大全中文版 , 包括工具、项目、镜像、资源等](https://github.com/Ewenwan/awesome-raspberry-pi-zh)

[基于树莓派的 免费数字标牌软件 Screenly OSE](https://github.com/Ewenwan/screenly-ose)

[OpenCV检测场景内是否有移动物体 并播放音乐](http://blog.topspeedsnail.com/archives/10797)

[有人已经把TensorFlow移植到了树莓派](https://github.com/samjabrahams/tensorflow-on-raspberry-pi)

[人脸检测](http://blog.topspeedsnail.com/archives/10931)

[树莓派 python 机器人编程](https://github.com/PacktPublishing/Learn-Robotics-Programming)

[树莓派3B+英特尔神经计算棒进行高速目标检测](https://github.com/ahangchen/windy-afternoon/blob/master/linux/raspbian/ncs_detection.md)

# 1. 无屏幕和键盘配置树莓派WiFi和SSH，连接wifi后，支持远程登录

## a. WiFi 网络配置
    用户可以在未启动树莓派的状态下单独修改 /boot/wpa_supplicant.conf 文件配置 WiFi 的 SSID 和密码，
    这样树莓派启动后会自行读取 wpa_supplicant.conf 配置文件连接 WiFi 设备。
    
    方法：
        将刷好 Raspbian 系统的 SD 卡用电脑读取。
        在 boot 分区，也就是树莓派的 /boot 目录下新建 wpa_supplicant.conf 文件，
        按照下面的参考格式填入内容并保存 wpa_supplicant.conf 文件。
        
        在TF卡的boot分区，创建wpa_supplicant.conf文件，加入如下内容：     
        
        在树莓派通电后会自动添加到/etc/wpa_supplicant/wpa_supplicant.conf文件里面，进行自动连接。
        
> 文件内容： 
```
country=CN
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
 
network={
ssid="WiFi-A"
psk="12345678"
key_mgmt=WPA-PSK
priority=1
}
 
network={
ssid="WiFi-B"
psk="12345678"
key_mgmt=WPA-PSK
priority=2
scan_ssid=1
}

```
> 文件说明:

    #ssid: 网络的ssid 网络名称
    #psk:  / wep_key0: 密码
    #priority: 连接优先级，数字越大优先级越高（不可以是负数）
    #scan_ssid: 连接隐藏WiFi时需要指定该值为1
    #key_mgmt:  wifi加密方式

> 1. 如果你的 WiFi 没有密码

    network={
    ssid="你的无线网络名称（ssid）"
    key_mgmt=NONE
    }

> 2.如果你的 WiFi 使用WEP加密

    network={
    ssid="你的无线网络名称（ssid）"
    key_mgmt=NONE
    wep_key0="你的wifi密码"
    }

> 3. 如果你的 WiFi 使用WPA/WPA2加密

    network={
    ssid="你的无线网络名称（ssid）"
    key_mgmt=WPA-PSK
    psk="你的wifi密码"
    }
> 4. WiFi 的加密模式, 由路由器设置wifi时决定

## b. 开启 SSH 服务, 进行远程登录
      开启SSH远程：（自2016年9月开始，raspberry默认关闭ssh连接）
      在TF卡分区里面创建一个名为 “ssh” 空文件即可（不要有txt后缀！）
      
      如果通过 ssh 连接树莓派出现 Access denied 这个提示则说明 ssh 服务没有开启。
      要手动开启的话，和 WiFi 配置相似，同样在 boot 分区新建一个文件，空白的即可，文件命名为 ssh。
      注意要小写且不要有任何扩展名。
      树莓派在启动之后会在检测到这个文件之后自动启用 ssh 服务。
      
      随后即可通过登录路由器找到树莓派的 IP 地址，(也可使用网段扫描法找树莓派IP )
       ipscan22.exe  https://pan.baidu.com/share/link?shareid=3434443053&uk=605377859&errno=0&errmsg=Auth%20Login%20Sucess&&bduss=&ssnerror=0&traceid=
      
      通过 ssh (只有命令行界面)连接到树莓派了。

      手动启用 SSH 服务，命令如下：
      service sshd restart
      
      https://blog.csdn.net/u012313335/article/details/73920256
      
      如需在手机或者平板上进行连接，需要在应用商店下载安装Serverauditor，完成安装后打开软件。
      点击右上角的“+”号添加设备，在连接界面中，填写主机IP、用户名以及密码（其他可不填写），
      填写完成后点击右上角的保存图标。完成之后即会在服务器界面出现树莓派IP的选项，
      点击进行连接即可连接到树莓派。
      
      
      window下客户端推荐使用putty.exe这个小巧的ssh客户端软件，进行远程ssh登录。
      linus下：
        下可直接使用 终端 ssh登陆
          ssh pi@192.168.1.176     用户名@IP地址
          输入密码 raspberry （注意密码不回显） 回车即可登陆

      SSH登陆后，可开启 VNC 服务，可以使用 VNC（或者window自带的远程桌面应用）远程图像界面登录。

      window自带的远程桌面应用登录：
      1. 树莓派端安装 xdrp服务 
          sudo apt-get install xrdp

      2. 卸载 再安装
         sudo apt-get purge tightvnc xrdp
         sudo apt-get install tightvncserver xrdp

      3. 最后启动 xrdp 服务，sudo /etc/init.d/xrdp restart，此时会启用相应端口，配置方面默认即可。
         通过命令：netstat -tnl
         查看 3350 3389 5910 这三个端口处于LISTEN，一般就没问题了。
         dpkg -L + 软件包的名字，可以知道这个软件包包含了哪些文件。
      
      VNC 远程 图像界面登录：
     1.  安装 tightvncserver
        sudo apt-get install tightvncserver
     2. 启动vnc服务 
        tightvncserver
          设置8位密码
          选择n     y会有错误
          电脑vcn远程登陆   192.168.31.47:1   后面需要添加:1
     3. 结束服务
        tightvncserver -kill 
     4. 卸载
        sudo apt-get remove tightvncserver
[参考](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/raspberry/%E6%A0%91%E8%8E%93%E6%B4%BE%E5%9F%BA%E6%9C%AC%E4%BD%BF%E7%94%A8.txt)


      串口登录
![](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/raspberry/raspberry_io.PNG)
      

## c. HDMI输出
    开启强制HDMI输出：（很多现在的显示器在树莓派上并不能识别）
    在TF卡分区，打开config.txt文件(开机后位置： /boot/config.txt)，修改如下：

    hdmi_safe=1
    config_hdmi_boost=4
    hdmi_ignore_edid=0xa5000080
    hdmi_group=2
    hdmi_mode=82   
    
     一些参数介绍：

        项	含义
        hdmi_safe=1	安全启动HDMI
        config_hdmi_boost=4	开启热插拔
        hdmi_group=1	CEA电视显示器
        hdmi_group=2	DMT电脑显示器
        hdmi_ignore_edid=0xa5000080	忽略自动探测的分辨率
        输出分辨率：	
        hdmi_mode=4	640x480 60Hz
        hdmi_mode=9	800x600 60Hz
        hdmi_mode=16	1024x768 60Hz
        hdmi_mode=82	1080p 60Hz


    
# 2. 树莓派上的软件安装和卸载命令汇总

## a. 基础命令
        安装软件 apt-get install softname1 softname2 softname3……
        卸载软件 apt-get remove softname1 softname2 softname3……
        卸载并清除配置 apt-get remove –purge softname1
        更新软件信息数据库 apt-get update
        进行系统升级 apt-get upgrade
        搜索软件包 apt-cache search softname1 softname2 softname3……

        如果使用 apt-get 遇到速度慢或者源不存在等错误，可能需要更换源，请参考此处。
        http://shumeipai.nxez.com/2013/08/31/raspbian-chinese-software-source.html

        安装deb软件包  dpkg -i xxx.deb     install
        删除软件包     dpkg -r xxx.deb     remove
        连同配置文件一起删除 dpkg -r –purge xxx.deb
        查看软件包信息 dpkg -info xxx.deb
        查看文件拷贝详情 dpkg -L xxx.deb    
        查看系统中已安装软件包信息 dpkg -l   list
        重新配置软件包 dpkg-reconfigure xxx

        清除所有已删除包的残馀配置文件

        dpkg -l |grep ^rc|awk '{print $2}' |sudo xargs dpkg -P
        如果报如下错误，证明你的系统中没有残留配置文件了，无须担心。
        dpkg: –purge needs at least one package name argument

        dpkg安裝的可以用apt卸載，反之亦可。
        
> 常用apt命令列表
```sh
apt-cache search # ------(package 搜索包)
apt-cache show #------(package 获取包的相关信息，如说明、大小、版本等)
sudo apt-get install # ------(package 安装包)
sudo apt-get install # -----(package - - reinstall 重新安装包)
sudo apt-get -f install # -----(强制安装?#"-f = --fix-missing"当是修复安装吧...)
sudo apt-get remove #-----(package 删除包)
sudo apt-get remove - - purge # ------(package 删除包，包括删除配置文件等)
sudo apt-get autoremove --purge # ----(package 删除包及其依赖的软件包+配置文件等（只对6.10有效，强烈推荐）)
sudo apt-get update #------更新源
sudo apt-get upgrade #------更新已安装的包
sudo apt-get dist-upgrade # ---------升级系统
sudo apt-get dselect-upgrade #------使用 dselect 升级
apt-cache depends #-------(package 了解使用依赖)
apt-cache rdepends # ------(package 了解某个具体的依赖?#当是查看该包被哪些包依赖吧...)
sudo apt-get build-dep # ------(package 安装相关的编译环境)
apt-get source #------(package 下载该包的源代码)
sudo apt-get clean && sudo apt-get autoclean # --------清理下载文件的存档 && 只清理过时的包
sudo apt-get check #-------检查是否有损坏的依赖
```
## b. aptitude 命令
        aptitude 与 apt-get 一样，是 Debian 及其衍生系统中功能极其强大的包管理工具。
        与 apt-get 不同的是，aptitude 在处理依赖问题上更佳一些。
        举例来说，aptitude 在删除一个包时，会同时删除本身所依赖的包。
        这样，系统中不会残留无用的包，整个系统更为干净。
        以下是笔者总结的一些常用 aptitude 命令，仅供参考。
        

        aptitude update 更新可用的包列表
        aptitude upgrade 升级可用的包
        aptitude dist-upgrade 将系统升级到新的发行版
        aptitude install pkgname 安装包
        aptitude remove pkgname 删除包
        aptitude purge pkgname 删除包及其配置文件
        aptitude search string 搜索包
        aptitude show pkgname 显示包的详细信息
        aptitude clean 删除下载的包文件
        aptitude autoclean 仅删除过期的包文件

当然，你也可以在文本界面模式中使用 aptitude。


# 树莓派硬件系统
在上一章节中，简单的介绍了树莓派及各版本树莓派参数的对比。这一章节会重点介绍树莓派的硬件系统（树莓派的 CPU 架构将会在 AMR 章节中讲述）。首先，我们会直接介绍树莓派的原理图。然后在拿一款真实的树莓派，简单的看看在电路图中的各组成部分。

## 供电系统及振荡电路
![系统芯片的电源及接地](https://github.com/sundway/iot/blob/master/imgs/1.1.1-1.png)
https://github.com/sundway/iot/blob/master/imgs/1.1.1-1.png
上面这个看起来很复杂的几个电路，其实只做了一件事，就是给系统芯片（BCM2835）供电。可以看到所有管脚只连接到了 VCC（5v、3.3v、1.8v）和 GND。其中一些管脚还需要做些限流，限压等，接了一些电容，电阻及电感等。然而硬件工程师并不需要自己去计算这些元件的参数，因为芯片厂商在设计每款芯片时，都会设计这些外围电路，并在芯片手册中附上最佳电路图。而使用这些芯片的硬件工程师只需要画 PCB 版的时候设计如何布局及连线。

![usb及转压芯片](https://github.com/sundway/iot/blob/master/imgs/1.1.1-2.png)

上图左边的是usb供电电路，右边是转压电路，这两个电路在所有的硬件设计中都是比较简单和常用的电路。左边图中2、3、4三个关键是 usb 的数据及主从控制管脚，一般 usb 都有是四个或五个管脚（其中两个为电源和地，另外两个为数据读写，还有一个可以作为主从控制，复杂的 usb 集群通信可以用到）。这里没有画出它们的连接。右边图主要用了 NCP1117-3v3、LP2980-2v5、NCP117-1v8 三款芯片将5v分别转化成3.3v，2.5v及1.8v。从在上一张图中可以看出，系统芯片出来需要5v，还分别需要1.8v及3.3v(很多芯片都是5v或3.3v供电)，单从上一张图中并没有看出为什么会转一个2.5v的电压出来。

![振荡器](https://github.com/sundway/iot/blob/master/imgs/1.1.1-3.png)

上面的这个看似不起眼的电路其实就是整个系统的心脏，可以看到途中有 osc 的字母，osc 是英文字母 oscillator (振荡器)的缩写，MCU (微控制器)中通常对应的为晶振。振荡器主要作用是将直流电能转换为具有一定频率的交流电能的一种装置。而晶振的作用也就是将直流转化成频率，因为 MCU 工作时，是一条一条地从 ROM 中取指令，然后一步一步地执行。单片机访问一次存储器的时间，称之为一个机器周期，这是一个时间基准。一个机器周期包括12个时钟周期，而晶振就是提供这个时间基准。我们通常可以由所选的晶振频率计算出一个机器周期的时间，对应到汇编语言语言中，大概就能估算到一段代码的执行时间。图中的 19.2MHZ，那么一个时钟周期就是 12/19.2（0.625）微秒。这里也可以理解为什么不选用 19MHZ的晶振频率了，因为选用 19MHZ 作为晶振频率时，将会得到一个无限不循环小数的时钟周期，不利与计算。

## PWM及DAC

![PWM及DAC电路图](https://github.com/sundway/iot/blob/master/imgs/1.1.1-4.png)

上图是 DAC（数字模拟转换器）电路，DAC 是英文单词 Digital to Analog Converter 的缩写。在很多数字系统中（例如计算机），信号以数字方式存储和传输，而数字模拟转换器可以将这样的信号转换为模拟信号，从而使得它们能够被外界（人或其他非数字系统）识别，从这张图中可以看到上面的 2.5V 电压的用处，这里的 2.5V 是作为输入的基准电压。与 DAC 对应的还有 ADC ，顾名思义，ADC 的作用是将模拟信号转化为数字信号。很多传感器输出的事模拟信号，而MCU无法读取模拟信号，只能通过 ADC 将模拟信号先转化为数字信号，再进行读取。在选择传感器（如加速度、重力等传感器），这些传感器一半都有数字传感器或模拟传感器，数字传感器往往比模拟传感器精准。PWM 是英文单词 Pulse Width Modulation（脉冲宽度调制） 的缩写。脉冲宽度调制是利用微处理器的数字输出来对模拟电路进行控制的一种非常有效的技术，广泛应用在从测量、通信到功率控制与变换的许多领域中。最典型的例子就是用 PWM 控制电机转速，而目前市面上的多数 MCU 都带有 PWM 模块。51等传统单片机是没有 PWM 模块的，往往可以通过程序模拟，或外接 PWM 模块支持。

## GPIO
![GPIO电路图](https://github.com/sundway/iot/blob/master/imgs/1.1.1-5.png)
GPIO 是英文单词 General Purpose Input Output（通用输入/输出）的缩写。对于程序员来说，GPIO 应该是我们接触最多也最熟悉的部分了。控制外接电路，往往是连接 GPIO 管脚。使用树莓派，无论用 node、python、C# 或者是其他语言，往往都会引用一个 GPIO 的库来进行控制，而这些库往往又是基于 C 或 C++ 进行的底层操作。不引用第三方库，直接用 C 对这些管脚进行控制也是相当简单的。比如，硬件领域的 “hello world”（点亮 LED 灯），只需要对一个管脚写入0或1，便可控制 LED 亮灭。上面第一张图可以注意到，很多管脚是具有第二功能的，比如上面的两路输出 PWM 的管脚以及 SD 卡相关的管脚。下图中有两个管脚需要格外注意，TXD 及 RXD 这是串口通信的管脚，串口通信对应的还有并口通信，串口通信相比较并口通信的优势就是在于拿时间换取空间。

## SD卡

![SD卡电路](https://github.com/sundway/iot/blob/master/imgs/1.1.1-6.png)

从上面的图可以看到需要操作的管脚是4、5、2、7、8、9、1几个管脚。SD的这个电路为通用的电路，换其它MCU时，这种连接依然可以work.

## HDMI、音频、网卡电路
HDMI和音频是系统芯片内部的功能，主要按照数据手册里方式的进行连接。该图也较为复杂，就没有必要贴出来了。网卡及串口在树莓派中借助的是 LAN9512 芯片，Microchip公司生产的 LAN9512/LAN9512i 是第一款工业的全集成、高速2.0 USB、高性能10/100M网络控制器。具体的电路连接也是需要阅读 LAN9512 的数据手册。

## 树莓派正面

![树莓派正面](https://github.com/sundway/iot/blob/master/imgs/1.1.1-7.png)

上面位于正中心的那块是这个系统的核心芯片 BCM2835，左边为 LAN9154，布局的规则也决定，网卡接口及USB接口设计在了 LAN9154 的右边。USB供电系统旁边有三个转压芯片，这一部分区域是给系统提供4种电压。树莓派为双层板，正面布满系统需要的绝大部分元器件。

![树莓派反面](https://github.com/sundway/iot/blob/master/imgs/1.1.1-8.png)

PCB 板反面右边是SD卡接口，中心是一个19.2MHZ的晶振，对应正面中心的 BCM2835 芯片。


# 树莓派软件系统
树莓派和其它MUC一样，可以直接以汇编或C语言进行编程，在编写一些简单的程序时，是不会有什么问题的。编写复杂的程序，这样显然会更加吃力。而且学习成本也高，往往我们需要关心的不是用何种语言去编程，而是不要管底层驱动。跟树莓派装上操作系统，便可以解决这些问题。树莓派官方提供了很多可选的[系统镜像](https://www.raspberrypi.org/downloads/)，这里会以两款比较流行的系统，树莓派官方系统Raspbian及win10 iot进行安装学习。

## mac下安装Raspbian

windows下有现成的工具可用，所以windows下其实更简单，但手头没有可用的windows系统，就用mac给树莓派装系统。
### 下载镜像
可以到树莓派[官方下载地址](https://www.raspberrypi.org/downloads/)，下载镜像。我选择Raspbian，官方的一个操作系统。
### 安装系统
* 将SD卡插到电脑上，输入```df -h```查看已经挂在的卷。（可以找到我们的SD卡在系统里对应的设备文件）
* 使用```diskutil unmount```将这些分区卸载。(如我的SD卡在系统里对应的设备文件/dev/disk4s1，则需要使用‘’‘diskutil unmount /dev/disk4s1将SD卡分区卸载。)
* 再使用```diskutil list```确认/dev/disk4s1已被卸载。
* 使用dd命令将系统镜像写入。
  * ```dd bs=4m if=2016-02-09-jessie-raspbian.img of=/dev/rdisk4｀```

SD系统写入成功。

![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-0.png)

#### PS:

* 系统镜像分‘Full desktop image’和‘Minimal image based on Debian Jessie’，我安装的是前面的，太大了，以至于32G的SD不够用，开发的还是建议按装lite版本。
* 如果第一次安装树莓派，外接显示器可能无法连接，需要修改配置文件config.txt里的参数

```
hdmi_safe=1 //取消注释
config_hdmi_boost=4 //取消注释
``` 
### 连接树莓派
采用ssh的方式连接树莓派，mac下下了一个Ip Scanner查看局域网里设备ip地址。找到Raspberra pi的ip地址。
![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-1.png)

树莓派默认的账户: pi，密码：raspberry。（之前的操作系统必须先链接显示器，用键盘确认），ssh首次登录会生成密钥，选择yes,连接```ssh pi@ip```,输入密码，登录成功。

![](https://img.alicdn.com/tps/TB1fD.MLFXXXXaFXpXXXXXXXXXX-564-155.png)

由于我的系统时重装的，所以登录时会报错。本机直接输入```ssh-keygen -R hostname```重新生成密钥即可。

![](https://gw.alicdn.com/tps/TB1RsZSLFXXXXa5XXXXXXXXXXXX-564-364.png)

### 安装nodejs
因为项目时基于node开发的，所以只需要装一下node即可。树莓派使用的架构用的架构是ARMv6，所以需要下载一个ARMv6版本的node，linux下下载命令``` wget https://nodejs.org/dist/v4.3.1/node-v4.3.1-linux-armv6l.tar.xz ```

![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-4.png)

等待下载，家里网速太慢。。。

下载成功之后需要解压.xz文件，先```xz -d xxx.tar.xz```将xxx.tar.xz解压成xxx.tar。然后，再用 ```tar xvf xxx.tar```来解包

![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-3.png)

由于node已经编译过了，不需要在本地编译了，直接使用即可，done.

### 测试Demo

![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-5.png)

![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-6.png)

![](https://github.com/sundway/iot/blob/master/imgs/1.1.2-7.png)

Done，环境搭建完成。




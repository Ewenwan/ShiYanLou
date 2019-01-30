# 摘要
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




# 树莓派应用
# 1. 无屏幕和键盘配置树莓派WiFi和SSH，连接wifi后，支持远程登录
## a. WiFi 网络配置
    用户可以在未启动树莓派的状态下单独修改 /boot/wpa_supplicant.conf 文件配置 WiFi 的 SSID 和密码，
    这样树莓派启动后会自行读取 wpa_supplicant.conf 配置文件连接 WiFi 设备。
    
    方法：
        将刷好 Raspbian 系统的 SD 卡用电脑读取。
        在 boot 分区，也就是树莓派的 /boot 目录下新建 wpa_supplicant.conf 文件，
        按照下面的参考格式填入内容并保存 wpa_supplicant.conf 文件。
        
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

      如果通过 ssh 连接树莓派出现 Access denied 这个提示则说明 ssh 服务没有开启。
      要手动开启的话，和 WiFi 配置相似，同样在 boot 分区新建一个文件，空白的即可，文件命名为 ssh。
      注意要小写且不要有任何扩展名。
      树莓派在启动之后会在检测到这个文件之后自动启用 ssh 服务。
      
      随后即可通过登录路由器找到树莓派的 IP 地址，(也可使用网段扫描法找树莓派IP )
       ipscan22.exe  https://pan.baidu.com/share/link?shareid=3434443053&uk=605377859&errno=0&errmsg=Auth%20Login%20Sucess&&bduss=&ssnerror=0&traceid=
      
      通过 ssh (只有命令行界面)连接到树莓派了。

      手动启用 SSH 服务，命令如下：
      service sshd restart

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
    
    
    
    
    
    



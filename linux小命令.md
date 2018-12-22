# 查询文件夹大小
    du -h 所有子文件大小
    du -h -d 1  当前1级目录大小
    df -h 查看各个盘使用比例情况

# 1、输出字符 banner   全#号
    sudo apt-get install sysvbanner

    banner linux
    printerbanner -w 50 AB
    类似的命令 toilet    #号 "号 字母m
    sudo apt-get install toilet

    figlet     -号
    sudo apt-get install figlet


# 2、一双眼睛
    xeyes
    sudo apt-get install x11-apps



# 3、文字雨
    cmatrix
    sudo apt-get install cmatrix


# 4、文字火炉
    aafire
    sudo ape-get install libaa-bin



# 5、压缩解压
    zip：
    打包 ：zip something.zip something （目录请加 -r 参数）
    压缩 oRB_SLAM2文件夹 到 orb_slam.zip
    zip -q -r orb_slam.zip oRB_SLAM2/

    解包：unzip something.zip
    指定路径：-d 参数


    tar：
    打包：tar -zcvf something.tar something
    解包：tar -zxvf something.tar

    指定路径：-C 

    rar:
    # rar a all *.jpg
    这条命令是将所有.jpg的文件压缩成一个rar包，名为all.rar，该程序会将.rar 扩展名将自动附加到包名后。
    # sudo unrar e all.rar all_file -y
    这条命令是将all.rar中的所有文件 解压出来到all_file下  
    # sudo unrar x all.rar all_file -y
    按原文件夹文件层级 关系解压文件到 all_file下 


    各种压缩包的压缩和解压方法
## .tar.gz和.tgz

    解压：tar zxvf FileName.tar.gz

    压缩：tar zcvf FileName.tar.gz DirName

    PS:tar命令对于长选项和短选项的顺序有要求，例如，不覆盖已有文件的选项--skip-old-files:

    tar --skip-old-files -zxvf FileName.tar.gz #正确

    tar -zxvf --skip-old-files FileName.tar.gz #错误
## .tar

    解包：tar xvf FileName.tar

    打包：tar cvf FileName.tar DirName

    （注：tar是打包，不是压缩！）
## .gz

    解压1：gunzip FileName.gz

    解压2：gzip -d FileName.gz

    压缩：gzip FileName
## .bz2

    解压1：bzip2 -d FileName.bz2

    解压2：bunzip2 FileName.bz2

    压缩： bzip2 -z FileName
## .tar.bz2

    解压：tar jxvf FileName.tar.bz2

    压缩：tar jcvf FileName.tar.bz2 DirName
## .bz

    解压1：bzip2 -d FileName.bz

    解压2：bunzip2 FileName.bz
## .tar.bz

    解压：tar jxvf FileName.tar.bz
    .Z

    解压：uncompress FileName.Z

    压缩：compress FileName
## .tar.z

    解压：tar Zxvf FileName.tar.z

    压缩：tar Zcvf FileName.tar.z DirName
## .zip

    解压：unzip FileName.zip

    压缩：zip FileName.zip DirName
## .rar

    解压：rar x FileName.rar

    压缩：rar a FileName.rar DirName
##  .lha

    解压：lha -e FileName.lha

    压缩：lha -a FileName.lha FileName
##  .rpm

    解包：rpm2cpio FileName.rpm | cpio -div
##  .deb

    解包：ar p FileName.deb data.Tar.gz | Tar zxf -
## 万能脚本

    解压：sEx x FileName.*

    压缩：sEx a FileName.* FileName

    sEx只是调用相关程序，本身并无压缩、解压功能，请注意！

    sEx请到： http://sourceforge.net/projects/sex下载！

# 6、动物文字
    cowsay

    sudo apt-get install cowsay

    cowsay hello shiyanlou   默认牛
    cowsay -l 打印支持的所有动物
    cowsay -f elephant hello shiyanlou

# 7、搜狗拼音输入法无法输入中文
    cd ~/.config
    rm -rf SogouPY SogouPY.users sogou-qimpanel
    killall fcitx
    fcitx
    install
    sudo add-apt-repository ppa:fcitx-team/nightly
    sudo apt-get install fcitx sogoupinyin 
    sudo apt-get remove fcitx sogoupinyin

# 7.2 fcitx框架下谷歌输入法的安装
    sudo apt-get install fcitx fcitx-googlepinyin im-config
    im-config
    点击OK按钮，再点击Yes按钮，将fcitx设为默认的输入法框架，最后点击OK。
    然后重启系统。
    点击fcitx的键盘图标，选择text entry  settings
    在打开的窗口中点击+号
    然后在文本框中输入pinyin查找谷歌拼音输入法，将其添加到输入法列表中。
    如果查找不到，重启系统后再试一次。
    接下来你就能使用谷歌拼音输入法了。
    
# 8、linux 设置默认声卡
    终端里打alsamixer 按 f6 会出现声卡列表 
    一般来说，usb 声卡总是第二位的，声卡编号是1(默认声卡是0) 
    在 $HOME 下建立 .asoundrc 里面输入 
    defaults.ctl.card 1 defaults.pcm.card 1 
    最后的1是声卡编号 然后重启一下alsa就把1号声卡(usb声卡)设置成默认声卡了。 


    //////////　　可能不行
    pcm.!default {
        type asym
        playback.pcm {
            type plug
            slave.pcm "hw:0,0"
        }
        capture.pcm {
            type plug
            slave.pcm "hw:1,0"
        }
    }

# Ubuntu14.04使用su切换到root时提示:Authentication failure
    这是因为Ubuntu系统默认没有激活root用户，因此需手动激活,操作如下:
    sudo passwd
    
    输入密码
    确认密码
    之后就可以了
# linux 连接不到源的问题
[参考解决](http://www.linuxdiyf.com/linux/23934.html)

    在是还用atp-get的时候出现了apt-get: Could not resolve 'archive.ubuntu.com'的错误，
    一开始按照网上的说法，修改apt-get的源网址，
    添加了国内的一些网址资源以后还是不行，比如添加了以下的一些资源网址：

    deb http://mirrors.ustc.edu.cn/ubuntu/ precise-updates main restricted
    deb-src http://mirrors.ustc.edu.cn/ubuntu/ precise-updates main restricted
    deb http://mirrors.ustc.edu.cn/ubuntu/ precise universe
    deb-src http://mirrors.ustc.edu.cn/ubuntu/ precise universe

    至于怎么修改该文件，可以通过通过SSH Secure File Transfer
    软件打开目录/etc/apt下面的source.list文件通过记事本进行编辑，
    也就是将上面的内容添加到文件的后面。

    但是添加完以上网址后，还是出现Could not resolve 'archive.ubuntu.com'的错误，
    这种原因其实是因为还缺少一个步骤，就是讲添加的这几个网址的网址和IP进行映射，
    修改/etc/目录下的host文件，用记事本打开

    添加链接不上的地址 直接的ip地址
    10.1.14.235     mirrors.hikvision.com.cn
    
#  vmware虚拟机设置共享文件夹
    在VMware 工具栏选择 虚拟机（M），下拉菜单中有安装VMware Tools 选项，
    我刚开始选的时候一直出现下述问题
    “客户机操作系统已将 CD-ROM 门锁定，并且可能正在使用 CD-ROM，
    这可能会导致客户机无法识别介质的更改。
    如果可能，请在断开连接之前从客户机内部弹出 CD-ROM。”

    主要是 CD-ROM 在之前安装时选择的是 ununtu的安装文件
    发现是因为客户机（即虚拟机）的驱动是指向安装Ubuntu的iso的

    现在安装好了，可以更改回来了
    先关闭虚拟机，在虚拟机 设置 硬件 中CD/DVD选择自动检测

    重新打开虚拟机，此时装载即可成功
    创建 挂载点
    mkdir /mnt/cdrom  
    挂载 CD/DVD
    mount /dev/cdrom /mnt/cdrom  
    此时可以在 /mnt/cdrom 找到 VMwareTools 的安装文件

    解压
    tar -zxvf /mnt/cdrom/VMwareTools-x.x.x-yyyy.tar.gz 
    生成 vmware-tools-distrib  
    进入安装
    cd /mnt/vmware-tools-distrib  
    sudo ./vmware-install.pl -y

    设置共享文件夹地址
    虚拟机 -> 设置-> 选项 -> 共享文件夹 -> 总是启用 添加主机共享文件夹路径

    在虚拟机文件夹下 /mnt/hgfs  可以查看到 共享文件夹


# apt-get无法安装 
    解决方法：强制解锁
    sudo rm /var/cache/apt/archives/lock3
    sudo rm /var/cache/apt/archives/lock 
    sudo rm /var/lib/dpkg/lock
    
    
# 怎么防止远程Linux自动断开SSH连接
    下面的操作是在本地ssh客户端上，不是远程主机。

    编辑SSH配置文件：
    $ vim ~/.ssh/config    # 当前登陆用户生效

    添加：
    Host *
     ServerAliveInterval 30

    *号代表所有主机，你可以指定某个主机，如：
    Host server1
     ServerAliveInterval 30

    ServerAliveInterval 30
    表示ssh客户端每隔30秒给远程主机发送一个no-op包，
    no-op是无任何操作的意思，这样远程主机就不会关闭这个SSH会话。

    为了使所有用户生效，你可以在/etc/ssh/ssh_config全局配置文件添加如下一行：
    ServerAliveInterval 30

    还可以在连接时使用选项：
    $ ssh -o ServerAliveInterval=30 user@remote-ssh-server-ip

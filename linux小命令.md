# 0、查询文件夹大小
    du -h 所有子文件大小
    du -h -d 1  当前1级目录大小
    df -h 查看各个盘使用比例情况

# 0.1 cpu使用信息 

> a. top 命令
     
        Cpu(s): 0.3% us    用户空间占用CPU百分比
        1.0% sy            内核空间占用CPU百分比
        0.0% ni            用户进程空间内改变过优先级的进程占用CPU百分比
        98.7% id    空闲CPU百分比
        0.0% wa            等待输入输出的CPU时间百分比
        0.0% hi    
        0.0% si 
        
        最后两行为内存信息。内容如下：
        Mem: 191272k total    物理内存总量
        173656k used          使用的物理内存总量
        17616k free           空闲内存总量
        22052k buffers        用作内核缓存的内存量
        Swap: 192772k total   交换区总量
        0k used               使用的交换区总量
        192772k free          空闲交换区总量
        123988k cached        缓冲的交换区总量。
        
        进程的详细信息，各列的含义。
        列名    含义
        PID     进程id
        PPID    父进程id
        RUSER   Real user name
        UID     进程所有者的用户id
        USER    进程所有者的用户名
        GROUP   进程所有者的组名
        TTY     启动进程的终端名。不是从终端启动的进程则显示为 ?
        PR      优先级
        NI      nice值。负值表示高优先级，正值表示低优先级
        P       最后使用的CPU，仅在多CPU环境下有意义
        %CPU    上次更新到现在的CPU时间占用百分比
        TIME    进程使用的CPU时间总计，单位秒
        TIME+   进程使用的CPU时间总计，单位1/100秒
        %MEM    进程使用的物理内存百分比
        VIRT    进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES
        SWAP    进程使用的虚拟内存中，被换出的大小，单位kb。
        RES     进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA
        CODE    可执行代码占用的物理内存大小，单位kb
        DATA    可执行代码以外的部分(数据段+栈)占用的物理内存大小，单位kb
        SHR     共享内存大小，单位kb
        nFLT    页面错误次数
        nDRT    最后一次写入到现在，被修改过的页面数。
        S       进程状态。
                    D=不可中断的睡眠状态
                    R=运行
                    S=睡眠
                    T=跟踪/停止
                    Z=僵尸进程
        COMMAND  命令名/命令行
        WCHAN    若该进程在睡眠，则显示睡眠中的系统函数名
        Flags    任务标志，参考 sched.h

        top命令中load average显示的是最近1分钟、5分钟和15分钟的系统平均负载。
        1核，标准负载1，超过1，表示CPU超负荷，需要优化；
        实际中 系统负载均值应不高于 cpu 核心数
        
> b.vmstat 查看系统负载

        b.1 procs
            r 列表示运行和等待cpu时间片的进程数，如果长期大于1，说明cpu不足，需要增加cpu。
            b 列表示在等待资源的进程数，比如正在等待I/O、或者内存交换等。 
        b.2 cpu 表示cpu的使用状态
            us 列显示了用户方式下所花费 CPU 时间的百分比。us的值比较高时，
               说明用户进程消耗的cpu时间多，但是如果长期大于50%，需要考虑优化用户的程序。
            sy 列显示了内核进程所花费的cpu时间的百分比。这里us + sy的参考值为80%，
               如果us+sy 大于 80%说明可能存在CPU不足。
            wa 列显示了IO等待所占用的CPU时间的百分比。这里wa的参考值为30%，
               如果wa超过30%，说明IO等待严重，这可能是磁盘大量随机访问造成的，
               也可能磁盘或者磁盘访问控制器的带宽瓶颈造成的(主要是块操作)。
            id 列显示了cpu处在空闲状态的时间百分比
     
        b.3 system 显示采集间隔内发生的中断数
            in 列表示在某一时间间隔中观测到的每秒设备中断数。
            cs 列表示每秒产生的上下文切换次数，
               如当 cs 比磁盘 I/O 和网络信息包速率高得多，都应进行进一步调查。

        b.4 memory
            swpd 切换到内存交换区的内存数量(k表示)。如果swpd的值不为0，
                 或者比较大，比如超过了100m，只要si、so的值长期为0，系统性能还是正常
            free 当前的空闲页面列表中内存数量(k表示)
            buff 作为buffer cache的内存数量，一般对块设备的读写才需要缓冲。
            cache: 作为page cache的内存数量，一般作为文件系统的cache，
                   如果cache较大，说明用到cache的文件较多，如果此时IO中bi比较小，说明文件系统效率比较好。

        b.5 swap
            si 由内存进入内存交换区数量。
            so由内存交换区进入内存数量。
            
> c. iostat 查看磁盘负载

        iostat -x 1 10
        每隔2秒统计一次磁盘IO信息，
        直到按Ctrl+C终止程序，
        -d 选项表示统计磁盘信息，
        -k 表示以每秒KB的形式显示，
        -t 要求打印出时间信息，
        2 表示每隔 2 秒输出一次。
        第一次输出的磁盘IO负载状况提供了关于自从系统启动以来的统计信息。
        随后的每一次输出则是每个间隔之间的平均IO负载状况。
        
        rrqm/s: 每秒进行 merge 的读操作数目。即 delta(rmerge)/s
        wrqm/s: 每秒进行 merge 的写操作数目。即 delta(wmerge)/s
        r/s: 每秒完成的读 I/O 设备次数。即 delta(rio)/s
        w/s: 每秒完成的写 I/O 设备次数。即 delta(wio)/s
        rsec/s: 每秒读扇区数。即 delta(rsect)/s
        wsec/s: 每秒写扇区数。即 delta(wsect)/s
        rkB/s: 每秒读K字节数。是 rsect/s 的一半，因为每扇区大小为512字节。(需要计算)
        wkB/s: 每秒写K字节数。是 wsect/s 的一半。(需要计算)
        avgrq-sz: 平均每次设备I/O操作的数据大小 (扇区)。delta(rsect+wsect)/delta(rio+wio)
        avgqu-sz: 平均I/O队列长度。即 delta(aveq)/s/1000 (因为aveq的单位为毫秒)。
        await: 平均每次设备I/O操作的等待时间 (毫秒)。即 delta(ruse+wuse)/delta(rio+wio)
        svctm: 平均每次设备I/O操作的服务时间 (毫秒)。即 delta(use)/delta(rio+wio)
        %util: 一秒中有百分之多少的时间用于 I/O 操作，或者说一秒中有多少时间 I/O 队列是非空的。即 delta(use)/s/1000 (因为use的单位为毫秒)

        如果 %util 接近 100%，说明产生的I/O请求太多，I/O系统已经满负荷，该磁盘可能存在瓶颈。
        
        idle小于70% IO压力就较大了,一般读取速度有较多的wait.

        同时可以结合vmstat 查看查看b参数(等待资源的进程数)和wa参数(IO等待所占用的CPU时间的百分比,高过30%时IO压力高)

> d. 获取核心的处理器

        在 Linux 下，可以使用

          cat /proc/cpuinfo

        获取你系统上的每个处理器的信息。如果你只想得到数字，那么就使用下面的命令：

          grep 'model name' /proc/cpuinfo | wc -l

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


# thefuck: 自动纠正前一个命令的拼写错误

    https://github.com/nvbn/thefuck
    
    thefuck是一个使用Python编写的开源小工具，它可以自动纠正前一个命令的拼写错误。
    这个工具非常酷，尤其对于常常使用命令行的童鞋。
    
    安装
        $ sudo apt install python3-dev python3-pip
        $ sudo -H pip3 install thefuck

    创建一个别名alias，首先编辑bashrc配置文件：
    $ vim ~/.bashrc
    
    在文件尾加入一行：
    eval "$(thefuck --alias fuck)"
    
    使生效：
    $ source ~/.bashrc
    
    sdo vim /etc/passwd
    >>> sdo comand not found
    >>> fuck
    sudo vim /etc/passwd[enter or ctrl c]
    直接回车执行纠正过的命令。
    
https://github.com/nvbn/thefuck/blob/master/README.md





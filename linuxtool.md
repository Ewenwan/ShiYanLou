[ Linux基础知识 总结](https://blog.csdn.net/cyningsun/article/category/1269142)

#  1、Clion的简单介绍
    CLion是以IntelliJ为基础，专为开发C及C++所设计的跨平台IDE，可以在Windows、Linux及MacOS使用，这里我是在ubuntu 16.0.4基础上安装。
## 2、下载 Linux版Clion的.tar.gz的压缩包 
    wget https://download.jetbrains.8686c.com/cpp/CLion-2016.2.2.tar.gz
## 3、解压CLion-2016.2.2.tar.gz到当前文件夹
    tar -zxvf CLion-2016.2.2.tar.gz  
## 4、运行clion.sh 安装脚本
    cd clion-2016.2.2/bin/  
    ./clion.sh  
## 5、打开workspace
    第一次打开点击ok
    然后同意就行
    CLion是收费的，可以选择试用30天，搭建了一个破解的服务器，
    地址是：
    http://bash.pub:1017 可能连接不上
    选择自己喜欢的主题。
    然后一直往下走，插件先不安装。
    然后新建一个C项目
## 6、激活码激活
    获取  激活码  http://idea.lanyus.com/

## Kdevelop 安装
在终端输入下面指令安装Kdevelop(要连接网络)

先安装cmake

    sudo apt-get install cmake

安装kdevelop
    
    sudo apt-get install kdevelop



# 二、nfs服务器搭建
    多台电脑之间创建共享文件夹
## nfs服务器端配置
### 1、安装nfs服务
    1.1-安装Ubuntu nfs服务器端：
        sudo apt-get install nfs-kernel-server
        (在安装nsf-kernel-server的时候，也会安装nfs-commom。如果没有安装这个软件包，则要执行1.2中的命令了)
    1.2-安装nfs的客户端：
    sudo apt-get install nfs-common
### 2、修改配置文件   
    sudo vim /etc/exports

    修改添加内容如下：
    /home *(rw,sync,no_root_squash)
    
    更长 ：
    /home/ewenwan/nfs *(insecure,rw,sync,nohide,no_root_squash,no_subtree_check)

    /home   ：共享的目录
    *       ：指定哪些用户可以访问
                *  所有可以ping通该主机的用户
                192.168.1.*  指定网段，在该网段中的用户可以挂载
                192.168.1.12 只有该用户能挂载
    (ro,sync,no_root_squash)：  权限
            ro : 只读
            rw : 读写
            sync :  同步
            no_root_squash: 不降低root用户的权限
        其他选项man 5 exports 查看
    前面那个目录是与nfs服务客户端共享的目录，
    *代表允许所有的网段访问（也可以使用具体的IP）
    rw：挂接此目录的客户端对该共享目录具有读写权限
    sync：资料同步写入内存和硬盘
    no_root_squash：客户机用root访问该共享文件夹时，不映射root用户。
    root_squash：客户机用root用户访问该共享文件夹时，将root用户映射成匿名用户
    no_subtree_check：不检查父目录的权限。

    2.2-nfs是一个RPC程序，使用它前，需要映射好端口，通过rpcbind 设定：
        sudo service rpcbind restart restart
    2.3-重启nfs服务：
        sudo service restartnfs-kernel-server restart

        sudo /etc/init.d/nfs-kernel-server restart
    2.4-测试运行以下命令来显示一下共享出来的目录：
        showmount -e

    !!!需要修改 远程挂载目录 /home/ewenwan/nfs的权限   chmod 777  /home/ewenwan/nfs   !!!!!!!
    
    ## 客户端访问配置


### window  安装 nfs 客户端
    win10 控制面板 程序和功能 启用或关闭Windows功能 勾选NFS服务 确定后系统进行自动安装
    打开 cmd 
    
    mount \\\10.65.223.171\目录 x:
    
    挂载远程主机的 目录 到 本机上 的x盘

    挂载 umount x:
### 客户端 无权 限访问问题
[参考解决](https://jingyan.baidu.com/article/c910274bfd6800cd361d2df3.html)

    mount查看 客户端设置的访问权限
    打开注册表编辑器 regedit 
    HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\ClientForNFS\CurrentVersion\Default
    下新增两项 DWORD(32位)值D 
    AnonymousUid = 0
    AnonymousGid = 0

## linux 系统之间挂载共享目录

在工作主机下:
  
  mount -t nfs 目标机ip:目标机目标目录 工作机目录 -o nolock
  
  例如:
  
     mount -t nfs 10.1.110.123:/home/ws /home/wanyouwen -o nolock
     
     就可以在 当前主机的 /home/wanyouwen 目录访问 远程主机10.1.110.123  下的目录 /home/ws
    



### winscp 访问 远程服务器文件 ftp协议
[百度参考](https://jingyan.baidu.com/article/d621e8da79ee562865913fce.html)

### 远程 命令行登陆服务器 shell
[Xmanager 服务器远程控制程序 ](https://www.netsarang.com/products/xme_overview.html)

[putty ssh远程登录 命令行shell](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)
 
# 三、 linux配置
[ubuntu大法-快速翻墙装驱动配置开发环境](https://github.com/DragonFive/myblog/blob/master/source/_posts/nvidiaDriverInstall.md)


Ubuntu14.04升级CMake：

    Ubuntu14.04 默认CMake版本是2.8.X 当我们需要更高级版本的时候就需要升级CMake。具体步骤如下： 
        通过PPA安装：

    sudo add-apt-repository ppa:george-edison55/cmake-3.x
    sudo apt-get update
    sudo apt-get upgrade 
    cmake --version1234

    编译安装步骤如下：



    sudo apt-get install build-essential //如果系统已安装build-essential则不需要执行此步骤
    wget http://www.cmake.org/files/v3.2/cmake-3.2.2.tar.gz
    tar xf cmake-3.2.2.tar.gz
    cd cmake-3.2.2
    ./configure
    make


# 四、Linux环境编程
[博客专栏](https://blog.csdn.net/column/details/tennysonsky-linux.html)
## ubuntu下添加新用户、解决远程登录问题
###  添加新用户
    添加新用户

    sudo useradd ewenwan -d /home/ewenwan -m
    # 第一个ewenwan，指用户名
    # -d /home/ewenwan 指定用户登录系统时的主目录，宿主目录
    # -m 如果/home/ewenwan 路径不存在，自动创建
    
    查看新建的用户
    cat /etc/passwd | grep ewenwan
    
    给用户设置密码
    sudo passwd ewenwan
    
    新用户增加sudo权限
    修改/etc/sudoers权限
    sudo chmod u+w /etc/sudoers
    #增加写权限
    
    通过编辑/etc/sudoers文件 也可以实现
    sudo vi /etc/sudoers
![](https://img-blog.csdn.net/20180104160715584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGVubnlzb25za3k=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    
    root ALL=(ALL:ALL) ALL
    ewenwan ALL=(ALL:ALL) ALL
    
    
    恢复原来/etc/sudoers权限
    sudo chmod u-w /etc/sudoers
    
### 解决远程登录问题
    
    修改/etc/passwd文件
    sudo vim /etc/passwd
    找到新增的用户ewenwan，添加如下内容： 
    
    ewenwan:x:1001:1001::/home/ewenwan:/bin/bash
    
# 命令行 翻译工具
    安装 sudo pip install dict-cli
    使用 dict test 

# linux下 插件按caj
    
    安装 火狐扩展插件 Greasemonkey https://addons.mozilla.org/zh-CN/firefox/addon/greasemonkey/
     Greasemonkey 安装脚本 cnki-pdf-special.user.js
    
    http://kns.cnki.net/KCMS/****  >>>  http://www.cnki.net/KCMS/****  就会出现pdf版本


# linux下图像工具
    1. 截图工具 gnome-screenshot  
       PrintScreen键：整个界面  
       Alt+PrintScreen键:当前窗口 
       Shift+PrintScreen键：区域截图
       打开软件：下拉菜单截图，延时截图
       
    2. Linux系统下的Photoshop 位图处理软件GIMP
       sudo add-apt-repository ppa:otto-kesselgulasch/gimp
       sudo apt-get update
       sudo apt-get install gimp
       
       卸载： 
       sudoapt-get install ppa-purge
       sudo ppa-purge ppa:otto-kesselgulasch/gimp
     
     3. 矢量图绘制软件Inkscape  与Illustrator、Freehand、CorelDraw、Xara X 等其他软件相似
       在矢量图绘制软件中，画框就是画框，画完框后可以改变这个框的边线颜色及样式，
       可以改变它的填充颜色及样式，可以拖动它的位置，可以改变它的大小，但是在位图处理软件中，
       就得换一种思考模式，在位图处理软件中，要画框只能先建立一个矩形的选区，然后对这个选区进行描边和填充，
       还可以对选取进行羽化，这样可以得到一个模模糊糊的框框，一旦描边和填充结束，则这个框框就再不能更改。
       所以Inkscape适合做添加框框、文字这样的工作，
       而GIMP适合做对选区羽化、非规则区域切图、添加阴影和发光这样的工作。
       
       sudo add-apt-repository ppa:inkscape.dev/stable
       sudo apt update
       sudo apt-get install inkscape
     
     4. 示意图绘制软件Dia
        wget wget http://sourceforge.net/projects/dia-installer/files/dia/0.97.2/dia-0.97.2.tar.xz/download

        tar xvf dia-0.97.2.tar.xz
        cd dia-0.97.2
        ./configure --enable-gnome

        Configuration:
            Source code location:    .
            Compiler:        gcc
            Gnome support:                yes
            Python support:                no
            Libart support (PNG export):        yes
            Cairo support (PNG, PDF, print):    yes
            Dia Python bindings with SWIG        no
            WMF plug-in with libEMF:                no
            html doc:                               no
            pdf and ps doc:                         no

        Now type make to build dia...
        # make && make install
        直接在命令行输入
        #dia
        打开画图软件dia
        
      5. 建模工具Umbrello
        mkdir -p $HOME/src
        cd $HOME/src
        git clone git://anongit.kde.org/umbrello  
        cd umbrello
        mkdir build
        cd build


# linux 命令行查看 十六进制
    https://github.com/sharkdp/hexyl
    
    安装: 
    wget "https://github.com/sharkdp/hexyl/releases/download/v0.4.0/hexyl_0.4.0_amd64.deb"
    sudo dpkg -i hexyl_0.4.0_amd64.deb


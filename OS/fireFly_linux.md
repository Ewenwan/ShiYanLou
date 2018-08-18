# firefly 嵌入式板子安装Linux系统
    http://developer.t-firefly.com/thread-10874-1-1.html
    ###########################################
    3.14内核系统安装 firefly-3.14-kernel 
    ################################################


# 一、安装内核开发包
    Linux内核编译需要主机安装一些开发包，具体的开发工具有那些可以通过包名自行补脑。
    安装过程中需要留意开发包是否都有安装成功，若有开发包安装失败可以单独再重新安装。

    命令行：
    sudo apt-get install build-essential lzop libncurses5-dev libssl-dev

    注释：
     build-essential 提供编译程序必须软件包的列表信息 头文件在哪 才知道库函数在哪
     lzop            是开源的压缩算法框架
     libncurses5-dev 系统的必备库
      g++:这是一个编译器包 libssl-dev:这是开发工具库包 apache:这是一个web服务器包

    如果使用的是 64 位的 Ubuntu，还需要安装：
    命令行：
    sudo apt-get install libc6:i386      //64位  c环境

    Ubuntu 13.10/14.04 软件包安装：
       sudo apt-get install git-core gnupg flex bison gperf libsdl1.2-dev \
     libesd0-dev libwxgtk2.8-dev squashfs-tools build-essential zip curl \
     libncurses5-dev zlib1g-dev pngcrush schedtool libxml2 libxml2-utils \
     xsltproc lzop libc6-dev schedtool g++-multilib lib32z1-dev lib32ncurses5-dev \
     lib32readline-gplv2-dev gcc-multilib libswitch-perl libudev-dev


    // 注意 libudev-dev libudev0 libudev1为后添加
    ./upgrade_tool
    ./upgrade_tool: error while loading shared libraries: libudev.so.1: cannot open shared object file: No such file or directory
    sudo find / -name "libudev.so*"
    >>>> /lib/x86_64-linux-gnu/libudev.so.1
    cd /lib/x86_64-linux-gnu/  
    sudo ln -s libudev.so.1 libudev.so.0

    # cat /etc/ld.so.conf
    include ld.so.conf.d/*.conf
    # echo "/usr/local/lib" >> /etc/ld.so.conf  # 将 /usr/local/lib下也加入到 库配置文件

    # ldconfig # 默认会将 /lib和/usr/lib 下的动态库加入到搜索路径



        Ubuntu 12.04 软件包安装：
    sudo apt-get install git gnupg flex bison gperf build-essential \
     zip curl libc6-dev libncurses5-dev:i386 x11proto-core-dev \
     libx11-dev:i386 libreadline6-dev:i386 libgl1-mesa-glx:i386 \
     g++-multilib mingw32 tofrodos gcc-multilib ia32-libs\
     python-markdown libxml2-utils xsltproc zlib1g-dev:i386


        安装 JDK 6：  双系统android 编译工具
       sudo add-apt-repository ppa:webupd8team/java
       sudo apt-get update
       sudo apt-get install oracle-java6-installer

            system.img ：Android 的 system 分区映像，ext4 文件系统格式。
            boot.img ：Android 的初始文件映像，负责初始化并加载 system 分区
            misc.img ：misc 分区映像，负责启动模式切换和急救模式的参数传递。
            recovery.img ：急救模式映像。



## 二、安装交叉编译工具链
        纯Linux开发的朋友可以通过Github单独下载交叉编译工具链，内核编译用到arm-eabi-4.6交叉编译工具链，下载链接为：
    http://pan.baidu.com/s/1sl7v1EX
        下载好放到指定目录~/UbuntuDev/toolchain后解压：
    mkdir -p ~/UbuntuDev/toolchain
    cd ~/UbuntuDev/toolchain
    tar -xvf arm-eabi-4.6.tar.gz
        如果是android开发者，可以直接按android SDK编译的方法编译SDK里的内核，android SDK里自带交叉编译工具链等。

    或者命令行安装
        安装 ARM 交叉编译工具链和编译内核相关软件包：
    sudo apt-get install gcc-arm-linux-gnueabihf \
     lzop libncurses5-dev \
     libssl1.0.0 libssl-dev


## 三、下载Firefly Linux内核源代码
        从Firefly github仓库下载kernel源代码：
          cd ~/UbuntuDev
          git clone https://github.com/T-Firefly/firefly-3.14-kernel
        至此主机安装了开发包，同时内核源码和交叉编译工具链都准备完毕。可谓是万事具备，只欠编译！GO！


## 四、编译内核
          进入内核源码目录：
      cd ~/UbuntuDev/firefly-3.14-kernel/
          设置ARCH环境变量，告诉编译器目标平台的架构：
      export ARCH=arm
          设置交叉编译器arm-eabi-gcc 根文件路径(与自己安装路径相关)：
      export CROSS_COMPILE=./../toolchain/arm-eabi-4.6/bin/arm-eabi-
          载入Firefly RK3288开发板内核配置：
      make firefly-rk3288_defconfig

      一次性编译内核文件
         make -j8 firefly-rk3288.img

        编译正确 log信息
                OBJCOPY arch/arm/boot/zImage                   # 内核二进制文件
                Kernel: arch/arm/boot/zImage is ready
                Image:  kernel.img is ready                    # 内核映像。  /boot/zImage + resource.img
                Pack to resource.img successed!                
                     # 资源映像，内含开机图片和内核的设备树信息。 资源镜像文件(应该由.dtb设备文件得到)
                Image:  resource.img (with rk3288-firefly.dtb logo.bmp) is ready


      也可分开执行 ：
        1） 编译zImage，zImage是可以加载执行的二进制代码：
            make -j4 zImage
           正常编译结束后，会在arch/arm/boot/目录生成zImage文件：
        2） 编译DTB文件。dtb是.dts被DTC编译后的二进制格式的Device Tree设备数 描述，可由Linux内核解析。
           通常在我们为电路板制作NAND、SD启动image时，会为.dtb文件单独留下一个很小的区域以存放之，
           之后bootloader在引导kernel的过程中，会先读取该.dtb到内存。
           make firefly-rk3288.dtb
        3） 合并 生成kernel.img文件
           cat arch/arm/boot/zImage arch/arm/boot/dts/firefly-rk3288.dtb > zImage-dtb //两个文件放在一起
           mkkrnlimg -a zImage-dtb kernel.img
           编译出来的kernel.img文件和一次性编译内核文件步骤编译出来的firefly-rk3288.img  是一样的

         内核文件包含    资源镜像文件(设备数文件得来) 和 内核二进制文件
                kernel.img  =  /boot/zImage + resource.img

         即 
        ----cd ~/proj/linux-rockchip/kernel
         export ARCH=arm
         export CROSS_COMPILE=./../toolchain/arm-eabi-4.6/bin/arm-eabi-
         make firefly-rk3288_defconfig
         make -j4 zImage
         make rk3288-firefly.dtb
         cat arch/arm/boot/zImage arch/arm/boot/dts/rk3288-firefly.dtb > zImage-dtb
         mkkrnlimg zImage-dtb kernel.img    
         # 编译并安装模块
         make -j4 modules
         [ -d install_mod ] && rm -rf install_mod
         make INSTALL_MOD_PATH=$PWD/install_mod modules_install
         # 复制 install_mod 目录里的文件到根文件系统

           内核模块是需要拷到根文件系统中即可：

              rsync -av ./modules_install/ /path/to/your/rfs/
             也可以远程拷贝到开发板的根文件系统中，这需要开发板可以通过 ssh 远程连接：

              rsync -av ./modules_install/ root@开发板IP:/
             最后清理一下模块安装目录（该目录含有链接，会影响 SDK 的编译）：

      rm -rf ./modules_install

	 
## 五、编译 模块
      编译并安装模块，先在kernel目录创建modules_install目录，用于存放编译好的内核模块。
      cd kernel
      mkdir modules_install
      make INSTALL_MOD_PATH=./modules_install modules_install

      现在的kernel和modules都已经编译完成，但还不能直接烧录到开发板运行



## 六、编译初始RAM磁盘（initrd） 
       初始RAM磁盘（initrd）是在实际根文件系统可用之前挂载到系统中的一个  初始根文件系统
       initrd与内核绑定在一起，并作为内核引导过程的一部分进行加载
        内核会将这个initrd文件作为其两阶段引导过程的一部分来加载模块，这样才能稍后使用真正的文件系统，
       并挂载实际的根文件系统。
       initrd中包含了实现这个目标所需要的目录和可执行程序的最小集合，
       例如将内核模块加载到内核中所使用的 insmod工具。
       在桌面或服务器Linux系统中，initrd是一个临时的文件系统。其生存周期很短，只会用作到真实文件系统的一个桥梁。

     cd ~/UbuntuDev/firefly-3.14-kernel
     git clone https://github.com/TeeFirefly/initrd.git
     make -C initrd
     编译正确 会 在~/UbuntuDev/firefly-3.14-kernel 下生成  initrd.img      
       // 注意 由 git clone https://github.com/TeeFirefly/initrd.git 上生成的 initrd.img 
          可能有问题 导致系统启动不了  文件系统挂载问题
       可以由官方 的固件 rk3288_Ubuntu_1604_2016_09031544_Beta.img  解包后得到  linux-boot.img 
        再将  linux-boot.img 解包得到 initrd.img (实际解包后的名称可能不同  linux-boot.img-ramdisk.gz )
      ---- 七
       ./mkbootimg --kernel arch/arm/boot/zImage --ramdisk linux-boot.img-ramdisk.gz --second resource.img  -o linux-boot.img

 
## 七、 打包   打包内核和RAM磁盘等  成 linux-boot.img 
        解包 mkbootimg 工具
        tar -xvf mkbootimg.tar.gz ~/UbuntuDev/firefly-3.14-kernel
        cd ~/UbuntuDev/firefly-3.14-kernel
        sudo chmod +x mkbootimg 添加执行权限 


                  内核镜像                   初始ram文件系统              资源镜像
      ./mkbootimg --kernel arch/arm/boot/zImage --ramdisk initrd.img --second resource.img  -o linux-boot.img

    Firefly官方发布的Ubuntu系统都是只有boot分区的，所以要将 kernel(zImage+ .dtb)和 initrd 打包成 linux-boot.img，在烧录到指定分区才能上kernel飞~~


###################################################
# 4.4内核系统编译  
####################################################
## 一 交叉编译器编译环境 
      1） arm-eabi-gcc

          http://pan.baidu.com/s/1sl7v1EX
          下载好放到指定目录~/UbuntuDev/toolchain后解压：
          mkdir -p ~/UbuntuDev/toolchain
          cd ~/UbuntuDev/toolchain
          tar -xvf arm-eabi-4.6.tar.gz
          编译  linux 内核 kernel ,包含在 AndroidSDK_5.1 里 AndroidSDK_5.1/prebuilts/gcc/linux-x86/arm
          arm-eabi-4.6  arm-eabi-gcc 版本为 4.6.

      2） arm-linux-gnueabihf-gcc
          sudo apt-get install gcc-arm-linux-gnueabihf lzop libncurses5-dev libssl1.0.0 libssl-dev   
          编译linux 平台上 应用程序 applications 的编译器  如果只需要编译内核，该项不需要

## 二 编译4.4内核
      1） 下载 4.4内核源代码
          git clone https://gitlab.com/TeeFirefly/linux-kernel.git

        最新内核应该是：https://github.com/rockchip-linux/kernel.git
    此内核源码能够编译成功，但是不支持HDMI和VGA输出，需要自己修改dts，关键是还要兼容arm64，dts修改没搞成功

      2） 检查 切换
          git checkout firefly
      3） 配置编译环境
                   // 设置ARCH环境变量，告诉编译器目标平台的架构：
            export ARCH=arm 
                  // 设置交叉编译器arm-eabi-gcc 根文件路径(与自己安装路径相关)：
                 //export CROSS_COMPILE=<path to arm-eabi-gcc root dir>/bin/arm-eabi-
            export CROSS_COMPILE=./../toolchain/arm-eabi-4.6/bin/arm-eabi-
                        // 载入Firefly RK3288开发板内核配置：
            make rockchip_linux_defconfig
                        // 一次性编译内核文件
            make rk3288-firefly.img 
      4) 编译正确 log信息
              OBJCOPY arch/arm/boot/zImage
              Kernel: arch/arm/boot/zImage is ready     # 内核二进制文件
              Image:  kernel.img is ready
              Pack to resource.img successed!           # dtb设备文件得到 资源镜像文件
              Image:  resource.img (with rk3288-firefly.dtb logo.bmp) is ready

## 三 编辑初始RAM磁盘（initrd） 
       1） 下载源文件 
           git clone https://github.com/TeeFirefly/initrd.git
       2） 切换到 4.4内核镜像
           git checkout for-kernel_4.4
       3） 编译
           make -C initrd
       4） 编译正确 会生成 initrd.img

      // 注意 由 git clone https://github.com/TeeFirefly/initrd.git 上生成的 initrd.img 可能有问题
       可以由官方 的固件 rk3288_Ubuntu_1604_2016_09031544_Beta.img  解包后得到  linux-boot.img 
        再将  linux-boot.img 解包得到 initrd.img (实际解包后的名称可能不同  linux-boot.img-ramdisk.gz )

      进行四
       ./mkbootimg --kernel arch/arm/boot/zImage --ramdisk linux-boot.img-ramdisk.gz --second resource.img -o linux-boot.img


	
########################
##  四  合并 初始RAM磁盘 initrd.img、 内核 zImage 以及 设备数资源镜像resource.img  >>> linux-root.img
        mkbootimg  打包工具需要下载  

         cd ~/UbuntuDev/kernel_4.4/
         ./mkbootimg --kernel arch/arm/boot/zImage --ramdisk initrd.img --second resource.img -o linux-boot.img

       linux-boot.img 生成后将会在 第六步使用

######################
## 五 制作根文件系统 rootfs of ubuntu16.04   linux-rootfs.img将会在 第六步使用
         1） 下载  ubuntu16.04最小系统
           mkdir linux-rootfs
         cd linux-rootfs
           wget -c http://cdimage.ubuntu.com/ubuntu-base/releases/16.04/release/ubuntu-base-16.04-core-armhf.tar.gz
         2） 解压
            mkdir ubuntu
            sudo tar -xpf ubuntu-base-16.04-core-armhf.tar.gz -C ubuntu
         3） 安装仿真环境 执行根文件系统   要对根文件系统做一些配置修改，copy qemu
            sudo apt-get install qemu-user-static
          cd ubuntu
            sudo cp /usr/bin/qemu-arm-static usr/bin/
         4） 复制本地 DNS 配置 到 文件系统  拷贝PC主机端的dns配置，因为待会安装程序时要用网络：
            sudo cp -b /etc/resolv.conf  etc/resolv.conf
         5） 为文件系统 添加Ubuntu 软件源
            在 linux-rootfs/ubuntu/etc/apt/sources.list 尾部下添加
          sudo vim etc/apt/source.list   编辑
            deb http://ports.ubuntu.com/ubuntu-ports/ xenial main universe
            deb-src http://ports.ubuntu.com/ubuntu-ports/ xenial main universe
         6） 使用挂载脚本 挂载 核心文件系统
           下载  http://developer.t-firefly.com/forum.php?mod=misc&action=attachcredit&aid=3053&formhash=43f1c44c
           https://pan.baidu.com/s/1pLo36jD

            tar -xvf ch-mount.sh.tar.gz
            sudo chmod +x ch-mount.sh  // 添加执行权限
         7） 挂载文件系统  不要使用 sudo 权限
            cd ~/UbuntuDev/linux-rootfs
            ./ch-mount.sh -m ubuntu
           可以看到执行后，终端显示的用户名发生了变化，用ls可以看到文件系统也有变化，
         此终端里的根文件系统已经切换到binary目录里的根文件系统。
         可以在终端里做安装程序等操作了，就像进入了一个新系统，所以我也叫作是模拟运行根文件系统。


            已经 boot身份 进入仿真的 bash 脚本环境，可以为文件系统安装想要的
             a)  安装网络工具 network tools 使能 ifconfig 命令行命令
                 apt-get install net-tools
             b)  安装 vim编辑器  SSH远程登录服务器
                 apt-get install vim openssh-server git
             c)  安装会话  图像界面
              apt-get install ubuntu-session xubuntu-desktop
                   // Please wait to be done.
                   // Start dbus server to solve the error:
                  // Failed to connect to socket /var/run/dbus/system_bus_socket: No such file or directory
                /etc/init.d/dbus start
             d）使能自动连接到网络 network
                  #echo auto eth0 > /etc/network/interfaces.d/eth0
                  #echo iface eth0 inet dhcp >> /etc/network/interfaces.d/eth0
             e) 启动图像界面 startup desktop
                update-alternatives --config x-session-manager
                  选择:
                      desktop | x-session
                     ---------------------
                      xubuntu | startxfce4
                     ---------------------     
                      lubuntu | startlxde
             f） 设置默认登录界面 login screen
                  # dpkg-reconfigure lightdm

             g)  设置登录的用户名和密码  user and password to login
                  #useradd -s '/bin/bash' -m -G adm,sudo ubuntu
             //修改ubuntu用户密码，回车后按提示输入两次密码：
                  #passwd ubuntu
             // 修改root帐号的密码，回车后按提示输入两次密码
                  #passwd root
             h) 退出仿真 bash 脚本环境 simulate bash
                  #exit
       8） 挂载文件系统后 退出挂载   unmount the rootfs 退出根文件系统
           ./ch-mount.sh -u ubuntu

       9） 创建新的镜像文件linux-rootfs.img 用于 保存 文件系统  rootfs
           用dd工具创建镜像文件：
         在  ~/UbuntuDev/linux-rootfs 下 
           dd if=/dev/zero of=linux-rootfs.img bs=1M count=4096
         格式化镜像文件，并加入linuxroot卷标：
           sudo mkfs.ext4 -F -L linuxroot linux-rootfs.img

       10）挂载 镜像文件ubuntu.img 复制 rootfs 到里面
           挂载镜像并往里面拷入修改后的文件系统：
           mkdir ubuntu-mount
           sudo mount linux-rootfs.img ubuntu-mount // 将空文件img 挂在到 ubuntu-mount
           sudo cp -rfp ubuntu/* ubuntu-mount       
          // 将之前 制作的文件系统 linux-rootfs/ubuntu 内容全部拷贝到 ubuntu-mount  
          // 即将文件系统导入到linux-rootfs.img 

       11） 退出挂载mnt  重置大小  缷载镜像：
               sudo umount ubuntu-mount
          //检查并修复ubuntu.img镜像的文件系统
               /sbin/       e2fsck -p -f linux-rootfs.img                 !!!!
          //减小ubuntu.img镜像文件的大小：
               /sbin/       resize2fs -M linux-rootfs.img                 !!!!
           执行上面两命令后，linux-rootfs.img瘦身不少，可以用来烧录了

       12） 现在 linux-rootfs.img文件系统已经做好了 将要被用在 第六步

#############
## 六  制作 update.img
          使用工具 
          In AndroidSDK_5.1, the tools is avaiable:
          AndroidSDK_5.1/RKTools/linux/Linux_Upgrade_Tool_v1.2/rockdev
         或者 https://github.com/rock64-android/RKTools/tree/rk/linux/Linux_Pack_Firmware/rockdev
          步骤：
          1） 复制 rockdev 到任何地方
          2） 复制 linux-boot.img 和 linux-rootfs.img 到 rockdev/Image 文件夹下
          3） 修改部分参数和包  可以直接使用 paramter  和 package-file
              // Firefly-rk3288_Ubuntu_1604_2016_10181203_Beta.img 官方修改更新 包内的
              mv Firefly-rk3288_Ubuntu_1604_2016_10181203_Beta.img update.img
              sudo chmod +x unpack.sh
              ./unpack.sh     
          // 将官方固件解包  包含                                        起始地址
      boot	    linux-boot.img          内核 初始文件系统  资源镜像文件   0x00002000  到  0x00012000
      linuxroot	linux-rootfs.img        标准文件系统                      0x00014000   ------
            package-file            固件包信息                       ------------    
            paramter                各种参数                          0x00000000
      bootloader	RKLoader.bin            bootloader 引导分区               0x00000000
            RESERVED                保留  分区                        0x00012000       0x00014000



      paramter文件：
      FIRMWARE_VER:5.0.0
      MACHINE_MODEL:rk3288
      MACHINE_ID:007
      MANUFACTURER:RK3288
      MAGIC: 0x5041524B
      ATAG: 0x60000800
      MACHINE: 3288
      CHECK_MASK: 0x80
      PWR_HLD: 0,0,A,0,1
      #KERNEL_IMG: 0x62008000
      #FDT_NAME: rk-kernel.dtb
      #RECOVER_KEY: 1,1,0,20,0
      CMDLINE:earlyprintk=ttyS2,115200 console=tty0 console=ttyS2,115200 androidboot.selinux=permissive androidboot.hardware=rk30board androidboot.console=ttyS2 root=/dev/mmcblk0p3 linuxroot rw rootfstype=ext4 init=/sbin/init initrd=0x62000000,0x00800000 mtdparts=rk29xxnand:0x00010000@0x00002000(boot),0x00002000@0x00012000(backup),-@0x00014000(linuxroot)

      package-file文件(根据实际编译生成的img 修改第二行)：
      package-file	package-file
      bootloader	RKLoader.bin
      paramter	paramter
      boot		linux-boot.img        挂载点依次为  root=/dev/mmcblk0p1
      backup		RESERVED                            root=/dev/mmcblk0p2
      linuxroot	linux-rootfs.img                    root=/dev/mmcblk0p3

          4） 运行mkupdate.sh 打包生成 固件 update.img
              sudo chmod +x mkupdate.sh
             ./mkupdate.sh

      linux  mkupdate.sh

      #!/bin/bash
      pause()
      {
      echo "Press any key to quit:"
      read -n1 -s key
      exit 1
      }
      echo "start to make update.img..."
      if [ ! -f "parameter" -a ! -f "parameter.txt" ]; then # parameter.txt文件
        echo "Error:No found parameter!"
        exit 1
      fi
      if [ ! -f "package-file" ]; then                                  #  package-file 文件 在 同级目录下
        echo "Error:No found package-file!"
        exit 1
      fi
      ./afptool -pack ./ update_src.img || pause                            # 打包
      ./rkImageMaker -RK322H RKLoader.bin update_src.img update.img -os_type:androidos || pause # 转换
      echo "Making update.img OK."
      rm update_src.img
      #echo "Press any key to quit:"
      #read -n1 -s key
      exit $?


      注意以上文件目录需要调整！！！！！

	   
##########
## 七、 下载 update.img至 board 

          1> 同时按住 恢复按键 recovery button 和 重置按键reset button  

          2> 释放 重置按键reset button 保存按住 恢复按键 recovery button 2 秒以上.

          3> 进入loader模式 下载 update.img
                  $sudo upgrade_tool uf update.img

          4> 等待板子自动重启

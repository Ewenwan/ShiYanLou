# Qemu 架构 硬件模拟器
    Qemu 是纯软件实现的虚拟化模拟器，
    几乎可以模拟任何硬件设备，
    我们最熟悉的就是能够模拟一台能够独立运行操作系统的虚拟机，
    虚拟机认为自己和硬件打交道，
    但其实是和 Qemu 模拟出来的硬件打交道，
    Qemu 将这些指令转译给真正的硬件。
![](https://images2017.cnblogs.com/blog/431521/201711/431521-20171118213522843-322721697.png)
    
    从本质上看，虚拟出的每个虚拟机对应 host 上的一个 Qemu 进程，
    而虚拟机的执行线程（如 CPU 线程、I/O 线程等）对应 Qemu 进程的一个线程。

# 1. 源码下载

    centos：
    sudo apt-get install qemu
    ubuntu：
    sudo yum install qemu -y
    安装包：
    
    $wget http://wiki.qemu-project.org/download/qemu-2.0.0.tar.bz2
    $tar xjvf qemu-2.0.0.tar.bz2
    
    wget https://download.qemu.org/qemu-2.12.0.tar.x
    xz -d qemu-2.12.0.tar.xz
    tar xvf qemu-2.12.0.tar qemu-2.12.0/
    
    Git渠道：
    $git clone git://git.qemu-project.org/qemu.git
# 2. 配置编译及安装
        $cd qemu-2.12.0 //如果使用的是git下载的源码，执行cd qemu
    配置:
        $./configure --enable-kvm --enable-debug --enable-vnc --enable-werror  --target-list="x86_64-softmmu"  x86 64位格式
        $./configure --enable-kvm --enable-debug --enable-vnc --enable-werror  --target-list="i386-softmmu"　　32位格式
    
        configure 脚本用于生成 Makefile，其选项可以用 ./configure --help 查看。
    编译:
        $make -j4　
    安装:
        $sudo make install

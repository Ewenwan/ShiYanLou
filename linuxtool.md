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
    
# nfs服务器搭建
    多台电脑之间创建共享文件夹
## nfs服务器端配置
### 1、安装nfs服务
    sudo apt install nfs-common
### 2、修改配置文件
    sudo vim /etc/exports
    
    修改内容如下：
    /home *(rw,sync,no_root_squash)

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

### 4、重启nfs服务
    sudo /etc/init.d/nfs-kernel-server restart　　
    到此，nfs的服务就搭建好了。 

## 客户端访问配置

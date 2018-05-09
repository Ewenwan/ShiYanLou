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
    # sudo unrar e all.rar -y
    这条命令是将all.rar中的所有文件解压出来


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
    
    

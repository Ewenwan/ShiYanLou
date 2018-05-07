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

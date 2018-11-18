# doxygen
      doxygen是一个自动文档生成工具，根据代码里写的注释，
      自动生成html和latex格式的文档。
      通常是给c++用的，通常是出html格式的，
      你看到的pcl网上文档就是拿doxygen生成的。
      
# 安装与使用=================

      事实上，随意一份代码，都可以拿doxygen生成一个文档，然后在本地看它的代码结构。 我们拿以难读著称的lsd-slam举个例子。
      首先，建个文件夹，把lsd-slam的代码拷下来：
      git clone https://github.com/tum-vision/lsd_slam.git
      
      拷完之后lsd就在你电脑里躺着了。进到package_dir/lsd_slam/lsd_slam_core，
      打开一看？哇，一堆文件。仔细一看？哇，好多类和变量，还有藏着掖着的全局变量，还有SSE指令！
      太多了，没法看，怎么办？别急，我们用doxygen生成一个网页来看。
      网页的好处是可以随处跳转。
      
      于你电脑里可能还没有doxygen，
      我们先来安装一下：
      
       sudo apt-get install doxygen doxygen-gui

      装好了之后，在该文件夹下开一个doc文件夹，把一会儿生成的文档放里头：
       mkdir doc; cd doc

      然后，输入doxywizard调出gui面板。
       doxywizard
      
      step1： 填写文档生成的地址
      step2：主要的是源代码目录 source code directory 
             请把它设到lsd_slam_core/src下，并钩上Scan recursively，表明我们要扫描子文件夹。
      然后， destination directory 输出文件夹设成当前目录./即可，本来我们就在当前目录嘛。
      
      这步做好了，点左边的“Mode”，进入下一个版块。下个版块里会让你选语言，既然咱们用c++，保持默认的就可以了。
　　    Output和Diagrams也没什么好动的。
      
      现在准备工作完成啦！切到Run，点“Run doxygen"，哗啦哗啦，等文档生成完。
      然后点“show html output"，就可以在浏览器里看到文档啦。
      如果你下一次想直接看，直接打开doc/html/index.html就可以了！

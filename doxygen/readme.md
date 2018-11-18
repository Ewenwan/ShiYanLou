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


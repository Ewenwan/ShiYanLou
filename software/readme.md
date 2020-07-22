
# 小巧的软件
   图像处理方面 https://www.cnblogs.com/Imageshop/p/7993319.html#4139486

# svn 软连接

      SVN可以设置文件夹链接，据说1.6之后版本还支持文件链接。
      与HG相比，它不是分布式，速度稍慢，而且每个文件夹产生一个垃圾文件夹.svn，不太好，但它的权限控制功能较完善，有些时候还是会用到。
      今天用了它的文件夹链接功能，记录一下。
      比如：目录结构 a/b，b需要链接到http://abc/svn/Proj/trunk/b/mem
      那么操作方法如下：
      在a目录里任意空白处点鼠标右键，选择TortoiseSVN->Properties，
      然后在属性窗口点new，选择svn:externels，在下面窗口中输入一行：
      b http://abc/svn/Proj/trunk/b/mem
      然后commit，即可。
      注意，必须有http://abc/svn/Proj/trunk/b/mem权限的人，才能取到a目录所有代码。b目录下的内容与mem中的内容是相同

# SVN分支

       创建branch
      在/trunk/MyProject目录上右键，依次选择"TortoiseSVN" -> "Branch/tag..."，在弹出窗口的"To URL"中填入分支的

      地址，在这里目标revision选择HEAD revision，如下图所示，添加log后点击ok分支便建立了。这个操作速度非常快，

      新建的branch在repository中其实只是一个指向trunk某个revision的软连接而已，并没有真的复制文件。


       Check out分支
      右键TestSVN目录选择"TortoiseSVN Update"即可将刚刚建立的分支下载回本地。进入/branches/MyProject目录下你会

      发现其文件结构和/trunk/MyProject一模一样。



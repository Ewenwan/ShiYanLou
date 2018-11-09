# python图像处理之scikit-image基本用法

[参考](https://blog.csdn.net/u012300744/article/details/80083282)

# python有很多的数字图像处理相关的包
     像

        PIL
        Pillow
        OpenCV
        scikit-image
        等等

    其中PIL和Pillow只提供最基础的数字图像处理，功能有限；

    opencv实际上是一个c++库，只是提供了python接口，

    scikit-image是基于scipy的一款图像处理包，它将图片作为numpy数组进行处理，
    正好与matlab一样，因此，我们最终选择scikit-image进行数字图像处理。
# 安装 
     sudo pip install scikit-image      python2
     sudo pip3 install scikit-image     python3
     
          scikit-image是基于numpy，
          因此需要安装numpy和scipy，
          同时需要安装matplotlib进行图片的实现等。

          因此，需要安装如下的包：

          numpy (1.13.3)
          matplotlib (2.1.0)
          scikit-image (0.13.1)
          scipy (1.0.0)
# 测试
     若显示正常，则可以认为相关的库安装成功
```python

import numpy as np 
import scipy as sp 
import matplotlib.pyplotas plt 
from skimage import io 
img = io.imread("./cat.png") 
print(img.shape) 
plt.imshow(img) plt.show()


```

# skimage库子模块介绍
     skimage库的全称scikit-image Scikit，
     是对scipy.ndimage进行了扩展，提供了更多的图片处理功能。
     skimage包含很多的子模块，各个子模块具有不同的功能，如下.

     子模块名称     实现功能
     io            读取,保存和显示图片和视频
     color         颜色空间变换
     data          提供一些测试图片和样本数据
     filters       图像增强，边缘检测，排序滤波器，自动阈值等
     draw          操作于numpy数组上的基本图形绘制，包括线条，矩阵，圆和文本等
     transform     几何变换和其他变换，如旋转，拉伸和Radon(拉东)变换等
     exposure      图像强度调整，例如，直方图均衡化等
     feature       特征检测和提取， 例如，纹理分析等
     graph         图论操作，例如，最短路径
     measure       图像属性测量，例如，相似度和轮廓
     morphology    形态学操作，如开闭运算，骨架提取等
     novice        简化的用于教学目的的接口
     restoration   修复算法，例如去卷积算法，去噪等
     segmentation  图像分割为多个区域
     util          通用工具
     viewer        简单图形用户界面用于可视化结果和探索参数

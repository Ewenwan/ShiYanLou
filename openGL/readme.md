# 　OpenGL 3D图形库

[LearnOpenGL中文化工程](https://github.com/Ewenwan/LearnOpenGL-CN)

OpenGL是一套应用程序编程接口(API)，借助这个API我们开发人员就可以开发出对图形硬件具有访问的能力的程序。
我们可以使用OpenGL开发出运行效率较高的图形程序或游戏，因为OpenGL非常接近底层硬件并且OpenGL
使得我们不必去关注图形硬件的细节。既然我们开发人员不必关注图形硬件的细节，那么我们需要关注什么呢？
我们需要关注OpenGL如何绘制，按照专业术语就是根据物体的规格参数及相关属性，
借助虚拟照相机和光照生成一幅该物体的图像。OpenGL程序与平台是无关的，
所以OpenGL API中不包含任何输入函数或窗口函数，原因是因为这两种函数都要依赖于特定的平台，
例如Windows，Linux或是其他系统。

OpenGL API是过程性的，不是描述性的，即OpenGL不是面向对象的，所以OpenGL无法利用面向对象的特性，
例如重载，继承等，但是我们可以使用面向对象的程序与OpenGL的实现进行链接就可以了。
作为开发人员来说，我们不需要去描述场景的性质和外观，而是去确定一些操作步骤，
为些操作步骤是为实现一定图形或图像所服务的。我们在实现这些步骤时可以调用OpenGL中的一些命令，
可以利用这些命令绘制点、直线、多边形或是其它图形，还可以调用这些命令实现光照、着色，动画等各种效果。

OpenGL的实现可以是软件实现，也可以是硬件实现。
软件实现是对OpengGL函数调用时作出的响应并创建二维或三维图像的函数库，
那么硬件实现则是通过设置能够绘制图形或图像的图形卡驱动程序。
一般来说，硬件实现要比软件实现快得多。我们都应该熟悉，
在Windows上，是由图形设备接口将图形或图像显示在屏幕上或是其他显示设备上的。
OpenGL的实现就软件实现来说，在Windows上会根据程序命令的要求，生成相应的图形或图像，
然后会将这个图形或图像移交给图形设备接口，由图形设备接口将图形或是图像显示在我们的屏幕上或是其他显示设备。
这样一说，我们可能会明白一点OpengGL原来是在应用程序和图形设备接口之间运作.

## Vulkan OpenGL 升级版
[Vulkan中文教程 ](https://github.com/Ewenwan/VulkanTutorialCN)

[OpenGL的替代者——Vulkan](https://blog.csdn.net/jxt1234and2010/article/details/53440644)

从OpenGL升级到Vulkan可以引用一个形象的比喻：API这个中间商从以往的小超市摇身一变成为“某东”，造就价低量又足。（没有中间商，自己赚差价）

Vulkan是Khronos Group(OpenGL标准的维护组织)开发的一个新API，它提供了对现代显卡的一个更好的抽象，与OpenGL和Direct3D等现有api相比，Vulkan可以更详细的向显卡描述你的应用程序打算做什么，从而可以获得更好的性能和更小的驱动开销。Vulkan的设计理念与Direct3D 12和Metal基本类似，但Vulkan作为OpenGL的替代者，它设计之初就是为了跨平台实现的，可以同时在Windows、Linux和Android开发。甚至在Mac OS系统上，Khronos也提供了Vulkan的SDK，虽然这个SDK底层其实是使用MoltenVK实现的。

MoltenVK实际上是一个将Vulkan API映射到Metal API的一个框架，Vulkan这里只是相当于一个抽象层。
然而，为了得到一个更好的性能，Vulkan引入了一个非常冗余的API。相比于OpenGL驱动帮我们做了大量的工作，Vulkan与图像api相关的每一个细节，都需要从头设置，包括初始帧缓冲区的创建与缓冲、纹理内存的管理等等。因此，哪怕只画一个三角形，我们都要写数倍于OpenGL的代码。

而Google在Android 7.0后提供了对Vulkan的支持，并且提供了一系列工具链与Validation Layers（后面会进行说明）。在Android Studio中，只要将Shader代码放在src/main/shaders文件夹下面，项目编译时会自动被编译成.spv字节码，可以作为assets使用。
由于Vulkan的使用非常冗长，


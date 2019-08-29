# Hisilicon海思半导体

[HiSi的Hi3559，Hi3519，Hi3516 NNIE 硬件加速器使用 环境搭建](https://blog.csdn.net/u011728480/article/details/91125581)

[HiSi的Hi3559，Hi3519，Hi3516 NNIE 硬件加速器使用 模型生成及模型仿真](https://blog.csdn.net/u011728480/article/details/91294917)

[HiSi的Hi3559，Hi3519，Hi3516 NNIE 硬件加速器使用 真机调试](https://blog.csdn.net/u011728480/article/details/92069793)

[海思AI芯片(Hi3519A/3559A)方案学习（一）资料以及术语介绍 ](https://blog.csdn.net/ltshan139/article/category/9293521/1)

海思半导体是一家半导体公司，海思半导体有限公司成立于2004年10月，前身是创建于1991年的华为集成电路设计中心。海思公司总部位于深圳，在北京、上海、美国硅谷和瑞典设有设计分部。
海思的产品覆盖无线网络、固定网络、数字媒体等领域的芯片及解决方案，成功应用在全球100多个国家和地区；在数字媒体领域，已推出SoC网络监控芯片及解决方案、可视电话芯片及解决方案、DVB芯片及解决方案和IPTV芯片及解决方案。

HI3559A是专业（移动摄像机） 8K ultra HD Mobile Camera SOC,提供8K30/4K120广播级图像质量的数字视频录制，支持多了sensor输入，支持H.265编码输出或影视级的RAW数据输出，并集成高性能ISP处理，集成4个arm核，2个nnie硬核，4个dsp软核，同时采用先进低功耗工艺和低功耗架构设计，可应用于多功能的运动相机、无人一体机、专业相机和VR相机。

海思AI芯片Hi35xx系列具体包括的型号Hi3559A V100, Hi3559C V100, Hi3519A V100, Hi3516C V500以及Hi3516D V300。

3519A里面有一个硬件NNIE（Neural Network Inference Egine），这个是寒武纪IP（下一代估计就要用海思自己开发的IP了），顾名思义，它可以定点并行运算来进行深度学习算法模型推理。

常用容易糊涂的术语：

MPP: Media Processing plaform  多媒体处理平台 和 音视频H264编码等相关

IPC: Internet Protocol Camera  网络摄像机 

LDC: Lens Distortion Correction  镜头畸变校正

AI： Auto Iris Control  自动光圈控制  这里不是我们所说的人工智能啊

VPSS: video process sub-system  视频前处理单元

VEDU: Video Encoder Unit  视频编码单元

IVE: Intelligent Video Engine 智能加速引擎  常用图像处理 视觉算法的硬件处理单元 不要和AI引擎NNIE 混淆

DPU: Depth Process Unit  深度信息处理单元 用来双目三维建模的

## IVE
IVE是用来图像加速处理的，那么我们必须要有一个图像结构(海思自带的是IVE_IMAGE_S)


## 视频前处理元VPSS（Video Process Sub-System）
[海思HI35XX之----视频处理单元各通道间的关系 ](https://www.cnblogs.com/cyyljw/p/6861593.html)

最近在折腾HI3518C的芯片，应用到IPCamera上，最终获取多路不同分辨率的视频流供不同需求的预览切换。此处简单记录一下视频前处理元VPSS（Video Process Sub-System）的多通道间的关系以及使用心得。

HI3518C的视频子系统包括：视频系统初始化模块（MPP_SYS_CONF_S）、视频输入模块（VI）、视频前处理模块（VPSS）、视频编码模块（VENC）和视频输出模块（VO）组成。

HI3518C的VPSS是视频数据编码前的预处理单元，主要完成对一幅输入图像的统一预处理，如去噪、去隔行等，然后再对各通道分别进行缩放、锐化等处理（如：PreScale、De-ring/De-block、NR、IE、DIE、Sharpen 等，具体可参见VPSS_GRP_ATTR_S结构体），最后输出多种不同分辨率的图像。
        
其中，HI3518C对用提供GROUP的概念来管理VPSS，最多支持128个GROUP（我目前只使用了一个组：GROUP0）。VPSS的GROUP又包含多个通道（CHANNEL），对于不同规格的芯片，每个组包含的通道数也不相同，规格文档上都有详细说明。
        
HI3518C的各个GROUP包含8个CHANNEL，其中2个物理通道（PHY CHN）、一个直通通道(Bypass)和5个扩展通道(EXT CHN)。
        
物理通道是由硬件提供的，各物理通道的功能和限制各不相同；直通通道具备CROP功能，可以把不需要作预处理的一幅幅图像直接透传给接收者；扩展通道具备缩放功能，当它绑定到物理通道后，可以把从物理通道获取到的图像缩小或放大成用户所需要的不同分辨率的图像。Hi3518C 提供 5 路扩展通道以提供更好的灵活性，扩展通道具备缩放和帧率控制功能，不经过其他硬件算法单元处理。
        
用户在使用的时候，需要首先创建VPSS GROUP，然后设置好GROUP属性并启用VPSS GROUP之后，再把VI作为源通道，把VPSS作为目标通道，通过调用 HI_MPI_SYS_Bind 绑定VI到VPSS，就可以建立视频输入到视频预处理间的通路，使得前端采集到的视频数据经由此通路到达VPSS单元。注意，每个 GROUP 仅可与一个输入源绑定。
        
然后，就是根据实际需要来创建多个VPSS CHANNEL，从CHANNEL中获得所需要的目标分辨率的图像输出了。
        
HI3518C的物理通道0无缩放能力，支持锐化，不支持抽场，一般用于大码流编码和JPEG抓拍。
        
HI3518C的物理通道1，水平、垂直均最大支持 8 倍缩小，水平、垂直均最大支持 8 倍放大，支持锐化， 不支持抽场，一般用于小码流编码和CVBS（复合视频广播信号）预览。
        
因此，用户可以通过CHANNEL（0）来获取到前端图像采集模块所支持的最大像素分辨率的图像，也就是说，如果前端sensor支持的最大图像分辨率为1920*1080像素，那么经CHANNEL0所得到的图像就是1920*1080像素格式，并且不可以把该通道的数据再作为其它扩展通道的源来进行图像的放大或缩小处理。
        
用户可以通过CHANNEL（1）来获取到的图像为用户任意指定的合法的像素大小，通过指定像素格式及宽高大小，进行预处理后可以作为视频编码模块(VENC)的输入。同时，用户可以把该通道作为后面所要建立的扩展通道的源，通过设置所要建立的扩展通道的属性（主要指：绑定的源通道号、图像帧率、图像宽高等，见VPSS_EXT_CHN_ATTR_S结构体），再经由各扩展通道来获取到的所需的目标分辨率。
        
用户还可通过扩展通道CHANNEL（3、4、5、6、7）来获取除VI所支持的最大分辨率以外的各种符合标准的不同分辨率图像，比如前端sensor支持最大分辨率为1920*1080，那么结合扩展通道的使用，用户就可以获取到HD720(1280*720)、VGA(640*480)、QVGA（320*240）、CIF（352*288）、QCIF（176*144）等分辨率的图像了。
        
注意，当把通道 1 作为输入源的时候，要设置通道 1 为USER模式。因为HI3518C只有两个物理通道，通道0不具备缩放功能，所以只有通道1可以作为扩展通道的源，对图像进行缩放处理。
        
其中，VPSS GROUP有两种工作模式：AUTO 和 USER，两种模式间可动态切换。默认的工作模式为 AUTO，此模式下各通道仅可与一个接收者绑定。若想使用 USER 模式，则需调用 MPI 接口进行设置，同时指定所需图像的大小和格式，此模式下各通道可与多个接收者绑定。需要特别注意的是，USER 模式主要用于对同一通道图像进行多路编码的场景，此模式下播放控制不生效，因此预览和回放场景下不建议使用。Bypass 通道只能工作在 AUTO 模式，可与多个接收者绑定。
        

## NNIE(Neural Network Inference Engine)硬核 深度学习专用加速器

NNIE简介

NNIE是 Neural Network Inference Engine 的 简 称 是 海思 媒体 S oC 中 专门针对神经网
络特别是深度学习卷积神经网络进行加速处理的硬件单元。----- 摘自hisi sdk svp部分《HiSVP开发指南.pdf》

NNIE 工作流程简介

海思提供了一个NNIE Mapper的工具（Linux ， Win都有）。由于NNIE只支持Caffe框架，我们需要的是把Caffe的模型转换为NNIE可以使用的模型。
在我们转换的时候，需要我们提供一个NNIE转换的配置文件，然后根据配置文件把相关的caffe模型转换为NNIE的模型。然后我们在板子上加载这个模型，调用相关的API就可以完成这个网络的加速计算。

NNIE 环境搭建

工欲善其事必先利其器。NNIE最开始接触的时候，我觉得贼难受，觉得很难。但是当你把环境配置好了，你就会觉得事半功倍，很舒服。
以下内容，我都是按照HISI SDK的SVP部分的《HI SVP开发指南.pdf》做的，只是由于时效性的原因，有些内容需要做一定的改变适应才行。



# SVP smart vision processing 智能视觉异构加速平台

hisi svp 整体框架:

vision app  做图像数据的准备以及结果处理

--------------------

mpi(MPP Program Interface)   做mmz内存分配（海思特有的内存空间）

--------------------

driver(ko)   驱动

--------------------

nnie（hardware）   nnie做forward

## 开发
```c
/*
* mpp System init  海思特有的内存空间系统初始化
*/
HI_MPI_SYS_Exit()
HI_MPI_VB_Exit()
//设置MPP 视频缓存 池 属性 。
HI_MPI_VB_SetConfig()
HI_MPI_VB_Init()
HI_MPI_SYS_Init()


/// 载入模型
//load model
//在mmz中分配一部分内存来存放model
HI_MPI_SYS_MmzAlloc()
//从mmz内存中解析模型
HI_MPI_SVP_NNIE_LoadModel()


//NNIE 初始化

//NNIE  Param  Init

//forward prepare
//------------------------根据model的配置，为每一段（这里你可以简单理解为层）的forward ctrl param , src Blob, dst Blob.也就是初始化SVP_NNIE_FORWARD_CTRL_S[]，SVP_NNIE_FORWARD_WITHBBOX_CTRL_S[]，SVP_SRC_BLOB_S[]，SVP_DST_BLOB_S[]数组元素的值。
HI_MPI_SVP_NNIE_GetTskBufSize()//获取给定网络任务 各段 辅助内存
HI_MPI_SVP_NNIE_AddTskBuf()//记录TskBuf 地址 信息

//----------------给第一层送入预处理好的图片到SVP_SRC_BLOB_S

HI_MPI_SYS_MmzFlushCache()//刷新内存
HI_MPI_SVP_NNIE_Forward()//forward
HI_MPI_SVP_NNIE_Query()//查询forward任务是否完成
HI_MPI_SYS_MmzFlushCache()//刷新内存


```



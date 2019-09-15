# 单片机MCU 树莓派  飞控
## stm32
## TI
## arduino

[一些有趣的软硬件设计](https://github.com/Ewenwan/iceory.gitbook.io/tree/master/Some%20Thing%20Fun)

[嵌入式与linux shell 单片机 电机控制](https://www.cnblogs.com/ironstark/category/745952.html)

[基于树莓派和Arduino制作的多种移动机器人](https://github.com/Ewenwan/hellobot-raspberry)


# 电路设计
[Altium Designer 导出pcb制作文件](https://blog.csdn.net/qq997758497/article/details/77654883)


# ARM 

先来谈一下ARM的发展史：1978年12月5日，物理学家赫尔曼·豪泽（Hermann Hauser）和工程师Chris Curry，在英国剑桥创办了CPU公司（Cambridge Processing Unit），主要业务是为当地市场供应电子设备。1979年，CPU公司改名为Acorn计算机公司。

80年代中期，Acorn的一个小团队要为他们的下一代计算机挑选合适的处理器，根据他们提供的技术需求，在当时的市场上无法找到合适的处理器，于是Acorn决定自己设计一个处理器（真有魄力）。一个小团队仅仅用了18个月就完成了从设计到实现的全过程，这是一台RISC指令集的计算机，叫做Acorn RISC Machine（简称ARM）。后来Acorn公司没落了，而处理器设计部门被分了出来，组成了一家新公司。

ARM公司主要设计ARM系列AISC处理器内核，它不生产芯片，只提供IP核。先以一个例子解释一下架构、核、处理器和芯片：S3C2440，这是一款SoC芯片，注意，它不是cpu，2440和我们熟知的51单片机有点类似，都属于嵌入式，嵌入式的发展到目前经历了三个阶段，分别是SCM、MCU、SoC。51属于SCM或MCU，而2440就属于SoC(system on chip)了。

51 mcu  = cpu + 外设(io\时钟\uart\...)

SoC = cpu + 外设\控制器（ip）

不同的SOC，架构不同(就是CPU如何和IP联系起来，有的以总线为核心，有的以DDR为核心)，所以，海思是拥有自主产权的SOC架构。可是，无论任何厂商，再怎么折腾，都没有怎么动过CPU，ARM核心就好好的呆在那里，那就是中央处理器。


ARM命名规则：

第一个数字：系列名称：eg.ARM7、ARM9

第二个数字：Memory system

                    2：带有MMU
                    4：带有MPU
                    6：无MMU与MPU
                    
第三个数字：Memory size

                    0：标准Cache（4-128k）
                    2：减小的Cache
                    6：可变的Cache
                    
第四个字符：T：表示支持Thumb指令集

                    D：表示支持片上调试（Debug）
                    M：表示内嵌硬件乘法器（Multiplier）
                     I ：支持片上断点和调试点
                     E：表示支持增强型DSP功能
                     J ：表示支持Jazelle技术，即Java加速器
                     S：表示全合成式
                     
[ARMV8 datasheet学习笔记](https://www.cnblogs.com/smartjourneys/tag/ARMV8/)


                     

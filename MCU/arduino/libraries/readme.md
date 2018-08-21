# 依赖库
## 1. PWM 库
    PWM.h
    ATimerDefs.cpp 
    ATimerDefs.h 
    BTimerDefs.cpp 
    BTimerDefs.h
    PWM_demo.c
```

PWM的行为是由叫作定时器的集成元件决定的。每个定时器有2个或4个通道。
每个通道连到一个控制器引脚上。改变一个引脚的频率需要改变它所连接的定时器的频率，
这样反过来也会改变其它连到同一个定时器的引脚的频率。

Timer0通常被用作实现Arduino的计时功能（例如，millis()函数）。
改变timer0的频率将会破坏你工程里使用计时函数的其它程序段部分。

Arduino中有两种定时器，8位定时器和16位定时器。
简单来说，就是它们所存在的细微差别使得不限制一方或另一方的代码实现变得困难。


使用一个8位的定时器来设定定制的频率（使用预分频产生不了的频率）需要牺牲一个通道。
换句话说，每个设定定制频率的8位定时器会失去在一个引脚产生pwm的能力（连接A通道的引脚会更准

确些）。
除了Leonardo的所有Arduino都有两个8位定时器，
这意味着如果你把所有定时器设定特殊的频率，上述所说的Arduino控制器总共将会牺牲两个引脚。

#######################################
# Syntax Coloring Map PWM
#######################################

#######################################
# Datatypes (KEYWORD1)
#######################################

#######################################
# Methods and Functions (KEYWORD2)
#######################################

InitTimers	KEYWORD2
     初始化所有定时器。需要在改变定时器频率或设定一个引脚的占空比之前用。

InitTimersSafe	KEYWORD2
     除了为了保留计时功能函数而不初始化timer0外，其它都与InitTimers()一样。


pwmWrite	KEYWORD2
     与‘analogWrite()’一样，但是只有在相应定时器初始化后才工作
     0~255

pwmWriteHR	KEYWORD2
     0~65535

SetPinFrequency	KEYWORD2
     设定引脚的频率（hz）,返回一个设定成功与否的布尔值

SetPinFrequencySafe	KEYWORD2
     除了不影响timer0外，其它都与SetPinFrequency函数一样

GetPinResolution KEYWORD2
     分辨率

使用这个库，16位的定时器的频率可以从1hz变化到8Mhz，
8位定时器的频率可以从31hz变化到2Mhz。
但是随着频率的越来越大，占空比可变化的范围就越小。
将频率设定为8Mhz是可以实现的，但是这样占空比的变化范围将变得非常小。
请确定定时器的频率正确的设置了，并检查这个函数的返回值。


下面是跟帖中比较有用的部分总结：
硬件上，Mega系统控制板，
11引脚连着timer1，
引脚9连接timer2， 
引脚7连接timer4。
这是软件改变不了的。

关于分辨率的问题：
8位的定时器兼容8位的分辨率，16位的定时器兼容16位的分辨率。
为了与analogWrite()保持一致性，pwmWrite()函数都使用8位分辨率。
如何需要更高的分辨率时，使用pwmWriteHR()代替。

Void pwmWrite(uint8_t pin, uint8_t duty) 8-bit, 0-255 
Void pwmWriteHR(uint8_t pin, uint16_t duty) 16-bit 0-65535



#########################################

Timer0_GetFrequency	KEYWORD2
     读取定时器的频率（hz）

Timer0_SetFrequency	KEYWORD2
     设定定时器的频率

Timer0_GetPrescaler	KEYWORD2
     读取预分频因子

Timer0_SetPrescaler	KEYWORD2
     设定预分频因子

Timer0_GetTop	KEYWORD2
     读取定时器寄存器最大值

Timer0_SetTop	KEYWORD2
     设定定时器寄存器最大值

Timer0_Initialize	KEYWORD2
     初始化定时器

Timer0_GetResolution KEYWORD2



###########################################

Timer1_GetFrequency	KEYWORD2
Timer1_SetFrequency	KEYWORD2
Timer1_GetPrescaler	KEYWORD2
Timer1_SetPrescaler	KEYWORD2
Timer1_GetTop	KEYWORD2
Timer1_SetTop	KEYWORD2
Timer1_Initialize	KEYWORD2
Timer1_GetResolution KEYWORD2

Timer2_GetFrequency	KEYWORD2
Timer2_SetFrequency	KEYWORD2
Timer2_GetPrescaler	KEYWORD2
Timer2_SetPrescaler	KEYWORD2
Timer2_GetTop	KEYWORD2
Timer2_SetTop	KEYWORD2
Timer2_Initialize	KEYWORD2
Timer2_GetResolution KEYWORD2

Timer3_GetFrequency	KEYWORD2
Timer3_SetFrequency	KEYWORD2
Timer3_GetPrescaler	KEYWORD2
Timer3_SetPrescaler	KEYWORD2
Timer3_GetTop	KEYWORD2
Timer3_SetTop	KEYWORD2
Timer3_Initialize	KEYWORD2
Timer3_GetResolution KEYWORD2

Timer4_GetFrequency	KEYWORD2
Timer4_SetFrequency	KEYWORD2
Timer4_GetPrescaler	KEYWORD2
Timer4_SetPrescaler	KEYWORD2
Timer4_GetTop	KEYWORD2
Timer4_SetTop	KEYWORD2
Timer4_Initialize	KEYWORD2
Timer4_GetResolution KEYWORD2

Timer5_GetFrequency	KEYWORD2
Timer5_SetFrequency	KEYWORD2
Timer5_GetPrescaler	KEYWORD2
Timer5_SetPrescaler	KEYWORD2
Timer5_GetTop	KEYWORD2
Timer5_SetTop	KEYWORD2
Timer5_Initialize	KEYWORD2
Timer5_GetResolution KEYWORD2

#######################################
# Constants (LITERAL1)
#######################################
// 分频 常量 两种
ps_1 LITERAL1
ps_8 LITERAL1
ps_64 LITERAL1
ps_256 LITERAL1
ps_1024 LITERAL1

psalt_1 LITERAL1
psalt_8 LITERAL1
psalt_32 LITERAL1
psalt_64 LITERAL1
psalt_128 LITERAL1
psalt_256 LITERAL1
psalt_1024 LITERAL1
```
    
## 2. PID控制
    PID.h 
    PID.c
    PID_demo.c
## 3. 定时器1
    ThmerOne_demo.c 
    TimerOne.cpp 
    TimerOne.h 
    TimerOne_known_16bit_timers.h
## 4. 定时器2
    MsTimer2.cpp 
    MsTimer2.h 
    MsTimer2_demo.c
    
    FlexiTimer2.cpp 
    FlexiTimer2.h 
    FlexiTimer2_demo.c 
## 5. 定时器3
    TimerThree.cpp 
    TimerThree.h 
    TimerThree_demo.C

    

# 资料手册


## 1. PWM口资源=======================
### a. ATMEGA 2560   Arduino MEGA 2560  一共 12 个 PWM口=================

    arduino mega2560 开发板。
    PWM波是通过timer/counter产生的，
    该单片机有timer0、timer1、timer2、timer3、timer4、timer5共六个定时/计数器，
    其中timer0、timer2为8位的定时/计数器，每个定时/计数器又有A、B两个通道，
    而定时/计数器timer1、timer3、timer4、timer5为16位的，每个又有A、B、C三个通道。
    首先来看一下定时/计数器的各个通道与arduino mega2560 引脚的对应关系：
    
    Timer0  8bit  PWM  (13) 和 4 口  
            两个独立比较输出单元  OC0A-PB7  PWM13口  OC0B-PG5-PWM4口  支持 pwm输出功能

    Timer2  8bit PWM 10 和  9 口
            两个独立比较输出单元  OC2A-PB4  PWM10口    OC2B-PH6-PWM9口   支持 pwm输出功能

    Timer1  16bit  PWM  11  12 口
            三个独立比较输出单元 OC1A-PB5-PWM11口  OC1B-PB6-PWM12口   ( OC1C-PB7(与OC0A复用)-PWM13口 ) 
            可以看到timer0的A通道与timer1的C通道共用一个引脚，若这两个通道同时被使能，将会输出两者相与的结果。
    Timer3  16bit  PWM   5  2  3 
            三个独立比较输出单元  OC3A-PE3-PWM5口  OC3B-PE4-PWM2口   OC3C-PE5-PWM3口 
        支持 pwm输出功能

    Timer4  16bit  PWM 6 7 8
            三个独立比较输出单元  OC4A-PH3-PWM6口  OC4B-PH4-PWM7口   OC4C-PH5-PWM8口 
        支持 pwm输出功能


    Timer5  16bit  无PWM   44 45 46
        三个独立比较输出单元  OC5A-PL3-44口  OC5B-PL4-45   OC5C-PL5-46  
        支持 pwm输出功能


### b. ATMEGA   328P   Arduino  uno  一共六个 PWM 口======================

      Timer0  8bit  PWM  5 和 6
              两个独立比较输出单元  
          OC0A-PD6  PWM 6 口  
          OC0B-PD5 -PWM 5 口  支持 pwm输出功能

      Timer2  8bit PWM 3 和 11
              两个独立比较输出单元  
          OC2A-PB3-PWM11口 复用 SPI的MOSI 口 
          OC2B-PD3-PWM3口   支持 pwm输出功能

      Timer1  16bit  PWM 9 10口
              两个独立比较输出单元  
          OC1A-PB1-PWM9口    
          OC1B-PB2-PWM10口   支持 pwm输出功能	

### pwm口：
      timer   bits   channel   Arduino pin   Mega2560 pin
      timer0   8       A          6              13
      timer0   8       B          5              4
      timer1   16      A          9              11
      timer1   16      B          10             12
      timer2   8       A          11             10
      timer2   8       B          3              9
      timer3   16      A          -              5
      timer3   16      B          -              2
      timer3   16      C          -              3
      timer4   16      A          -              6
      timer4   16      B          -              7
      timer4   16      C          -              8
      timer5   16      A          -              44
      timer5   16      B          -              45
      timer5   16      C          -              46


## 2. 中断口资源====================================

      型号         int.0 int.1 int.2 int.3 int.4 int.5
      UNO\Ethernet  2     3 　 　 　 　
      Mega2560      2     3     21     20    19   18
      Leonardo      3     2     0      1 　 　
      Due           　所有IO口均可
      
## 3. 通信
    a. 串口
    b. i2c
    c. spi
    d. can

## 4. pwm 冷门知识
### a. 普通io延时实现
```c
int pin = 13;
void setup() {
  pinMode(pin, OUTPUT);
}
void loop(){
  digitalWrite(pin, HIGH);
  delayMicroseconds(100); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(pin, LOW);
  delayMicroseconds(1000 - 100);
}
// 时间周期是 1000 us = 1ms, 所以一秒循环 1000次, 因此 频率Frequency 是 1 KHz,
// 每个循环中, 有电(高电平状态)的比率是 100/1000 * 100% = 10%, 所以 duty cycle (占空比)为 10%;
// 模拟的电压为 5Volt x 10% = 0.5 Volt 的电压!
```
    这种方法的好处是任意一个IO管脚都可以实现，而坏处是 频率和占空比都会受到中断的影响，而变的不准确。
    在一个是，做了这个，cpu就干不了其他的事情了。
### b. 通过定时器和比较器来实现
```c
// Arduino UNO 的 MCU 有三個 timer,
// 其中 timer0 控制 pin 5, pin 6; timer1 控制 pin 9, pin 10; timer 2 控制 pin 11, pin 3;
// 所以, 我们可以对这些 pin 用 analogWrite(pin, val); 输出 0 到 255 的 val 值到 pin ;
// 如果输出 val 是 0, 它会偷偷直接改用 digitalWrite(pin, 0); 输出低电平,
// 如果 val 是 255, 也是会偷偷直接改用 digitalWrite(pin, 1); 輸出高电平!
// 如果 val 是 1 到 254, 会下命令请 pin 脚对应的 timer定时器帮忙!!

// 系统时钟通过分屏器会接到定时器上，每来一个时钟，定时器的计数器 TCNT? 的 计数值counter会加1，
// 定时器有几种计数模式，一般有 增计数模式、减计数模式、增减计数模式。
// 每個 定时器timer 的分频 Prescaler 是独立设置的, 通常可以設 1, 2, 4, 8, 64, 256, or 1024 等,
// 这必须查看 单片机 MCU 的 手册datasheet.

// 要设定 定时器timer 的 计数模式Mode， 可以通过修改 timer 的控制寄存器, TCCR?A, TCCR?B(TCCR1A, TCCR1B,TCCR2A, TCCR2B),
// 注意以 ATmega328 為例, TCCR?A 和 TCCR?B 要合起來用, 此處的 A, B 和 通道channel A, channel B无关，不是一个事!!

// Arduino 每个 timer 有两个比较器, 分別命名 OCR?A 和 OCR?B ,
// 其中 ? 是 0, 1, 2 分別对应到 timer0, timer1, 和 timer2 着三个定时器.
// 每当 定时器的计数器 TCNT? 的值 等于 比较器的值，定时器就有在这个时刻改变 通道的电平，
// 而当计数器 TCNT? 溢出时， 通道的电平也可能改变。

// arduino-1.8.1\hardware\arduino\avr\cores\arduino\wiring_analog.c
// arduino-1.8.1\hardware\arduino\avr\cores\arduino\wiring.c
//  analogWrite() 的实现
那 PWM 的 Frequency 可不可以更改?
A: 可以, 偷改 timer 的 Prescaler 就可以達到更改 Frequency 的目的 !
   但是, 千萬不要更改 timer0 的 Prescaler, 否則 millis( ) 和 micros( ) 以及 delay() 都會受到影響 !!!
   以下是以 timer1 控制的 pin 9, pin 10 為例(注意兩個 pin 的頻率相同!)
   在你的 setup( ) { 內, 寫如下兩句即可:
      int fff = 3;  // 可以是 1, 2, 3, 4, 5
      TCCR1B = TCCR1B & 0xF8 | fff;
  其中 fff 與對應頻率如下:
  fff   Prescaler   Frequency
   1           1        31372.549 Hz
   2           8          3921.569
   3         64            490.196   <--DEFAULT
   4       256           122.549
   5    1024               30.637 Hz

至於 timer2 控制的 pin 11 和 pin 3,
則在 setup( ) { 內寫:
     TCCR2B = TCCR2B & 0xF8 | ?;
此處的 ? 可以有七種:
   ?  Prescaler   Frequency
   1         1       31372.549 Hz
   2         8         3921.569
   3            32          980.392
   4        64         490.196   <--DEFAULT
   5      128         245.098
   6      256         122.549
   7    1024           30.637 Hz

如果你堅持要改 timer0 的 Prescaler, 以更改 pin 5, pin 6 的 PWM 頻率:
   (注意 millis( ) 和 micros( ) 以及 delay() 都會受到影響 !! )
則在 setup( ) { 內寫:
     TCCR0B = TCCR0B & 0xF8 | ?;
此處的 ? 可以有五種:
   ?  Prescaler   Frequency
   1          1       362500 Hz
   2          8           7812.5
   3        64             976.5625   <--DEFAULT
   4      256             244.140625
   5    1024               61.03515625 Hz

參考:
   http://playground.arduino.cc/Main/TimerPWMCheatsheet
   http://www.atmel.com/Images/doc8161.pdf
   
脉冲生成模式控制位（WGM）：用来设置时钟的模式
时钟选择位（CS）：设置时钟的预定标器
输出模式控制位（COMnA和COMnB）：使能/禁用/反相 输出A和输出B
输出比较器（OCRnA和OCRnB）：当计数器等于这两个值时，输出值根据不同的模式进行变化

你改这几个寄存器就好了，我之前用的是arduino nuo 。
下面这个例子以Timer2为例，把Pin3和Pin11作为快速PWM的两个输出管脚。其中：
WGM的设置为011，表示选择了快速PWM模式；
COM2A和COM2B设置为10，表示A和B输出都是非反转的PWM；
CS的设置为100，表示时钟周期是系统时钟的1/64；
OCR2A和OCR2B分别是180和50，表示两路输出的占空比；

pinMode(3, OUTPUT);  
pinMode(11, OUTPUT);  
TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  
TCCR2B = _BV(CS22);  
OCR2A = 180;  
OCR2B = 50;  
这段代码看上去有点晕，其实很简单。_BV(n)的意思就是1< COM2A1，表示COM2A的第1位（靠，其实是第2位，不过程序员们是从0开始数数的）。所以_BV(COM2A1)表示COM2A = 10；
类似的，_BV(WGM21) | _BV(WGM20) 表示 WGM2 = 011。

在Arduino Duemilanove开发板，上面这几行代码的结果为：
输出 A 频率: 16 MHz / 64 / 256 = 976.5625Hz
输出 A 占空比: (180+1) / 256 = 70.7%
输出 B 频率: 16 MHz / 64 / 256 = 976.5625Hz
输出 B 占空比: (50+1) / 256 = 19.9%

CS就是改它的分频数的。比如你改成TCCR2B = _BV(CS20)。你用示波器测它的变化貌似会达到44KHZ。

analogWrite(); 的变化好像就是是980hz，490hz。你自己多试验一下。
你可以参考http://www.diy-robots.com/?p=852
```
## 定时器控制寄存器详解

    每个timer/counter有 计数、快速PWM 及 相位校准PWM等模式（常用这三个吧），
    控制这些timer/counter工作方式的寄存器有TCCRnA、TCCRnB、OCRnA、OCRnB、OCRnC（若定时器有C通道），
    其中n代表timer/counter的名字。TCCRnA及TCCRnB为控制寄存器，他们可以控制的参数有：
    1. 脉冲生成模式控制位（WGM）：用来设置timer/counter的工作模式，
        2bit位于寄存器TCCRnA，2bit（16位timer/counter） 2bit（8位timer/counter）位于寄存器TCCRnB；
    2. 时钟选择位（CS）：设置时钟的分频系数（计数器工作的速度），3bit位于寄存器TCCRnB；
    3. 输出模式控制位（COMnA、COMnB及COMnC）：使能/禁用/反相 输出波形，各占2bit，位于寄存器TCCRnA。
    4. 输出比较器（OCRnA、OCRnB及OCRnC）：该值为计数器与之进行比较的值，当计数器等于这三个值时，
        相应通道的输出值根据不同的模式进行变化，这些寄存器跟对应的定时/计数器有相同的位数。
        
TCCRnA  决定通道口模式 和 定时器模式(前两位)：

![](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/arduino/menu_pdf/TCCRnA.PNG)

TCCRnB  决定定时器模式(后两位) 和 分频系数时钟选择位（CS）：

![](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/arduino/menu_pdf/TCCRnB.PNG)


## 定时器模式 脉冲生成模式 WGMn3 WGMn2 WGMn1 WGM0
    模式id    WGMn3 WGMn2 WGMn1 WGM0
    0          0     1      0    0    CTC计数模式
    1          1     1      0    0    CTC计数模式
    2          1     1      1    1    快速PWM模式    计数到 OCRnA
    3          1     1      1    0    快速PWM模式    计数到 ICRn
    4          0     1      0    1    快速PWM模式    8bits  计数到0x00ff   256
    5          0     1      1    0    快速PWM模式    9bits  计数到0x01ff   512
    6          0     1      1    1    快速PWM模式    10bits 计数到0x01ff   1024
    7          0     0      0    1    相位校准PWM模式8bits   计数到0x00ff   256
    8          0     0      1    0    相位校准PWM模式9bits   计数到0x01ff   512
    9          0     0      1    1    相位校准PWM模式10bits  计数到0x01ff   1024
    10         1     0      1    0    相位校准PWM模式    计数到 ICRn
    11         1     0      1    1    相位校准PWM模式    计数到 OCRnA
    12         1     0      0    0    相位校准+频率校准PWM模式 计数到 ICRn
    13         1     0      0    1    相位校准+频率校准PWM模式 计数到 OCRnA
    14         0     0      0    0    正常   计数到0xffff  65535
    15         1     1      0    1    保留
## 通道口模式 输出模式 （COMnA、COMnB及COMnC）

## 示例程序
```c
void setPwmFrequency2560(int pin, int divisor)// 管脚 分频
{
  byte mode;
  if((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46)) 
  {
     switch(divisor) 
     {// 分频
        case 1: mode = 0x01; break;   // 001b
        case 8: mode = 0x02; break;   // 010b
        case 64: mode = 0x03; break;  // 011b
        case 256: mode = 0x04; break; // 100b
        case 1024: mode = 0x05; break;// 101b
        default: return;
      }
      
      if(pin == 4 || pin == 13)//Timer0
      {
        TCCR0B = TCCR0B & 0b11111000 | mode;  // 后三位 分频=======
  // 还可以设置 TCCR0A 确定工作模式
      }
      
      else if(pin == 11 || pin == 12 || pin == 13)// Timer1
      {
        TCCR1B = TCCR1B & 0b11111000 | mode; // 
      }
      
      else if(pin == 8 || pin == 9)//Timer2
      {
        TCCR2B = TCCR2B & 0b11111000 | mode;  
      }
      
      else if(pin == 5 || pin == 2 || pin == 3)// Timer3
      {
        TCCR3B = TCCR3B & 0b11111000 | mode;  
      }
      
      else if(pin == 6 || pin == 7 || pin == 8)// Timer4
      {
        TCCR4B = TCCR4B & 0b11111000 | mode;    
      }
      
      else if(pin == 46 || pin == 45 || pin == 44)// Timer5
      {
        TCCR5B = TCCR5B & 0b11111000 | mode;    
      }
  } 
}

void setup() 
{                
  
  pinMode(44, OUTPUT);
  setPwmFrequency2560(44,8);
  
  pinMode(7, OUTPUT);
  setPwmFrequency2560(6,1);
  
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  setPwmFrequency2560(5,1); //pin2,pin5属于同一个计时器，设置一次即可。
}

void loop() 
{
   analogWrite(44,128);
   analogWrite(7,128);
   analogWrite(5,128);
   analogWrite(2,128);
   delay(500); 
}

```




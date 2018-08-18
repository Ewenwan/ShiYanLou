
## 1. PWM口资源=======================
### a. ATMEGA 2560   Arduino MEGA 2560  一共 12 个 PWM口=================

    Timer0  8bit  PWM  (13) 和 4  
            两个独立比较输出单元  OC0A-PB7  PWM13口  OC0B-PG5-PWM4口  支持 pwm输出功能

    Timer2  8bit PWM 10 和 9
            两个独立比较输出单元  OC2A-PB4-PWM10口    OC2B-PH6-PWM9口   支持 pwm输出功能

    Timer1  16bit  PWM  11  12 口
            三个独立比较输出单元 OC1A-PB5-PWM11口  OC1B-PB6-PWM12口   ( OC1C-PB7(与OC0A复用)-PWM13口 ) 
        支持 pwm输出功能

    Timer3  16bit  PWM  2  3  5
            三个独立比较输出单元  OC3A-PE3-PWM5口  OC3B-PE4-PWM2口   OC3C-PE5-PWM3口 
        支持 pwm输出功能

    Timer4  16bit  PWM 6 7 8
            三个独立比较输出单元  OC4A-PH3-PWM6口  OC4B-PH4-PWM7口   OC4C-PH5-PWM8口 
        支持 pwm输出功能


    Timer5  16bit  无PWM   44
        三个独立比较输出单元  OC5A-PL3  OC5B-PL4   OC5C-PL5  
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

## 

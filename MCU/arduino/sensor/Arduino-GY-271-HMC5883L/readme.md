## 基于Arduino的GY-271磁场模块角度测量（HMC5883L）

[TOC]

### 1. 3轴磁场模块

[GY-271模块介绍](http://www.dipmicro.com/store/GY271-MOD)

[HMC5883L芯片资料](http://www.dipmicro.com/?datasheet=HMC5883L.pdf)

#### 1.1 GY-271

![](C:\Users\liuyang\Documents\GitHub\2018-EducationalRobot-PosionModule\img\HMC5883L.png)

- **Power 3-5V (3.3V/5V，Recommend 3.3V)**
- Chipset HMC5883L
- Communication via I2C protocol
- Dimensions 14.8 x 13.5 x 3.5mm

#### 1.2 HM5883L芯片

HMC5883L作为一个IIC兼容装置，该装置包含一个7-bit串行地址，并支持IIC协议。HMC5883L可以支持标准和快速模式，分别为100kHz和400kHz，但不支持高速模式（Hs）。

| VCC  | 2.16V-3.6V                                                   |
| ---- | ------------------------------------------------------------ |
| GND  |                                                              |
| SCL  | 串行时钟——IIC总线主/从时钟                                   |
| SDA  | 串行数据——IIC总线主/从数据                                   |
| DRDY | 数据准备，中断引脚，内部被拉高，选项为连接，当数据位于输出寄存器上时会在低电位上停250us |

### 2. HMC5883L测量原理

地磁场是一个矢量，对于一个固定的地点来说，这个矢量可以被分解为两个与当地水平面平行的分量和一个与当地水平面垂直的分量。如果保持电子罗盘和当地的水平面平行，那么罗盘中磁力计的三个轴就和这三个分量对应起来，如图所示。

![](C:\Users\liuyang\Documents\GitHub\2018-EducationalRobot-PosionModule\img\yuanli.jpg)

实际上对水平方向的两个分量来说，他们的矢量和总是指向磁北的。罗盘中的航向角（Azimuth）就是当前方向和磁北的夹角。由于罗盘保持水平，只需要用磁力计水平方向两轴（通常为X轴和Y轴）的检测数据就可以用式计算出航向角。当罗盘水平旋转的时候，航向角在0- 360度之间变化。因此，有以下两种计算方法：

- 当磁场模块与水平面平行时，Z轴同地磁场垂直分量，水平分量被映射到X轴和Y轴上。

  ![](C:\Users\liuyang\Documents\GitHub\2018-EducationalRobot-PosionModule\img\XY.png)

所以X轴与地磁水平分量的夹角$\alpha=\arctan\frac{Y}{X}$, 代码为`angle=atan2(y,x) * (180 / PI) + 180;` ，`atan2()`函数返回弧度角，范围为±$\pi$ ，化为0-360度。

- 当磁场模块在空间中任意角度放置时，需要利用X,Y，Z轴数据来修正。

  [知乎：三轴螺旋仪空间修正](https://zhuanlan.zhihu.com/p/20082486)

![](C:\Users\liuyang\Documents\GitHub\2018-EducationalRobot-PosionModule\img\shanwei.png)

### 3. 基于Arduino的角度测量

#### 3.1 硬件连接

- `Vcc-GND:3.3V(5V兼容，推荐3.3V)`
- `SDA:A4`
- `SCL:A5`

#### 3.2 软件设计

> 工程文件见文件夹 GY271_Arduino

```c
/*
 * HMC5883L Demo.
 * dipmicro electronics, 2014
 *
 * Hardware Used: GY-271 HMC5883L
 * 
 * Arduino UNO/compatible/Nano
 * GY271 module (dipmicro part DE4196
 *  Arduino GND -> GY271/HMC5883L GND
 *  Arduino 3.3V -> GY271/HMC5883L VCC
 *  Arduino A4 (SDA) -> GY271/HMC5883L SDA
 *  Arduino A5 (SCL) -> GY271/HMC5883L SCL 
 */

#include <Wire.h>          //I2C Arduino Library

#define HMC5883L_ADDR 0x1E //0011110b, I2C 7bit address of HMC5883

bool haveHMC5883L = false;

bool detectHMC5883L ()
{
  // read identification registers
  Wire.beginTransmission(HMC5883L_ADDR); //open communication with HMC5883
  Wire.write(10); //select Identification register A
  Wire.endTransmission();
  Wire.requestFrom(HMC5883L_ADDR, 3);
  if(3 == Wire.available()) {
    char a = Wire.read();
    char b = Wire.read();
    char c = Wire.read();
    if(a == 'H' && b == '4' && c == '3')
      return true;
  }

  return false;
}

void setup()
{
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  Serial.println("GY271 TEST");
  Wire.begin();
  // lower I2C clock http://www.gammon.com.au/forum/?id=10896
  TWBR = 78;  // 25 kHz 
  TWSR |= _BV (TWPS0);  // change prescaler  
}

void loop()
{
  bool detect = detectHMC5883L();

  if(!haveHMC5883L) 
  {
    if(detect) 
    {
      haveHMC5883L = true;
      Serial.println("We have HMC5883L, moving on");
      // Put the HMC5883 IC into the correct operating mode
      Wire.beginTransmission(HMC5883L_ADDR); //open communication with HMC5883
      Wire.write(0x02); //select mode register
      Wire.write(0x00); //continuous measurement mode
      Wire.endTransmission();
    }
    else
    {  
      Serial.println("No HMC5883L detected!");
      delay(2000);
      return;
    }
  }
  else
  {
    if(!detect) {
      haveHMC5883L = false;
      Serial.println("Lost connection to HMC5883L!");
      delay(2000);
      return;
    }
  }
  
  int x,y,z; //triple axis data

  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC5883L_ADDR);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();

 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC5883L_ADDR, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.print(z);
  float angle=atan2(y,x) * (180 / PI) + 180; 
  Serial.print("    Current Angle: ");
  Serial.println(angle);
  
  delay(250);
}

```

#### 3.3 测试结果

波特率设置为9600，打开串口获取当前3轴磁场强度和方位角。

![](C:\Users\liuyang\Documents\GitHub\2018-EducationalRobot-PosionModule\img\gy271ans.png)


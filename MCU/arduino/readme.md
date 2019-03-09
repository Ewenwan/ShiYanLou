# arduino 
[北京创客读书会，Beginning Arduino读书会授课代码 光 声音 通信 显示器 电机舵机 单总线 spi pwm脉冲 以太网 物联网](https://github.com/Ewenwan/maker)

[arduino IOT物联网编程 ](https://github.com/PacktPublishing/Mastering-IoT-with-Arduino)

[Arduino 大师](https://github.com/PacktPublishing/Mastering-Arduino)

[Programming Arduino: Getting Started with Sketches 示例](https://github.com/Ewenwan/programming_arduino_ed2)

[arduino_course_es 课程 课件](https://github.com/simonmonk/arduino_course_es)

[CC1101_arduino 无线模块](https://github.com/simonmonk/CC1101_arduino)

[Arduino + ESP8266物联网模块 连接无线热点 设置无限热点 套接字和WEB编程等](https://github.com/hujiese/ESP8266-Arduino)

[arduino加显示器制作的小游戏 ](https://github.com/Ewenwan/game-maker)

# Ardunio 简介
Arduino 是一款比较流行的开源硬件，相对于传统的硬件平台，Arduino所具有的优势就是采用 Creative Commons 许可。 Creative Commons（CC）是为保护开放版权行为而出现的类似GPL的一种许可（license）。在 Creative Commons许 可下，任何人都被允许生产电路板的复制品，还能重新设计，甚至销售原设计的复制品。Ardunio 其实是包含硬件部分（各种型号的Arduino板）和软件部分（Arduino IDE)。

## Arduino板
Ardunio 开发板众多，仅主板就有各种系列，还有各种对应扩展板和传感器。Ardunio 基本都是采用 Atmel 公司的16位及32位芯片。下面是几款主要的Ardunio 发板：

### Arduino Uno
![](https://github.com/sundway/iot/blob/master/imgs/1.2-1.jpg)

广受青睐的 Arduino Uno 开发板是以 ATmega328 MCU 控制器为基础。而 ATmega328 是一款低功耗，高性能的 AVR® 8位微处理器，这款芯片采用的是精简指令集计算机（RISC）架构，4/8/16/32K
字节系统内可编程闪存。Arduino Uno 开发板所具有的特性都是来自于 ATmega328 这款芯片。更多特性可以查看 [ATmega328官方数据手册](http://www.alldatasheet.com/view.jsp?Searchword=ATMEGA328)。


### Arduino Nano
![](https://github.com/sundway/iot/blob/master/imgs/1.2-9.jpg)

Arduino Nano 是一款基于 ATmega328(Arduino Nano 3.x) 或 ATmega168(Arduino Nano2.x)的开发卡，体积小巧、功能全面且适用于电路板。

### Arduino Due
![](https://github.com/sundway/iot/blob/master/imgs/1.2-3.jpg)

与前两款不同，Arduino Due 是基于 ATSAM3X8EA 芯片。而 ATSAM3X8EA 是32位 ARM Cortex-M3 RISC 处理器，Cortex－M3 处理器采用 ARMv7-M 架构。更多细节可以查看[ATSAM3X8EA](http://html.alldatasheet.com/html-pdf/475970/ATMEL/ATSAM3X8EA-AU/318/2/ATSAM3X8EA-AU.html).

### Arduino Mega(2560)
![](https://github.com/sundway/iot/blob/master/imgs/1.2-6.jpg)

Arduino Mega 采用 ATmega2560 作为核心处理器。ATmega2560 是一款256K字节系统内可编程闪存的微处理器，这里不再做过多介绍，更多内容可以看[数据手册](http://html.alldatasheet.com/html-pdf/107092/ATMEL/ATMEGA2560/153/1/ATMEGA2560.html)

### Arduino Zero
![](https://github.com/sundway/iot/blob/master/imgs/1.2-11.png)

Arduino Zero 采用的 SAMD21 作为主控制器，而SAMD21 是32位ARM Cortex M0+ 内核，CortexM0 其核心架构为 ARMv6M，关于 ARM 架构会在 ARM 章节进行讲解。Arduino Zero 是 Atmel 与 Arduino 合作推出 Zero 开发板，它是一款简洁、优雅、功能强大的32位平台扩展板。附上[SAMD20手册](http://html.alldatasheet.com/html-pdf/554411/ATMEL/ATSAMD20E14A-AU/322/2/ATSAMD20E14A-AU.html)

### Arduino Yún
![](https://github.com/sundway/iot/blob/master/imgs/1.2-4.jpg)

ArduinoYún的比较独特，采用了 ATmega32U4 处理器，同时还带有 AtherosAR9331（wifi模块），而主芯片则和 Arduino Leonardo 相同。不同的是Yún板具备内置以太网和Wi-Fi支持器，Yún还可以与板上Linux分配通信，Arduino带来了功能强大的联网计算机。

![](https://github.com/sundway/iot/blob/master/imgs/1.2-13.png)
以上是 Ardunio 的系列产品，除此之外还有很多已经废弃的产品，更多 Ardunio 产品看查看[Arduino Products](https://www.arduino.cc/en/Main/Products)

## Ardunio IDE
Ardunio IDE 同时支持 windows、linux、mac 三种平台，适用于任何 Ardunio 板。具体安装和使用方法可以参照官方[Getting Started with Arduino](https://www.arduino.cc/en/Guide/HomePage)， Ardunio IDE 提供了自己特有的[语法](https://www.arduino.cc/en/Reference/HomePage).很多人喜欢用 Sublime Text ，没有关系，安装 Ardunio 的插件，和 Ardunio IDE 具有的功能类似。以下是 Ardunio IDE 的界面：
![](https://github.com/sundway/iot/blob/master/imgs/1.2-12.png)



# 1. 软件安装及教程资源链接

[Windows软件下载](https://coding.net/u/coloz/p/arduino-installer/git/raw/master/1.8.5/arduino-1.8.5-windows.exe) 

[Mac os软件下载](https://coding.net/u/coloz/p/arduino-installer/git/raw/master/1.8.5/arduino-1.8.5-macosx.zip) 

[驱动安装方法](https://www.arduino.cn/thread-1008-1-1.html)

[Arduino教程汇总](https://www.arduino.cn/thread-1066-1-1.html)

[Arduino官网](http://www.arduino.cc/)

# 2. Arduino语言

1. **Arduino使用C/C++编写程序**，虽然C++兼容C语言，但这是两种语言，C语言是一种面向过程的编程语言，C++是一种面向对象的编程语言。早期的Arduino核心库使用C语言编写，后来引进了面向对象的思想，目前最新的Arduino核心库采用C与C++混合编写而成。  

2. 传统开发方式中，你需要厘清每个**寄存器**的意义及之间的关系，然后通过配置多个寄存器来达到目的。  而在Arduino中，使用了清楚明了的**API**替代繁杂的寄存器配置过程，如以下代码： `pinMode(13,OUTPUT);digitalwrite(13,HIGH)`，`pinMode(13,OUTPUT)`即是设置引脚的模式，这里设定了13脚为输出模式；而`digitalWrite(13,HIGH) `是让13脚输出高电平数字信号。 

3. 如果你使用过C/C++语言，你会发现Arduino的程序结构与传统的C/C++结构的不同——Arduino程序中没有main函数。  其实并不是Arduino没有main函数，而是main函数的定义隐藏在了Arduino的核心库文件中。Arduino开发一般不直接操作main函数，而是使用`setup()`和`loop()`这个两个函 数。

   Arduino控制器通电或复位后，**即会开始执行`setup() `函数中的程序，该部分只会执行一次**。 通常我们会在`setup() `函数中完成Arduino的初始化设置，如**配置I/O口状态，初始化串口等操作。** 

   在`setup()` 函数中的程序执行完后，Arduino会接着执行`loop()` 函数中的程序。而`loop()`函数是一个死循环，其中的程序会不断的重复运行。 通常我们会在`loop() `函数中完成程序的主要功能，如**驱动各种模块，采集数据**等。

4. C/C++编程基础（补充）

   ```c
   常量定义：const 类型 常量名 = 常量值
   布尔型变量，即boolean。它的值只有两个：false（假）和true（真）。boolean会占用1个字节的内存空间。
   ```
# 3. 示例总结
   
# io
```c
// LED 延时 闪烁=================
#define LED_ONE 13

void setup() {
  pinMode(LED_ONE, OUTPUT);
}

void loop() {
  digitalWrite(LED_ONE, HIGH);   
  delay(500);                       
  digitalWrite(LED_ONE, LOW);    
  delay(500); 
}

// pwm 呼吸灯======================
#define LED_ONE 11 //具有pwm功能

int val = 0;
int change = 5;         

void setup()
{
  pinMode(LED_ONE, OUTPUT);   
}

void loop()
{
  val += change;
  if (val > 250 || val < 5) {
    change *= -1;
  }
  analogWrite(LED_ONE, val); // 0~255/255 * 5v
  delay(100);
}

//========面对对象编程===========================
// led.h ==================
#ifndef LED_H
#define LED_H

#define LED_ONE 3   // 端口
#define LED_TWO 11

class Led{
  int ledPin;
  long onTime;  // 点亮时间
  long offTime;
  public:
    Led(int pin, long on, long off);// 类构造函数
    void blinkLed();
    void turnOn();
    void turnOff();
};

#endif

// led.cpp ==================
#include "Arduino.h"
#include "led.h"

// 类构造函数====
Led::Led(int pin, long on, long off) {
  ledPin = pin;
  pinMode(ledPin, OUTPUT);     
    
  onTime = on;
  offTime = off;
}
// 类方法--灯闪烁
void Led::blinkLed() {
  this->turnOn();
  delay(onTime);                       
  this->turnOff();   
  delay(offTime); 
}
// 关
void Led::turnOn() {
  digitalWrite(ledPin, HIGH);  
}
// 开
void Led::turnOff(){
  digitalWrite(ledPin, LOW); 
}

// =====使用类对象==============
#include "led.h"
Led led(LED_ONE, 1000, 500); // 创建led类 对象===
void setup() {
  // put your setup code here, to run once:
}
void loop() {
  // put your main code here, to run repeatedly:
  led.blinkLed();// 调用类的 公有方法
}

//  按键输入=========================================
#define BUTTON_ONE 12 // 按键接口 管脚

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // 串口
  pinMode(BUTTON_ONE, INPUT);  // 管脚输入 模式
  
}

void loop() {
// 未做消抖处理=====
  // put your main code here, to run repeatedly:
  int val = digitalRead(BUTTON_ONE);  // 读取端口状态
  if (val == HIGH) {         
    Serial.println("Button HIGH");// 高电平
  } else {
    Serial.println("Button LOW"); // 低电平
  }
}


// 模拟量检测 温度测量 结构体================================
#define TEMP_PIN 5
// 温度结构体
struct tmp36_reading {
  double voltage;
  double tempC;
  double tempF;
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  struct tmp36_reading temp;
  int pinValue = analogRead(TEMP_PIN);// 读取模拟量端口值
  temp.voltage = pinValue * 0.0049;
  temp.tempC = (temp.voltage - .5) * 100.0; // 摄氏度
  temp.tempF = (temp.tempC * 1.8) + 32;     // 华氏度

  showTemp(temp);
  
  delay(2000);
}
// 串口打印信息
void showTemp(struct tmp36_reading temp) {
  Serial.print(temp.tempC);
  Serial.print("  -  ");
  Serial.println(temp.tempF);
}

// ==========联合体=================================
union some_data {
  int i;
  double d;
  char s[20];
};

void setup() {
  Serial.begin(9600);
  union some_data myData;
  myData.i = 42;
  myData.d = 3.14;
  strcpy( myData.s, "Arduino");
  Serial.println(myData.s);
  Serial.println(myData.d);
  Serial.println(myData.i);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// 开关量检测 ===================================
#define MOTION_SENSOR 3 // 开关量输入===
#define LED 5           // 开关量输出===

void setup() {
  pinMode(MOTION_SENSOR, INPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(MOTION_SENSOR);// 开关量输入测量
  if (sensorValue == HIGH) {
    Serial.println("Motion Detected");
  }

  digitalWrite(LED, sensorValue);// 开关来输出
  delay(500);
}


```

# 通信

```c
//  单总线协议 DHT 温湿度传感器=======================================
// https://github.com/PacktPublishing/Mastering-Arduino/tree/master/Chapter%209/Code
#include "DHT.h"

#define DHT_PIN 3
#define RAIN_PIN A2
#define DHT_TYPE DHT11
#define RAIN_SENSOR_MAX 1024
#define RAIN_SENSOR_MIN 0
#define RAIN_OUT_MAX 20
#define RAIN_OUT_MIN 0

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float celsius = dht.readTemperature();
  float fahreheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(celsius) || isnan(fahreheit)) {
    Serial.println("Read Failed");
    return;
  }

 // float hif = dht.computeHeatIndex(fahreheit, humidity);
 // float hic = dht.computeHeatIndex(celsius, humidity, false);

  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" *C ");
  Serial.print(fahreheit);
  Serial.println(" *F");

  int rain = analogRead(RAIN_PIN);
  if (isnan(rain)) {
    Serial.println("Read Failed");
    return;
  }
  int range = map(rain, RAIN_SENSOR_MIN, RAIN_SENSOR_MAX, RAIN_OUT_MAX, RAIN_OUT_MIN);
  Serial.print("Rain: ");
  Serial.println(range);

  Serial.println("-------------------------");
  delay(3000);
  /*
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println(" *C ");
  Serial.print(hif);
  Serial.printlnln(" *F");
  */

}


// ==============SPI显示屏====================================
// https://github.com/PacktPublishing/Mastering-Arduino/blob/master/Chapter%2013/Sketch/Nokia5110/Nokia5110.ino
// Nokia5110 <Adafruit_PCD8544.h>


// 软件串口--蓝牙通信
//  https://github.com/PacktPublishing/Mastering-Arduino/blob/master/Chapter%2020/BluetoothLELed/BluetoothLELed.ino

```

# Arduino UNO硬件介绍

本想在这一章会对照开发板简单讲解一下硬件原理图，所以如果你想深入了解硬件原理图，这里附上官网的[PDF版](https://www.arduino.cc/en/uploads/Main/Arduino_Uno_Rev3-schematic.pdf) 。下图是一张原理图和开发板的概览图：

![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-1.png)

## 稳压电路

![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-3.png)

外接电源输入进来的电压可能在5V附近，为了保证更加精准的5V电压，防止电压过高穿透芯片。这里选用的是 NCP117ST50T3G 稳压芯片，这款稳压芯片输出电压可调范围是1.5-12V。稳压电路的连接方式及计算公式可查[此链接](http://html.alldatasheetcn.com/html-pdf/174874/ONSEMI/NCP1117ST50T3G/1826/8/NCP1117ST50T3G.html)。

## 下载电路

![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-4.png)
![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-5.png)

这块 UNO 板采用 Atmega16U2 芯片做下载电路，一般我们可能需要单独的下载模块下载程序，但此开发板直接将下载电路集成到了开发板上。这里直接可连 USB 线进行下载，像 Ardunino mini 这种小系统板是需要单独的下载模块的。

## 主芯片最小系统

![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-6.png)
![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-7.png)

如上图，这里可以独立出 UNO 的最小系统板，我们编程最关心的就是这个最小系统的 GPIO 接口。晶振的选择几个元器件的选择可参考 Atmega328 数据手册。下图是我们经常操作的一些管脚，比如，我们下一章节将要用的的开发板中的13管脚，它对应的是芯片的19管脚，B端口的第五个管脚，而除了作为普通的 IO 管脚外，它的第二功能就是SCK（同步时钟），SPI通信中作为时钟信号。
![](https://github.com/sundway/iot/blob/master/imgs/1.2.1-8.png)



# 点亮LED灯

上一节介绍了 Ardunio UNO 的硬件结构，这一节将会写一个 UNO 的 “hello word” 程序，在软件领域通常会以输出 “hello word” 作为开始，而在硬件领域通常是以点亮 LED 灯作为开始。

## 搭建环境
首先，在编写程序之前，需要下载编译环境和安装驱动。这里以mac为例，具体的下载过程可以参考官方的[Getting Started w/ Arduino on Mac OS X](https://www.arduino.cc/en/Guide/MacOSX)。从官方下载IDE时可能需要先捐献才能下载，为了表示对IDE的支持，我捐献了两美元。当然如果你没有信用卡或者其他的合适的支付手段，这里提供一个第三方的[下载平台](http://www.maczapp.com/arduino)。按照上述方法安装完毕之后，需要再安装串口驱动，[下载链接](http://www.ftdichip.com/Drivers/VCP/MacOSX/FTDIUSBSerialDriver_v2_2_18.dmg)。驱动安装完毕之后，可以检查驱动是否安装成功。插上连接线，输入```ls /dev/tty.*```

![](https://github.com/sundway/iot/blob/master/imgs/1.2.2-1.png)

这里是“/dev/tty.usbmodem1421”，驱动安装成功。

## 更改配置
在编写程序之前，还需要更改一些配置项：

第一步，选择对应的开发板，这里选用的是 UNO 开发板。

![](https://github.com/sundway/iot/blob/master/imgs/1.2.2-2.png)

第二部，选择对应的下载端口，上面打印的端口是“/dev/tty.usbmodem1421”，这里选择它。

![](https://github.com/sundway/iot/blob/master/imgs/1.2.2-3.png)

## 编写程序
Ardunio 采用自己的一套语言规范，程序可以被分解为三个主要部分：结构、值（变量和常量）以及函数。
一般结构中包含两个函数：“setup()”、“loop()”。“setup()” 是在“sketch”开始时被调用的，用于初始化变量、管脚、开始使用库文件等等。这个函数只会在开发板上电或复位时被调用一次。而“loop()”函数则是在“setup()”函数执行完毕后，一直循环执行。点亮 LED 的思路是只需要在程序开始时，将 LED 置于点亮状态。上一章可以看到，UNO 开发板的13管脚连接在 LED 的灯的一端，而 LED 的灯另一端连接到了低电平，也就是说，只需在初始化时，给13管脚上赋上高电平便可完成操作。

```
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  // turn the LED on (HIGH is the voltage level)
   digitalWrite(13, HIGH);
}

// the loop function runs over and over again forever
void loop() {
    
}
```

编译下载到 UNO 板中，Done，效果如下：

![](https://github.com/sundway/iot/blob/master/imgs/1.2.2-4.png)



# arduino 

[arduino IOT物联网编程 ](https://github.com/PacktPublishing/Mastering-IoT-with-Arduino)

[Arduino 大师](https://github.com/PacktPublishing/Mastering-Arduino)

[Programming Arduino: Getting Started with Sketches 示例](https://github.com/Ewenwan/programming_arduino_ed2)

[arduino_course_es 课程 课件](https://github.com/simonmonk/arduino_course_es)

[CC1101_arduino 无线模块](https://github.com/simonmonk/CC1101_arduino)

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


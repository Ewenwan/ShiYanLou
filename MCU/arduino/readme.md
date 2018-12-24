# arduino 

[arduino IOT物联网编程 ](https://github.com/PacktPublishing/Mastering-IoT-with-Arduino)

[Arduino 大师](https://github.com/PacktPublishing/Mastering-Arduino)


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


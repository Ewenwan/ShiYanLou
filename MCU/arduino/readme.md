# arduino 

[arduino IOT物联网编程 ](https://github.com/PacktPublishing/Mastering-IoT-with-Arduino)

[Arduino 大师](https://github.com/PacktPublishing/Mastering-Arduino)


# io
```c
// LED 延时 闪烁
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

// pwm 呼吸灯===
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

//========面对对象编程
// led.h ====
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

// led.cpp ============
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

// =====使用类对象====
#include "led.h"
Led led(LED_ONE, 1000, 500); // 创建led类 对象===
void setup() {
  // put your setup code here, to run once:
}
void loop() {
  // put your main code here, to run repeatedly:
  led.blinkLed();// 调用类的 公有方法
}

```

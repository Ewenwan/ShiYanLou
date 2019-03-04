/*
1个外接LED 与 板载 13pin上的LED指示灯的跑马灯效果。

主要内容：程序基本结构、数字信号输出
目标：
基本目标：交替闪烁。
扩展目标：设计不同的闪烁效果，比如：模拟音乐节拍、鼓点节奏。

说明：
外接LED：12pin
板载LED指示灯：13pin

就基本目标交替闪烁而言：
只需要使用数字信号输出digitalWrite 交替控制 输出到LEDpin口的电压为HIGH或LOW即可，
其中HIGH为高电压（5V），此时LED灯亮，LOW为低电压（0V），此时LED灯灭。
流程：12pin口LED灯亮，1s后灭，再过1s，13pin口LED灯亮，1s后灭，重复上述循环。

对于扩展目标：设计不同的闪烁效果，比如：模拟音乐节拍、鼓点节奏。

我采用的是用 随机函数random 来控制哪个pin口的LED灯，为什么要random呢？
一般内置的闪烁效果只会有一种或少数几种，而使用random产生的闪烁效果就不同了，
理论上是有无数多种的:)流程：先通过random(0,10)产生0-9的随机数字，
如果是偶数，12pin口的外接LED灯亮1s，如果是奇数，则板载13pin上的LED指示灯亮1s。

*/
#define EXTERN_LED 12  // 外界LED灯
#define BUILTIN_LED 13 // 板载LED灯 

void setup() {
  // 输出模式
  pinMode(EXTERN_LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  // 产生随机数
  int choose = random(0,10);
  if(choose%2==0)
  {// 偶数情况
    digitalWrite(EXTERN_LED, HIGH);
    delay(1000);
    digitalWrite(EXTERN_LED, LOW);
    delay(1000);
    }
  else
  {// 奇数情况
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
    }
}

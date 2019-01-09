# Arduino - 看门狗定时器（WDT：Watch Dog Timer）
[参考](https://blog.csdn.net/sdlgq/article/details/50396518)

        看门狗定时器（WDT：Watch Dog Timer）实际上是一个计数器。 
        一般给看门狗一个大数，程序开始运行后看门狗开始倒计数。 
        如果程序运行正常，过一段时间CPU应该发出指令让看门狗复位，令其重新开始倒计数。 
        如果看门狗计数减到0，就认为程序没有正常工作（因为没有及时复位），就强制整个系统复位（单片机重启）。

        所以，当你开启看门狗后，需要在看门狗超时（计数减到0）前，对其进行 喂狗（复位）操作，否则看门狗会强制你的单片机重启，从头运行程序。

        如果看门狗在休眠或空闲模式下超时，器件将唤醒并从PWRSAV指令执行处继续执行代码，同时“休眠”状态位（RCON<3>）或“空闲”状态位（RCON<2>）会置1，表示器件之前处于省电模式。

        功能作用：看门狗可以在你的程序陷入死循环的时候，让单片机复位而不用整个系统断电，从而保护你的硬件电路。

        使用看门狗需要引用头文件 【 avr/wdt.h 】，在wdt.h中，提供了3个看门狗API： 
        wdt_enable(timeout) //看门狗启动，并设置超时时间 
        wdt_disable() //看门狗停止 
        wdt_reset() //看门狗复位（喂狗）

        wdt_enable(timeout) 中timeout为超时时间，当超过这个时间后没有喂狗，则单片机重启。 
        这个时间可使用如下常量： 
        // 0=15(16)ms, 1=30(32)ms,2=60(64)ms,3=120(128)ms,4=250ms,5=500ms 
        // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec 
        0、WDTO_15MS： 看门狗定时器15ms超时 
        1、WDTO_30MS： 看门狗定时器30ms超时 
        2、WDTO_60MS：看门狗定时器60ms超时 
        3、WDTO_120MS： 看门狗定时器120ms超时 
        4、WDTO_250MS： 看门狗定时器250ms超时 
        5、WDTO_500MS： 看门狗定时器500ms超时 
        6、WDTO_1S： 看门狗定时器1S超时 
        7、WDTO_2S： 看门狗定时器2S超时 
        8、WDTO_4S： 看门狗定时器4S超时 
        9、WDTO_8S： 看门狗定时器8S超时

使用看门狗很简单，只需要做下面三步即可：

      1、引用头文件 #include avr/wdt.h

      2、Setup函数中启动看门狗，并设置超时时间为两秒：wdt_enable(WDTO_2S);

      3、Loop函数中喂狗，防止饿死（重启）： wdt_reset();

代码如下：
```c
#include <avr/wdt.h>  

int ledPin = 13;

void setup() {  
  pinMode(ledPin, OUTPUT);   
  //启动看门狗，设置喂狗时间不能超过2秒  
  wdt_enable(WDTO_2S);      
}  

void loop()  
{  
  digitalWrite(ledPin, HIGH);    
  delay(500);     
  digitalWrite(ledPin, LOW);    
  //喂狗。如果超过2S没有喂狗，则单片机重启。 
  //也就是说，如果本循环执行时间超过2S的话，单片机就会自动重启。
  wdt_reset();    
} 
```
      其它应用： 
      【利用看门狗进行休眠唤醒】 
      用下面的代码，代替wdt_enable()，并且不要喂狗。 
      这样就实现了看门狗超时后，执行唤醒函数，而不是重启单片机。

```c
void wdt_setup(int ii) {
      // ii为看门狗超时时间，支持以下数值：
      // 0=16毫秒, 1=32毫秒,2=64毫秒,3=128毫秒,4=250毫秒,5=500毫秒
      // 6=1秒 ,7=2秒, 8=4秒, 9=8秒
      byte bb;
      if (ii > 9 ) ii = 9;
      bb = ii & 7;
      if (ii > 7) bb |= (1 << 5);
      bb |= (1 << WDCE);
      //开始设置看门狗中断   
      MCUSR &= ~(1<<WDRF);  //清除复位标志
      WDTCSR |= (1<<WDCE) | (1<<WDE);
      //设置新的看门狗超时时间
      WDTCSR = bb;
      //设置为定时中断而不是复位
      WDTCSR |= _BV(WDIE); 
      //别忘了设置【看门狗唤醒执行函数】
}
```
看门狗唤醒执行函数：

ISR(WDT_vect){
                  //唤醒后执行的代码
}

  
  

// arduino要测量脉宽时间很简单。有专门的库函数pulseIn( )。
// 问题在于这个库函数使用查询方式，程序在测量期间一直陷在这里，CPU利用率太低。
// 因此下面代码采用中断方式，效率很高。
//
//read PPM signals from 2 channels of an RC reciever
//http://arduino.cc/forum/index.php/topic,42286.0.html
 
//接收机两个通道分别接arduino的数字口2、3脚
//Most Arduino boards have two external interrupts: numbers 0 (on digital pin 2) and 1 (on digital pin 3).
//The Arduino Mega has an additional four: numbers 2 (pin 21), 3 (pin 20), 4 (pin 19), and 5 (pin 18).
int ppm1 = 2;  
int ppm2 = 3;
 
unsigned long rc1_PulseStartTicks,rc2_PulseStartTicks;       
volatile int rc1_val, rc2_val;  
 
void setup() {
 
        Serial.begin(9600);  
 
        //PPM inputs from RC receiver
        pinMode(ppm1, INPUT);
        pinMode(ppm2, INPUT);
 
        // 电平变化即触发中断
        attachInterrupt(0, rc1, CHANGE);   
        attachInterrupt(1, rc2, CHANGE);   
}
 
void rc1()
{
        // did the pin change to high or low?
        if (digitalRead( ppm1 ) == HIGH)
                rc1_PulseStartTicks = micros();    // store the current micros() value
        else
                rc1_val = micros() - rc1_PulseStartTicks;
}
 
void rc2()
{
        // did the pin change to high or low?
        if (digitalRead( ppm2 ) == HIGH)
                rc2_PulseStartTicks = micros();   
        else
                rc2_val = micros() - rc2_PulseStartTicks;
}
 
void loop() {
 
        //print values
        Serial.print("channel 1:  ");
        Serial.print(rc1_val);  
        Serial.print("        ");
        Serial.print("channel 2:  ");   
        Serial.println(rc2_val);  
}

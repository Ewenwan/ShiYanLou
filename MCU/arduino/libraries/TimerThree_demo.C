#include <PWM.h>
//#include "TimerThree/TimerThree.h"

#define PWM_left         11  // 左轮电机 pwm 口
#define PWM_right        12  // 右轮电机 PWM 口
#define DIR_left         22 // 左轮方向 IO口
#define DIR_right        23 // 右轮方向 IO口
#define Frequence     18000 // PWM 波频率

#define RESOLUTION 65536    // Timer3 is 16 bit
unsigned char clockSelectBits;
void Timer3SetPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  ICR3 = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  TCCR3B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR3B |= clockSelectBits;                                                     // reset clock select register
}
void Timer3Init(long microseconds)
{
  TCCR3A = 0;                 // clear control register A 
  TCCR3B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer
  Timer3SetPeriod(microseconds);
}

void setup() {
   pinMode(13, OUTPUT);
  // put your setup code here, to run once:
  //// 电机方向口/////// 
   pinMode(DIR_left, OUTPUT);   // 左轮
   digitalWrite(DIR_left, LOW);   
   pinMode(DIR_right, OUTPUT);  // 右轮
   digitalWrite(DIR_right, LOW);// LOW倒退   HIGH 前进 
//// 电机PWM口 //////
   //SetPinFrequency(PWM_right, Frequence);   //右电机控制频率初始化
   //pwmWriteHR(PWM_right,15000);              //右电机控制占孔比初始化
   //SetPinFrequency(PWM_left, Frequence);    //左电机控制频率初始化
   //pwmWriteHR(PWM_left,15000);                //左电机控制占孔比初始化
   
  //Timer3.initialize(500000);         // initialize timer1, and set a 1/2 second period
  //Timer3.attachInterrupt(callback);
  Timer3Init(500000);
  TIMSK3 = _BV(TOIE1);                      // sets the timer overflow interrupt enable bit
  sei();                                    // ensures that interrupts are globally enabled
  TCCR3B |= clockSelectBits;
  
}

ISR(TIMER3_OVF_vect)   // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  static boolean output = HIGH;
  digitalWrite(13, output);
  output = !output;
}

void loop() {
 // digitalWrite(DIR_left, LOW);
 // pwmWriteHR(PWM_right, 1000);          //右电机控制占孔比初始化
  // put your main code here, to run repeatedly:
   //SetPinFrequency(PWM_right, Frequence);   //右电机控制频率初始化
  // pwmWriteHR(PWM_right,15000);              //右电机控制占孔比初始化

}



#include <PID_class.h>

PID car_pid;
volatile bool Left_Direction=0,Right_Direction=0;
volatile long timer=0,pub_countL=0,pub_countR=0,pluse=0;  
#define En_CHL           2  // 编码器计数口  左轮 对应中断0 
#define En_CHR           3  // 编码器计数口  右轮 对应中断1

// #define Frequence     18000 // PWM 波频率

///// 右轮 编码器计数////
void interruptR()                      
{
   car_pid.countR += 1;
}
////// 左轮 编码器计数////
void interruptL()
{
   car_pid.countL += 1;
}


void Control_Period() 
{
   car_pid.PID_cacu_control();                           
  // pub_countL += car_pid.countL;
  // pub_countR += car_pid.countR;
   car_pid.countR = car_pid.countL = 0;
  // timer++;                
}

void setup()
{
   ////编码器中断引脚初始化/////  
   pinMode(En_CHL, INPUT);//设置中断脚为输入状态
   pinMode(En_CHR, INPUT);// 
   // 上升沿触发 
   attachInterrupt(0, interruptL, FALLING);  // step0  input on interrupt 1 - pin 2
   attachInterrupt(1, interruptR, FALLING);  // step1  input on interrupt 1 - pin 3
   
  // 电机 pid 10ms 中断 外加 超声测量 编制设定 
   FlexiTimer2::set(10, 1.0/1000, Control_Period); // call every 10 1ms "ticks" 
   FlexiTimer2::start();
}

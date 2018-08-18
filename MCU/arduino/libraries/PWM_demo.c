
#include <PWM.h>

#define PWM_left         11  // 左轮电机 pwm 口   6  TIMER3 pwm频率库肯不支持
#define PWM_right        12  // 右轮电机 PWM 口   7
#define DIR_left         22  // 左轮方向 IO口
#define DIR_right        23  // 右轮方向 IO口

void setup(){

//// 电机方向口/////// 
   pinMode(DIR_left, OUTPUT);   // 左轮
   digitalWrite(DIR_left, LOW); // LOW 后退   HIGH向前  
   pinMode(DIR_right, OUTPUT);  // 右轮
   digitalWrite(DIR_right, LOW);  
//// 电机PWM口 //////
   SetPinFrequency(PWM_right, Frequence);   //右电机控制频率初始化
   pwmWriteHR(PWM_right, 0);                //右电机控制占孔比初始化
   SetPinFrequency(PWM_left, Frequence);    //左电机控制频率初始化
   pwmWriteHR(PWM_left,0);                  //左电机控制占孔比初始化
   
   }
   
 //// 主循环 //////
void loop()
{
  // tesk();
}

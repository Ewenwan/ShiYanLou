#ifndef PID_H
#define PID_H


#define PWM_SCOPE   65535
#define PWM_MAX      PWM_SCOPE*0.9
#define PWM_MIN      0
#define Infrared_Forward A0
#define Infrared_Left    A1
#define Infrared_Right   A2
#define Infrared_Back    A5

#define Left_Motor_pwm       10
#define Right_Motor_pwm     9

class PID     //定义类主体及类名
{
  public:  //以下定义为公共方法
     volatile long   Left_Output=0,Right_Output=0,
Left_Speedwant=0,Right_Speedwant=0;
     volatile unsigned long countL = 0;                                             //左电机脉冲值计数
     volatile unsigned long countR = 0;                                             //右电机脉冲值计数 
     volatile double Infrared_value[4]={0}; 
     void Infraed_read();
     void PID_cacu_control();
     void  Left_Speed_Limit();
     void  Right_Speed_Limit();
  private:  //以下定义为私有方法
     volatile long Left_Input=0, Err_Left[2]={0},Error_i_Left=0,Left_last=0;                 
//Left_Speedwant为左电机设定速度值，Left_Input为左电机实际速度
//Left_Output为左电机控制占空比
    const long Kp_Left=90, Ki_Left=10, Kd_Left=5;                                
//左电机PID参数

    volatile long Right_Input=0,Err_Right[2]={0},Error_i_Right=0,Right_last=0;           
//Right_Speedwant为右电机设定速度值，Right_Input为右电机实际速度 
//Right_Output为右电机控制占空比
    const long Kp_Right=90, Ki_Right=10, Kd_Right=5;                         
//右电机PID参数
    
};




#endif



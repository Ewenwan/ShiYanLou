#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include<PID.h>
#include<PWM.h>
void  PID::Left_Speed_Limit()
{   
    if(Left_Output>PWM_MAX)  Left_Output=PWM_MAX;
    if(Left_Output<PWM_MIN)   Left_Output=PWM_MIN;      
  }
  
  void  PID::Right_Speed_Limit()
{   if(Right_Output>PWM_MAX)  Right_Output=PWM_MAX;
    if(Right_Output<PWM_MIN)  Right_Output=PWM_MIN;      
  }
void PID::Infraed_read()
{
        Infrared_value[0]=analogRead(Infrared_Forward);        //AD读取一次数据
        Infrared_value[0]=13146*pow(Infrared_value[0],-1.161);
        /**/if(Infrared_value[0]>=99)
           {
                Infrared_value[0]=99;
              }
           else if  (Infrared_value[0]<=10)
	   {
                Infrared_value[0]=10;
              }
              
        Infrared_value[1]=analogRead(Infrared_Left);        //AD读取一次数据
        Infrared_value[1]=13146*pow(Infrared_value[1],-1.161);
        if(Infrared_value[1]>=99)
           {
                Infrared_value[1]=99;
            }
            else if (Infrared_value[1]<=10){
                Infrared_value[1]=10;
              } 

         Infrared_value[2]=analogRead(Infrared_Right);        //AD读取一次数据
        Infrared_value[2]=13146*pow(Infrared_value[2],-1.161);
        if(Infrared_value[2]>=99)
           {
                Infrared_value[2]=99;
            }
            else if (Infrared_value[2]<=10){
                Infrared_value[2]=10;
              } 
              
        Infrared_value[3]=analogRead(Infrared_Back);        //AD读取一次数据
        Infrared_value[3]=(10500*Infrared_value[3]/8192);   //电池电压转换
        if(Infrared_value[3]>=1500)
           {
                Infrared_value[3]=1200;
            }
            else if (Infrared_value[3]<=10){
                Infrared_value[3]=1200;
              } 

  }

 void PID::PID_cacu_control()
 {
               Left_Input=countL; 
               Err_Left[0]=Left_Speedwant-Left_Input;
               Error_i_Left+=Err_Left[0];
		Err_Left[1]=Err_Left[0];

               Left_Output=Kp_Left*Err_Left[0]+Kd_Left*(Err_Left[0]-Err_Left[1])+Ki_Left*Error_i_Left;
               
               Left_Speed_Limit() ;
               Left_last=Left_Output;

               Right_Input=countR; 
               Err_Right[0]=Right_Speedwant-Right_Input;
               Error_i_Right+=Err_Right[0];
		Err_Right[1]=Err_Right[0];

               Right_Output=Kp_Right*(Err_Right[0])+Kd_Right*(Err_Right[0]-Err_Right[1])+Ki_Right*Error_i_Right;            
               
               Right_Speed_Limit() ;
               Right_last=Right_Output;
     
     
               if(Right_Speedwant==0)
               {
                     pwmWriteHR(Right_Motor_pwm, 0);                     //左电机控制频率初始化
                     Right_Output=0;
                     Err_Right[2]=Err_Right[1]=Err_Right[0]=0;
                     Error_i_Right=0;
                     Right_last=0;
                     
               }
               else
               {
                     pwmWriteHR(Right_Motor_pwm, Right_Output);                     //左电机控制频率初始化
               }
     
                if(Left_Speedwant==0)
               {
                    pwmWriteHR(Left_Motor_pwm, 0);                              //右电机控制占孔比初始化
                    Left_Output=0;
                     Err_Left[2]=Err_Left[1]=Err_Left[0]=0;
                     Error_i_Left=0;
                     Left_last=0;
               }
               else
               {
                    pwmWriteHR(Left_Motor_pwm, Left_Output);                     //右电机控制占孔比初始化
                }
  }





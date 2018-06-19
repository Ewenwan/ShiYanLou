#include "stm32f10x.h"
#include "PidContoller.h"
//#include "UserConfiguration.h"

extern s32 CNT2,CNT3,CNT4,CNT5,V2,V3,V4,V5;

//绝对式PID算法
void PID_AbsoluteMode(PID_AbsoluteType* PID)
{
 if(PID->kp      < 0)    PID->kp      = -PID->kp;
 if(PID->ki      < 0)    PID->ki      = -PID->ki;
 if(PID->kd      < 0)    PID->kd      = -PID->kd;
 if(PID->errILim < 0)    PID->errILim = -PID->errILim;

 PID->errP = PID->errNow;  //读取现在的误差，用于kp控制

 PID->errI += PID->errNow; //误差积分，用于ki控制

 if(PID->errILim != 0)	   //微分上限和下限
 {
  if(     PID->errI >  PID->errILim)    PID->errI =  PID->errILim;
  else if(PID->errI < -PID->errILim)    PID->errI = -PID->errILim;
 }
 
 PID->errD = PID->errNow - PID->errOld;//误差微分，用于kd控制

 PID->errOld = PID->errNow;	//保存现在的误差
 
 PID->ctrOut = PID->kp * PID->errP + PID->ki * PID->errI + PID->kd * PID->errD;//计算绝对式PID输出

}


/*******************************************************************************************************/



//增量式PID算法
void PID_IncrementMode(PID_IncrementType* PID)
{
 float dErrP, dErrI, dErrD;
 
 if(PID->kp < 0)    PID->kp = -PID->kp;
 if(PID->ki < 0)	PID->ki = -PID->ki;
 if(PID->kd < 0)    PID->kd = -PID->kd;

 dErrP = PID->errNow - PID->errOld1;

 dErrI = PID->errNow;

 dErrD = PID->errNow - 2 * PID->errOld1 + PID->errOld2;

 PID->errOld2 = PID->errOld1; //二阶误差微分
 PID->errOld1 = PID->errNow;  //一阶误差微分

 /*增量式PID计算*/
 PID->dCtrOut = PID->kp * dErrP + PID->ki * dErrI + PID->kd * dErrD;
 
 if(PID->kp == 0 && PID->ki == 0 && PID->kd == 0)   PID->ctrOut = 0;

 else PID->ctrOut += PID->dCtrOut;
}


/*****************************************电机速度环伺服***********************************************/

s32 spdTag, spdNow, control;//定义一个目标速度，采样速度，控制量

PID_AbsoluteType PID_Control;//定义PID算法的结构体

void User_PidSpeedControl(s32 SpeedTag)
{
   spdNow = V2; spdTag = SpeedTag;

   PID_Control.errNow = spdTag - spdNow; //计算并写入速度误差
   	
   PID_Control.kp      = 15;             //写入比例系数为15
   PID_Control.ki      = 5;              //写入积分系数为5
   PID_Control.kd      = 5;              //写入微分系数为5
   PID_Control.errILim = 1000;           //写入误差积分上限为1000 下限为-1000

   PID_AbsoluteMode(&PID_Control);       //执行绝对式PID算法
	
   control = PID_Control.ctrOut;         //读取控制值

   //UserMotorSpeedSetOne(control);        //放入PWM，用于收敛速度的控制中

}











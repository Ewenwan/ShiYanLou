#include "control.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
#include "can.h"
/************************************************
 
************************************************/

//全局变量声明
TCtrl            TForkLift[2];   //定义叉车结构体 数组
ptCtrl           ptForkLift;     //定义叉车结构体指针
ptCtrl           ptForkLift2;    //定义叉车结构体指针

#define ABS(x)		((x>0)? (x): (-x)) 

void abs_limit(float *a, float ABS_MAX){
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}

/*位置式PID算法*/
float POSITION_PID(ptCtrl ptCtrlCar, int CurrentSpeed)
{
	                         // 期望值  -               反馈值     -(10000 ~ 10000)   500转/分钟 * 19 
	ptCtrlCar->s16error[0] = ptCtrlCar->s16speedwant - (float) CurrentSpeed;
	
	if((ptCtrlCar->u16ErrorDeadBand != 0)&&(ABS(ptCtrlCar->s16error[0])<ptCtrlCar->u16ErrorDeadBand))
	return 0;
	
	//printf("ptCtrlCar->s16ErrorSum =  %f\r\n",ptCtrlCar->s16ErrorSum);		
	ptCtrlCar->s16speedout = ptCtrlCar->s16speed_p*ptCtrlCar->s16error[0] 
							+ ptCtrlCar->s16speed_d*(ptCtrlCar->s16error[0] - ptCtrlCar->s16error[1])
							+ ptCtrlCar->s16speed_i*ptCtrlCar->s16ErrorSum;
	
	ptCtrlCar->s16error[1] = ptCtrlCar->s16error[0];//上次误差	
	/******wyw加入为了得到 增量式PID*******/
	ptCtrlCar->s16error[2] = ptCtrlCar->s16error[1];//上上次误差	
	ptCtrlCar->s16ErrorSum += ptCtrlCar->s16error[0];
	
	abs_limit(&(ptCtrlCar->s16speedout), MAX_Carrent_out);//限幅
	return 1;
}

/*增量式PID算法*/
float ZL_PID(ptCtrl ptCtrlCar,int  CurrentSpeed)
{
	
	ptCtrlCar->s16error[0] = ptCtrlCar->s16speedwant - (float) CurrentSpeed;	
	if((ptCtrlCar->u16ErrorDeadBand != 0)&&(ABS(ptCtrlCar->s16error[0])<ptCtrlCar->u16ErrorDeadBand))
		return 0;
	
	ptCtrlCar->s16speedout += ptCtrlCar->s16speed_p*(ptCtrlCar->s16error[0] - ptCtrlCar->s16error[1])						
							+ ptCtrlCar->s16speed_i*ptCtrlCar->s16error[0]
							+ ptCtrlCar->s16speed_d*(ptCtrlCar->s16error[0] - 2 * ptCtrlCar->s16error[1] + ptCtrlCar->s16error[2]);
	
	ptCtrlCar->s16error[1] = ptCtrlCar->s16error[0];//上次误差
	ptCtrlCar->s16error[2] = ptCtrlCar->s16error[1];//上上次误差	
	
  abs_limit(&(ptCtrlCar->s16speedout), MAX_Carrent_out);//限幅
	return 1;
}


void set_moto_current( s16 iq1, s16 iq2, s16 iq3, s16 iq4){

	//ptForkLift->Can_Txmessage.StdId=0x102;		// 0X201...0x204四个电调接收的 电流设定值
	ptForkLift->Can_Txmessage.Data[0] = iq1 >> 8;//  左轮 201
	ptForkLift->Can_Txmessage.Data[1] = iq1;     //  
	ptForkLift->Can_Txmessage.Data[2] = iq2 >> 8;//  右轮 202
	ptForkLift->Can_Txmessage.Data[3] = iq2;
	ptForkLift->Can_Txmessage.Data[4] = 0 >> 8;
	ptForkLift->Can_Txmessage.Data[5] = 0;
	ptForkLift->Can_Txmessage.Data[6] = 0 >> 8;
	ptForkLift->Can_Txmessage.Data[7] = 0;
  Can_Send_Msg();  //发送消息到 电机的电调
}

#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define MAX_Carrent_out    15000   // -15000 ~15000 对应电机电流值  -18A ~ 18A    -16384~16384
#define CanBoxPost         1
#define CanBoxPend         0


typedef struct TControl//与控制相关的参数全部在此结构体中
{
//	u16      u16PWM;    //此处不是  pwm的值  是电机转子电流值  电流值越大转速越快 -15000~15000 对应 -18A~18A   -16384~16384
//	u8       bDrection;
	u8		   u8CarID;    //  
	
	u16     u16RunPIDControl;

	float	 s16error[3];
	float	 s16ErrorSum;
	float     s16speedwant;// 电机转子速度 -500*19.2  ~ 500*19.2 rpm
	float     s16speedout; // 电机电枢电流 -15000 ~ 15000
  u16     u16ErrorDeadBand;//误差忽略
	
	float     s16speed_p;
	float     s16speed_i;
	float     s16speed_d;
	int       pid_time;//ms计


	
	// 无刷电调 参数
u16 angle;        //转子角度  0~8191  >>>  0~360
u16 last_angle;

int   speed_rpm;    //反馈值 转子转速  rpm 转/分钟   -10000 ~ 10000 转子转速   轴转速 /19.2  减速比 3951/:187 =19.2  
s32   s32speed50ms; //转速和
float s16MeanSpeed;

u16   u16EncoderSum;//编码器和  0~65535 溢出后为 0

int   last_speed_rpm;

int   real_current;//


float tempature;

	//右手定则定义的车底盘速度
	float     s16x;           // +- 1.0~1.0   m/s
	float     s16y;           // +- 1.0~1.0   m/s
	float     s16w;           // +- 0.5~0.5   rad/s	
	
	
//*************CAN总线相关*********************//	
	u8       bCanComBox;        // can 状态标识
  u8			 bCanSendCounter;  //每发送一个包，这个值++
	u16	     u16CanComCount[2]; //u16CanComCount[0] can 中断 接收消息计数
	u16      u16CanComPeriod;   //记录系统时钟1ms中断次数  1~500 每100ms  500ms 判断一次  更新 can 中断计数 状态 
//u16	     u16RunDetADC;      //电机电流采样时间计数
	
	u16	     u16RunRecvPeriodMotor;
	u16	     u16RunSendPeriodMotor;
	CanTxMsg Can_Txmessage;
	//u8		   u8Can_Msg_Send_flag;
	
	u8		   u8Cmd_receive_lag;
	
	//u8       u8Can_speed_Msg_Send_flag;
	u16	     u16SerialComCount[2];//串口接收指令标志
	u8       u8SerialCmdFlag;
//*************CAN总线相关*********************//	
	
	u16      u16MotorControlFlag;
	

}TCtrl,*ptCtrl;


typedef struct Filter_Queue_node{
	
	s16 date;
	struct Filter_Queue_node* next;
	
}Filter_Queue, *queue_ptr;
  

typedef struct Linkqueue{

	Filter_Queue*  front;//前指针
	Filter_Queue*  rear;//后指针
	
}linkqueue;

/*
class Ctrl1
{
	public:
  
    
	protected:
  
};
*/



//extern TCtrl TForkLift;     //定义叉车结构体
extern TCtrl TForkLift[2];     //定义叉车结构体 数组
extern ptCtrl ptForkLift;   //定义叉车结构体指针
extern ptCtrl ptForkLift2;   //定义叉车结构体指针
//extern TCtrl RoboMaster[2];     //定义叉车结构体数组

void SetPwmDir(ptCtrl ptCtrlCar);
void SetPwmDir_new(ptCtrl ptCtrlCar);
float POSITION_PID(ptCtrl ptCtrlCar,int EncoderSpeed);
float ZL_PID(ptCtrl ptCtrlCar,int EncoderSpeed);
void uart_init(u32 bound);
void set_moto_current( s16 iq1, s16 iq2, s16 iq3, s16 iq4);

#endif

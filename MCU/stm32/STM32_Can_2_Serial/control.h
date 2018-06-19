#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define MAX_Carrent_out    15000   // -15000 ~15000 对应电机电流值  -18A ~ 18A
#define CanBoxPost         1
#define CanBoxPend         0


typedef struct TControl//与控制相关的参数全部在此结构体中
{

	u8		   u8CarID;    //  

	s32      s32EncoderCounter_l; //左轮 编码器的值
	s32      s32EncoderCounter_r; //右轮 编码器的值
	s32      s32EncoderCounter_a; //臂   编码器的值

	u16      u16ADC_Carrent_l;//左轮 电机电流值
	u16      u16ADC_Carrent_r;//右轮 电机电流值
	u16      u16ADC_Carrent_a;//臂   电机电流值
	//右手定则定义的车底盘速度
	float     s16x;           // +- 1.0~1.0   m/s
	float     s16y;           // +- 1.0~1.0   m/s
	float     s16w;           // +- 0.5~0.5   rad/s
	
//右手定则定义的臂速度
	float     s16arm_x;       // +- 0.1~0.1   m/s
	
//*************CAN总线相关*********************//	
	u8       bCanComBox;        // can 状态标识
  u8			 bCanSendCounter;  //每发送一个包，这个值++
	u16	     u16CanComCount[2]; //u16CanComCount[0] can 中断 接收消息计数
	u16      u16CanComPeriod;   //记录系统时钟1ms中断次数  1~500 每100ms  500ms 判断一次  更新 can 中断计数 状态 
	
	u16	     u16RunRecvPeriodMotor;
	//u16	     u16RunSendPeriodMotor;
	CanTxMsg Can_Txmessage;
	u8		   u8Can_Msg_Send_flag;
	
  u8		   u8Serial_Msg_Send_flag_Left;
  u8		   u8Serial_Msg_Send_flag_Right;
	u8		   u8Serial_Msg_Send_flag_Arm;

}TCtrl,*ptCtrl;


extern TCtrl TForkLift;     //定义叉车结构体
extern ptCtrl ptForkLift;   //定义叉车结构体指针

void uart_init(u32 bound);

#endif

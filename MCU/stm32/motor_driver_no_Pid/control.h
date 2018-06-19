#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define MAX_PWM            1700   //PWM1500速度对应490的脉冲
#define MIN_PWM            0
#define FOWARD             0
#define BACK               1
#define CanBoxPost         1
#define CanBoxPend         0
#define MotorPeriodIn      1
#define MotorPeriodOut     0

typedef struct TControl//与控制相关的参数全部在此结构体中
{
	u16      u16PWM;     // 电机PWM值
	u8       bDrection;  // 方向
	u8		   u8CarID;    // 电机编号
	
	s16      s16EncoderSpeed;// 编码器的速度
	u16      u32ADCResult;   // 电机母线采用电流  ad采样值


//*************CAN总线相关*********************//	
	u8       bCanComBox;           // can 状态标识
	u16	     u16CanComCount[2];    //u16CanComCount[0] can 中断 接收消息计数 
	u16      u16CanComPeriod;      //记录系统时钟1ms中断次数  1~500 每100ms  500ms 判断一次  更新 can 中断计数 状态   
	
	u16	     u16RunRecvPeriodMotor;//记录系统时钟1ms中断次数 1~10 每十次检查  每10ms执行一次
	u16	     u16RunSendPeriodMotor;//记录系统时钟1ms中断次数 1~50 每十次检查  每50ms执行一次  发送 can 状态
	u16	     u16RunDetADC;
	u16      u16RunPwmChange;
	
	CanTxMsg Can_Txmessage;        //can发送数据消息类型   包含 can状态  Can_Txmessage.Data[0]  0~255   255 不正常
  
	u16	     u16time_ms;           //

//*************CAN总线相关*********************//	
	
	u16      u16MotorControlFlag;
	
//volatile uint16_t ADC1ConvertedValue;//[5];
float ADC_Value;
u16   u16ADC_Value;

float I_Value;
u16   u16I_Value;

u8  ADC_convert_start; 
u8  PWM_change_start;
}TCtrl,*ptCtrl;

/*
class Ctrl1
{
	public:
  
    
	protected:
  
};
*/



// 全局变量
extern TCtrl TForkLift;     //定义叉车结构体
extern ptCtrl ptForkLift;   //定义叉车结构体指针



void SetPwmDir(ptCtrl ptCtrlCar);
void SetPwmDir_new(ptCtrl ptCtrlCar);
void uart_init(u32 bound);


#endif

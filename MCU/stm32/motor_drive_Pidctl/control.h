#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define MAX_PWM            3500   //PWM1500速度对应490的脉冲
#define MIN_PWM            0
#define FOWARD             0
#define BACK               1
#define CanBoxPost         1
#define CanBoxPend         0
#define MotorPeriodIn      1
#define MotorPeriodOut     0

#define 	Queue_Lenth      15


typedef struct TControl//与控制相关的参数全部在此结构体中
{
	u16      u16PWM;     // 电机速度 pwm值
	u8       bDrection;  // 电机方向  0为前进  1为后退
	u8		   u8CarID;    // 由拨码盘确定的 电机编号
	//****************PID控制参数**************************//	
	u16	     u16RunPIDControl;//PID控制标识
	float	 s16error[3];//最近几次误差
	float	 s16ErrorSum;//误差和
	float      s16speedwant;//期望速度
	float      s16speedout; //控制器输出
   // pid 参数
	float      s16speed_p;
	float      s16speed_i;
	float      s16speed_d;
	
	s16      s16EncoderSpeed;     //编码器反馈的 脉冲数 pid 控制周期内（1ms）正负
	s32      s32EncoderSpeed_sum; //上位机控制周期内 编码器 的 脉冲数  正转为正  反转为负
	u32      u32ADCResult;        //电机电流采样


//*************CAN总线相关*********************//	
  u8       bCanComBox;           // can 状态标识
	u16	     u16CanComCount[2];    //u16CanComCount[0] can 中断 接收消息计数 
	u16      u16CanComPeriod;      //记录系统时钟1ms中断次数  1~500 每100ms  500ms 判断一次  更新 can 中断计数 状态   
	
	u16	     u16RunRecvPeriodMotor;//记录系统时钟1ms中断次数 1~10 每十次检查  每10ms执行一次
	u16	     u16RunSendPeriodMotor;//记录系统时钟1ms中断次数 1~50 每十次检查  每50ms执行一次  发送 can 状态
	CanTxMsg Can_Txmessage;        //can发送数据消息类型   包含 can状态  Can_Txmessage.Data[0]  0~255   255 不正常
//**** 发送 编码器值标志 wyw添加 ***  
  u8   Encoder_can_Tx_flag;
	
	u16	     u16time_ms;

//*************CAN总线相关*********************//	
	
	u16      u16MotorControlFlag;
// wyw 添加  DMA 存ADC1 6 PA6 口的 ADC采样值
__IO uint16_t ADC1ConvertedValue;
	
}TCtrl,*ptCtrl;



//队列 滤波
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




extern TCtrl TForkLift;     //定义叉车结构体
extern ptCtrl ptForkLift;   //定义叉车结构体指针
extern linkqueue  middle_filter_queue;
extern linkqueue* ptmiddle_filter_queue;

void SetPwmDir(ptCtrl ptCtrlCar);
void SetPwmDir_new(ptCtrl ptCtrlCar);
void POSITION_PID(ptCtrl ptCtrlCar,int EncoderSpeed);
void uart_init(u32 bound);

void create_queue(linkqueue *  Fliter_q);
void en_queue(linkqueue *  Fliter_q, uint16_t data);
int de_queue(linkqueue *  Fliter_q);
void printf_queue(linkqueue *  Fliter_q);
uint16_t get_queue_value(linkqueue *  Fliter_q,int num);
void swap_data(linkqueue *  Fliter_q,int i,int j);
void set_data(linkqueue *  Fliter_q,int i,uint16_t value);
void Quick_Sort(linkqueue *  Fliter_q,int left,int right);
int average_data(linkqueue *  Fliter_q);

void gotoerror(void);
#endif

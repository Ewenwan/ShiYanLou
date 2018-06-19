#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "timer.h" 
#include "control.h"
 
/************************************************

总体概述：  can 接收 0X201 ~0x208 电调 电机转子转速信息  速度 反馈信息
                接收 firefly 0x01 主机的 速度设定指令   
								pid 控制后发送 can消息 0x200 给电机电调
************************************************/
u8 debug_pid  =1;// 为1正常运行  为0 用于调试 pid

float encoder_fliter=0;
float encoder_fliter2=0;

 int main(void)
 {	 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级(0~3)，2位响应优先级(0~3)
	uart_init(115200);	 	//串口初始化为115200
	printf("\r\nSystem init OK\r\n");
	 
	ptForkLift=&TForkLift[0]; //左轮 结构体 指针
	ptForkLift2=&TForkLift[1];//右轮 结构体 指针
	 
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
  LED_Init();	
  if(debug_pid==1)
		
	// 360000/(1+4+7)/12=250Kbps
 // CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//CAN初始化正常模式,波特率250Kbps    
  
	// 360000/(1+4+7)/6=1000Kbps=1Mbps
  // CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,6,CAN_Mode_Normal);
	
	// 360000/(1+4+7)/3=1000Kbps=1Mbps
  	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,3,CAN_Mode_Normal);
	
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
	
		 

	CarID_Select();
	printf("CarID:%d\r\n",ptForkLift->u8CarID);


  if(ptForkLift->u8CarID==0)//主控
		ptForkLift->Can_Txmessage.StdId=0x200;   // 标准标识符  0x200控制 201~204 电调  0x1FF控制205~208电调
 
//	else if(ptForkLift->u8CarID==3)
//		ptForkLift->Can_Txmessage.StdId=0x203;   //  电调 201~208

  ptForkLift->Can_Txmessage.ExtId=0x12;				    // 设置扩展标示符 
  ptForkLift->Can_Txmessage.IDE=CAN_Id_Standard;  // 标准帧
  ptForkLift->Can_Txmessage.RTR=CAN_RTR_Data;		  // 数据帧
  ptForkLift->Can_Txmessage.DLC=8;				        // 要发送的数据长度   
	
	
// 大疆M3508电机
	//左轮pid参数
	ptForkLift->pid_time=4.0;//  电机电调上传 速度反馈值为 250KHz  即4ms发送一次    1.5  0.1
	ptForkLift->s16speedwant = 0;  //速度范围  -500*19.2 ~ 500*19.2   //600 一分钟30圈  2秒 一圈
	ptForkLift->s16speed_p = 1.8;
	ptForkLift->s16speed_i = 0.03;
	ptForkLift->s16speed_d = 0.03;
	ptForkLift->u16ErrorDeadBand=10;
	
	//右轮pid参数
	ptForkLift2->s16speedwant = 0;  //速度范围  -500*19.2 ~ 500*19.2   //600 一分钟30圈  2秒 一圈
	ptForkLift2->s16speed_p = 1.8;
	ptForkLift2->s16speed_i = 0.03;
	ptForkLift2->s16speed_d = 0.03;
  ptForkLift2->u16ErrorDeadBand=10;
	
//叉车物理参数
/*
ptForkLift->s16fb_d=0.032; //前后叉轮子直径 0.032m    3.2cm
ptForkLift->s16ud_d=0.032; //上下叉轮子直径
ptForkLift->s16dp_d=0.485; //底盘直径      //0.485m   48.5cm
ptForkLift->s16dpl_d=0.125;//底盘轮子直径   //0.125m   12.5cm

ptForkLift->u32dpl_econder=500;//底盘轮线数
ptForkLift->u32dpl_econder_reduc=55;//底盘轮减速比

ptForkLift->u32ud_econder=500;//底盘轮线数
ptForkLift->u32ud_econder_reduc=55;//底盘轮减速比

ptForkLift->u32fb_econder=500;//前后轮线数
ptForkLift->u32fb_econder_reduc=160;//前后轮减速比
*/

//初始化
	ptForkLift->u16RunPIDControl = 0;
	ptForkLift->s32EncoderCounter = 0;
	ptForkLift->bCanSendCounter = 0;
	ptForkLift->u8Can_Msg_Send_flag=0;//can消息发送标志
	delay_ms(1000);
	
	TIM3_Int_Init();   

	printf("\r\nSystem Running\r\n");
	
 	while(1)
	{	
		
if(ptForkLift->u16RunRecvPeriodMotor==10)
	{			
		//				printf("u16RunRecvPeriodMotor %d\r\n",ptForkLift->u16RunRecvPeriodMotor );
		if(debug_pid==1)//进行 pid测试时为0  正常运行改为1
		{	
      ///*			
			if((ptForkLift->bCanComBox == CanBoxPend)||(ptForkLift2->bCanComBox == CanBoxPend))//离线状态
			{
				ptForkLift->s16speedwant = 0;
				ptForkLift->s16error[0] = 0;
				ptForkLift->s16error[1] = 0;
				ptForkLift->s16error[2] = 0;
				ptForkLift->s16ErrorSum = 0;
				
				ptForkLift2->s16speedwant = 0;
				ptForkLift2->s16error[0] = 0;
				ptForkLift2->s16error[1] = 0;
				ptForkLift2->s16error[2] = 0;
				ptForkLift2->s16ErrorSum = 0;
			}
     //*/	

		///*		
			else if((ptForkLift->bCanComBox == CanBoxPost)&&(ptForkLift2->bCanComBox == CanBoxPost))//在线状态 
			{		
			 ptForkLift->s16speedwant = 000;
			 ptForkLift2->s16speedwant = 000;	
			}
			//*/												
    }	
	}		
		if(debug_pid)//进行 pid测试时为0  正常运行改为1
		 {
			 /*
			if(ptForkLift->u8Can_Msg_Send_flag == 1)// 50ms发送一次 编码器计数  移入 主程序 WHILE(1)中
					{
					
						ptForkLift->u8Can_Msg_Send_flag = 0;
						ptForkLift->bCanSendCounter = ptForkLift->bCanSendCounter + 1;

						ptForkLift->Can_Txmessage.Data[0] = (ptForkLift->s32EncoderCounter)       & 0xff;
						ptForkLift->Can_Txmessage.Data[1] = ((ptForkLift->s32EncoderCounter)>>8)  & 0xff;
						ptForkLift->Can_Txmessage.Data[2] = ((ptForkLift->s32EncoderCounter)>>16) & 0xff;
						ptForkLift->Can_Txmessage.Data[3] = ((ptForkLift->s32EncoderCounter)>>24) & 0xff;
						ptForkLift->Can_Txmessage.Data[4] = (u8)(ptForkLift->bCanSendCounter)     & 0xff;
						//ptForkLift->Can_Txmessage.Data[5] = ( ptForkLift->u16I_Value)             & 0xff;
						//ptForkLift->Can_Txmessage.Data[6] = ((ptForkLift->u16I_Value)>>8)         & 0xff;			
						ptForkLift->s32EncoderCounter = 0;			
						Can_Send_Msg(); //包飞: 放到主程序中			
					}
			 */
		 }				
 }		 
}



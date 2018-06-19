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
 ALIENTEK战舰STM32开发板实验26
 CAN通信实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
 
 二次开发：
 创建者：汪自强
 创建时间：2017/03/20
 文件说明：智乘网络科技公司叉车电机驱动程序
 
 开发log：
 2017/03/20  CAN通信稳定
 2017/03/21  添加串口，时基系统
 2017/04/24  添加正交编码功能，使用TIM2
 
 万有文 完善
 2017/06/18  新旧板子 pwm 输出
 2017/06/21  电机母线电流  ADC DMA 中断滤波采样
 

************************************************/


/************************************************

总体概述：
串口 :      PB6  , USART1_TX     PB7 , USART1_RX	 串口   115200bps
CAN通讯:    PA11 , RX  PA12 , TX  ——> 经过 TJA1050 转成  CAN_H  CAN_L  电平信号
JLINK下载:  PA13 , SWDIO 数据线   PA14，SWCLK 时钟线

电机输出：  PA8  PA9 PWM口   TIM1  CH1  CH2
电机母线采样：      ADC-DMA  PA6 
电机编号拨码盘设定；新板子 ，PB10  PB1  PB0            老板子， PA2 PA3 PA4 PA5
电机编码器采样 ：   TIM2  正交编码  PA0 A相 ，  PA1   B相

系统运行指示灯：    PB12



************************************************/
u8 board_type =1; //0 - 旧板子   1 - 新板子

 int main(void)
 {	 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级(0~3)，2位响应优先级(0~3)
	
	 /*           先占优先级         从优先级3级
	  CAN1             	0                  	2
	  
	  TIM1 			   	    0				          	3   PWM输出
	  TIM2			      	1					          0		正交编码
	  UART1            	3                  	3
	 	 
	 ADC-DMA            2                   1
	 */
	 

	uart_init(115200);	 	//串口初始化为115200

	printf("\r\nSystem init OK\r\n");


  LED_Init();	 
  
// CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//CAN初始化正常模式,波特率250Kbps    
  
	
	TIM1_PWM_Init();          //定时器1用于PWM输出，频率20KHz
	TIM2_Encoder_Init();
	//TIM3_Int_Init();          //定时器3用作系统时基,1ms进入一次中断  	 
	
	 
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
		 
	ptForkLift=&TForkLift; //
	ptmiddle_filter_queue = &middle_filter_queue;
	
	CarID_Select();
	printf("CarID:%d\r\n",ptForkLift->u8CarID);

/*
	if(ptForkLift->u8CarID==1)
		ptForkLift->Can_Txmessage.StdId=0x02;		 // 标准标识符  carID = 1 canid = 2 左侧轮子 
	else if(ptForkLift->u8CarID==0) 
		ptForkLift->Can_Txmessage.StdId=0x03;		// 标准标识符  carId = 0 canid = 3  右侧轮子
*/
  if(ptForkLift->u8CarID==1)
		ptForkLift->Can_Txmessage.StdId=0x04;   // 标准标识符  carID = 1 canid = 4  前轮
	else if(ptForkLift->u8CarID==2) 
		ptForkLift->Can_Txmessage.StdId=0x05;		// 标准标识符   carId = 2 canid = 5  左轮
	else if(ptForkLift->u8CarID==3) 
		ptForkLift->Can_Txmessage.StdId=0x06;		// 标准标识符   carId = 3 canid = 6  后轮
	else if(ptForkLift->u8CarID==4) 
		ptForkLift->Can_Txmessage.StdId=0x07;		// 标准标识符   carId = 4 canid = 7  右轮
	else if(ptForkLift->u8CarID==5) 
		ptForkLift->Can_Txmessage.StdId=0x08;		// 标准标识符   carId = 5 canid = 8  上下叉
	else if(ptForkLift->u8CarID==6) 
		ptForkLift->Can_Txmessage.StdId=0x09;		// 标准标识符   carId = 6 canid = 9  前后叉
	

  ptForkLift->Can_Txmessage.ExtId=0x12;				    // 设置扩展标示符 
  ptForkLift->Can_Txmessage.IDE=CAN_Id_Standard;  // 标准帧
  ptForkLift->Can_Txmessage.RTR=CAN_RTR_Data;		  // 数据帧
  ptForkLift->Can_Txmessage.DLC=8;				        // 要发送的数据长度   
	
	/*
	//三轮车 25ms控制周期
	ptForkLift->s16speed_p = 0.6;    // 15  //
	ptForkLift->s16speed_i = 0.12;    // 1  //0.12
	ptForkLift->s16speed_d = 0.03;    // 3     //
	*/
	/*
  ptForkLift->s16speed_p = 0.405;    // 15  //
	ptForkLift->s16speed_i = 0.055;    // 1  //0.12
	ptForkLift->s16speed_d = 0.081;      // 3     //
	*/
	//pid参数初始化
	//四轮车 1ms控制周期    0.19  0.03  3
	ptForkLift->s16speedwant = 0;    //注意想要的速度只能在PID控制函数外部设置，不能在POSITION_PID里面设置	
	ptForkLift->s16speed_p = 0.18;
	ptForkLift->s16speed_i = 0.03;
	ptForkLift->s16speed_d = 3;
	
	
	
//叉车物理参数
///*
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
//*/



//初始化
	ptForkLift->bDrection =FOWARD;  // BACK  FOWARD
	ptForkLift->u16PWM=00;
	ptForkLift->u16RunPIDControl = 0;
	ptForkLift->u32ADCResult = 0;
	ptForkLift->s32EncoderCounter = 0;
	ptForkLift->bCanSendCounter = 0;
	
	if (board_type ==0)
		SetPwmDir(ptForkLift);//老板  
	else
		SetPwmDir_new(ptForkLift);//新版子  
		
	create_queue(ptmiddle_filter_queue);   //创建一个编码器滤波队列
	 
//  新老板子 定义在  control.h 文件内 
/***正反转需要修改的地方*************************/
	 ptForkLift->s16speedwant = 00;   // 0~200
	// ptForkLift->s16speedwant =-50; //反转   范围 +- 1700
	// ptForkLift->s16speedwant = 50;  //正转   范围 +- 1700

  // Adc_Init();            // 普通 ADC 
	// u16 MotorCarrentADC1Value;
	// MotorCarrentADC1Value=Get_Adc_Average(ADC_Channel_0,5);
	// temp=(float)MotorCarrentADC1Value*(3.3/4096);
	
	//TIM1_PWM_Init();          //定时器1用于PWM输出，频率20KHz
	//TIM2_Encoder_Init();
	
	motor_carrent_io_init();  // PA6口配置为  ADC1 DMA 传输 电机母线电流采样口 输入	
		
	TIM3_Int_Init();          //定时器3用作系统时基,1ms进入一次中断  
  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//CAN初始化正常模式,波特率250Kbps 
	
	printf("\r\nSystem Running\r\n");
	
 	while(1)
	{	
		
		if(ptForkLift->u16RunRecvPeriodMotor==10)
		{			
//				printf("u16RunRecvPeriodMotor %d\r\n",ptForkLift->u16RunRecvPeriodMotor );
if(1)//进行 pid测试时为0  正常运行改为1
{	
///*			
			if(ptForkLift->bCanComBox == CanBoxPend)//离线状态
			{
				ptForkLift->s16speedwant = 0;
				ptForkLift->s16error[0] = 0;
				ptForkLift->s16error[1] = 0;
				ptForkLift->s16error[2] = 0;
				ptForkLift->s16ErrorSum = 0;
			}

//*/	
}	
		/*		
			else if(ptForkLift->bCanComBox == CanBoxPost)//在线状态 
			{
				ptForkLift->Can_Txmessage.Data[0] = (ptForkLift->s32EncoderCounter) & 0xff;
				ptForkLift->Can_Txmessage.Data[1] = ((ptForkLift->s32EncoderCounter)>>8) & 0xff;
				ptForkLift->Can_Txmessage.Data[2] = ((ptForkLift->s32EncoderCounter)>>16) & 0xff;
				ptForkLift->Can_Txmessage.Data[3] = ((ptForkLift->s32EncoderCounter)>>24) & 0xff;
			}		
		*/	
/*				
				ptForkLift->Can_Txmessage.Data[0] = (ptForkLift->s16EncoderSpeed) & 0xff;
				ptForkLift->Can_Txmessage.Data[1] = ((ptForkLift->s16EncoderSpeed)>>8) & 0xff;
*/							
				/*				
				if(ptForkLift->u16time_ms < 3000 )
				{
					ptForkLift->Can_Txmessage.Data[0]=254;
				}
				else
				{
					ptForkLift->Can_Txmessage.Data[0]=255;
				}
*/					
//				printf("stop motor\r\n");
//				printf("%d \r\n",ptForkLift->s16EncoderSpeed);
							
		}
	/*	
			if(ptForkLift->u16RunSendPeriodMotor == 50)// 50ms发送一次 编码器计数  移入 主程序 WHILE(1)中
			{
			
				ptForkLift->u16RunSendPeriodMotor = 0;
				ptForkLift->bCanSendCounter = ptForkLift->bCanSendCounter + 1;

				ptForkLift->Can_Txmessage.Data[0] = (ptForkLift->s32EncoderCounter)       & 0xff;
				ptForkLift->Can_Txmessage.Data[1] = ((ptForkLift->s32EncoderCounter)>>8)  & 0xff;
				ptForkLift->Can_Txmessage.Data[2] = ((ptForkLift->s32EncoderCounter)>>16) & 0xff;
				ptForkLift->Can_Txmessage.Data[3] = ((ptForkLift->s32EncoderCounter)>>24) & 0xff;
				ptForkLift->Can_Txmessage.Data[4] = (u8)(ptForkLift->bCanSendCounter)     & 0xff;
				ptForkLift->Can_Txmessage.Data[5] = ( ptForkLift->u16I_Value)             & 0xff;
				ptForkLift->Can_Txmessage.Data[6] = ((ptForkLift->u16I_Value)>>8)         & 0xff;			
				ptForkLift->s32EncoderCounter = 0;			
				Can_Send_Msg(); //包飞: 放到主程序中			
			}
		*/
	    //printf("EncoderSpeed=%d \r\n",ptForkLift->s16EncoderSpeed);
/*			
	 // 开启 ADC采样
		if(ptForkLift->u16RunDetADC==50){          // 50ms采集一次 电机电流电压
			 ptForkLift->u16RunDetADC=0;
	   	 ADC_SoftwareStartConvCmd(ADC1,ENABLE);  // 使能ADC1 的软件转换功能  可以在主程序  开启功能  			
		}
*/		
					
     }
		 
}




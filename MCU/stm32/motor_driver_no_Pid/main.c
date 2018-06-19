#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "timer.h" 
#include "control.h"
//#include "stm32f10x.h"
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
 
 

************************************************/


/************************************************

总体概述：

        6个电机 



************************************************/
	int k=0;
	//k=0;
	int dir=0;
 int main(void)
 {	 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	 /*           先占优先级         从优先级3级
	  CAN1         	0                  	2
	  
	  TIM1 			   	0				  	        3       PWM输出
	  TIM2			   	1				            0		正交编码
	  UART1         3                  	3
	 
	 */
	 
	uart_init(115200);	 	    //串口初始化为115200 用于调试

  LED_Init();	 
    
//CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//CAN初始化正常模式,波特率250Kbps    
	
	 
	TIM1_PWM_Init();          //定时器1用于PWM输出，频率20KHz
	TIM2_Encoder_Init();      //定时器2用于 编码器的正交解码
	
			
	printf("\r\nSystem init OK\r\n");

	//Adc_Init();//
 // printf("\r\nSystem init OK\r\n");
	 
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
	delay_ms(1000);
	delay_ms(1000);
	 
	ptForkLift=&TForkLift; // 定义叉车结构体指针
	ptForkLift->u16RunRecvPeriodMotor=0;  //电机 pwm 值 设置 上传 计数   0~10 系统时钟中断计数
	ptForkLift->u16RunSendPeriodMotor=0;  //记录系统时钟1ms中断次数 0~50 每十次检查  每50ms执行一次
	ptForkLift->u16RunDetADC=0;
	ptForkLift->ADC_convert_start=0;
	
	  //配置DMA传输方式
//ptForkLift->u32ADCResult=0;
//ptForkLift->ADC1ConvertedValue=0;

	
	CarID_Select();        // 根据 拨码盘输入确定 电机  三位的话可以确定 0~7 八个电机
  printf("CarID:%d\r\n",ptForkLift->u8CarID);
	ptForkLift->Can_Txmessage.StdId=0x02;				     // 标准标识符 
  ptForkLift->Can_Txmessage.ExtId=0x12;				     // 设置扩展标示符 
  ptForkLift->Can_Txmessage.IDE=CAN_Id_Standard;   // 帧格式  CAN_Id_Standard 标准格式 ，  CAN_Id_Extended 扩展格式 
  ptForkLift->Can_Txmessage.RTR=CAN_RTR_Data;		   // 帧类型  CAN_RTR_Data    数据帧       CAN_RTR_Remote 远程帧
  ptForkLift->Can_Txmessage.DLC=8;				         // 要发送的数据长度 字节    
	

// 测试需要修改的地方 

/******************************************************************/
	
 /*老版本板子   正反转 测试  不加PID 控制 */
// 正转
	ptForkLift->bDrection = FOWARD ;  //BACK  FOWARD
	ptForkLift->u16PWM=0;  // 最大为1700 对应速度最快         
	SetPwmDir_new(ptForkLift);   // 设置 PWM和方向值
// 反转 
  /*
  ptForkLift->bDrection = BACK;
	ptForkLift->u16PWM=300;
	SetPwmDir(ptForkLift);  // 设置 PWM和方向
	*/
	
/*新版本板子  正反转测试  不加PID 控制 */	
   /*
	ptForkLift->bDrection = FOWARD;
	ptForkLift->u16PWM=300;      // 最大为1700 对应速度最快         
	SetPwmDir_new(ptForkLift);   // 设置 PWM和方向值
  */
// 反转
 /*
  ptForkLift->bDrection = BACK;
	ptForkLift->u16PWM=300;
	SetPwmDir_new(ptForkLift);  // 设置 PWM和方向
 */
	
/******************************************************************************/
	//int rt;
	motor_carrent_io_init();
		//Adc_Init();
	TIM3_sys_Init();          //定时器3用作系统时基,1ms进入一次中断 
  //ADC_SoftwareStartConvCmd(ADC1,ENABLE);     // 使能ADC1 的软件转换功能  可以在主程序  开启功能   有异常

 	while(1)
	{
		
		if(ptForkLift->ADC_convert_start==1){
			 ptForkLift->ADC_convert_start=0;
			
		   ADC_SoftwareStartConvCmd(ADC1,ENABLE);     // 使能ADC1 的软件转换功能  可以在主程序  开启功能   有异常

			

						
		 // ADC_SoftwareStartConvCmd(ADC1,ENABLE);     // 使能ADC1 的软件转换功能  可以在主程序  开启功能   有异常
		 
/*			
			ptForkLift->u32ADCResult=Get_Adc_Average(ADC_Channel_6 , 1);
			//ptForkLift->u32ADCResult=Get_Adc_Average_(3);
		  ptForkLift->ADC_Value = (float)(ptForkLift->u32ADCResult)/4096*3.3;
			printf("%f\r\n",ptForkLift->ADC_Value);
	*/		 
		}
	if(ptForkLift->PWM_change_start==1){
		ptForkLift->PWM_change_start=0;
/*	
			if((k<50)&&(dir==0)) k++;
			else dir=1;
			
			if(dir==1) k--;
			if(k==0) dir=0;
			
			if (k<0) k=0;
			if (k>50) k=50;
	*/		
			//ptForkLift->u16PWM=0;        // 最大为1700 对应速度最快 
			ptForkLift->u16PWM=100;         // 最大为1700 对应速度最快      100   1ms  17  ms       6.875 的话 4s转一圈
		                                 // 叉车 pwm 81 好长时间才转   14脉冲
		                                 // 70不转
			//ptForkLift->u16PWM=k*10;        // 最大为1700 对应速度最快 
	    SetPwmDir_new(ptForkLift);   // 设置 PWM和方向值
	}
	//ptForkLift->bDrection = FOWARD;
	//ptForkLift->u16PWM=100;
	//SetPwmDir(ptForkLift);  // 设置 PWM和方向值
	//	delay_ms(4000);
	
	//ptForkLift->bDrection = BACK;	
	//ptForkLift->u16PWM=100;
	//SetPwmDir(ptForkLift);  // 设置 PWM和方向值
	//	delay_ms(4000);
/*
		if(ptForkLift->u16RunRecvPeriodMotor==10)//每十次检查  每10ms执行一次
		{			
			
			if(ptForkLift->bCanComBox == CanBoxPost)// can 正常  发送
			{
				
				SetPwmDir(ptForkLift);
				ptForkLift->Can_Txmessage.Data[0]++;      //包含can的状态
				if(ptForkLift->Can_Txmessage.Data[0]==254)//can 状态正常 为 0~254
					ptForkLift->Can_Txmessage.Data[0]=0;
				
				//printf("crtl motor\r\n");
				
			}
				
			else if(ptForkLift->bCanComBox == CanBoxPend)// can 请求  can 不正常 请求 重启 can
			{
				ptForkLift->u16PWM = 0;//速度为0
				SetPwmDir(ptForkLift); //停止
				
				if(ptForkLift->u16time_ms < 3000 )
				{
					ptForkLift->Can_Txmessage.Data[0]=254;	
				}
				else
				{
					ptForkLift->Can_Txmessage.Data[0]=255;//系统正常运行3s后 如果can 还不正常  正发生can 重启请求
				}
					
				//printf("stop motor\r\n");
			}
						
				
		}
	*/	
		//printf("speed %d\r\n",ptForkLift->s16EncoderSpeed);
					
     }
}



#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
//#include "lcd.h"
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
 
 

************************************************/


/************************************************

总体概述：

        6个电机  根据 拨码盘确定哪一个
				
				一个系统运行指示灯  PB12
				
				一个电机母线电流ADC PA6口采样口
				
				拨码盘 采样 PB0 PB1  PB10		

************************************************/

 int main(void)
 {	 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	 /*           先占优先级         从优先级3级
	  CAN1             	0                  	2
	  
	  TIM1 			   	0				  	3       PWM输出
	  TIM2			   	1					0		正交编码
	  UART1            	3                  	3
	 
	 */
	 
	uart_init(115200);	 	  // 串口初始化为115200
  LED_Init();	            // 系统运行指示灯  PB12 输出   电机正反转控制口 输出  PA9
 // motor_carrent_io_init();// PA6口配置为  ADC1 电机母线电流采样口 输入
 //CarID_Select();         // 根据 拨码盘 io口 的状态 确定电机编号 输入 PB0 PB1  PB10	
	 
 //	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//CAN初始化正常模式,波特率250Kbps    
	
	TIM1_PWM_Init();          //定时器1用于PWM输出，频率20KHz
	TIM2_Encoder_Init();      //编码器 读数  计数器
//	TIM3_Int_Init();          //定时器3用作系统时基,1ms进入一次中断  
	printf("\r\nSystem init OK\r\n");//串口输出消息 指示 系统 运行正常
	 
	
	 
	delay_ms(1000);//延时时间不能超过1800，多延时就要多调用
	delay_ms(1000);
	delay_ms(1000);
	 
	ptForkLift=&TForkLift; //结构体
	ptmiddle_filter_queue = &middle_filter_queue;//滤波列队
	
	
	//can 发送消息
	//  确定can消息标识符 用发送 编码器消息
	if (ptForkLift->u8CarID == 0x00)
	ptForkLift->Can_Txmessage.StdId=0x01;				     // 标准标识符  0x02
	if (ptForkLift->u8CarID == 0x01)
	ptForkLift->Can_Txmessage.StdId=0x02;				     // 标准标识符  0x02
	if (ptForkLift->u8CarID == 0x02)
	ptForkLift->Can_Txmessage.StdId=0x03;				     // 标准标识符  0x02
	if (ptForkLift->u8CarID == 0x03)
	ptForkLift->Can_Txmessage.StdId=0x04;				     // 标准标识符  0x02
	if (ptForkLift->u8CarID == 0x04)
	ptForkLift->Can_Txmessage.StdId=0x05;				     // 标准标识符  0x02
	if (ptForkLift->u8CarID == 0x05)
	ptForkLift->Can_Txmessage.StdId=0x06;				     // 标准标识符  0x02
	
  ptForkLift->Can_Txmessage.ExtId=0x12;				     // 设置扩展标示符 
  ptForkLift->Can_Txmessage.IDE=CAN_Id_Standard;   // 帧格式  CAN_Id_Standard 标准格式 ，  CAN_Id_Extended 扩展格式 
  ptForkLift->Can_Txmessage.RTR=CAN_RTR_Data;		   // 帧类型  CAN_RTR_Data    数据帧       CAN_RTR_Remote 远程帧
  ptForkLift->Can_Txmessage.DLC=8;				         // 要发送的数据长度 字节 
	
	//pid参数初始化  0.19 0.03 3    pwm1700     pwm3500  0.3 0.03 3
	ptForkLift->s16speedwant = 0;    //注意想要的速度只能在PID控制函数外部设置，不能在POSITION_PID里面设置	
	ptForkLift->s16speed_p = 0.3;  
	ptForkLift->s16speed_i = 0.03;
	ptForkLift->s16speed_d = 3;
	
	// 编码器总计数 
	ptForkLift->s32EncoderSpeed_sum=0;
	
	ptForkLift->bDrection = BACK;
	ptForkLift->u16PWM=000;
	SetPwmDir_new(ptForkLift);// 初始化电机速度 为 0
	
	create_queue(ptmiddle_filter_queue);   //创建一个编码器滤波队列

	ptForkLift->s16speedwant = 10;    //注意想要的速度只能在PID控制函数外部设置，不能在POSITION_PID里面设置	
	                                   // 叉车 底盘电机   //  1ms  35———> 0.5m/s  18----->0.2m/s
																		 
																		 
	
  TIM3_Int_Init();          //定时器3用作系统时基,1ms进入一次中断  
 	while(1)
	{

		if(ptForkLift->u16RunRecvPeriodMotor==10)//每十次检查  每10ms执行一次
		{			
			
			if(ptForkLift->bCanComBox == CanBoxPost)// can 正常  设置  编码器值
			{
				// 编码器计数 多久发送一次？？
				//ptForkLift->Can_Txmessage.Data[0] = ptForkLift->s16EncoderSpeed;
				
				//SetPwmDir(ptForkLift);
				//ptForkLift->Can_Txmessage.Data[0]++;//包含can的状态
				//if(ptForkLift->Can_Txmessage.Data[0]==254)//can 状态正常 为 0~254
				//	ptForkLift->Can_Txmessage.Data[0]=0;
				
				//printf("crtl motor\r\n");
				
			}
				
			else if(ptForkLift->bCanComBox == CanBoxPend)//与上位机通信已断开，或上位机长时间没发送速度
			{
//    	ptForkLift->s16speedwant = 0;                    //设置速度为零
				//SetPwmDir(ptForkLift);
				
				// 发送can状态标识
				if(ptForkLift->u16time_ms < 3000 )
				{
					ptForkLift->Can_Txmessage.Data[1]=254;
				}
				else
				{
					ptForkLift->Can_Txmessage.Data[1]=255;//系统正常运行3s后 如果can 还不正常  正发生can 重启请求
				}				
				//printf("stop motor\r\n");
			}
			
		}
	
/*		
		// 发送编码器 计数
		if (ptForkLift->Encoder_can_Tx_flag == 1)
		 {
			  if (ptForkLift->s32EncoderSpeed_sum >0)
				{
					  ptForkLift->Can_Txmessage.Data[0] = 0x01;
			      ptForkLift->Can_Txmessage.Data[1] = ptForkLift->s32EncoderSpeed_sum/100; // 不能超过 255
					 // 如果不采用 缩放  的话 可以分开成 多个字节传输
				}
				else 
				{
						ptForkLift->Can_Txmessage.Data[0] = 0x00;
			      ptForkLift->Can_Txmessage.Data[1] = - ptForkLift->s32EncoderSpeed_sum/100;
				}
				
		    //Can_Send_Msg();
			  ptForkLift->s32EncoderSpeed_sum=0;//设置为0
			  ptForkLift->Encoder_can_Tx_flag = 0;
		 }
		
		//printf("speed %d\r\n",ptForkLift->s16EncoderSpeed);
	*/
	
     }
}



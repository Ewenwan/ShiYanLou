#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "timer.h" 
#include "control.h"
#include <string.h>//memset

/************************************************

总体概述：  can 主机
            接收上位机 串口速度控制指令等
						通过can下发给 下面的can电机驱动板
						同时接收 下面can电机驱动板的 can消息(编码器反馈信息 和 电机电流信息 等 )
						通过串口上传给上位机
************************************************/
u8 debug_pid  =1;// 为1正常运行  为0 用于调试 pid


// 串口收到的字符
char chr;
// 解析上位机收到的命令
char cmd;
// 上位机发送的 字符后的 三个参数
char argv1[32];
char argv2[32];
char argv3[32];
// 三个参数解析成 数字
float arg1=0;
float arg2=0;
float arg3=0;

int arg = 0;
int index_ = 0;

unsigned int format_vel=0;

#define VELOCITY      'v'
//#define LEFT_Motor_Encoder  'l'
//#define Right_Motor_Encoder 'r'
//#define Aam_Motor_Encoder   'a'

/*  清空 命令参数 */
void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  memset(argv3, 0, sizeof(argv3));
  arg1 = 0;
  arg2 = 0;
  arg3 = 0;
  arg = 0;
  index_ = 0;
}
/* 接收指令*/
void runCommand() {
// 字符串转到数 stdlin.h  atof 到浮点型 atoi 到整数    atol长整形
  arg1 = atof(argv1);
  arg2 = atof(argv2);
  arg3 = atof(argv3);
// 测试 v  
switch(cmd) {
  case VELOCITY:    // 接收上位机的 速度指令  v X Y w
		 ptForkLift->s16x=arg1;
	   ptForkLift->s16y=arg2;
	   ptForkLift->s16w=arg3;
	   ptForkLift->u8Can_Msg_Send_flag=1;//置位  can下发标志
	  // printf("\r\nv %.3f\r\n",ptForkLift->s16x);
    break;
    
  default:
    printf("Inv Cmd");
    break;
  }
}

 int main(void)
 {	 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级(0~3)，2位响应优先级(0~3)
	uart_init(115200);	 	//串口初始化为115200  与上位机同步
//printf("\r\nSystem init OK\r\n");
	delay_ms(500);//延时时间不能超过1800，多延时就要多调用
  LED_Init();	
	 
	ptForkLift=&TForkLift; // can接收中断内 有参数赋值 需要先初始化，不然会卡死
	delay_ms(500);

	 
  if(debug_pid==1)
		
	// 360000/(1+4+7)/12=250Kbps
  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_7tq,12,CAN_Mode_Normal);//CAN初始化正常模式,波特率250Kbps    
  
	// 360000/(1+5+2)/9=500Kbps
	// CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_5tq,CAN_BS1_2tq,9,CAN_Mode_Normal);
	
	// 360000/(1+2+1)/9=1000Kbps=1Mbps
	//CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_2tq,CAN_BS1_1tq,9,CAN_Mode_Normal);
  	
	delay_ms(500);//延时时间不能超过1800，多延时就要多调用
	
		 

	CarID_Select();
//	printf("CarID:%d\r\n",ptForkLift->u8CarID);


  if(ptForkLift->u8CarID==0)
		ptForkLift->Can_Txmessage.StdId=0x01;   //  can_2_serial  充当can主机
	
  ptForkLift->Can_Txmessage.ExtId=0x12;				    // 设置扩展标示符 
  ptForkLift->Can_Txmessage.IDE=CAN_Id_Standard;  // 标准帧
  ptForkLift->Can_Txmessage.RTR=CAN_RTR_Data;		  // 数据帧
  ptForkLift->Can_Txmessage.DLC=8;				        // 要发送的数据长度   
	
//初始化	
	ptForkLift->s32EncoderCounter_l = 0;//编码器
	ptForkLift->s32EncoderCounter_r = 0;//
	ptForkLift->s32EncoderCounter_a = 0;
	ptForkLift->u16ADC_Carrent_l=0;     //电流
	ptForkLift->u16ADC_Carrent_r=0;
	ptForkLift->u16ADC_Carrent_a=0;
	
	ptForkLift->bCanSendCounter = 0;
	ptForkLift->u8Can_Msg_Send_flag=0;//can消息发送标志
  ptForkLift->u8Serial_Msg_Send_flag_Left=0; //串口消息发送标志 左轮
	ptForkLift->u8Serial_Msg_Send_flag_Right=0; //串口消息发送标志 右轮
	ptForkLift->u8Serial_Msg_Send_flag_Arm=0; //串口消息发送标志  臂
	
	delay_ms(1000);
	
	TIM3_Int_Init();   

	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口中断
	
//	printf("\r\nSystem Running\r\n");
	
 	while(1)
	{	
		
if(ptForkLift->u16RunRecvPeriodMotor==10)
	{			
		//				printf("u16RunRecvPeriodMotor %d\r\n",ptForkLift->u16RunRecvPeriodMotor );
		if(debug_pid==1)//进行 pid测试时为0  正常运行改为1
		{	
      ///*			
			if(ptForkLift->bCanComBox == CanBoxPend)//离线状态
			{
			}
     //*/	
		/*		
			else if(ptForkLift->bCanComBox == CanBoxPost)//在线状态 
			{		}*/												
    }	
	}		
if(debug_pid)//进行 pid测试时为0  正常运行改为1
		 {
			 
// can下发 控制命令			 
			if(ptForkLift->u8Can_Msg_Send_flag == 1)//  
					{
						ptForkLift->u8Can_Msg_Send_flag = 0;
						ptForkLift->Can_Txmessage.Data[6] =	0x00;
						//2m/s等分256  x轴速度
						if(ptForkLift->s16x>0)
						  {
                 ptForkLift->Can_Txmessage.Data[6] |=		0x01<<4;	// 1表示正 0 表示负
								 format_vel = (unsigned int)(ptForkLift->s16x*127.5);//  255/0.8=318.75   255/2.00=127.5
						     ptForkLift->Can_Txmessage.Data[1] = (format_vel>=256)?255:format_vel;
							}
							else{
								 format_vel = (unsigned int)(-ptForkLift->s16x*127.5);
						     ptForkLift->Can_Txmessage.Data[1] = (format_vel>=256)?255:format_vel;
							}
						//3.14 rad/s 等分 256	 自转 w速度
						if(ptForkLift->s16w>0)
						  {
                 ptForkLift->Can_Txmessage.Data[6] |=		0x01<<3;
								 format_vel = (unsigned int)(ptForkLift->s16w*81.21); // 255/3.14=81.21
						     ptForkLift->Can_Txmessage.Data[0] = (format_vel>=256)?255:format_vel;
						  }
							else{
								 format_vel = (unsigned int)(-ptForkLift->s16w*81.21);// 255/3.14=81.21
						     ptForkLift->Can_Txmessage.Data[0] = (format_vel>=256)?255:format_vel;								
							}

					 	 ptForkLift->Can_Txmessage.Data[2] = 0x00;
					   ptForkLift->Can_Txmessage.Data[3] = 0x00;
						 ptForkLift->Can_Txmessage.Data[4] = 0x00;
						 ptForkLift->Can_Txmessage.Data[5] = 0x00;
						 ptForkLift->Can_Txmessage.Data[7] = 0x00;					
						 Can_Send_Msg(); 
          //   printf("\r\nw %d\r\n",ptForkLift->Can_Txmessage.Data[0]);
					// printf("\r\nx %d\r\n",ptForkLift->Can_Txmessage.Data[1]);
							
					}
			
// 串口上传 左轮 右轮 臂 的编码器信息和 电流值	
///*					
					if(ptForkLift->u8Serial_Msg_Send_flag_Left==1){
						ptForkLift->u8Serial_Msg_Send_flag_Left=0;
						printf("l %d %d\r",ptForkLift->s32EncoderCounter_l, ptForkLift->u16ADC_Carrent_l);//左轮
					}
					if(ptForkLift->u8Serial_Msg_Send_flag_Right==1){
						ptForkLift->u8Serial_Msg_Send_flag_Right=0;
						printf("r %d %d\r",ptForkLift->s32EncoderCounter_r, ptForkLift->u16ADC_Carrent_r);//右轮
					}
					if(ptForkLift->u8Serial_Msg_Send_flag_Arm==1){
						ptForkLift->u8Serial_Msg_Send_flag_Arm=0;
						printf("a %d %d\r",ptForkLift->s32EncoderCounter_a, ptForkLift->u16ADC_Carrent_a);//臂
					}
//					*/
					
		 }
	 
 }		 
}
 
// 串口接收中断
void USART1_IRQHandler(void)
  { 
  if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) 
  chr = USART_ReceiveData(USART1); 
  // printf("%c",chr);
	
	    // 接收到 命令结束符
    if (chr == 13) { // CR ASCII码 13  回车'/r'
      if (arg == 1)      argv1[index_] = NULL;// 第二个参数 最后下标的位置清理为 \0 ？
      else if (arg == 2) argv2[index_] = NULL;// 第三个参数清空
      else if (arg == 3) argv3[index_] = NULL;// 第四个参数清空
      runCommand(); // 解析命令
      resetCommand();
    }
    
    //接收到分割命令行参数的 ' '空格
    else if (chr == ' ') {     //遇到空格了，新的参数开始了
      if (arg == 0)    arg = 1;//第二个字符开始
      else if (arg == 1)  {
        argv1[index_] = NULL;   //argv1 最大下标处的值清理
        arg = 2;                //切换到下一个字符
        index_ = 0;             //下标归零
      }
      else if (arg == 2)  {     
        argv2[index_] = NULL;   //argv2 最大下标处的值清理
        arg = 3;                //切换到下一个字符
        index_ = 0;             //下标归零
      }
       // continue; 跳过第三个 arg = 3的情况
    }
    
 // 接收字符串
    else {
      if (arg == 0) {
        // 第一个为cmd 命令标识符 (一个字节)
        cmd = chr;
      }
      else if (arg == 1) {
        // 第二个参数
        argv1[index_] = chr;//接收字符
        index_++;
      }
      else if (arg == 2) {//第三个参数 接收字符
        argv2[index_] = chr;
        index_++;
      }
      else if (arg == 3) {// 第四个参数
        argv3[index_] = chr;
        index_++;
      }
    }
		
}

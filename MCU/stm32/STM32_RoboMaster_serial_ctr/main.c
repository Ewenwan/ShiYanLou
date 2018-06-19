#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "timer.h" 
#include "control.h"
#include <string.h>//使用memset();
/************************************************

总体概述：  can 接收 0X201 ~0x208 电调 电机转子转速信息  速度 反馈信息
                接收 firefly 0x01 主机的 速度设定指令   
								pid 控制后发送 can消息 0x200 给电机电调
************************************************/
u8 debug_pid  =1;// 为1正常运行  为0 用于调试 pid


float encoder_fliter=0;
float encoder_fliter2=0;

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
	   ptForkLift->u8Cmd_receive_lag=1;//置位  命令接收标志
	   //ptForkLift->u8Can_speed_Msg_Send_flag=1;//置位
   	 ptForkLift->u16SerialComCount[0]++;//串口接收计数
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
	ptForkLift->bCanSendCounter = 0;
	ptForkLift->u8Can_Msg_Send_flag=0;//can消息发送标志
	ptForkLift->u16CanComCount[1]=ptForkLift->u16CanComCount[0]=0;
	ptForkLift2->u16CanComCount[1]=ptForkLift2->u16CanComCount[0]=0;
	ptForkLift->u16SerialComCount[1]=ptForkLift->u16SerialComCount[0]=0;
	//ptForkLift2->u16SerialComCount[1]=ptForkLift2->u16SerialComCount[0]=0;
	
	delay_ms(1000);
	
	TIM3_Int_Init();   

	printf("\r\nSystem Running\r\n");
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口中断
	
	
 	while(1)
	{	
		
if(ptForkLift->u16RunRecvPeriodMotor>=10)
	{			
		//				printf("u16RunRecvPeriodMotor %d\r\n",ptForkLift->u16RunRecvPeriodMotor );
		if(debug_pid==1)//进行 pid测试时为0  正常运行改为1
		{	
      ///*			
			if((ptForkLift->bCanComBox == CanBoxPend)||(ptForkLift2->bCanComBox == CanBoxPend)||(!(ptForkLift->u8SerialCmdFlag)))//离线状态
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
	
			else if((ptForkLift->bCanComBox == CanBoxPost)&&(ptForkLift2->bCanComBox == CanBoxPost))//在线状态 
			{		
			// ptForkLift->s16speedwant = 000;
			// ptForkLift2->s16speedwant = 000;	
			}				
    }	
	}		
		if(debug_pid)//进行 pid测试时为0  正常运行改为1
		 {
			 	if((ptForkLift->u8Cmd_receive_lag == 1)&&(ptForkLift->u8SerialCmdFlag))// 接收到了正确的 命令  且串口 接收命令正常
					{
								ptForkLift->u8Cmd_receive_lag = 0;
								//ptForkLift->s16speedwant  = ((ptForkLift->s16x + ptForkLift->s16w*0.2)*60/3.14/0.1*19.1); //底盘半径 0.2m 车轮直径 0.1m收到的为 0.1m/s
								ptForkLift->s16speedwant  = ((ptForkLift->s16x - ptForkLift->s16w*0.2)*3650);//左轮 201  减速比19.1 逆时针正转 正方向
								ptForkLift2->s16speedwant = ((-ptForkLift->s16x - ptForkLift->s16w*0.2)*3650);//右轮 202  逆时针正转 反方向
							//	printf("l %d\r\n",(int16_t)ptForkLift->s16speedwant);
							// printf("r %d\r\n",(int16_t)ptForkLift2->s16speedwant);
					}
					
			    if(ptForkLift->u8Can_Msg_Send_flag == 1)// 50ms  通过串口上报发送一次  转速 电流等
					 {
						  ptForkLift->u8Can_Msg_Send_flag =0;
						 //	printf("l %.1f %d\r",ptForkLift->s16MeanSpeed, ptForkLift->real_current);//左轮
						 //	printf("r %.1f %d\r",ptForkLift2->s16MeanSpeed, ptForkLift2->real_current);//右轮  前进为 负值							
					 }
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

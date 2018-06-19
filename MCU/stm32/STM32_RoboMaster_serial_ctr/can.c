#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "control.h" 
 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:时间段2的时间单元.范围:1~8;    CAN_BS2_1tq ... CAN_BS2_8tq
//tbs1:时间段1的时间单元.范围:1~16;	  CAN_BS1_1tq ... CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;(实际要加1,也就是1~1024) tq=(brp)*tpclk1
//注意以上参数任何一个都不能设为0,否则会乱.
//波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,普通模式;1,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
//则波特率为:36M/((1+8+7)*5)=450Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	  GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
 	  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO
	  
 	//CAN单元设置
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= mode;	         //模式设置： mode:0,普通模式;1,回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // 初始化CAN1 

 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器组0 STM32普通型的芯片CAN有14组过滤器 (互联型有28组) 每组过滤器可以有 1、2、4个过滤器
   	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
		//屏蔽位模式  CAN_FilterMode_IdMask  CAN_FxR1中某一位为1的话，CAN_FxR0中相应位的数字 必须与收到的 can的标识符一置，can消息才能通过过滤
		//标识符列表模式 CAN_FilterMode_IdList 收到的can的标识符必须与 CAN_FxR0 CAN_FxR1中的一个吻合才能通过
		// 31-21位 对应 STDID 10-0位
		// 20-3 位 对应 EXID  0-17位
		// 2位     对应 IDE
		// 1位     对应 RTR   
		// 0位     为0  不对应
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位   32位过滤器 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;// 指定21位为1其余 stdid为0   32位ID    收到的与这个 在MASK中指定的位上的数字一样
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK 指定匹配 31-21位 对应 STDID 10-0位
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;// 
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器组0关联到FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0

  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	
#if CAN_RX0_INT_ENABLE
	
  	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // 主优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 次优先级为2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // 开启can中断
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
 // printf("id:%x\r\n",RxMessage.StdId);
	
	if(RxMessage.StdId == 0x01)//firelay 发来的消息的 can_stdid  为 0x01 
	{
	 
// 得到各个电机的 转速设定值
// printf("%f\r\n",ptForkLift->s16speedwant);

		
	if (ptForkLift->s16speedwant==0)
		{
				ptForkLift->s16error[0] = 0;
				ptForkLift->s16error[1] = 0;
				ptForkLift->s16error[2] = 0;
				ptForkLift->s16ErrorSum = 0;
		}
 }
	
 // 左轮 反馈值
	if(RxMessage.StdId == 0x201)//左轮  0x201    C620编号为1的电调 CANID  ... 0x208 C620编号为8的电调 CANID
	{
		//printf("%d\r\n",(uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]));//转子角度   0~8191  >>>  0~360
		ptForkLift->speed_rpm  = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
		ptForkLift->real_current = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
	//	printf("%d\r\n",(int16_t)(ptForkLift->speed_rpm));//转子速度
		//printf("%d\r\n",(int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5])/-5);//转矩电流
		
		ptForkLift->u16CanComCount[0]++;	//CAN 1 接收 主控制器 状态 计数器	
	}
	
	//右轮  反馈值
		if(RxMessage.StdId == 0x202)//右轮 0x202   C620编号为2的电调 CANID  ... 0x208 C620编号为8的电调 CANID
	{
		//printf("%d\r\n",(uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]));//转子角度   0~8191  >>>  0~360
		ptForkLift2->speed_rpm  = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
		ptForkLift2->real_current = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
	//	printf("2:%d\r\n",(int16_t)(ptForkLift2->speed_rpm));//转子速度
		//printf("%d\r\n",(int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5])/-5);//转矩电流
		
		ptForkLift2->u16CanComCount[0]++;	//CAN 2 接收 主控制器 状态 计数器	
	}
	
/*
if(RxMessage.StdId == 0x201)// 0x201 C620编号为1的电调 CANID  ... 0x208 C620编号为8的电调 CANID
	{
	ptForkLift->last_angle = ptForkLift->angle;//转子角度  0~8191  >>>  0~360
	ptForkLift->angle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]) ;
	
 // 反馈值
	ptForkLift->last_speed_rpm = ptForkLift->speed_rpm;	//转子转速=转轴速度*减速比  -500~500 * 19 =-10000~10000 rpm	 	
	ptForkLift->speed_rpm  = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
		
		                                                  // 转子电流
  ptForkLift->real_current = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5])/-5;		
	ptForkLift->tempature = RxMessage.Data[6];          // 转子温度		

//打印反馈值 		
	printf("r %d\r\n",ptForkLift->speed_rpm);// 转子 转速反馈值
  printf("a %d\r\n",ptForkLift->angle);    // 转子  角度	
	}
*/	
 // if(RxMessage.StdId == 0x203){
 //  printf("r %d\r\n",RxMessage.Data[0]<<8|RxMessage.Data[1]);
 //  }
	
}
#endif

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(void)
{	
  u8 mbox;
  u16 i=0;	
  mbox= CAN_Transmit(CAN1, &ptForkLift->Can_Txmessage);   
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}

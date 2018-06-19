#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
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

//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 Can_Tx_Msg(u32 id,u8 ide,u8 rtr,u8 len,u8 *dat);		//发送数据
u8 Can_Msg_Pend(u8 fifox);								//查询邮箱报文
void Can_Rx_Msg(u8 fifox,u32 *id,u8 *ide,u8 *rtr,u8 *len,u8 *dat);//接收数据
u8 Can_Tx_Staus(u8 mbox);  								//返回发送状态
u8 Can_Send_Msg(u8* msg,u8 len);						//发送数据
u8 Can_Receive_Msg(u8 *buf);							//接收数据
#endif

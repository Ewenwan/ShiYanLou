#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PBout(12)// PB5
#define LED1 PEout(5)// PE5	

void LED_Init(void);//初始化
void motor_carrent_io_init(void);
void  Adc_Init(void);
u16 Get_Adc_Average(u8 ch,u8 times);
u16 Get_Adc_Average_(u8 times);
void CarID_Select(void);
void filter(void);	 	

#endif

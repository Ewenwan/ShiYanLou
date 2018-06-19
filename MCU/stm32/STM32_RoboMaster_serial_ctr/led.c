#include "led.h"
#include "control.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
/*
GPIO 模式
(1)GPIO_Mode_AIN         模拟输入 
(2)GPIO_Mode_IN_FLOATING 浮空输入
(3)GPIO_Mode_IPD         下拉输入
(4)GPIO_Mode_IPU         上拉输入
(5)GPIO_Mode_Out_OD      开漏输出
(6)GPIO_Mode_Out_PP      推挽输出
(7)GPIO_Mode_AF_OD       复用开漏输出
(8)GPIO_Mode_AF_PP       复用推挽输出
GPIO_Speed_10MHz         最高输出频率 10MHz 
GPIO_Speed_2MHz          最高输出频率 2MHz 
GPIO_Speed_50MHz         最高输出频率 50MHz
*/



/*****************系统运行指示灯 PB12**************/
//初始化PB12为输出口.并使能这个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				       //LED0-->PB.12 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					         //根据设定参数初始化GPIOB.12
	 GPIO_ResetBits(GPIOB,GPIO_Pin_12);						           //PB.12 输出低
}


void CarID_Select(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10;				 // PB0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入	  右轮为0  左轮 为1
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0	 		
	 ptForkLift->u8CarID=(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))+(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)<<1)+(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)<<2);
}

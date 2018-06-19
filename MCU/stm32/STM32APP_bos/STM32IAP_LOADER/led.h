#ifndef _LED_H_
#define _LED_H_
/*Led闪烁类*/
/*
stm32 GPIO八种模  四种输入，四种输出 每个都可以配置为中断模式 
typedef enum
{ GPIO_Mode_AIN = 0x0,         //模拟输入
  GPIO_Mode_IN_FLOATING = 0x04,//浮空输入
  GPIO_Mode_IPD = 0x28,        //弱下拉输入
  GPIO_Mode_IPU = 0x48,        //弱上拉入
  GPIO_Mode_Out_OD = 0x14,     //开漏输出
  GPIO_Mode_Out_PP = 0x10,     //推挽输出
  GPIO_Mode_AF_OD = 0x1C,      //复用开漏输出
  GPIO_Mode_AF_PP = 0x18       //复用推挽输出
}GPIOMode_TypeDef;
*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

class Led{
	public:
		Led(){
			ledInit();
		};
		void led1On();
		void led1Off();
		void led2On();
		void led2Off();
		void ledDelay();
	private:
		void ledInit(void);
		GPIO_InitTypeDef GPIO_InitStruct;	// //GPIO_InitStruct:????GPIO_InitTypeDef???,?????GPIO?????
};

void Led::ledInit(void)
{
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;//引脚选择
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE); // 端口时钟使能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStruct);	
}

void Led::led1Off()
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);	
}
void Led::led2Off()
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_14);
}

void Led::led1On()
{
	GPIO_SetBits(GPIOD, GPIO_Pin_13);	
}

void Led::led2On()
{
	GPIO_SetBits(GPIOG, GPIO_Pin_14);
}

void Led::ledDelay()
{
	for(int32_t i=0xfffef; i>0; i--);
}

#endif

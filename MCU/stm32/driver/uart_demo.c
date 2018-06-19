#include "stm32f10x.h"
#include "uart.h"
void GPIO_Config(void)//GPIO配置
{
   GPIO_InitTypeDef GPIO_InitStructure; 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能gpioc的时钟		

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  //选择管脚PC.13
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //管脚速度为50M
 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //设置输出模式为推挽输出
   GPIO_Init(GPIOC, &GPIO_InitStructure);              //将上述设置写入到GPIOC里去
	
		 
}

 	void NVIC_Config(void)         //中断控制器的配置
{
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     //优先组设置
NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;     //TIM2中断选通 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         //子优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断控制
NVIC_Init(&NVIC_InitStructure);
}
void Timer_Config(void)         //定时器的配置
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;    
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  //Timer2 时钟使能 
TIM_DeInit(TIM2);                                //复位TIM2定时器       
TIM_TimeBaseStructure.TIM_Period=1000;          //定时器周期
TIM_TimeBaseStructure.TIM_Prescaler=36000-1;    //预分频数
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM2时钟分频,为1表示不分频
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//定时器计数为向上计数模式
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);      
TIM_ClearFlag(TIM2, TIM_FLAG_Update);         //清除定时器2的溢出标志位
TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      //使能定时器2溢出中断
TIM_Cmd(TIM2, ENABLE);                        //定时器2使能
}


int main(void)
{ 
  SystemInit();//初始化时钟，配置为72MHz，我试过将这句注释掉，好像不影响结果。查了一下，在配置
	             //main函数之前的启动代码有这样一句  LDR R0, =SystemInit，我疑惑的是难道启动的时候就配成72Mhz?  
  GPIO_Config(); 
	NVIC_Config();
	Timer_Config();
	usart_debug_config();
  USART1_printf( USART1, "*****************************\r\n" );	
	USART1_printf( USART1, "*****************************\r\n" );
	USART1_printf( USART1, "******定时器与串口打印*******\r\n" );
	USART1_printf( USART1, "*****************************\r\n" );
 
  while(1)
		
	{  	
    ;
   }
	 
 }


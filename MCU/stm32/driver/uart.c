
/*
当你需要设计一个上位机是串口通信必然是最基本的组件，STM32固件库为串口通信的操作提供了很多有用的函数，
使得我们在使用串口时不必关注底层硬件的操作，使用相关函数即可，下面介绍串口操作的要点。
使用串口必然就要用到GPIO口，
那么第一步就是配置GPIO口，STM32USART1的发送和接受引脚分别是PA9，PA10，初始化该两只引脚的操作如下：

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOA, &GPIO_InitStructure);
配置PA9为复用推挽输出，PA10为浮空输入。
打开PA口和串口时钟：
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
配置串口工作模式：
USART_InitStructure.USART_BaudRate = 115200; 
USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
USART_InitStructure.USART_StopBits = USART_StopBits_1; 
USART_InitStructure.USART_Parity = USART_Parity_No ; 
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; USART_Init(USART1, &USART_InitStructure); 

 
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
USART_Cmd(USART1, ENABLE);
配置接受中断：
NVIC_InitTypeDef NVIC_InitStructure; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
 NVIC_Init(&NVIC_InitStructure);
 
 
以上均为串口硬件的配置，为了方便的使用stdio.h中的printf函数，还需要重定向fputc函数：
///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
USART_SendData(USART1, (uint8_t) ch);
while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
return (ch);
}
好了，下面就可以直接在主函数中使用printf函数输出了，在接收字符时，需要在中断函数中编写相应的函数：
void USART1_IRQHandler(void)
  { 
 unsigned char ch;
 if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) 
 ch = USART_ReceiveData(USART1); 
 printf("%c",ch); 
}
效果如下：在主函数中输出想要输出的内容，在串口调试助手中输入内容，最后所有字符都显示在串口调试助手的接收处：

*/

#include"uart.h"

void GPIO_Configuration(void) 
{ 
GPIO_InitTypeDef  GPIO_InitStructure; 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    
GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9; 
GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
 
GPIO_Init(GPIOA,&GPIO_InitStructure); 
 
GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
GPIO_Init(GPIOA,&GPIO_InitStructure); 
} 

void USART1_config(void)
{
USART_InitTypeDef	USART_InitStructure;
USART_InitStructure.USART_BaudRate= 115200;             
USART_InitStructure.USART_WordLength = USART_WordLength_8b;   
USART_InitStructure.USART_StopBits = USART_StopBits_1;      
USART_InitStructure.USART_Parity= USART_Parity_No ;         
 
USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; 
 
USART_InitStructure.USART_Mode =USART_Mode_Rx |USART_Mode_Tx;
USART_Init(USART1, &USART_InitStructure);  

USART_Cmd(USART1, ENABLE);  
}


void usart_debug_config(void)
{			 
   GPIO_Configuration();
   USART1_config();

}

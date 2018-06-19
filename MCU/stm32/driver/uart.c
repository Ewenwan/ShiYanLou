
#include"usart_debug.h"

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

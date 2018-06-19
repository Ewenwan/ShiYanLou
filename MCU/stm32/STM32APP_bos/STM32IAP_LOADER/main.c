/*******************************************************************************
** 文件名: 		mian.c
** 版本：  		1.0
** 工作环境: 	RealView MDK-ARM 5
** 作者: 		wuguoyana
** 生成日期: 	2011-04-28
** 功能:		USART初始化和RCC设置，然后从common.c中执行主菜单
** 相关文件:	stm32f10x.h
** 修改日志：	2011-04-29   创建文档
*******************************************************************************/
/* 包含头文件 *****************************************************************/
#include "common.h"
//#include "stm32f10x_flash.icf"
/* 类型声明 ------------------------------------------------------------------*/
/* 宏 ------------------------------------------------------------------------*/
//#define LED2   GPIO_Pin_6
//#define LED3   GPIO_Pin_7
//#define LED4   GPIO_Pin_8
//#define LED5   GPIO_Pin_9
#define Sys_Led   GPIO_Pin_12   // PB12
#define Update_Key GPIO_Pin_10  // PB10
//#define STM32F10X_MD
/* 变量 ----------------------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

u8 g_bUpdateFlag=0x00;
u8 g_b_receive_UpdateFlag=0x00;
//extern void Serial_PutString(uint8_t *s);

//串口接收
//u8 RxBuffer[6]={'o'};
//u8 i=0;


/* 函数声明 ------------------------------------------------------------------*/
void Delay(__IO uint32_t nCount);
void LED_Configuration(void);
static void IAP_Init(void);
void KEY_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);



/* 函数功能 ------------------------------------------------------------------*/


/*******************************************************************************
  * @函数名称	main
  * @函数说明   主函数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
/*
 // IAP程序 中断向量表设置
NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   // NVIC_VectTab_FLASH 宏定义 值为0x08000000
 // 用户程序 中断向量表设置
NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000); // 与 common.h 中 ApplicationAddress 地址对应的IAP大小对应

*/
int main(void)
{
	
  //系统复位重新加载选项字
 // NVIC_SystemReset();
    // 中断向量表 0x8000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); // 初始化中断向量表地址  防止从用户程序跳转过来 向量表改变了
	
	  //FLASH_WriteByte(g_bUpdateFlag_Address,0x00);
    //Flash 解锁
    LED_Configuration(); // 配置 系统指示灯
    KEY_Configuration(); // 配置升级按键
    IAP_Init();          // 配置串口
	
    //按键是否按下串口是否收到数据  或 U盘是否插入(stm32需要扩展SD卡软硬件部分)  或者检查flash中固定位置上的标志(有串口发送数据改变)
    // http://blog.csdn.net/xiaobai20131/article/details/50890275	
	  // 读取某个FLASH地址 中的值(升级标志)
    g_bUpdateFlag = FLASH_ReadByte(g_bUpdateFlag_Address);
		FLASH_WriteByte(g_bUpdateFlag_Address,0x00); //清理 升级标志
	
	//FLASH写保护 解锁 
	 FLASH_Unlock();
	
    if ( g_bUpdateFlag==0x01)//||GPIO_ReadInputDataBit(GPIOB,Update_Key)  == 0x00))//||(g_b_receive_UpdateFlag==0x01))  
			//上电时启动 在线升级标志(按键触发 ) 或者 串口发送设置标志触发
    {
        //假如按键按下 或其他升级标志
        //执行IAP驱动程序更新Flash程序

        SerialPutString("\r\n============================================================");
        SerialPutString("\r\n=              (C) COPYRIGHT 2017 Lierda                    =");
        SerialPutString("\r\n=                                                           =");
        SerialPutString("\r\n=   In-Application Programming Application  (Version 1.0.0) =");
        SerialPutString("\r\n=                                                           =");
        SerialPutString("\r\n=                By wyw                                     =");
        SerialPutString("\r\n=============================================================");
        SerialPutString("\r\n\r\n");
        Main_Menu();
    }
    //否则执行用户程序
    else
    {
       go_to_usr_application();
    }

    while (1)
    {
    }
}


/*******************************************************************************
  * @函数名称	LED_Configuration
  * @函数说明   配置使用LED
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void LED_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //使能LED所在GPIO的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
    //初始化LED的GPIO
    GPIO_InitStructure.GPIO_Pin = Sys_Led;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,Sys_Led);  //点亮Sys_Led

}

/*******************************************************************************
  * @函数名称	KEY_Configuration
  * @函数说明   按键初始化
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void KEY_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		  //  升级按键接口IO配置
	  GPIO_InitStructure.GPIO_Pin = Update_Key;				// PB10
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	// 上拉输入
	  GPIO_Init(GPIOB, &GPIO_InitStructure);				  // 初始化PB10
}

/*******************************************************************************
  * @函数名称	GPIO_Configuration
  * @函数说明   配置使用USART1的相关IO管脚 IAP_Init() 函数调用
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // 使能 USART1、B端口以及 端口复用功能  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE); //使能端口重映射 可以按照官方 重映射 表格进行功能管脚的重映射
    //USART1_TX 发送管脚  GPIOB.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;          //PB.6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 速度
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);             // 初始化GPIOB.6
   
    //USART1_RX	接收管脚  GPIOB.7 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;            //PB7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化GPIOB.7  
}
   
/*******************************************************************************
  * @函数名称	IAP_Init
  * @函数说明   配置使用IAP  配置串口与上位机  终端通信
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void IAP_Init(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USART1 配置 ------------------------------------------------------------
         USART1 配置如下:
          - 波特率      = 115200 baud
          - 字长        = 8 Bits
          - 一个停止位
          - 无校验
          - 无硬件流控制
          - 接受和发送使能
    --------------------------------------------------------------------------*/
    USART_InitStructure.USART_BaudRate = 115200;  // 波特率 115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 数据位 8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     // 停止位 1位
    USART_InitStructure.USART_Parity = USART_Parity_No;        // 无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// 无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                // 串口发送和接收
/*
	//串口中断配置
	  NVIC_InitTypeDef NVIC_InitStructure;
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		// 接受中断使能
 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
*/		
    GPIO_Configuration(); // 串口收发管脚配置
    USART_Init(USART1, &USART_InitStructure);// 串口初始化
    USART_Cmd(USART1, ENABLE);    // 使能 USART1 串口1
}

/*******************************************************************************
  * @函数名称	Delay
  * @函数说明   插入一段延时时间
  * @输入参数   nCount: 指定延时时间长度
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}


	/*
 向FLASH的addr中写如数据 flashdata1
	void FLASH_WriteByte(u32 addr ,u16 flashdata1)
{
 FLASH_Status FLASHstatus = FLASH_COMPLETE;
 FLASH_Unlock();//??
 FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
 FLASHstatus=FLASH_ErasePage(addr);//????
 FLASHstatus=FLASH_ProgramHalfWord(addr, flashdata1);//????
 FLASH_Lock();//??
}
	#define Flash_update_flag_Address 0x08014000
	读取FLASH 
	rdata=*(u16 *)Flash_update_flag_Address
	*/

#ifdef  USE_FULL_ASSERT

/*******************************************************************************
  * @函数名称	assert_failed
  * @函数说明   报告在检查参数发生错误时的源文件名和错误行数
  * @输入参数   file: 源文件名
  				line: 错误所在行数
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* 用户可以增加自己的代码用于报告错误的文件名和所在行数,
       例如：printf("错误参数值: 文件名 %s 在 %d行\r\n", file, line) */

    //死循环
    while (1)
    {
    }
}
#endif

/*
// 串口1的接收中断函数
void USART1_IRQHandler(void)
{
	
//======================>> 接收数据中断
    if( USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET )
    {
		  RxBuffer[i] = USART_ReceiveData(USART1);
			i++;
			if(i>=6) i=0;
		if((RxBuffer[0]== 'u')&&(RxBuffer[1]== 'p')&&(RxBuffer[2]== 'd')&&(RxBuffer[3]== 'a')&&(RxBuffer[4]== 't')&&(RxBuffer[5]== 'e'))
		{
			  g_b_receive_UpdateFlag = 0x01;	
    }				
        USART_ClearITPendingBit( USART1, USART_IT_RXNE ); 
    }
}
*/


//  NVIC_SystemReset();    重启系统函数


/*******************************文件结束***************************************/

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


#define ADC1_DR_Address    ((uint32_t)0x4001244C)
// #define ADC3_DR_Address    ((uint32_t)0x40013C4C)
// #define ADC1_DR_Address    ((u32)0x40012400+0x4c)   

// 参考手册 P178  0x4c  为ADC1_DR 的具体地址
//0x40012400  为ADC寄存器的首地址    P28 

// 电流采样 均值滤波
#define num  2
__IO uint16_t ADC1ConvertedValue_[num]={0};

//__IO uint16_t ADC1ConvertedValue = 0;

// define     __IO    volatile  易失型变量  需要直接读取内存地址中的值 
// typedef unsigned short int  uint16_t;
 


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
	/* 老板子  PA2 PA3 PA4 PA5  全部低电平 */
	/* 新板子  PB0  PB1  PB10   全部高电平 */
	/*
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // PA5
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入	  老板子为 0  新板子为 1
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.5
	 ptForkLift->u8CarID=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);//   0 右侧   1左侧(最新)

	*/
	
	/* 新板子1  PB0  PB1  PB10   右轮 接地 为0 */
	/* 新板子2  PB0  PB1  PB10   左轮 悬空 上拉输入 全部高电平 */	
   GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 // PB0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入	  右轮为0  左轮 为1
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.0
	 		
	// ptForkLift->u8CarID=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);//   0 右侧   1左侧(最新)
		 ptForkLift->u8CarID=(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))+(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)<<1)+(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)<<2);

	
}





// ADC  DMA传输 电机母线电流采 端口配置   PA6

void motor_carrent_io_init(void)
 {

	 /*老板子新板子 电机母线电流采样 均为  PA6口*/
	 // 采样电阻 为 两个 0.01R 并联  即为 0.05R  即5mR	     内部固定放大10*2倍   经过1.8 和 3.3 电阻分压
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
	
//DMA中断	 
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	//端口时钟配置	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // ADC1时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA口 时钟使能
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   // 开启 DMA1 的时钟
	 
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   // ADC采样时钟为 72MHz / 6  = 12MHz    最高不能超过14MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //设置PA6口为电流采样口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
  DMA_DeInit(DMA1_Channel1);//复位 DMA1 将外设DMA1的全部寄存器设置为缺省值 
  //配置DMA传输方式
  DMA_InitStructure.DMA_PeripheralBaseAddr=  ADC1_DR_Address;   //外设基地址 (uint32_t) (u32)&(ADC1->DR);  ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr= (u32)&(ADC1ConvertedValue_); //内存基地址  u32 可以试试  (uint32_t 不太好!!!)
  
	DMA_InitStructure.DMA_DIR= DMA_DIR_PeripheralSRC; //DMA传输方向    外设到内存
  /**需要修改的地方**/
	DMA_InitStructure.DMA_BufferSize = num;             // DMA通道的DMA缓存的大小 传输单位  如果需要滤波 可设置多个 进中断处理
  DMA_InitStructure.DMA_PeripheralInc= DMA_PeripheralInc_Disable;//外设地址指针增量设为不自动增加 外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc= DMA_MemoryInc_Enable; //内存地址指针增量设为不自动增加 内存地址寄存器也不变  如果 传输单位超过1 则应设置为 DMA_MemoryInc_Enable
  
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_HalfWord;//外设数据半字 2个字节     16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;       //内存数据大小设为半字     16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;    //DMA传输方式设为循环传输   循环写//内存基地址 buffer  // DMA_Mode_Normal;  //工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA优先级设为高           // DMA_Priority_Medium; //DMA通道 x拥有中优先级 
  /*软件优先级 00低 01 中 10 高  11最高，硬件优先级 同软件优先级 通道号低的 优先级高  DMA1 1~7个通道 DMA2 1~5个*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;       //内存到内存的传输失能
  DMA_Init(DMA1_Channel1,&DMA_InitStructure);        //初始化DMA1，开启通道1
  //DMA_Cmd(DMA1_Channel1,ENABLE);                   //使能 DMA 通道1  放在 ADC校准初始化后


// DMA中断使能  可以在  主程序 内开启中断
 DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //使能 DMA1传输完成Transfer complete中断（达到指定的DMA_BufferSize大小后） 可用于中断滤波

//DMA 中断配置
  // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 中断向量组配置
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 或者改为3 试试
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);          // Enable the DMA Interrup
/////////////////////


// ADC 模式配置
  ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // ADC 工作在独立模式
 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;       // 单通道采样 扫描关闭 ENABLE 使能多通道采样 这里可是改为 ENABLE 试一试
  /**需要修改的地方**/	
  ADC_InitStructure.ADC_ContinuousConvMode= DISABLE;   // 连续转换模式 即不停的 进行AD转换  DISABLE为单次转换模式  或者改为 DISABLE  单通道单次采样 
  
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 非外部触发  有软件控制触发转换  软件触发为 设置 ADC_CR2 寄存器ADON 
  ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;              // 数据右对齐  位数12  不足16位所以右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;                            // 采样一个通道	
  ADC_Init(ADC1,&ADC_InitStructure);
	
	//配置ADC1的通道6 PA6 转换次数为1   采样时间  为28.5个采样周期  ADC_SampleTime_55Cycles5  ADC_SampleTime_239Cycles5
  ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_28Cycles5);
  ADC_DMACmd(ADC1,ENABLE);//使能DMA-ADC
  ADC_Cmd(ADC1,ENABLE);   //使能ADC1
	
	ADC_ResetCalibration(ADC1);                //复位校准寄存器
  while(ADC_GetResetCalibrationStatus(ADC1));/* 检查 重置校准寄存器 的状态  等待复位校准结束*/
  ADC_StartCalibration(ADC1);                //开始校准 Start ADC1 calibration 
  while(ADC_GetCalibrationStatus(ADC1));     // 检查校准寄存器的状态    等待校准结束
	
	 ADC_SoftwareStartConvCmd(ADC1,ENABLE);     // 使能ADC1 的软件转换功能  可以在主程序  开启功能
	
 // ADC_SoftwareStartConvCmd(ADC1,ENABLE);     // 使能ADC1 的软件转换功能
	
	//	ADC_Value = (float)(ptForkLift->ADC1ConvertedValue)/4096*3.3; 位电压值
	
	 DMA_Cmd(DMA1_Channel1,ENABLE);                     //使能 DMA 通道1	
 }

 //中断标志位方式
 //ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                       
//while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )); 
//ADC_GetConversionValue(ADC1);

 // DMA1_Channel1 的中断服务函数
 
//float ADC_Value1;
//#define N 5   // 应该与 DMA_InitStructure.DMA_BufferSize  一样大  单个通道
void filter(void)
{
  u16  sum = 0;
  u8 count;
    for ( count=0;count<num;count++)
    {
       sum += ADC1ConvertedValue_[count];// 注意 ptForkLift->ADC1ConvertedValue 应设置为相应大小的数组  ADC1ConvertedValue[10]
    // sum  += ptForkLift->ADC1ConvertedValue;   
		}
    ptForkLift->u32ADCResult = sum/num;
    sum=0;
		ptForkLift->ADC_Value = (float)(ptForkLift->u32ADCResult)*33/40960;
		//ptForkLift->u16ADC_Value = (unsigned int)(ptForkLift->u32ADCResult)*3300>>12;  // 放大1000倍
		//ptForkLift->u16ADC_Value = (unsigned int)(ptForkLift->u32ADCResult)*3300/4096;  // 放大1000倍
		ptForkLift->u16ADC_Value = (u16)(ptForkLift->ADC_Value*1000.0);// 放大1000倍
		
		//ptForkLift->I_Value = (float)(ptForkLift->ADC_Value)/3.3*(1.8+3.3)/20/0.005;
		//ptForkLift->I_Value = (float)__fabs(1.597 - ptForkLift->ADC_Value)*15.4545454545454545;
		
		//前
		if(ptForkLift->u8CarID==0){
		ptForkLift->I_Value = (float)__fabs(1.596 - ptForkLift->ADC_Value)*5.934;//*15.455*0.384;
		ptForkLift->u16I_Value = (u16)(ptForkLift->I_Value*1000);  // 放大1000
		}
		//最新
		else if(ptForkLift->u8CarID==1){
		ptForkLift->I_Value = (float)__fabs(1.599 - ptForkLift->ADC_Value)*5.934;//15.455*0.49;
		ptForkLift->u16I_Value = (u16)(ptForkLift->I_Value*1000);  // 放大1000
		}
		//printf("\r\n电机电流采样\t");
		//printf("%d\r\n",ptForkLift->u16I_Value);
	//printf("%f\r\n",ptForkLift->ADC_Value);
		
}
 
 void DMA1_Channel1_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
  {
  filter();
  DMA_ClearITPendingBit(DMA1_IT_TC1);  //清中断标志位
	//DMA_ClearFlag(DMA1_FLAG_TC1);        /*新加入*/
  }
}
 
 

//初始化ADC 普通 
//这里我们仅以规则通道为例
//我们默认将开启通道6																   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA6 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1 工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1	
	ADC_ResetCalibration(ADC1);	//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
// Get_Adc_Average(ADC_Channel_6 , 5);
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 

 

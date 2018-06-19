/*


12位ADC是一种逐次逼近型模拟数字数字转换器。它有多达18个通道，可测量16个外部和2个内部信号源。

ADC的输入时钟不得超过14MHZ，它是由PCLK2经分频产生。

如果被ADC转换的模拟电压低于低阀值或高于高阀值，AWD模拟看门狗状态位被设置。

关于ADC采样与DMA关系，引用网上一段解释：

 
STM32 时钟不算快，72MHZ，
也不能扩展大容量的RAM FLASH，
同样没有 DSP 那样强大的指令集。
它的优势在哪里呢？
---就在快速采集数据，快速处理上。

ARM 的特点就是方便。
这个快速采集，高性能的ADC 就是一个很好的体现，
12 位精度，最快1uS 的转换速度，通常具备2 个以上独立的ADC 控制器，
这意味着，
STM32 可以同时对多个模拟量进行快速采集，
这个特性不是一般的MCU具有的。
以上高性能的 ADC，配合相对比较块的指令集和一些特色的算法支持，
就构成了STM32 在电机控制上的强大特性。
好了，正题，怎末做一个简单的ADC，注意是简单的，
ADC 是个复杂的问题，涉及硬件设计，电源质量，参考电压，信号预处理等等问题。
我们只就如何在MCU内完成一次ADC 作讨论。
谈到 ADC，我们还要第一次引入另外一个重要的设备DMA.
DMA是什么东西呢。
通常在 8 位单片机时代，很少有这个概念。
在外置资源越来越多以后，
我们把一个MCU内部分为主处理器和 外设两个部分。
主处理器当然是执行我们指令的主要部分，
外设则是串口 I2C ADC 等等用来实现特定功能的设备
回忆一下，8 位时代，我们的主处理器最常干的事情是什么？
逻辑判断？不是。那才几个指令
计算算法？不是。大部分时候算法都很简单。
事实上，主处理器就是作个搬运工，
把 USART 的数据接收下来，存起来
把 ADC 的数据接收下来，存起来
把要发送的数据，存起来，一个个的往USART 里放。
…………

为了解决这个矛盾，
人们想到一个办法，让外设和内存间建立一个通道，
在主处理器允许下，
让外设和内存直接读写，这样就释放了主处理器，
这个东西就是DMA。
打个比方：
一个MCU是个公司。
老板就是主处理器
员工是外设
仓库就是内存
从前 仓库的东西都是老板管的。
员工需要原料工作，就一个个报给老板，老板去仓库里一个一个拿。
员工作好的东西，一个个给老板，老板一个个放进仓库里。
老板很累，虽然老板是超人，也受不了越来越多的员工和单子。
最后老板雇了一个仓库保管员，它就是DMA
他专门负责入库和出库，
只需要把出库和入库计划给老板过目
老板说 OK，就不管了。
后面的入库和出库过程，
员工只需要和这个仓库保管员打交道就可以了。
--------闲话，马七时常想，让设备与设备之间开DMA，岂不更牛X
比喻完成。
ADC 是个高速设备，前面提到。
而且 ADC 采集到的数据是不能直接用的。即使你再小心的设计外围电路，测的离谱的数据总会出现。
那么通常来说，是采集一批数据，然后进行处理，这个过程就是软件滤波。
DMA用到这里就很合适。让ADC 高速采集，把数据填充到RAM 中，填充一定数量，比如32 个，64 个MCU再来使用。
-----多一句，也可以说，单次ADC 毫无意义。
下面我们来具体介绍，如何使用DMA来进行ADC 操作。
初始化函数包括两部分，DMA 初始化和ADC 初始化
我们有多个管理员--DMA
一个管理员当然不止管一个DMA 操作。所以DMA有多个Channel

以下是程序分析：
程序基于STM32F103VET6，库函数实现


RCC部分：（忽略系统时钟配置）
   //启动DMA时钟 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);   
    //启动ADC1时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
GPIO部分：(ADC引脚参见上表)
   //ADC_CH10--> PC0    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入   
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    // PC2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   
    GPIO_Init(GPIOC, &GPIO_InitStructure);   
	
ADC1配置：（两外部输入，另采样内部温度传感器）

void ADC1_Configuration(void)   
{   
    ADC_InitTypeDef ADC_InitStructure;   
   
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //转换模式为独立，还有交叉等非常多样的选择   
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;       // 多通道采样 
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续转换开启    
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   
    ADC_InitStructure.ADC_NbrOfChannel = 3;     //设置转换序列长度为3，三通道    
    ADC_Init(ADC1, &ADC_InitStructure);   
      
    //ADC内置温度传感器使能（要使用片内温度传感器，切忌要开启它）    
    ADC_TempSensorVrefintCmd(ENABLE);   
      
    //常规转换序列1：通道10    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);   
    //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5); 
     ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 3, ADC_SampleTime_239Cycles5);  
     //输入参数：ADC外设，ADC通道，转换序列顺序，采样时间 
    // Enable ADC1    
    ADC_Cmd(ADC1, ENABLE);   
    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）    
    ADC_DMACmd(ADC1, ENABLE);   
      
    // 下面是ADC自动校准，开机后需执行一次，保证精度    
    // Enable ADC1 reset calibaration register    
    ADC_ResetCalibration(ADC1);   
    // Check the end of ADC1 reset calibration register    
    while(ADC_GetResetCalibrationStatus(ADC1));   
   
    // Start ADC1 calibaration    
    ADC_StartCalibration(ADC1);   
    // Check the end of ADC1 calibration    
    while(ADC_GetCalibrationStatus(ADC1));   
    // ADC自动校准结束---------------    
     ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ADC启动    
}  
DMA配置：（无软件滤波）
void DMA_Configuration(void)   
{   
    DMA_InitTypeDef DMA_InitStructure;   
      
    DMA_DeInit(DMA1_Channel1);   
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;   //DMA外设地址，在头部定义
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;        //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //外设至内存模式
    //BufferSize=2，因为ADC转换序列有2个通道    
    //如此设置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]    
    DMA_InitStructure.DMA_BufferSize = 3;                                           //一次转换三个
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //接受一次后，设备地址不后移
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;        //接受一次后，内存地址后移
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   //每次传输半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   
    //循环模式开启，Buffer写满后，自动回到初始地址开始传输    
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);   
    //配置完成后，启动DMA通道    
    DMA_Cmd(DMA1_Channel1, ENABLE);   
} 
此DMA例程用于单次ADC转换，配合软件滤波可做如下改动：
全局声明：
vu16 AD_Value[30][3];   //AD采样值
vu16 After_filter[3];   //AD滤波后

DMA部分：(带中断滤波)
void DMA_Configuration(void)   
{   
    DMA_InitTypeDef DMA_InitStructure;   
      
    DMA_DeInit(DMA1_Channel1);   
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;   
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;   
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;   
    //BufferSize=2，因为ADC转换序列有2个通道    
    //如此设置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]    
    DMA_InitStructure.DMA_BufferSize = 90;   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;   
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   
    //循环模式开启，Buffer写满后，自动回到初始地址开始传输    
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);   
    //配置完成后，启动DMA通道    
    DMA_Cmd(DMA1_Channel1, ENABLE);
DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //使能DMA传输完成中断 
   
} 
NVIC部分：
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQChannel; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);          // Enable the DMA Interrupt 
stm32f10x_it.c文件：
void DMA1_Channel1_IRQHandler(void)
{
if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
{
  filter();
  DMA_ClearITPendingBit(DMA1_IT_TC1);
}
}
滤波部分：（均值滤波）
#define N 30
void filter(void)
{
   int  sum = 0;
   u8 count,i;
   for(i=0;i<2;i++)
   {
    for ( count=0;count<N;count++)
    {
       sum += AD_Value[count][i];
    }
    After_filter[i]=sum/N;
    sum=0;
   }
}
采样数据与实际电压/温度转换：
u16 GetTemp(u16 advalue)   
{   
    u32 Vtemp_sensor;   
    s32 Current_Temp;   
      
//    ADC转换结束以后，读取ADC_DR寄存器中的结果，转换温度值计算公式如下：    
//          V25 - VSENSE    
//  T(℃) = ------------  + 25    
//           Avg_Slope    
//   V25：  温度传感器在25℃时 的输出电压，典型值1.43 V。    
//  VSENSE：温度传感器的当前输出电压，与ADC_DR 寄存器中的结果ADC_ConvertedValue之间的转换关系为：    
//            ADC_ConvertedValue * Vdd    
//  VSENSE = --------------------------    
//            Vdd_convert_value(0xFFF)    
//  Avg_Slope：温度传感器输出电压和温度的关联参数，典型值4.3 mV/℃。    
   
    Vtemp_sensor = advalue * 330 / 4096;   
    Current_Temp = (s32)(143 - Vtemp_sensor)*10000/43 + 2500;   
    return (s16)Current_Temp;   
}    
    
u16 GetVolt(u16 advalue)   
{
   
    return (u16)(advalue * 330 / 4096);   
} 
滤波部分思路为：ADC正常连续采样三个通道，由DMA进行搬运，一次搬运90个数据，即为1-2-3-1-2-3循环，
每个通道各30次，存在 AD_Value[30][3]中，30为每通道30个数据，3为三个通道，
根据二维数组存储方式此过程自动完成。而每当一次DMA过程结束后，触发 DMA完成中断，
进入滤波函数将30个数据均值成一个， 存入After_filter[3]。
整个过程滤波计算需要CPU参与，而在程序中采样结果值随时均为最新，
尽力解决程序复杂性和CPU负载。 x=GetVolt(After_filter[0]);即可得到即时电压值。
*/

 #include "adc.h"
 #include "delay.h"

/*
adc.h
#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 

#endif 

*/		   
//初始化ADC1  可以为   ADC1 ~ ADC3 
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	// 初始化结构体 
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	// 外设时钟使能 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
	// ADC时钟配置 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12M,  ADC最大时间不能超过14M

	// 端口配置  PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	// ADC1模式设置  
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	    //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	            //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	                //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	// 使能  校验 
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1	
	ADC_ResetCalibration(ADC1);	//使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束	
	ADC_StartCalibration(ADC1);	 //开启AD校准 
	while(ADC_GetCalibrationStatus(ADC1));	    //等待校准结束
 
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

	return ADC_GetConversionValue(ADC1);        	//返回最近一次ADC1规则组的转换结果
}

// 均值滤波
//    	u16 adcx; 	float temp; 
// 调用示例 adcx=Get_Adc_Average(ADC_Channel_1,10);   电压值 temp=(float)adcx*(3.3/4096);  adcx=temp; temp-=adcx;  temp*=1000; 小数部分  
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


///////////////////////////////////////////////////////////////////
////// adc  dma传输



/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

//地址 
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)

// 初始化 结构体变量 
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
__IO uint16_t ADC1ConvertedValue = 0, ADC3ConvertedValue = 0;

// #define num  20
// __IO uint16_t ADC1ConvertedValue[num] = {0}

// 配置初始化函数 
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* 系统时钟配置 System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* 中断优先级 配置NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* 输入输出口配置 GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

  /* 直接内存存取控制器配置 加快速度 DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;     //源地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC1ConvertedValue;//结果保存地址 (uint32_t) 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = num;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//源地址  不增加
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  // DAM 转移的目标地址 递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //循环转移   即 目标地址 一有数据 则进行转移  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
  /* 使能 Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);  //可能要放到   ADC 使能后 放置 错位

  
  // DMA 传输完成中断
 DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); //ê1?ü DMA1′?ê?íê3éTransfer complete?D??￡¨′?μ????¨μ?DMA_BufferSize′óD?oó￡? ?éó?óú?D????2¨

//DMA  中断优先级配置
  // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //主优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //次优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);                // Enable the DMA Interrup
/////////////////////
//  中断滤波 采样多次 均值滤波
/*
//#define N num   // 
void filter(void)
{
  u32  sum = 0;
  u8 count;
    for ( count=0;count<num;count++)
    {
       //sum += ptForkLift->ADC1ConvertedValue[count];// ×￠òa ptForkLift->ADC1ConvertedValue ó|éè???a?àó|′óD?μ?êy×é  ADC1ConvertedValue[10]
     sum += ADC1ConvertedValue_[count];
			// sum  += ptForkLift->ADC1ConvertedValue;   
		}
    ptForkLift->u32ADCResult = sum/num;
    sum=0;
		ptForkLift->ADC_Value = (float)(ptForkLift->u32ADCResult)*33/40960;
		ptForkLift->u16ADC_Value = (unsigned int)(ptForkLift->u32ADCResult)*3300>>12;  // ·?′ó1000±?
		
		//ptForkLift->I_Value = (float)(ptForkLift->ADC_Value)/3.3*(1.8+3.3)/20/0.005;
		//ptForkLift->I_Value = (float)__fabs(1.6 - ptForkLift->ADC_Value)*15.4545454545454545;
		ptForkLift->I_Value = (float)__fabs(1.5991 - ptForkLift->ADC_Value)*15.454545;
		ptForkLift->u16I_Value = (unsigned int)__fabs(1599 - ptForkLift->u16ADC_Value)*15.455;  // ·?′ó1000±?
		//printf("\r\nμ??úμ?á÷2é?ù\t");
		//printf("%f\r\n",ptForkLift->ADC_Value);
		
}

// DMA1 通道1 中断程序
void DMA1_Channel1_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
  {
	//printf("DMA_IT\r\n");	
  filter();
	//ptForkLift->ADC_Value = (float)(ptForkLift->u32ADCResult)/4096*3.3;
	//ptForkLift->ADC_Value = (float)(ADC1ConvertedValue_*33)/40960; 
	  //printf("%f\r\n",ptForkLift->ADC_Value);
		//printf("%d\r\n",ptForkLift->u16ADC_Value);
		//printf("%f\r\n",ptForkLift->I_Value);
		printf("%d\r\n",ptForkLift->u16I_Value);
    DMA_ClearITPendingBit(DMA1_IT_TC1);  //
	//DMA_ClearFlag(DMA1_FLAG_TC1);     
  }
}
  

*/
  
  
  
  /* DMA2 channel5 configuration ----------------------------------------------*/
  DMA_DeInit(DMA2_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);  
  /* 使能 Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, ENABLE);


//   ADC1 的初始化  
  /* ADC1 configuration ------------------------------------------------------*/
      /* 默认值
	      ADC_Mode                ADC_Mode_Independent
		  ADC_ScanConvMode        DISABLE
		  ADC_ContinuousConvMode  DISABLE
		  ADC_ExternalTrigConv    ADC_ExternalTrigConv_T1_CC1
		  ADC_DataAlign           ADC_DataAlign_Right
		  ADC_NbrOfChannel        1
	  
	  */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// 工作模式
     /* 
         ADC_Mode_Independent      ADC1 和 ADC2 工作在独立模式
         ADC_Mode_RegInjecSimult ADC1 和 ADC2 工作在同步规则和同步注入模式
         ADC_Mode_RegSimult_AlterTrig ADC1 和 ADC2 工作在同步规则模式和交替触发模式
         ADC_Mode_InjecSimult_FastInterl ADC1 和 ADC2 工作在同步规则模式和快速交替模式
         ADC_Mode_InjecSimult_SlowInterl ADC1 和 ADC2 工作在同步注入模式和慢速交替模式
         ADC_Mode_InjecSimult ADC1 和 ADC2 工作在同步注入模式
         ADC_Mode_RegSimult ADC1 和 ADC2 工作在同步规则模式
         ADC_Mode_FastInterl ADC1 和 ADC2 工作在快速交替模式
         ADC_Mode_SlowInterl ADC1 和 ADC2 工作在慢速交替模式
         ADC_Mode_AlterTrig ADC1 和 ADC2 工作在交替触发模式
     */ 
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;      // ADC_ScanConvMode 规定了模数转换工作在扫描模式（多通道）还是单次（单通道）模式。 ENABLE 或者 DISABLE
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // ADC_ContinuousConvMode 规定了模数转换工作在连续还是单次模式。    单次的话需要软件启动转换或者 外部触发转换                 ENABLE 或者 DISABLE
  //   ADC_SoftwareStartConvCmd(ADC1, ENABLE); 连续 模式开启一次后 就不停转换 ，单词模式 ，开启一次转换一次
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ADC_ExternalTrigConv 定义了使用外部触发来启动规则通道的模数转换，
      /*
	     ADC_ExternalTrigConv_T1_CC1 选择定时器 1 的捕获比较 1 作为转换外部触发
	  	 ADC_ExternalTrigConv_T1_CC2 选择定时器 1 的捕获比较 2 作为转换外部触发
	  	 ADC_ExternalTrigConv_T1_CC3 选择定时器 1 的捕获比较 3 作为转换外部触发
	  	 ADC_ExternalTrigConv_T2_CC2 选择定时器 2 的捕获比较 2 作为转换外部触发
	  	 ADC_ExternalTrigConv_T3_TRGO 选择定时器 3 的 TRGO 作为转换外部触发
	  	 ADC_ExternalTrigConv_T4_CC4 选择定时器 4 的捕获比较 4 作为转换外部触发
	  	 ADC_ExternalTrigConv_Ext_IT11 选择外部中断线 11 事件作为转换外部触发
	  	 ADC_ExternalTrigConv_None 转换由软件而不是外部触发启动
	  */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC_DataAlign 规定了 ADC 数据向左边对齐还是向右边对齐。
      /*
          ADC_DataAlign_Right ADC 数据右对齐
          ADC_DataAlign_Left ADC 数据左对齐	   
	  */
  ADC_InitStructure.ADC_NbrOfChannel = 1;  // ADC_NbreOfChannel 规定了顺序进行规则转换的 ADC 通道的数目。这个数目的取值范围是 1 到 16
  // 按 初始化结构体初始化 ADC1 
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
  // 为了能够正确地配置每一个 ADC 通道，用户在调用 ADC_Init()之后，必须调用 ADC_ChannelConfig() 来配置每个所使用通道的转换次序和采样时间 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_28Cycles5);
     /*
          通道 范围 ADC_Channel_0 ~ ADC_Channel_17 
          Rank：规则组采样顺序。取值范围 1 到 16  规定多个通道 每一次转换 的 转换次序
          采样时间  ：
		  		ADC_SampleTime_1Cycles5 采样时间为 1.5 周期 
		  		ADC_SampleTime_7Cycles5 采样时间为 7.5 周期
		  		ADC_SampleTime_13Cycles5 采样时间为 13.5 周期
		  		ADC_SampleTime_28Cycles5 采样时间为 28.5 周期
		  		ADC_SampleTime_41Cycles5 采样时间为 41.5 周期
		  		ADC_SampleTime_55Cycles5 采样时间为 55.5 周期
		  		ADC_SampleTime_71Cycles5 采样时间为 71.5 周期
		  		ADC_SampleTime_239Cycles5 采样时间为 239.5 周期
     */
  
      
  /* Enable ADC1 DMA */
  // 使能或者失能指定的 ADC 的 DMA 请求(可以不经过 CPU 进行数据传输)
  ADC_DMACmd(ADC1, ENABLE);
  
   // 使能或者失能指定的 ADC 的中断 ADC_ITConfig(ADC2, ADC_IT_EOC | ADC_IT_AWD, ENABLE); 




//   ADC2 的初始化  
  /* ADC2 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC2, &ADC_InitStructure);
  /* ADC2 regular channels configuration */ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_13, 1, ADC_SampleTime_28Cycles5);
  /* Enable ADC2 EOC interrupt */
  ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);


//   ADC3 的初始化  
  /* ADC3 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  
  ADC_Init(ADC3, &ADC_InitStructure);
  /* ADC3 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_28Cycles5);
  /* Enable ADC3 DMA */
  ADC_DMACmd(ADC3, ENABLE);

// ADC1 的校准 
  /* 使能 Enable ADC1 */
  //  函数 ADC_Cmd 只能在其他 ADC 设置函数之后被调用。 
  ADC_Cmd(ADC1, ENABLE);
  /* 重置校准 Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* 检查 重置校准寄存器 的状态*/
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* 开始校准 Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* 检查校准寄存器的状态 */
  while(ADC_GetCalibrationStatus(ADC1));

// ADC2 的校准 
  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);
  /* 重置校准 Enable ADC2 reset calibration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));
  /* 开始校准 Start ADC2 calibration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));
// ADC3 的校准 
  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);
  /* 重置校准  Enable ADC3 reset calibration register */   
  ADC_ResetCalibration(ADC3);
  /* Check the end of ADC3 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC3));
  /* 开始校准 Start ADC3 calibration */
  ADC_StartCalibration(ADC3);
  /* Check the end of ADC3 calibration */
  while(ADC_GetCalibrationStatus(ADC3));

// 启动转换 
  /* 使能 软件转换  ADC1 Software Conversion */ 
  // 使能或者失能指定的 ADC 的软件转换启动功能
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  /* Start ADC2 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC2, ENABLE);
  /* Start ADC3 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC3, ENABLE);
  
  // 获取 ADC 软件转换启动状态     FlagStatus Status;  Status = ADC_GetSoftwareStartConvStatus(ADC1); // while(ADC_GetSoftwareStartConvStatus(ADC1)); 
  // 对 ADC 规则组通道配置间断模式 ADC_DiscModeChannelCountConfig(ADC1, Number); Number：间断模式规则组通道计数器的值。这个值得范围为 1 到 8。 2
  // 使能或者失能指定的 ADC 规则组通道的间断模式 ADC_DiscModeCmd(ADC1, ENABLE);
  // 使能或者失能 ADCx 的经外部触发启动转换功能  ADC_ExternalTrigConvCmd(ADC1, ENABLE);
  // 返回最近一次 ADCx 规则组的转换结果          u16 DataValue;    DataValue = ADC_GetConversionValue(ADC1);      
  // 返回最近一次双 ADC 模式下的转换结果         u32 DataValue;    DataValue = ADC_GetDualModeConversionValue();
  // 使能或者失能指定 ADC 在规则组转化后自动开始注入组转换         ADC_AutoInjectedConvCmd(ADC2, ENABLE);
  // 使能或者失能指定 ADC 的注入组间断模式                         ADC_InjectedDiscModeCmd(ADC2, ENABLE);
  // 配置 ADCx 的外部触发启动注入组转换功能  ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigConv_T1_CC4);
       /*
	      ADC_ExternalTrigInjecConv_T1_TRGO 选择定时器 1 的 TRGO 作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_T1_CC4 选择定时器 1 的捕获比较 4 作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_T2_TRGO 选择定时器 2 的 TRGO 作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_T2_CC1 选择定时器 2 的捕获比较 1 作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_T3_CC4 选择定时器 3 的捕获比较 4 作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_T4_TRGO 选择定时器 4 的 TRGO 作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_Ext_IT15 选择外部中断线 15 事件作为注入转换外部触发
		  ADC_ExternalTrigInjecConv_None 注入转换由软件而不是外部触发启动
		  
	   */
  //  检查制定 ADC 标志位置 1 与否    FlagStatus Status;    Status = ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC);
       /*
				ADC_FLAG_AWD 模拟看门狗标志位
				ADC_FLAG_EOC 转换结束标志位
				ADC_FLAG_JEOC 注入组转换结束标志位
				ADC_FLAG_JSTRT 注入组转换开始标志位
				ADC_FLAG_STRT 规则组转换开始标志位	   
	   */
  //  清除 ADCx 的待处理标志位 	    ADC_ClearFlag(ADC2, ADC_FLAG_STRT); 
  //  检查指定的 ADC 中断是否发生   ITStatus Status;     Status = ADC_GetITStatus(ADC1, ADC_IT_AWD); 
  //  清除 ADCx 的中断待处理位      ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
  
  
  while (1)
  {
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* ADCCLK = PCLK2/4  4分频 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
    
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 and DMA2 clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);

  /* Enable ADC1, ADC2, ADC3 and GPIOC clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 |
                         RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PC.02, PC.03 and PC.04 (ADC Channel12, ADC Channel13 and 
     ADC Channel14) as analog inputs */
     // 设置   PC.02, PC.03 and PC.04 为模拟量输入端口 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief  Configures Vector Table base location.
  * @param  None
  * @retval None
  */
  //  interrupt contorl 中断控制器 
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 配置和使能ADC中断  ADC interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT
// 调试用 
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

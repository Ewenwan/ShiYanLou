#include "led.h"
#include "control.h"
//////////////////////////////////////////////////////////////////////////////////	 
/*
GPIO 模式
(1)GPIO_Mode_AIN         模拟输入 
(2)GPIO_Mode_IN_FLOATING 浮空输入
(3)GPIO_Mode_IPD         下拉输入
(4)GPIO_Mode_IPU         上拉输入
(5)GPIO_Mode_Out_OD      开漏输出
(6)GPIO_Mode_Out_PP      推挽输入
(7)GPIO_Mode_AF_OD       复用开漏输出
(8)GPIO_Mode_AF_PP       复用推荐输出
GPIO_Speed_10MHz         最高输出频率 10MHz 
GPIO_Speed_2MHz          最高输出频率 2MHz 
GPIO_Speed_50MHz         最高输出频率 50MHz
*/						  
////////////////////////////////////////////////////////////////////////////////// 	   
#include "usart.h"	

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

//__IO uint16_t ADC1ConvertedValue = 0;

// 系统运行指示灯 PB12
//初始化PB12 输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	// 系统电运行指示灯 PB12
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB 端口时钟		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				     //LED0-->PB.12 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.12
	 GPIO_ResetBits(GPIOB,GPIO_Pin_12);						         //PB.12 输出低电平
	
/*	
	// 电机正反转控制口  PA 9   要修改  在 TM3内设置
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA 端口时钟		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				       //motor_dir -->PA.9 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					         //根据设定参数初始化GPIOA.9
	 GPIO_ResetBits(GPIOA,GPIO_Pin_9);						           //PA.9 输出低电平
	*/
}

// ADC 电机母线电流采 端口配置   PA6

void motor_carrent_io_init(void)
 {

	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
	//端口时钟配置	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); // ADC1时钟使能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA口 时钟使能
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   // 开启 DMA1 的时钟
	 
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   // ADC采样时钟为 72MHz / 6  = 12MHz   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //设置PA6口为电流采样口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
  DMA_DeInit(DMA1_Channel1);
  //配置DMA传输方式
	ptForkLift->ADC1ConvertedValue=0;
  DMA_InitStructure.DMA_PeripheralBaseAddr= ADC1_DR_Address;           //外设基地址
  DMA_InitStructure.DMA_MemoryBaseAddr= (uint32_t)&(ptForkLift->ADC1ConvertedValue); //内存基地址
  DMA_InitStructure.DMA_DIR= DMA_DIR_PeripheralSRC; //DMA传输方向
  DMA_InitStructure.DMA_BufferSize = 1;             //传输单位
  DMA_InitStructure.DMA_PeripheralInc= DMA_PeripheralInc_Disable;//外设地址指针增量设为不自动增加
  DMA_InitStructure.DMA_MemoryInc= DMA_MemoryInc_Disable;        //内存地址指针增量设为不自动增加
  DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_HalfWord;//外设数据半字
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;       //内存数据大小设为半字
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;    //DMA传输方式设为循环传输
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA优先级设为高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;       //内存到内存的传输失能
  DMA_Init(DMA1_Channel1,&DMA_InitStructure);        //初始化DMA1，开启通道1
  DMA_Cmd(DMA1_Channel1,ENABLE);                     //使能 DMA 通道1

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ADC 工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;       // 单通道采样
  ADC_InitStructure.ADC_ContinuousConvMode= ENABLE;  // 连续采样
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 非外部触发
  ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;              // 数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;                            // 采样一个通道

  ADC_Init(ADC1,&ADC_InitStructure);
	
	//配置ADC1的通道6  PA6 转换次数为1   采样时间  为28.5个采样周期  ADC_SampleTime_55Cycles5
  ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_28Cycles5);
  ADC_DMACmd(ADC1,ENABLE);//使能DMA-ADC
  ADC_Cmd(ADC1,ENABLE);//使能ADC1
	
	ADC_ResetCalibration(ADC1);//复位校准寄存器
  while(ADC_GetResetCalibrationStatus(ADC1));/* 检查 重置校准寄存器 的状态*/
  ADC_StartCalibration(ADC1);//开始校准 Start ADC1 calibration 
  while(ADC_GetCalibrationStatus(ADC1)); // 检查校准寄存器的状态 
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);  // 使能ADC1 的软件转换功能
	
	//	ADC_Value = (float)(ptForkLift->ADC1ConvertedValue)/4096*3.3; 位电压值
		
 }


// 拨码盘 采样 PB0 PB1  PB10			
void CarID_Select(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
		
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟	
	
	 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	
	// GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	
//拨码盘接口设置 PB0 PB1  PB10
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10; // 使能PB0 PB1  PB10			
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 // 上拉输入

	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIO PB0 PB1  PB10 
	 // byte b=a   |  (b<<1)  | (c<<2)  三位  可判断8种情况
	 ptForkLift->u8CarID= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) | (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)<<1)| (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)<<2);
			
	 
}










 

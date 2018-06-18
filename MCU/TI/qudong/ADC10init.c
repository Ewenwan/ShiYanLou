
//P2.5口接DHT11数据线DATA
#include "msp430g2553.h"

void ADC10_init_mq7(void);
void ADC10_init_mq2(void);
void ADC10_init_mq135(void);

/******************MQ7 ADC10 初始化函数***************************/
void ADC10_init_mq7(void)     //ADC模数转换初始化函数段
{
	  P1SEL|= BIT0;           //P1.0设置为ADC功能A0
	  P1DIR &= ~BIT0;
	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_2 + INCH_0 + ADC10DIV_1;  //单通道转换； input A1模拟信号输入选择通道A1即p1.0
	  ADC10DTC1|=0x08;        //一共采样8次
	  ADC10AE0 |= BIT0;       // 打开A0的模拟输入通道
}

/******************MQ2 ADC10 初始化函数***************************/
void ADC10_init_mq2(void)    //ADC模数转换初始化函数段
{
	  P1SEL|= BIT1;          //P1.1设置为ADC功能A1
	  P1DIR &= ~BIT1;
	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_2 + INCH_1 + ADC10DIV_1;  //单通道转换； input A1模拟信号输入选择通道A1即p1.1
	  ADC10DTC1|=0x08;       //一共采样8次
	  ADC10AE0 |= BIT1;      // 打开A1的模拟输入通道
}

/******************MQ135 ADC10 初始化函数***************************/
void ADC10_init_mq135(void)  //ADC模数转换初始化函数段
{
	  P1SEL|= BIT2;          //P1.2设置为ADC功能A2
	  P1DIR &= ~BIT2;
	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_2 + INCH_2 + ADC10DIV_1;  //单通道转换； input A1模拟信号输入选择通道A2即p1.2
	  ADC10DTC1|=0x08;       //一共采样8次
	  ADC10AE0 |= BIT2;      // 打开A2的模拟输入通道
}


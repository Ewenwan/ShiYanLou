// 内部温度计 adc测量 均值滤波
#include <msp430g2553.h>   //头文件包含
long temp,sum;             //长字节型变量 temp
long IntDegC,AD[10];       //长字节型变量IntDegC 摄氏度
int ge,shi,sf,bf;
void ADC10_init(void);     //模数转换芯片ADC10初始化函数声明

void main()                //主函数开始
{
	ADC10_init();          //调用ADC10初始化函数
    while(1)               //死循环
    { int i,j;
    	for(i=0;i<10;i++){
      ADC10CTL0 |= ENC + ADC10SC;                    //控制字0 使能转换 +开始转换
      __bis_SR_register(CPUOFF + GIE);
      AD[i] = ADC10MEM;                               //ADC模数转换后的数据存储单元
    	}
    	for(j=0;j<10;j++)
    	{
    		sum =AD[0]+AD[1]+AD[2]+AD[3]+AD[4]+AD[5]+AD[6]+AD[7]+AD[8]+AD[9];
    	}
    	temp=sum/10;
     // IntDegC = (((temp - 673) * 423) / 1024);         //转换后的数字量变成摄氏温度
      shi = (int)((((temp - 673) * 423) / 1024)/10);
      ge = (int)((((temp - 673) * 423) / 1024)%10);
      sf = (int)(((temp - 673) * 423)*10 / 1024%10);
     // bf = ((temp - 673) * 423)*100 / 1024;
      //bf = ((temp - 673) * 423-IntDegC*1024)*100 / 1024 %10;
      __no_operation();                             // 设置断点
    }
}

void ADC10_init(void)                              //ADC模数转换初始化函数段
{
    ADC10CTL0 &= ~ENC;                             //控制字0（辅助）  关 使能转换
    ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;       //控制字0  中断使能+ADC10使能+参照使能+64倍ADC时钟+参照选择字节1
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10; //控制字1
    __delay_cycles(30000);
}
//ADC中断处理函数
#pragma vector=ADC10_VECTOR                        //
__interrupt void ADC10_ISR_HOOK(void)
{
	__bic_SR_register_on_exit(LPM4_bits);
}
////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//////////////////////////////////其二////////////////////////////////////////////
// 外部温度传感器 adc测量 
#include  <msp430G2553.h>
long temp;                                   //长字节型变量 temp
long IntDegCO;                                //长字节型变量IntDegCO 一氧化碳
int COg,COs,COb;
void ADC10_init(void);
void main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // 关闭看门狗
  ADC10_init();                             //调用ADC10初始化函数
  while(1)
  {

   ADC10CTL0 &= ~ENC;                      //关闭采样使能
   while(ADC10CTL1&BUSY);                   //检测AD是否繁忙
   ADC10CTL0 |= ENC + ADC10SC;               //控制字0 使能转换 +开始转换
   __bis_SR_register(CPUOFF + GIE);          // LPM0, ADC10_ISR will force exit
   temp = ADC10MEM;                          //ADC模数转换后的数据存储单元
   IntDegCO = ((temp/204.8) * 200);         //转换后的数字量变成 ppm

   COg=IntDegCO%10;
   COs=IntDegCO/10%10;
   COb=IntDegCO/100;
   __no_operation();                         // 设置断点
  }

}
void ADC10_init(void)                              //ADC模数转换初始化函数段
{
	  P1SEL|= BIT1;                                //P1.1设置为ADC功能A1
	  P1DIR &= ~BIT1;
	  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE+ MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_0+INCH_1;                    //单通道转换； input A1模拟信号输入选择通道A1即p1.1
	  ADC10DTC1|=0x08;                            //一共采样8次
	  ADC10AE0 |= BIT1;                           // 打开A1的模拟输入通道
}
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(LPM4_bits);;          // Enter active mode on exit
}


//////////////////////////////////////////////////////////////
////////////////////////////其三//////////////////////////////////////////////////////////
#include  <msp430G2553.h>
long D0,D1,D2;                                   //长字节型变量 temp

float COvol;                                //CO 电压
long COppm;
int COg,COs,COb;

float CH4vol;                                //CO 电压
long CH4ppm;
int CH4g,CH4s,CH4b;

//MQ135   p1.2
float Benvol;                               //Ben 电压
long Benppm;
int Beng,Bens,Benb;


void ADC10_init_mq7(void);
void ADC10_init_mq2(void);
void ADC10_init_mq135(void);

void main(void)
{

  WDTCTL = WDTPW+WDTHOLD;                   // 关闭看门狗
 // ADC10_init_mq7();                       //调用ADC10初始化函数

  while(1)
  {

   ADC10_init_mq135();
   ADC10CTL0 &= ~ENC;                      //关闭采样使能
   while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
   ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
  //  __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
   D0 = ADC10MEM;


   ADC10_init_mq2();                       //调用ADC10初始化函数
   ADC10CTL0 &= ~ENC;                      //关闭采样使能
   while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
   ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
  // __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
   D2 = ADC10MEM;
   //ADC模数转换后的数据存储单元
   __no_operation();

   ADC10_init_mq7();                       //调用ADC10初始化函数
   ADC10CTL0 &= ~ENC;                      //关闭采样使能
   while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
   ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
  // __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
   D1 = ADC10MEM;                         //ADC模数转换后的数据存储单元

   __no_operation();





   COvol =(D0/1024.0)*2.5;               //转换后的数字量变成 ppm
   COppm = (long)(COvol*200);              //0~5v 对应 0～1000ppm一氧化碳
   COg =  (int)COppm%10;
   COs =  (int)COppm/10%10;
   COb =  (int)COppm/100;

   CH4vol =(D1/1024.0)*2.5;               //转换后的数字量变成 ppm
   CH4ppm = (long)(CH4vol*200);              //0~5v 对应 0～1000ppm一氧化碳
   CH4g =  (int)CH4ppm%10;
   CH4s =  (int)CH4ppm/10%10;
   CH4b =  (int)CH4ppm/100;

   //MQ135
   Benvol = (D2/1024.0)*2.5;               //转换后的数字量变成 ppm
   Benppm = (long)(Benvol*200);              //0~5v 对应 0～1000ppm   Ben
   Beng =  (int)Benppm%10;
   Bens =  (int)Benppm/10%10;
   Benb =  (int)Benppm/100;


   __no_operation();                         // 设置断点
  }

}
void ADC10_init_mq7(void)                              //ADC模数转换初始化函数段
{
	  P1SEL|= BIT0;                                //P1.0设置为ADC功能A0
	  P1DIR &= ~BIT0;
	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_2 + INCH_0 + ADC10DIV_1;  //单通道转换； input A1模拟信号输入选择通道A1即p1.0
	  ADC10DTC1|=0x08;                            //一共采样8次
	  ADC10AE0 |= BIT0;                           // 打开A1的模拟输入通道
}

void ADC10_init_mq2(void)                              //ADC模数转换初始化函数段
{
	  P1SEL|= BIT1;                                //P1.1设置为ADC功能A1
	  P1DIR &= ~BIT1;
	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_2 + INCH_1 + ADC10DIV_1;  //单通道转换； input A1模拟信号输入选择通道A1即p1.1
	  ADC10DTC1|=0x08;                            //一共采样8次
	  ADC10AE0 |= BIT1;                           // 打开A1的模拟输入通道
}

void ADC10_init_mq135(void)                              //ADC模数转换初始化函数段
{
	  P1SEL|= BIT2;                                //P1.2设置为ADC功能A2
	  P1DIR &= ~BIT2;
	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_2 + INCH_2 + ADC10DIV_1;  //单通道转换； input A1模拟信号输入选择通道A2即p1.2
	  ADC10DTC1|=0x08;                            //一共采样8次
	  ADC10AE0 |= BIT2;                           // 打开A2的模拟输入通道
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{

  __bic_SR_register_on_exit(LPM4_bits);;          // Enter active mode on exit
}
/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////其三/////////////////////////////////////////
///    adc中断处理
#include  <msp430G2553.h>

unsigned long D0,D1,D2;                     //长字节型变量 temp
//MQ7     p1.0
float COvol;                                //CO 电压
long COppm;
int COg,COs,COb;
//MQ2     p1.1
float CH4vol;                               //CH4 电压
long CH4ppm;
int CH4g,CH4s,CH4b;
//MQ135   p1.2
float Benvol;                               //Ben 电压
long Benppm;
int Beng,Bens,Benb;


void ADC10_init(void);
void main(void)
{

  WDTCTL = WDTPW+WDTHOLD;                 // 关闭看门狗
  ADC10_init();                           //调用ADC10初始化函数

  while(1)
  {

   ADC10CTL0 &= ~ENC;                     //关闭采样使能
   while(ADC10CTL1&BUSY);                 //检测AD是否繁忙
   ADC10SA = 0x200;                       //ADC10数据传输起始地址
   ADC10CTL0 |= ENC + ADC10SC;            //控制字0 使能转换 +开始转换
  __bis_SR_register(CPUOFF + GIE);        // LPM0,ADC10_ISR 将迫使退出
   __no_operation();                      // 设置断点
  }

}
void ADC10_init(void)                     //ADC模数转换初始化函数段
{
	 // P1SEL|= BIT0 + BIT1 + BIT2;       //P1.0设置为ADC功能A0
	 // P1DIR &= ~BIT0 + BIT1 + BIT2;
	  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REF2_5V  +  REFON + ADC10ON + ADC10IE + MSC; // ADC10ON, interrupt enabled
	  ADC10CTL1 = CONSEQ_3 + INCH_0+ INCH_1 + INCH_2+ ADC10DIV_1;  //多通道转换； input A1模拟信号输入选择通道A1即p1.0
	  ADC10DTC1|=0x0a;                   //一共采样8次
	  ADC10AE0 |= 0X07;                  //P1.0 1.1 1.2 模拟转换使能
}

//ADC10中断服务程序
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{

  __bic_SR_register_on_exit(LPM4_bits);          // Enter active mode on exit

  D0=*(unsigned long *)0x200;    //A2通道的转换结果，注意此处的0x200要与ADC10SA的值一致           Ben 电压
  D1=*(unsigned int *)0x202;     //A1通道的转换结果,注意ADC10是先转换高通道再转换低通道                  CH4 电压
  D2=*(unsigned long *)0x204;    //A0                                       CO 电压
//MQ135
  Benvol = (D0/1024.0)*2.5;               //转换后的数字量变成 ppm
  Benppm = (long)(Benvol*200);              //0~5v 对应 0～1000ppm一氧化碳
  Beng =  (int)Benppm%10;
  Bens =  (int)Benppm/10%10;
  Benb =  (int)Benppm/100;
//MQ2
  CH4vol =(D1/1024.0)*2.5;               //转换后的数字量变成 ppm
  CH4ppm = (long)(CH4vol*200);              //0~5v 对应 0～1000ppm一氧化碳
  CH4g =  (int)CH4ppm%10;
  CH4s =  (int)CH4ppm/10%10;
  CH4b =  (int)CH4ppm/100;
//MQ7
  COvol =(D2/1024.0)*2.5;               //转换后的数字量变成 ppm
  COppm = (long)(COvol*200);              //0~5v 对应 0～1000ppm一氧化碳
  COg =  (int)COppm%10;
  COs =  (int)COppm/10%10;
  COb =  (int)COppm/100;
}

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// pm2.5传感器 自制pwm驱动 adc测量
/*
 Sharp pin 1 (V-LED)   => 5V (connected to 150ohm resister)
 Sharp pin 2 (LED-GND) => MSP430 GND pin
 Sharp pin 3 (LED)     => MSP430 pin 1.4
 Sharp pin 4 (S-GND)   => MSP430 GND pin
 Sharp pin 5 (Ao)      => MSP430 A0 pin 1.3
 Sharp pin 6 (Vcc)     => 5V
 */

 #include "msp430g2553.h"
 //P1.4口输出PWM波驱动LED灯
 #define PM_OUTPUT  P1DIR |= BIT6        //为1，输出
 #define PM_H       P1OUT |= BIT6        //高电平
 #define PM_L       P1OUT &=~ BIT6       //低电平

 #define CPU           (1000000)
 #define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
 #define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))

 void ADC10_init(void);
 long temp;                               //长字节型变量 temp
 float PMvol;                            //pm2.5 电压
 long PMppm;
 int PMg,PMs,PMb;


 void main(void)
   {
     WDTCTL = WDTPW + WDTHOLD;            //关狗
     ADC10_init();
     PM_OUTPUT;
     ADC10CTL0 &= ~ENC;                      //关闭采样使能
     while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
     ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
     __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit

     while(1)
      {
   // 	PM_L;
    	PM_H;
    	delay_us(280);
        temp  = ADC10MEM;                        //ADC模数转换后的数据存储单元
        PMvol = (temp/1024.0)*2.5;               //转换后的数字量变成 ppm
        PMppm = (long)(PMvol*200);              //0~5v 对应 0～1000ppm一氧化碳
        PMg   = (int)PMppm%10;
        PMs   = (int)PMppm/10%10;
        PMb   = (int)PMppm/100;
        delay_us(40);
     // PM_H;
        PM_L;
    	delay_us(9680);


         __no_operation();                         // 设置断点
      }
   }

 void ADC10_init(void)                              //ADC模数转换初始化函数段
   {
   	  P1SEL|= BIT3;                                 //P1.3 设置为ADC功能A3
   	  P1DIR &= ~BIT3;                               //p1.3 为输入
   	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
   	  ADC10CTL1 = CONSEQ_2 + INCH_3 + ADC10DIV_1;   //单通道转换； input A3模拟信号输入选择通道A2即p1.3
   	  ADC10DTC1|=0x04;                              //一共采样4次
   	  ADC10AE0 |= BIT3;                              // 打开A3的模拟输入通道
   }

   #pragma vector=ADC10_VECTOR
   __interrupt void ADC10_ISR (void)
   {

     __bic_SR_register_on_exit(LPM4_bits);;          // Enter active mode on exit
   }
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// 定时器pwm驱动 pm2.5传感器 adc测量
  #include "MSP430G2553.h"
  #include "TA_PWM.h"
  void ADC10_init(void);
  long temp;                                   //长字节型变量 temp
  float PMvol;                                //CO 电压
  long PMppm;
  int PMg,PMs,PMb;

  void main(void)
  {
      WDTCTL = WDTPW + WDTHOLD;            //关狗
      //-----初始化TA0为ACLK输入，不分频，通道1超前PWM，通道2关闭-----
      TA0_PWM_Init('A',1,'F',0);           //TA时钟设为ACLK,通道1超前PWM输出，通道2不作TA用。
      TA0_PWM_SetPeriod(10000);              //设定PWM周期       10000
      TA0_PWM_SetPermill(1,968);            //通道1,占空比  9.68/10=96.8%     320us低电平
      ADC10_init();                          //调用ADC10初始化函数

      while(1)
       {

        ADC10CTL0 &= ~ENC;                      //关闭采样使能
        while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
        ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
       // __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
        temp  = ADC10MEM;                        //ADC模数转换后的数据存储单元
        PMvol = (temp/1024.0)*2.5;               //转换后的数字量变成 ppm
        PMppm = (long)(PMvol*200);              //0~5v 对应 0～1000ppm一氧化碳
        PMg   = (int)PMppm%10;
        PMs   = (int)PMppm/10%10;
        PMb   = (int)PMppm/100;
        __no_operation();                         // 设置断点
       }

   }

  void ADC10_init(void)                              //ADC模数转换初始化函数段
  {
  	  P1SEL|= BIT3;                                //P1.3设置为ADC功能A3
  	  P1DIR &= ~BIT3;
  	  ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
  	  ADC10CTL1 = CONSEQ_2 + INCH_3 + ADC10DIV_1;  //单通道转换； input A3模拟信号输入选择通道A2即p1.3
  	  ADC10DTC1|=0x08;                            //一共采样8次
  	  ADC10AE0 |= BIT3;                           // 打开A3的模拟输入通道
  }

  #pragma vector=ADC10_VECTOR
  __interrupt void ADC10_ISR (void)
  {

    __bic_SR_register_on_exit(LPM4_bits);;          // Enter active mode on exit
  }
  ////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  
  

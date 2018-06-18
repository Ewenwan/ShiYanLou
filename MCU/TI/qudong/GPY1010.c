/*
 Sharp pin 1 (V-LED)   => 5V (通过150欧姆电阻接到5V电源上，通过220uF电解电容接到GND上)
 Sharp pin 2 (LED-GND) => MSP430 GND pin
 Sharp pin 3 (LED)     => MSP430 pin 1.6   输出PWM波通过10K电阻，再通过npn晶体管驱动传感器内部LED
 Sharp pin 4 (S-GND)   => MSP430 GND pin
 Sharp pin 5 (Ao)      => MSP430 A0 pin 1.6 采样传感器输出的模拟量端口，需通过分压电阻分压
 Sharp pin 6 (Vcc)     => 5V
 */

 #include "msp430g2553.h"
 #include "GPY1010.h"

 void GPY1010_start(void);
 //void get_pm(void);
 void Tran_VAL(unsigned int Hex_Val);
 void Tran_ppm( unsigned char *p);

 unsigned int PM2_5=0;                //PM2.5的数字量（电压）
 unsigned char PM[4]={0};                 //pm2.5的模拟量（电压）
 unsigned char ppm[3]={0};                //pm2.5的实际浓度
 unsigned int ppm_zero_vol=700;       //无粉尘时传感器输出电压（扩大1000倍） 默认0.7V 即 700


//*************************************
//PM2.5传感器GPY1010工作过程
//*************************************
void GPY1010_start(void)
  {
       // GPY1010_init();
	int i;
	for(i=0;i<10;i++)
	{
	PM_H;
	delay_us(320);
	PM_L;
	delay_us(9680);
	}
    PM_H;
    delay_us(272);
    //ADC10CTL0 &= ~ENC;          //关闭采样使能
    //while(ADC10CTL1&BUSY);      //检测AD是否繁忙
    ADC10CTL0 |= ENC + ADC10SC; //控制字0 使能转换 +开始转换
        //delay_us(10);        //等待采样完成   13个ADC10CLKs后转换完成
    while((ADC10CTL0 & ADC10IFG) == 1);//等待ADC10IFG标志变高（转换完成）
    //__bis_SR_register(CPUOFF + GIE); // LPM0低功耗模式, ADC10_ISR中断内 会退出低功耗模式
    //delay_us(10);
    PM2_5=ADC10MEM;           //ADC模数转换后的数据存储单元
    delay_us(40);
    PM_L;
    delay_us(9680);
   }
//*************************************
//采样10次求平均，结果保存在变量PM2_5内
//*************************************
//void get_pm(void)
//  {
	//int m;
     //for(m=0;m<10;m++)
//     GPY1010_start();
//     PM2_5 = PM2_5;
//   }
/************************************************
  * 将16进制数字量转换为10进制模拟电压量，结果保存在数组PM内
  * PM[0]~PM[3]分别为模拟电压的，个位，十分位，百分位，千分位
*************************************************/
void Tran_VAL(unsigned int Hex_Val)
   {
   unsigned long temp1,temp2;
   unsigned int Volt,t1,t2;
   temp1 = Hex_Val;           //保护数据
   temp1 = (temp1<<4) + Hex_Val +(Hex_Val<<3);    //temp1 = Hex_Val*25  AD采样参考电压2.5v，放大1000倍，
   temp2 = temp1;             //保护数据                                              //防止出现浮点数
   temp1 = ((temp1<<3) + (temp2<<1))*10;          //temp1 = temp1*100
   Volt = temp1>>12;          //Volt=temp1/1024
   PM[0] = Volt/1000;         //Hex转十进制，千位   个位
   t1 = Volt - (PM[0]*1000);
   PM[1] = t1/100;            //百位     十分位
   t2 = t1 - (PM[0]*1000)-(PM[1]*100);
   PM[2] = t2/10;             //十位     百分位
   PM[3] = t2-PM[2]*10;       //个位     千分位
    }
/*************************************
 * 将电压量根据GPY1010传感器灵敏度曲线转化为浓度值，结果保存在数组ppm内
 * p[0]~p[2] 分别为 粉尘浓度值的 十分位，百分位，千分位
 * 注，此电压量是传感器输出经过电阻分压得出，实际输出电压，应以分压比确定
*************************************/
void Tran_ppm( unsigned char *p)
{
 unsigned long temp2,temp3;
 unsigned int  a1;
 temp2 = 500*((p[0]*1000 + p[1]*100 + p[2]*10 + p[3])*2-ppm_zero_vol);//最高灵敏度0.5mg/m3
 temp3 = 3500 - ppm_zero_vol;                     //0.5mg/m3时为3.5v
 a1= temp2/temp3;
 ppm[0] = a1/100;                                 //百位      十分位
 ppm[1] = (a1 - ppm[0]*100)/10;                   //十位       百分位
 ppm[2] = a1 - ppm[0]*100 - ppm[1]*10;            //个位       千分位
 }
 
//   #pragma vector=ADC10_VECTOR
//   __interrupt void ADC10_ISR (void)
//   {
//    __bic_SR_register_on_exit(LPM4_bits);;      // Enter active mode on exit
//   }

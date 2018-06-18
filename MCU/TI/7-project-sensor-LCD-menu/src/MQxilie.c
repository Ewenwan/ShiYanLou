/*
 * p1.0  接mq7   模拟输入
 * p1.1  接 mq2   模拟输入
 * p1.2  接mq135 模拟输入
 */


#include  <msp430G2553.h>
#include  <ADC10init.h>
#include  <LCD12864.h>


void MQ1_adc10_init(void);
void MQ2_adc10_init(void);
void PM_adc10_init(void);
void MQ1_Vol_Con(void);
void MQ2_Vol_Con(void);
void PM_Vol_Con(void);
void MQ1_printf(void);
void MQ2_printf(void);
void PM_printf(void);

#define PM_OUTPUT  P1DIR |= BIT4        //为1，输出
#define PM_H       P1OUT |= BIT4        //高电平
#define PM_L       P1OUT &=~ BIT4       //低电平

#define CPU           (1000000)
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))


unsigned long D0,D1,D2,D3,D4;                     //长字节型变量 temp
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

float jqvol;                               //甲醛  电压
long jqppm;
int jqg,jqs,jqb;


float PMvol;                               //pm2.5电压
long PMppm;
int PMg,PMs,PMb;

void MQ1_adc10_init(void)
{
	   ADC10_init_mq7();
	   ADC10CTL0 &= ~ENC;                      //关闭采样使能
	   while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
	   ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
	   // __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
	    D0 = ADC10MEM;


    ADC10_init_mq2();
    ADC10CTL0 &= ~ENC;                      //关闭采样使能
    while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
    ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
    // __bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
     D1 = ADC10MEM;   //ADC模数转换后的数据存储单元
}


void MQ2_adc10_init(void)
{
 ADC10_init_mq135();
 ADC10CTL0 &= ~ENC;                      //关闭采样使能
 while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
 ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
 //__bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
  D2 = ADC10MEM;

  ADC10_init_VCO();
   ADC10CTL0 &= ~ENC;                      //关闭采样使能
   while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
   ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
   //__bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
    D3 = ADC10MEM;
}

void PM_adc10_init(void)
{
 ADC10_init_PM();
 PM_OUTPUT;
 ADC10CTL0 &= ~ENC;                      //关闭采样使能
 while(ADC10CTL1&BUSY);                  //检测AD是否繁忙
 ADC10CTL0 |= ENC + ADC10SC;             //控制字0 使能转换 +开始转换
 //__bis_SR_register(CPUOFF + GIE);      // LPM0, ADC10_ISR will force exit
 PM_L;
 delay_us(280);
 D4 = ADC10MEM;
 delay_us(40);
 PM_H;
 delay_us(9680);
}

void MQ1_Vol_Con(void)
{
    //MQ7
     COvol =(D0/1024.0)*2.5;               //转换后的数字量变成 ppm
     COppm = (long)(COvol*200)-55;              //0~5v 对应 0～1000ppm一氧化碳
     COg =  (int)COppm%10;
     COs =  (int)COppm/10%10;
     COb =  (int)COppm/100;
   //MQ2
     CH4vol =(D1/1024.0)*2.5;               //转换后的数字量变成 ppm
     CH4ppm = (long)(CH4vol*200)-125;              //0~5v 对应 0～1000ppm CH4
     CH4g =  (int)CH4ppm%10;
     CH4s =  (int)CH4ppm/10%10;
     CH4b =  (int)CH4ppm/100;
     __no_operation();                      // 设置断点
}

void MQ2_Vol_Con(void)
{
    //MQ135
     Benvol = (D2/1024.0)*2.5;               //转换后的数字量变成 ppm
     Benppm = (long)(Benvol*200)-280;              //0~5v 对应 0～1000ppm   Ben
     Beng =  (int)Benppm%10;
     Bens =  (int)Benppm/10%10;
     Benb =  (int)Benppm/100;
     //甲醛
     jqvol = (D3/1024.0)*2.5;               //转换后的数字量变成 ppm
     jqppm = (long)(jqvol*200)-70;              //0~5v 对应 0～1000ppm   Ben
     jqg =  (int)jqppm%10;
     jqs =  (int)jqppm/10%10;
     jqb =  (int)jqppm/100;

   __no_operation();                      // 设置断点

  }

void PM_Vol_Con(void)
{
    //MQ135
     PMvol = (D4/1024.0)*2.5;               //转换后的数字量变成 ppm
     PMppm = (long)(PMvol*200)+34;              //0~5v 对应 0～1000ppm   Ben
     PMg =  (int)PMppm%10;
     PMs =  (int)PMppm/10%10;
     PMb =  (int)PMppm/100;
   __no_operation();                      // 设置断点

  }


void MQ1_printf(void)                    //CO&ch4
{
	write_com(0x83);                //显示位置，第一行，第四列
	write_figurer(COb);             //显示CO数据     /ppm
	write_figurer(COs);
	write_figurer(COg);

	write_com(0x93);                //显示位置，第二行，第四列
	write_figurer(CH4b);            //显示CH4数据     /ppm
	write_figurer(CH4s);
	write_figurer(CH4g);

}

void MQ2_printf(void)               //甲醛&ben
{
	write_com(0x83);                //显示位置，第一行，第四列
	write_figurer(jqb);             //显示CO数据     /ppm
	write_figurer(jqs);
	write_figurer(jqg);

    write_com(0x93);                 //显示位置，第三行，第四列
    write_figurer(Benb);             //显示Ben数据     /ppm
    write_figurer(Bens);
    write_figurer(Beng);
}

void PM_printf(void)               //甲醛&ben
{
	write_com(0x83);                //显示位置，第一行，第四列
	write_figurer(PMb);             //显示CO数据     /ppm
	write_figurer(PMs);
	write_figurer(PMg);

}



//ADC10中断服务程序
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{

  __bic_SR_register_on_exit(LPM4_bits);          // Enter active mode on exit

}

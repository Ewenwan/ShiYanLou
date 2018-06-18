/***********************************************
 * auther：万有文，上海海事大学 date：2016.3.13
 * 版权所有，引用请保留此项！
 *****温湿度传感器DHT11模块硬件连接方式*************
 * P2.5口接数据线DATA，为输入，普通数字I/O
 ****WIFI模块硬件连接方式***********************
 * P1.4接CSN 为输出，P1.5接GDO0 为输入，P2.0接SCLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能。
 *****PM2.5传感器硬件连接方式********************
 * 数字量传感器接P1.1 单片机RX端（2400bps）
 *********************************************/
#include "msp430g2553.h"
#include "CC1101.h"
#include "DHT11.h"
//#include "GPY1010.h"

//INT8U table[16]="  上海海事大学  ";
INT8U leng=8;                    //发送数据包长度，16字节
INT8U table[8]={0};              //存储发送数据变量的数组
int t=0,period=20;               //t 定时器内数据发送周期循环变量，period发送数据周期  50ms*20 约1s
int sendflag=0;

INT8U senser[7]={0},sum=0,temp=0; //PM2.5传感器数据存储
int i=0,j,cormatflag=0;  //pM2.5串口接收处理变量
float Vo=0.0;
INT16U PPm;
INT8U ppm[3]={0};                //pm2.5的实际浓度
//senser[0]:起始位 0xAA  senser[1]:Vout(H) senser[2]:Vout(L)
//senser[3]:Vref(H)   senser[4]:Vref(L)  senser[5]:校验位   senser[6]:结束位 0xFF
//V(OUT)=(Vout(H)*256+Vout(L))/1025*5
//粉尘浓度 Ud=A*V(out)    A = 0.2~0.5 V/ 0.1mg/m3  典型 0.35

/************************串口发送单个数字函数****************************/
void Uart0Send(INT8U n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}

void senddata(INT8U dat)
{
    if(dat==170)              //判断起始位  0xAA，（0xAA==170）
	{
      i=0;                    //清零指针
      senser[i]=dat;          //等待匹配到第一个字符
	}
    else
	{
    	i++;
		senser[i]=dat;
		if((i==6)&(senser[6]==255))     //判断结束位  0xff，（0xff==255）
		{
			sum= senser[1]+senser[2]+senser[3]+senser[4];  //sum=Vout(H)+Vout(L)+Vref(H)+Vref(L)
			if(sum == senser[5])   //sum与校验位相同  &&  最后一位是0xFF结束位
			{
				for(j=0;j<7;j++)
				{Uart0Send(senser[j]);
				}
				Vo=(senser[1]*256+senser[2])/1024.0*5.0;  //接收到的数据按公式计算后得到Vo的值：Vo=(Vout(H)*256+Vout(L))/1024*5
				PPm=Vo*0.3*1000;
				ppm[0]=PPm/100;
				ppm[1]=(PPm-ppm[0]*100)/10;
				ppm[2]=PPm-ppm[0]*100-ppm[1]*10;
				cormatflag=1;
			}
		}
     }
}


void main(void)

{
    //初使化系统（时钟等）
    WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
    //在一个PUC信号之后，MCLK和SMCLK开始/从DCO（大约在1.1MHz），ACLK从LFXT1CLK（内部电容为6pF）获取时钟
    //将DCO校准至精准的1MHz
    DCOCTL=0;
    BCSCTL1 = CALBC1_1MHZ;   //设定范围1MHZ
    DCOCTL = CALDCO_1MHZ;    //设定DCO模式
    //配置主时钟
    BCSCTL2 &= ~(SELM1+SELM0); //选择dco为主系统（CPU）时钟    bit7：6 00
    BCSCTL2 &= ~(DIVM0+DIVM1); //选择dco为主时钟   //不分频         bit5：4 00
    //配置子系统时钟
    BCSCTL2 &= ~SELS;          //选择DCO为子系统时钟   bit3 0
    BCSCTL2 &= ~(DIVS_3);      //不分频                                     bit2：1 00 不分频
    //LED（发送指示灯）端口设置
    LED1OUT;       //发送指示灯
    LED1_SEL;     //普通数字I/O
    LED1_0 ;
    P1DIR |= BIT6;   //LED2为PM2.5接收指示灯
    P1SEL &= ~BIT6;
    P1OUT &= ~BIT6;
/*************SPI状态初始化 wifi模块I/O口初始化**********/
/*P1.4接GDO0 为输入，P1.5接CSN 为输出，P2.0接CLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能*/
    GDO0_INPUT;
    CSN_OUTPUT;
    SCK_OUTPUT;
    MOSI_OUTPUT;
    MISO_INPUT;
    GDO0_SEL;     //普通数字I/O
    CSN_SEL;     //普通数字I/O
    SCKMOMI_SEL;   //普通数字I/O
    SCK_0;MOSI_0;
    CSN_1;         //关闭
/*************DHT11_port_init********************/
    DHT11_SEL;                     //普通数字I/O 端口
    DHT11_OUTPUT;                   //输出
   // DHT11_H;
//定时器初始化
    TACCR0 = 50000;                     //计到50000，约50ms
    TACTL = TASSEL_2+ID_0+MC_1+TACLR;  //设置TIMERA的时钟源为SMCLK(1M),不分频,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                   //打开比较模块0中断
//ADC10初始化
/*
    P1SEL|= BIT7;                    //P1.7 设置为ADC功能
 	P1DIR &= ~BIT7;                  //p1.7 为输入
 	ADC10CTL0 = SREF_1 + ADC10SHT_1 + REF2_5V  +  REFON + ADC10ON + MSC; // ADC10ON, interrupt enabled
 	ADC10CTL1 = CONSEQ_2 + INCH_7 + ADC10DIV_1;   //单通道多次采样； input A6模拟信号输入选择通道,即p1.7
 	ADC10DTC1|=0x04;                 //一共采样4次
 	ADC10AE0 |= BIT7;                // 打开A3的模拟输入通道
*/
//串口初始化（数字量PM2.5传感器接口，单片机RX 仅用于接收 传感器2400bps）
 	  UCA0CTL1 |= UCSWRST;           // USCI_A0 进入软件复位状态
 	  UCA0CTL1 |= UCSSEL_2;          //时钟源选择 SMCLK   1MHz
 	  P1SEL = BIT1 + BIT2 ;          // P1.1 = RXD, P1.2=TXD
 	  P1SEL2 = BIT1 + BIT2 ;         // P1.1 = RXD, P1.2=TXD
 	  UCA0BR0 = 0xA0;                //时钟源 1MHz 1M/9600=104=0X0068
 	  UCA0BR1 = 0x01;                //时钟源 1MHz   1M/2400=416=OX01A0
 	  UCA0MCTL = UCBRS0;             //小数分频器
 	  UCA0CTL1 &= ~UCSWRST;          //初始化 USCI_A0 状态机
 	  IE2 |= UCA0RXIE;               //使能 USCI_A0 接收中断
    Init_CC1101();    //wifi模块（CC1101）初始化

    _enable_interrupts();        //使能全局中断
     while(1)
     {
    	 if(sendflag)
    	 {
    	 if(start_DHT11());
    	 /*
         GPY1010_start();
    	Tran_VAL(PM2_5);
    	Tran_ppm(PM);
    	*/
    	 if(cormatflag==1){
    			/*Vo=(senser[1]*256+senser[2])/1024*5;  //接收到的数据按公式计算后得到Vo的值：Vo=(Vout(H)*256+Vout(L))/1024*5
                PPm=Vo*0.3*1000;
                ppm[0]=PPm%100;
                ppm[1]=(PPm-ppm[0]*100)%10;
                ppm[2]=PPm-ppm[0]*100-ppm[1]*10;
                */
                P1OUT ^= BIT6;
            cormatflag=0;
    	 }
    	table[0]= ppm[0]+48;          //PM2.5浓度十分位 ASCII码
        table[1]= ppm[1]+48;          //PM2.5浓度百分位 ASCII码
    	table[2]= ppm[2]+48;          //PM2.5浓度千分位 ASCII码
        table[3]= Tshi+48;            //温度十位 ASCII码
        table[4]= Tge+48;             //温度个位 ASCII码
    	table[5]= RHshi+48;           //湿度十位 ASCII码
        table[6]= RHge+48;            //湿度十位 ASCII码
    	 //table[0]=
    	 // table[0]=
    	// CC1101_TxData(0x01,table,leng);//0x01地址（用于接收时地址校验）；无线数传
    	halRfSendPacket(table,leng,0x01);
    	 sendflag=0;
    	 }
     }

  }


 //--------定时器中断函数------------
 #pragma vector=TIMER0_A0_VECTOR
 __interrupt void Timer_A(void)     //CCIFG中断被响应后，该标志位自动清零
 {
   t++;
   if(t>=(period+1)) t=1;      //因定时器定时时间较短，需要通过二级循环来改变发送数据的周期
   if(t==period)
	 {
	  sendflag=1;
	  LED1_0_1;
	}

 }


#pragma vector = USCIAB0RX_VECTOR                      //接收中断
__interrupt void USCI0RX_ISR(void)
{
	temp = UCA0RXBUF;               //保存接收到的数据
	 senddata(temp);
}


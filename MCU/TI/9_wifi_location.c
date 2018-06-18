/***********************************************
 * auther：万有文，上海海事大学 date：2016.3.13
 * 版权所有，引用请保留此项！
 ****WIFI模块硬件连接方式***********************
 * P1.4接CSN 为输出，P1.5接GDO0 为输入，P2.0接SCLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能。
 *********************************************/
#include "msp430g2553.h"
#include "CC1101.h"
#include "math.h"
//#include "DHT11.h"
//#include "GPY1010.h"

//INT8U table[16]="  上海海事大学  ";
INT8U WifiRsize=5;
INT8U WifiRx[5]={0};             //存储接收数据变量的数组
INT8U RSSI[2]={0};               //存储信号强度的数组

INT8U WifiTsize=8;               //发送数据包长度，16字节
INT8U WifiTx[8]={0};             //存储发送数据变量的数组

int t=0,period=20;               //t 定时器内数据发送周期循环变量，period发送数据周期  50ms*20 约1s
int sendflag=0;
INT16U RSSI_offset=74;
uclong adj_dbm=138.73;
/************************串口发送单个数字函数****************************/
void Uart0Send(INT8U n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}
 INT8U CalculateRssi(INT8U RSSI_dec)
 {
	 INT8U temp;
	 if(RSSI_dec>=128)
	 {
		temp= (RSSI_dec-256)/2-RSSI_offset;
	 }
	 else
	 {
	 	temp= RSSI_dec/2-RSSI_offset;
	 }
	 return temp;
 }

 double Get_diatance(INT8U RSSI_dbm)
 {
	 double lgd;
	 lgd=-RSSI_dbm-adj_dbm;
	 return lgd;//pow(10,lgd);
 }


 void get_dis2(INT8U *dis)
 {
	 double juli;
	 juli=Get_diatance(CalculateRssi(RSSI[0]));
	 dis[0]=juli*10000/100;                      //距离十位
	 dis[1]=(juli-dis[0]*100)/10;                //距离个位
	 dis[2]=juli-dis[0]*100-dis[1]*10;           //距离小数位
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

//定时器初始化
    TACCR0 = 50000;                     //计到50000，约50ms
    TACTL = TASSEL_2+ID_0+MC_1+TACLR;   //设置TIMERA的时钟源为SMCLK(1M),不分频,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                    //打开比较模块0中断

//串口初始化（数字量PM2.5传感器接口，单片机RX 仅用于接收 传感器2400bps）
 	  UCA0CTL1 |= UCSWRST;           // USCI_A0 进入软件复位状态
 	  UCA0CTL1 |= UCSSEL_2;          //时钟源选择 SMCLK   1MHz
 	  P1SEL = BIT1 + BIT2 ;          // P1.1 = RXD, P1.2=TXD
 	  P1SEL2 = BIT1 + BIT2 ;         // P1.1 = RXD, P1.2=TXD
 	  UCA0BR0 = 0xA0;                //时钟源 1MHz 1M/9600=104=0X0068
 	  UCA0BR1 = 0x01;                //时钟源 1MHz   1M/2400=416=OX01A0
 	  UCA0MCTL = UCBRS0;             //小数分频器
 	  UCA0CTL1 &= ~UCSWRST;          //初始化 USCI_A0 状态机
 	 // IE2 |= UCA0RXIE;               //使能 USCI_A0 接收中断
    Init_CC1101();    //wifi模块（CC1101）初始化

    _enable_interrupts();        //使能全局中断
     while(1)
     {
    	 if(halRfReceivePacket(WifiRx,&WifiRsize,RSSI));
    	 {
    		 int i;
    		 get_dis2(WifiTx);
    		 for(i=2;i>=0;i--)
    		 {WifiTx[i+2]=WifiTx[i];}
    		  WifiTx[0]=0x6f;                           //o,o 原点位置WIFI信号接收从机
    		  WifiTx[1]=0x6f;
    		 halRfSendPacket(WifiTx,8, 0x04);

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

////////////////////////////////////////////////////////////////////////
/////////////////////////////////节点2//////////////////////////////////////
/***********************************************
 * auther：万有文，上海海事大学 date：2016.3.13
 * 版权所有，引用请保留此项！
 ****WIFI模块硬件连接方式***********************
 * P1.4接CSN 为输出，P1.5接GDO0 为输入，P2.0接SCLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能。
 *********************************************/
#include "msp430g2553.h"
#include "CC1101.h"
#include "math.h"
//#include "DHT11.h"
//#include "GPY1010.h"

//INT8U table[16]="  上海海事大学  ";
INT8U WifiRsize=5;
INT8U WifiRx[5]={0};             //存储接收数据变量的数组
INT8U RSSI[2]={0};               //存储信号强度的数组

INT8U WifiTsize=8;               //发送数据包长度，16字节
INT8U WifiTx[8]={0};             //存储发送数据变量的数组

int t=0,period=20;               //t 定时器内数据发送周期循环变量，period发送数据周期  50ms*20 约1s
int sendflag=0;
INT16U RSSI_offset=74;
uclong adj_dbm=138.73;
/************************串口发送单个数字函数****************************/
void Uart0Send(INT8U n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}
 INT8U CalculateRssi(INT8U RSSI_dec)
 {
	 INT8U temp;
	 if(RSSI_dec>=128)
	 {
		temp= (RSSI_dec-256)/2-RSSI_offset;
	 }
	 else
	 {
	 	temp= RSSI_dec/2-RSSI_offset;
	 }
	 return temp;
 }

 double Get_diatance(INT8U RSSI_dbm)
 {
	 double lgd;
	 double X=10;
	 lgd=-RSSI_dbm-adj_dbm;
	 return pow(X,lgd);
 }


 void get_dis2(INT8U *dis)
 {
	 double juli;
	 juli=Get_diatance(CalculateRssi(RSSI[0]));
	 dis[0]=juli*10000/100;                      //距离十位
	 dis[1]=(juli-dis[0]*100)/10;                //距离个位
	 dis[2]=juli-dis[0]*100-dis[1]*10;           //距离小数位
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

//定时器初始化
    TACCR0 = 50000;                     //计到50000，约50ms
    TACTL = TASSEL_2+ID_0+MC_1+TACLR;   //设置TIMERA的时钟源为SMCLK(1M),不分频,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                    //打开比较模块0中断

//串口初始化（数字量PM2.5传感器接口，单片机RX 仅用于接收 传感器2400bps）
 	  UCA0CTL1 |= UCSWRST;           // USCI_A0 进入软件复位状态
 	  UCA0CTL1 |= UCSSEL_2;          //时钟源选择 SMCLK   1MHz
 	  P1SEL = BIT1 + BIT2 ;          // P1.1 = RXD, P1.2=TXD
 	  P1SEL2 = BIT1 + BIT2 ;         // P1.1 = RXD, P1.2=TXD
 	  UCA0BR0 = 0xA0;                //时钟源 1MHz 1M/9600=104=0X0068
 	  UCA0BR1 = 0x01;                //时钟源 1MHz   1M/2400=416=OX01A0
 	  UCA0MCTL = UCBRS0;             //小数分频器
 	  UCA0CTL1 &= ~UCSWRST;          //初始化 USCI_A0 状态机
 	 // IE2 |= UCA0RXIE;               //使能 USCI_A0 接收中断
    Init_CC1101();    //wifi模块（CC1101）初始化

    _enable_interrupts();        //使能全局中断
     while(1)
     {
    	 if(halRfReceivePacket(WifiRx,&WifiRsize,RSSI));
    	 {
    		 int i;
    		 get_dis2(WifiTx);
    		 for(i=2;i>=0;i--)
    		 {WifiTx[i+2]=WifiTx[i];}
    		  WifiTx[0]=0x6f;                           //o,o 原点位置WIFI信号接收从机
    		  WifiTx[1]=0x6f;
    		 halRfSendPacket(WifiTx,8, 0x04);

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

////////////////////////////////////////////////////////////////////
/////////////////////////////////节点2/////////////////////////////

/***********************************************
 * auther：万有文，上海海事大学 date：2016.3.13
 * 版权所有，引用请保留此项！
 ****WIFI模块硬件连接方式***********************
 * P1.4接CSN 为输出，P1.5接GDO0 为输入，P2.0接SCLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能。
 *********************************************/
#include "msp430g2553.h"
#include "CC1101.h"
#include "math.h"
//#include "DHT11.h"
//#include "GPY1010.h"

//INT8U table[16]="  上海海事大学  ";
INT8U WifiRsize=5;
INT8U WifiRx[5]={0};             //存储接收数据变量的数组
INT8U RSSI[2]={0};               //存储信号强度的数组

INT8U WifiTsize=8;               //发送数据包长度，16字节
INT8U WifiTx[8]={0};             //存储发送数据变量的数组

int t=0,period=20;               //t 定时器内数据发送周期循环变量，period发送数据周期  50ms*20 约1s
int sendflag=0;
INT16U RSSI_offset=74;
uclong adj_dbm=138.73;
/************************串口发送单个数字函数****************************/
void Uart0Send(INT8U n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}
 INT8U CalculateRssi(INT8U RSSI_dec)
 {
	 INT8U temp;
	 if(RSSI_dec>=128)
	 {
		temp= (RSSI_dec-256)/2-RSSI_offset;
	 }
	 else
	 {
	 	temp= RSSI_dec/2-RSSI_offset;
	 }
	 return temp;
 }

 double Get_diatance(INT8U RSSI_dbm)
 {
	 double lgd;
	 lgd=-RSSI_dbm-adj_dbm;
	 return pow(10,lgd);
 }


 void get_dis2(INT8U *dis)
 {
	 double juli;
	 juli=Get_diatance(CalculateRssi(RSSI[0]));
	 dis[0]=juli*10000/100;                      //距离十位
	 dis[1]=(juli-dis[0]*100)/10;                //距离个位
	 dis[2]=juli-dis[0]*100-dis[1]*10;           //距离小数位
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

//定时器初始化
    TACCR0 = 50000;                     //计到50000，约50ms
    TACTL = TASSEL_2+ID_0+MC_1+TACLR;   //设置TIMERA的时钟源为SMCLK(1M),不分频,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                    //打开比较模块0中断

//串口初始化（数字量PM2.5传感器接口，单片机RX 仅用于接收 传感器2400bps）
 	  UCA0CTL1 |= UCSWRST;           // USCI_A0 进入软件复位状态
 	  UCA0CTL1 |= UCSSEL_2;          //时钟源选择 SMCLK   1MHz
 	  P1SEL = BIT1 + BIT2 ;          // P1.1 = RXD, P1.2=TXD
 	  P1SEL2 = BIT1 + BIT2 ;         // P1.1 = RXD, P1.2=TXD
 	  UCA0BR0 = 0xA0;                //时钟源 1MHz 1M/9600=104=0X0068
 	  UCA0BR1 = 0x01;                //时钟源 1MHz   1M/2400=416=OX01A0
 	  UCA0MCTL = UCBRS0;             //小数分频器
 	  UCA0CTL1 &= ~UCSWRST;          //初始化 USCI_A0 状态机
 	 // IE2 |= UCA0RXIE;               //使能 USCI_A0 接收中断
    Init_CC1101();    //wifi模块（CC1101）初始化

    _enable_interrupts();        //使能全局中断
     while(1)
     {
    	 if(halRfReceivePacket(WifiRx,&WifiRsize,RSSI));
    	 {
    		 int i;
    		 get_dis2(WifiTx);
    		 for(i=2;i>=0;i--)
    		 {WifiTx[i+2]=WifiTx[i];}
    		  WifiTx[0]=0x6f;                           //o,o 原点位置WIFI信号接收从机
    		  WifiTx[1]=0x6f;
    		 halRfSendPacket(WifiTx,8, 0x04);

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

//////////////////////////////////////////////////////
////////////////////////////////主控接收信号 三圆定位 显示
/***********************************************
 * auther：万有文，上海海事大学 date：2016.3.13
 * 版权所有，引用请保留此项！
 *****温湿度传感器DHT11模块硬件连接方式*************
 * P2.5口接数据线DATA，为输入，普通数字I/O
 ****WIFI模块硬件连接方式***********************
 * P1.4接CSN 为输出，P1.5接GDO0 为输入，P2.0接SCLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能。
 ****lcd12864硬件连接方式****************************
 * 串行两线MCU控制LCD
 * 用到SID（数据线）P2.3  CLK（时钟线）P2.4
 * PSB默认低电平（串行）
 * CS片选信号默认高电平（使能LCD）
 * REST复位信号默认高电平或悬空（不复位LCD）
 *********************************************/
#include "msp430g2553.h"
#include "CC1101.h"
#include "DHT11.h"
#include "LCD12864.h"
//#include "circle.c"

#define LED2_OUT P1DIR |= BIT6;   //LED2为报警指示灯
#define LED2_SEL P1SEL &= ~BIT6;
#define LED2_0  P1OUT &= ~BIT6;
#define LED2_1  P1OUT &= ~BIT6;
#define LED2_0_1  P1OUT ^= BIT6;

uchar hang1[]={"上海海事大学"};
uchar hang2[]={"物流工程学院"};
uchar hang3[]={"测控技术仪器"};
uchar hang4[]={"ZY室内定位仪"};
uint period=5,t=0,showflag=0;    //数据采样刷新时间计数5*50ms=0.25s

INT8U leng=8;                    // 16字节, 如果需要更长的数据包,请正确设置
INT8U RxBuf[8];                  //接收wifi数据缓存
INT8U temp[8]={48};              //保存数据的变量数组
int xxrec_flg=0,xyrec_flg=0,yyrec_flg=0,wsdrec_flg=0;


INT8U Txbuff[8];
//t 定时器内数据发送周期循环变量，period发送数据周期  50ms*20 约1s    一个周期发四次
int sendflag=0;                  //向PC上位机发送标志

INT16U distance0y[3]={0},distancexy[3]={0},distancex0[3]={0};
INT16U Dimension_l=10,Dimension_w=5;
INT16U location_x[3]={0},location_y[3]={0};
INT16U wuti_wsd[4]={0};


/************************串口发送单个数字函数****************************/
void Uart0Send(INT8U n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}

 void date_send_update(void)
 {
	 if(start_DHT11());      //测温湿度
	 Txbuff[0]=Tshi;
	 Txbuff[1]=Tge;
	 Txbuff[2]=RHshi;
	 Txbuff[3]=RHge;
	 int m;
	 for(m=0;m<4;m++)  {Uart0Send(Txbuff[m]);}
 }

 /*******************************************************
  * LCD12864 欢迎页面
 *******************************************************/
 void Welcome_Menu(void)
  {
   write_string(1,1,hang1);
   write_string(1,2,hang2);
   write_string(1,3,hang3);
   write_string(1,4,hang4);
  }
 /*******************************************************
  * LCD12864显示数据
 *******************************************************/
 void data_display(void)
 {
      write_string(0,1,(INT8U*)"东北：");
      if(distance0y[0]>0)
      write_figurer(3,1,distance0y[0]);
      else write_string(3,1,(INT8U*)" ");

      write_figurer(4,1,distance0y[1]);
      write_string(5,1,(INT8U*)".");
      write_figurer(6,1,distance0y[2]);
      write_string(7,1,(INT8U*)"米");

      write_string(0,2,(INT8U*)"西北：");
      if(distancexy[0]>0)
        write_figurer(3,2,distancexy[0]);
      else write_string(3,2,(INT8U*)" ");
      write_figurer(4,2,distancexy[1]);
      write_string(5,2,(INT8U*)".");
      write_figurer(6,2,distancexy[2]);
      write_string(7,2,(INT8U*)"米");

      write_string(0,3,(INT8U*)"西南：");
      if(distancex0[0]>0)
        write_figurer(3,3,distancex0[0]);
      else write_string(3,3,(INT8U*)" ");
      write_figurer(4,3,distancex0[1]);
      write_string(5,3,(INT8U*)".");
      write_figurer(6,3,distancex0[2]);
      write_string(7,3,(INT8U*)"米");

      write_string(0,4,(INT8U*)"位置:(");
      write_figurer(3,4,location_x[1]);
      write_string(4,4,(INT8U*)",");
      write_figurer(5,4,location_y[1]);
      write_string(6,4,(INT8U*)")");
      write_string(7,4,(INT8U*)"米");
 }


 void date_dis_update(void)
  {
     if(distance0y[0]>0)
       write_figurer(3,1,distance0y[0]);
     else write_string(3,1,(INT8U*)" ");
     write_figurer(4,1,distance0y[1]);
     write_figurer(6,1,distance0y[2]);
     if(distancexy[0]>0)
       write_figurer(3,2,distancexy[0]);
     else write_string(3,2,(INT8U*)" ");
     write_figurer(4,2,distancexy[1]);
     write_figurer(6,2,distancexy[2]);
     if(distancex0[0]>0)
       write_figurer(3,3,distancex0[0]);
     else write_string(3,3,(INT8U*)" ");
     write_figurer(4,3,distancex0[1]);
     write_figurer(3,4,location_x[1]);
     write_figurer(5,4,location_y[1]);
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
    LED1OUT;                   //LED1发送指示灯
    LED1_SEL;                  //普通数字I/O
    LED1_0 ;
    LED2_OUT;
    LED2_SEL;
    LED2_0;
/*************SPI状态初始化 wifi模块I/O口初始化**********/
/*P1.4接GDO0 为输入，P1.5接CSN 为输出，P2.0接CLK 为输出，P2.1 接MOSI 为输出，
 * P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能*/
    GDO0_INPUT;
    CSN_OUTPUT;
    SCK_OUTPUT;
    MOSI_OUTPUT;
    MISO_INPUT;
    GDO0_SEL;      //普通数字I/O
    CSN_SEL;       //普通数字I/O
    SCKMOMI_SEL;   //普通数字I/O
    SCK_0;MOSI_0;
    CSN_1;         //关闭
/*************温湿度传感器DHT11端口初始化********************/
    DHT11_SEL;                     //普通数字I/O 端口
    DHT11_OUTPUT;                   //输出
   // DHT11_H;
/*************lcd1286液晶显示器端口初始化************************/
    SID_SEL;CLK_SEL;
    SID_OUT;CLK_OUT;
    SID_0;CLK_0;
//定时器初始化
    TACCR0 = 50000;                     //计到50000，约50ms
    TACTL = TASSEL_2+ID_0+MC_1+TACLR;  //设置TIMERA的时钟源为SMCLK(1M),不分频,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                   //打开比较模块0中断
//串口初始化（数字量PM2.5传感器接口，单片机RX 仅用于接收 传感器2400bps）
 	  UCA0CTL1 |= UCSWRST;           // USCI_A0 进入软件复位状态
 	  UCA0CTL1 |= UCSSEL_2;          //时钟源选择 SMCLK   1MHz
 	  P1SEL = BIT1 + BIT2 ;          // P1.1 = RXD, P1.2=TXD
 	  P1SEL2 = BIT1 + BIT2 ;         // P1.1 = RXD, P1.2=TXD
 	  UCA0BR0 = 0x68;                //时钟源 1MHz 1M/9600=104=0X0068
 	  UCA0BR1 = 0x00;                //时钟源 1MHz 1M/2400=416=OX01A0
 	  UCA0MCTL = UCBRS0;             //小数分频器
 	  UCA0CTL1 &= ~UCSWRST;          //初始化 USCI_A0 状态机
 	 // IE2 |= UCA0RXIE;               //使能 USCI_A0 接收中断
 	   Init_CC1101();    //wifi模块（CC1101）初始化
 	 lcdinit();
 	 Welcome_Menu();
 	 delay_ms(4000);          //等待4秒
 	 lcdinit();
 	 data_display();
    _enable_interrupts();            //使能全局中断

     while(1)
     {
         if(halRfReceivePacket(RxBuf,&leng))//判断是否接收到数据
         {
        	 if((RxBuf[0]==0x79)&(RxBuf[1]==0x79))    //西北
        	 {
        		 distance0y[0]= RxBuf[2]-48;
        		 distance0y[1]= RxBuf[3]-48;
        		 distance0y[2]= RxBuf[4]-48;
        		 xxrec_flg=1;
        		 RxBuf[0]=0;
        	 }
        	if((RxBuf[0]==0x78)&(RxBuf[1]==0x79))      //东北
        	 {
        	     distancexy[0]= RxBuf[2]-48;
        	     distancexy[1]= RxBuf[3]-48;
        	     distancexy[2]= RxBuf[4]-48;
        	     xyrec_flg=1;
        	     RxBuf[0]=0;
        	 }
        	if((RxBuf[0]==0x78)&(RxBuf[1]==0x79))      //东南
        	  {
        	     distancex0[0]= RxBuf[2]-48;
        	     distancex0[1]= RxBuf[3]-48;
        	     distancex0[2]= RxBuf[4]-48;
        	     yyrec_flg=1;
        	     RxBuf[0]=0;
        	  }

        	if((RxBuf[0]==0x77)&(RxBuf[1]==0x73))       //物体温湿度
        	  {
        		wuti_wsd[0]= RxBuf[2]-48;               //温度十
        		wuti_wsd[1]= RxBuf[3]-48;
        		wuti_wsd[2]= RxBuf[4]-48;               //湿度十
        		wuti_wsd[3]= RxBuf[5]-48;
        		wsdrec_flg=1;
        	    RxBuf[0]=0;
        	   }


        	if((xxrec_flg==1)&&(xyrec_flg==1)&&(yyrec_flg==1))
        	{
        		LED1_0_1;         //wifi接收到三个距离后指示灯
        	   //调用球圆交点函数
        		date_dis_update();
        		//if(wsdrec_flg) date_send_update();     向上位机电脑发送数据
        		//wsdrec_flg=0;
        	}


         }


          if((Tshi*10+Tge)>=30)  LED2_0_1;        //查看温度是否超界限
          if((Tshi*10+Tge)<30)   LED2_0;
  }

}


 //--------定时器中断函数------------
 #pragma vector=TIMER0_A0_VECTOR
 __interrupt void Timer_A(void)     //CCIFG中断被响应后，该标志位自动清零
 {
   t++;
   if(t==period)
	   {
	   LED2_0_1;
	   sendflag=1;}
   if(t>=(period)) t=0;           //因定时器定时时间较短，需要通过二级循环来改变发送数据的周期
 }

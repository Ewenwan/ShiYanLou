#include "msp430G2553.h"
long temp;                //长字节型变量 temp  用于保存AD转换后的数字量
long wendu;                  //长字节型变量IntDegC 摄氏度
int  ge,shi,sf,bf;             //温度数据的个位，十位、十分位、百分位变量
int ul=25,ll=15;               //温度报警上下限
char receive=' ';              //接收上位机字符变量
int flag1s=0,t=0,zhouqi=5,i=1; //发送数据周期相关变量
void Uart0Sends(char *s);      //串口发送字符串函数声明
void Uart0Send(int n);         //串口发送单个数字（字符）函数声明
void main( )
{
  WDTCTL = WDTPW + WDTHOLD;             //停止看门狗  防止时间到了复位
  /****************串口初始化初始化程序********************/
    UCA0CTL1 |= UCSWRST;                // USCI_A0 进入软件复位状态
    UCA0CTL1 |= UCSSEL_2;               //时钟源选择 SMCLK
    BCSCTL1 = CALBC1_1MHZ;              //设置 DCO 频率为 1MHz
    DCOCTL = CALDCO_1MHZ;
    P1SEL = BIT1 + BIT2 ;               // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;              // P1.1 = RXD, P1.2=TXD
    P1DIR |= BIT0;
    UCA0BR0 = 0x68;                     //时钟源 1MHz 时波特率为9600
    UCA0BR1 = 0x00;                     //时钟源 1MHz 时波特率为9600
    UCA0MCTL = UCBRS0;                  //小数分频器
    UCA0CTL1 &= ~UCSWRST;               //初始化 USCI_A0 状态机
    IE2 |= UCA0RXIE;                    //使能 USCI_A0 接收中断

   /*********************定时器初始化********************/
    TACCR0 =10000;                     //计到10000，约100ms
    TACTL = TASSEL_2+ID_3+MC_1+TACLR;  //设置TIMERA的时钟源为SMCLK(1M),8分频后（125K）,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                    //打开 比较模块0中断
/*************************ADC10初始化（模数转换模块初始化）*******/
    ADC10CTL0 &= ~ENC;                                                 //控制字0（辅助）  关闭使能转换
    ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;       //控制字0  中断使能+ADC10使能+内部参考源+64倍ADC时钟+参照选择位1+1.5v参考电压源
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10; //控制字1  单通道单次转换+ADC10OSC+小数分频+ADC10SC+通道10
    __delay_cycles(30000);                                             //延时


    _EINT();                            //使能总中断
  while(1)
  {
	  ADC10CTL0 |= ENC + ADC10SC;                     //控制字0 使能转换 +开始转换
	  __bis_SR_register(CPUOFF + GIE);
      temp=ADC10MEM;                                  //ADC模数转换后的数字量
	  wendu = ((temp - 673) * 423) / 1024.0;      //转换后的数字量变成摄氏温度
	  shi = wendu/10 + 48;                      //十位数字ASCII码
	  ge = wendu%10 + 48;                       //各位数字ASCII码
	  sf = ((temp - 673) * 423)*10 / 1024%10+48;  //十分位数字ASCII码
	  bf = ((temp - 673) * 423)*100 / 1024%10+48; //百分位数字ASCII码
/****************************底层下位机回差报警程序***************************/
	  if((wendu>=ul)||(wendu<=ll))            //温度上下限条件
	   {
	     P1DIR |= BIT0;
	     __delay_cycles(300000);
	     P1OUT ^= BIT0;                           //单片机1.0口的LED灯闪烁报警
	    }
	   else {
	     P1DIR |= BIT0;
	     P1OUT &= ~BIT0;                         //灯灭
	      }
/*******************定时器内循环周期随上位机发送的指令改变而改变**********************/
	 	  if (receive=='M') {zhouqi++;receive='Q';}    //提高发送周期
	 	  if (receive=='K') {zhouqi--;receive='Q';}    //减小发送周期
	 	  if(zhouqi<2) zhouqi=2;                       //周期上限
	 	  if(zhouqi>=9) zhouqi=9;                       //周期下限
/*********根据上位机发送来的数据精度要求把相应精度的温度数据发送给上位机********************/
	 if(flag1s&(receive=='1'))                         //一位小数
	    {
        Uart0Send(shi);                                //发送十位数字
        Uart0Send(ge);                                 //发送个位数字
        Uart0Sends(".");                               //发送小数点
        Uart0Send(sf);                                 //发送十分位
        flag1s=0;                                      //清零发送周期标志位
	       }
	 else if(flag1s&(receive=='0'))                    //仅整数
        {
        Uart0Send(shi);                                //发送十位数字
        Uart0Send(ge);                                 //发送个位数字
        flag1s=0;}                                     //清零发送周期标志位
	 else if(flag1s&(receive=='2'))                    //两位小数
	    {
	    Uart0Send(shi);                                //发送十位数字
	    Uart0Send(ge);                                 //发送个位数字
	    Uart0Sends(".");                               //发送小数点
	    Uart0Send(sf);                                 //发送十分位
	    Uart0Send(bf);                                 //发送白分位
	    flag1s=0;                                      //清零发送周期标志位
	     }
     else {
    	 if(flag1s)                                    //默认发送两位小数
         {
         Uart0Send(shi);
         Uart0Send(ge);
         Uart0Sends(".");
         Uart0Send(sf);
         Uart0Send(bf);
         flag1s=0;
         }
       }
  }
}
/***************************串口接收中断函数****************************/
#pragma vector = USCIAB0RX_VECTOR                      //接收中断
__interrupt void USCI0RX_ISR(void)
{
  receive = UCA0RXBUF;                                 //保存上位机发送来的数据
}

/****************************ADC10中断处理函数*********************** */
#pragma vector=ADC10_VECTOR                           //ADC10中断
__interrupt void ADC10_ISR_HOOK(void)
{
	__bic_SR_register_on_exit(LPM4_bits);             //超低功耗模式
}
/************************定时器中断函数********************************/
 #pragma vector=TIMER0_A0_VECTOR
 __interrupt void Timer_A(void)     //CCIFG中断被响应后，该标志位自动清零
 {
 	if(t>=(zhouqi+1)) t=1;          //因定时器定时时间较短，需要通过二级循环来改变发送数据的周期
 	if(t==zhouqi)
 	{
      if(i>=3) i=1;                 //i在1和2间变化
      switch(i)
      {
      case 1:flag1s=1;break;        //发送周期标志flag1s置1
      case 2:flag1s=0;break;        //发送周期标志flag1s清零
      default:        break;
      }
      i++;
 	}
	  t++;
 }
/************************串口发送字符串函数*****************************/
void Uart0Sends(char *s)
{
  while(*s!='\0')
  {
    UCA0TXBUF=*s;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
    s++;
  }
}

/************************串口发送单个数字函数****************************/
void Uart0Send(int n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}



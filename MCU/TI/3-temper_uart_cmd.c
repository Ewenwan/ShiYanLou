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
	  wendu = ((temp - 673) * 423) / 1024;      //转换后的数字量变成摄氏温度
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
	 	  if (receive=='Z') {zhouqi++;receive='Q';}    //提高发送周期
	 	  if (receive=='J') {zhouqi--;receive='Q';}    //减小发送周期
	 	  if(zhouqi<2) zhouqi=2;                       //周期上限
	 	  if(zhouqi>=9) zhouqi=9;                       //周期下限
/*********数据发送给上位机********************/
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

/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////其二///////////////////////////////////////////////////////

#include "msp430G2553.h"
long temp;                //长字节型变量 temp  用于保存AD转换后的数字量
long wendu;                  //长字节型变量IntDegC 摄氏度
int  gw,sw,sfw,bfw;             //温度数据的个位，十位、十分位、百分位变量
char jieshou=' ';              //接收上位机字符变量
int flag1s=0,t=0,zhouqi=5,i=1; //发送数据周期相关变量
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
  /***************ADC10初始化（模数转换模块初始化）*****××××××**/
    ADC10CTL0 &= ~ENC;                                                 //控制字0（辅助）  关闭使能转换
    ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;       //控制字0  中断使能+ADC10使能+内部参考源+64倍ADC时钟+参照选择位1+1.5v参考电压源
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10; //控制字1  单通道单次转换+ADC10OSC+小数分频+ADC10SC+通道10
    __delay_cycles(30000);                                             //延时
    /***开总中断*****/
    _EINT();
    /******死循环*******/
  while(1)
  {
	  ADC10CTL0 |= ENC + ADC10SC;              //控制字0 使能转换 +开始转换
	  __bis_SR_register(CPUOFF + GIE);         //CPU进入休眠模式，等待AD10转换完成
      temp=ADC10MEM;                           //ADC模数转换后的数字量
	  wendu = ((temp - 673) * 423) / 1024;     //转换后的数字量变成摄氏温度
	  sw = wendu/10 + 48;                      //十位数字ASCII码
	  gw = wendu%10 + 48;                       //各位数字ASCII码
	  sfw = ((temp - 673) * 423)*10 / 1024%10+48;  //十分位数字ASCII码
	  bfw = ((temp - 673) * 423)*100 / 1024%10+48; //百分位数字ASCII码
/*************定时器内循环周期随上位机发送的指令改变而改变****************/
	 	  if (jieshou=='m') {zhouqi++;jieshou=' ';}    //发送变慢
	 	  if (jieshou=='k') {zhouqi--;jieshou=' ';}    //发送变快
	 	  if(zhouqi<2) zhouqi=2;                       //周期上限
	 	  if(zhouqi>=9) zhouqi=9;                      //周期下限
/*********数据发送给上位机********************/
    	 if(flag1s)                           //根据flag1s的变换将数据发送给上位机
         {
         Uart0Send(sw);
         Uart0Send(gw);
         Uart0Send(46);                      //小数点的ASCII码
         Uart0Send(sfw);
         Uart0Send(bfw);
         flag1s=0;
         }
  }
}
/***************************串口接收中断函数****************************/
#pragma vector = USCIAB0RX_VECTOR                    //接收中断
__interrupt void USCI0RX_ISR(void)
{
	jieshou = UCA0RXBUF;                              //保存上位机发送来的数据
}
/****************************ADC10中断处理函数*********************** */
#pragma vector=ADC10_VECTOR                           //ADC10中断
__interrupt void ADC10_ISR_HOOK(void)
{
	__bic_SR_register_on_exit(LPM4_bits);             //ADC10转换完成后，CPU推出休眠模式
}
/************************定时器中断函数********************************/
 #pragma vector=TIMER0_A0_VECTOR
 __interrupt void Timer_A(void)     //CCIFG中断被响应后，该标志位自动清零
 {
 	if(t>=(zhouqi+1)) t=1;          //因定时器定时时间较短，需要通过二级循环来改变发送数据的周期
 	if(t==zhouqi)
 	{
      if(i>=3) i=1;              //i在1和2间变化
      if(i==1) flag1s=1;         //发送周期标志flag1s置1
      if(i==2) flag1s=0;         //发送周期标志flag1s清零
      i++;
 	}
	  t++;
 }
/************************串口发送单个数字函数****************************/
void Uart0Send(int n)
{
    UCA0TXBUF=n;
    while((IFG2&UCA0TXIFG)==0);    //查询发送是否结束
    IFG2&=~UCA0TXIFG;              //清除发送一标志位
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////其三///////////////////////////////////////////////////
#include "msp430G2553.h"
long temp;                //长字节型变量 temp  用于保存AD转换后的数字量
long wendu;                  //长字节型变量IntDegC 摄氏度
int  gw,sw,sfw,bfw;             //温度数据的个位，十位、十分位、百分位变量
int ul=25,ll=15;               //温度报警上下限
char js=' ';              //接收上位机字符变量
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
	  wendu = ((temp - 673) * 423) / 1024;      //转换后的数字量变成摄氏温度
	  sw = wendu/10 + 48;                      //十位数字ASCII码
	  gw = wendu%10 + 48;                       //各位数字ASCII码
	  sfw = ((temp - 673) * 423)*10 / 1024%10+48;  //十分位数字ASCII码
	  bfw = ((temp - 673) * 423)*100 / 1024%10+48; //百分位数字ASCII码
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
	 	  if (js=='s') {zhouqi++;js='a';}    //提高发送周期
	 	  if (js=='f') {zhouqi--;js='a';}    //减小发送周期
	 	  if(zhouqi<2) zhouqi=2;                       //周期上限
	 	  if(zhouqi>=10) zhouqi=10;                       //周期下限
/*********根据上位机发送来的数据精度要求把相应精度的温度数据发送给上位机********************/
	 if(flag1s&(js=='1'))                         //一位小数
	    {
        Uart0Send(sw);                                //发送十位数字
        Uart0Send(gw);                                 //发送个位数字
        Uart0Sends(".");                               //发送小数点
        Uart0Send(sfw);                                 //发送十分位
        flag1s=0;                                      //清零发送周期标志位
	       }
	 else if(flag1s&(js=='0'))                    //仅整数
        {
        Uart0Send(sw);                                //发送十位数字
        Uart0Send(gw);                                 //发送个位数字
        flag1s=0;}                                     //清零发送周期标志位
	 else if(flag1s)                                  //两位小数(默认)
	    {
	    Uart0Send(sw);                                //发送十位数字
	    Uart0Send(gw);                                 //发送个位数字
	    Uart0Sends(".");                               //发送小数点
	    Uart0Send(sfw);                                 //发送十分位
	    Uart0Send(bfw);                                 //发送白分位
	    flag1s=0;                                      //清零发送周期标志位
	     }


  }
}
/***************************串口接收中断函数****************************/
#pragma vector = USCIAB0RX_VECTOR                      //接收中断
__interrupt void USCI0RX_ISR(void)
{
  js = UCA0RXBUF;                                 //保存上位机发送来的数据
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
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 串口收发  定时器中断  adc测量 响应交互控制
#include "msp430G2553.h"
long temp;                //长字节型变量 temp  用于保存AD转换后的数字量
long IntDegC;                  //长字节型变量IntDegC 摄氏度
int  ge,shi,sf,bf;             //温度数据的个位，十位、十分位、百分位变量
int ul=24,ll=19;               //温度报警上下限
int ss,sg,xs,xg;               //接收到的温度上下限变量
char jdbs='2';              //精度标识为初始化为2
unsigned  char R[3]={'O'};
int j=0;


int flag1s=0,t=0,zhouqi=3,i=1; //发送数据周期相关变量

void ADC10_init(void);         //模数转换芯片ADC10初始化函数声明
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
    TACCR0 =20000;                     //计到20000，约200ms
    TACTL = TASSEL_2+ID_3+MC_1+TACLR;  //设置TIMERA的时钟源为SMCLK(1M),8分频后（125K）,计数模式为增计数模式,到TACCR0再自动从0开始计数
    TACCTL0 |= CCIE;                    //打开 比较模块0中断

    ADC10_init();                       //调用ADC10初始化函数
    _EINT();                            //使能总中断
  while(1)
  {
	  ADC10CTL0 |= ENC + ADC10SC;                     //控制字0 使能转换 +开始转换
	  __bis_SR_register(CPUOFF + GIE);
      temp=ADC10MEM;                                  //ADC模数转换后的数字量
	  IntDegC = ((temp - 673) * 423) / 1024;      //转换后的数字量变成摄氏温度
	  shi = IntDegC/10 + 48;                      //十位数字ASCII码
	  ge = IntDegC%10 + 48;                       //各位数字ASCII码
	  sf = ((temp - 673) * 423)*10 / 1024%10+48;  //十分位数字ASCII码
	  bf = ((temp - 673) * 423)*100 / 1024%10+48; //百分位数字ASCII码

/*******************定时器内循环周期随上位机发送的指令改变而改变**********************/

	  if(R[0]=='F')
	    			{
	    			if(R[1]=='K'){ zhouqi--;jdbs='2';R[1]='X';}		//匹配上命令字FK_ 发送变快
	    			if(R[1]=='M'){ zhouqi++;jdbs='2';R[1]='X';}		//匹配上命令字FM_ 发送变慢
	    			if(zhouqi<1) zhouqi=1;      //周期上限
	    		    if(zhouqi>8) zhouqi=8;      //周期下限
	    			}
	  if(R[0]=='J')
	    	  	    {
	    	  		if(R[1]=='2') jdbs='2';		//匹配上命令字J2_ 两位小数精度
	    	  		if(R[1]=='1') jdbs='1';		//匹配上命令字J1_ 一位小数精度
	    	  		if(R[1]=='0') jdbs='0';		//匹配上命令字J0_ 无小数
	    	  		}
	  if(R[0]=='H')
	    	  	    {
	               ss=R[1]-48;  sg=R[2]-48;ul=ss*10+sg;jdbs='2';
	    	  		}
	  if(R[0]=='L')
	    	  		{
	               xs=R[1]-48;  xg=R[2]-48;ll=xs*10+xg;jdbs='2';
	    	  		}
/****************************底层下位机回差报警程序***************************/
	  	  if((IntDegC>=ul)||(IntDegC<=ll-1))            //温度上下限条件
	  	   {
	  	     P1DIR |= BIT0;
	  	  //   __delay_cycles(30000);
	  	   //  P1OUT ^= BIT0;                           //单片机1.0口的LED灯闪烁报警
	  	     if(flag1s) P1OUT |= BIT0;
	  	     else P1OUT &= ~BIT0;
	  	    }
	  	   else {
	  	     P1DIR |= BIT0;
	  	     P1OUT &= ~BIT0;                         //灯灭
	  	      }

/*********根据上位机发送来的数据精度要求把相应精度的温度数据发送给上位机********************/
	 if(flag1s&(jdbs=='1'))                         //一位小数
	    {
        Uart0Send(shi);                                //发送十位数字
        Uart0Send(ge);                                 //发送个位数字
        Uart0Sends(".");                               //发送小数点
        Uart0Send(sf);                                 //发送十分位
        flag1s=0;                                      //清零发送周期标志位
	       }
	 else if(flag1s&(jdbs=='0'))                    //仅整数
        {
        Uart0Send(shi);                                //发送十位数字
        Uart0Send(ge);                                 //发送个位数字
        flag1s=0;}                                     //清零发送周期标志位
	 else if(flag1s&(jdbs=='2'))                    //两位小数
	    {
	    Uart0Send(shi);                                //发送十位数字
	    Uart0Send(ge);                                 //发送个位数字
	    Uart0Sends(".");                               //发送小数点
	    Uart0Send(sf);                                 //发送十分位
	    Uart0Send(bf);                                 //发送白分位
	    flag1s=0;                                      //清零发送周期标志位
	     }
  }
}
/***************************串口接收中断函数****************************/
#pragma vector = USCIAB0RX_VECTOR                      //接收中断
__interrupt void USCI0RX_ISR(void)
{
 // receive = UCA0RXBUF;                                 //保存上位机发送来的数据
  R[j] = UCA0RXBUF;               //发送接收到的数据
  j++;
  if(j>=3) j=0;
}
/****************************ADC10初始化函数***************************/
void ADC10_init(void)
{
    ADC10CTL0 &= ~ENC;                                                 //控制字0（辅助）  关闭使能转换
    ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_1;       //控制字0  中断使能+ADC10使能+内部参考源+64倍ADC时钟+参照选择位1+1.5v参考电压源
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_10; //控制字1  单通道单次转换+ADC10OSC+小数分频+ADC10SC+通道10
    __delay_cycles(30000);                                             //延时
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




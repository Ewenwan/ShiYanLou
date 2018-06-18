#include  "msp430g2553.h"
unsigned  char temp='O';
unsigned  char R[8]={'O'};
int i=0;
#define LED1_ON {P1DIR |=BIT0; P1OUT |=BIT0;}
#define LED1_OFF {P1DIR |=BIT0; P1OUT &=~BIT0;}
#define LED2_ON {P1DIR |=BIT6; P1OUT |=BIT6;}
#define LED2_OFF {P1DIR |=BIT6; P1OUT &=~BIT6;}
void Command_match();  // 字符匹配命令函数
void Uart0Sends(char *s);

char String4[]="I was born for these!\r\n";
char String5[]="I have got it!\r\n";
char String6[]="It is easy for me!\r\n";
char String7[]="As your wish!\r\n";

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;              //停止看门狗
  UCA0CTL1 |= UCSWRST;                   // USCI_A0 进入软件复位状态
  UCA0CTL1 |= UCSSEL_2;                  //时钟源选择 SMCLK
  BCSCTL1 = CALBC1_1MHZ;                 //设置 DCO 频率为 1MHz
  DCOCTL = CALDCO_1MHZ;
  P1SEL = BIT1 + BIT2 ;                           // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                          // P1.1 = RXD, P1.2=TXD
  P1DIR |= BIT0;
  UCA0BR0 = 0x68;                             //时钟源 1MHz 时波特率为9600
  UCA0BR1 = 0x00;                             //时钟源 1MHz 时波特率为9600
  UCA0MCTL = UCBRS0;                     //小数分频器
  UCA0CTL1 &= ~UCSWRST;                  //初始化 USCI_A0 状态机
  IE2 |= UCA0RXIE;                            //使能 USCI_A0 接收中断
  _EINT();                                    //开总中断
  while(1)
    {

	 // if (temp=='O') LED1_ON;
	//  if (temp=='F') LED1_OFF;
	  Command_match();
	 // Uart0Sends("Hellow ni好 ");
	  __delay_cycles (100000);
	 // Command_match();
    }
  }

  #pragma vector = USCIAB0RX_VECTOR          //接收中断
  __interrupt void USCI0RX_ISR(void)
  {
   // while ( !(IFG2&UCA0TXIFG) );                 //确保发送缓冲区准备好
   // P1OUT ^= BIT0;                           //接收指示灯状态改变
	 temp = UCA0RXBUF;               //发送接收到的数据
     R[i] = UCA0RXBUF;               //发送接收到的数据
     i++;
     if(i>=8) i=0;
  }

  void Command_match()  // 字符匹配命令
  {
  	unsigned char Command_Num=0;
  	//-----命令共4种：LED1_ON,LED1_OFF,LED2_ON,LED2_OFF-----
  	if((R[0]=='L')&&(R[1]=='E')&&(R[2]=='D')
  			&&(R[4]=='_')&&(R[5]=='O'))//先匹配共有字母LED?_O??
  			{
  					if((R[3]=='1')&&(R[6]=='N'))
  							Command_Num=1;		//匹配上命令字LED1_ON
  					if((R[3]=='1')&&(R[6]=='F')&&(R[7]=='F'))
  							Command_Num=2;		//匹配上命令字LED1_OFF
  					if((R[3]=='2')&&(R[6]=='N'))
  							Command_Num=3;		//匹配上命令字LED2_ON
  					if((R[3]=='2')&&(R[6]=='F')&&(R[7]=='F'))
  							Command_Num=4;		//匹配上命令字LED2_OFF
  			}
  	switch(Command_Num)
  	{
  	//case 0: UART_SendString(String3);break;		//没匹配上任何命令，发送错误提示语
  	 //-----执行LED控制命令，并给出正面积极提示语-----
  	case 1: LED1_ON;Uart0Sends(String4);  break;
  	case 2: LED1_OFF;Uart0Sends(String5); break;
  	case 3: LED2_ON;Uart0Sends(String6);  break;
  	case 4: LED2_OFF;Uart0Sends(String7); break;
  	default:          break;
  	}
  }

  void Uart0Sends(char *s)
  {
    while(*s!='\0')
    {
      UCA0TXBUF=*s;
      while((IFG2&UCA0TXIFG)==0); //查询发送是否结束
      IFG2&=~UCA0TXIFG; //清除发送一标志位
      s++;
    }
  }



//P1.5口接DHT11数据线DATA
#include "msp430g2553.h"
#define DHT11_OUTPUT   P1DIR |= BIT5         //为1，输出
#define DHT11_INPUT    P1DIR &= ~BIT5        //为0，输入
#define DHT11_H        P1OUT |= BIT5         //高电平
#define DHT11_L        P1OUT &= ~BIT5        //低电平
#define DHT11_IN       (P1IN&BIT5)           //P2.5输入状态
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define CPU          (1000000)              //1MHz
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))

uchar start_DHT11(void);
uchar DHT11_ReadChar(void);
unsigned int RHshi,RHge,Tshi,Tge;
uchar DHT11T_Data_H, DHT11T_Data_L, DHT11RH_Data_H, DHT11RH_Data_L,CheckData_temp;
/**********************************************************/
uchar start_DHT11(void)
{
uchar TData_H_temp,TData_L_temp,RHData_H_temp,RHData_L_temp,checktemp;
uchar flag=0;
uint  count;
DHT11_OUTPUT;                             //输出
DHT11_L;                                  //拉低18ms以上
delay_ms(20);                             //延时20毫秒
DHT11_H;                                  //变为高电平
delay_us(30);                             //等待40微妙，DATA为输出
DHT11_INPUT;                              //转为输入状态
if(!DHT11_IN )                            //DHT11_IN=0时，即DATA输出低电平应答信号
  {
   count=2;                               //
   while((!DHT11_IN)&&count++);           //等待低电平
   count=2;
   while((DHT11_IN)&&count++);
      RHData_H_temp = DHT11_ReadChar();   //先出高位湿度整数
      RHData_L_temp = DHT11_ReadChar();   //出湿度小数
      TData_H_temp = DHT11_ReadChar();    //出温度整数
      TData_L_temp = DHT11_ReadChar();    //出温度小数
      CheckData_temp = DHT11_ReadChar();  //出校验位

      checktemp = (RHData_H_temp + RHData_L_temp + TData_H_temp + TData_L_temp);                                                             //计算的校验位
      if (checktemp == CheckData_temp)
      {                                   //校验正确
         DHT11RH_Data_H = RHData_H_temp;
         DHT11RH_Data_L = RHData_L_temp;
         DHT11T_Data_H = TData_H_temp;
         DHT11T_Data_L = TData_L_temp;
         flag=1;                          //正确，标志位为1
         RHshi=DHT11RH_Data_H/10;
         RHge=DHT11RH_Data_H%10;
         Tshi=DHT11T_Data_H/10;
         Tge=DHT11T_Data_H%10;
      }
  }
return flag;
}

/*************************读DHT11数据函数*****************************/
uchar DHT11_ReadChar(void)
{
   unsigned char dat,temp;                      //数据位
   unsigned int flag1;                          //计数防止死等
   unsigned char i;
   for(i=0;i<8;i++)                             //每次读8个二进制位
   {
	   flag1=2;
      while((!DHT11_IN)&&flag1++);              //等待50us低电平结束
            delay_us(10);                       //延时10us
            delay_us(10);                       //延时10us
            delay_us(10);                       //延时10us
            delay_us(10);                       //延时10us
          if(DHT11_IN)                          //
            temp=1;
          else
        	temp=0;
          dat<<=1;
          dat |= temp;
          flag1=2;
          while((DHT11_IN)&&flag1++);
              if(flag1==1)                      //超时则跳出for循环
                break;

       }

       return dat;
}
/******************************DHT11_UART_init***********************************/
void DHT11_UART_init(void)
{
  UCA0CTL1 |= UCSWRST;                     // USCI_A0 进入软件复位状态
  UCA0CTL1 |= UCSSEL_2;                    // 时钟源选择 SMCLK
  UCA0BR0 = 104;                           // 时钟源 1MHz 时波特率为9600
  UCA0BR1 = 0;                             // 时钟源 1MHz 时波特率为9600
  UCA0MCTL = UCBRS0;                       // 小数分频器
  UCA0CTL1 &= ~UCSWRST;                    // 初始化 USCI_A0 状态机
  IE2 |= UCA0RXIE;                         // 使能 USCI_A0 接收中断

  BCSCTL1 = CALBC1_1MHZ;                   //基础时钟控制字1 时钟频率1MHz
  DCOCTL = CALDCO_1MHZ;                    //数控晶振时钟控制字  数控时钟1MHz
  P1SEL = BIT1 + BIT2 ;                    //P1.1(接收)\P1.2（发送）第二功能选择
  P1SEL2 = BIT1 + BIT2 ;
}
/**********************************DHT11_port_init***********************************/
void DHT11_port_init(void)
{
  P1SEL &= ~BIT5;                         //关闭P2.5的第二功能
 // P2SEL |= BIT5;                        //开P2.5的第二功能
  delay_ms(1000);
}

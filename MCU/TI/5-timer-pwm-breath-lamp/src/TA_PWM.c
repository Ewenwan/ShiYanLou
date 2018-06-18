// 初始化函数TA0_PWM_Init() 我们在编程中，总是会有大量的初始化函数，这类函数的共同特点就是一般只需设定一 次，不会被经常修改。
//==========================TA_PWM.c==========================
  #include "MSP430G2553.h"
  #define DEADTIME 20                          //预设死区时间，以TA 的clk为单位
  /*******设定TA输出IO 口，目前设定为MSP430G2553，20Pin封装无TA0.2********/
  #define TA01_SET P1SEL |= BIT6; P1DIR |= BIT6 //P1.6          P1.6 （PWM 输出口）控制LED 来实现呼吸灯。
//  #define TA01_SET P1SEL |= BIT0; P1DIR |= BIT0 //P1.0
  #define TA02_SET P3SEL |= BIT0; P3DIR |= BIT0 //P3.0
  #define TA11_SET P2SEL |= BIT2; P2DIR |= BIT2 //P2.2
  #define TA12_SET P2SEL |= BIT4; P2DIR |= BIT4 //P2.4
  #define TA01_OFF P1SEL&= ~BIT6                //P1.6
//  #define TA01_OFF P1SEL&= ~BIT0                //P1.0
  #define TA02_OFF P3SEL &= ~BIT0               //P3.0
  #define TA11_OFF    P2SEL &= ~BIT2            //P2.2
  #define TA12_OFF    P2SEL &= ~BIT4            //P2.4
   /******************************************************************************************************
   *  名  称：TA0_PWM_Init()
   *  功    能：TA0定时器作为PWM发生器的初始化设置函数
   *  入口参数： ClOCk:时钟源 'S'=SMCLK;              'A'=ACLK ;    'E'=TACLK(外部输入)； 'e'= TACLK取反
                  Div:时钟分频系数: 1/2/4/8
                  Mode1:通道1的输出模式 'F'设为超前PWM  （模式7 ），'B'滞后PWM  （模式3 ），'D'
                         带死区增PWM  （模式6 ），0=禁用
                  Mode2:通道2 的输出模式 'F'设为超前PWM  （模式7 ），'B'滞后PWM  （模式3 ），'D'
                         带死区减PWM  （模式2 ），0=禁用
                 设置输出带死区控制的PWM时，两通道均需使用，且均为死区模式。
   * 出口参数：1表示设置成功，0表示参数错误，设置失败。
   *  说     明：在调用PWM相关函数之前，需要调用该函数设置TA 的模式和时钟源。
   *  范  例：TA0_PWM_Init('A',1,'F','F')TA时钟设为ACLK,通道1和通道2均为超前PWM输出
          TA0_PWM_Init('S',4,'D','D')TA时钟设为SMCLK/4,  通道1为死区增PWM、通道2为死区减PWM
          TA0_PWM_Init('A',1,'F',0)TA时钟设为ACLK,通道1超前PWM输出，通道2不作TA用。
   ******************************************************************************************************/
   char TA0_PWM_Init(char Clk,char Div,char Mode1,char Mode2)
   {
     TA0CTL =0;                                                 //  清除以前设置
     switch(Clk)                                                //为定时器TA选择时钟源
     {
       case 'A': case 'a':  TA0CTL|=TASSEL_1; break;            //ACLK
       case 'S': case 's':  TA0CTL|=TASSEL_2; break;            //SMCLK
       case 'E':             TA0CTL|=TASSEL_0; break;           //外部输入(TACLK)
       case 'e':             TA0CTL|=TASSEL_3; break;           //外部输入(TACLK取反)
       default :  return(0);                                    //设置参数有误，返回0
     }
     switch(Div)                                                //为定时器TA选择分频系数
     {
       case 1:    TA0CTL|=ID_0; break;                          //1
       case 2:    TA0CTL|=ID_1; break;                          //2
       case 4:    TA0CTL|=ID_2; break;                          //4
       case 8:    TA0CTL|=ID_3; break;                          //8
       default :  return(0);                                    //设置参数有误，返回0
     }
     switch(Mode1)                                                     //为定时器选择计数模式
          {
          case 'F': case 'f': case 'B':case 'b':  TA0CTL |=MC_1; break; //普通PWM，增计数
          case 'D': case 'd':                 TA0CTL |=MC_3; break;         //死区PWM，增减计数
          default : return(0);                              //其他情况都是设置参数有误，返回0
          }
         switch(Mode1)                                      //设置PWM通道1的输出模式。
          {
              case 'F': case 'f': TA0CCTL1 = OUTMOD_7; TA01_SET;break;
              case 'B': case 'b': TA0CCTL1 = OUTMOD_3; TA01_SET;break;
              case 'D': case'd': TA0CCTL1 = OUTMOD_6; TA01_SET;break;
              case '0': case 0:   TA01_OFF;break;           //如果设置为禁用TA0.1恢复为普通IO 口
              default :  return(0);                         //设置参数有误，返回0
          }
          switch(Mode2)                                     //设置PWM通道2 的输出模式。
          {
               case 'F': case 'f': TA0CCTL2 = OUTMOD_7; TA02_SET; break;
               case 'B': case 'b': TA0CCTL2 = OUTMOD_3;TA02_SET; break;
               case 'D': case 'd':TA0CCTL2 = OUTMOD_2;TA02_SET;break;
               case '0': case 0: TA02_OFF; break;           //如果设置为禁用TA0.1恢复为普通IO 口
               default :  return(0);                        //设置参数有误，返回0
             }
          return(1);
        }
   /***************************************************************************
      *  名  称：TA0_PWM_SetPeriod()    周期设定函数TA0_PWM_SetPeriod()    对TA0CCR0 赋值即可改变PWM 的周期。
      *  功     能：设置PWM发生器的周期
      *  入口参数：Period:周期(0~65535)  时钟个数
      * 出口参数：1：设置成功                  0：设置失败
      *  说     明： 普通PWM与带死区PWM周期相差一倍
      *  范  例：TA0_PWM_SetPeriod(500)设置PWM方波周期为500或1000个时钟周期
      ****************************************************************************/

   char TA0_PWM_SetPeriod(unsigned int Period)
   {
        if (Period>65535)  return(0);
         TA0CCR0 = Period;
        return(1);
   }
   /******************************************************************************************************
     *  名  称：TA0_PWM_SetPermill()
     *  功    能：设置PWM输出的占空比(千分比)
     *  入口参数：Channel:  当前设置的通道号 1/2
                 Duty: PWM高电平有效时间的千分比 (0~1000)，
     * 出口参数：1设置成功，0设置失败
     *  说    明: 1000=100.0%    500=50.0%  ，依次类推。死区模式时，两channel 同时设定。
     *  范  例: TA0_PWM_SetPermill(1,300)设置PWM通道1方波的占空比为30.0%
                TA0_PWM_SetPermill(2,825)设置PWM通道2方波的占空比为82.5%
      ******************************************************************************************************/
     char TA0_PWM_SetPermill(char Channel,unsigned int Duty)
     {
          unsigned char Mod = 0;
          unsigned int DeadPermill=0;
          unsigned long int Percent=0;                   // 防止乘法运算时溢出
          Percent=Duty;
          DeadPermill=((DEADTIME*1000)/TACCR0);          //将绝对死区时间换算成千分比死区时间
          switch (Channel)                               //先判断出通道的工作模式
              {
          case 1:
        	  Mod = (TA0CCTL1& 0x00e0)>>5;break;                   //读取输出模式，OUTMOD0位于5-7位
        	          case 2:
        	                 Mod = (TA0CCTL2 & 0x00e0)>>5;break;                 //读取输出模式，OUTMOD1位于5-7位
        	          default:    return(0);
        	                }
        	          switch(Mod)                                                //根据模式设定TACCRx
       	                {
        	          case 2: case 6:
       	               //-----死区模式2,6时，需要判断修正死区时间，且同时设定TA0CCR1/2                                        的值-----
        	                {
        	                      if((1000-2*Percent)<= DeadPermill)             //预留死区时间
        	                            Percent=(1000- DeadPermill)/2;
        	                      TA0CCR1=Percent*TA0CCR0/1000;
                              TA0CCR2= TA0CCR0-TA0CCR1;
        	                      break;
        	                }
        	                case 7:
        	                {
        	                     if(Percent>1000)        Percent=1000;
        	                      if(Channel==1) TA0CCR1=Percent* TA0CCR0/1000;
        	                      if(Channel==2) TA0CCR2=Percent* TA0CCR0/1000;
        	                      break;
        	                }
        	                case 3:                            // 占空比一律为正脉宽，所以需要TA0CCR0减去占空比
        	                {
        	                      if(Percent>1000)        Percent=1000;
        	                      if(Channel==1) TA0CCR1= TA0CCR0-Percent*TA0CCR0/1000;
        	                      if(Channel==2) TA0CCR2= TA0CCR0-Percent*TA0CCR0/1000;
        	                      break;
        	                }
        	                default: return(0);
        	                }
        	                 return (1);
        	          }



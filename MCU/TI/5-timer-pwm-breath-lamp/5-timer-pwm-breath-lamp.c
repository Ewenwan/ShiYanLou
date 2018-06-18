   #include "MSP430G2553.h"
   #include "TA_PWM.h"
   #include "Breath_Lamp_Event.h"
   #include "Breath_Lamp_Global.h"
   #define PWM_PERIOD 100                   //设定PWM周期
   void main(void)
   {
       WDTCTL = WDTPW + WDTHOLD;
       //-----初始化TA定时器-----
       TA0_PWM_Init('A',1,'F',0);           //ACLK,  不分频，TA0.1  输出超前PWM
       TA0_PWM_SetPeriod(PWM_PERIOD);       //设定PWM的周期
       //-----初始化看门狗定时器-----
       WDTCTL = WDT_ADLY_16;                //WDT设为16ms定时器模式
        IE1 |= WDTIE;                      //使能寄存器IE1中相应的WDT 中断位
        _enable_interrupts();               //使能总中断，等同_EINT()
       _bis_SR_register(LPM3_bits) ;       //等同LPM3
   }
   /********************************************************************************
    *  名  称：WDT_ISR()
    *  功   能：响应WDT定时中断服务
    *  入口参数：无
    *  出口参数：无
    *  说    明：WDT定时中断独占中断向量，所以无需进一步判断中断事件，也无需人工清除标志
              位。所以，在WDT定时中断服务子函数中，直接调用WDT事件处理函数就可以了。
    *  范  例：无
  ****************************************************************************/
  #pragma vector=WDT_VECTOR
  __interrupt void WDT_ISR(void)
  {
       WDT_Ontime();
  }


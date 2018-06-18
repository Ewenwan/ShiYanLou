 #include<msp430G2553.h>
 // 看门狗定时器实现 led闪烁
   void   main( void )
   {
     unsigned int i=0;
     WDTCTL=WDTPW+WDTHOLD;      //关闭看门狗   watch dog control
                                //(看门狗控制寄存器)=（赋值）watch dog password（0×5A00）+ WDTCTL这个寄存器的HOLD位置1 (0×0080)
     P1DIR |=BIT0;              //P1.0为1，输出  红灯
     P1OUT &=~BIT0;             //暗
     for(i=0;i<16000;i++);
  //   for(i=0;i<10;i++);
     P1OUT |=BIT0;             //亮
     WDTCTL= WDT_ARST_1000;    //启动看门狗为1000ms定时
	 // 看门狗发小cpu死循环不响应，会使cpu重启
	 // 就会执行上面的初始化程序 灯会闪烁
   //  WDTCTL= WDT_ARST_250;   // 250ms
     while(1);
   }
/////////////////////////////////////////////////////////////////////////
///////////////////////////////其二//////////////////////
/*设定看门狗定时器工作模式为看门狗，P1.0口接 LED，正常工作时为常亮，由于 看门狗的复位作用，
 * LED  会闪烁。引入“喂狗”后，看门狗不会再复位，LED也不再闪烁。 */

   #include<MSP430G2553.h>
   void   main( void )
   {
     unsigned int i=0;
     WDTCTL=WDTPW+WDTHOLD; //关闭看门狗   watch dog control
                           //(看门狗控制寄存器)=（赋值）
//watch dog password（0×5A00）+ WDTCTL这个寄存器的HOLD位置1 (0×0080)
     P1DIR |=BIT0;
     P1OUT &=~BIT0;                                    //暗
     for(i=0;i<60000;i++);
     P1OUT |=BIT0;                                     //亮
     WDTCTL= WDT_ARST_1000;                             //启动看门狗为1000ms定时
     while(1)
          {
          for(i=0;i<1000;i++); //主函数任务
          WDTCTL = WDT_ARST_1000 + WDTCNTCL; 
		  // 喂狗，且不影响看门狗定时设置  watch   dog   timer   counter   clear 看门狗定时器清零
          // 除了喂狗，还得重新设定看门狗定时值
// 注： “#define   WDT_ARST_1000   (WDTPW+WDTCNTCL+WDTSSEL) ”是将看门狗复位定 时值设为 1s 的宏定义组合
          }
   }
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////其三//////////////////////////////////////////
 // 使用看门狗定时器，低功耗的实现P1.0 所接LED 每秒1 次频率闪烁。
# include    <msp430G2553.h>
   void main(void)
   {
         WDTCTL = WDT_ADLY_1000;                           //定时周期为1000ms
         IE1 |= WDTIE;                                     //使能WDT 中断
         P1DIR |= 0x01;                                    // P1.0输出
          _enable_interrupts();                            //等同_EINT() ， 系统总中断允许
         while(1)                                          //循环等待定时器溢出中断
         {
         _bis_SR_register(LPM3_bits);                       //进入 LPM3
         _NOP();
         }
   }
   //===========看门狗中断服务子程序========
   #pragma vector= WDT_VECTOR
   __interrupt void WDT_ISR (void)                          //此处开头为双下划线
   {
         P1OUT ^= 0x01;                                     //P1.0取反
   }
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////其四///////////////////////////////////////////////
// 将 RST/NMI  引脚设为NMI 模式，主程序中点亮 P1.0  口LED，在NMI  中断中关掉
// LED。现象为当按下RST 按键时，LED 熄灭，并且再也不亮（除非重新上电）。
   #include<MSP430G2553.h>
   void   main( void )
   {
     WDTCTL = WDTPW + WDTHOLD + WDTNMI;                    //NMI模式（非Reset模式）
      IE1=NMIIE;                                          //开NMI中断，无需开总中断
      P1DIR |=BIT0;                                       //P1.0设为输出口
    //  P1DIR |=BIT6;
      P1OUT |=BIT0;                                       //P1.0 亮灯   RED
    //  P1OUT |=BIT6;
      _bis_SR_register(LPM3_bits);                         //低功耗模式3
   }
   //==========NMI 中断服务子函数==============
   #pragma vector=NMI_VECTOR
   __interrupt void NMI_ISR(void)                          //不可屏蔽中断
   {
	      P1OUT &= ~BIT0;                       //灭灯
}




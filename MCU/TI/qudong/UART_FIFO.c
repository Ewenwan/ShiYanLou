/*
 * UART_FIFO.c
 */
#include "MSP430G2553.h"
#include "UART_Global.h"
   /******************************************************************************************************
    *  名  称：Rx_FIFO_WriteChar()
    *  功   能：往Rx接收FIFO中写1字节
    *  入口参数：Data：待写入FIFO的数据
    * 出口参数：1：写入数据成功，0：写入数据失败
    *  说   明：操作FIFO时需要关闭总中断
    *  范  例：无
   ******************************************************************************************************/
   char Rx_FIFO_WriteChar(unsigned char Data)
   {
       if(Rx_FIFO_DataNum==RX_FIFO_SIZE) return(0);
                                    //判断FIFO是否已装满未读数据，如果装满返回0
       _disable_interrupts();                  //操作FIFO前一定要关总中断
       Rx_FIFO_DataNum++;                      //未读取数据个数加一
       Rx_FIFO[Rx_FIFO_IndexW]=Data;           //将数据写入写读指针位置的FIFO数组
       Rx_FIFO_IndexW++;                        //写指针移位
       if (Rx_FIFO_IndexW>=RX_FIFO_SIZE)       //判断指针是否越界
            Rx_FIFO_IndexW=0;                  //写指针循环归零
       _enable_interrupts();                    //恢复总中断使能
       return(1);                              //返回成功
   }
   /******************************************************************************************************
     *  名  称：Rx_FIFO_ReadChar()
     *  功   能：从Rx接收FIFO中读1字节
     *  入口参数：*Chr ：待存放字节变量的指针
    * 出口参数：“1”读取数据成功，“0”读取数据失败
    *  说   明：操作FIFO时需要关闭总中断
    *  范  例：无
   ******************************************************************************************************/
   char Rx_FIFO_ReadChar(unsigned char *Chr)
   {
       if(Rx_FIFO_DataNum==0) return(0);   //判断FIFO是是否有未读数据，如果没有返回0
       _disable_interrupts();              //操作FIFO前一定要关总中断
       Rx_FIFO_DataNum--;                  //待读取数据个数减一
       *Chr=Rx_FIFO[Rx_FIFO_IndexR];        //将读指针位置的FIFO数据赋给指针所指变量
       Rx_FIFO_IndexR++;                    //读指针移位
       if (Rx_FIFO_IndexR>=RX_FIFO_SIZE)    //判断指针是否越界
            Rx_FIFO_IndexR=0;               //读指针循环归零
       _enable_interrupts();               //恢复总中断使能
       return(1);
   }
   /******************************************************************************************************
    *  名  称：Rx_FIFO_Clear()
    *  功   能：清空Rx接收FIFO区
    *  入口参数：无
    *  出口参数：无
    *  说   明：清空并不需要真的去将FIFO每一个字节的数据写0，
    *  只需读写指针清零和空满计数清零即可。
    *  范  例：无
   ******************************************************************************************************/
   void Rx_FIFO_Clear()
   {
       _disable_interrupts();                   //操作FIFO前一定要关总中断
       Rx_FIFO_DataNum=0;                       //FIFO 中未读取数据数目清零
       Rx_FIFO_IndexR=0;                 //FIFO 中模拟读指针清零
        Rx_FIFO_IndexW=0;                 //FIFO 中模拟写指针清零
        _enable_interrupts();             //恢复总中断使能
         }
   /******************************************************************************************************
      *  名  称：Tx_FIFO_WriteChar()
      *  功  能：往Tx发送FIFO中写1字节
      *  入口参数：Data：待写入FIFO的数据
      * 出口参数：1：写入数据成功，0：写入数据失败
*  说   明：“全新”一次发送数据必须手动触发Tx中断；“非全新”发送一定不能手动触发Tx 中断。

               全新发送的判据必须同时满足FIFO无数据和Tx不Busy两个条件。
    *  范  例：无
   ******************************************************************************************************/
   char Tx_FIFO_WriteChar(unsigned char Data)
   {
       if(Tx_FIFO_DataNum==TX_FIFO_SIZE) return(0);
                                       //判断FIFO是否已装满未读数据，如果装满返回0
       _disable_interrupts();                   //操作FIFO前一定要关总中断
       //-----“全新”一次发送数据必须手动触发Tx中断-----
       if((Tx_FIFO_DataNum==0) &&( !(UCA0STAT & UCBUSY)))
                                               //判断是否为一次“全新”发送
       IFG2 |=UCA0TXIFG;                  //  手动触发一次
       Tx_FIFO_DataNum++;                      //未读取数据个数加一
       Tx_FIFO[Tx_FIFO_IndexW]=Data;           //将数据写入写读指针位置的FIFO数组
       Tx_FIFO_IndexW++;                       //写指针移位
       if (Tx_FIFO_IndexW >= TX_FIFO_SIZE)     //判断指针是否越界
           Tx_FIFO_IndexW=0;                   //写指针循环归零
       _enable_interrupts();                   //恢复总中断使能
       return(1);                              //返回成功
   }
   /******************************************************************************************************
      *  名  称：Tx_FIFO_ReadChar()
      *  功   能：从Tx发送FIFO中读1字节
      *  入口参数：*Chr ：待存放字节变量的指针
      * 出口参数：“1”读取数据成功，“0”读取数据失败
      *  说   明：操作FIFO时需要关闭总中断
      *  范  例：无
     ***********************************************************************
     **********************************************************************/
   char Tx_FIFO_ReadChar(unsigned char *Chr)
      {
           if(Tx_FIFO_DataNum==0) return(0);   //判断FIFO是是否有未读数据，如果没有返回0
          _disable_interrupts();               //操作FIFO前一定要关总中断
          Tx_FIFO_DataNum--;                   //待读取数据个数减一
          *Chr=Tx_FIFO[Tx_FIFO_IndexR];       //将读指针位置的FIFO数据赋给指针所指变量
          Tx_FIFO_IndexR++;                    //读指针移位
           if (Tx_FIFO_IndexR>=TX_FIFO_SIZE)    //判断指针是否越界
               Tx_FIFO_IndexR=0;                //读指针循环归零
          _enable_interrupts();                 //恢复总中断使能
           return(1);                           //返回成功
      }
   /******************************************************************************************************
       *  名  称：Tx_FIFO_Clear()
       *  功    能：清空Tx发送FIFO区
       *  入口参数：无
       *  出口参数：无
       *  说    明：清空并不需要真的去将FIFO每一个字节的数据写0，
       *  只需读写指针清零和空满计数清零即可。
       *  范  例：无
      ******************************************************************************************************/
      void Tx_FIFO_Clear()
      {
          _disable_interrupts();                     //操作FIFO前一定要关总中断
          Tx_FIFO_DataNum=0;                         //FIFO 中未读取数据数目清零
          Tx_FIFO_IndexR=0;                          //FIFO 中模拟读指针清零
          Tx_FIFO_IndexW=0;                          //FIFO 中模拟写指针清零
          _enable_interrupts();                      //恢复总中断使能
      }

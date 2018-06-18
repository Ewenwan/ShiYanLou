
#ifndef CC1101_H
#define CC1101_H

#include "msp430g2553.h"
#include "CC1101_ResAddr.h"
//=========================================================================
#define INT8U unsigned char             //无符号字符
#define INT8S signed char               //有符号字符
#define INT16U unsigned int             //无符号整型
#define INT16S signed int               //有符号整型       
#define INT32U unsigned long            //无符号长整型
#define INT32S signed long              //有符号长整型
 //*********************SPI状态初始化 wifi模块I/O口初始化**********************
 //P1.4接GDO0 为输入，P1.5接CSN 为输出，P2.0接CLK 为输出，P2.1 接MOSI 为输出，
 //P2.2接MISO 为输入，GDO2 可不接，且均为普通数字I/O，即关闭第二功能。
//==========================GDO0状态========================================
#define GDO0_0 P2OUT &= ~BIT0
#define GDO0_1 P2OUT |=  BIT0          //0000 0000 BIT0
#define GDO0_IN (P2IN & BIT0)          //GDO0输入状态
#define GDO0_INPUT P2DIR &= ~BIT0      //为0，输入
#define GDO0_SEL   P2SEL &= ~BIT0      //普通数字I/O
//=============================片选 SPI使能端口=================================
#define CSN_0 P1OUT &= ~BIT5           //0010 0000 BIT5
#define CSN_1 P1OUT |=  BIT5
#define CSN_OUTPUT   P1DIR |= BIT5     //为1，输出
#define CSN_SEL   P1SEL &= ~BIT5       //普通数字I/O
//==============================SPI时钟端口===================================
#define SCK_0 P1OUT &= ~BIT4           //0001 0000 BIT4
#define SCK_1 P1OUT |=  BIT4
#define SCK_OUTPUT  P1DIR |= BIT4     //为1，输出
//==================================主出从入==================================
#define MOSI_0 P1OUT &= ~BIT2          //0000 0100 BIT2
#define MOSI_1 P1OUT |=  BIT2
#define MOSI_1IN (P1IN & BIT2)         //MOSI输入状态
#define MOSI_OUTPUT  P1DIR |= BIT2     //为1，输出
//================================主入从出/GDO1===============================
#define MISO_0 P1OUT &= ~BIT1
#define MISO_1 P1OUT |= BIT1           //0000 0010 BIT1
#define MISO_IN (P1IN & BIT1)          //MISO输入状态
#define MISO_INPUT P1DIR &= ~BIT1      //为0，输入
#define SCKMOMI_SEL P2SEL &= ~(BIT4+BIT1+BIT2) //普通数字I/O
//=========================GDO2状态（可不连）===================================
//=========================LED 输出（发送指示灯）=================================
#define  LED1_0     P1OUT &= ~BIT0            //输出0
#define  LED1_1     P1OUT |= BIT0            //输出1
#define  LED1_0_1   P1OUT ^= BIT0            //输出1/0
#define  LED1OUT    P1DIR |= BIT0;           //为1，输出
#define  LED1_SEL   P1SEL &= ~BIT0;          //普通数字I/O

//自己定义的数据类型
typedef enum { TX_MODE, RX_MODE }TRMODE;
typedef enum { BROAD_ALL, BROAD_NO, BROAD_0, BROAD_0AND255 }ADDR_MODE;
typedef enum { BROADCAST, ADDRESS_CHECK}TX_DATA_MODE;

//*******************************功率设置*****************************************
//*****更多功率参数设置可详细参考DATACC1100英文文档中第48-49页的参数表**********
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04}; //-30dBm 功率最小
extern INT8U PaTabel[8];   //=  {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm;
//INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0}; //10dBm 功率最大

//************************************错误信号************************************
#define MISO_ERROR 0x01 //等待MISO错误
#define GDO0_ERROR 0x02 //等待GND0错误
//*********************************函数声明*************************************//
extern INT8U MNSPI_ExchangeByte(INT8U dat);                //SPI 发送1字节 并接收1字节函数  最基础的函数
extern void XW_CC1101WORInit( void );                     //
extern void XW_CC1101WriteReg(INT8U addr, INT8U value);       //SPI写寄存器函数
extern void XW_CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size );     //SPI连续写 配置寄存器 函数
extern void XW_CC1101WriteCmd( INT8U command );                 //写命令函数
extern INT8U XW_CC1101ReadReg(INT8U addr);                    //读寄存器函数 
extern void XW_CC1101ReadMultiReg(INT8U addr, INT8U *buffer, INT8U size);      //连续读 配置寄存器 函数
extern INT8U XW_CC1101ReadStatus(INT8U addr);                 //SPI读寄存器状态函数
extern void XW_CC1101SetTRMode( TRMODE mode );
extern void XW_CC1101RESET(void);
extern void XW_CC1101SetIdleMODE( void );
extern void XW_CC1101ClrTXBuff( void );
extern void XW_CC1101ClrRXBuff( void );
extern void XW_CC1101SetAddress( INT8U address, ADDR_MODE AddressMode);
extern void XW_CC1101SetSYNC( INT16U sync );
extern void XW_CC1101Init(void);
extern INT8U XW_CC1101GetRXCount( void );

extern void XW_CC1101SendPacket( INT8U *txBuffer, INT8U size, TX_DATA_MODE mode );  //CC1100发送一组数据函数
extern INT8U XW_CC1101RecPacket( INT8U *rxBuffer );                                 //CC1100接收一组数据函数
#endif //#ifndef CC1101_H

/************************************************************************
*MSP430G2553单片机串行控制LCD12864头文件
*by:万有文 2015-2-20
*文件名称：LCD12864.c
P2.0 P2.1 P2.2 P2.3
PSB接低电平，选择串行收发送数据、命令
*************************************************************************/
#include  "msp430g2553.h"
#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long

#define SID_1  P2OUT |= BIT1       //数据、指令信号（R/W），P2.1输出，置1
#define SID_0  P2OUT &= ~BIT1      //置0
#define SCLK_1 P2OUT |= BIT2       //时钟信号（E）.P2.2输出。置1
#define SCLK_0 P2OUT &= ~BIT2      //置0
#define CS_1   P2OUT |= BIT0       //片选信号（RS）P2.0输出。置1
#define CS_0   P2OUT &= ~BIT0      //置0
#define RST_1  P2OUT |= BIT3       //复位信号，P2.3输出，RESET=1 ,置位LCD12864
#define RST_0  P2OUT &= ~BIT0      //RESET=0 ,复位LCD12864
const uchar display[] = {
"温度："
"℃"
"湿度："
"甲醛："
"甲烷："
"CO:"
"PM2.5:"

};
/**********************n (毫秒)延时函数 delay（n）******************************/
void delay(uint nms)
{
  while(nms>0)                     //循环条件
  {  __delay_cycles(1000);         //延时1000次为1毫秒
     --nms;                        //自减一
  }
}

/*******************LCD12864连接端口初始化函数LCDPort_Init**********************/
void LCDPort_Init()
{
  P2SEL &= ~(BIT0+BIT1+BIT2+BIT3); //关闭P2.0、P2.1、P2.2、P2.3的第二功能;
  P2DIR |= BIT0+BIT1+BIT2+BIT3;    //P2.0 P2.1 P2.2 P2.3置1, 1为输出
  P2OUT = 0xff;                    //全出高电平
}
/*************************************************************************
*名    称：LCD_Write_cmd()
*功    能：写一个命令到LCD12864
*入口参数：command:待写入的命令，无符号字节形式
*出口参数：无
*说    明：写入命令时，RW=0，RS=0 扩展成24位串行发送
*格    式：11111 RW RS 0   xxxx0000    xxxx0000    串行时序图
*          |最高的字节  |命令的bit7~4|命令的bit3~0|
*范例：LCD_Write_cmd(0x01)，清屏显示  ，
*************************************************************************/
void LCD_Write_cmd(uchar cmd)
{
  uchar i,high4bits,low4bits;
  ulong lcdcmd;
  high4bits = cmd & 0xf0;            //高4位保持，低四位置0，取指令的高四位，××××0000
  low4bits = (cmd<<4) & 0xf0;        //指令左移四位，在与11110000，取指令低四位，××××0000
  lcdcmd=((ulong)0xf8<<16)+((ulong)high4bits<<8)+(ulong)low4bits;
  //11111000 00000000 00000000  + ××××0000 00000000 + ××××0000 形成24位LCD12864指令
  //11111ABC,A为数据传输方向，0为写，1为读；B为数据类型选择，1为显示的数据，0为控制命令
  CS_1;                              //片选信号，高电平有效
  SCLK_0;                            //同步时钟信号
  for(i=0;i<24;i++)
  {
    SID_0;                           //数据口，起始为0
    if(lcdcmd & 0x00800000) SID_1;   //第24位，为1时，数据信号置1，从P2.1口，生成对应的控制信号
    lcdcmd <<=1;                     //控制信号左移一位
    SCLK_1;                          //时钟信号置1
    _NOP();                          //停顿下，
    SCLK_0;                          //时钟信号翻转为0
  }
  CS_0;                              //生成24位控制命令后片选低电平，关能。
}
void Write_cmd(uchar cmd)            //写命令到LCD12864
{ LCD_Write_cmd(cmd);
}
/************************************************************************
*名    称：LCD_Write_Byte()
*功    能：向LCD12864写入一个字节数据
*入口参数：byte:待写入的字符，无符号形式
*出口参数：无
*范    例：LCD_Write_Byte('F') //写入字符'F'
*************************************************************************/
void LCD_Write_Byte(uchar byte)
{
  uchar i,high4bits,low4bits;
  ulong lcddat;
  //while(IsBusy());
  high4bits = byte & 0xf0;            //字节高四位    ××××0000
  low4bits = (byte<<4)&0xf0;          //字节第四位   ××××0000
  lcddat=((ulong)0xfa<<16)+((ulong)high4bits<<8)+(ulong)low4bits;
  //11111010 00000000 00000000  + ××××0000 00000000 + ××××0000 形成24位LCD12864指令
  //11111ABC,A为数据传输方向，0为写，1为读；B为数据类型选择，1为显示的数据，0为控制命令
  CS_1;
  for(i=0;i<24;i++)
  {                                   //片选高电平，有效
    SID_0;                            //数据口，信号起始位为0
    if(lcddat & 0x00800000) SID_1;    //如果第24位为1，数据位置1
    lcddat <<=1;                      //控制信号左移一位，为判断第23位做准备
    SCLK_1;                           //时钟信号置1
    _NOP();                           //停顿下，
    SCLK_0;                           //时钟信号置0
  }
  CS_0;                               //生成24位数据信号后，片选信号低电平，关能。
}
void Write_data(uchar dat)            //写一个字符到LCD12864
{ LCD_Write_Byte(dat);
}
void Write_char(uchar ch)             //写一个字符到LCD12864
{ LCD_Write_Byte(ch);
}

/****************************LCD12864初始化函数*******************************/
void LCD_Init()
{
  RST_0;                             //复位LCD12864
  delay(50);                         //延时50ms
  RST_1;                             //置位LCD12864
  delay(50);                         //再延时50ms

  LCD_Write_cmd(0x30);               //地址计数器清零，游标归位，开显示
  delay(15);                         //延时15ms，等待执行完毕

  LCD_Write_cmd(0x30);               //二次显示模式，不用判忙
  delay(15);                         //延时15ms

  LCD_Write_cmd(0x0c);               //开显示及光标位置
  __delay_cycles(150);               //150us

  LCD_Write_cmd(0x01);               //清屏显示
  delay(15);                         //延时15ms

  LCD_Write_cmd(0x06);               //进入点设定，显示光标位置移动
  __delay_cycles(150);               //150us

  LCD_Write_cmd(0x02);               //地址计数器清零，游标归位，开显示
  delay(15);                         //延时15ms

  LCD_Write_cmd(0x80);               //第一行第一列显示
  __delay_cycles(10);                //150us
}
/***********************************************************
*名    称：LCD_pos()              of site
*功    能：设置液晶的显示位置
*入口参数：x:第几行，1~4对应第1行~第4行
*          y:第几列，0~15对应第1列~第16列
*出口参数：无
*范    例：LCD_pos(2,3) //第二行，第四列
***********************************************************/
void LCD_pos(uchar x,uchar y)        //x范围：1~4;   y范围：0~7
{
  uchar pos;                         //地址坐标
  switch(x)                          //横坐标
  {
    case 1:pos=0x80;break;           //第一行起始地址
    case 2:pos=0x90;break;           //第二行
    case 3:pos=0x88;break;           //第三行
    case 4:pos=0x98;break;           //第四行
    default:pos=0x80;                //错误的话，默认第一行
  }
  pos += y;                          //site（位置）=行坐标+纵坐标
  LCD_Write_cmd(pos);                //写地址
}
void Write_pos(uchar x,uchar y)      //写地址 等效LCD_pos()
{ LCD_pos(x,y);
}
void Set_pos(uchar x,uchar y)        //x,1~4,  y,1~8, 按自然习惯设置行列位置
{ LCD_pos(x,y-1);
}
/***********************************************************
*名    称：LCD_flash()
*功    能：LCD闪烁函数
*入口参数：times:屏幕闪烁的次数 1~255次
*出口参数：无
*说    明：闪烁周期为800ms左右
***********************************************************/
void LCD_flash(uchar times)
{
  while(times>0)
  {
    LCD_Write_cmd(0x08);             //整体显示关
    delay(400);                      //延时400毫秒
    LCD_Write_cmd(0x0C);             //整体显示开
    delay(400);                      //延时400毫秒
    --times;                         //闪烁此时时间自减1
  }
}
/************************************************************************
*名    称：LCD_Write_Date()
*功    能：向LCD12864写入一个数字：0～9；对应代码0×30～0×39
*入口参数：Date:待写入的数字，无符号形式，0000××××
*出口参数：无
*范    例：LCD_Write_Date(5)                        //写入数字5
*************************************************************************/
void Write_Data(uchar dat)                     //写一个数字到LCD12864
{
  switch(dat)                                    //横坐标
  {
    case 0:LCD_Write_Byte(0x30);break;         //写0
    case 1:LCD_Write_Byte(0x31);break;         //写1
    case 2:LCD_Write_Byte(0x32);break;         //写2
    case 3:LCD_Write_Byte(0x33);break;         //写3
    case 4:LCD_Write_Byte(0x34);break;         //写4
    case 5:LCD_Write_Byte(0x35);break;         //写5
    case 6:LCD_Write_Byte(0x36);break;         //写6
    case 7:LCD_Write_Byte(0x37);break;         //写7
    case 8:LCD_Write_Byte(0x38);break;         //写8
    case 9:LCD_Write_Byte(0x39);break;         //写9
    default:LCD_Write_cmd(0x80);               //错误的话，默认第一行
  }
}

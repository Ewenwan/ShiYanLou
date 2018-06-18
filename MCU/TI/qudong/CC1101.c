#include "CC1101.h"
#include "msp430g2553.h"
//******************************************************************************
/*****更多功率参数设置可详细参考DATACC1100英文文档中第48-49页的参数表**********/
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04}; //-30dBm 功率最小
//INT8U PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};   //0dBm
//INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0}; //10dBm 功率最大

//=======================定义RF1100-1101配置寄存器结构体数组 ============================
//10, 7, 5, 0, -5, -10, -15, -20, dbm output power, 0x12 == -30dbm
INT8U PaTabel[8] = { 0xc0, 0xC8, 0x84, 0x60, 0x68, 0x34, 0x1D, 0x0E};

// Sync word qualifier mode = 30/32 sync word bits detected 
// CRC autoflush = false 
// Channel spacing = 199.951172 
// Data format = Normal mode 
// Data rate = 2.00224 
// RX filter BW = 58.035714 
// PA ramping = false 
// Preamble count = 4 
// Whitening = false 
// Address config = No address check 
// Carrier frequency = 400.199890 
// Device address = 0 
// TX power = 10 
// Manchester enable = false 
// CRC enable = true 
// Deviation = 5.157471 
// Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Packet length = 255 
// Modulation format = GFSK 
// Base frequency = 399.999939 
// Modulated = true 
// Channel number = 1 
// PA table 
//#define PA_TABLE {0xc2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}
/**************CC1101寄存器配置字*****************************/
static const INT8U XW_CC1101InitData[22][2]= 
{
  {CC1101_IOCFG0,      0x06},
  {CC1101_FIFOTHR,     0x47},
  {CC1101_PKTCTRL0,    0x05},   //数据包自动控制
  {CC1101_CHANNR,      0x01},   //通道号
  {CC1101_FSCTRL1,     0x06},   //频率合成器 控制器
  {CC1101_FREQ2,       0x0F},   //频率合成控制字，高字节
  {CC1101_FREQ1,       0x62},
  {CC1101_FREQ0,       0x76},
  {CC1101_MDMCFG4,     0xF6},   //解调器配置
  {CC1101_MDMCFG3,     0x43},
  {CC1101_MDMCFG2,     0x13},
  {CC1101_DEVIATN,     0x15},   //调制器背离设置
  {CC1101_MCSM0,       0x18},   //主通信控制状态机配置
  {CC1101_FOCCFG,      0x16},   //频率偏移补偿配置
  {CC1101_WORCTRL,     0xFB},
  {CC1101_FSCAL3,      0xE9},   //频率合成器校准
  {CC1101_FSCAL2,      0x2A},
  {CC1101_FSCAL1,      0x00},
  {CC1101_FSCAL0,      0x1F},
  {CC1101_TEST2,       0x81},   //多种测试设置
  {CC1101_TEST1,       0x35},
  {CC1101_MCSM1,       0x3B},   //主通信控制状态机配置
};

/***************************************************************
 * 函数名：INT8U MNSPI_ExchangeByte(INT8U dat)
 * 输入：发送的数据
 * 输出：接收到的状态信息
 *　功能描述：SPI发送一个字节，并接收一个字节的状态信息   //基础SPI读写函数
****************************************************************/
INT8U MNSPI_ExchangeByte(INT8U dat)
{
//----------------------以下是模拟SPI时序方式-----------------------------------
    INT8U i,temp;
    temp = 0;
    SCK_0 ;                   //时钟信号，低电平
    for(i=0; i<8; i++)
    {
        if(dat & 0x80)       //判断数据最高位是1或是0
        {
            MOSI_1;          //MOSI发出高电平
        }
        else
        {
            MOSI_0;          //MOSI_0发出低电平
        }
        dat <<= 1;           //数据左移一位    
        SCK_1;               //时钟信号高电平
        temp <<= 1;          //读取的数据
        if(MISO_IN)   
              temp |= 0x01;  //读取MISO状态
        else
              temp &= 0xfe;
        SCK_0 ;              //时钟信号低电平  
    }
    return temp;             //接收到的一个字节的状态信息
}

/*初始化CC1101的WOR功能*/
void  XW_CC1101WORInit( void )
{

    XW_CC1101WriteReg(CC1101_MCSM0,0x18);
    XW_CC1101WriteReg(CC1101_WORCTRL,0x78); //Wake On Radio Control
    XW_CC1101WriteReg(CC1101_MCSM2,0x00);
    XW_CC1101WriteReg(CC1101_WOREVT1,0x8C);
    XW_CC1101WriteReg(CC1101_WOREVT0,0xA0);	
    XW_CC1101WriteCmd( CC1101_SWORRST );
}


/*******************************************************************
 * 函数名：INT8U XW_CC1101ReadReg(INT8U addr)
 * 输入：地址
 * 输出：该寄存器的配置字
 * 功能描述：读寄存器
********************************************************************/
INT8U XW_CC1101ReadReg(INT8U addr)
{
    unsigned char j = 0;
    INT8U value;
    CSN_0;                                    //使能
    do
    {
        j++;
        if(j == 200) return MISO_ERROR;
    }while(MISO_IN);                          //读取MISO口状态,电平为低电平时  开始写

    MNSPI_ExchangeByte(addr | READ_SINGLE);   //写入地址和读寄存器命令
    value = MNSPI_ExchangeByte(0xFF);                //读出寄存器配置字  //主出从入   //0x00
    CSN_1;                                    //关能
    return value;                             //返回寄存器配置字
}
/**********************************************************************
 * 函数名：XW_CC1101ReadMultiReg((INT8U addr, INT8U *buffer, INT8U size)
 * 输入：地址，读出数据后暂存的缓冲区，读出配置个数
 * 输出：无
 * 功能描述：SPI连续读配置寄存器
***********************************************************************/
void XW_CC1101ReadMultiReg(INT8U addr, INT8U *buffer, INT8U size)
{
    unsigned char j = 0;
    INT8U i,k;
    CSN_0;                                 //使能

    do
    {
    j++;
    if(200 == j) return;
    }while(MISO_IN);                       //读取MISO口状态，电平为低电平时  开始写

    MNSPI_ExchangeByte(addr | READ_BURST); //写入要读的配置寄存器地址和读命令
    for (i = 0; i < size; i++)
    {
      for( k = 0; k < 20; k ++ );          //等
     buffer[i] = MNSPI_ExchangeByte(0xFF); //读寄存器配置字  后暂存的缓冲区  //主出从入
    }
    CSN_1;                                 //关能
}

/*****************************************************************
 * 函数名：XW_CC1101ReadStatus(INT8U addr)
 * 输入：该寄存器的地址
 * 输出：该状态寄存器当前值
 * 功能描述：读状态寄存器的值
******************************************************************/
INT8U XW_CC1101ReadStatus(INT8U addr)
{
    unsigned char j = 0;
    INT8U value;
    CSN_0;                                 //使能

    do
    {
        j++;
        if(200 == j) return MISO_ERROR;
    }while(MISO_IN);                      //读取MISO口状态，电平为低电平时  开始写

    MNSPI_ExchangeByte(addr | READ_BURST);//写入要读的状态寄存器的地址和读命令
    value = MNSPI_ExchangeByte(0xFF);     //读状态寄存器的值
    CSN_1;                                //关能
    return value;                         //返回状态寄存器的值
}
/*****************************************************************************
 * 函数名：XW_CC1101WriteReg(INT8U addr, INT8U value)
 * 输入：地址和配置字
 * 功能描述：给寄存器写一个字节的配置字
******************************************************************************/
void XW_CC1101WriteReg(INT8U addr, INT8U value)
{
    unsigned char j = 0;
    CSN_0;                    //使能

    do
    {
        j++;
        if(200 == j) return;
    }while(MISO_IN);          //读取MISO状态,电平为低电平时   开始写

    MNSPI_ExchangeByte(addr); //写地址
    MNSPI_ExchangeByte(value);//写入配置
    CSN_1;                    //关能
}
/**********************************************************************
 * 函数名：XW_CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size )
 * 输入：地址，写入缓冲区，写入个数
 * 功能描述：SPI连续写配置寄存器
***********************************************************************/
void XW_CC1101WriteMultiReg( INT8U addr, INT8U *buffer, INT8U size )
{
    unsigned char j = 0;
    INT8U i,k;
    CSN_0;                                 //使能

    do
    {
        j++;
        if(200 == j) return;
    }while(MISO_IN);                       //读取MISO状态,电平为低电平时  开始写

   MNSPI_ExchangeByte(addr | WRITE_BURST); //写入地址和连续写入指令
    for (i = 0; i < size ; i++)
    {
      for( k = 0; k < 10; k ++ );          //等  添加的
      MNSPI_ExchangeByte(buffer[i]);       //写入缓冲区数据
    }
    CSN_1;                                 //关能
}
/******************************************************************
 * 函数名：XW_CC1101WriteCmd( INT8U command )
 * 输入：命令
 * 输出：无
 * 功能描述：SPI写命令
*******************************************************************/
void XW_CC1101WriteCmd( INT8U command )
{
    unsigned char j = 0;
    CSN_0;                       //使能

    do
    {
        j++;
        if(200 == j) return;
    }while(MISO_IN);             //读取MISO状态 p5.1 口电平为低电平时  开始写

    MNSPI_ExchangeByte(command); //写入命令
    CSN_1;                       //关能
}
/*****************************************************************
 * 函数名：XW_CC1101SetTRMode( TRMODE mode )
 * 输入：该寄存器的地址
 * 输出：该状态寄存器当前值
 * 功能描述：读状态寄存器的值
******************************************************************/
void XW_CC1101SetTRMode( TRMODE mode )
{
    if( mode == TX_MODE )                  //设置为发送模式
    {
        XW_CC1101WriteReg(CC1101_IOCFG0,0x46);
        XW_CC1101WriteCmd( CC1101_STX );
    }
    else if( mode == RX_MODE )             //设置为接收模式
    {
        XW_CC1101WriteReg(CC1101_IOCFG0,0x46);
        XW_CC1101WriteCmd( CC1101_SRX );
    }
}

/****************************************************************
 * 函数名：void XW_CC1101RESET(void)
 *　输入：无
 *　输出：无
 * 功能描述：复位CC1100
****************************************************************/
void XW_CC1101RESET(void)
{
    INT8U t;
    CSN_1;
    CSN_0;
    CSN_1;
    for( t = 0; t < 100; t ++ );
    XW_CC1101WriteCmd(CC1101_SRES);      //写入复位命令
}
/****************************************************************
 * 函数名：XW_CC1101SetIdleMODE( void )
 * 功能描述：设置CC1101为IDLE模式
****************************************************************/
void XW_CC1101SetIdleMODE( void )
{
    XW_CC1101WriteCmd(CC1101_SIDLE);
}
/****************************************************************
 * 函数名：XW_CC1101ClrTXBuff( void )
 * 功能描述：Q清空CC1100发送缓存区，强制进入空闲模式
****************************************************************/
void XW_CC1101ClrTXBuff( void )
{
    XW_CC1101SetIdleMODE();                  //CC1101必须在IDLE模式下
    XW_CC1101WriteCmd( CC1101_SFTX );
}
/****************************************************************
 * 函数名：XW_CC1101ClrRXBuff( void )
 * 功能描述：Q清空CC1100接送缓存区，强制进入空闲模式
****************************************************************/
void XW_CC1101ClrRXBuff( void )
{
    XW_CC1101SetIdleMODE();                  //CC1101必须在IDLE模式下
    XW_CC1101WriteCmd( CC1101_SFRX );
}
/****************************************************************
 * 函数名：XW_CC1101SetAddress( INT8U address, ADDR_MODE AddressMode)
 * 功能描述：设置CC1101地址
****************************************************************/
void XW_CC1101SetAddress( INT8U address, ADDR_MODE AddressMode)
{
    INT8U btmp =XW_CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03;
    XW_CC1101WriteReg(CC1101_ADDR, address);
    if     ( AddressMode == BROAD_ALL )     {}
    else if( AddressMode == BROAD_NO  )     { btmp |= 0x01; }
    else if( AddressMode == BROAD_0   )     { btmp |= 0x02; }
    else if( AddressMode == BROAD_0AND255 ) { btmp |= 0x03; }   
}
/****************************************************************
 * 函数名：XW_CC1101SetSYNC( INT16U sync )
 * 功能描述：设置CC1101 SYNC 控制字
****************************************************************/
void XW_CC1101SetSYNC( INT16U sync )
{
    XW_CC1101WriteReg(CC1101_SYNC1, 0xFF & ( sync>>8 ) );//写高八位
    XW_CC1101WriteReg(CC1101_SYNC0, 0xFF & sync );       //写低八位
}

/****************************************************************
 * 函数名：XW_CC1101ClrRXBuff( void )
 * 功能描述：Q清空CC1100接送缓存区，强制进入空闲模式
****************************************************************/

void XW_CC1101Init(void)
{
    INT8U i;
    XW_CC1101RESET();                                    //CC1101复位
    for( i = 0; i < 22; i++ )
    {
        XW_CC1101WriteReg( XW_CC1101InitData[i][0], XW_CC1101InitData[i][1] );
    }
    XW_CC1101SetAddress( 0x05, BROAD_0AND255 );          //设置地址
    XW_CC1101SetSYNC( 0x8799 );                          //设置SYNC控制字
    XW_CC1101WriteReg(CC1101_MDMCFG1,   0x72);           //Modem Configuration
    XW_CC1101WriteMultiReg(CC1101_PATABLE, PaTabel, 8 ); //设置功耗模式
    i = XW_CC1101ReadStatus( CC1101_PARTNUM );//for test, must be 0x80
    i = XW_CC1101ReadStatus( CC1101_VERSION );//for test, refer to the datasheet
}
/****************************************************************
 * 函数名：INT8U XW_CC1101GetRXCount( void )
 * 功能描述：得到接收的字节数
****************************************************************/
INT8U XW_CC1101GetRXCount( void )
{
    return ( XW_CC1101ReadStatus( CC1101_RXBYTES )  & BYTES_IN_RXFIFO );
}


/**********************************************************************
 * 函数名：XW_CC1101SendPacket( INT8U *txBuffer, INT8U size, TX_DATA_MODE mode )
 * 输入：发送的缓冲区，发送数据个数
 * 功能描述：CC1100发送一组数据
***********************************************************************/
void XW_CC1101SendPacket( INT8U *txBuffer, INT8U size, TX_DATA_MODE mode )
{
    INT8U address;
    if( mode == BROADCAST )          { address = 0; }
    else if( mode == ADDRESS_CHECK ) { address = XW_CC1101ReadReg( CC1101_ADDR ); } //得到准确地址

    XW_CC1101ClrTXBuff( );            //清空发送缓冲区
    
    if( ( XW_CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 ) != 0 )
    {
        XW_CC1101WriteReg( CC1101_TXFIFO, size + 1 );
        XW_CC1101WriteReg( CC1101_TXFIFO, address );
    }
    else
    {
        XW_CC1101WriteReg( CC1101_TXFIFO, size );
    }

    XW_CC1101WriteMultiReg( CC1101_TXFIFO, txBuffer, size );  //发送数据
    XW_CC1101SetTRMode( TX_MODE );
    while(!(GDO0_IN));             //等待GDO0为高电平，GDO0被设定为同步传输模式，置高表示数据报头已经发送
    while(GDO0_IN);                //等待GDO0为低电平，GDO0清除，包结束信号，置低表示数据包已经发送完成
    XW_CC1101ClrTXBuff( );         //清空发送缓冲区
}


/**************************************************************
 * 函数名：XW_CC1101RecPacket( INT8U *rxBuffer )
 * 输入：接收的缓冲区
 * 输出：正确返回数据或接受错误返回0
 * 功能描述：CC1100接收一组数据
***************************************************************/
INT8U XW_CC1101RecPacket( INT8U *rxBuffer )
  {
    INT8U status[2];           //存储CRC校验码数值
    INT8U pktLen;              //有效数据包长度
    INT16U x=0;

    if (XW_CC1101GetRXCount()!=0)                   //如果接的字节数不为0
    {
        pktLen = XW_CC1101ReadReg(CC1101_RXFIFO);;//读出数据长度字
        if((XW_CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03 )!= 0)   //地址校验并且广播地址为0x00和0xFF
         {
            x=XW_CC1101ReadReg(CC1101_RXFIFO);    //读出数据包中的地址
         }
        if( pktLen == 0 )           { return 0; }
            else                    { pktLen --; }
        XW_CC1101ReadMultiReg(CC1101_RXFIFO, rxBuffer, pktLen); // Pull data
        XW_CC1101ReadMultiReg(CC1101_RXFIFO, status, 2);        // Read  status bytes
        XW_CC1101ClrRXBuff( );

        if( status[1] & CRC_OK ) {   return pktLen; }
        else                     {   return 0; }
    }
    else { return 0;  }                                          //接收数据长度为0
  }


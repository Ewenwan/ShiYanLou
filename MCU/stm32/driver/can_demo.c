#include "sysdef.h"

#define MAX_MAIL_NUM  3

//CAN总线调试:0=运行 1=自环调试
#define CAN_DEBUG     0

//CAN总线波特率:0=250kbps,1=500kbps,2=1Mbps
#define CAN1_BPS      0
//地址 
unsigned char can1_addr = 0;
//收发计数 
unsigned short  Can1_Tx_Count =0;
unsigned short  Can1_Rx_Count =0;
//发送延迟 
unsigned short  Can1_Send_Delay =0;
//收发数据缓冲区
unsigned char   Can1_Send_Buf[10]={0xeb,0x90,0x01,0x55,0xAA};
unsigned char   Can1_Recv_Buf[10]={0};

extern int  angle_num ; //圈数
extern unsigned int  angle_data ;//当前脉冲数 

static  u8 CAN_msg_num[MAX_MAIL_NUM];   // 发送邮箱标记  日志 记录

void CAN1_Config_init(void)
{
	// 初始化结构体 
     CAN_InitTypeDef        CAN_InitStructure;
     CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    // 默认值初始化 
     CAN_DeInit(CAN1);
     CAN_StructInit(&CAN_InitStructure);
  
     /* CAN cell init */ //36MHz 500Kbps
     CAN_InitStructure.CAN_TTCM=DISABLE;//禁止时间触发通信模式
     CAN_InitStructure.CAN_ABOM=DISABLE;//软件对CAN_MCR寄存器的INRQ位进行置1随后清0后,一旦硬件检测
                                        //到128次11位连续的隐性位,就退出离线状态
     CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过清除CAN_MCR寄存器的SLEEP位,由软件唤醒
     CAN_InitStructure.CAN_NART=DISABLE;//CAN报文是否只发1次,不管发送的结果如何(成功/出错或仲裁丢失)
     CAN_InitStructure.CAN_RFLM=DISABLE;//在接收到溢出时FIFO未被锁定,当接收到FIFO报文未被读出,下一个收到的报文会覆盖原有的报文
     CAN_InitStructure.CAN_TXFP=DISABLE;//发送的FIFO优先级由报文的标识符来决定
      
#if CAN_DEBUG
     CAN_InitStructure.CAN_Mode= CAN_Mode_LoopBack;
#else
     CAN_InitStructure.CAN_Mode= CAN_Mode_Normal; 
#endif

     //传输波特率

     if(CAN1_BPS ==  0)
     {
        CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//重新同步跳跃宽度1个时间单位
        CAN_InitStructure.CAN_BS1=CAN_BS1_12tq;//时间段1为12个时间单位
        CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;//时间段2为3个时间单位
        CAN_InitStructure.CAN_Prescaler= 9;//36M/(1+12+3)/9= 250kbps
                                            //36M/(1+5+2)/9 = 500kbps
                                           //36M(1+2+1)/9 = 1M
     }
     else if(CAN1_BPS == 1)
     {
        CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//重新同步跳跃宽度1个时间单位
        CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;//时间段1为5个时间单位
        CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;//时间段2为2个时间单位
        CAN_InitStructure.CAN_Prescaler= 9; //36M/(1+5+2)/9 = 500kbps
                                           
        
     }
     else
     {
        CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//重新同步跳跃宽度1个时间单位
        CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;//时间段1为2个时间单位
        CAN_InitStructure.CAN_BS2=CAN_BS2_1tq;//时间段2为1个时间单位
        CAN_InitStructure.CAN_Prescaler= 9;//36M(1+2+1)/9 = 1M
         
     }
     CAN_Init(CAN1,&CAN_InitStructure); 
                                         
    // can 过滤器 初始化  
     CAN_FilterInitStructure.CAN_FilterNumber=0;                    //指定了待初始化的过滤器0
     CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;  //指定了过滤器将被初始化到的模式为标识符屏蔽位模式
     CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //给出了过滤器位宽1个32位过滤器 
     CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;               //用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）
     CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;                //用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个)
     CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;           //用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个)
     CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;            //用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个)
     CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;    //设定了指向过滤器的FIFO0 
     CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;           //使能过滤器
     CAN_FilterInit(&CAN_FilterInitStructure);

}

// ***************************************************************
//       BaudRate = 1 / NominalBitTime
//       NominalBitTime = 1tq + tBS1 + tBS2
//       tq = (BRP[9:0] + 1) x tPCLK
//       tPCLK = CAN's clock = APB1's clock
//   1Mbps 速率下，采用点的位置在6tq位置处，BS1=5, BS2=2
//   500kbps 速率下，采用点的位置在8tq位置处，BS1=7, BS2=3
//   250kbps 速率下，采用点的位置在14tq位置处，BS1=13, BS2=2
//   125k, 100k, 50k, 20k, 10k 的采用点位置与 250K 相同
// ****************************************************************

void  CAN1_Com_init(void)
{
	// 初始化 结构体参数 
      GPIO_InitTypeDef GPIO_InitStruct;
      NVIC_InitTypeDef NVIC_InitStructure;
      
      /* 使能can 中断 */
      NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
       
      NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
       
      NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
       
  	// 外设时钟 使能     
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
      
    // Can 接收端口配置 
      GPIO_InitStruct.GPIO_Pin  =  GPIO_Pin_11;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IPU;
      GPIO_Init(GPIOA,&GPIO_InitStruct);
      
    //Can 发送端口配置 
      GPIO_InitStruct.GPIO_Pin  =  GPIO_Pin_12;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF_PP;
      GPIO_Init(GPIOA,&GPIO_InitStruct);
       
      CAN1_Config_init();
      
   // 接收 中断    FIFO0 \FIFO1中断 
      CAN_ITConfig(CAN1,CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);  // fifo0中断
      CAN_ITConfig(CAN1,CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, ENABLE);  // fifo1中断
   // 发送 中断 
      CAN_ITConfig(CAN1,CAN_IT_TME, DISABLE);                             // 发送中断
      CAN_ITConfig(CAN1,CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC | CAN_IT_ERR | CAN_IT_WKU | CAN_IT_SLK, ENABLE);  // ERR中断

      // CAN缓存初始化
      memset(CAN_msg_num,0,MAX_MAIL_NUM);   
}

// 发送 成功与否 的 日记记录 
int CAN1_Tx_msg(CanTxMsg TxMessage)
{
    u8 TransmitMailbox = 0;
  
    TransmitMailbox = CAN_Transmit(CAN1,&TxMessage);
    if(CAN_NO_MB == TransmitMailbox)
    {
        //发送失败
        return 0;
    }
    else
    {         
        CAN_msg_num[TransmitMailbox] = 1;
       
    }
    CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE);
     
    return 1;
}


// can 发消息 
u16 angle=0,angle_h=0,angle_l=0;
extern unsigned char angle_dir;
int CAN1_Tx_data(void)
{
    CanTxMsg TxMessage;
    u8 TransmitMailbox = 0,i=0;
    
    /* transmit */
    TxMessage.StdId=0x6f1;     //设定标准标识符  (标识) 
    TxMessage.ExtId=0x1234;    //设置扩展标识符
    TxMessage.RTR=CAN_RTR_DATA;//设定待传输消息的帧类型
    TxMessage.IDE=CAN_ID_STD;  //设定消息标识符的类型
    TxMessage.DLC=6;           //数据长度
     
    angle=angle_data/10;
    if(angle>9999) angle=9999;
    angle_h=angle/100;
    angle_h=angle_h/10*16+angle_h%10;
    angle_l=angle%100;
    angle_l=angle_l/10*16+angle_l%10;
    Can1_Send_Buf[0]=angle_num;  //圈数
    Can1_Send_Buf[1]=angle_h;    //总角度
    Can1_Send_Buf[2]=angle_l;
    Can1_Send_Buf[3]=0;
    Can1_Send_Buf[4]=0;
    Can1_Send_Buf[5]=angle_dir;
     
    for(i=0;i < TxMessage.DLC;i++)
    {
       TxMessage.Data[i] = Can1_Send_Buf[i];
    }
     
    TransmitMailbox = CAN_Transmit(CAN1,&TxMessage);
    if(CAN_NO_MB == TransmitMailbox)
    {
        //发送失败,没有空邮箱
        return 0;
    }
    else
    {         
        CAN_msg_num[TransmitMailbox] = 1;   
    }
    CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE);
     
    Can1_Tx_Count++;
    if(Can1_Tx_Count > 10000)
       Can1_Tx_Count =0;
     
    Can1_Send_Delay =200;
    return 1;
}

//can收数据 解析数据
void  CAN1_Rx_Data(CanRxMsg RxMessage)
{     
      u8  i =0;
      if((RxMessage.StdId==0x6f1) && (RxMessage.IDE==CAN_ID_STD) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xEB90))
      {
           
          for(i=0;i < RxMessage.DLC;i++)
          {
              Can1_Recv_Buf[i] =  RxMessage.Data[i];
          }
         
          Can1_Rx_Count++;
          if(Can1_Rx_Count > 10000)
             Can1_Rx_Count =0;
      }
}


//发送完中断函数
void CAN1_Send(void)
{
     if(CAN_msg_num[0])// 日志消息记录 发送成功 
     {
        if(CAN_GetITStatus(CAN1,CAN_IT_RQCP0))
        {
            CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP0);
            CAN_ITConfig(CAN1,CAN_IT_TME, DISABLE);
            CAN_msg_num[0] = 0;
        }
     }
     if(CAN_msg_num[1])
     {
        if(CAN_GetITStatus(CAN1,CAN_IT_RQCP1))
        {
            CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP1);
            CAN_ITConfig(CAN1,CAN_IT_TME, DISABLE);
            CAN_msg_num[1] = 0;
        }
     }  
    if(CAN_msg_num[2])
    {
        if(CAN_GetITStatus(CAN1,CAN_IT_RQCP2))
        {
            CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP2);
            CAN_ITConfig(CAN1,CAN_IT_TME, DISABLE);
            CAN_msg_num[2] = 0;
        }
    }
}


//接收中断函数
// CAN过滤器FIFO关联寄存器（CAN_FFA1R） 的 FFAx   对应位为0，则存放到FIFO0；如果为1，则存放到FIFO1
void CAN1_Recv(unsigned char num)
{
     CanRxMsg RxMessage;
     switch(num)
     {
     case 0:
          if(CAN_GetITStatus(CAN1,CAN_IT_FF0))
          {
             CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
          }
          else if(CAN_GetITStatus(CAN1,CAN_IT_FOV0))
          {
             CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
          }
          else
          {
             CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
             //解析数据:
             CAN1_Rx_Data(RxMessage);
          } 
          break;
     case 1:
          if(CAN_GetITStatus(CAN1,CAN_IT_FF1))
          {
             CAN_ClearITPendingBit(CAN1,CAN_IT_FF1);
          }
          else if(CAN_GetITStatus(CAN1,CAN_IT_FOV1))
          {
             CAN_ClearITPendingBit(CAN1,CAN_IT_FOV1);
          }
          else
          {
             CAN_Receive(CAN1,CAN_FIFO1, &RxMessage);
            //解析数据
             CAN1_Rx_Data(RxMessage);
          } 
          break;
     } 
}



void  CAN1_Main(unsigned char flg )
{
      if(flg)
      {
        if(Can1_Send_Delay == 0)
           CAN1_Tx_data();
      }
}

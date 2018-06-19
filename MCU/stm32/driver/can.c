#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

	    
//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:1~3;
//tbs2:时间段2的时间单元.范围:1~8;
//tbs1:时间段1的时间单元.范围:1~16;
//brp :波特率分频器.范围:1~1024;(实际要加1,也就是1~1024) tq=(brp)*tpclk1
//注意以上参数任何一个都不能设为0,否则会乱.
//波特率=Fpclk1/((tbs1+tbs2+1)*brp);
//mode:0,普通模式;1,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
//则波特率为:36M/((8+7+1)*5)=450Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	u16 i=0;
 	if(tsjw==0||tbs2==0||tbs1==0||brp==0)return 1;
	tsjw-=1;//先减去1.再用于设置
	tbs2-=1;
	tbs1-=1;
	brp-=1;

	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	 
	GPIOA->CRH&=0XFFF00FFF; 
	GPIOA->CRH|=0X000B8000;//PA11 RX,PA12 TX推挽输出   	 
    GPIOA->ODR|=3<<11;
					    
	RCC->APB1ENR|=1<<25;//使能CAN时钟 CAN使用的是APB1的时钟(max:36M)
	CAN->MCR=0x0000;	//退出睡眠模式(同时设置所有位为0)
	CAN->MCR|=1<<0;		//请求CAN进入初始化模式
	while((CAN->MSR&1<<0)==0)
	{
		i++;
		if(i>100)return 2;//进入初始化模式失败
	}
	CAN->MCR|=0<<7;		//非时间触发通信模式
	CAN->MCR|=0<<6;		//软件自动离线管理
	CAN->MCR|=0<<5;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN->MCR|=1<<4;		//禁止报文自动传送
	CAN->MCR|=0<<3;		//报文不锁定,新的覆盖旧的
	CAN->MCR|=0<<2;		//优先级由报文标识符决定
	CAN->BTR=0x00000000;//清除原来的设置.
	CAN->BTR|=mode<<30;	//模式设置 0,普通模式;1,回环模式;
	CAN->BTR|=tsjw<<24; //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位
	CAN->BTR|=tbs2<<20; //Tbs2=tbs2+1个时间单位
	CAN->BTR|=tbs1<<16;	//Tbs1=tbs1+1个时间单位
	CAN->BTR|=brp<<0;  	//分频系数(Fdiv)为brp+1
						//波特率:Fpclk1/((Tbs1+Tbs2+1)*Fdiv)
	CAN->MCR&=~(1<<0);	//请求CAN退出初始化模式
	while((CAN->MSR&1<<0)==1)
	{
		i++;
		if(i>0XFFF0)return 3;//退出初始化模式失败
	}
	//过滤器初始化
	CAN->FMR|=1<<0;			//过滤器组工作在初始化模式
	CAN->FA1R&=~(1<<0);		//过滤器0不激活
	CAN->FS1R|=1<<0; 		//过滤器位宽为32位.
	CAN->FM1R|=0<<0;		//过滤器0工作在标识符屏蔽位模式
	CAN->FFA1R|=0<<0;		//过滤器0关联到FIFO0
	CAN->sFilterRegister[0].FR1=0X00000000;//32位ID
	CAN->sFilterRegister[0].FR2=0X00000000;//32位MASK
	CAN->FA1R|=1<<0;		//激活过滤器0
	CAN->FMR&=0<<0;			//过滤器组进入正常模式

#if CAN_RX0_INT_ENABLE
 	//使用中断接收
	CAN->IER|=1<<1;			//FIFO0消息挂号中断允许.	    
	MY_NVIC_Init(1,0,USB_LP_CAN_RX0_IRQChannel,2);//组2
#endif
	return 0;
}   
//id:标准ID(11位)/扩展ID(11位+18位)	    
//ide:0,标准帧;1,扩展帧
//rtr:0,数据帧;1,远程帧
//len:要发送的数据长度(固定为8个字节,在时间触发模式下,有效数据为6个字节)
//*dat:数据指针.
//返回值:0~3,邮箱编号.0XFF,无有效邮箱.
u8 Can_Tx_Msg(u32 id,u8 ide,u8 rtr,u8 len,u8 *dat)
{	   
	u8 mbox;	  
	if(CAN->TSR&(1<<26))mbox=0;			//邮箱0为空
	else if(CAN->TSR&(1<<27))mbox=1;	//邮箱1为空
	else if(CAN->TSR&(1<<28))mbox=2;	//邮箱2为空
	else return 0XFF;					//无空邮箱,无法发送 
	CAN->sTxMailBox[mbox].TIR=0;		//清除之前的设置
	if(ide==0)	//标准帧
	{
		id&=0x7ff;//取低11位stdid
		id<<=21;		  
	}else		//扩展帧
	{
		id&=0X1FFFFFFF;//取低32位extid
		id<<=3;									   
	}
	CAN->sTxMailBox[mbox].TIR|=id;		 
	CAN->sTxMailBox[mbox].TIR|=ide<<2;	  
	CAN->sTxMailBox[mbox].TIR|=rtr<<1;
	len&=0X0F;//得到低四位
	CAN->sTxMailBox[mbox].TDTR&=~(0X0000000F);
	CAN->sTxMailBox[mbox].TDTR|=len;		   //设置DLC.
	//待发送数据存入邮箱.
	CAN->sTxMailBox[mbox].TDHR=(((u32)dat[7]<<24)|
								((u32)dat[6]<<16)|
 								((u32)dat[5]<<8)|
								((u32)dat[4]));
	CAN->sTxMailBox[mbox].TDLR=(((u32)dat[3]<<24)|
								((u32)dat[2]<<16)|
 								((u32)dat[1]<<8)|
								((u32)dat[0]));
	CAN->sTxMailBox[mbox].TIR|=1<<0; //请求发送邮箱数据
	return mbox;
}
//获得发送状态.
//mbox:邮箱编号;
//返回值:发送状态. 0,挂起;0X05,发送失败;0X07,发送成功.
u8 Can_Tx_Staus(u8 mbox)
{	
	u8 sta=0;					    
	switch (mbox)
	{
		case 0: 
			sta |= CAN->TSR&(1<<0);			//RQCP0
			sta |= CAN->TSR&(1<<1);			//TXOK0
			sta |=((CAN->TSR&(1<<26))>>24);	//TME0
			break;
		case 1: 
			sta |= CAN->TSR&(1<<8)>>8;		//RQCP1
			sta |= CAN->TSR&(1<<9)>>8;		//TXOK1
			sta |=((CAN->TSR&(1<<27))>>25);	//TME1	   
			break;
		case 2: 
			sta |= CAN->TSR&(1<<16)>>16;	//RQCP2
			sta |= CAN->TSR&(1<<17)>>16;	//TXOK2
			sta |=((CAN->TSR&(1<<28))>>26);	//TME2
			break;
		default:
			sta=0X05;//邮箱号不对,肯定失败.
		break;
	}
	return sta;
} 
//得到在FIFO0/FIFO1中接收到的报文个数.
//fifox:0/1.FIFO编号;
//返回值:FIFO0/FIFO1中的报文个数.
u8 Can_Msg_Pend(u8 fifox)
{
	if(fifox==0)return CAN->RF0R&0x03; 
	else if(fifox==1)return CAN->RF1R&0x03; 
	else return 0;
}
//接收数据
//fifox:邮箱号
//id:标准ID(11位)/扩展ID(11位+18位)	    
//ide:0,标准帧;1,扩展帧
//rtr:0,数据帧;1,远程帧
//len:接收到的数据长度(固定为8个字节,在时间触发模式下,有效数据为6个字节)
//dat:数据缓存区
void Can_Rx_Msg(u8 fifox,u32 *id,u8 *ide,u8 *rtr,u8 *len,u8 *dat)
{	   
	*ide=CAN->sFIFOMailBox[fifox].RIR&0x04;//得到标识符选择位的值  
 	if(*ide==0)//标准标识符
	{
		*id=CAN->sFIFOMailBox[fifox].RIR>>21;
	}else	   //扩展标识符
	{
		*id=CAN->sFIFOMailBox[fifox].RIR>>3;
	}
	*rtr=CAN->sFIFOMailBox[fifox].RIR&0x02;	//得到远程发送请求值.
	*len=CAN->sFIFOMailBox[fifox].RDTR&0x0F;//得到DLC
 	//*fmi=(CAN->sFIFOMailBox[FIFONumber].RDTR>>8)&0xFF;//得到FMI
	//接收数据
	dat[0]=CAN->sFIFOMailBox[fifox].RDLR&0XFF;
	dat[1]=(CAN->sFIFOMailBox[fifox].RDLR>>8)&0XFF;
	dat[2]=(CAN->sFIFOMailBox[fifox].RDLR>>16)&0XFF;
	dat[3]=(CAN->sFIFOMailBox[fifox].RDLR>>24)&0XFF;    
	dat[4]=CAN->sFIFOMailBox[fifox].RDHR&0XFF;
	dat[5]=(CAN->sFIFOMailBox[fifox].RDHR>>8)&0XFF;
	dat[6]=(CAN->sFIFOMailBox[fifox].RDHR>>16)&0XFF;
	dat[7]=(CAN->sFIFOMailBox[fifox].RDHR>>24)&0XFF;    
  	if(fifox==0)CAN->RF0R|=0X20;//释放FIFO0邮箱
	else if(fifox==1)CAN->RF1R|=0X20;//释放FIFO1邮箱	 
}

#if CAN_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	u8 rxbuf[8];
	u32 id;
	u8 ide,rtr,len;     
 	Can_Rx_Msg(0,&id,&ide,&rtr,&len,rxbuf);
    printf("id:%d\r\n",id);
    printf("ide:%d\r\n",ide);
    printf("rtr:%d\r\n",rtr);
    printf("len:%d\r\n",len);
    printf("rxbuf[0]:%d\r\n",rxbuf[0]);
    printf("rxbuf[1]:%d\r\n",rxbuf[1]);
    printf("rxbuf[2]:%d\r\n",rxbuf[2]);
    printf("rxbuf[3]:%d\r\n",rxbuf[3]);
    printf("rxbuf[4]:%d\r\n",rxbuf[4]);
    printf("rxbuf[5]:%d\r\n",rxbuf[5]);
    printf("rxbuf[6]:%d\r\n",rxbuf[6]);
    printf("rxbuf[7]:%d\r\n",rxbuf[7]);
}
#endif

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;	  	 						       
    mbox=Can_Tx_Msg(0X12,0,0,len,msg);
	while((Can_Tx_Staus(mbox)!=0X07)&&(i<0XFFF))i++;//等待发送结束
	if(i>=0XFFF)return 1;							//发送失败?
	return 0;										//发送成功;
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
	u32 id;
	u8 ide,rtr,len; 
	if(Can_Msg_Pend(0)==0)return 0;			//没有接收到数据,直接退出 	 
  	Can_Rx_Msg(0,&id,&ide,&rtr,&len,buf); 	//读取数据
    if(id!=0x12||ide!=0||rtr!=0)len=0;		//接收错误	   
	return len;	
}



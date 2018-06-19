#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "control.h" 
 
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEKÕ½½¢STM32¿ª·¢°å
//CANÇı¶¯ ´úÂë	   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ĞŞ¸ÄÈÕÆÚ:2012/9/11
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//CAN³õÊ¼»¯
//tsjw:ÖØĞÂÍ¬²½ÌøÔ¾Ê±¼äµ¥Ôª.·¶Î§:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:Ê±¼ä¶Î2µÄÊ±¼äµ¥Ôª.·¶Î§:1~8;
//tbs1:Ê±¼ä¶Î1µÄÊ±¼äµ¥Ôª.·¶Î§:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :²¨ÌØÂÊ·ÖÆµÆ÷.·¶Î§:1~1024;(Êµ¼ÊÒª¼Ó1,Ò²¾ÍÊÇ1~1024) tq=(brp)*tpclk1
//×¢ÒâÒÔÉÏ²ÎÊıÈÎºÎÒ»¸ö¶¼²»ÄÜÉèÎª0,·ñÔò»áÂÒ.
//²¨ÌØÂÊ=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,ÆÕÍ¨Ä£Ê½;1,»Ø»·Ä£Ê½;
//Fpclk1µÄÊ±ÖÓÔÚ³õÊ¼»¯µÄÊ±ºòÉèÖÃÎª36M,Èç¹ûÉèÖÃCAN_Normal_Init(1,8,7,5,1);
//Ôò²¨ÌØÂÊÎª:36M/((1+8+7)*5)=450Kbps
//·µ»ØÖµ:0,³õÊ¼»¯OK;
//    ÆäËû,³õÊ¼»¯Ê§°Ü;

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	  GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
 	  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//Ê¹ÄÜPORTAÊ±ÖÓ	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//Ê¹ÄÜCAN1Ê±ÖÓ	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍì
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//³õÊ¼»¯IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//ÉÏÀ­ÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯IO
	  
 	//CANµ¥ÔªÉèÖÃ
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //·ÇÊ±¼ä´¥·¢Í¨ĞÅÄ£Ê½  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //Èí¼ş×Ô¶¯ÀëÏß¹ÜÀí	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //Ë¯ÃßÄ£Ê½Í¨¹ıÈí¼ş»½ĞÑ(Çå³ıCAN->MCRµÄSLEEPÎ»)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//½ûÖ¹±¨ÎÄ×Ô¶¯´«ËÍ //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //±¨ÎÄ²»Ëø¶¨,ĞÂµÄ¸²¸Ç¾ÉµÄ // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //ÓÅÏÈ¼¶ÓÉ±¨ÎÄ±êÊ¶·û¾ö¶¨ //
  	CAN_InitStructure.CAN_Mode= mode;	         //Ä£Ê½ÉèÖÃ£º mode:0,ÆÕÍ¨Ä£Ê½;1,»Ø»·Ä£Ê½; //
  	//ÉèÖÃ²¨ÌØÂÊ
  	CAN_InitStructure.CAN_SJW=tsjw;				//ÖØĞÂÍ¬²½ÌøÔ¾¿í¶È(Tsjw)Îªtsjw+1¸öÊ±¼äµ¥Î»  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1¸öÊ±¼äµ¥Î»CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1¸öÊ±¼äµ¥Î»CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //·ÖÆµÏµÊı(Fdiv)Îªbrp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // ³õÊ¼»¯CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //¹ıÂËÆ÷0
   	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32Î» 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32Î»ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32Î»MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//¹ıÂËÆ÷0¹ØÁªµ½FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //¼¤»î¹ıÂËÆ÷0

  	CAN_FilterInit(&CAN_FilterInitStructure);//ÂË²¨Æ÷³õÊ¼»¯
	
#if CAN_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0ÏûÏ¢¹ÒºÅÖĞ¶ÏÔÊĞí.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // Ö÷ÓÅÏÈ¼¶Îª0
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // ´ÎÓÅÏÈ¼¶Îª2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               // ¿ªÆôcanÖĞ¶Ï
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//Ê¹ÄÜRX0ÖĞ¶Ï
//ÖĞ¶Ï·şÎñº¯Êı			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	
//	i++;
//	if(i==100)
//	{
//		LED0=!LED0;//ÌáÊ¾ÏµÍ³ÕıÔÚÔËĞĞ	
//		i=0;
//	}
	
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	printf("id:%d",RxMessage.StdId);
	
	if(RxMessage.StdId == 0x01)//firelay ·¢À´µÄÏûÏ¢µÄ can_stdid  Îª 0x01 
	{
//		printf("\r\ncan_receive¢t");
//		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\r\n",RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);

/////////////Á½ÂÖ³µ
/*		
		if(ptForkLift->u8CarID==0)  //ÓÒÂÖ
		{

			ptForkLift->bDrection = (RxMessage.Data[6] & 0x02)>>1;

			if ((RxMessage.Data[6] & 0x02)>>1)
				ptForkLift->s16speedwant = -(float)((RxMessage.Data[3]<<8)| RxMessage.Data[2])*2.5; //25msÄÚµÄÂö³åÊı 
			else
				ptForkLift->s16speedwant = (float)((RxMessage.Data[3]<<8) | RxMessage.Data[2])*2.5;//25msÄÚµÄÂö³åÊı 
		}
		
		if(ptForkLift->u8CarID==1) //×óÂÖ
		{
			ptForkLift->bDrection = (RxMessage.Data[6] & 0x01);

//			printf("bDrection:%d",ptForkLift->bDrection);


			if ((RxMessage.Data[6] & 0x01))
				ptForkLift->s16speedwant = -(float)((RxMessage.Data[1]<<8) | RxMessage.Data[0])*2.5; //25msÄÚµÄÂö³åÊı 
			else
				ptForkLift->s16speedwant = (float)((RxMessage.Data[1]<<8) | RxMessage.Data[0])*2.5;//25msÄÚµÄÂö³åÊı 
			}
*/
///////////////////////////////²æ³µ//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/*
ÏÂĞĞ²¿·Ö£¨²æ³µ£©
£¨1£©µç»úÇı¶¯
×Ö½Ú7£º
X X X X 0 0 0 0 //message_type = 0x0
×Ö½Ú6£º£¨¸÷¸ö·½Ïò¿ØÖÆ£©
X X X X X - - -  
£¨bit7  ²æ³µ±ÛÇ°ºó     1 ÏòÇ° 0 Ïòºó£©
£¨bit6  ²æ³µ±ÛÉÏÏÂ     1 ÏòÉÏ 0 ÏòÏÂ£©
£¨bit5  ²æ³µX·½Ïò      1 Õı   0 ¸º£©
£¨bit4  ²æ³µY·½Ïò      1 Õı   0 ¸º£©
£¨bit3  ²æ³µĞı×ª·½Ïò   1 Õı   0 ¸º£©
×Ö½Ú5£º±£Áô
×Ö½Ú4£º£¨²æ³µ±ÛÇ°ºó£©
X X X X X X X X //£¨0.32m/s µÈ·Ö256£© 
×Ö½Ú3£º£¨²æ³µÉÏÏÂ£©
X X X X X X X X //£¨0.32m/s µÈ·Ö256£© 
×Ö½Ú2£º£¨²æ³µXËÙ¶È£©
X X X X X X X X //£¨0.8m/s µÈ·Ö256£©
×Ö½Ú1 £º£¨²æ³µYËÙ¶È£©
X X X X X X X X //£¨0.8m/s µÈ·Ö256£©
×Ö½Ú0 £º£¨½ÇËÙ¶È£©
X X X X X X X X // £¨3.14»¡¶È/s µÈ·Ö256
µ×ÅÌËÄ¸öÂÖ×Ó×ªÒ»È¦ 27500¸öÂö³å
ÂÖ×ÓÖ±¾¶ 0.125m   12.5cm
*/
/*
//½ÇËÙ¶È
if ((RxMessage.Data[6] & 0x08)) // ×Ö½Ú6 µÄbit3
  ptForkLift->s16w  =(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
else
  ptForkLift->s16w =-(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
//yÖáËÙ¶È
if ((RxMessage.Data[6] & 0x10)) // ×Ö½Ú6 µÄbit4
  ptForkLift->s16y  =(float)(RxMessage.Data[1]/255.0*0.8);  //0~255 -> 0~0.8m/s
else
  ptForkLift->s16y =-(float)(RxMessage.Data[1]/255.0*0.8);  //0~255 -> 0~0.8m/s
//xÖáËÙ¶È
if ((RxMessage.Data[6] & 0x20)) // ×Ö½Ú6 µÄbit5
  ptForkLift->s16x  =(float)(RxMessage.Data[2]/255.0*0.8);  //0~255 -> 0~0.8m/s
else
  ptForkLift->s16x =-(float)(RxMessage.Data[2]/255.0*0.8);  //0~255 -> 0~0.8m/s
//ÉÏÏÂ²æËÙ¶È
if ((RxMessage.Data[6] & 0x40)) // ×Ö½Ú6 µÄbit5
  ptForkLift->s16ud =(float)(RxMessage.Data[3]/255.0*0.32); //0~255 -> 0~0.32m/s
else
  ptForkLift->s16ud=-(float)(RxMessage.Data[3]/255.0*0.32); //0~255 -> 0~0.32m/s¡
//Ç°ºó²æËÙ¶È
if ((RxMessage.Data[6] & 0x80)) // ×Ö½Ú6 µÄbit5
  ptForkLift->s16fb =(float)(RxMessage.Data[4]/255.0*0.32); //0~255 -> 0~0.32m/s
else  ptForkLift->s16fb=-(float)(RxMessage.Data[4]/255.0*0.32); //0~255 -> 0~0.32m/s
*/
//printf("w:%f\r\n",ptForkLift->s16w);
/*
//²æ³µÎïÀí²ÎÊı
ptForkLift->s16fb_d=0.032; //Ç°ºó²æÂÖ×ÓÖ±¾¶ 0.032m    3.2cm
ptForkLift->s16ud_d=0.032; //ÉÏÏÂ²æÂÖ×ÓÖ±¾¶
ptForkLift->s16dp_d=0.485; //µ×ÅÌÖ±¾¶      //0.485m   48.5cm
ptForkLift->s16dpl_d=0.125;//µ×ÅÌÂÖ×ÓÖ±¾¶   //0.125m   12.5cm

ptForkLift->u32dpl_econder=500;//µ×ÅÌÂÖÏßÊı
ptForkLift->u32dpl_econder_reduc=55;//µ×ÅÌÂÖ¼õËÙ±È

ptForkLift->u32ud_econder=500;//µ×ÅÌÂÖÏßÊı
ptForkLift->u32ud_econder_reduc=55;//µ×ÅÌÂÖ¼õËÙ±È

ptForkLift->u32fb_econder=500;//Ç°ºóÂÖÏßÊı
ptForkLift->u32fb_econder_reduc=160;//Ç°ºóÂÖ¼õËÙ±È
*/
///*

if(ptForkLift->u8CarID==1)  //Ç°ÂÖ  v1=Vy+w*R
		{
	//		/*
//½ÇËÙ¶È
if ((RxMessage.Data[6] & 0x08)) // ×Ö½Ú6 µÄbit3
  ptForkLift->s16w  =(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
else
  ptForkLift->s16w = -(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
//yÖáËÙ¶È
if ((RxMessage.Data[6] & 0x10)) // ×Ö½Ú6 µÄbit4
  ptForkLift->s16y  =(float)(RxMessage.Data[1]/255.0*0.8);  //0~255 -> 0~0.8m/s
else
  ptForkLift->s16y = -(float)(RxMessage.Data[1]/255.0*0.8);  //0~255 -> 0~0.8m/s
//*/
			
ptForkLift->s16speedwant = (float)((ptForkLift->s16y + ptForkLift->s16w * ptForkLift->s16dp_d*0.5)/(3.14*ptForkLift->s16dpl_d)*ptForkLift->u32dpl_econder*ptForkLift->u32dpl_econder_reduc/1000); //1msÄÚµÄÂö³åÊı 

}
		
	
if(ptForkLift->u8CarID==2)  //×óÂÖ v1=-Vx+w*R
		{
//			/*
//½ÇËÙ¶È
if ((RxMessage.Data[6] & 0x08)) // ×Ö½Ú6 µÄbit3
  ptForkLift->s16w  =(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
else
  ptForkLift->s16w = -(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
//xÖáËÙ¶È
if ((RxMessage.Data[6] & 0x20)) // ×Ö½Ú6 µÄbit5
  ptForkLift->s16x  =(float)(RxMessage.Data[2]/255.0*0.8);  //0~255 -> 0~0.8m/s
else
  ptForkLift->s16x = -(float)(RxMessage.Data[2]/255.0*0.8);  //0~255 -> 0~0.8m/s
//*/
ptForkLift->s16speedwant = (float)((-ptForkLift->s16x + ptForkLift->s16w * ptForkLift->s16dp_d*0.5)/(3.14*ptForkLift->s16dpl_d)*ptForkLift->u32dpl_econder*ptForkLift->u32dpl_econder_reduc/1000); //1msÄÚµÄÂö³åÊı 
		}
		
		
if(ptForkLift->u8CarID==3)  //ºóÂÖ v1=-Vy+w*R
		{
//			/*
//½ÇËÙ¶È
if ((RxMessage.Data[6] & 0x08)) // ×Ö½Ú6 µÄbit3
  ptForkLift->s16w  =(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
else
  ptForkLift->s16w = -(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
//yÖáËÙ¶È
if ((RxMessage.Data[6] & 0x10)) // ×Ö½Ú6 µÄbit4
  ptForkLift->s16y  =(float)(RxMessage.Data[1]/255.0*0.8);  //0~255 -> 0~0.8m/s
else
  ptForkLift->s16y = -(float)(RxMessage.Data[1]/255.0*0.8);  //0~255 -> 0~0.8m/s
//*/
ptForkLift->s16speedwant = (float)((-ptForkLift->s16y + ptForkLift->s16w * ptForkLift->s16dp_d*0.5)/(3.14*ptForkLift->s16dpl_d)*ptForkLift->u32dpl_econder*ptForkLift->u32dpl_econder_reduc/1000); //1msÄÚµÄÂö³åÊı 
		}
		
		
if(ptForkLift->u8CarID==4)  //ÓÒÂÖ v1=Vx+w*R
    {
//			/*
//½ÇËÙ¶È
if ((RxMessage.Data[6] & 0x08)) // ×Ö½Ú6 µÄbit3
  ptForkLift->s16w  =(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
else
  ptForkLift->s16w = -(float)(RxMessage.Data[0]/255.0*3.14); //0~255 -> 0~3.14rad/s
//xÖáËÙ¶È
if ((RxMessage.Data[6] & 0x20)) // ×Ö½Ú6 µÄbit5
  ptForkLift->s16x  =(float)(RxMessage.Data[2]/255.0*0.8);  //0~255 -> 0~0.8m/s
else
	ptForkLift->s16x = -(float)(RxMessage.Data[2]/255.0*0.8);  //0~255 -> 0~0.8m/s
//*/
ptForkLift->s16speedwant = (float)((ptForkLift->s16x + ptForkLift->s16w * ptForkLift->s16dp_d*0.5)/(3.14*ptForkLift->s16dpl_d)*ptForkLift->u32dpl_econder*ptForkLift->u32dpl_econder_reduc/1000); //1msÄÚµÄÂö³åÊı 
		}
		
		
if(ptForkLift->u8CarID==5)  //ÉÏÏÂ²æ
		{
    	// ptForkLift->s16speedwant = (float)(ptForkLift->s16ud/(3.14*ptForkLift->s16ud_d)*ptForkLift->u32ud_econder*ptForkLift->u32ud_econder_reduc/1000); //1msÄÚµÄÂö³åÊı
			ptForkLift->s16speedwant =0;
		}
if(ptForkLift->u8CarID==6)  //Ç°ºó²æ
		{
//			/*
//Ç°ºó²æËÙ¶È
if ((RxMessage.Data[6] & 0x80)) // ×Ö½Ú6 µÄbit5
  ptForkLift->s16fb =(float)(RxMessage.Data[4]/255.0*0.32); //0~255 -> 0~0.32m/s
else
  ptForkLift->s16fb = -(float)(RxMessage.Data[4]/255.0*0.32); //0~255 -> 0~0.32m/s
//*/
ptForkLift->s16speedwant = (float)(ptForkLift->s16fb/(3.14*ptForkLift->s16fb_d)*ptForkLift->u32fb_econder*ptForkLift->u32fb_econder_reduc/1000); //1msÄÚµÄÂö³åÊı 
		 //ptForkLift->s16speedwant =0;
		}
//*/

//printf("%f\r\n",ptForkLift->s16speedwant);

			if (ptForkLift->s16speedwant==0)
			{
				ptForkLift->s16error[0] = 0;
				ptForkLift->s16error[1] = 0;
				ptForkLift->s16error[2] = 0;
				ptForkLift->s16ErrorSum = 0;
			}
			

//		printf("direction: %d\t value: %f\t\r\n",ptForkLift->bDrection,ptForkLift->s16speedwant);
		ptForkLift->u16CanComCount[0]++;
		
	}
	   
	
	
	
	//for(i=0;i<8;i++)
	//printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

//can·¢ËÍÒ»×éÊı¾İ(¹Ì¶¨¸ñÊ½:IDÎª0X12,±ê×¼Ö¡,Êı¾İÖ¡)	
//len:Êı¾İ³¤¶È(×î´óÎª8)				     
//msg:Êı¾İÖ¸Õë,×î´óÎª8¸ö×Ö½Ú.
//·µ»ØÖµ:0,³É¹¦;
//		 ÆäËû,Ê§°Ü;
u8 Can_Send_Msg(void)
{	
  u8 mbox;
  u16 i=0;

//	printf("can_send_msg:%d\r\t\n",ptForkLift->s32EncoderCounter);
	
  mbox= CAN_Transmit(CAN1, &ptForkLift->Can_Txmessage);   
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//µÈ´ı·¢ËÍ½áÊø
  if(i>=0XFFF)return 1;
  return 0;		

}
//can¿Ú½ÓÊÕÊı¾İ²éÑ¯
//buf:Êı¾İ»º´æÇø;	 
//·µ»ØÖµ:0,ÎŞÊı¾İ±»ÊÕµ½;
//		 ÆäËû,½ÓÊÕµÄÊı¾İ³¤¶È;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//Ã»ÓĞ½ÓÊÕµ½Êı¾İ,Ö±½ÓÍË³ö 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//¶ÁÈ¡Êı¾İ	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}















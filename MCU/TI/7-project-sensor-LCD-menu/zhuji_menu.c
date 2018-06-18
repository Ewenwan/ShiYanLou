/*
 * 基于MSP430G2553简单的多按键中断菜单例程
 * 硬件描述：Launchpad G2553开发板上P2.2\P2.3\P2.4\P2.5分别接四个按钮
 * 功能描述：按键按下的瞬间，显示相应的菜单
 *  Created on: 2015-4-3
 *  Author: 万有文
 */

#include "MSP430G2553.h"
#include "MENU.h"
#include "LCD12864.h"
#include "MQxilie.h"
#include "DHT11.h"

void timer_init();
void GPIO_init();
void P23_ON();
void P24_ON();
void P25_ON();
void Hangxs();
void baojpd();
void baojqd();
//typedef unsigned char u8;                              //数据类型定义
//typedef unsigned int  u16;
//typedef unsigned long u32;

int Hang1=1,Hang2=0,Hang3=0,Ent1=0,Ent2=0,Ent3=0;
int M1=0,M21=0,M22=0,M31=0,M32=0,M33=0,M34=0,M35=0,M36=0,M37=0;
int flag3s1=0,flag3s2=0,flag3s3=0,flag3s4=0;
int i=0,j=0;
//u16 T_set=30,M_set=60,CO_set=40,CH4_set=40,jq_set=40,ben_set=40,pm_set=40;
//u8 pm_dj=4,baoj=1;


#define CPU          (1000000)            //1MHz
#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))

void main(void)

{
	WDTCTL = WDTPW + WDTHOLD;	 //关狗
	//GPIO_init();				 //IO初始化
	P2DIR=0x03;                  //就是少了这条指令，导致研究好久！！！
	 sync();
	 GPIO_init();

	 timer_init();
	welcom_m();
	delay_ms(2000);
	_enable_interrupts();		//使能总中断
	//_bis_SR_register(LPM3_bits);//休眠
	while(1)
	{

	while(M1==0)
	  {
		if(flag3s1)
			gund_1();
		//delay_ms(3000);
		if(flag3s2)
			gund_2();
		//delay_ms(3000);
		if(flag3s3)
			gund_3();
		//delay_ms(3000);
		if(flag3s4)
			gund_4();
		//delay_ms(3000);
		delay_ms(400);
	  }

	while(M1==1)
			{
				if((M21==0)&(M22==0)) manu_1();
				if(Hang1>3) Hang1=1;
				if(Hang1<1) Hang1=3;

				if(Hang1==1)                         //一级菜单
				{
					if(Hang2==0) Hangxs(Hang1);
					if(Ent1&M1)
					{
					M21=1;
					//Hang2=1;
					if(Hang2>=9) Hang2=1;
					if(Hang2<1)  Hang2=8;
					if((M31==0)&(M32==0)&(M33==0)&(M34==0)&(M35==0)&(M36==0)&(M37==0))
					 {
						if(Hang2<=4) manu_211();
		                if(Hang2>4)  manu_212();
					 }

					if(Hang2==1)                         //温度报警设定
					{
						if(Hang3==0) Hangxs(Hang2);
						if(Ent2&M21)
						{
							M31=1;
							//Hang3=2;
							manu_31();
							if(Hang3>=5) Hang3=2;
							if(Hang3<2)  Hang3=4;
							if(Hang3==2)
							{
								Hangxs(Hang3);
								if(Ent3&M31)
								{
									T_set++;
									Ent3=0;
								}
							 }
							if(Hang3==3)
							{
								Hangxs(Hang3);
							   if(Ent3&M31)
							   {
						    	 T_set--;
						    	 Ent3=0;
							   }
						     }
							if(Hang3==4)
							{
								Hangxs(Hang3);
								    if(Ent3&M31)
								     {
										//M2=0;//如何返回
										Hang3=0;
										Ent3=0;
										M31=0;
										Ent2=0;
								     }
						     }
						}
					}


					   if(Hang2==2)                       //湿度报警设定
							{
						        if(Hang3==0) Hangxs(Hang2);
								if(Ent2&M21)
								{
									M32=1;
									//Hang3=2;
									manu_32();
									if(Hang3>=5) Hang3=2;
									if(Hang3<2)  Hang3=4;
									if(Hang3==2)
									{
										Hangxs(Hang3);
										if(Ent3&M32)
										{
											M_set++;
											Ent3=0;
										}

									 }
									if(Hang3==3)
									{
										Hangxs(Hang3);
									   if(Ent3&M32)
									   {
								    	 M_set--;
								    	 Ent3=0;
									   }
								     }
									if(Hang3==4)
									{
										Hangxs(Hang3);
										    if(Ent3&M32)
										     {
												//M2=0;//如何返回
												Hang3=0;
												Ent3=0;
												M32=0;
												Ent2=0;
										     }
								     }
								}
							}




					 	if(Hang2==3)                        //CO报警设定
							{
					 		 if(Hang3==0) Hangxs(Hang2);
							 if(Ent2&M21)
								{
									M33=1;
									//Hang3=2;
									manu_33();
									if(Hang3>=5) Hang3=2;
									if(Hang3<2)  Hang3=4;
									if(Hang3==2)
										{
											Hangxs(Hang3);
											if(Ent3&M33)
											{
													CO_set++;
													Ent3=0;
											}
										 }
									if(Hang3==3)
										{
											Hangxs(Hang3);
											  if(Ent3&M33)
											   {
										    	 CO_set--;
										    	 Ent3=0;
											   }
										 }
									if(Hang3==4)
										{
											Hangxs(Hang3);
										     if(Ent3&M33)
												    {
														//M2=0;//如何返回
														Hang3=0;
														Ent3=0;
														M33=0;
														Ent2=0;
												    }
						                 }
									}
							}





						if(Hang2==4)                                 //CH4报警设定
							{
							if(Hang3==0) Hangxs(Hang2);
								 if(Ent2&M21)
								    {
										M34=1;
										//Hang3=2;
										manu_34();
										if(Hang3>=5) Hang3=2;
										if(Hang3<2)  Hang3=4;
										if(Hang3==2)
										{
											Hangxs(Hang3);
											if(Ent3&M34)
											{
												CH4_set++;
												Ent3=0;
											 }
										 }
									    if(Hang3==3)
									    	{
									    	    Hangxs(Hang3);
												if(Ent3&M34)
												 {
												    CH4_set--;
												    Ent3=0;
												 }
											 }
										if(Hang3==4)
											{
											Hangxs(Hang3);
											  if(Ent3&M34)
											   {
												//M2=0;//如何返回
												Hang3=0;
												Ent3=0;
												M34=0;
												Ent2=0;
											   }
											}
								    }
							}




						if(Hang2==5)                                 //甲醛报警设定
							{
							  if(Hang3==0) Hangxs(1);
								 if(Ent2&M21)
								    {
										M35=1;
										//Hang3=2;
										manu_35();
										if(Hang3>=5) Hang3=2;
										if(Hang3<2)  Hang3=4;
										if(Hang3==2)
										{
											Hangxs(Hang3);
											if(Ent3&M35)
											{
												jq_set++;
												Ent3=0;
											 }
										 }
									    if(Hang3==3)
									    	{
									    	  Hangxs(Hang3);
											  if(Ent3&M35)
												 {
												    jq_set--;
												    Ent3=0;
												 }
											 }
										if(Hang3==4)
											{
											  Hangxs(Hang3);
											  if(Ent3&M35)
											   {
												//M2=0;//如何返回
												Hang3=0;
												Ent3=0;
												M35=0;
												Ent2=0;
											   }
											}
								    }
							}


						if(Hang2==6)                                 //苯报警设定
							{
							   if(Hang3==0) Hangxs(2);
								 if(Ent2&M21)
								    {
										M36=1;
										//Hang3=2;
										manu_36();
										if(Hang3>=5) Hang3=2;
										if(Hang3<2)  Hang3=4;
										if(Hang3==2)
										{
										   Hangxs(Hang3);
											if(Ent3&M36)
											{
												ben_set++;
												Ent3=0;
											 }
										 }
									    if(Hang3==3)
									    	{
									    	 Hangxs(Hang3);
												if(Ent3&M36)
												 {
												    ben_set--;
												    Ent3=0;
												 }
											 }
										if(Hang3==4)
											{
											 Hangxs(Hang3);
											  if(Ent3&M36)
											   {
												//M2=0;//如何返回
												Hang3=0;
												Ent3=0;
												Ent2=0;
												M36=0;
											   }
											}
								    }
							}


						if(Hang2==7)                                 //PM2.5报警设定
							{
							 if(Hang3==0) Hangxs(3);
								 if(Ent2&M21)
								    {
										M37=1;
										//Hang3=2;
										manu_37();
										if(Hang3>=5) Hang3=2;
										if(Hang3<2)  Hang3=4;
										if(Hang3==2)
										{
											Hangxs(Hang3);
											if(Ent3&M37)
											{
												pm_set++;
												Ent3=0;
											 }
										 }
									    if(Hang3==3)
									    	{
									    	Hangxs(Hang3);
												if(Ent3&M37)
												 {
												    pm_set--;
												    Ent3=0;
												 }
											 }
										if(Hang3==4)
											{
											Hangxs(Hang3);
											  if(Ent3&M37)
											   {
												//M2=0;//如何返回
												Hang3=0;
												Ent3=0;
												Ent2=0;
												M37=0;
											   }
											}
								    }
							}

		                   if(Hang2==8)                                //二级菜单返回
		                    {
		                	    Hangxs(4);
		                	    if(Ent2&M21)
		                	   	  {
		                	    	//Hang1=1;
		                	    	Ent1=0;
		                	    	M21=0;
		                	    	Ent2=0;
		                	    	Hang2=0;
		                	   	  }
		                    }
					}
				}






				if(Hang1==2)                          //一级菜单
				{
					if(Hang2==0) Hangxs(Hang1);
					  if(Ent1&M1)
						{
						  M22=1;
						 // Hang2=2;
						  if(Hang2>4) Hang2=2;
						  if(Hang2<2) Hang2=4;
						   manu_22();

						  if(Hang2==2)
						  	{
							  Hangxs(Hang2);
						  	if(Ent2&M22)
						    	{
						  		   //M3
						  		   baoj=0;
						  		   Ent2=0;
						  	   }
						    }

						  if(Hang2==3)
						  	{
							  Hangxs(Hang2);
						  	if(Ent2&M22)
						    	{
						  		   baoj=1;
						  		   Ent2=0;
						  	   }
						    }
						  if(Hang2==4)                          //  二级菜单返回
						  	{
							  Hangxs(Hang2);
						  	if(Ent2&M22)
						    	{
		                       Ent1=0;
		                       Hang2=0;
		                       Ent2=0;
		                       M22=0;
						  	   }
						    }

						}

				}


				if(Hang1==3)                                  //一级菜单
				{
					Hangxs(Hang1);
					if(Ent1)
					{
						M1=0;Ent1=0;Hang1=0;
					}
				}

		    }

   }

}
void timer_init()
{
  TACCR0 =50000;  //计到50000，约400ms
  TACTL = TASSEL_2+ID_3+MC_1+TACLR;  //设置TIMERA的时钟源为SMCLK(1M),8分频后（125K）,计数模式为up,到CCR0再自动从0开始计数
  TACCTL0 |= CCIE;                    //打开 比较模块0中断
 // _EINT();
}

//--------定时器中断函数------------
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)     //CCIFG中断被响应后，该标志位自动清零
{
	i++;
	if(i>7) i=1;
	if(i==7)
	{
     j++;
     if(j>4) j=1;
     switch(j)
     {
     case 1:flag3s1=1,flag3s2=0,flag3s3=0,flag3s4=0;break;
     case 2:flag3s2=1,flag3s1=0,flag3s3=0,flag3s4=0;break;
     case 3:flag3s3=1,flag3s1=0,flag3s2=0,flag3s4=0;break;
     case 4:flag3s4=1,flag3s1=0,flag3s2=0,flag3s3=0;break;
     default:                                       break;
     }
	}
}


void GPIO_init()
{
	P2REN |= BIT2+BIT3+BIT4+BIT5;  //启用P2.2...内部上下拉电阻
	P2OUT |= BIT2+BIT3+BIT4+BIT5;  //将电阻设置为上拉
	//-----配置P1.3中断参数-----
	P2DIR &= ~(BIT2+BIT3+BIT4+BIT5); // P2.2....设为输入(可省略)
	P2IES |= BIT2+BIT3+BIT4+BIT5;    // P2.2....设为下降沿中断
	P2IE  |= BIT2+BIT3+BIT4+BIT5;    // 允许P2.2..中断
}


#pragma vector = PORT2_VECTOR         //端口2的中断向量
__interrupt void PORT2_ISR(void)
{

	unsigned int Push_Key=0;
		//-----排除输出IO的干扰后，锁定唯一被触发的中断标志位-----
		Push_Key=P2IFG&(~P2DIR);
		//-----延时一段时间，避开机械抖动区域-----
		__delay_cycles(10000);			//消抖延时
		//----判断按键状态是否与延时前一致-----
		if((P2IN&Push_Key)==0) 	        //如果该次按键确实有效
	   {
		 //----判断具体哪个IO被按下，调用该IO的事件处理函数-----
		  switch(Push_Key){
		 case BIT2: 	if(M1==0) M1=1; break;
		 case BIT3: 	P23_ON();		break;
		 case BIT4: 	P24_ON();		break;
		 case BIT5: 	P25_ON();		break;
		 default:					break;		//任何情况下均加上default
		 }
	   }

   	P2IFG=0;                    //退出中断前必须手动清除IO口中断标志
}


void P23_ON()
{
    if((M21==0)&(M22==0)&(M31==0)&(M32==0)&(M33==0)&(M34==0)&(M35==0)&(M36==0)&(M37==0)&M1) Hang1++;
    // else Hang1=Hang1;
    if((M31==0)&(M32==0)&(M33==0)&(M34==0)&(M35==0)&(M36==0)&(M37==0)&(M21||M22))           Hang2++;
   // else Hang2=Hang2;
    if((M31==1)||(M32==1)||(M33==1)||(M34==1)||(M35==1)||(M36==1)||(M37==1))                Hang3++;
}
void P24_ON()
{
	if((M21==0)&(M22==0)&(M31==0)&(M32==0)&(M33==0)&(M34==0)&(M35==0)&(M36==0)&(M37==0)&M1) Hang1--;
	//else Hang1=Hang1;
	if((M31==0)&(M32==0)&(M33==0)&(M34==0)&(M35==0)&(M36==0)&(M37==0)&(M21||M22))           Hang2--;
	//else Hang2=Hang2;
	if((M31==1)||(M32==1)||(M33==1)||(M34==1)||(M35==1)||(M36==1)||(M37==1))                Hang3--;
}
void P25_ON()
{
   if(M1)                                Ent1=1;
   if(M21||M22)                          Ent2=1;
   if(M31||M32||M33||M34||M35||M36||M37) Ent3=1;
}


//行选择标志显示
void Hangxs(int a)
{
switch (a)
  {
 case 1:
   {
     write_string(0,3,"");
     write_string(0,2,"");
     write_string(0,4,"");
     write_string(0,1,"<");
   } break;
 case 2:
   {
     write_string(0,3,"");
     write_string(0,1,"");
     write_string(0,4,"");
     write_string(0,2,"<");
   } break;
 case 3:
   {
     write_string(0,1,"");
     write_string(0,2,"");
     write_string(0,4,"");
     write_string(0,3,"<");
   } break;
 case 4:
   {
     write_string(0,3,"");
     write_string(0,2,"");
     write_string(0,1,"");
     write_string(0,4,"<");
   } break;
 default:	break;
  }
}


//报警模块驱动程序

 void baojpd()
   {
	 int T_flag,M_flag,CO_flag,CH_flag,jq_flag,ben_flag,pm_flag;
	 if(Tem>T_set) T_flag=1;
	   else if(Tem<(T_set-5)) T_flag=0;
	 if(Mix>M_set) M_flag=1;
	   else if(Mix<(M_set-5)) M_flag=0;
	 if((int)COppm>CO_set)    CO_flag=1;
	   else if((int)COppm<(CO_set-5)) CO_flag=0;
	 if((int)CH4ppm>CH4_set)    CH_flag=1;
	   else if((int)CH4ppm<(CH4_set-5)) CH_flag=0;
	 if((int)jqppm>jq_set)    jq_flag=1;
	   else if((int)jqppm<(jq_set-5)) jq_flag=0;
	 if((int)Benppm>ben_set)    ben_flag=1;
	   else if((int)Benppm<(ben_set-5)) ben_flag=0;
	 if((int)PMppm>pm_set)    pm_flag=1;
	   else if((int)PMppm<(pm_set-5)) pm_flag=0;

	 if((T_flag==0)&(M_flag==0)&(CO_flag==0)&(CH_flag==0)&(jq_flag==0)&(ben_flag==0)&(pm_flag==0))
		 baoj=0;
	 else baoj=1;
   }

 void baojqd(int bj)
 {
	if(bj==1) P1DIR =BIT7, P1OUT^=BIT6;
	else P1OUT &= ~BIT7;
 }





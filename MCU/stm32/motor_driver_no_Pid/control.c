#include "control.h"
#include "PidContoller.h"
/************************************************
 
 ´´½¨Õß£ºÍô×ÔÇ¿
 ´´½¨Ê±¼ä£º2017/03/20
 ÎÄ¼þËµÃ÷£ºÖÇ³ËÍøÂç¿Æ¼¼¹«Ë¾²æ³µµç»úÇý¶¯³ÌÐò
 
 µç»úÉÏ²ã¿ØÖÆ³ÌÐò¶¼ÔÚ´ËÎÄ¼þÖÐ
 
************************************************/

//È«¾Ö±äÁ¿ÉùÃ÷
TCtrl TForkLift;     //¶¨Òå²æ³µ½á¹¹Ìå
ptCtrl ptForkLift;   //¶¨Òå²æ³µ½á¹¹ÌåÖ¸Õë

// ÉèÖÃ PWMºÍ·½ÏòÖµ
void SetPwmDir(ptCtrl ptCtrlCar)
{
	// pwm ÏÞ·ù Êä³ö
	if(ptCtrlCar->u16PWM>MAX_PWM)
	{
		ptCtrlCar->u16PWM=MAX_PWM;
	}
	else if(ptCtrlCar->u16PWM<=MIN_PWM)
	{
		ptCtrlCar->u16PWM=MIN_PWM;
	}
	
	//TIM_SetCompare3(TIM1,(ptCtrlCar->u16PWM)/2);//AD´¥·¢ÖÐ¶Ï
	//TIM_SetCompare3(TIM1,ptCtrlCar->u16PWM);//AD´¥·¢ÖÐ¶
	 TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
	//TIM1->CCR1 = ptCtrlCar->u16PWM;       // Í¨µÀ1 pwm
	
	// //PA.9 ¿ØÖÆ·½Ïò ¸ß Ç°½ø  µÍ ºóÍË
	if(ptCtrlCar->bDrection==FOWARD)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);  //dir	
		//GPIO_SetBits(GPIOA,GPIO_Pin_9);			//PA.9 Êä³ö¸ß
		TIM_SetCompare2(TIM1,1799);           // Í¨µÀ2 pwm Îª 100% Âö³å ¼´Îª¸ßµçÆ½
		//TIM1->CCR2 = ptCtrlCar->1799;       // Í¨µÀ2 pwm
	}
	
	else if(ptCtrlCar->bDrection==BACK)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);  //dir	
		//GPIO_ResetBits(GPIOA,GPIO_Pin_9);			//PA.9 Êä³öµÍ
	  TIM_SetCompare2(TIM1,0);           // Í¨µÀ2 pwm Îª 0% Âö³å ¼´ÎªµÍµçÆ½
		//TIM1->CCR2 = ptCtrlCar->0;       // Í¨µÀ2 pwm
		
	}	
}		


// ÉèÖÃ PWMºÍ·½ÏòÖµ
void SetPwmDir_new(ptCtrl ptCtrlCar)
{
	// pwm ÏÞ·ù Êä³ö
	if(ptCtrlCar->u16PWM>MAX_PWM)
	{
		ptCtrlCar->u16PWM=MAX_PWM;
	}
	else if(ptCtrlCar->u16PWM<=MIN_PWM)
	{
		ptCtrlCar->u16PWM=MIN_PWM;
	}
	
	//TIM_SetCompare3(TIM1,(ptCtrlCar->u16PWM)/2);//AD´¥·¢ÖÐ¶Ï
	//TIM_SetCompare3(TIM1,ptCtrlCar->u16PWM);
	//TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
	//TIM1->CCR1 = ptCtrlCar->u16PWM;       // Í¨µÀ1 pwm
	
	// //PA.9 ¿ØÖÆ·½Ïò ¸ß Ç°½ø  µÍ ºóÍË
	if(ptCtrlCar->bDrection==FOWARD)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);  //dir	
		//GPIO_SetBits(GPIOA,GPIO_Pin_9);			//PA.9 Êä³ö¸ß
		
	 /*Èô·½ÏòÏà·´£¬ Ôò ½« if  ºÍ else if ÄÚÈÝ»¥»» */
		TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
		TIM_SetCompare2(TIM1,0);           // Í¨µÀ2 pwm Îª  0% Âö³å ¼´ÎªµÍµçÆ½
	}
	else if(ptCtrlCar->bDrection==BACK)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);  //dir	
		//GPIO_ResetBits(GPIOA,GPIO_Pin_9);			//PA.9 Êä³öµÍ
		
	 /*Èô·½ÏòÏà·´£¬ Ôò ½« if  ºÍ else if ÄÚÈÝ»¥»» */
		TIM_SetCompare1(TIM1,0);                // Í¨µÀ1 pwm Îª 0% Âö³å ¼´ÎªµÍµçÆ½
	  TIM_SetCompare2(TIM1,ptCtrlCar->u16PWM); 
	}	
}	


void PID_Control()
{
	
}






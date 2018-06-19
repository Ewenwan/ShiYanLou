#include "control.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
/************************************************
 
 创建者：汪自强
 创建时间：2017/03/20
 文件说明：智乘网络科技公司叉车电机驱动程序
 
 电机上层控制程序都在此文件中
 
************************************************/

//全局变量声明
TCtrl            TForkLift;     //定义叉车结构体
ptCtrl           ptForkLift;    //定义叉车结构体指针
linkqueue        middle_filter_queue;
linkqueue*       ptmiddle_filter_queue;


/*老板子任采用 1个PWM  1个普通IO口形式*/
/*
void SetPwmDir(ptCtrl ptCtrlCar)
{
	if(ptCtrlCar->u16PWM>MAX_PWM)
	{
		ptCtrlCar->u16PWM=MAX_PWM;
	}
	else if(ptCtrlCar->u16PWM<=MIN_PWM)
	{
		ptCtrlCar->u16PWM=MIN_PWM;
	}
	TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
	
	//TIM1->CCR1 = ptCtrlCar->u16PWM;       //pwm
	
	if(ptCtrlCar->bDrection==FOWARD)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);  //dir	
		GPIO_SetBits(GPIOA,GPIO_Pin_9);			//PA.9 输出高
	}
	else if(ptCtrlCar->bDrection==BACK)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);  //dir	
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);			//PA.9 输出低
		
	}	
}	
*/


// 老板子如果也采用 新板子  两个均为 PWM口的 情况  一开机会产生 pwm 脉冲 原因 PA9口 未下拉
// 设置 PWM和方向值
void SetPwmDir(ptCtrl ptCtrlCar)
{
	// pwm 限幅 输出
	if(ptCtrlCar->u16PWM>MAX_PWM)
	{
		ptCtrlCar->u16PWM=MAX_PWM;
	}
	else if(ptCtrlCar->u16PWM<=MIN_PWM)
	{
		ptCtrlCar->u16PWM=MIN_PWM;
	}
	TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
	//TIM1->CCR1 = ptCtrlCar->u16PWM;       // 通道1 pwm
	
	// //PA.9 控制方向 高 前进  低 后退
	if(ptCtrlCar->bDrection==FOWARD)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);  //dir	
		//GPIO_SetBits(GPIOA,GPIO_Pin_9);			//PA.9 输出高
		TIM_SetCompare2(TIM1,1799);           // 通道2 pwm 为 100% 脉冲 即为高电平
		//TIM1->CCR2 = ptCtrlCar->1799;       // 通道2 pwm
	}
	
	else if(ptCtrlCar->bDrection==BACK)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);  //dir	
		//GPIO_ResetBits(GPIOA,GPIO_Pin_9);			//PA.9 输出低
	  TIM_SetCompare2(TIM1,0);           // 通道2 pwm 为 0% 脉冲 即为低电平
		//TIM1->CCR2 = ptCtrlCar->0;       // 通道2 pwm
		
	}	
}		


// 设置 PWM和方向值 新板子
void SetPwmDir_new(ptCtrl ptCtrlCar)
{
	// pwm 限幅 输出
	if(ptCtrlCar->u16PWM>MAX_PWM)
	{
		ptCtrlCar->u16PWM=MAX_PWM;
	}
	else if(ptCtrlCar->u16PWM<=MIN_PWM)
	{
		ptCtrlCar->u16PWM=MIN_PWM;
	}
	
	//TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
	//TIM1->CCR1 = ptCtrlCar->u16PWM;       // 通道1 pwm
	
	// //PA.9 控制方向 高 前进  低 后退
	if(ptCtrlCar->bDrection==FOWARD)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);  //dir	
		//GPIO_SetBits(GPIOA,GPIO_Pin_9);			//PA.9 输出高
		
	 /*若方向相反， 则 将 if  和 else if 内容互换 */
		TIM_SetCompare1(TIM1,ptCtrlCar->u16PWM);
		TIM_SetCompare2(TIM1,0);           // 通道2 pwm 为  0% 脉冲 即为低电平
	}
	else if(ptCtrlCar->bDrection==BACK)
	{
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);  //dir	
		//GPIO_ResetBits(GPIOA,GPIO_Pin_9);			//PA.9 输出低
		
	 /*若方向相反， 则 将 if  和 else if 内容互换 */
		TIM_SetCompare1(TIM1,0);                // 通道1 pwm 为 0% 脉冲 即为低电平
	  TIM_SetCompare2(TIM1,ptCtrlCar->u16PWM); 
	}	
}	

// 增量式PID
/*
ptCtrlCar->s16speedout += ptCtrlCar->s16speed_p*(ptCtrlCar->s16error[0] - ptCtrlCar->s16error[1])						
							+ ptCtrlCar->s16speed_i*ptCtrlCar->s16error[0]
							+ ptCtrlCar->s16speed_d*(ptCtrlCar->s16error[0] - 2 * ptCtrlCar->s16error[1] + ptCtrlCar->s16error[2]);
*/ 

/*位置式PID算法*/
void POSITION_PID(ptCtrl ptCtrlCar,int EncoderSpeed)
{
	
	ptCtrlCar->s16error[0] = ptCtrlCar->s16speedwant - (float)EncoderSpeed;
	
	//printf("ptCtrlCar->s16ErrorSum =  %f\r\n",ptCtrlCar->s16ErrorSum);		
	ptCtrlCar->s16speedout = ptCtrlCar->s16speed_p*ptCtrlCar->s16error[0] 
							+ ptCtrlCar->s16speed_d*(ptCtrlCar->s16error[0] - ptCtrlCar->s16error[1])
							+ ptCtrlCar->s16speed_i*ptCtrlCar->s16ErrorSum;
	
	ptCtrlCar->s16error[1] = ptCtrlCar->s16error[0];//上次误差
	
	/******wyw加入为了得到 增量式PID*******/
	ptCtrlCar->s16error[2] = ptCtrlCar->s16error[1];//上上次误差
	
	ptCtrlCar->s16ErrorSum += ptCtrlCar->s16error[0];
	
	/***************调试信息****************/
	printf("\r\n\tPID_OUTPUT\t");
	printf("%f\r\n",ptCtrlCar->s16speedout);
	
	if(ptCtrlCar->s16speedout < 0)
	{
		ptCtrlCar->bDrection = BACK;
		ptCtrlCar->u16PWM= -ptCtrlCar->s16speedout;
	}
	
	else if(ptCtrlCar->s16speedout > 0)
	{
		ptCtrlCar->bDrection = FOWARD;
		ptCtrlCar->u16PWM=ptCtrlCar->s16speedout;
	}
	else
	{
		ptCtrlCar->u16PWM=0;
	}
	
	/***************调试信息****************/
	printf("\r\n\tPWM_SET\t");
	printf("%d\r\n",ptCtrlCar->u16PWM);
		
	/*************wyw 2017.6.20 加入新老板子区别*************/
  //SetPwmDir(ptCtrlCar);
  if(board_type ==0) SetPwmDir(ptCtrlCar);// 右侧  老板子
	
	else SetPwmDir_new(ptCtrlCar);//左侧 新板子

	
}




void Bubble_Sort()
{
	int a[10] = {1,23,45,67,89,11,33,57,11,45};
	int temp,i,j;
	for(i = 0; i< 9;i++)
	{
		for(j=i;j<9;j++)
		{
			if(a[j] < a[i])
			{
				temp = a[j];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
	
	
}

void Quick_Sort(linkqueue *  Fliter_q,int left,int right)
{
	int i = left;
	int j = right;

	
	int threshold = get_queue_value(Fliter_q,left);

	if (left > right)
	{
		return;
	}
	
	while (i < j)
	{
		while (get_queue_value(Fliter_q,j) >= threshold && i < j)
		{
			j--;
		}
		
		swap_data(Fliter_q,i,j);
		
		while (get_queue_value(Fliter_q,i) <= threshold && i < j)
		{
			i++;
		}
		
		swap_data(Fliter_q,i,j);
		

	}

	set_data(Fliter_q,i,threshold);

	
	Quick_Sort(Fliter_q, left, i - 1);

	Quick_Sort(Fliter_q, i + 1, right);
}

/*

创建一个队列
*/

void create_queue(linkqueue *  Fliter_q)
{
		
	Fliter_q->rear = Fliter_q->front = (Filter_Queue*)malloc(sizeof (Filter_Queue));
	
	if(!Fliter_q->front){

		gotoerror();
	}
	Fliter_q->front->next = NULL;
			
}

/*
   入队 
*/
void en_queue(linkqueue *  Fliter_q, uint16_t data)
{
	Filter_Queue * node;
	node = (Filter_Queue*)malloc(sizeof (Filter_Queue));
	
	if(!node){
		gotoerror();
	}
	
	node->date=data;
	node->next=NULL;
	
	
	Fliter_q->rear->next = node; //先把两个节点链起来
	Fliter_q->rear = node;       //再将尾指针向后移动一位，使其指向队尾
	
	//free(node);这里不能free，如果在这里去free，那刚刚开辟的内存空间就被释放了，这个节点随着消逝

	
}

/*

 判断一个队列是否为空，空返回1，不空返回0

*/
int is_empty(linkqueue *  Fliter_q)
{
	return Fliter_q->front == Fliter_q->rear ? 1 : 0 ;
	
}


/*
 出队，失败返回0，成功返回1
*/
int de_queue(linkqueue *  Fliter_q){
	
	Filter_Queue*  node;
	
	if(is_empty(Fliter_q))
	{
		return 0;
	}
	node = Fliter_q->front;
	
	Fliter_q->front = node->next;
	
	free(node);//一定要释放掉头结点指向的内存空间
	
	return 1;
	

	
}

/*
打印队列的所有值

*/
void printf_queue(linkqueue *  Fliter_q)
{
	Filter_Queue * node;
	
	node = Fliter_q->front;
	
	while(node)
	{
		printf("%d \r\n",node->date);
		
		node = node->next;
		
	}
}


/*
返回队列的第num个节点的data成员变量
*/
uint16_t get_queue_value(linkqueue *  Fliter_q,int num)
{
	Filter_Queue * node;
	int i=0;
	
	node = Fliter_q->front;
	
	for(i=0;i<num;i++)
	{
		node = node->next;
	}
	
	return node->date;

}








/*
   交换队列中第i个和第j个节点的data成员变量
*/
void swap_data(linkqueue *  Fliter_q,int i,int j)
{
	Filter_Queue * node1;
	
	Filter_Queue * node2;
	int k;
	uint16_t temp;
	
	node1 = Fliter_q->front;
	node2 = Fliter_q->front;
	
	for(k=0;k<i;k++)
	{
		node1 = node1->next;
	}
	
	for(k=0;k<j;k++)
	{
		node2 = node2->next;
	}
	
	temp = node1->date;
	
	node1->date = node2->date;
	node2->date = temp;
		
}


/*
   重新设置队列中第i个节点的data成员变量的值
*/
void set_data(linkqueue *  Fliter_q,int i,uint16_t value)
{
	Filter_Queue * node1;
	int k=0;
	node1 = Fliter_q->front;
	
	for(k=0;k<i;k++)
	{
		node1 = node1->next;
	}
	node1->date = value;
	
}


/*
   求队列数据的均值
*/
int average_data(linkqueue *  Fliter_q)
{
	Filter_Queue * node;
	int avg = 0;
	node = Fliter_q->front;
	
	while(node)
	{
		avg += node->date;		
		node = node->next;
		//printf("%d ",avg);
	}
	//printf("\r\n");
	return avg/10;
}


void gotoerror(void)
{
	while(1){
		
		printf("error\r\n");
		delay_ms(500);//延时时间不能超过1800，多延时就要多调用
	
	}
	
}











// 位置式pid 控制
#ifndef PID_H
#define PID_H

#define MAX_PWM 255   // 0~255
#define MIN_PWM 0
#define FOWARD  1
#define BACK    0

typedef struct pidControl
{
// pid 参数===============
float    s16speed_p;
float    s16speed_i;
float    s16speed_d;
// ======================
float	   s16error[3];//最近几次误差
float	   s16ErrorSum;//误差和
float    s16speedwant;//设定值
float    s16speedout; //输出值
// 输出=================
unsigned int  u16PWM; // 电机速度 pwm值
int      direction_pin;// 方向口
int      pwm_pin;      // pwm 口
bool     bDrection;   // 电机方向  1为前进  0为后退

}pidControl, * PpidControl; 
// pid控制器结构体，结构体指针=================

//extern pidControl pidCon;
//extern PpidControl PpidCon;

// 设置pwm和方向===
void SetPwmDir(PpidControl pidCont)
{
// 限制幅度====
  if(pidCont->u16PWM > MAX_PWM)
  {
    pidCont->u16PWM = MAX_PWM;
  }
  else if(pidCont->u16PWM < MIN_PWM)
  {
    pidCont->u16PWM = MIN_PWM;
  }

// 实际的 pwm接口 需要根据实际代码跟换==========================
  // 方向口======================================
  if(pidCont->bDrection ) 
              digitalWrite(pidCont->direction_pin, HIGH); //  HIGH向前，LOW 后退  
  else  
              digitalWrite(pidCont->direction_pin, LOW);
  // pwm值 0～255
  analogWrite(pidCont->pwm_pin, pidCont->u16PWM); 
} 

// 设置pwm
void SetPwm(PpidControl pidCont)
{
// 限制幅度====
  if(pidCont->u16PWM > MAX_PWM)
  {
    pidCont->u16PWM = MAX_PWM;
  }
  else if(pidCont->u16PWM < MIN_PWM)
  {
    pidCont->u16PWM = MIN_PWM;
  }
  // pwm值 0～255
  analogWrite(pidCont->pwm_pin, pidCont->u16PWM); 
} 

// 设置方向====
void SetDir(PpidControl pidCont)
{
// 实际的 pwm接口 需要根据实际代码跟换==========================
  // 方向口======================================
  if(pidCont->bDrection ) 
              digitalWrite(pidCont->direction_pin, HIGH); //  HIGH向前，LOW 后退  
  else  
              digitalWrite(pidCont->direction_pin, LOW);
} 

// pidCont 需要设定 s16speedwant 期望值===========================
void POSITION_PID(PpidControl pidCont,int feedback)
{
  
	pidCont->s16error[0] = pidCont->s16speedwant - (float)feedback;// 期望值-反馈值====
 
  if(abs(pidCont->s16error[0]) < 2) pidCont->s16error[0]=0;
  
	//printf("ptCtrlCar->s16ErrorSum =  %f\r\n",ptCtrlCar->s16ErrorSum);
	// 位置式pid控制=====
	pidCont->s16speedout = pidCont->s16speed_p  * pidCont->s16error[0] 
			      + pidCont->s16speed_d * (pidCont->s16error[0] - pidCont->s16error[1])
			      + pidCont->s16speed_i * pidCont->s16ErrorSum;

	pidCont->s16error[1] = pidCont->s16error[0];  // 更新误差
	pidCont->s16ErrorSum += pidCont->s16error[0]; // 误差和

	if(pidCont->s16speedout < 0)// 反转
	{
		// pidCont->bDrection = BACK;
		// pidCont->u16PWM= -pidCont->s16speedout;
    pidCont->u16PWM=0;//停止
	}

	else if(pidCont->s16speedout > 0)// 正转
	{
		// pidCont->bDrection = FOWARD;
		pidCont->u16PWM = pidCont->s16speedout;
	}
	else
	{
		pidCont->u16PWM=0;//停止
	}
 // 	SetPwmDir(pidCont);
}

#endif // PID_H

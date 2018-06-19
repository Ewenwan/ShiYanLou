#ifndef __PidContoller_H
#define __PidContoller_H

typedef enum {Speed = 0, Position = !Speed} PidControlMode;//选择速度模式或位置模式

/*绝对式PID算法，接口参数结构类型*/
typedef struct 
{
 /*PID算法接口变量，用于给用户获取或修改PID算法的特性*/
 float kp;     //比例系数
 float ki;     //积分系数
 float kd;     //微分系数
 float errILim;//误差积分上限
 
 float errNow;//当前的误差
 float ctrOut;//控制量输出
 
 /*PID算法内部变量，其值不能修改*/
 float errOld;
 float errP;
 float errI;
 float errD;
 
}PID_AbsoluteType;


/*增量式PID算法，接口参数结构类型*/
typedef struct 
{
 /*PID算法接口变量，用于给用户获取或修改PID算法的特性*/
 float kp;     //比例系数
 float ki;     //积分系数
 float kd;     //微分系数
 
 float errNow; //当前的误差
 float dCtrOut;//控制增量输出
 float  ctrOut;//控制输出
 
 /*PID算法内部变量，其值不能修改*/
 float errOld1;
 float errOld2;
 
}PID_IncrementType;


extern void get_encoder_ONE(void);
	
extern void get_encoder_TWO(void);	

extern void get_encoder_THR(void);

extern void get_encoder_FOU(void);

extern void User_PidSpeedControl(s32 SpeedTag);

extern void User_PidPositionControl(s32 Position,s32 SpeedTag);

#endif







 

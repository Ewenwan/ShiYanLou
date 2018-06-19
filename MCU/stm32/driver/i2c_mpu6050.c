#define  YCC_GYRO_GLOBALS
#include "stm32f10x.h"
#include "main.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
//定义MPU6050内部地址********************
#define SMPLRT_DV    0x19 //采样率分频，典型值:0x07(125Hz)
#define CONFIG       0x1A //低通滤波频率，典型值:0x06(5Hz)
#define GYRO_CONFIG  0x1B //陀螺仪自检及测量范围及高通滤波频率，典型值:0x18(不自检，2000deg/s)
#define ACCEL_CONFIG 0x1C //加速计自检、测量范围及高通滤波频率，典型值:0x01(不自检,2G,5Hz)
#define AX_H         0x3B //储存最近的X,Y,Z轴加速度计测量值
#define AX_L         0x3C
#define AY_H         0x3D
#define AY_L         0x3E
#define AZ_H         0x3F
#define AZ_L         0x40
#define TEMP_H       0x41 //储存最近温度传感器的测量值
#define TEMP_L       0x42
#define GX_H         0x43 //储存最近的X,Y,Z轴陀螺仪感应器的测量值
#define GX_L         0x44
#define GY_H         0x45
#define GY_L         0x46
#define GZ_H         0x47
#define GZ_L         0x48
#define PWR_M        0x6B //电源管理，典型值:0x00(正常启动)
#define FIFO_COUNT_H 0x72 //必须按从高到低的顺序读取
#define FIFO_COUNT_L 0x73
#define WHO_AM_I     0x75 //IIC地址寄存器(默认数值:0x68,只读)
#define MPU6050_Addr 0xD0  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
//******角度参数************
static float Angle_x,Angle_y,Angle_z;  //小车最终倾斜角度
static float Accel_x,Accel_y,Accel_z;  //小车加速度计算角度 
//****************************
//加速度轴和陀螺仪轴的零点漂移，取4000个数据的平均值
#define Ax_offset 0x03D2
#define Ay_offset 0xFED4
#define Az_offset 0x5956
#define Gx_offset 0x0011
#define Gy_offset 0xFFE3
#define Gz_offset 0xFFF5
#define PI        3.14
#define gyro_time 0.01  //s
//******卡尔曼参数************                
float Q_angle=0.001;  
float Q_gyro=0.003;
float R_angle=0.5;
float dt=0.01;                          //dt为kalman滤波器采样时间;
float C_0 = 1.0;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };
//*********************************************************
typedef struct
{
float ax;
float ay;
float az;
float gx;
float gy;
float gz;
float temp;
}GYRO_INFO;

static GYRO_INFO gyro_info;
 
static void Single_Write(u8 register_add,u8 date)
{
I2C_Bus(I2C_DEV_MPU3050,I2CMD_IOTODEV,register_add,&date,1);
}

static s16 Single_Read(u8 register_add)
{
u16 temp;
u8 date[2];
I2C_Bus(I2C_DEV_MPU3050,I2CMD_DEVTOIO,register_add,date,2);
temp= (date[0]<<8)|date[1];
return (s16)temp;
}

void Gyro_Init()
{
   Single_Write(PWR_M, 0x00);   //重置所有寄存器，温度传感器失能
   Single_Write(SMPLRT_DV,0x07);//陀螺仪&加速度计采样率，典型值：0x07(125Hz)
   Single_Write(CONFIG,0x06);   //低通滤波频率，典型值：0x06(5Hz)
   Single_Write(GYRO_CONFIG,0x08);//(+-500deg/s
//   Single_Write(GYRO_CONFIG,0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
   Single_Write(ACCEL_CONFIG,0x01);//加速度计(不自检，2g)
}
 
//******读取MPU3050数据****************************************
static u8 READ_MPU3050()
{
u8 power=0x40,ok;
s16 temp[3];
I2C_Bus(I2C_DEV_MPU3050,I2CMD_DEVTOIO,PWR_M,&power,1);
   if(power != 0x40)
   { 
      temp[0] = Single_Read(AX_H);
      temp[1] = Single_Read(AY_H);
      temp[2] = Single_Read(AZ_H);
      gyro_info.ax=(s16)(temp[0])/16384.00; //范围为2g时，换算关系：16384 LSB/g
      gyro_info.ay=(s16)(temp[1])/16384.00; //角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14
      gyro_info.az=(s16)(temp[2])/16384.00; //因为x>=sinx,故乘以1.3适当放大
      gyro_info.gx=(s16)(Single_Read(GX_H)-Gx_offset)/65.5;///16.4; 
      gyro_info.gy=(s16)(Single_Read(GY_H)-Gy_offset)/65.5;///16.4;
      gyro_info.gz=(s16)(Single_Read(GZ_H)-Gz_offset)/65.5;///16.4;
/*   Accel_x = atan(gyro_info.ay/gyro_info.az)*180/PI;
  Accel_y = atan(gyro_info.ax/gyro_info.az)*180/PI;*/
      Accel_x =asin((s16)(temp[0]-Ax_offset)/16384.00)*180/PI;
      Accel_y =asin((s16)(temp[1]-Ay_offset)/16384.00)*180/PI;
      Accel_z =asin((s16)(temp[2]-Az_offset)/16384.00)*180/PI;
     ok=1;
   }
   else 
   {
     Gyro_Init();
     ok=0;
   }
   return ok;
}
/*********************************************************/
/*************倾角计算(卡尔曼滤波融合)********************/
/*********************************************************/
//Kalman滤波，20MHz的处理时间约0.77ms；
void Kalman_Filter(float Accel,float* Gyro,float* outAngle)//,float*outAngleDot)
{
//static float Angle,AngleDot;
*outAngle+=(*Gyro - Q_bias) * dt; //先验估计
Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
Pdot[1]=- PP[1][1];
Pdot[2]=- PP[1][1];
Pdot[3]=Q_gyro;
PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
PP[1][0] += Pdot[2] * dt;
PP[1][1] += Pdot[3] * dt;
Angle_err = Accel -*outAngle;//zk-先验估计
PCt_0 = C_0 * PP[0][0];
PCt_1 = C_0 * PP[1][0];
E = R_angle + C_0 * PCt_0;
K_0 = PCt_0 / E;
K_1 = PCt_1 / E;
t_0 = PCt_0;
t_1 = C_0 * PP[0][1];
PP[0][0] -= K_0 * t_0; //后验估计误差协方差
PP[0][1] -= K_0 * t_1;
PP[1][0] -= K_1 * t_0;
PP[1][1] -= K_1 * t_1;
*outAngle+= K_0 * Angle_err; //后验估计
Q_bias+= K_1 * Angle_err; //后验估计
*Gyro = *Gyro - Q_bias; //输出值(后验估计)的微分=角速度
 
//*outAngle=*outAngle;
//*outAngleDot=AngleDot;
}
 
/******************互补滤波**********************************
*补偿原理是取当前倾角和加速度获得倾角差值进行放大，然后与
* 陀螺仪角速度叠加后再积分，从而使倾角最跟踪为加速度获得的角度
* 0.5为放大倍数，可调节补偿度；gyro_time为系统周期10ms         
**************************************************************/
 void Angle_Calcu(void) 
{
READ_MPU3050();
Kalman_Filter(Accel_y,&gyro_info.gx,&Angle_x);
    Kalman_Filter(Accel_x,&gyro_info.gy,&Angle_y);
//Kalman_Filter(Angle_z,Accel_z,gyro_info.gz);
//Angle_x = Angle_x +(((Accel_y-Angle_x)*0.5 + gyro_info.gx)*gyro_time);
//Angle_y = Angle_y +(((Accel_x-Angle_y)*0.5 + gyro_info.gy)*gyro_time);
//Angle_z = Angle_z +(((Accel_x-Angle_y)*0.5 + gyro_info.gy)*gyro_time);
}
#ifdef DEBUG_MODE
void Gyro_Report()
{
float* p;
u8 i,len,temp[50];
char str1[2]={'a','g'};
char str2[3]={'x','y','z'};
float Angle[3];
p=(float*)(&gyro_info);
//p=(s16*)(&gyro_offset);
Angle[0]=Angle_x;
Angle[1]=Angle_y;
Angle[2]=Angle_z;
if(I2C_Probe(I2C_DEV_MPU3050)==1)
{
for(i=0;i<6;i++)
{
if(i<3)  len=sprintf((char*)temp,"%c%c:%.2f",str1[0],str2[i],*p++);
else     len=sprintf((char*)temp,"%c%c:%.2f",str1[1],str2[i-3],*p++);
Host_Report_Event(HOST_EVENT_DEBUG,temp,len);
}
for(i=0;i<3;i++)
{
len=sprintf((char*)temp,"Angle%c:%.2f",str2[i],Angle[i]);
Host_Report_Event(HOST_EVENT_DEBUG,temp,len);
}
}
}
#endif
 
void Gyro_Main()
{ 
 
Angle_Calcu();
 
} 

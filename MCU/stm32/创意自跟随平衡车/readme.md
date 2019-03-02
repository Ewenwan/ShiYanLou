# 创意自跟随平衡车 两轮平衡车的设计与制作
[参考](https://github.com/njuliuyang/2018-EducationalRobot-PosionModule/tree/master/%E5%88%9B%E6%84%8F%E8%87%AA%E8%B7%9F%E9%9A%8F%E5%B9%B3%E8%A1%A1%E8%BD%A6)

STM32F103 + 惯性传感器MPU6050 + 超声波测距自动跟随 + 蓝牙通信遥控

本平衡车控制系统的设计以STM32F103系列的MCU作为主控，通过惯性传感器MPU6050采集姿态数据，实现实时姿态的测量，用蓝牙完成遥控器或者超声波到平衡车的无线通信，使用TB6612FNG的电机驱动IC，然后再加入电源电路、按键控制电路以及LED指示灯电路组成最后的平衡车控制电路板。


用意法半导体公司的STM32F103系列的微控制器作为本控制系统的主控芯片，使用惯性传感器MPU6050实时采集得到加速度的值及角速度的值，经过卡尔曼滤波算法融合加速度和角度计算出两轮平衡车系统的倾角，并通过PID控制器计算出控制PWM，通过PWM信号输出到电机驱动上，从而控制电机的速度和方向，从而控制平衡车的平衡和简单行走。


平衡车的姿态，指的是平衡车在水平方向的倾角，一般可以用欧拉角来描述，包括横滚角（roll）、俯仰角（pitch）和偏航角（yaw）。根据传感器的安装位置不同，平衡车的姿态角在横滚角和俯仰角之间得到。


要实现平衡车自动跟随的功能，需要选用能获取当前小车的位置和人的位置的传感器。本文选用单发单收的超声波模块进行测距，在车上的两端各安装一个单收超声波，人手持一个单发的超声波模块。这样左侧与右侧离人的距离就构成了一个三角形。简单示意图如下图2-3所示。当小车正对着人时，距离A=B，当人左拐时，A必定小于B，同理当人右拐时，A大于B。当人向前走时，A和B的距离必定大于设定距离。由此可获得人与车的距离以及人的行走路线，来实现小车的自动跟随功能。

> 软件系统结构

![](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/stm32/%E5%88%9B%E6%84%8F%E8%87%AA%E8%B7%9F%E9%9A%8F%E5%B9%B3%E8%A1%A1%E8%BD%A6/struct.png)


其中系统模块初始化需要完成的工作就是初始化好主控的各个IO口，完成IIC、USART及ADC的初始化，然后再完成传感器、无线模块、超声波模块的初始化配置。

之后便进入整个控制系统的大循环体中依次执行相关的任务。

检测按键就是通过按键设置平衡车的工作模式，第一种是通过遥控器通过蓝牙通信，获得操控着对平衡车的操控命令，比如说加减速，前进后退等。第二种模式，小车自动跟随手上持有超声波发射器的人。

采集传感器数据第一个是主控芯片通过IIC总线与MPU6050惯性传感器通信，得到传感器采集的原始数据，然后在做一些滤波处理，给后续的姿态解算提供一个可靠的数据，第二个是主控芯片通过USART与蓝牙模块通信，为模式一提供数据。第三个是主控芯片通过USART与超声波模块通信，为模式二提供数据。

更新当前的姿态是主控根据传感器所采集到的有效数据计算出当前姿态的过程，这部分姿态解算程序的设计可以算得上是整个软件系统设计中最核心的部分，因为这部分的程序设计的好坏可以直接影响到最终平衡车行走的效果。
在解算出当前的姿态和得到操控者的控制命令后，就需要计算出相应的控制输出量，然后输出控制，以达到操控着的命令要求。

其实有上面这些步骤已经可以保证平衡车稳定运行，但是为了保证平衡车的安全性，还需要时刻检测电池电压的状态防止损坏小车以及电池过放。

出于对控制系统中对陀螺仪积分运算的要求，保证每次积分时间固定且可靠，要求程序主题循环每10ms执行一次，即积分时间为0.01s，姿态、控制的刷新频率为100Hz。


## 遥控模式
![](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/stm32/%E5%88%9B%E6%84%8F%E8%87%AA%E8%B7%9F%E9%9A%8F%E5%B9%B3%E8%A1%A1%E8%BD%A6/yaokong.png)


## 自动跟随模式
![](https://github.com/Ewenwan/ShiYanLou/blob/master/MCU/stm32/%E5%88%9B%E6%84%8F%E8%87%AA%E8%B7%9F%E9%9A%8F%E5%B9%B3%E8%A1%A1%E8%BD%A6/auto.png)



## 姿态解算C语言实现

得到MPU6050可靠数据之后，还需要使用这些数据来解算出当前的平衡车的姿态。MPU6050中陀螺仪可以得到当前平衡车系统XYZ三个方向的角速度，按理说，通过对角速度积分，便能得到当前系统的姿态角，但是陀螺仪本身极易受噪声干扰，产生漂移误差，误差会越来越大。目前常用于姿态解算算法有卡尔曼滤波算法以及互补滤波算法。

> **卡尔曼滤波算法**

卡尔曼滤波算法是一种线性最小方差估计，是一种离散线性滤波的递推算法，姿态计算的精度很高，在平衡车系统中，卡尔曼滤波利用传统姿态结算方法建立滤波模型，然后通过预测更新过程得到下一时刻的姿态角。
卡尔曼滤波算法的C语言程序实现如下：

```c
float K1 =0.02; 
float angle, angle_dot; 	
float Q_angle=0.001;
float Q_gyro=0.003;
float R_angle=0.5;
float dt=0.005;                 
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };
void Kalman_Filter(float Accel,float Gyro)		
{
	angle+=(Gyro - Q_bias) * dt; //先验估计
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; 
	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;  
	PP[0][1] += Pdot[1] * dt;  
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;		
	Angle_err = Accel - angle;	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;                     		
	angle	+= K_0 * Angle_err;
	Q_bias	+= K_1 * Angle_err;
	angle_dot   = Gyro - Q_bias;
}
```

> **互补滤波法**

互补滤波法的基本解算思路：把加速度计的误差构造成纠正旋转，然后叠加到陀螺仪测量出来的角增量上来纠正陀螺仪的积分误差。

互补滤波法的C语言程序实现如下：
```c
void sensfusion6UpdateQ(float gx, float gy, float gz, float ax, float ay, float az, float dt)
{
  float recipNorm;
  float halfvx, halfvy, halfvz;
  float halfex, halfey, halfez;
  float qa, qb, qc;
  gx = gx * M_PI / 180;
  gy = gy * M_PI / 180;
  gz = gz * M_PI / 180;
  if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
  {
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;
    halfvx = q1 * q3 - q0 * q2;
    halfvy = q0 * q1 + q2 * q3;
    halfvz = q0 * q0 - 0.5f + q3 * q3;
         halfex = (ay * halfvz - az * halfvy);
    halfey = (az * halfvx - ax * halfvz);
    halfez = (ax * halfvy - ay * halfvx);
    if(twoKi > 0.0f)
    {
      integralFBx += twoKi * halfex * dt;  
      integralFBy += twoKi * halfey * dt;
      integralFBz += twoKi * halfez * dt;
      gx += integralFBx; 
      gy += integralFBy;
      gz += integralFBz;
    }
    else
    {
      integralFBx = 0.0f; 
      integralFBy = 0.0f;
      integralFBz = 0.0f;
    }
    gx += twoKp * halfex;
    gy += twoKp * halfey;
    gz += twoKp * halfez;
  }
  gx *= (0.5f * dt);  
  gy *= (0.5f * dt);
  gz *= (0.5f * dt);
  qa = q0;
  qb = q1;
  qc = q2;
  q0 += (-qb * gx - qc * gy - q3 * gz);
  q1 += (qa * gx + qc * gz - q3 * gy);
  q2 += (qa * gy - qb * gz + q3 * gx);
  q3 += (qa * gz + qb * gy - qc * gx);
  recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}
```


## 经典PID控制器C语言实现

PID控制的优点在于不需要精确的数学建模，只需要知道姿态偏差就能够通过参数整定满足要求的动态响应。
本平衡车控制系统软件中的PID控制器的输入量是目标姿态与当前姿态的偏差，输出量是绕三个轴的力矩，最后对应到两个电机的转速上去。

PID控制器的C语言程序实现如下：
```c
float pidUpdate(PidObject* pid, const float measured, const bool updateError)
{
  float output;
  if (updateError)
  {
    pid->error = pid->desired - measured;
  }
  pid->integ += pid->error * IMU_UPDATE_DT;
  if (pid->integ > pid->iLimit)
  {
    pid->integ = pid->iLimit;
  }
  else if (pid->integ < -pid->iLimit)
  {
    pid->integ = -pid->iLimit;
  }
  pid->deriv = (pid->error - pid->prevError) / IMU_UPDATE_DT;

  pid->outP = pid->kp * pid->error;
  pid->outI = pid->ki * pid->integ;
  pid->outD = pid->kd * pid->deriv;

  output = (pid->kp * pid->error) +
           (pid->ki * pid->integ) +
           (pid->kd * pid->deriv);

  pid->prevError = pid->error;

  return output;
}
```

//#define test_motor // 测试电机，去除的话，电机未收到消息不会转动

// mega 2560  
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

//#define USE_USBCON
#include <ros.h>
#include <std_msgs/UInt32.h>

#include <FlexiTimer2.h>// 系统任务定时器 
//#include <PWM.h> // 有一个单独的函数可以配置pwm波频率，再使用 analogwrite() 设置pwm

//从上位机接收到的指令解析后的数据
float angle_speed_B = 0.0;//B车角速度   bit6 - bit 0 （3.14弧度/s 等分128）
bool direction_angle_speed_B = 0;//  bit 7 0-- 逆时针  1  -- 顺时针转  
float liner_speed_B = 0.0;//B车线速度  bit15 - bit8 0.2m/s 等分256
float liner_speed_A = 0.0;//A车线速度  bit23 - bit16 0.2m/s 等分256
bool servo_1 = 0;//清洗面6的摆动舵机  bit24   1 摆动   0  不摆动
bool direction_A = 0;//A车前进方向 bit25  1 向前    0 向后
bool direction_B = 0;//B车前进方向 bit26  1 向前    0 向后
bool servo_2 = 0;//抬升面5/7刷的舵机 bit27  1 往复    0 停止归位
bool motor_1_value = 0;//清洗面6的电机    bit28      1 旋转  0 不转
bool motor_2_value = 0;//清洗面5/7的电机    bit29      1 旋转  0 不转
bool motor_3_value = 0;//清洗面3/4的电机    bit30      1 旋转  0 不转
bool motor_4_value = 0;//清洗面1/2的电机    bit31      1 旋转  0 不转

// 发送消息的变量====================
//sensor
int battery_percent = 0;//0-100表示电池电量百分比  bit6-bit0
bool battery_state = 0;//bit 7  0 -- 放电中  1 -- 充电中
volatile int Infrared_1 = 0;//1号红外测距数据  bit8-bit15
volatile int Infrared_2 = 0;//2号红外测距数据  bit16-bit23
volatile int Infrared_3 = 0;//3号红外测距数据  bit24-bit31

//sensor_1
volatile int Infrared_4 = 0;//4号红外测距数据  bit0-bit7
volatile int Infrared_5 = 0;//5号红外测距数据  bit8-bit15
volatile int Drawstring = 0;//拉绳传感器数据  bit16-bit23

//
bool led_on_off_flag=0;
//int led_count=0;
//bool led__ = false;


//ROS参数定义
ros::NodeHandle   nh;                               //创建ROS句柄
void Motor_contrl( const std_msgs::UInt32& cmd_msg);//函数声明
void Motor_1_contrl( const std_msgs::UInt32& cmd_msg);//函数声明
         
//定义编码器脉冲广播消息格式
std_msgs::UInt32  Sensor_msg; 
// 定义向上位机发送指令，Publisher-发送，Sensor-，"Sensor"-消息头，Sensor_msg-待发送的消息结构
ros::Publisher Sensor("Sensor", &Sensor_msg); 

std_msgs::UInt32  Sensor_msg_1;              
ros::Publisher Sensor_1("Sensor_1", &Sensor_msg_1);  
               
// 定义从上位机接收话题，Subscriber-接收，std_msgs-，"motor"-消息头，Motor_contrl-得到上位机指令后调用的函数
ros::Subscriber<std_msgs::UInt32> sub("motor", Motor_contrl);
ros::Subscriber<std_msgs::UInt32> sub1("motor_1", Motor_1_contrl);


//LED参数
long int led_flag=0;//当前LED状态
int led_flag_last=0;//上一LED状态
const int ledPin =  LED_BUILTIN;//LED管脚


//拉绳参数定义
const int analogInPin = A0;  // 线位移传感器模拟量输出口
volatile int  sensorValue = 0;        //模拟量输出口的电压值

// 串口2接收============== 
//电源模块,充电时，电源模块红线接充电器的负，黑线接电池的负
unsigned char data[15];//保存传输过来的15个字节
unsigned char count=0;//计数
int eletric_percent;//


///*
// 舵机的实现 定时器1
#include <Servo.h>

//----面6舵机的代码---
Servo myservo;  // 创建 servo 对象来控制一个 servo （面6舵机）
const int pin = 11;//面6舵机的控制信号引脚
const int ang_pin = A1;//将舵机的角度输出信号连接至A1（调整面6角度的舵机）

bool zf_flag = 0;//符号标志选择正反转 0 正 1500 ->1900  1 反
int r_zl = 2;    //每次增量
int min_r = 600; //最小位置（500，若设置为1500，水平位置）//600，最小适合值
int r_current = min_r;//当前位置
int max_r =2400;  //最大角度位置//2400最大合适值
float angle;      // 舵机反馈的角度值 对应角度值
int   angle_int;
int   angle_int_100ms;//100ms时对应的角度
bool direction_change_flag = 0; // 舵机转向变化flag 向任意一个方向转动到转不动了，就需要改变方向。

// 20ms  舵机控制 任务 （控制面6摆动的舵机） =========================
void servo_control(){
   if(r_current <=  min_r)   zf_flag = 0;//当前位置小于等于最小位置，标志位置0
  else if(r_current >=  max_r) zf_flag = 1;//当前位置大于等于最大位置，标志位置1
  if(zf_flag) r_current -=  r_zl;//正转
  else r_current +=  r_zl;//反转
  myservo.writeMicroseconds(r_current);//向9脚写入当前状态
  angle = (analogRead(ang_pin)*(-2.132) + 2592.7)/100*15;
  angle_int = angle;
}
//--- end 面6舵机代码-----


//----面5/7舵机的代码---
Servo myservo2;  // 创建 servo 对象来控制一个 servo （面5/7舵机）
const int pin2 = 12;//面5/7舵机的控制信号引脚
const int ang_pin2 = A2;//将舵机的角度输出信号连接至A3 2（调整面5/7角度的舵机）

bool zf2_flag = 0;//符号标志选择正反转 0 正 1500 ->1900  1 反
int r2_zl = 2;    //每次增量
int min_r2 = 600; //最小位置（500，若设置为1500，水平位置）//600，最小适合值
int r2_current = min_r;//当前位置
int max_r2 =2400;  //最大角度位置//2400最大合适值
float angle2;      // 舵机反馈的角度值 对应角度值
int   angle2_int;
int   angle2_int_100ms;//100ms时对应的角度
bool direction2_change_flag = 0; // 舵机转向变化flag 向任意一个方向转动到转不动了，就需要改变方向。

void servo2_control(){
	if(r2_current <=  min_r2)
		zf2_flag = 0;//当前位置小于等于最小位置，标志位置0
	else if(r2_current >=  max_r2) 
		zf2_flag = 1;//当前位置大于等于最大位置，标志位置1
  
	if(zf2_flag) 
		r2_current -=  r2_zl;//正转
	else 
		r2_current +=  r2_zl;//反转
  
	myservo2.writeMicroseconds(r2_current);//向9脚写入当前状态
	angle2 = (analogRead(ang_pin2)*(-2.132) + 2592.7)/100*15;
	angle2_int = angle2;
}
//--- end 面5/7舵机代码-----




//*/

////////////////////超声模块////////////////////////////////////////
const int  trigger_io = 4;  // 触发口
const int  pwm_io_1 = 2;
const int  pwm_io_2 = 3;
const int  pwm_io_3 = 21;// I2C口占用
const int  pwm_io_4 = 20;
const int  pwm_io_5 = 18;// 串口1占用
unsigned long DYP_PulseStartTicks_1,DYP_PulseStartTicks_2,DYP_PulseStartTicks_3,DYP_PulseStartTicks_4,DYP_PulseStartTicks_5;     
volatile int DYP_val = 0 ;
volatile float DYP_distalce = 0.0;


void DYP_plus_measure_1(void)
{
        if (digitalRead( pwm_io_1 ) == HIGH)// 低电平到高电平的 时间点
                DYP_PulseStartTicks_1 = micros();    // 微秒 us  
        else// 高电平变成低电平的 时间点
                // DYP_val = micros() - DYP_PulseStartTicks_1;
                Infrared_1 = (micros() - DYP_PulseStartTicks_1)/ 58.0;
        //Infrared_1 = DYP_val / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
 
}
void DYP_plus_measure_2(void)
{
        if (digitalRead( pwm_io_2 ) == HIGH)// 低电平到高电平的 时间点
                DYP_PulseStartTicks_2 = micros();    // 微秒 us  
        else// 高电平变成低电平的 时间点
              //  DYP_val = micros() - DYP_PulseStartTicks_2;
        Infrared_2 = (micros() - DYP_PulseStartTicks_2) / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
 
}
void DYP_plus_measure_3(void)
{
        if (digitalRead( pwm_io_3 ) == HIGH)// 低电平到高电平的 时间点
                DYP_PulseStartTicks_3 = micros();    // 微秒 us  
        else// 高电平变成低电平的 时间点
               // DYP_val = micros() - DYP_PulseStartTicks_3;
            Infrared_3 = (micros() - DYP_PulseStartTicks_3) / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
}
void DYP_plus_measure_4(void)
{
        if (digitalRead( pwm_io_4 ) == HIGH)// 低电平到高电平的 时间点
                DYP_PulseStartTicks_4 = micros();    // 微秒 us  
        else// 高电平变成低电平的 时间点
               // DYP_val = micros() - DYP_PulseStartTicks_4;
              Infrared_4 = (micros() - DYP_PulseStartTicks_4) / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
 
}
void DYP_plus_measure_5(void)
{
        if (digitalRead( pwm_io_5 ) == HIGH)// 低电平到高电平的 时间点
                DYP_PulseStartTicks_5 = micros();    // 微秒 us  
        else// 高电平变成低电平的 时间点
               // DYP_val = micros() - DYP_PulseStartTicks_5;
             Infrared_5 = (micros() - DYP_PulseStartTicks_5) / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
 
}
// 超声波 触发信号=======
int count_2ms = 0;
const int trigger_time_zones = 50;// 50*2ms =100ms 触发周期
const int trigger_time_duration = 1;// 持续时间
void  sensor_trigger(void)
{
   count_2ms++;
   if(count_2ms == trigger_time_zones) 
   {
     digitalWrite(trigger_io, HIGH);
     if(DYP_distalce > 0) Serial.println(DYP_distalce);
    }
   if(count_2ms >= trigger_time_zones) count_2ms = 0;
   if(count_2ms == trigger_time_duration) digitalWrite(trigger_io, LOW);
}

int Motor_contrl_msg_count = 0;// ros话题接收消息次数 计数
int Motor_contrl_msg_count_100ms = 0;// 100ms时的计数
bool master_firefly_online = 0;// 主机在线标志，在发送消息


///////////////////////电机参数定义////////////////////////////////////////
//后轮电机
#define Frequence     18000 // PWM 波频率

//B车=======================================
/*
#define PWM_left_front        6  // 左轮电机 pwm 口   6  TIMER3 pwm频率库肯不支持
#define PWM_right_front        7  // 右轮电机 PWM 口   7
#define PWM_left_behind         8  // 左轮电机 pwm 口   6  TIMER3 pwm频率库肯不支持
#define PWM_right_behind        44  // 右轮电机 PWM 口   7
*/
#define DIR_left_front         5 // 左轮方向 IO口
#define DIR_right_front        13 // 右轮方向 IO口
#define DIR_left_behind         9 // 左轮方向 IO口
#define DIR_right_behind        10 // 右轮方向 IO口

float vel_l = 0;//左轮转速
float vel_r = 0;//右轮转速

int PWM_left_value = 0;
int PWM_right_value = 0;

float car_length = 0.28;
//普通电机口=====================================
#define motor_1    42
#define motor_2    40
#define motor_3    38
#define motor_4    36

//A车==================================
/*
#define PWM_A_1        45  
#define PWM_A_2        46// 预留接口 
*/
#define PWM_A        6  // A
#define PWM_left        7  // B-L
#define PWM_right         8  // B-R
#define DIR_A          47 
float vel_A = 0;//转速
int PWM_A_value = 0;


/////////////2ms执行一次定时器中断服务程序=======================
int send_ros_count = 0;
int servo_count = 0;
int servo_count2 = 0;
int Motor_contrl_msg_time_count=0;
int servo_change_dir_dect_time_count = 0;
bool servo_run_flag = 0;// 舵机运动的标志，来自 上位机给的指令

// int temp1, temp2, temp3,temp4; // 舵机多次测量差值
int servo_angle_diff_filter=0;
int servo_angle_diff_filter_thersh = 5;
bool change_flag_last=0;
bool sensor_sample_fla=0;
void send_ros();
void task(){
	// 超声波传感器触发信号，测量任务会自动IO口中断执行=========================
	sensor_trigger();
  
	// 20ms  舵机控制 任务（清洗面6的舵机控制）=========================
	if(servo_1){		
		servo_count++;
		if(servo_count >= 10)
		{
			servo_control();
			servo_count = 0;
		}
	}	 

	// 20ms  舵机控制 任务（清洗面5/7的舵机控制）=========================
	if(servo_2){		
		servo_count2++;
		if(servo_count2 >= 10)
		{
			servo2_control();
			servo_count2 = 0;
		}
	}	 
	
	// ros消息上传=====================
	send_ros_count++;
  if(send_ros_count >= 200)
  {
    sensor_sample_fla = 1;// 模拟量采样任务
    send_ros();           // ros消息上传
    send_ros_count = 0;
  }

// 主机在线离线判断===============================
  Motor_contrl_msg_time_count++;
  if(Motor_contrl_msg_time_count == 50)  Motor_contrl_msg_count_100ms = Motor_contrl_msg_count;// 100ms接收消息次数
  if(Motor_contrl_msg_time_count >= 250) { // 500ms
     if( Motor_contrl_msg_count > Motor_contrl_msg_count_100ms) master_firefly_online = 1;// 主机在线
     else master_firefly_online = 0;// 主机离线
     
     Motor_contrl_msg_time_count=0;// 时间计数清零   500ms周期判断主机是否在线        
     Motor_contrl_msg_count = 0;   // 消息计数清零
    }
	
// 舵机调换方向判断=========转不动了改变方向============== servo1
  servo_change_dir_dect_time_count++;
  if(servo_change_dir_dect_time_count == 50) angle_int_100ms = angle_int; // 100ms时的角度值
  if(servo_change_dir_dect_time_count >= 500){//1s
    int temp = abs(angle_int - angle_int_100ms);// 两次 角度差值绝对值
    servo_angle_diff_filter_thersh = (servo_angle_diff_filter_thersh*9 + temp)/10;
    servo_angle_diff_filter = (servo_angle_diff_filter*2 + temp)/3;
   
   // Serial3.println(servo_angle_diff_filter);// 打印看看正常情况下 会有多少的 角度差
   
   Serial3.print(PWM_left_value);// 打印看看正常情况下 会有多少的 角度差
   Serial3.print("  ");
   Serial3.println(PWM_right_value);// 打印看看正常情况下 会有多少的 角度差 
    
    // 或者这里 使用 temp的平均值来作为阈值（自适应）
    // 小于阈值，舵机可能堵住了=====
    if(change_flag_last && ( servo_angle_diff_filter < servo_angle_diff_filter_thersh-2))// 可能需要多次都小于一个值才能够判断出 舵机堵转 500ms正常差值 6~7   1s差值 11~20
     direction_change_flag = 1;// 舵机转不动了， 变换转动方向
     // else 

   // 上一次标志
    change_flag_last = (servo_angle_diff_filter < (servo_angle_diff_filter_thersh-2));

    
    servo_change_dir_dect_time_count = 0;
    }
}
// 舵机调换方向判断 ==== servo2=================


/////////////////////////////PWM实现函数//////////////////////////////////////////
void setPwmFrequency2560(int pin, int divisor) {
  byte mode;
  if((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46)) 
  {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
      if(pin == 4 || pin == 13)//Timer0
      {
        TCCR0B = TCCR0B & 0b11111000 | mode;  
      }
      else if(pin == 11 || pin == 12 || pin == 13)//Timer1
      {
        TCCR1B = TCCR1B & 0b11111000 | mode;  
      }
      else if(pin == 8 || pin == 9)//Timer2
      {
        TCCR2B = TCCR2B & 0b11111000 | mode;  
      }
      else if(pin == 5 || pin == 2 || pin == 3)//Timer3
      {
        TCCR3B = TCCR3B & 0b11111000 | mode;  
      }
      else if(pin == 6 || pin == 7 || pin == 8)//Timer4
      {
        TCCR4B = TCCR4B & 0b11111000 | mode;    
      }
      else if(pin == 46 || pin == 45 || pin == 44)//Timer5
      {
        TCCR5B = TCCR5B & 0b11111000 | mode;    
      }
  } 
}


// 订阅话题的回调函数(得到上位机指令后，调用该函数进行解析)
void Motor_contrl( const std_msgs::UInt32& cmd_msg)
{  
  angle_speed_B =           (cmd_msg.data & 0x0000007f)*3.14/128;
  direction_angle_speed_B = (cmd_msg.data & 0x00000080)>>7;
  liner_speed_B =          ((cmd_msg.data & 0x0000ff00)>>8)*0.2/256;
  liner_speed_A =          ((cmd_msg.data & 0x00ff0000)>>16)*0.2/256;

//A车速度解析==========================================  
PWM_A_value = (int)(liner_speed_A * 255 / 0.2 );
if (PWM_A_value>253) PWM_A_value = 253;

//B车速度解析==========================================
if(direction_angle_speed_B)
{
  vel_l = (2*liner_speed_B - car_length*angle_speed_B)/2;//左轮转速
  vel_r = (2*liner_speed_B + car_length*angle_speed_B)/2;//右轮转速
}
else
{
  vel_l = (2*liner_speed_B + car_length*angle_speed_B)/2;//左轮转速
  vel_r = (2*liner_speed_B - car_length*angle_speed_B)/2;//右轮转速
}

  PWM_left_value = abs(vel_l) * 255  / 0.2;
  PWM_right_value = abs(vel_r) * 255  / 0.2;
  
  if (PWM_left_value>253) PWM_left_value = 253;
  if (PWM_right_value>253) PWM_right_value = 253;
  
  
  servo_1 =                 (cmd_msg.data & 0x01000000)>>24;
  direction_A =             (cmd_msg.data & 0x02000000)>>25;
  direction_B =             (cmd_msg.data & 0x04000000)>>26;
  servo_2 = (cmd_msg.data & 0x08000000)>>27;
   motor_1_value = (cmd_msg.data & 0x10000000)>>28;
  motor_2_value = (cmd_msg.data & 0x20000000)>>29;
  motor_3_value = (cmd_msg.data & 0x40000000)>>30;
  motor_4_value = (cmd_msg.data & 0x80000000)>>31;

 //普通电机口===================================== 
if(motor_1_value) digitalWrite(motor_1, HIGH);
else  digitalWrite(motor_1, LOW);

if(motor_2_value) digitalWrite(motor_2, HIGH);
else  digitalWrite(motor_2, LOW);

if(motor_3_value||motor_4_value)
{
  digitalWrite(motor_3, HIGH);
  digitalWrite(motor_4, HIGH);
}
else  
{
  digitalWrite(motor_3, LOW);
  digitalWrite(motor_4, LOW);
}



//A车电机方向口======================================
  if(direction_A) digitalWrite(DIR_A, HIGH); // LOW 后退   HIGH向前 
  else  digitalWrite(DIR_A, LOW);

//A车电机PWM口 ===========================
/*
   analogWrite(PWM_A_1,PWM_A_value);            
   analogWrite(PWM_A_2,PWM_A_value); 
*/
    analogWrite(PWM_A,PWM_A_value); 
//B车电机方向口======================================
  if(direction_B)
  {
   if (vel_l >= 0) 
   {
   digitalWrite(DIR_left_front, HIGH); // LOW 后退   HIGH向前  
   digitalWrite(DIR_left_behind, HIGH);
   }
   else 
   {
   digitalWrite(DIR_left_front, LOW); // LOW 后退   HIGH向前  
   digitalWrite(DIR_left_behind, LOW);
   }
   if (vel_r >= 0) 
   {
   digitalWrite(DIR_right_front, HIGH); // LOW 后退   HIGH向前  
   digitalWrite(DIR_right_behind, HIGH);
   }
   else 
   {
   digitalWrite(DIR_right_front, LOW); // LOW 后退   HIGH向前  
   digitalWrite(DIR_right_behind, LOW);
   }
  }
  else
   {
   if (vel_l >= 0) 
   {
   digitalWrite(DIR_left_front, LOW); // LOW 后退   HIGH向前  
   digitalWrite(DIR_left_behind, LOW);
   }
   else 
   {
   digitalWrite(DIR_left_front, HIGH); // LOW 后退   HIGH向前  
   digitalWrite(DIR_left_behind, HIGH);
   }
 
   if (vel_r >= 0) 
   {
   digitalWrite(DIR_right_front, LOW); // LOW 后退   HIGH向前  
   digitalWrite(DIR_right_behind, LOW);
   }
   else 
   {
   digitalWrite(DIR_right_front, HIGH); // LOW 后退   HIGH向前  
   digitalWrite(DIR_right_behind, HIGH);
   }
  }

//B车电机PWM口 ===========================
/*
   analogWrite(PWM_left_front,PWM_left_value);           
   analogWrite(PWM_left_behind,PWM_left_value);          
   analogWrite(PWM_right_front, PWM_right_value);         
   analogWrite(PWM_right_behind, PWM_right_value);

*/
    analogWrite(PWM_left,PWM_left_value);          
   analogWrite(PWM_right, PWM_right_value);
  
  //*/
  
 // led__ = !led__;
 // digitalWrite(ledPin, !led__);

 Motor_contrl_msg_count++; // 接收上位机消息次数++，用于判断 上位机是否在发送消息=====
}

// 订阅话题的回调函数(得到上位机指令后，调用该函数进行解析,该函数对面6的舵机的摆动角度进行设置)
void Motor_1_contrl( const std_msgs::UInt32& cmd_msg)
{  
///*
  min_r =           (cmd_msg.data & 0x000000ff);    //------需要换算-----------------
  max_r =          ((cmd_msg.data & 0x0000ff00)>>8); 
}

int Infrared_1_filter=0;

//发送消息的函数,
void send_ros(){
  
  nh.spinOnce();

  Sensor_msg.data=        (long(Infrared_3               ) & 0xffffffff)<<24
                         |(long(Infrared_2               ) & 0xffffffff)<<16 
                         |(long(Infrared_1               ) & 0xffffffff)<<8    
                         |(long( battery_state           ) & 0xffffffff)<<7
                         |(long( battery_percent         ) & 0xffffffff);                 
  Sensor.publish(&Sensor_msg);
  Infrared_1_filter = (Infrared_1_filter*3 + Infrared_1*2)/5.0;// 数值滤波

  // Serial2.println(Infrared_1);
   //Serial2.println(Infrared_1_filter);
  
  Sensor_msg_1.data=      (long(Drawstring               ) & 0xffffffff)<<16
                         |(long(Infrared_5               ) & 0xffffffff)<<8 
                         |(long(Infrared_4               ) & 0xffffffff);                
  Sensor_1.publish(&Sensor_msg_1);
          
  }  
void setup()
{
  //pinMode(LED_BUILTIN, OUTPUT);//系统运行指示灯
  Serial3.begin(9600);//串口初始化  
  
  Serial2.begin(9600);//串口初始化 库仑计信号
  // delay(3000);
   
   nh.initNode(); //ROS节点初始化   Serial0  9600     
  // delay(3000);

 // A车初始化================================
   pinMode(DIR_A, OUTPUT);     // 左轮方向口
   digitalWrite(DIR_A, LOW);   // LOW 后退   HIGH向前  
      
   setPwmFrequency2560(PWM_A,1);//46
   pinMode(PWM_A, OUTPUT);     
   analogWrite(PWM_A,0);   
   /*
   setPwmFrequency2560(PWM_A_1,1);//45
   pinMode(PWM_A_1, OUTPUT);     
   analogWrite(PWM_A_1,0);                  
   
   setPwmFrequency2560(PWM_A_2,1);//46
   pinMode(PWM_A_2, OUTPUT);     
   analogWrite(PWM_A_2,0);  

              */

   // 普通电机口初始化================================
   pinMode(motor_1, OUTPUT);
   digitalWrite(motor_1, LOW);

   pinMode(motor_2, OUTPUT);
   digitalWrite(motor_2, LOW);

   pinMode(motor_3, OUTPUT);
   digitalWrite(motor_3, LOW);

   pinMode(motor_4, OUTPUT);
   digitalWrite(motor_4, LOW);
   
    
  
 // B车初始化================================
   pinMode(DIR_left_front, OUTPUT);     // 左轮方向口
   digitalWrite(DIR_left_front, LOW);   // LOW 后退   HIGH向前  
   pinMode(DIR_right_front, OUTPUT);    // 右轮方向口
   digitalWrite(DIR_right_front, HIGH); // LOW 后退   HIGH向前  

   pinMode(DIR_left_behind, OUTPUT);     // 左轮方向口
   digitalWrite(DIR_left_behind, LOW);   // LOW 后退   HIGH向前  
   pinMode(DIR_right_behind, OUTPUT);    // 右轮方向口
   digitalWrite(DIR_right_behind, HIGH); // LOW 后退   HIGH向前 
/*
   setPwmFrequency2560(PWM_left_front,1);
    pinMode(PWM_left_front, OUTPUT);     // 左轮方向口——6
   analogWrite(PWM_left_front,0);                  //左电机控制占孔比初始化 
   
   setPwmFrequency2560(PWM_right_front,1);
   pinMode(PWM_right_front, OUTPUT);     // 左轮方向口——7
   analogWrite(PWM_right_front,0);                 //左电机控制占孔比初始化

   
   setPwmFrequency2560(PWM_right_front,1);//  8----7
   pinMode(PWM_left_behind, OUTPUT);     // 左轮方向口  
   analogWrite(PWM_left_behind,0);                  //左电机控制占孔比初始化
   
   setPwmFrequency2560(PWM_right_behind,1);
   pinMode(PWM_right_behind, OUTPUT);     // 左轮方向口——44
   analogWrite(PWM_right_behind,0);                 //左电机控制占孔比初始化
    */
    setPwmFrequency2560(PWM_left,1);
    pinMode(PWM_left, OUTPUT);     // 左轮方向口——6
   analogWrite(PWM_left,0);                  //左电机控制占孔比初始化 
   
   setPwmFrequency2560(PWM_right,1);
   pinMode(PWM_right, OUTPUT);     // 左轮方向口——7
   analogWrite(PWM_right,0);                 //左电机控制占孔比初始化

    
   /*
   SetPinFrequency(PWM_left_front, Frequence);    //左电机控制频率 初始化
   pwmWriteHR(PWM_left_front,0);                  //左电机控制占孔比初始化
   SetPinFrequency(PWM_right_front, Frequence);   //右电机控制频率 初始化
   pwmWriteHR(PWM_right_front,0);                 //左电机控制占孔比初始化

      SetPinFrequency(PWM_left_behind, Frequence);    //左电机控制频率 初始化
   pwmWriteHR(PWM_left_behind,0);                  //左电机控制占孔比初始化
   SetPinFrequency(PWM_right_behind, Frequence);   //右电机控制频率 初始化
   pwmWriteHR(PWM_right_behind,0);                 //左电机控制占孔比初始化
*/
   //消息初始化===============================
   nh.advertise(Sensor);    //消息发布初始化
   nh.advertise(Sensor_1);    //消息发布初始化
   nh.subscribe(sub);        //消息订阅初始化,会影响串口
   nh.subscribe(sub1);        //消息订阅初始化,会影响串口

// 舵机初始化=====================================
   myservo.attach(pin);  // 将控制面6舵机的输入连接至11脚
   myservo2.attach(pin2);  // 将控制面5/7舵机的输入连接至12脚

   
   delay(200);
   pinMode(ledPin, OUTPUT);
   digitalWrite(ledPin, HIGH);

   pinMode( trigger_io, OUTPUT);
   digitalWrite(trigger_io, LOW);
    
   FlexiTimer2::set(2,1.0/1000,task );//2ms执行一次定时器中断服务程序
   FlexiTimer2::start();

   pinMode(pwm_io_1, INPUT);          // 中断输入口
   attachInterrupt(0, DYP_plus_measure_1, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
   pinMode(pwm_io_2, INPUT);          // 中断输入口
   attachInterrupt(1, DYP_plus_measure_2, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
   pinMode(pwm_io_3, INPUT);          // 中断输入口
   attachInterrupt(2, DYP_plus_measure_3, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
   pinMode(pwm_io_4, INPUT);          // 中断输入口
   attachInterrupt(3, DYP_plus_measure_4, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
   pinMode(pwm_io_5, INPUT);          // 中断输入口
   attachInterrupt(5, DYP_plus_measure_5, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数 
}

//// 主循环 //////
void loop()
{
#ifndef test_motor  
  if(!master_firefly_online){// 主机离线 关机所有电机==============
    // 可能还需要一个标志，确保master_firefly_online在变换时，只执行一次
    /*
    analogWrite(PWM_left_front,0); // 关闭pwm电机
    analogWrite(PWM_right_front,0);
    analogWrite(PWM_left_behind,0);
    analogWrite(PWM_right_behind,0);
    */
     analogWrite(PWM_left,0);
    analogWrite(PWM_right,0);
    analogWrite(PWM_A,0);
    
    // 关闭 继电器电机=========
   digitalWrite(motor_1, LOW);
   digitalWrite(motor_2, LOW);
   digitalWrite(motor_3, LOW);
   digitalWrite(motor_4, LOW);
   
    // 关闭舵机 ===========
    // servo_run_flag = 0;
      servo_1 = 0;
      servo_2 = 0;
      
    }
#endif

// 舵机切换方向============================ servo1
  // 可能还需要一个标志，确保 direction_change_flag 在变换时，只执行一次
if(direction_change_flag) {zf_flag = !zf_flag;direction_change_flag=0;}

// 舵机切换方向============================ servo2

  if(sensor_sample_fla){
    sensor_sample_fla = 0;

//拉绳传感器数据处理
  sensorValue = analogRead(analogInPin);
  Drawstring = map(sensorValue, 0, 1011, 0, 120);//电压到距离的线性映射
 // Drawstring = distance;
// Serial2.println(Drawstring);
//  Serial2.println(sensorValue);
//Serial2.println(Infrared_5);
}

 while (Serial2.available()) {
    data[count] = (unsigned char)Serial2.read();//读取一个新字节
    if(count==0 && data[0]!=0x5A) return;//如果读取的第一个字节不是0x5A，跳出去
    count++;
    if(count == 14){//当读满13个字节
      count = 0;
      if(data[2] == 0x10)//当第三个字节为0x10，读取接收到的消息
      {
      eletric_percent = data[3];
      battery_percent = eletric_percent;
      battery_state = data[12];
      }
    }
  }
}

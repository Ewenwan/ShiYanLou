/* **************************************
发现一个问题：就是analogRead需要花时间，读角度在20ms中断中，读电流在Loop中，会产生冲突。
修改方式：模拟量统一放在Task中每100ms读取一次。
****************************************/

//#define test_motor // 测试电机，去除的话，电机未收到消息不会转动

//#define test_seri_0 // 测试串口 0 打印 传感器消息 ros不能用 
// mega 2560  

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

//#define USE_USBCON
#include <ros.h>
#include <std_msgs/UInt32.h>
//#include <FlexiTimer2.h>// 
#include <TimerThree.h>  // 系统任务定时器 
//#include <TimerOne.h>		// http://www.arduino.cc/playground/Code/Timer1
#include "pid.h"
#include<avr/wdt.h>  //watchdog

#define SERIAL_BUFFER_SIZE 128

//#include <PWM.h> // 有一个单独的函数可以配置pwm波频率，再使用 analogwrite() 设置pwm
#include <Servo.h>  //舵机

//陀螺仪
#include <Wire.h>
#include <JY901.h>

#define SERIAL_RX_BUFFER_SIZE 512
#define SERIAL_TX_BUFFER_SIZE 512

#define EP_WRITE_BLOCK(address, block) eeprom_write_block((const void*)&block, (void*)address, sizeof(block))
#define EP_READ_BLOCK(address, block) eeprom_read_block((void*)&(block), (void*)address, sizeof(block))

/* B车前刷摆动方向的定义 */
#define DIR_BF_UP_2_DOWN   LOW
#define DIR_BF_DOWN_2_UP   HIGH

/* B车前摆速度的定义 */
#define SPEED_VALUE_GOOD 10 // 10 //  模拟量 反馈差值  25

/*B车前刷PWM的最大最小值定义 */
int LACK_BF = 100;//95   //这个需要现场进行调试
int EXCEED_BF = 115; //105   //这个需要现场进行调试

//2018-12-08
/* B车后摆转动速度定义 */
#define SPEED_VALUE_GOOD_BACK 10 //  模拟量 反馈差值  25

#define MAX_BACK_BRUSH_PWM   40//60  //后摆堵转的PWM

/* B车后刷摆动角度和速度的定义 */  
/* //2018-12-08
int r_zl = 100;    //每次增量
int min_r = 1150; //1550; //最小位置（新舵机1550）（1150旧舵机 -- 最右侧）（1150-1500）
int max_r = 1850; //1950;  //最大角度位置（新舵机1950） （1850旧舵机 -- 最左侧）（1450-1850）

const int MIN_R = 1200; //1550; //最小位置（新舵机1550）（1150旧舵机 -- 最右侧）（1150-1500）
const int MAX_R = 1850; //1950;  //最大角度位置（新舵机1950） （1850旧舵机 -- 最左侧）（1450-1850）
*/
//2018-12-08
int angel_left_back =  600;//700;//420;//900;//450;// 车后摆转动范围====
int angel_right_back = 250;//400;//150;//650;// 700;


/* B车前刷摆动时角度 --- 实际需要进行调整 */
// 0~360 映射到 0~1023  这里是 电位器反馈值====
// 0~90  
int angel_down = 460;//600;//900;//450;// 转动范围====
int angel_up = 300;//200;//300;//650;// 700;

int angle_z = 0;

/* 舵机角度和设置值的换算 */
/* //2018-12-08
float k =  -1.11508;  
float b = 2103;
*/

/* 堵转检测最大电流定义 */
#define MAX_CIRCUIT_B_LF   580 //540//580   //实测B车行走电机的电流在500-550之间波动
#define MAX_CIRCUIT_B_RF   580   //实测B车行走电机的电流在500-550之间波动
#define MAX_CIRCUIT_B_LB   580 //实测B车行走电机的电流在500-550之间波动
#define MAX_CIRCUIT_B_RB   580   //实测B车行走电机的电流在500-550之间波动
#define MAX_CIRCUIT_B_B_BRUSH   580   //实测后刷的电流在500-550之间波动，舵机堵转电流变化不大，电机堵转电流超过580

#define MAX_BF_PWM   200

//--------------------------------------- 硬件引脚定义----------------------------------------------------------
// ---- digital table -----
// 	0: 串口0	 1:串口0		2: 顶部超声				3: 左前超声						4:超声触发
// 	5: B车左轮方向 6: 后摆PWM		7: B车左轮PWM	  8: 	A车PWM			9:前摆刷PWM
// 	10:前摆方向	 11: B车右轮PWM		12:	后摆方向		13:	LED				14:串口3
//	15:串口3	 16:串口2		17:串口2				18:串口1						19:串口1
//	20:			 21:左后超声	22:B车右轮方向
//	25:
//	30:			 31:			32:	保留				33:						34:		
//	35:	前刷电机		 36:	37:	后刷电机					38:				39:A车刷子电机  ----有改动----
//	40:			  41:			42:	A车驱动使能			43:						44:A车方向
//	45:			 46:前后摆使能	47:						48:B车左轮使能
//	50:B车右轮使能 51:			52:						53:	告警器					
//
// ---- Analog table -----
//  A0:	拉绳		A1:	B右后轮			A2:B右前轮堵转		A3:B车左后轮堵转		A4:B左前轮堵转
//	A5:	B前刷		A6: B后摆堵转		A7:A驱动			A8:	水位探针2			A9:水位探针
//	A10:前摆角度    A11:后摆角度		
//  
//----  其他 -----
// 串口3：遥控器    串口2: 电流计   		串口1: 陀螺仪


/* B车前进电机引脚定义 */
#define PWM_left        12//7  // B-L
#define PWM_right       11//  8  // B-R
#define DIR_left_front         5 // 左轮方向 IO口  //old 10
#define DIR_left_behind         5 // 左轮方向 IO口
#define DIR_right_front        22//13 // 右轮方向 IO口
#define DIR_right_behind        22//13 // 右轮方向 IO口   //old 5

/* A车前进电机引脚定义 */
#define PWM_A       8// 6  // A
#define DIR_A        44//    （还有一个42是使能）

/* B车前刷(B car Front Swing) 摆动电机引脚定义 */ //BJF新增
#define PWM_BF     9   //BJF 
#define DIR_BF    10 	//BJF  -- 将原来的舵机的PWM引脚拿过来用

/* B车前刷(B car Front Swing) 摆动电机角度传感器 */
#define ANGEL_FB A10 // B车前摆舵机的角度模拟量反馈 管脚


/* B车后刷舵机的管脚 */
// 2018-12-08
/*
const int pin = 11;//面6舵机的控制信号引脚
*/
const int ang_pin = A11;//将舵机的角度输出信号连接至A1（调整面6角度的舵机）

//2018-12-08
#define PWM_B_BACK     6 //42 //后摆PWM
#define DIR_B_BACK     7 //12 //44	//后摆方向


/* 超声触发和输入管脚定义 */
const int  trigger_io = 4;  // 触发口
const int  pwm_io_1 = 2;  //顶部超声
const int  pwm_io_2 = 3;  //左前超声
const int  pwm_io_3 = 21;// 左后超声 I2C口占用

/* 水位探针引脚 */
#define  WATER_PIN  A9   
#define  WATER_PIN2  A8   

#define ALARM	53  //告警

//普通电机口=====================================
#define motor_3    40    //保留
#define motor_2    36    //后刷电机
#define motor_1    38 //    //前刷电机  
#define motor_4    34//38    //面1/2/3/4电机

/* LED管脚 */
const int ledPin =  LED_BUILTIN;//LED管脚

/* 拉绳管脚 */
const int analogInPin = A0;  // 线位移传感器模拟量输出口

/* EEPROM 地址偏移量定义 （用于保存后刷舵机初始位置） */
int addr = 0;
//int addr2 = 16;

/* 驱动板使能PIN */
#define EN_B_LEFT_PWM  		48   //左轮
#define EN_B_RIGHT_PWM  	50   //右轮
#define EN_B_FRONT_PWM  46
#define EN_B_BACK_PWM  46  //后摆使能  2018-12-08
#define EN_A_PWM		42  //A车驱动使能

/* 堵转电路ADC管脚定义 */
#define CIRCUIT_B_LF  		A4  //B车左前轮堵转电流
#define CIRCUIT_B_RF  		A2  //B车右前轮堵转电流
#define CIRCUIT_B_LB  		A3  //B车左后轮堵转电流
#define CIRCUIT_B_RB  		A1  //B车右后轮堵转电流
#define CIRCUIT_A 	 		A7  //A车驱动轮堵转电流
#define CIRCUIT_B_F_BRUSH	A5  //B车前刷电机堵转电流
#define CIRCUIT_B_B_BRUSH	A6  //B车后刷摆动电机堵转电流

//---------------------------- 全局变量定义-----------------------------

//2018-12-08
/*
///////////  控制B车后刷舵机转动方向使用的全局变量 
Servo myservo;  // 创建 servo 对象来控制一个 servo （面6舵机）
bool zf_flag = 0;//符号标志选择正反转 0 正 1500 ->1900  1 反
float angle;      // 舵机反馈的角度值 对应角度值
int   angle_int;
int   angle_int_100ms;//100ms时对应的角度
bool direction_change_flag = 0; // 舵机转向变化flag 向任意一个方向转动到转不动了，就需要改变方向。
int r_current = min_r;//当前位置
int r_control = min_r;  //控制位置
//防堵转的变量
int servo_change_dir_dect_time_count = 0;  //用于来计算是否堵转的计时器
//bool servo_run_flag = 0;// 舵机运动的标志，来自 上位机给的指令
int servo_angle_diff_filter=0;  //用于判断角度是否发生变化的计数
int servo_angle_diff_filter_thersh = 5;//用于判断角度是否发生变化的计数阈值
int change_flag_last=0;
int prev_zf_flag = 0;  //上一次的转动方向
int prev_zf_value;   //上一次的角度值
int cur_zf_value;       //当前的角度值
int a0;  //读到的角度值
*/
int angel_read_back = 0;  //后摆角度的读取值
int angel_last_back=0;// 上次后摆角度的读取值
int angel_diff_feedback_back=0;// 两次反馈差值
pidControl pidCon_back; // pid 控制参数结构体
PpidControl PpidCon_back = NULL;// 指针
int max_pwm_back_counter = 0;


//控制前摆的参数
int angel_read = 0;
int angel_last=0;// 上次 反馈值
int angel_diff_feedback=0;// 两次反馈差值
pidControl pidCon; // pid 控制参数结构体
PpidControl PpidCon = NULL;// 指针
int average_pwm_bf = 0;   //当前500ms内的前刷摆动电机的平均设定PWM值 
int sum_pwm_bf = 0;
int pwm_bf_count = 0;
bool stop_and_brush_flag = false;  //是否采用停-刷模式
int brush_gap_time = 30;   //两次刷之间的停时间<车这个时候只走>(100ms - 因为fb_control 100ms控制一次)
int brush_status = 0; // 0 - 又走又刷 1 - 只刷 2 - 只走
int brush_gap_timer = 0;
float sum_liner_speed_B = 0.0;

///////   超声全局变量定义
unsigned long DYP_PulseStartTicks_1,DYP_PulseStartTicks_2,DYP_PulseStartTicks_3,DYP_PulseStartTicks_4,DYP_PulseStartTicks_5;     
volatile int DYP_val = 0 ;
volatile float DYP_distalce = 0.0;
int 	dyp1_failure_counter = 0;
int 	dyp2_failure_counter = 0;
int 	dyp3_failure_counter = 0;

// 超声波 触发信号=======
int count_2ms = 0;
const int trigger_time_zones = 25;//50;// 50*2ms =100ms 触发周期
const int trigger_time_duration = 1;// 持续时间

/////////  ROS和上位机通信相关控制全局变量
int Motor_contrl_msg_count = 0;// ros话题接收消息次数 计数
int Motor_contrl_msg_count_100ms = 0;// 100ms时的计数
bool master_firefly_online = 0;// 主机在线标志，在发送消息

/////// B车速度和方向控制的全局变量
float vel_l = 0;//左轮转速
float vel_r = 0;//右轮转速
int PWM_left_value = 0;
int PWM_right_value = 0;
float car_length = 0.25;  //B车车长
float car_width = 0.25;   //B车车宽

///////  A车速度和方向控制的全局变量
float vel_A = 0;//转速
int PWM_A_value = 0;

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

//LED参数
long int led_flag=0;//当前LED状态
int led_flag_last=0;//上一LED状态
bool led_on_off_flag=0;
//int led_count=0;
//bool led__ = false;

//拉绳参数定义
volatile int  sensorValue = 0;        //模拟量输出口的电压值

//水位探针检测定时
int water_read_count = 0;
int alarm_water_counter = 0;    //有时候电压波动会导致误检测

// 串口2接收============== 
//电源模块,充电时，电源模块红线接充电器的负，黑线接电池的负
unsigned char data[15];//保存传输过来的15个字节
unsigned char count=0;//计数
int eletric_percent;//

// 串口1接收 ===================
unsigned char data_ps[8];//保存传输过来的8个字节
unsigned char count_ps=0;//计数

//遥控手柄 ==========================
unsigned char buttons = 99; //遥控手柄按键
unsigned char L1int = 128; 
unsigned char L2int = 128;
unsigned char R1int = 128; 
unsigned char R2int = 128; 
std_msgs::UInt32 localctrl_msg;  //遥控手柄的控制消息
unsigned int format_vel = 0;
float  speed = 0, theta = 0, speed_a = 0;
unsigned int format_w = 0;
unsigned int  localctrl_mode = 0;  // 0 - 正常模式 1 - 本地控制模式 2 - 退出本地控制模式 
unsigned int localctrl_timer = 0;
bool rcv_ps = false;
bool control_ab = true;  //切换AB车控制 true： B车  false: A车

//Task任务控制全局变量==============================
int send_ros_count = 0;  //用于向上位机发送数据的周期控制计数器
int servo_count = 0;  //用于控制后刷舵机转动的周期控制计数器
int motor_count = 0;  //用于控制前刷摆动电机转动的周期控制计数器
int Motor_contrl_msg_time_count=0;  //用于判断上位机是否离线
bool sensor_sample_fla=0;  //用于拉绳传感器模拟量数据采集
bool gyro_sample_fla = 0; //陀螺仪串口采样

//  ROS参数定义=======================
ros::NodeHandle   nh;                               //创建ROS句柄
void Motor_contrl( const std_msgs::UInt32& cmd_msg);//函数声明
//void Motor_1_contrl( const std_msgs::UInt32& cmd_msg);//函数声明  //2018-12-08
void Setup_contrl( const std_msgs::UInt32& cmd_msg);//函数声明
         
//定义编码器脉冲广播消息格式
std_msgs::UInt32  Sensor_msg; 
// 定义向上位机发送指令，Publisher-发送，Sensor-，"Sensor"-消息头，Sensor_msg-待发送的消息结构
ros::Publisher Sensor("Sensor", &Sensor_msg); 

std_msgs::UInt32  Sensor_msg_1;              
ros::Publisher Sensor_1("Sensor_1", &Sensor_msg_1);  

std_msgs::UInt32  to_upper_ctrl_msg;              
ros::Publisher Localctrl("Localctrl", &to_upper_ctrl_msg);  
               
// 定义从上位机接收话题，Subscriber-接收，std_msgs-，"motor"-消息头，Motor_contrl-得到上位机指令后调用的函数
ros::Subscriber<std_msgs::UInt32> sub("motor", Motor_contrl);
ros::Subscriber<std_msgs::UInt32> sub_setup("setup", Setup_contrl);
//ros::Subscriber<std_msgs::UInt32> sub1("motor_1", Motor_1_contrl); //2018-12-08

bool send_ros_flag = false;

//按钮开关防抖动
//unsigned long lastDebounceTime = 0;
//int debounceDelay = 10;  //4ms抖动
//int			   	buttonState = 0;   //0 - OFF状态
//int			    lastButtonState = 0;  //0 -- OFF状态

//堵转电流检测
bool check_motor_circuit_flag = false;  //每100ms检测一次电流
int Circuit_time_count = 0;  
float circuit_b_lf = 0;
float circuit_b_rf = 0;
float circuit_b_lb = 0;
float circuit_b_rb = 0;
int circuit_a = 0;
float circuit_b_f_brush = 0;
float circuit_b_b_brush = 0;
bool  dz_motor_flag = false;  //是否检测到堵转
int	  cur_motor_value_1 = 0;  //记录当前的控制电机的状态，用于电机刚启动时1秒不进行堵转检测
int	  cur_motor_value_2 = 0;  //记录当前的控制电机的状态，用于电机刚启动时1秒不进行堵转检测
int	  cur_motor_value_3 = 0;  //记录当前的控制电机的状态，用于电机刚启动时1秒不进行堵转检测
int	  cur_motor_value_4 = 0;  //记录当前的控制电机的状态，用于电机刚启动时1秒不进行堵转检测
int  delay_check_du_zhuan = 0;  //电机刚启动时，1秒内不进行堵转检测，这个变量是倒计时

/* 模拟量读取计数定时 */
int analog_read_count = 0;
bool read_analog_fla = false; 

//用于测试自动靠左和自动靠右的代码
bool auto_left = false;
bool auto_right = false;
bool ab_forward = false;
bool send_upper_ctrl = false;

//陀螺仪
int gyro_failure_counter = 0;

//---------------------------- 函数声明和定义 --------------------------------------------------------------
void send_ros();
void setPwmFrequency2560(int pin, int divisor);
//void readButton();
void check_motor_circuit(); //检测堵转电流

//读取并处理堵转电流
//如果发现堵转，立刻停止对应的电机，并设置告警标志
void read_analog_value()
{
	//读取舵机角度值
	//a0 = analogRead(ang_pin);

	
	//B车行走电机实测的结果是：在500-550之间
	circuit_b_lf = (circuit_b_lf *0.95 + 0.05 * analogRead(CIRCUIT_B_LF));  
	circuit_b_rf = circuit_b_rf *0.95 + 0.05 * analogRead(CIRCUIT_B_RF);    
	circuit_b_lb = (circuit_b_lb *0.95 + 0.05 * analogRead(CIRCUIT_B_LB));   //实测好像是右前轮 >530肯定堵转了
	circuit_b_rb = (circuit_b_rb *0.95 + 0.05 * analogRead(CIRCUIT_B_RB));  
	
/*
	Serial.print(analogRead(CIRCUIT_B_LF));
	Serial.print("\t");
	Serial.print(analogRead(CIRCUIT_B_RF));
	Serial.print("\t");
	Serial.print(analogRead(CIRCUIT_B_LB));  
	Serial.print("\t");
	Serial.print(analogRead(CIRCUIT_B_RB));
	Serial.print("\t");
	Serial.println(analogRead(CIRCUIT_B_B_BRUSH));
//*/	

	//A车很难堵转，对A车电流也没有必要进行检测
	//circuit_a = analogRead(CIRCUIT_A);
	
	//如果继电器没有打开（前刷），此时不进行电流读取
	//前刷的电流会收到其他电机的影响，且堵转对电流的影响很小，不尝试前刷堵转检测
/*	if (motor_1_value==0)
		circuit_b_f_brush = 0;
	else
		circuit_b_f_brush = circuit_b_f_brush *0.1 + 0.9 * analogRead(CIRCUIT_B_F_BRUSH);
*/
	
	//后刷实验室测试的结果是：在500~550之间，舵机堵转没有明显变化。可以暂设置堵转电流大小为590.
	//circuit_b_b_brush = (circuit_b_b_brush *0.95 + 0.05 * analogRead(CIRCUIT_B_B_BRUSH));  
	

}

void check_motor_circuit()
{
/*	
	Serial.print(circuit_b_lf);
	Serial.print("\t");
	Serial.print(circuit_b_rf);
	Serial.print("\t");
	Serial.print(circuit_b_lb);
	Serial.print("\t");
	Serial.print(circuit_b_rb);
	Serial.println("\t");
*/
	
	//如果发现任何堵转，都需要立刻停下来，并且进行上报
	if (circuit_b_lf>MAX_CIRCUIT_B_LF || circuit_b_rf>MAX_CIRCUIT_B_RF || circuit_b_lb>MAX_CIRCUIT_B_LB || circuit_b_rb > MAX_CIRCUIT_B_RB
		|| circuit_b_b_brush>MAX_CIRCUIT_B_B_BRUSH )
	{
		//Stop ALL MOTORS
		dz_motor_flag = true;
		//Serial.println("du zhuang occoured");
	
		digitalWrite(ALARM, HIGH);//输出LOW电平,停止发声
		
	}
	else 
	{
		//dz_motor_flag = false;  //一旦发现堵转，除非关电重启，否则不允许再让电机运行
	}
	
}


//2018-12-08
//----控制后摆摆动的电机代码---
void servo_control()
{
	//读取当前的角度值（*放在TASK这里读会不会影响运行？）
	angel_read_back = analogRead(ang_pin);

	//Serial.print(angel_read_back);
	//Serial.println('\t'); //  
	
	//防止0点穿越
	if (abs(angel_read_back - angel_last_back) > 900 )
	{
		angel_read_back = angel_read_back + 1024;
	}
	
	//angel_diff_feedback_back = abs( angel_read_back - angel_last_back);

	if (PpidCon_back->bDrection==false) //由左往右转时角度应逐步变小
	{
		angel_diff_feedback_back =  angel_last_back - angel_read_back;
	}
	else  //由右往左转时角度应逐步变大
		angel_diff_feedback_back =  angel_read_back - angel_last_back;

	angel_last_back = angel_read_back;
	
	
	//for test
	//Serial.print(angel_diff_feedback_back); // 差值
	//Serial.print('\t'); //  
	//Serial.print(PpidCon_back->bDrection);       
	//Serial.print('\t'); //  
	
	//根据当前所处的不同位置，设置不同的PID控制目标值
	if( PpidCon_back->bDrection==true && angel_read_back < (angel_left_back))// 由右往左转，且角度小于最左面的角度，继续转
	{
		PpidCon_back->s16speedwant  = SPEED_VALUE_GOOD_BACK;
	}
	else if ( PpidCon_back->bDrection==false && angel_read_back > (angel_right_back))// 由左往右转，且角度大于最右面的角度，继续转
	{
		PpidCon_back->s16speedwant  = SPEED_VALUE_GOOD_BACK;
	}
	else if ( PpidCon_back->bDrection==true && angel_read_back >= (angel_left_back))// 由右往左转，且角度大于最左面的角度，停下来
	{
		PpidCon_back->s16speedwant  = 0;
	}
	else if ( PpidCon_back->bDrection==false && angel_read_back <= (angel_right_back))// 由左往右转，且角度小于最右面的角度，停下来
	{
		PpidCon_back->s16speedwant  = 0;
	}
	
	//  20 到 0 到 1023 到 1000 区间 ， 差值不稳，剔除==== ？？？
	
	// 开启pid控制,并且防止速度的突然变化
	int cur_PWM = PpidCon_back->u16PWM;
       POSITION_PID( PpidCon_back,angel_diff_feedback_back);

	//如果Arduino先上电然后电机上电，PWM已经控制很高了，此时需要进行防范
	//防范的方法是：重新设置当前的PWM=0
	if (angel_diff_feedback_back > 2 * SPEED_VALUE_GOOD_BACK) {  //???? 好像不对???
		cur_PWM = 0;
		POSITION_CLEAR(PpidCon_back);
	}			
	
	
	//only for test
	int pwm_want = 0;
	if (PpidCon_back->s16speedwant!=0)
		pwm_want = PpidCon_back->u16PWM; 
	else {
		pwm_want = 0; 
		POSITION_CLEAR(PpidCon_back);
	}
	
	//Serial.print(pwm_want);          // 输出值 
	//Serial.print('\t'); //  
	
	if (pwm_want - cur_PWM > 40 )  //缓慢加速过程 20 - 根据情况调整
		PpidCon_back->u16PWM = cur_PWM + 40;
	else if (pwm_want - cur_PWM < -40 )  //缓慢减速过程 20 - 根据情况调整
		PpidCon_back->u16PWM = (cur_PWM>40)?(cur_PWM - 40):0;
	else
		PpidCon_back->u16PWM =  pwm_want;
	 
       SetPwm(PpidCon_back);

	//Serial.println(PpidCon_back->u16PWM);          // 输出值 
	//Serial.print('\t'); //  
  
	// 换向检测:如果角度已经超过阈值，或者减速到0了 ===========================================
	if( (angel_read_back < angel_right_back - 50 ) ||
	(PpidCon_back->u16PWM==0 && angel_read_back < angel_right_back) )
	{  
		PpidCon_back->bDrection = true;// 换向，变成由右往左
		POSITION_CLEAR(PpidCon_back);  //换向之前，先清除掉原来的PID累计误差 （因为有时候发现会突然甩过头）
		SetDir(PpidCon_back);
	}
	else if ( (angel_read_back > angel_left_back + 50) ||
	(PpidCon_back->u16PWM==0 && angel_read_back > angel_left_back) )
	{  
		PpidCon_back->bDrection = false;// 换向，变成由左往右
		POSITION_CLEAR(PpidCon_back);  //换向之前，先清除掉原来的PID累计误差 （因为有时候发现会突然甩过头）
		SetDir(PpidCon_back);
	}

	//如果转不动了，也需要进行换向
	if (PpidCon_back->u16PWM > MAX_BACK_BRUSH_PWM)
	{
		PpidCon_back->u16PWM = MAX_BACK_BRUSH_PWM;
		//计数器++
		max_pwm_back_counter ++;
		
		if (max_pwm_back_counter>=4) {
			PpidCon_back->bDrection = !PpidCon_back->bDrection;
			POSITION_CLEAR(PpidCon_back);  //换向之前，先清除掉原来的PID累计误差 （因为有时候发现会突然甩过头）
			SetDir(PpidCon_back);
			max_pwm_back_counter = 0;
		}
	}
	else 
		max_pwm_back_counter = 0;
	
	//test
	//Serial.println(PpidCon_back->u16PWM);          // 输出值 
	//Serial.print('\t'); //  
	//Serial.println(PpidCon_back->bDrection);          // 输出值 
	//Serial.println('\t'); //  
	//Serial.println(PpidCon_back->s16speedwant);  // 期望值  
	

}


//----控制前刷摆动的电机代码---
void fb_control()
{
	//如果处于刷-走-刷模式
	if (stop_and_brush_flag==true)
	{
		//开始计时
		if (brush_status==2) {
			brush_gap_timer++;
			sum_liner_speed_B += liner_speed_B;
		}
			
		//如果倒计时完成
		if (brush_gap_timer >= brush_gap_time
			&& sum_liner_speed_B>1.5 )  { //100ms fb_control执行1次 0.05的速度*30 = 1.5
			//开始刷，停止走
			brush_status = 1; 
			brush_gap_timer = 0;
			sum_liner_speed_B = 0;
		}
		
	}
	else
	{
		brush_status = 0;
		sum_liner_speed_B = 0;
	}

	//如果当前处于停止刷的状态，则直接退出
	if (brush_status==2)
		return;
	
	//读取当前的角度值（*放在TASK这里读会不会影响运行？）
	angel_read = analogRead(ANGEL_FB);

	//Serial.print(angel_read);       // 反馈值
	//Serial.print('\t'); //  

	//Serial.print(angel_last);       // 上一次的值
	//Serial.print('\t'); //  


	//防止0点穿越
	if (abs(angel_read - angel_last) > 900 )
	{
		angel_read = angel_read + 1024;
	}

	//angel_diff_feedback = abs( angel_read - angel_last);
	if (PpidCon->bDrection==false) //由下往上转时角度应逐步变小
	{
		angel_diff_feedback =  angel_last - angel_read;
		
		//如果角度反而变大，作为测量误差进行处理
		//作为角度和上次没有变化进行处理
		if (angel_diff_feedback<0)
			angel_diff_feedback = 0;
		//否则正常处理，并更新历史角度值
		else 
			angel_last = angel_read;
			
	}
	else  {//由上往下转时角度应逐步变大
		angel_diff_feedback =  angel_read - angel_last;

		//如果角度反而变小，作为测量误差进行处理
		//作为角度和上次没有变化进行处理
		if (angel_diff_feedback<0)
			angel_diff_feedback = 0;
		//否则正常处理，并更新历史角度值
		else 
			angel_last = angel_read;
	}
	
	//for test
	//Serial.print(angel_diff_feedback); // 差值
	//Serial.print('\t'); //  
	//Serial.print(angel_read);       // 反馈值
	//Serial.print('\t'); //  
	//Serial.print(PpidCon->bDrection);       
	//Serial.print('\t'); //  
	
	//根据当前所处的不同位置，设置不同的PID控制目标值
	if( PpidCon->bDrection==false && angel_read > (angel_up))// 由下往上转，且角度大于最上面的角度，继续转
	{
		PpidCon->s16speedwant  = SPEED_VALUE_GOOD;
	}
	else if ( PpidCon->bDrection==true && angel_read < (angel_down))// 由上往下转，且角度小于最下面的角度，继续转
	{
		PpidCon->s16speedwant  = SPEED_VALUE_GOOD;
	}
	else if ( PpidCon->bDrection==false && angel_read <= (angel_up))// 由下往上转，且角度小于最上面的角度，停下来
	{
		PpidCon->s16speedwant  = 0;
	}
	else if ( PpidCon->bDrection==true && angel_read >= (angel_down))// 由上往下转，且角度大于最下面的角度，停下来
	{
		PpidCon->s16speedwant  = 0;
	}
	
	//  20 到 0 到 1023 到 1000 区间 ， 差值不稳，剔除==== ？？？
	
	// 开启pid控制,并且防止速度的突然变化
	int cur_PWM = PpidCon->u16PWM;
    POSITION_PID( PpidCon,angel_diff_feedback);

	if (PpidCon->u16PWM>MAX_BF_PWM)  //堵转了
		dz_motor_flag = true;
	
	//如果Arduino先上电然后电机上电，PWM已经控制很高了，此时需要进行防范
	//防范的方法是：重新设置当前的PWM=0
/*	if (angel_diff_feedback > 2 * SPEED_VALUE_GOOD) {  //???? 好像不对???
		cur_PWM = 0;
		POSITION_CLEAR(PpidCon);
	}			
*/	
	//Serial.print(PpidCon->s16speedwant);          // 输出值 
	//Serial.print('\t'); //  
	
	//only for test
	int pwm_want = 0;
	if (PpidCon->s16speedwant!=0)
		pwm_want = PpidCon->u16PWM; 
	else {
		pwm_want = 0; 
		POSITION_CLEAR(PpidCon);
	}
	
	//Serial.print(pwm_want);          // 输出值 
	//Serial.print('\t'); //  
	
	if (pwm_want - cur_PWM > 40 )  //缓慢加速过程 20 - 根据情况调整
		PpidCon->u16PWM = cur_PWM + 40;
	else if (pwm_want - cur_PWM < -40 )  //缓慢减速过程 20 - 根据情况调整
		PpidCon->u16PWM = (cur_PWM>40)?(cur_PWM - 40):0;
	else
		PpidCon->u16PWM =  pwm_want;
	 
    SetPwm(PpidCon);  

	//Serial.println(PpidCon->u16PWM);          // 输出值 
	//Serial.print('\t'); //  
  
	// 换向检测:如果角度已经超过阈值，或者减速到0了 ===========================================
//2019-01-04
	if( (angel_read>angel_down + 50 ) ||
	(PpidCon->u16PWM==0 && angel_read>angel_down) )
	{  
		PpidCon->bDrection = false;// 换向，变成由下往上
		POSITION_CLEAR(PpidCon);  //换向之前，先清除掉原来的PID累计误差 （因为有时候发现会突然甩过头）
		SetDir(PpidCon);

		//开始只走不刷
		if (stop_and_brush_flag==true)
			brush_status = 2;
		
		digitalWrite(ALARM, HIGH);//输出LOW电平,停止发声
		delay(10);
		digitalWrite(ALARM, LOW);//输出LOW电平,停止发声
		
	}
	else if ( (angel_read<angel_up - 50) ||
	(PpidCon->u16PWM==0 && angel_read<angel_up) )
	{  
		PpidCon->bDrection = true;// 换向，变成由上往下
		POSITION_CLEAR(PpidCon);  //换向之前，先清除掉原来的PID累计误差 （因为有时候发现会突然甩过头）
		SetDir(PpidCon);
	}
	
	//计算PWM平均值
	int angel_middle = (angel_down+angel_up)/2;
	int angel_zone = (angel_down-angel_up)/2;
	if (PpidCon->bDrection==false && angel_read<(angel_middle+angel_zone/4) && angel_read>(angel_middle-angel_zone/4))
	{
		pwm_bf_count++;
		sum_pwm_bf += PpidCon->u16PWM;
		
		//Serial.print(PpidCon->u16PWM);
		//Serial.print('\t');
		//Serial.println(PpidCon->u16PWM);
		
	}
	else if (pwm_bf_count > 0)
	{
		average_pwm_bf = sum_pwm_bf/pwm_bf_count;
		pwm_bf_count = 0;
		sum_pwm_bf = 0;
		
	}
	
	//test
	//Serial.print(PpidCon->u16PWM);          // 输出值 
	//Serial.print('\t'); //  
	//Serial.println(PpidCon->bDrection);          // 输出值 
	//Serial.print('\t'); //  
	//Serial.println(PpidCon->s16speedwant);  // 期望值  
	

}

//------ 超声模块测量 -----------
void DYP_plus_measure_1(void)
{
	dyp1_failure_counter = 0;
	if (digitalRead( pwm_io_1 ) == HIGH)// 低电平到高电平的 时间点
		DYP_PulseStartTicks_1 = micros();    // 微秒 us  
	else// 高电平变成低电平的 时间点
		// DYP_val = micros() - DYP_PulseStartTicks_1;
		Infrared_1 = (micros() - DYP_PulseStartTicks_1);// / 58.0;
        //Infrared_1 = DYP_val / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
}
void DYP_plus_measure_2(void)
{
	dyp2_failure_counter = 0;
	if (digitalRead( pwm_io_2 ) == HIGH)// 低电平到高电平的 时间点
		DYP_PulseStartTicks_2 = micros();    // 微秒 us  
	else// 高电平变成低电平的 时间点
		//  DYP_val = micros() - DYP_PulseStartTicks_2;
	Infrared_2 = (micros() - DYP_PulseStartTicks_2);// / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
}
void DYP_plus_measure_3(void)
{
	dyp3_failure_counter = 0;
	if (digitalRead( pwm_io_3 ) == HIGH)// 低电平到高电平的 时间点
		DYP_PulseStartTicks_3 = micros();    // 微秒 us  
	else// 高电平变成低电平的 时间点
		// DYP_val = micros() - DYP_PulseStartTicks_3;
		Infrared_3 = (micros() - DYP_PulseStartTicks_3); // / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
}


// --- 串口输出打印，供测试 ---
void send_sensor_seri_0(){
	Serial.print(Infrared_1);//  
	Serial.print("\t");// 
	Serial.print(Infrared_2);//  
	Serial.print("\t");//  
	Serial.print(Infrared_3);//  
	Serial.print("\t");//  
	Serial.print(Infrared_4);//  
	Serial.print("\t");//  
	Serial.println(Infrared_5);//  
}


/////////////10ms /2ms执行一次定时器中断服务程序=======================
void task(){
	wdt_reset();
	

	//将100ms分成4个25ms，分别处理舵机(20-40ms) 和 前进电机(0-20ms)、后摆电机（40-60ms)、超声触发（60-100ms)
	motor_count += 2;
	if (motor_count==10)  //10*2ms
	{
		digitalWrite(LED_BUILTIN,led_on_off_flag);
		led_on_off_flag = !led_on_off_flag;
		
		// 50ms/150ms/..  清洗面5/7的摆动电机控制=========================
		if(servo_2){
			digitalWrite(EN_B_FRONT_PWM, LOW); 
			fb_control();
		}			
		else {
			if (servo_1==0)	
				digitalWrite(EN_B_FRONT_PWM, HIGH); //如果后摆也没有开，关断电机
			analogWrite(PWM_BF,0);  

			
		}
	}		
	else if (motor_count==20)  //20*2ms
	{
		// 33ms/133ms/..  清洗面6的摆动电机控制=========================
		if(servo_1){		
			digitalWrite(EN_B_BACK_PWM, LOW); 
			servo_control();
		}
		else {
			if (servo_2==0) //如果前摆也没有开
				digitalWrite(EN_B_BACK_PWM, HIGH); //关断电机
			analogWrite(PWM_B_BACK,0);  
		}
	}	
	else if (motor_count==30)  //30*2ms
	{
		// 超声波传感器触发信号，测量任务会自动IO口中断执行=========================
		digitalWrite(trigger_io, HIGH);
	}
	else if (motor_count==32) //32*2ms 超声高电平保持4ms
	{
		digitalWrite(trigger_io, LOW);
	}
	else if (motor_count==50)  //50*2ms
	{
		
		motor_count = 0;
		// 100ms/200ms/..  处理遥控消息对电机进行控制=========================
		if (localctrl_mode == 1) {
			Motor_contrl(localctrl_msg);
		}
		else if (localctrl_mode == 2)
		{
			Motor_contrl(localctrl_msg);
			localctrl_mode = 0;
		}
		
		gyro_failure_counter ++; //陀螺仪故障检测计数器
		dyp1_failure_counter ++;
		dyp2_failure_counter ++;
		dyp3_failure_counter ++;
		//Serial.println(dyp1_failure_counter);
		
	}
	

#ifndef test_seri_0	
	// ros消息上传=====================
	send_ros_count += 2;  //+=1
	if(send_ros_count >= 50)    
	{
		sensor_sample_fla = 1;// 模拟量采样任务
		gyro_sample_fla = 1;  //陀螺仪采样任务
		send_ros_flag = true;  
		send_ros_count = 0;
	}
#else
	send_ros_count += 2;  //+=1
	if(send_ros_count>=200){  
		send_sensor_seri_0();
		send_ros_count=0;
	}
#endif



// 主机在线离线判断===============================
// 注：本地遥控模式下，不进行离线判断
	if (localctrl_mode==0)
	{
		Motor_contrl_msg_time_count += 2;  //+=1
		if(Motor_contrl_msg_time_count == 50)     
			Motor_contrl_msg_count_100ms = Motor_contrl_msg_count;// 100ms接收消息次数
		if(Motor_contrl_msg_time_count >= 1000) { // 500ms  -> 1000ms
			if( Motor_contrl_msg_count > Motor_contrl_msg_count_100ms) 
				master_firefly_online = 1;// 主机在线
			else 
				master_firefly_online = 0;// 主机离线
     
			Motor_contrl_msg_time_count=0;// 时间计数清零   500ms周期判断主机是否在线        
			Motor_contrl_msg_count = 0;   // 消息计数清零
		}
	}


	//每100ms读取一次模拟量的值(注：前摆角度也在这里进行读取，因此前摆的PID控制频率不能小于100ms
	//每100ms计算一次当前前摆电机PWM是否达到最大限制
	analog_read_count += 2; //+=1
	if (analog_read_count>=50) { 
		read_analog_fla = true;
		analog_read_count = 0;
	}
	
	//每500ms读取一次水位探针的值
	water_read_count += 2; //+=1
	if (water_read_count>=250) { 
		//Serial.println(analogRead(WATER_PIN));
		if (analogRead(WATER_PIN)<900) { //100K的电阻进行分压测试
				alarm_water_counter ++;
				if (alarm_water_counter>100) {   
					digitalWrite(ALARM,HIGH); 
					alarm_water_counter =0;
				}	
		}
		else {
			alarm_water_counter = 0;
		}
		//Serial.println(analogRead(WATER_PIN2));  //100K的电阻进行分压测试
		water_read_count = 0;
	}
	
	
	//堵转电流检测，设置标志，主要工作在主函数进行(频度必须大于等于模拟量扫描的频度，这里是100ms)
	if (delay_check_du_zhuan>0) {
		delay_check_du_zhuan -= 2;  //-=1
		check_motor_circuit_flag = false;
	} else {
		Circuit_time_count += 2;  //+=1
		if (Circuit_time_count>50) {   
			check_motor_circuit_flag = true;
			Circuit_time_count = 0;
			//Serial.println(circuit_b_b_brush);	
		}	
	}
	
	if (localctrl_mode==1)
		localctrl_timer += 2;  //+=1
	
	
}

/////////////////////////////PWM实现函数/////////////////////
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
			//only for test
			//digitalWrite(LED_BUILTIN,led_on_off_flag);
			//led_on_off_flag = !led_on_off_flag;
/*
	//如果堵转了，不接收上位机的消息，除非重新启动单板  --- 好像有问题哦，应该停下来才对吧
	if (dz_motor_flag==true) {
		//Serial.println("already du zhuang");
		return;
	}
*/

	//如果当前处于刷-走-刷的状态
	if (brush_status==1) //1 表示现在只刷不走
	{
		angle_speed_B = 0;
		liner_speed_B = 0;
		liner_speed_A = 0;
	}
	else 
	{
		angle_speed_B =           (cmd_msg.data & 0x0000007f)*3.14/128;
		liner_speed_B =          ((cmd_msg.data & 0x0000ff00)>>8)*0.2/256;
		liner_speed_A =          ((cmd_msg.data & 0x00ff0000)>>16)*0.2/256;
	}
	
	//angle_speed_B =           (cmd_msg.data & 0x0000007f)*3.14/128;
	direction_angle_speed_B = (cmd_msg.data & 0x00000080)>>7;
	//liner_speed_B =          ((cmd_msg.data & 0x0000ff00)>>8)*0.2/256;
	//liner_speed_A =          ((cmd_msg.data & 0x00ff0000)>>16)*0.2/256;
	servo_1 =                 (cmd_msg.data & 0x01000000)>>24;
	direction_A =             (cmd_msg.data & 0x02000000)>>25;
	direction_B =             (cmd_msg.data & 0x04000000)>>26;
	servo_2 = (cmd_msg.data & 0x08000000)>>27;
	motor_1_value = (cmd_msg.data & 0x10000000)>>28;
	motor_2_value = (cmd_msg.data & 0x20000000)>>29;
	motor_3_value = (cmd_msg.data & 0x40000000)>>30;
	motor_4_value = (cmd_msg.data & 0x80000000)>>31;

	//A车速度解析==========================================  
	PWM_A_value = (int)(liner_speed_A * 255 / 0.2 );
	if (PWM_A_value>253) 
		PWM_A_value = 253;

	float delta_length;
	delta_length = (car_width + car_length*car_length/(4.0*car_width) ) /2.0;

	//B车速度解析==========================================
	if(direction_angle_speed_B)
	{
		vel_l = liner_speed_B - delta_length*angle_speed_B;//左轮转速
		vel_r = liner_speed_B + delta_length*angle_speed_B;//右轮转速
	}
	else
	{
		vel_l = liner_speed_B + delta_length*angle_speed_B;//左轮转速
		vel_r = liner_speed_B - delta_length*angle_speed_B;//右轮转速
	}
	PWM_left_value = abs(vel_l) * 255  / 0.2;
	PWM_right_value = abs(vel_r) * 255  / 0.2;
	if (PWM_left_value>253) 
		PWM_left_value = 253;
	if (PWM_right_value>253) 
		PWM_right_value = 253;
  

  
	//普通电机口===================================== 
	if(motor_1_value) 
		digitalWrite(motor_1, HIGH);
	else  
		digitalWrite(motor_1, LOW);

	if(motor_2_value) 
		digitalWrite(motor_2, HIGH);
	else  
		digitalWrite(motor_2, LOW);

	if(motor_3_value||motor_4_value)  //BJF  待确认
	{
		//digitalWrite(motor_3, HIGH);
		digitalWrite(motor_4, HIGH);
	}
	else  
	{
		//digitalWrite(motor_3, LOW);
		digitalWrite(motor_4, LOW);
	}

	//普通电机口堵转检测限定：如果刚启动电机，不进行堵转检测
	if ((cur_motor_value_1==0 && motor_1_value==1) 
		||(cur_motor_value_2==0 && motor_2_value==1)
		||(cur_motor_value_3==0 && motor_3_value==1)
		||(cur_motor_value_4==0 && motor_4_value==1)
		)
	{
		delay_check_du_zhuan = 500;
		//Serial.println(delay_check_du_zhuan);
	}
	cur_motor_value_1 = motor_1_value;
	cur_motor_value_2 = motor_2_value;
	cur_motor_value_3 = motor_3_value;
	cur_motor_value_4 = motor_4_value;
	
	
	//A车电机方向口======================================
	if(direction_A) 
		digitalWrite(DIR_A, HIGH); // LOW 后退   HIGH向前 
	else  
		digitalWrite(DIR_A, LOW);

	//A车电机PWM口 ===========================
    analogWrite(PWM_A,PWM_A_value); 
	if (PWM_A_value>0)
		digitalWrite(EN_A_PWM,LOW);
	else {
		digitalWrite(EN_A_PWM,HIGH);
	}

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
		if (vel_l >=0) 
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
	digitalWrite(EN_B_LEFT_PWM, LOW);
	digitalWrite(EN_B_RIGHT_PWM, LOW);
	
    analogWrite(PWM_left,PWM_left_value);          
	analogWrite(PWM_right, PWM_right_value);


	Motor_contrl_msg_count++; // 接收上位机消息次数++，用于判断 上位机是否在发送消息=====
}

// 订阅话题的回调函数(得到上位机指令后，调用该函数进行解析,该函数对面6的舵机的摆动角度进行设置) //BJF 待增加
//2018-12-08
/*
void Motor_1_contrl( const std_msgs::UInt32& cmd_msg)
{  
	float min_value;
	float max_value;
	min_value =           (cmd_msg.data & 0x000000ff);    //------需要换算-----------------
	max_value =          ((cmd_msg.data & 0x0000ff00)>>8); 

	min_r =(min_value*(MAX_R - MIN_R))/180 + 1200;
	max_r =(max_value*(MAX_R - MIN_R))/180 + 1200;

	//min_r = 1275;
	//max_r = 1650;
	
}
*/

// 订阅话题的回调函数(得到上位机指令后，调用该函数进行解析)
void Setup_contrl( const std_msgs::UInt32& cmd_msg)
{  
	int cmd = (cmd_msg.data & 0xff000000) >> 24;
	if (cmd==1)
	{
		EXCEED_BF = (cmd_msg.data & 0x000000ff);
	}
	else if (cmd==2)
	{
		LACK_BF = (cmd_msg.data & 0x000000ff);
	}
	
}

int Infrared_1_filter=0;
int Infrared_2_filter=0;
int Infrared_3_filter=0;

//发送消息的函数,
void send_ros(){

	Infrared_1 = Infrared_1/58;
	Infrared_2 = Infrared_2/58;
	Infrared_3 = Infrared_3/58;

	Infrared_1_filter = (Infrared_1_filter*3 + Infrared_1*2)/5.0;// 数值滤波
	Infrared_2_filter = (Infrared_2_filter*3 + Infrared_2*2)/5.0;// 数值滤波
	Infrared_3_filter = (Infrared_3_filter*3 + Infrared_3*2)/5.0;// 数值滤波
	
	Sensor_msg.data= 	  (long(Infrared_3_filter               ) & 0xffffffff)<<24
                         |(long(Infrared_2_filter               ) & 0xffffffff)<<16 
                         |(long(Infrared_1_filter               ) & 0xffffffff)<<8    
                         |(long( battery_state           ) & 0xffffffff)<<7
//                         |(long( battery_percent         ) & 0xffffffff);    
                         |(long( average_pwm_bf         ) & 0xffffffff);    

						 
						 
	Sensor.publish(&Sensor_msg);

	//Serial.print(Infrared_1);
	//Serial.print('\t');
	//Serial.print(Infrared_2);
	//Serial.print('\t');
	//Serial.println(Infrared_3);

	int exceed_bf=0;
	int lack_bf=0;
	if (average_pwm_bf > EXCEED_BF) {
		exceed_bf = 1;
	}
	else if (average_pwm_bf < LACK_BF) {
		lack_bf = 1;
	}
	
	int gyro_status = 0;
	if (gyro_failure_counter>30)
	{
		gyro_status = 1;
	}
	else
	{
		gyro_status = 0;
	}
	
	int dyp1_status = 0;
	if (dyp1_failure_counter>10)
		dyp1_status = 1;
	
	int dyp2_status = 0;
	if (dyp2_failure_counter>10)
		dyp2_status = 1;

	int dyp3_status = 0;
	if (dyp3_failure_counter>10)
		dyp3_status = 1;

	Sensor_msg_1.data=     (long( exceed_bf           ) & 0xffffffff)<<24
							|(long( lack_bf           ) & 0xffffffff)<<25
							|(long( gyro_status           ) & 0xffffffff)<<26
							|(long( dyp1_status           ) & 0xffffffff)<<27
							|(long( dyp2_status           ) & 0xffffffff)<<28
							|(long( dyp3_status           ) & 0xffffffff)<<29
							|(long(Drawstring               ) & 0xffffffff)<<16
							|((unsigned short)(angle_z        ) & 0xffffffff);      //JY901.stcAngle.Angle[2]             
	Sensor_1.publish(&Sensor_msg_1);
	
	//Serial.println(angle_z);
}

 
void localctrl()
{
	//判断当前向上的按键是否按下
	if (buttons==1)
	{
		localctrl_mode = 1;
		buttons = 99;
		if (control_ab==true)
		{
			//车辆前进速度++
			speed += 0.02;
			format_vel = (unsigned int)(fabs(speed)*256/0.2);
			format_vel = (format_vel>=256)?255:format_vel;
			localctrl_msg.data &= 0xFBFF00FF;
			localctrl_msg.data |= format_vel<<8;
			if (speed>=0)
				localctrl_msg.data |= (uint32_t)(1)<<26; //（bit2  B车前进方向   1 向前    0 向后）
			else
				localctrl_msg.data |= (uint32_t)(0)<<26; //（bit2  B车前进方向   1 向前    0 向后）
		}
		else
		{
			speed_a += 0.02;
			format_vel = (unsigned int)(fabs(speed_a)*256/0.2);
			format_vel = (format_vel>=256)?255:format_vel;
			localctrl_msg.data &= 0xFD00FFFF;
			localctrl_msg.data |= (long)format_vel<<16;
			if (speed_a>=0)
				localctrl_msg.data |= (uint32_t)(1)<<25; //（bit1  A车前进方向   1 向前    0 向后）
			else
				localctrl_msg.data |= (uint32_t)(0)<<25; //（bit1  A车前进方向   1 向前    0 向后）
			//Serial.println(localctrl_msg.data);
		}
		//Motor_contrl(localctrl_msg);	
		
	}
	else if (buttons==2)
	{
		localctrl_mode = 1;
		buttons = 99;
		if (control_ab==true)
		{
			//车辆前进速度--
			speed -= 0.02;
			format_vel = (unsigned int)(fabs(speed)*256/0.2);
			format_vel = (format_vel>=256)?255:format_vel;
			localctrl_msg.data &= 0xFBFF00FF;
			localctrl_msg.data |= format_vel<<8;
			if (speed>=0)
				localctrl_msg.data |= (uint32_t)(1)<<26; //（bit2  B车前进方向   1 向前    0 向后）
			else
				localctrl_msg.data |= (uint32_t)(0)<<26; //（bit2  B车前进方向   1 向前    0 向后）
		}
		else
		{
			speed_a -= 0.02;
			format_vel = (unsigned int)(fabs(speed_a)*256/0.2);
			format_vel = (format_vel>=256)?255:format_vel;
			localctrl_msg.data &= 0xFD00FFFF;
			localctrl_msg.data |= (long)format_vel<<16;
			if (speed_a>=0)
				localctrl_msg.data |= (uint32_t)(1)<<25; //（bit1  A车前进方向   1 向前    0 向后）
			else
				localctrl_msg.data |= (uint32_t)(0)<<25; //（bit1  A车前进方向   1 向前    0 向后）
			
		}
		//Motor_contrl(localctrl_msg);	
	}
	else if (buttons==3)
	{
		localctrl_mode = 1;
		buttons = 99;
		//车辆左转速度++
		theta += 0.02;
		format_w = (unsigned int)(fabs(theta)*128/3.14);
		format_w = (format_w>=128)?127:format_w;
		localctrl_msg.data &= 0xFFFFFF00;
		localctrl_msg.data |= format_w;
		if (theta>0)
			localctrl_msg.data |= 0x80;  //设置转向(逆时针）
		else
			localctrl_msg.data |= 0x00;  //设置转向(顺时针）
		//Motor_contrl(localctrl_msg);	
	}
	else if (buttons==4)
	{
		localctrl_mode = 1;
		buttons = 99;
		//车辆左转速度--
		theta -= 0.02;
		format_w = (unsigned int)(fabs(theta)*128/3.14);
		format_w = (format_w>=128)?127:format_w;
		localctrl_msg.data &= 0xFFFFFF00;
		localctrl_msg.data |= format_w;
		if (theta>0)
			localctrl_msg.data |= 0x80;  //设置转向(逆时针）
		else
			localctrl_msg.data |= 0x00;  //设置转向(顺时针）
		//Motor_contrl(localctrl_msg);	
	}
	else if (buttons==9)
	{
		localctrl_mode = 1;
		buttons = 99;
		servo_2 = !servo_2; //切换前刷
		localctrl_msg.data &= 0xE7FFFFFF;
		if (servo_2) {
			localctrl_msg.data |= (uint32_t)(1)<<27; //（bit3  前刷摆动）
			localctrl_msg.data |= (uint32_t)(1)<<28; //（bit4  前刷转动）
		}
		else {
			localctrl_msg.data |= (uint32_t)(0)<<27; //（bit3 前刷不摆动）
			localctrl_msg.data |= (uint32_t)(0)<<28; //（bit4 前刷不转）
		}
	}
	else if (buttons==10)
	{
		localctrl_mode = 1;
		buttons = 99;
		servo_1 = !servo_1; //切换后刷
		localctrl_msg.data &= 0xDEFFFFFF;
		if (servo_1) {
			localctrl_msg.data |= (uint32_t)(1)<<24; //（bit0  后摆摆动）
			localctrl_msg.data |= (uint32_t)(1)<<29; //（bit5  后刷转动）
		}
		else {
			localctrl_msg.data |= (uint32_t)(0)<<24; //（bit0 后摆不摆动）
			localctrl_msg.data |= (uint32_t)(0)<<29; //（bit5 后刷不转）
		}
	}
	else if (buttons==11)  //切换AB车控制
	{
		localctrl_mode = 1;
		buttons = 99;
		control_ab = !control_ab; //切换AB车控制
	}
	//用于测试自动靠左和自动靠右的代码
	else if (buttons==7) //R1 - 自动靠左
	{
		localctrl_mode = 0;
		auto_left = true;
		auto_right = false;
		to_upper_ctrl_msg.data= 0x00000001;
#ifndef test_seri_0	
		send_upper_ctrl = true;
#endif
	}	
	//用于测试自动靠左和自动靠右的代码
	else if (buttons==8)  //R2 - 自动靠右
	{
		localctrl_mode = 0;
		auto_left = false;
		auto_right = true;
		to_upper_ctrl_msg.data= 0x00000002;                   
#ifndef test_seri_0	
		send_upper_ctrl = true;
#endif
	}	
	//用于测试AB车协同的代码
	else if (buttons==5)  //L1 - AB车一起向前
	{
		localctrl_mode = 0;
		ab_forward = true;
		to_upper_ctrl_msg.data=  to_upper_ctrl_msg.data | 0x00000010;                   
#ifndef test_seri_0	
		send_upper_ctrl = true;
#endif
	}	
	//用于测试B车停-刷的代码
	else if (buttons==6)  //L2 - B车停刷模式
	{
		stop_and_brush_flag = !stop_and_brush_flag;
	}	
	else if (buttons==12)
	{
		localctrl_mode = 2;
		buttons = 99;
		theta = 0;
		speed = 0;
		speed_a = 0;
		localctrl_msg.data = 0;
		servo_2 = 0;
		servo_1 = 0;
		//Motor_contrl(localctrl_msg);	
		
		//目前仅用于测试自动靠左和自动靠右
		if (auto_left==true)
		{
			auto_left = false;
			ab_forward = false;
			to_upper_ctrl_msg.data= 0x00000003;                   
#ifndef test_seri_0	
		send_upper_ctrl = true;
#endif

		}
		else if (auto_right==true)
		{
			auto_right = false;
			ab_forward = false;
			to_upper_ctrl_msg.data= 0x00000004;                 
#ifndef test_seri_0	
		send_upper_ctrl = true;
#endif
		}
	}
		
	
}

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);//系统运行指示灯
	Serial3.begin(9600);//串口初始化 陀螺仪 
	Serial2.begin(9600);//串口初始化 库仑计信号
	Serial1.begin(9600);//串口初始化 遥控手柄信号
	// delay(5000);


	// A车初始化================================
	pinMode(DIR_A, OUTPUT);     // 左轮方向口
	digitalWrite(DIR_A, LOW);   // LOW 后退   HIGH向前  
      
	setPwmFrequency2560(PWM_A,1);//46
	pinMode(PWM_A, OUTPUT);     
	analogWrite(PWM_A,0);   

	// 普通电机口初始化================================
	pinMode(motor_1, OUTPUT);
	digitalWrite(motor_1, LOW);

	pinMode(motor_2, OUTPUT);
	digitalWrite(motor_2, LOW);

	pinMode(motor_3, OUTPUT);
	digitalWrite(motor_3, LOW);

	pinMode(motor_4, OUTPUT);
	digitalWrite(motor_4, LOW);
	
	
	//B车的3个电机使能信号初始化（关断）
	pinMode(EN_B_LEFT_PWM, OUTPUT);
	digitalWrite(EN_B_LEFT_PWM, HIGH);
	pinMode(EN_B_RIGHT_PWM, OUTPUT);
	digitalWrite(EN_B_RIGHT_PWM, HIGH);
	pinMode(EN_B_FRONT_PWM, OUTPUT);
	digitalWrite(EN_B_FRONT_PWM, HIGH);
	pinMode(EN_B_BACK_PWM, OUTPUT);  //2018-12-08
	digitalWrite(EN_B_BACK_PWM, HIGH);
	
	//A车驱动的使能信号初始化（关断）
	pinMode(EN_A_PWM, OUTPUT);  //2018-12-08
	digitalWrite(EN_A_PWM, HIGH);
	
	
	// B车初始化================================
	pinMode(DIR_left_front, OUTPUT);     // 左轮方向口   10   (PWM 7)
	digitalWrite(DIR_left_front, LOW);   // LOW 后退   HIGH向前  
	pinMode(DIR_left_behind, OUTPUT);    // 右轮方向口   50
	digitalWrite(DIR_left_behind,LOW ); // LOW 后退   HIGH向前
	pinMode(DIR_right_front, OUTPUT);    // 右轮方向口   13
	digitalWrite(DIR_right_front,LOW ); // LOW 后退   HIGH向前  
	pinMode(DIR_right_behind, OUTPUT);    // 右轮方向口     48
	digitalWrite(DIR_right_behind,LOW ); // LOW 后退   HIGH向前 
   

    setPwmFrequency2560(PWM_left,1);
    pinMode(PWM_left, OUTPUT);     // 左轮方向口——7
    analogWrite(PWM_left,0);                  //左电机控制占孔比初始化 
    setPwmFrequency2560(PWM_right,1);
    pinMode(PWM_right, OUTPUT);     // 左轮方向口——8
    analogWrite(PWM_right,0);                 //左电机控制占孔比初始化
	

	//前摆PID控制初始化
	PpidCon = &pidCon;// 指针
	PpidCon->direction_pin = DIR_BF;// 方向口
	PpidCon->pwm_pin       = PWM_BF;// pwm口
	PpidCon->bDrection     = true;// 正转(由上向下）
	PpidCon->s16speedwant  = SPEED_VALUE_GOOD;//   100ms 期望差值===25===
  
	PpidCon->s16speed_p=2.5; // pwm 目标值 125附近，反馈值 100ms差值 23~27左右 (2.5）
	PpidCon->s16speed_i=0.7; //0.7
	PpidCon->s16speed_d=0;
	
	//前刷摆动电机初始化
    setPwmFrequency2560(PWM_BF,1);
    pinMode(PWM_BF, OUTPUT);    
    analogWrite(PWM_BF,0);    
    pinMode(DIR_BF, OUTPUT);    
	digitalWrite(DIR_BF, LOW);   // LOW 后退   HIGH向前  

	
	//初始化当前的角度
	angel_last=analogRead(ANGEL_FB);

	//2018-12-08
	//后摆PID控制初始化
	PpidCon_back = &pidCon_back;// 指针
	PpidCon_back->direction_pin = DIR_B_BACK;// 方向口
	PpidCon_back->pwm_pin       = PWM_B_BACK;// pwm口
	PpidCon_back->bDrection     = true;// 正转(由左（上）向右
	PpidCon_back->s16speedwant  = SPEED_VALUE_GOOD_BACK;//   100ms 期望差值===25===
  
	PpidCon_back->s16speed_p=2.5; // 
	PpidCon_back->s16speed_i=0.7; //
	PpidCon_back->s16speed_d=0;
	
	//2018-12-08
	//后刷摆动电机初始化
    setPwmFrequency2560(PWM_B_BACK,1);
    pinMode(PWM_B_BACK, OUTPUT);    
    analogWrite(PWM_B_BACK,0);    
    pinMode(DIR_B_BACK, OUTPUT);    
	digitalWrite(DIR_B_BACK, LOW);   // LOW 后退   HIGH向前  
	
	//2018-12-08
	angel_last_back=analogRead(ang_pin);
	
	
 
 #ifndef test_seri_0  
	nh.initNode(); //ROS节点初始化   Serial0  9600     
	// delay(3000); 
	//消息初始化===============================
	nh.advertise(Sensor);    //消息发布初始化
	nh.advertise(Sensor_1);    //消息发布初始化
	nh.advertise(Localctrl);    //消息发布初始化
	nh.subscribe(sub);        //消息订阅初始化,会影响串口
	nh.subscribe(sub_setup);        //消息订阅初始化,会影响串口
	//nh.subscribe(sub1);        //消息订阅初始化,会影响串口  //2018-12-08
#else
	Serial.begin(9600);//串口0 打印传感器消息 于ros 冲突
#endif


/* 
	//test 舵机(后刷）  
	//digitalWrite(motor_2, HIGH);
	servo_1 = 1;
	digitalWrite(DIR_B_BACK, HIGH); // LOW 后退   HIGH向前 
	digitalWrite(EN_B_BACK_PWM, LOW);
	analogWrite(PWM_B_BACK,40); 
	delay(3000);
	int k,j;
	for (k=0;k<100;k++)
	{
		for (j=0;j<10;j++)
		{
			angel_read_back = analogRead(ang_pin);
			Serial.println(angel_read_back);
			delay(100);
		}
//		digitalWrite(DIR_B_BACK, LOW); // LOW 后退   HIGH向前 
//		delay(3000);
//		digitalWrite(DIR_B_BACK, HIGH); // LOW 后退   HIGH向前 
//		delay(3000);
	}	
*/

/*
	//test 前刷电机
	servo_2 = 1;
	digitalWrite(EN_B_FRONT_PWM, LOW);
	digitalWrite(DIR_BF, LOW); //  LOW -由上往下  
	
	//digitalWrite(motor_1, HIGH);
*/

/*
	//test B车行走电机
	std_msgs::UInt32 msg;
	msg.data = 0x04004000;
	Motor_contrl(msg);	

*/
/*
		std_msgs::UInt32 msg;
		msg.data = 0xb400;
		Motor_1_contrl(msg);	
*/
/*
	servo_2 = 1;
	digitalWrite(DIR_BF, HIGH); // LOW 后退   HIGH向前 
	digitalWrite(EN_B_FRONT_PWM, LOW);
	analogWrite(PWM_BF,40); 
	delay(1000000);
	int k,j;
	for (k=0;k<100;k++)
	{
		digitalWrite(DIR_BF, LOW); // LOW 后退   HIGH向前 
		delay(3000);
		digitalWrite(DIR_BF, HIGH); // LOW 后退   HIGH向前 
	}
	
*/

//测试A车
//	std_msgs::UInt32 msg;
//	msg.data = 0x00800000;
//	Motor_contrl(msg);	
//	digitalWrite(EN_A_PWM,LOW);
//	analogWrite(PWM_A,0x40); 	
//	digitalWrite(DIR_A, LOW);   // LOW 后退   HIGH向前  
//	delay(100000);
	
	pinMode(ALARM, OUTPUT);
	digitalWrite(ALARM, LOW);//输出LOW电平,停止发声
		
	//超声初始化
	pinMode( trigger_io, OUTPUT);
	digitalWrite(trigger_io, LOW);
	
	pinMode(pwm_io_1, INPUT_PULLUP);          // 中断输入口  上拉，确保不接传感器是，电平稳定，不然会经常进端口中断
	attachInterrupt(0, DYP_plus_measure_1, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
	pinMode(pwm_io_2, INPUT_PULLUP);          // 中断输入口
	attachInterrupt(1, DYP_plus_measure_2, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
	pinMode(pwm_io_3, INPUT_PULLUP);          // 中断输入口
	attachInterrupt(2, DYP_plus_measure_3, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数

	localctrl_msg.data = 0x00000000;
	
	localctrl_mode = 0;

   Timer3.initialize(4000);  //10000 //2000
   Timer3.attachInterrupt(task);
   Timer3.start();
	
	wdt_enable(WDTO_250MS); //120
}

//// 主循环 //////
void loop()
{
	//如果在非本地控制模式下，才处理来自Firefly的消息
	if (localctrl_mode==0)
		nh.spinOnce();

	if (send_ros_flag==true)
	{
		send_ros_flag = false;
		send_ros();           // ros消息上传
	}
	
	if (send_upper_ctrl==true)
	{
		send_upper_ctrl = false;
		Localctrl.publish(&to_upper_ctrl_msg);
	}		

#ifndef test_motor  
	//如果在非本地控制模式下，才进行判断
	if (localctrl_mode==0)
	{	
		if(!master_firefly_online  	// 主机离线 关机所有电机==============
			|| dz_motor_flag		// 电机堵转 关闭所有电机==============
			|| (gyro_failure_counter>30)  //陀螺仪失效 =====================
			|| (dyp1_failure_counter>10)  //A车顶超声失效 =====================
			|| (dyp2_failure_counter>10)  //B车左前超声失效 =====================
			|| (dyp3_failure_counter>10)  //B车左后超声失效 =====================
		){
			// 可能还需要一个标志，确保master_firefly_online在变换时，只执行一次
			analogWrite(PWM_left,0);
			analogWrite(PWM_right,0);
			analogWrite(PWM_A,0);
			analogWrite(PWM_BF,0);
		
			// 关闭 继电器电机=========
			digitalWrite(motor_1, LOW);
			digitalWrite(motor_2, LOW);
			digitalWrite(motor_3, LOW);
			digitalWrite(motor_4, LOW);
	
			//B车使能信号拉高（关断）
			digitalWrite(EN_B_LEFT_PWM, HIGH);
			digitalWrite(EN_B_RIGHT_PWM, HIGH);
			digitalWrite(EN_B_FRONT_PWM, HIGH);
			digitalWrite(EN_B_BACK_PWM, HIGH);  //2018-12-08
			
			// 关闭舵机 ===========
			servo_1 = 0;
			servo_2 = 0;
		}
	}
#endif

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

	//堵转电流检测
	if (check_motor_circuit_flag)
		check_motor_circuit();   


	if (read_analog_fla==true) {
		read_analog_value();
		read_analog_fla = false;
	}

//	Serial.println(test_motor_counter);
/*	if (test_motor_counter==5000) {
		test_motor_counter = 7000;
		//test B车行走电机
		std_msgs::UInt32 msg;
		msg.data = 0x21000000;
		Motor_contrl(msg);	
	}
*/	

	//串口连接的是电流计
	while (Serial2.available()) {
		data[count] = (unsigned char)Serial2.read();//读取一个新字节
		if(count==0 && data[0]!=0x5A) 
			return;//如果读取的第一个字节不是0x5A，跳出去
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
	
	//串口1连接的是陀螺仪,每100ms读一次
	while (Serial1.available()) {
		JY901.CopeSerialData(Serial1.read());
		gyro_failure_counter = 0;
	}

	//串口3连接的是遥控手柄
	while  (Serial3.available()) {

			data_ps[count_ps] = (unsigned char)Serial3.read();//读取一个新字节
		if(count_ps==0 && data_ps[0]!=0xAA) 
			return;//如果读取的第一个字节不是0xAA，跳出去
		if(count_ps==1 && data_ps[1]!=0xBB) { 
			count_ps = 0;
			return;//如果读取的第2个字节不是0xBB，跳出去
		}
		count_ps++;

		if(count_ps == 8){//当读满8个字节
			count_ps = 0;
			
			//CRC校验
			unsigned int crc_byte = 0;//定义CRC校验变量
			for(int i = 0; i < 7; i++)
				crc_byte += data_ps[i];
			while(crc_byte > 255)
				crc_byte -= 255;
			if (crc_byte != data_ps[7])
				return;
			
			buttons = data_ps[2];
			L1int = data_ps[3];
			L2int = data_ps[4];
			R1int = data_ps[5];
			R2int = data_ps[6];
			
			//if (buttons!=99)
			//	Serial.println(buttons);


			//处理收到的数据
			//localctrl_mode = true;
			localctrl_timer = 0;  //清除本地控制定时器
			
			//执行本地遥控
			//if (localctrl_mode==true) {
			localctrl();
			//}
			
			break;
		}
		
}

#if 0
	//长时间收不到遥控手柄的消息(1.5秒：停下来）
	if (localctrl_timer>300 && localctrl_timer<400)
	{
		buttons = 12;
		localctrl_timer = 401;
		localctrl();
	}
	else if (localctrl_timer>6000) //(30s,转自动模式）
	{
		localctrl_mode = 0;
	}
#endif
	
	if(gyro_sample_fla){
		gyro_sample_fla = 0;
		angle_z = JY901.stcAngle.Angle[2];
	}
	
}

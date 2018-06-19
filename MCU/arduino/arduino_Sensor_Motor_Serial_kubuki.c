//#define TEST//测试

// mega 2560
// 25GA-370 电机 280rpm  编码器 11线  11*4 =44  减速比 21/22？ 一圈 924/968脉冲  实测为 941(测量40圈求平均) 即减速比不为整数
// 轮直径 0.065m  速度 0.1m/s   0.1/0.065/3.14*941/1000=0.461脉冲 1ms    100ms为46脉冲   50ms  为 23脉冲
// 0.6m/s 2.7脉冲 1ms  100ms 为 270脉冲 50ms  为  135脉冲  
// 0.7m/s            158脉冲
// 0.8m/s            181脉冲
// 0.9m/s            204脉冲
// 1.0m/s            227脉冲
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "PID.h"//优先本地寻找
#include <PWM.h>
#include "SR04.h"//超声传感器
// #include <MsTimer2.h>  mega 2560  用不了
//pid控制器 定时器
#include <FlexiTimer2.h>     // Timer2  UN0 PWM 3 和 11用不了   2560  PWM 9和10 用不了
//#include <TimerOne.h>       // Timer1  UNO PWM 9 10口 用不了   2560  PWM  11  12   用不了   PWM库是 基于Timer1 Timer2的
// timer0 TimerZero 定时器 为系统计时函数 millis() pulsein 是调用 millis() 的
//信息发送 定时器  系统定时器
#include <TimerThree.h>       // Timer3

#ifdef TEST
const long interval = 500000; // 500000 microseconds = 500 ms   // 一般上位机发送测试
#else
const long interval = 1000; // 1000 microseconds = 1 ms
#endif

PID car_pid;//创建PID控制对象

volatile bool Left_Direction = false, Right_Direction = false; //0反转 1正转
volatile long timer = 0, pub_countL = 0, pub_countR = 0, pluse = 0;

word front_left = 0;           // 距离变量
word front_right = 0;
const int adc_charge_pin  = A0;// 充电电流ADC检测口
const int adc_battery_pin = A1;// 电池电压ADC检测口
const int falling_pin     = A2;// 防跌落红外传感器
const int falling_pin_left= A3;// 防跌落红外传感器 左轮 侧
const int falling_pin_right= A4;// 防跌落红外传感器 右轮 侧

volatile unsigned int adc_charge_value = 0;// 放电电流
volatile unsigned int temp_charge_value = 0;
uint64_t over_cur_time_ms = 0;

int adc_battery_value = 0;     // 电池电压
volatile bool left_wheel_over_cur = false; //左轮过流
volatile bool right_wheel_over_cur = false; //右轮过流

int adc_FallingSensorValue = 0;  // 防跌落红外传感器电压
float  vertical_Distance;        // 垂直距离 cm
volatile bool forward_clif_flag = false; //前置 防跌落检测

int adc_FallingSensorValue_left = 0;  // 左轮 防跌落红外传感器电压
float  vertical_Distance_left;        // 垂直距离 cm
volatile bool left_clif_flag = false; //左侧 防跌落检测

int adc_FallingSensorValue_right = 0;  // 防跌落红外传感器电压
float  vertical_Distance_right;        // 垂直距离 cm
volatile bool right_clif_flag = false; //前置 防跌落检测

volatile unsigned char timeCounts = 0;  //时间计数
unsigned char timeCounts2 = 0;  //时间计数
volatile bool task_flag = false; //任务标志
volatile unsigned char delayCounter = 3;  //3个64ms延迟
volatile bool sys_led_flag = false; //任务标志

#define En_CHL           2  // 编码器计数口 A相 左轮 对应中断0
#define En_CHLB          3  // 编码器计数口 A相 左轮 对应中断1
#define En_CHR           18  // 编码器计数口 A相 右轮 对应中断5
#define En_CHRB          19  // 编码器计数口 A相 右轮 对应中断4
//超声传感器
//#define TRIG_PIN 4//
//#define ECHO_PIN 5
//SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
//long tem_forw_dist;
//float forward_dist_4 = 0;

#define Frequence     18000 // PWM 波频率

#define PWM_left      11  // 左轮电机 pwm 口   6  TIMER3 pwm频率库肯不支持
#define PWM_right     12  // 右轮电机 PWM 口   7
#define DIR_left      22 // 左轮方向 IO口
#define DIR_right     23 // 右轮方向 IO口

volatile unsigned int SerialComCount[2] = {0}; //串口接收计数
volatile bool serial_cmd_flag = false;
int arg = 0;
int index = 0;

//Vx Vy W 转换成 整数
volatile float arg1 = 0.0;
volatile float arg2 = 0.0;
volatile float arg3 = 0.0;

volatile float V_L = 0.0; //左轮速度 -+ 0~0.6m/s
volatile float V_R = 0.0; //右轮速度 -+ 0~0.6m/s

//kobuki接口解析
#define clearBuffer 0
#define waitingForStx 1
#define waitingForPayloadSize 2
#define waitingForPayloadToEtx 3
#define waitingForEtx 4

//koubki接口消息头
#define u8  unsigned char
#define u16 unsigned short int
#define s16 signed short int

// 串口收到的字符
u8 chr;

//kobuki接口解析的状态
u8 cmd_state = clearBuffer;
u8 size_length_field = 1;
u8 size_payload;
bool found_package = false;
u8 identifier;
s16 motor_speed;
s16 motor_radius;
bool retController = false;
bool retExtData = false;
u8   extDataFlag;
volatile bool send_flag = false;

//串口收到的字符Buffer  （Kobuki)
u8 buffer1[50] = {0}; //用于仿kobuki
unsigned int index_ = 0;
u8 STX[2] ;//接收匹配数据头

volatile u16 send_time_step = 0;

volatile u16 u16EncoderSum_l = 0;
volatile u16 u16EncoderSum_r = 0;

//串口2 变量
unsigned char Re_buf[11],counter=0;//串口接收缓冲区
unsigned char sign=0;
float a[3],w[3],angle[3],T;//三轴加速度 角速度 角度 温度
volatile unsigned char z_angular_vel[2];//z轴角速度 0 低八位 1 高八位
volatile unsigned char z_angel[2];      //z轴角度   0 低八位 1 高八位

/* 当前控制命令参数重置为0*/
///*
// 解析上位机得到的指令
char cmd;
// 字符后的 三个参数
char argv1[32];
char argv2[32];
char argv3[32];
void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  memset(argv3, 0, sizeof(argv3));//缓冲区置0
  arg1 = 0;
  arg2 = 0;
  arg3 = 0;
  arg = 0;
  index = 0;
}
// Run a command.  Commands are defined in commands.h
#define Velocity 'v' //速度指令
int runCommand() {
  arg1 = atof(argv1);//字符转化成整数   Vx
  arg2 = atof(argv2);//                Vy
  arg3 = atof(argv3);//                W
  switch (cmd) {
    case Velocity:  //'v' 波特率
    
        if(arg1==0){
          if((abs(arg3)<1.5)&&(arg3<0)) arg3=-1.5;
          if((abs(arg3)<1.5)&&(arg3>0)) arg3=1.5;
          V_L = -0.154 * arg3 ;
          V_R = 0.154 * arg3  ;
        }
        else{
          V_L = arg1 - 0.154 * arg3;
          V_R = arg1 + 0.154 * arg3;
        }
        
      // 获取方向
      // Left_Direction=    arg2 & 0x01;// 左轮方向
      //  Right_Direction=   arg2 & 0x02;// 右轮方向
      // 设置 左轮方向
      if (V_L < 0) digitalWrite(DIR_left, LOW); // 后退
      else      digitalWrite(DIR_left, HIGH);  // 前进
      // 设置 右轮 方向
      if (V_R < 0) digitalWrite(DIR_right, LOW);
      else      digitalWrite(DIR_right, HIGH);
      //car_pid.Left_Speedwant =abs(V_L)/3.14/0.065*941/20;// 50ms内脉冲数 230.52
     if (forward_clif_flag ||left_clif_flag||right_clif_flag|| left_wheel_over_cur || right_wheel_over_cur) { //悬崖 或者过流
   // if (forward_clif_flag || left_clif_flag || left_wheel_over_cur || right_wheel_over_cur) { // 跌落  过流 停止
        car_pid.Left_Speedwant  = 0;
        car_pid.Right_Speedwant = 0;
      }
      else {
        car_pid.Left_Speedwant = abs(V_L) * 230.52;
        car_pid.Right_Speedwant = abs(V_R) * 230.52;
      }
      SerialComCount[0]++;//串口命令接收计数
      break;
  }
}
//*/

//1ms 定时器中断
void systme_time() {
  send_time_step++;//1ms加一次 0~65535
}
// 测试使用 串口
void send_msg_serial() {
//  Serial.print("cu: ");
//  Serial.println(adc_charge_value, DEC);
//  Serial.print("bt: ");
//  Serial.println(adc_battery_value, DEC);
//  Serial.print("vd: ");
//  Serial.println(vertical_Distance);
//  Serial.print("fd: ");
//  Serial.println(forward_dist_4);
//  Serial.print("z_a: ");
//  Serial.println(angle[2]); 
//  Serial.print("pid_o: ");
//  Serial.println(car_pid.Right_Output); 

}
void send_msg() {
  send_msg_serial();
}

////// 测量数据任务 150ms //////
void tesk() {
  if (task_flag) { // 任务
    task_flag = false;
    //    /*
    if ((!serial_cmd_flag) || forward_clif_flag || left_clif_flag || right_clif_flag || left_wheel_over_cur || right_wheel_over_cur) { //长时间未接收到  跌落  过流 停止
   // if ((!serial_cmd_flag) || forward_clif_flag || left_clif_flag || left_wheel_over_cur || right_wheel_over_cur) { //长时间未接收到  跌落  过流 停止
      car_pid.Right_Output = 0;
      car_pid.Left_Output  = 0;
      car_pid.Left_Speedwant  = 0;
      car_pid.Right_Speedwant = 0;
      car_pid.Err_Left[0]  = car_pid.Err_Left[1] = 0;
      car_pid.Error_i_Left = 0;
      car_pid.Err_Right[0] = car_pid.Err_Right[1] = 0;
      car_pid.Error_i_Right = 0;
    }
    //      */
    // 放电电流 电压
    temp_charge_value = analogRead(adc_charge_pin);  // 放电电流 电压
    adc_charge_value = (adc_charge_value * 4 + temp_charge_value) / 5.0;
    /*
      curent_pre[9]=curent_pre[8];//上上次的值
      curent_pre[8]=curent_pre[7];//上上次的值
      curent_pre[7]=curent_pre[6];//上上次的值
      curent_pre[6]=curent_pre[5];//上上次的值
      curent_pre[5]=curent_pre[4];//上上次的值
      curent_pre[4]=curent_pre[3];//上上次的值
      curent_pre[3]=curent_pre[2];//上上次的值
      curent_pre[2]=curent_pre[1];//上上次的值
      curent_pre[1]=curent_pre[0];//上次的值
      curent_pre[0]=adc_charge_value;//当前的值
      for(int i=0;i<10;i++) aver_cur += curent_pre[i];
      aver_cur =aver_cur/10.0;
      if((curent_pre[9] > 80)){
        //Serial.println((abs(curent_pre[1]-curent_pre[9]) > curent_pre[9]*0.06));
       // if( ((curent_pre[1]-curent_pre[9]) > curent_pre[9]*0.3)&&( (curent_pre[0]-curent_pre[9]) > curent_pre[9]*0.3 )&&(curent_pre[9]>80)&&((car_pid.countL<car_pid.Left_Speedwant)||(car_pid.countR<car_pid.Right_Speedwant)) )
        if(((curent_pre[0]-aver_cur) > aver_cur*0.3)&&((curent_pre[1]-aver_cur) > aver_cur*0.3)&&((car_pid.countL<car_pid.Left_Speedwant)||(car_pid.countR<car_pid.Right_Speedwant))&&(curent_pre[0]>130))
         {
           over_cur_fg[2]=over_cur_fg[1];
           over_cur_fg[1]=over_cur_fg[0];
           over_cur_fg[0]=true;
         }
         else over_cur_fg[0]=false;
         if(over_cur_fg[0]&&over_cur_fg[1]&&over_cur_fg[2]) {
               left_wheel_over_cur=true;  right_wheel_over_cur = true;//过流
          }
         else
         {
           left_wheel_over_cur=false;  right_wheel_over_cur = false;//没有过流
          }
      }
      else
      {
      left_wheel_over_cur=false;  right_wheel_over_cur = false;//没有过流
      }
    */

    if (adc_charge_value > 500) {
      over_cur_time_ms = millis();//毫米  micros() 微妙
      left_wheel_over_cur = true;
      right_wheel_over_cur = true;
    }
    //else if((adc_charge_value < 80)&&((millis()-over_cur_time_ms)>5000)){//5s后再启动
    else if((millis()-over_cur_time_ms) > 4000){//4s后再启动
      left_wheel_over_cur = false;
      right_wheel_over_cur = false;
      }

    // 电池电压  23.5v ---> 820 按最高29.35v 对应 1023预测
    adc_battery_value = analogRead(adc_battery_pin);// 电池电压  需要map  到167内(代表16.7v)上层认为 16.7v为满电压
    adc_battery_value = map(adc_battery_value, 0, 1023, 0, 167);//需要修改 1023的值 为 电池满电压对应的 值
    //红外距离传感器
    adc_FallingSensorValue = analogRead(falling_pin);//红外距离传感器 电压
    vertical_Distance = adc_FallingSensorValue / 1024.0 * 5.0;
    vertical_Distance = 5.3681 * pow(vertical_Distance, -1.366); //垂直距离  公式实际测试 有 excel 散点图 幂函数得到
    //垂直跌落标志
    if (vertical_Distance > 12) forward_clif_flag = true;
    else forward_clif_flag = false;
    
    //红外距离传感器 左侧
    adc_FallingSensorValue_left = analogRead(falling_pin_left);//红外距离传感器 电压
    vertical_Distance_left  = adc_FallingSensorValue_left  / 1024.0 * 5.0;
    vertical_Distance_left  = 5.3681 * pow(vertical_Distance_left , -1.366); //垂直距离  公式实际测试 有 excel 散点图 幂函数得到
    //垂直跌落标志
    if (vertical_Distance_left  > 12) left_clif_flag = true;
    else left_clif_flag = false;

    //红外距离传感器 右侧
    adc_FallingSensorValue_right = analogRead(falling_pin_right);//红外距离传感器 电压
    vertical_Distance_right = adc_FallingSensorValue_right / 1024.0 * 5.0;
    vertical_Distance_right = 5.3681 * pow(vertical_Distance_right, -1.366); //垂直距离  公式实际测试 有 excel 散点图 幂函数得到
    //垂直跌落标志
    if (vertical_Distance_right > 12) right_clif_flag = true;
    else right_clif_flag = false;
    
    
    //前方超声传感器
    //tem_forw_dist = sr04.Distance();
    //forward_dist_4 = (forward_dist_4 * 3 + tem_forw_dist) / 4.0;
  }
}
//每50ms执行一次 PID控制
void Control_Period()
{
  car_pid.PID_cacu_control();
  if (!Left_Direction)  u16EncoderSum_l -=  (u16)(2.722 * car_pid.countL); //反转
  else                 u16EncoderSum_l +=  (u16)(2.722 * car_pid.countL);//正转
  if (!Right_Direction) u16EncoderSum_r -=  (u16)(2.722 * car_pid.countR); //反转
  else                 u16EncoderSum_r +=  (u16)(2.722 * car_pid.countR);//正转
  Serial.println(car_pid.countL);
  car_pid.countR = car_pid.countL = 0; //编码器计数清零
}
// 50ms 中断函数   pid控制
void flash()
{
  Control_Period();
  timeCounts++;  // 超声波传感器 测量相关系数
  timeCounts2++;
  if (timeCounts >= 3) {
    timeCounts = 0;
    task_flag = true;//任务标志 150ms
  }
  if ( timeCounts2 == 5 ) SerialComCount[1] = SerialComCount[0]; //串口接收
  if ( timeCounts2 >= 20){
      timeCounts2 = 0;
      if (SerialComCount[0] - SerialComCount[1] > 2 ) serial_cmd_flag = true; //有收到控制命令
      else serial_cmd_flag = false; //长时间未收到控制命令  60ms内必须接收到 命令
      SerialComCount[1] = SerialComCount[0] = 0;
    }
  
}
//##############编码器中断计数##########
////// 左轮 编码器计数  A 相////
void interruptL()
{
  car_pid.countL += 1;
}
////// 左轮 编码器计数 B相////
void interruptLB()
{
  car_pid.countL += 1;
}
///// 右轮 编码器计数 A 相////
void interruptR()
{
  car_pid.countR += 1;
}
///// 右轮 编码器计数 B相////
void interruptRB()
{
  car_pid.countR += 1;
}
//##############编码器中断计数#################

// kubuki 函数

/* 发送基本传感器数据给Firefly */
void sendBasicData()
{
  u8 cs;
  u8 i;
  //当前时间戳 0~65535循环
  u16 cur_time = send_time_step;
  //左编码器累加值
  u16 encoder_left =  u16EncoderSum_l;
  //右编码器累加值
  u16 encoder_right = u16EncoderSum_r;
  //格式转换
  u8 sendBuf[21];
  //#if 0
  sendBuf[0] = 0xAA;//Header 0
  sendBuf[1] = 0x55;//Header 1
  sendBuf[2] = 0x11;//payload长度 = 17  总payload长度
  sendBuf[3] = 0x01;//payload id = 1    Sub-Payload 0  Payload 标识符
  sendBuf[4] = 0x0F;//payload length = 15
  sendBuf[5] = cur_time & 0x00FF; //ayload 时间戳 低位
  sendBuf[6] = (cur_time & 0xFF00) >> 8 ; //payload 时间戳 高位
  sendBuf[7] = 0x00;//payload 减震器 0x01右减震器 0x02中间  0x04 左边
  sendBuf[8] = 0x00;//payload 轮跌落
  sendBuf[9] = 0x02 & (forward_clif_flag << 1); //payload 悬崖跌落  0x01右跌落 0x02中间跌落  0x04 左边跌落
  sendBuf[10] = encoder_left & 0x00FF; //payload 左编码器累加值低位
  sendBuf[11] = (encoder_left & 0xFF00) >> 8; //payload 左编码器累加值高位
  sendBuf[12] = encoder_right & 0x00FF; //payload 右编码器累加值低位
  sendBuf[13] = (encoder_right & 0xFF00) >> 8; //payload 右编码器累加值高位
  sendBuf[14] = 0x00;//payload PWM左   有符号 代表正反转
  sendBuf[15] = 0x00;//payload PWM右
  sendBuf[16] = 0x00;//payload button
  sendBuf[17] = 0x00;//payload charge
  sendBuf[18] = adc_battery_value & 0x00FF; //payload battery  电压 0.1v单位  上层16.7v 认为满电压
  sendBuf[19] = (0x01 & left_wheel_over_cur) | (0x02 & (right_wheel_over_cur << 1)); //payload 过流告警  0x01 左轮  0x02 右轮
  //#endif
  //加上checksum校验位
  cs = 0x00;
  for (i = 2; i < 20; i++) cs ^= sendBuf[i];
  sendBuf[20] = cs;
  for (i = 0; i < 21; i++) Serial.write(sendBuf[i]);//发送ASCII码
}
/*惯性传感器数据*/
void send_Inertial_SensorData()
{
  u8 cs;
  u8 i;
  //格式转换
  u8 sendBuf[13];
  //#if 0
  sendBuf[0] = 0xAA;//Header 0
  sendBuf[1] = 0x55;//Header 1
  sendBuf[2] = 0x09;//payload长度 = 9    
  sendBuf[3] = 0x04;//payload id = 4   惯性传感器数据
  sendBuf[4] = 0x07;//payload date length = 7
  
  sendBuf[5] = ((s16)(angle[2] * 100)) & 0x00FF;       // Angle 低八位   
  sendBuf[6] = (((s16)(angle[2] * 100)) & 0xFF00 ) >> 8;// Angle 高八位
  sendBuf[7] = ((s16)(w[2] * 100)) & 0x00FF;        // Angle rate 低八位  
  sendBuf[8] = (((s16)(w[2] * 100)) & 0xFF00 ) >> 8 ;  // Angle rate 高八位
  sendBuf[9] = 0x00 ;//加速度 空闲   
  sendBuf[10] = 0x00 ;
  sendBuf[11] = 0x00 ;
  //加上checksum校验位
  cs = 0x00;
  for (i = 2; i < 12; i++) cs ^= sendBuf[i];
  sendBuf[12] = cs;
  for (i = 0; i < 13; i++) Serial.write(sendBuf[i]);//发送ASCII码
}
/*电流数据*/
void send_Current_Data()
{
  u8 cs;
  u8 i;
  //格式转换
  u8 sendBuf[8];
  //#if 0
  sendBuf[0] = 0xAA;//Header 0
  sendBuf[1] = 0x55;//Header 1
  sendBuf[2] = 0x04;//payload长度 = 4   
  sendBuf[3] = 0x06;//payload id = 6    电机电流数据
  sendBuf[4] = 0x02;//payload date length = 2
  sendBuf[5] = adc_charge_value & 0x00FF; //  左轮 电流 单位10mA
  sendBuf[6] = adc_charge_value & 0x00FF; // 右轮
  //加上checksum校验位
  cs = 0x00;
  for (i = 2; i < 7; i++) cs ^= sendBuf[i];
  sendBuf[7] = cs;
  for (i = 0; i < 8; i++) Serial.write(sendBuf[i]);//发送ASCII码
}

void sendControllerData()
{
  //格式转换
  u8 sendBuf[19];
  u8 cs;
  u8 i;
  sendBuf[0] = 0xAA;//Header 0
  sendBuf[1] = 0x55;//Header 1
  sendBuf[2] = 0x0F;//payload长度 = 15
  sendBuf[3] = 0x15;//payload id = 21
  sendBuf[4] = 0x0D;//payload length = 13
  sendBuf[5] = 0x00;//pid_type
  sendBuf[6] = 0x00 ;//pid_p
  sendBuf[7] = 0x00 ;//pid_p
  sendBuf[8] = 0x00 ;//pid_p
  sendBuf[9] = 0x00 ;//pid_p
  sendBuf[10] = 0x00 ;//pid_i
  sendBuf[11] = 0x00 ;//pid_i
  sendBuf[12] = 0x00 ;//pid_i
  sendBuf[13] = 0x00 ;//pid_i
  sendBuf[14] = 0x00 ;//pid_d
  sendBuf[15] = 0x00 ;//pid_d
  sendBuf[16] = 0x00 ;//pid_d
  sendBuf[17] = 0x00 ;//pid_d
  //加上checksum校验位
  cs = 0x00;
  for (i = 2; i < 18; i++)  cs ^= sendBuf[i];
  sendBuf[18] = cs;
  //输出串口
  for (i = 0; i < 19; i++) Serial.write(sendBuf[i]);//发送ASCII码
}

void sendExtData()
{
  //格式转换
  u8 sendBuf[30];
  u8 cs;
  u8 i;

  //一起发过去得了（hardware firmware UniqueDeviceId)

  sendBuf[0] = 0xAA;//Header 0
  sendBuf[1] = 0x55;//Header 1
  sendBuf[2] = 0x1a;//payload长度 = 26
  sendBuf[3] = 0x0a;//payload id = 10  ------ hardware version
  sendBuf[4] = 0x04;//payload length = 4
  sendBuf[5] = 0x00;//patch
  sendBuf[6] = 0x03 ;//minor
  sendBuf[7] = 0x01 ;//major
  sendBuf[8] = 0x00 ;//unused
  sendBuf[9] = 0x0b ;//payload id = 11 ---- firmware version
  sendBuf[10] = 0x04 ;//payload len = 4
  sendBuf[11] = 0x00 ;//patch
  sendBuf[12] = 0x03 ;//minor
  sendBuf[13] = 0x01 ;//major
  sendBuf[14] = 0x00 ;//unused
  sendBuf[15] = 0x13 ;//payload id = 19 ---- UDID
  sendBuf[16] = 0x0c ;//payload len
  sendBuf[17] = 0x00 ;//udid
  sendBuf[18] = 0x00 ;//udid
  sendBuf[19] = 0x00 ;//udid
  sendBuf[20] = 0x00 ;//udid
  sendBuf[21] = 0x00 ;//udid
  sendBuf[22] = 0x00 ;//udid
  sendBuf[23] = 0x00 ;//udid
  sendBuf[24] = 0x00 ;//udid
  sendBuf[25] = 0x00 ;//udid
  sendBuf[26] = 0x00 ;//udid
  sendBuf[27] = 0x00 ;//udid
  sendBuf[28] = 0x01 ;//udid
  //加上checksum校验位
  cs = 0x00;
  for (i = 2; i < 29; i++) cs ^= sendBuf[i];
  sendBuf[29] = cs;
  for (i = 0; i < 30; i++) Serial.write(sendBuf[i]);//发送ASCII码
}

// 匹配数据帧头
bool WaitForStx(u8 datum)
{
  bool found_stx;
  u8 i;
  found_stx = true;
  buffer1[index_] = datum;
  index_++;
  for (i = 0; i < index_ && i < 2; i++)
  {
    if (buffer1[i] != STX[i])//匹配数据头
    { 
      found_stx = false;
      index_ = 0;
      break;
    }
  }
  return (found_stx && (index_ == 2));
}
// 数据包头 长度
bool waitForPayloadSize(unsigned char incoming)
{
  buffer1[index_] = incoming;
  index_++;
  if (index_ < 2 + size_length_field)  //如果buffer总长度 < (0xaa 0x55 + <payload siez> )
  {
    return false;
  }
  else
  {
    switch (size_length_field)
    {
      case 1: // kobuki
        size_payload = buffer1[2];//第三个字节为 数据包长度
        break;
      default:
        // put assertion failure
        size_payload = 1;
        break;
    }
    return true;
  }
}

bool waitForPayloadAndEtx(u8 incoming)
{
  buffer1[index_] = incoming;
  index_++;
  //Serial.println(size_payload);
  //Serial.println(index_);
  //Serial3.write(buffer1[0]);
  if ( size_payload > 10 ) { //如果长度太长了
    cmd_state = clearBuffer;
    return false;
  }
  if (index_ < 2 + size_length_field + size_payload + 1)
  {
    return false;
  }
  else
  {
    //Serial3.write(buffer1[3]);
    found_package = true;
    //Serial.println(found_package);
    return true;
  }
}


//用于仿kobuki接口的命令解析
bool update1(u8 incoming)
{
  found_package = false;//找到数据包标识符
 // Serial3.println(cmd_state);
//  if ( cmd_state == clearBuffer ) {//情况接收缓冲区
//    memset(buffer1, 0, sizeof(buffer1));
//    index_ = 0;
//    found_package = false;
//    cmd_state = waitingForStx;
//  }
  switch (cmd_state)
  {
    case waitingForStx:
      if (WaitForStx(incoming) == true)
      {
        cmd_state = waitingForPayloadSize; // kobukibot
      }
      break;
    case waitingForPayloadSize:
      if (waitForPayloadSize(incoming) == true)
      {
        cmd_state = waitingForPayloadToEtx;
      }
      break;

    case waitingForPayloadToEtx:
      if (waitForPayloadAndEtx(incoming) == true)
      {
        cmd_state = clearBuffer;
      }
      break;

    default:
      cmd_state = waitingForStx;
      break;
  }
  if (found_package) {
    //    printf("here return true");
    return true;  //what happen if checksum is equal to false(== -1)?
  } else {
    return false;
  }
}

void runCommand_Motor(s16 speed1, s16 radius)//公转角速度 mm/s  转弯半径mm
{
  /*
  //源码
  const double epsilon = 0.0001;

  // Special Case #1 : Straight Run
  if( std::abs(wz) < epsilon ) {
    radius = 0.0f;
    speed  = 1000.0f * vx;
    velocity_mutex.unlock();
    return;
  }
  radius = vx * 1000.0f / wz;
  // Special Case #2 : Pure Rotation or Radius is less than or equal to 1.0 mm
  if( std::abs(vx) < epsilon || std::abs(radius) <= 1.0f ) {
    speed  = 1000.0f * bias * wz / 2.0f;// wz = 2*speed/1000/bias
    radius = 1.0f;
    velocity_mutex.unlock();
    return;
  }

  // General Case :
  if( radius > 0.0f ) {
    speed  = (radius + 1000.0f * bias / 2.0f) * wz;
  } else {
    speed  = (radius - 1000.0f * bias / 2.0f) * wz;
  }
  */
 // Serial3.print("speed1 ");
 // Serial3.println(speed1);
  //表示直行
  if (radius == 0)
  {
    arg1 = (float)(speed1 / 1000.0);  //Vx mm/s变成 m/s
    arg2 = 0;
    arg3 = 0;
    //return;
  }
  else if (radius > 0)
  {
    arg2 = 0;
    arg3 = (float)(speed1 / (radius + 115.0 )) ; //自转角速度 500 * 0.308 =154 500 *直径
    arg1 = (float)(radius * arg3 / 1000.0);           //线速度
  }
  else if (radius == 1) //原地旋转   公转半径 1mm相当于自转  公转角速度 = 自转角速度 
  {
    arg1 = 0;
    arg2 = 0;
    arg3 = (float)(speed1  / 115.0); //自转角速度  实际 轮距 0.308m  上层 kubuki是0.23      500 * 0.23 = 115  
   // return;
  }
  else
  {
    arg2 = 0;
    arg3 = (float)(speed1 / ( radius - 115.0 )) ; //自转角速度
    arg1 = (float)(radius * arg3 / 1000.0) ;    //线速度
  }
      /*
     Serial3.print("x ");
     Serial3.print(arg1);
     Serial3.print("w ");
     Serial3.println(arg3); 
    */
//    if(arg1==0){
//      if((abs(arg3)<1.5)&&(arg3<0)) arg3 = -1.5;
//      if((abs(arg3)<1.5)&&(arg3>0)) arg3 = 1.5;
//      V_L = -0.154 * arg3 ;
//      V_R = 0.154 * arg3  ;
//    }
//    else{
//      V_L = arg1 - 0.154 * arg3;
//      V_R = arg1 + 0.154 * arg3;
//    }
      V_L = arg1 - 0.154 * arg3;
      V_R = arg1 + 0.154 * arg3;
  // 获取方向
  if (V_L < 0) Left_Direction = false;  // 左轮方向 0 反转
  else      Left_Direction = true;      //  1 正转
  if (V_R < 0) Right_Direction = false; // 右轮方向 0 反转
  else      Right_Direction = true;     //  1 正转

  // 设置 左轮方向
  if (!Left_Direction) digitalWrite(DIR_left, LOW);  // 后退
  else      digitalWrite(DIR_left, HIGH);  // 前进
  // 设置 右轮 方向
  if (!Right_Direction) digitalWrite(DIR_right, LOW);
  else      digitalWrite(DIR_right, HIGH);

  //car_pid.Left_Speedwant =abs(V_L)/3.14/0.065*941/20;// 50ms内脉冲数 230.52
  if (forward_clif_flag || left_clif_flag || right_clif_flag || left_wheel_over_cur || right_wheel_over_cur) { //悬崖 或者过流
// if (forward_clif_flag || left_clif_flag || left_wheel_over_cur || right_wheel_over_cur) { //长时间未接收到  跌落  过流 停止
    car_pid.Left_Speedwant  = 0;
    car_pid.Right_Speedwant = 0;
  }
  else {
    car_pid.Left_Speedwant = abs(V_L) * 230.52;
    car_pid.Right_Speedwant = abs(V_R) * 230.52;
 }
  
//  Serial3.print("L: ");
//  Serial3.println(car_pid.Left_Speedwant, DEC);
//  Serial3.print("R");
//  Serial3.println(car_pid.Left_Speedwant, DEC);
  
  SerialComCount[0]++;//接收计数
}


void setup()
{
  // pid参数   100ms  90 30 10  p参数可能过大
  // 50ms  90 45 10
  car_pid.Kp_Left = car_pid.Kp_Right = 90;
  car_pid.Ki_Left = car_pid.Ki_Right = 45;
  car_pid.Kd_Left = car_pid.Kd_Right = 10;
  //delay(3000);
  //Serial.begin(9600);//与上位机通信 收发速度 编码器信息
  Serial.begin(115200);//与上位机通信 收发速度 编码器信息
  Serial2.begin(9600);// 惯性传感器模块
  //Serial3.begin(9600);// 惯性传感器模块
  ////编码器中断引脚初始化/////
  pinMode(En_CHL, INPUT);//设置中断脚为输入状态 2  0
  pinMode(En_CHLB, INPUT);//3                     1
  pinMode(En_CHR, INPUT); //18                    5
  pinMode(En_CHRB, INPUT);//19                    4
  // 上升沿触发
  attachInterrupt(0, interruptL, CHANGE);   // step0  input on interrupt 1 - pin 2
  attachInterrupt(1, interruptLB, CHANGE);  // step1  input on interrupt 1 - pin 3
  attachInterrupt(5, interruptR, CHANGE);   // step5  input on interrupt 1 - pin 18
  attachInterrupt(4, interruptRB, CHANGE);  // step4  input on interrupt 1 - pin 19
  /*
    型号         int.0 int.1 int.2 int.3 int.4 int.5
    UNO\Ethernet  2     3 　 　 　 　
    Mega2560      2     3     21     20    19   18
    Leonardo      3     2     0      1 　 　
    Due           　所有IO口均可
  */
  pinMode(LED_BUILTIN, OUTPUT);//系统运行指示灯
  //// 电机方向口///////
  pinMode(DIR_left, OUTPUT);   // 左轮
  digitalWrite(DIR_left, LOW); // LOW 后退   HIGH向前
  pinMode(DIR_right, OUTPUT);  // 右轮
  digitalWrite(DIR_right, LOW);
  //// 电机PWM口 //////
  SetPinFrequency(PWM_right, Frequence);//右电机控制频率初始化
  pwmWriteHR(PWM_right, 0);             //右电机控制占孔比初始化
  SetPinFrequency(PWM_left, Frequence); //左电机控制频率初始化
  pwmWriteHR(PWM_left, 0);              //左电机控制占孔比初始化
  //// 两个定制器//////
  // 电机 pid 50ms 中断 外加 超声测量 编制设定
  FlexiTimer2::set(50, 1.0 / 1000, flash); // call every 50 ms "ticks"
  FlexiTimer2::start();
  // 发送数据 定时器中断
  Timer3.initialize( interval );
  // Timer3.attachInterrupt( systme_time );//1ms 时间戳
  
  #ifdef TEST
    Timer3.attachInterrupt( send_msg );      //测试  500ms上传信息
  #else
    Timer3.attachInterrupt( systme_time );   //1ms 时间戳
  #endif

  /*测试代码*/
  // 设置左右轮子期望值
  //car_pid.Left_Speedwant  = 138;        // 左轮PWM期望值   100ms 控制周期   0.1m/s 为 46脉冲 0.2m/s 92脉冲 0.4m/s  184脉冲
  //car_pid.Right_Speedwant = 184;         // 右轮PWM期望值  50ms  控制周期   0.1m/s 为 23脉冲 0.2m/s 46脉冲 0.4m/s  92脉冲  0.6m/s  138脉冲 0.8m/s  184脉冲

  //无pid测试
  // pwmWriteHR(PWM_left,30000);               //左电机控制占孔比初始化  25GA-370  电机 4500以上才转 最大65530
  // pwmWriteHR(PWM_right,10000);
  send_Inertial_SensorData();

  STX[0] = 0xaa;
  STX[1] = 0x55;

/* 
//  aa 55 6 1 4 cb ff 1 0 36 
//  aa 55 6 1 4 71 0 38 ff b5

  //only for test 
  char tempBuf[50]; 
  char tempIndex;
  tempIndex = 0;
  memset(tempBuf, 0, sizeof(tempBuf)); //For kobuki,清空串口命令缓存
  tempBuf[0] = 0xaa;
  tempBuf[1] = 0x55;
  tempBuf[2] = 0x06;//长度
  tempBuf[3] = 0x01;//标识符
  tempBuf[4] = 0x04;
  tempBuf[5] = 71;
  tempBuf[6] = 0x00;
  tempBuf[7] = 0x38;
  tempBuf[8] = 0xff;
  tempBuf[9] = 0xb5;
  tempBuf[10] = 0x00;
  tempBuf[11] = 0xaa;
  tempBuf[12] = 0x55;
  tempBuf[13] = 0x04;
  tempBuf[14] = 0x09;
  tempBuf[15] = 0x02;
  tempBuf[16] = 0x0b;
  tempBuf[17] = 0x00;
  tempBuf[18] = 0x04;
  tempBuf[19] = 0xaa;
  tempBuf[20] = 0x55;
  tempBuf[21] = 0x03;
  tempBuf[22] = 0x0e;
  tempBuf[23] = 0x01;
  tempBuf[24] = 0x00;
  tempBuf[25] = 0x0c;
  tempBuf[26] = 0x00;
  tempBuf[27] = 0x00;
  
  //如下是仿Kobuki接口
  for (int i=0;i<28;i++) 
  {
   chr = tempBuf[i];   
   if (update1(chr))
   {
    //解析数据
    //获取identifier
    identifier = buffer1[3];
    Serial.write(identifier);
    switch (identifier)
      {
      case 0x01: //Base Control
        motor_speed = buffer1[5] | ( (u16)(buffer1[6])<<8);
        motor_radius = buffer1[7] |( (u16)(buffer1[8])<<8); 
        Serial.print("motor_speed");
        Serial.println(motor_speed);
        Serial.print("motor_radius");
        Serial.println(motor_radius);
        runCommand_Motor(motor_speed,motor_radius);
        break;
      case 0x0e: //getController
        retController = true;
        break;
      default:
        break;
      } 
   }

  }
*/ 
}

//// 主循环 //////
void loop()
{
tesk(); //测量电池电压 充电电流  超声数据 红外数据  安全监测

#ifdef TEST
  //解析 上位机的 命令
  while (Serial.available() > 0) {  //串口内有数据
    // 读取下一个字节
    chr = Serial.read();

    // 一个命令结束后执行命令
    if (chr == 13) { // CR键值为 13  回车键 '/r'  十六进制为D
      if (arg == 1)      argv1[index] = NULL;//第二个参数  清空
      else if (arg == 2) argv2[index] = NULL;//第三个参数
      else if (arg == 3) argv3[index] = NULL;//第四个参数
      runCommand(); //执行命令
      resetCommand();
    }
    //分割命令行参数 命令内以空格 间隔
    else if (chr == ' ') {     //遇到空格了，是新的控制参数开始了
      // Step through the arguments
      if (arg == 0)    arg = 1;//上一个参数结束(控制标识) 第二个参数开始
      else if (arg == 1)  {
        argv1[index] = NULL;   //先清空 第二个参数存储区域argv1
        arg = 2;               //第三个参数开始
        index = 0;             //参数 下标归零
      }
      else if (arg == 2)  {
        argv2[index] = NULL;   //先清空第三个参数存储区域
        arg = 3;               //第四个参数开始
        index = 0;
      }
      continue;
    }
    // 得到命令的各个参数
    else {
      if (arg == 0) {
        // 第一个参数是 命令标志字符（一个字节）
        cmd = chr;
      }
      else if (arg == 1) {
        // 第二个参数
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {//第三个参数
        argv2[index] = chr;
        index++;
      }
      else if (arg == 3) {
        argv3[index] = chr;//第四个参数
        index++;
      }
    }
  }  
#else
  //串口0 解析 上位机的控制 命令
  while (Serial.available() > 0) {  //串口内有数据
    // 读取下一个字节
    chr = Serial.read();
    //如下是仿Kobuki接口
    if (update1(chr))//update(chr)返回1 说明收到了 完整的控制命令
    {
      //解析数据
      //获取identifier
      identifier = buffer1[3];//标识符
      switch (identifier)
      {
        case 0x01: //Base Control
          motor_speed = buffer1[5] | ( (u16)(buffer1[6]) << 8);
          motor_radius = buffer1[7] | ( (u16)(buffer1[8]) << 8);
          runCommand_Motor(motor_speed, motor_radius);
          break;
        case 0x0e: //getController
          retController = true;
          break;
        case 0x09: //get ext data
          retExtData = true;
          extDataFlag = buffer1[5];
          break;
        default:
          break;
      }    
    }
     // Serial3.print(cmd_state);
    if (!cmd_state) {//情况接收缓冲区
    memset(buffer1, 0, sizeof(buffer1));
    index_ = 0;
    found_package = false;
    cmd_state = waitingForStx;
  }
 }
 
  //返回Controller响应
  if (retController == true) {
    retController = false;
   // send_flag = true;
    sendControllerData();
  }
  if (retExtData == true) {
    retExtData = false;
    //send_flag = true;
    delayCounter = 5;
    sendExtData();
  }

  //每64ms发送一次惯性传感器数据  64 = 1000000B  ~ 111111B  = 0           
//  if ( (send_time_step & 0x003F) == 0 )// 64 = XX00 0000B & 0011 1111  0   64   128 ...
 if ( (send_time_step & 0x007F) == 0 )// 128ms = X000 0000B & 0111 1111  0   128  256 ...  
  {
       if (delayCounter>0) delayCounter --;
       send_Inertial_SensorData();
  }
   
 if (serial_cmd_flag==true)
 {
   if (delayCounter==0)
   {
       // if ( (send_time_step & 0x000F) == 0 )   // 16ms       XXXX 0000B ~ 0000 1111B  = xxx0 0000       0  16 32  48  64  80 ...
       //if ( (send_time_step & 0x003F) == 20 )     // 64ms间隔   XX01 0100B ~ 0011 1111B  = 0001 0100       20 84 144...
       if ( (send_time_step & 0x007F) == 40 )     // 128ms间隔    X010 1000B ~ 0111 1111B  = 0010 1000       40 168 296...       
          {
            sendBasicData();
          }
      // if ( (send_time_step & 0x003F) == 40 ) // 64ms    XX10 1000B ~ 0011 1111B  = 0010 1000         40 104 164...
       if ( (send_time_step & 0x007F) == 80 ) // 64ms      X101 0000B ~ 0111 1111B  = 0101 0000         80 208 316...
          {
           send_Current_Data();
          }
   } 
 }

#endif

////系统运行指示灯

    if((send_time_step & 0x01FF) == 0){//512ms   2^10  XX00 0000 0000  & 11 1111 1111   =  00 0000 0000
       sys_led_flag = !sys_led_flag;
       digitalWrite(LED_BUILTIN, sys_led_flag);
    }


// 惯性传感器 接收区
  while (Serial2.available()) {  
    Re_buf[counter]=(unsigned char)Serial2.read();//缓冲区
    if(counter==0&&Re_buf[0]!=0x55) return;        //第0号数据不是帧头              
    counter++;       
    if(counter==11)             //接收到11个数据
    {    
       counter=0;               //重新赋值，准备下一帧数据的接收 
       sign=1;                  //置位接收完成标识符
    }   
  }
  
  if(sign)
  {  
   sign=0;             //清零接收完成标志
   if(Re_buf[0]==0x55) //检查帧头
   {  
    switch(Re_buf [1])     //信息标识符
    {
//    case 0x51://加速度标识 X Y Z轴加速度
//      a[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*16;//单位为g 9.8m/s^2
//      a[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*16;
//      a[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*16;
//      T = (short(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25;//温度
//     // Re_buf [10]为校验和 
//     // Re_buf [10]=Sum=0x55+0x51+AxH+AxL+AyH+AyL+AzH+AzL+TH+TL
//      break;
    case 0x52://角速度标识  X Y Z轴角速度
      w[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*2000;//单位 度/s
      w[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*2000;
      w[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*2000;
      T = (short(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25;//温度 C摄氏度
      z_angular_vel[0] = Re_buf [6];//z轴角速度 低八位
      z_angular_vel[1] = Re_buf [7];//z轴角速度 高八位
      break;
    case 0x53://角度
      angle[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*180;//角度
      angle[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*180;
      angle[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*180;
      //T = (short(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25; 
      z_angel[0] =  Re_buf [6];//z轴角度 低八位
      z_angel[1] =  Re_buf [7];//z轴角度 高八位
   // 调用互补滤波算法 
   //filter.updateIMU(w[0], w[1], w[2], a[0], a[1], a[2]);
   //yaw = filter.getYaw();
  //Serial.println(yaw);
   // Serial.print("z_a: ");
   // Serial.println(angle[2]);
     break;
    } 
   }
 } 
  
}

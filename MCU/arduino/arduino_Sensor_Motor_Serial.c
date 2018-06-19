// mega 2560  
// 25GA-370 电机 280rpm  编码器 11线  11*4 =44  减速比 21/22？ 一圈 924/968脉冲  实测为 941(测量40圈求平均) 即减速比不为整数
// 轮直径 0.065m  速度 0.1m/s   0.1/0.065/3.14*941/1000=0.461脉冲 1ms    100ms为46脉冲
// 0.6m/s 2.7脉冲 1ms  100ms 为 270脉冲
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
//信息发送 定时器 
#include <TimerThree.h>       // Timer3
const long interval = 500000; // 50000 microseconds = 500 ms  
// Timer3.initialize( interval ); Timer1.attachInterrupt( ggyy );
                            
PID car_pid;//创建PID控制对象

volatile bool Left_Direction=0,Right_Direction=0;
volatile long timer=0,pub_countL=0,pub_countR=0,pluse=0;  
    
word front_left=0;             // 距离变量
word front_right=0;
const int adc_charge_pin  = A0;// 充电电流ADC检测口
const int adc_battery_pin = A1;// 电池电压ADC检测口
const int falling_pin     = A2;// 防跌落红外传感器

int adc_charge_value = 0;      // 放电电流 
int temp_charge_value=0;    
int adc_battery_value = 0;     // 电池电压

int adc_FallingSensorValue = 0;  // 防跌落红外传感器电压
float  vertical_Distance;    // 垂直距离 cm 

unsigned char timeCounts =0;   //时间计数
volatile bool task_flag=false; //任务标志

#define En_CHL           2  // 编码器计数口 A相 左轮 对应中断0
#define En_CHLB          3  // 编码器计数口 A相 左轮 对应中断1
#define En_CHR           18  // 编码器计数口 A相 右轮 对应中断5
#define En_CHRB          19  // 编码器计数口 A相 右轮 对应中断4

#define TRIG_PIN 4//
#define ECHO_PIN 5
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long tem_forw_dist;
float forward_dist_4 = 0;

#define Frequence     18000 // PWM 波频率

#define PWM_left         11  // 左轮电机 pwm 口   6  TIMER3 pwm频率库肯不支持
#define PWM_right        12  // 右轮电机 PWM 口   7
#define DIR_left         22 // 左轮方向 IO口
#define DIR_right        23 // 右轮方向 IO口


unsigned int SerialComCount[2];//串口接收计数
bool serial_cmd_flag=false;
int arg = 0;
int index = 0;
// 串口收到的字符
char chr;
// 解析上位机得到的指令
char cmd;
// 字符后的 三个参数
char argv1[32];
char argv2[32];
char argv3[32];
// 转换成 整数
volatile float arg1=0.0;
volatile float arg2=0.0;
volatile float arg3=0.0;

volatile float V_L=0.0;//左轮速度 -+ 0~0.6m/s
volatile float V_R=0.0;//右轮速度 -+ 0~0.6m/s
#define Velocity 'v' //速度指令
/* 当前控制命令参数重置为0*/
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

/* Run a command.  Commands are defined in commands.h */
int runCommand() {
  arg1 = atof(argv1);//字符转化成整数   Vx
  arg2 = atof(argv2);//                Vy
  arg3 = atof(argv3);//                W
  switch(cmd) {
  case Velocity:  //'v' 波特率
  V_L=arg1-0.193*arg3;
  V_R=arg1+0.193*arg3; 
  // 获取方向
   // Left_Direction=    arg2 & 0x01;// 左轮方向 
  //  Right_Direction=   arg2 & 0x02;// 右轮方向
  // 设置 左轮方向
    if(V_L<0) digitalWrite(DIR_left, LOW);   // 后退      
    else      digitalWrite(DIR_left, HIGH);  // 前进 
  // 设置 右轮 方向
    if(V_R<0) digitalWrite(DIR_right, LOW);              
    else      digitalWrite(DIR_right, HIGH); 
  //car_pid.Left_Speedwant =abs(V_L)/3.14/0.065*941/20;// 50ms内脉冲数 230.52
  car_pid.Left_Speedwant =abs(V_L)*230.52;
   car_pid.Right_Speedwant=abs(V_R)*230.52;
 //Serial.println(V_L, DEC);
 //Serial.println(V_R, DEC);
 SerialComCount[0]++;//接收计数
  break;
  }
}

  /*
////// ros_serial 发送话题信息 ////
void send_msg_ros_serial(){ 
   nh.spinOnce();
 // 编码器数据     
    ENC_Pluse.data=(0x00               & 0xffffffff)<<30
                   |(Left_Direction    & 0xffffffff)<<25
                   |(Right_Direction   & 0xffffffff)<<24
                   |(pub_countL        & 0xffffffff)<<12
                   |(pub_countR        & 0xffffffff);
    Pluse.publish(&ENC_Pluse);
    pub_countL = pub_countR = 0; // 清零编码器计数         
  // 超声
    ENC_Pluse.data=(0x01                            & 0xffffffff)<<30
                  |(long(0                        ) & 0xffffffff)<<18 
                  |(long(front_left/10            ) & 0xffffffff)<<9  // front_left
                  |(long(front_right/10           ) & 0xffffffff);    // front_right               
    Pluse.publish(&ENC_Pluse);
          
    //电压
    ENC_Pluse.data=(0x03                            & 0xffffffff)<<30
                   |(long(0                       ) & 0xffffffff)<<20 
                   |(long(adc_charge_value        ) & 0xffffffff)<<10 // charge
                   |(long(adc_battery_value       ) & 0xffffffff);    // battery                
    Pluse.publish(&ENC_Pluse);
  }
  */
  void send_msg_serial(){
    Serial.print("cu: ");
    Serial.println(adc_charge_value, DEC);
    Serial.print("vd: ");
    Serial.println(vertical_Distance); 
    Serial.print("fd: ");
    Serial.println(forward_dist_4);      
    
    }
void send_msg(){
  send_msg_serial();
  }

////// 测量数据任务//////
void tesk(){
  if(task_flag){// 任务
    task_flag = false;
    /*
    if(!serial_cmd_flag){//长时间未接收到 串口速度指令  停止 清理期望速度 和 pid误差
      car_pid.Left_Speedwant  = 0;        
      car_pid.Right_Speedwant = 0;
      car_pid.Err_Left[0]=car_pid.Err_Left[1]=0;
      car_pid.Error_i_Left=0;
      car_pid.Err_Right[0]=car_pid.Err_Right[1]=0;
      car_pid.Error_i_Right=0;      
      }
      */
    temp_charge_value = analogRead(adc_charge_pin);  // 放电电流 电压
    adc_charge_value =(adc_charge_value*4 + temp_charge_value)/5.0;
    adc_battery_value = analogRead(adc_battery_pin);// 电池电压
    //红外距离传感器 
    adc_FallingSensorValue = analogRead(falling_pin);//红外距离传感器 电压
    vertical_Distance =adc_FallingSensorValue/1024.0*5.0;
    vertical_Distance=5.3681*pow(vertical_Distance,-1.366);//垂直距离  公式实际测试 有 excel 散点图 幂函数得到
    //前方超声传感器
    tem_forw_dist = sr04.Distance();
    forward_dist_4 = (forward_dist_4*3 + tem_forw_dist)/4.0;
   }
}
//每50ms执行一次 PID控制
void Control_Period() 
{
   car_pid.PID_cacu_control();                           
   pub_countL+=car_pid.countL;
   pub_countR+=car_pid.countR;
   car_pid.countR=car_pid.countL=0;//编码器计数清零           
}
// 50ms 中断函数   pid控制
void flash()
{
  //Serial.println(car_pid.countL, DEC);
    Control_Period();
  // Serial.println(car_pid.countR, DEC);
  // car_pid.countL=0;
  // car_pid.countR=0;
   timeCounts++;  // 超声波传感器 测量相关系数
   if(timeCounts == 2) SerialComCount[1]=SerialComCount[0];//串口接收
   if(timeCounts>=4) {
      timeCounts=0;      
      task_flag = true;//任务标志
      if(SerialComCount[0]>SerialComCount[1]) serial_cmd_flag=true;
      else serial_cmd_flag=false;
      SerialComCount[1]=SerialComCount[0]=0;
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

void setup()
{
// pid参数   100ms  90 30 10  p参数可能过大
// 50ms  90 45 10    
    car_pid.Kp_Left=car_pid.Kp_Right=90;
    car_pid.Ki_Left=car_pid.Ki_Right=45;
    car_pid.Kd_Left=car_pid.Kd_Right=10;
//delay(3000);
   Serial.begin(9600);//与上位机通信 收发速度 编码器信息
////串口初始化，用于调试/////
  //Serial2.begin(9600); 
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
//// 电机方向口/////// 
   pinMode(DIR_left, OUTPUT);   // 左轮
   digitalWrite(DIR_left, LOW); // LOW 后退   HIGH向前  
   pinMode(DIR_right, OUTPUT);  // 右轮
   digitalWrite(DIR_right, LOW);  
//// 电机PWM口 //////
   SetPinFrequency(PWM_right, Frequence);//右电机控制频率初始化
   pwmWriteHR(PWM_right, 0);             //右电机控制占孔比初始化
   SetPinFrequency(PWM_left, Frequence); //左电机控制频率初始化
   pwmWriteHR(PWM_left,0);               //左电机控制占孔比初始化
//// 两个定制器//////   
  // 电机 pid 10ms 中断 外加 超声测量 编制设定 
   FlexiTimer2::set(50, 1.0/1000, flash); // call every 50 ms "ticks" 
   FlexiTimer2::start();
  // 发送数据 定时器中断   
 Timer3.initialize( interval );
 Timer3.attachInterrupt( send_msg );

/*测试代码*/
    // 设置左右轮子期望值         
    //car_pid.Left_Speedwant  = 138;        // 左轮PWM期望值    100ms 控制周期   0.1m/s 为 46脉冲 0.2m/s 92脉冲 0.4m/s  184脉冲
    //car_pid.Right_Speedwant = 184;         // 右轮PWM期望值  50ms  控制周期   0.1m/s 为 23脉冲 0.2m/s 46脉冲 0.4m/s  92脉冲  0.6m/s  138脉冲 0.8m/s  184脉冲
   
//无pid测试
   // pwmWriteHR(PWM_left,30000);               //左电机控制占孔比初始化  25GA-370  电机 4500以上才转 最大65530
   // pwmWriteHR(PWM_right,10000); 
}

//// 主循环 //////
void loop()
{
  tesk(); //测量电池电压 充电电流  超声数据 红外数据  安全监测
 
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
  
}

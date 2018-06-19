// mega 2560  
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <PID.h>
#include <std_msgs/UInt32.h>
#include <PWM.h>

// #include <MsTimer2.h>  mega 2560  用不了
#include <FlexiTimer2.h>     // Timer2  UN0 PWM 3 和 11用不了   2560  PWM 9和10 用不了
//#include <TimerOne.h>       // Timer1  UNO PWM 9 10口 用不了   2560  PWM  11  12   用不了   PWM库是 基于Timer1 Timer2的 
#include <TimerThree.h>       // Timer3

const long interval = 50000; // 50000 microseconds = 50 ms  
// Timer1.initialize( interval ); Timer1.attachInterrupt( ggyy );
#include <Wire.h> // i2c总线 

ros::NodeHandle   nh;        //创建ROS句柄
std_msgs::UInt32  ENC_Pluse; //定义编码器脉冲广播消息格式
ros::Publisher Pluse("Pluse", &ENC_Pluse);                                
PID car_pid;
volatile bool Left_Direction=0,Right_Direction=0;
volatile long timer=0,pub_countL=0,pub_countR=0,pluse=0;  
    
#define KS103_left  0xe8 >> 1  // 前左侧 超声I2C总线地址 0xe8
#define KS103_right  0xea >> 1 // 前右侧 超声I2C总线地址 0xea 
#define reduce_noise  0x73     // 噪声滤波指令
#define read_distance 0xb4     // 距离测量指令
word front_left=0;             // 距离变量
word front_right=0;
const int adc_charge_pin  = A0;// 充电电流ADC检测口
const int adc_battery_pin = A1;// 电池电压ADC检测口
int adc_charge_value = 0;      // 充电电流电压      
int adc_battery_value = 0;     // 电池电压

unsigned char timeCounts =0;   //超声 测量标志 时间计数
bool left_10ms = false;
bool left_20ms = false;
bool right_130ms = false;
bool right_140ms = false;
bool adc_det     = false;      //电压测量标志

#define En_CHL           2  // 编码器计数口  左轮 对应中断0 
#define En_CHR           3  // 编码器计数口  右轮 对应中断1
#define Frequence     18000 // PWM 波频率

#define PWM_left         11  // 左轮电机 pwm 口   6  TIMER3 pwm频率库肯不支持
#define PWM_right        12  // 右轮电机 PWM 口   7
#define DIR_left         22 // 左轮方向 IO口
#define DIR_right        23 // 右轮方向 IO口

///// 电机控制 监控////////
void Speed_SerialMonitor()
{
        Serial2.print("\nright    ");
        Serial2.print("-----");
        Serial2.println(car_pid.countR);
        Serial2.print("left    "); 
        Serial2.print("-----");
        Serial2.println(car_pid.countL); 
}
////// 发送串口调试信息//////
void send_msg_serial(){
 // /*   
  Serial2.print("L: ");
  Serial2.print("\t");
  Serial2.print(front_left/10, DEC);
  Serial2.print("\t");
  Serial2.print("R: ");
  Serial2.print("\t");
  Serial2.print(front_right/10, DEC);
  Serial2.print("\t");
  Serial2.print("C: ");
  Serial2.print("\t");
  Serial2.print(adc_charge_value, DEC);
  Serial2.print("\t");
  Serial2.print("B: ");
  Serial2.print("\t");
  Serial2.println(adc_battery_value, DEC);
 // */
  /*
  Serial2.print("LR: ");
  Serial2.print(car_pid.Left_Speedwant, DEC);
  Serial2.print("\t");
  Serial2.print("RR: ");
  Serial2.print(car_pid.Right_Speedwant, DEC);
  //Serial2.print("\t");
  Serial2.print("LO: ");
  Serial2.print(car_pid.Left_Output, DEC);
  Serial2.print("\t");
  Serial2.print("RO: ");
  Serial2.println(car_pid.Right_Output, DEC);
  //Serial2.print("\t");
   */
  } 
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
void send_msg(){
 // send_msg_serial();
  send_msg_ros_serial();
  }


////// 发送命令程序///////
void KS103_command(char addr ,byte command){  
  Wire.beginTransmission(addr); // 开始通信
  Wire.write(byte(0x02));       // 写控制寄存器
  Wire.write(command);          // 发送控制命令
  Wire.endTransmission();       // 结束通信
}

/////// 前左侧 超声数据 //////
void KS103_read_command_left(byte command){  // sending the command and read the data in the register
  if(left_10ms){
      left_10ms = false; 
      KS103_command(KS103_left,command); // 发送测量 命令
  }
  if(left_20ms){
     left_20ms = false;
     Wire.beginTransmission(KS103_left); // 通信
     Wire.write(byte(0x02));             // 发送读取数据命令-
     Wire.endTransmission();       
     Wire.requestFrom(KS103_left, 2);    // 接收数据
     if(2 <= Wire.available())           // 等待数据接收完成
     {
       front_left = (Wire.read()) << 8;  // 高八位
       front_left |= Wire.read();        // 低八位
      }
 }
}
/////// 前右侧 超声数据 /////////
void KS103_read_command_right(byte command){  // sending the command and read the data in the register
  if(right_130ms){
     right_130ms=false;
     KS103_command(KS103_right,command);  // 发送测量 命令
  }
  if(right_140ms){
     right_140ms = false;
     Wire.beginTransmission(KS103_right); // 通信
     Wire.write(byte(0x02));              // 发送读取数据命令 -   
     Wire.endTransmission();      
     Wire.requestFrom(KS103_right, 2);    // 接收数据
     if(2 <= Wire.available())            // 等待数据接收完成
      {
        front_right = (Wire.read()) << 8; // 高八位.
        front_right |= Wire.read();       // 低八位
      }
  }
}
////// 测量数据任务//////
void tesk(){
  KS103_read_command_left(read_distance); //前左超声测距
  KS103_read_command_right(read_distance);//前右超声测距
  if(front_left>5110)  front_left=5110;
  if(front_right>5110) front_right=5110; 
  if(adc_det){// 电压检测
    adc_charge_value = analogRead(adc_charge_pin);  // 充电电压
    adc_battery_value = analogRead(adc_battery_pin);// 电池电压 
    adc_det = false;
   }
}
// 订阅话题的 回调函数
void Motor_contrl( const std_msgs::UInt32& cmd_msg)
{ // 获取方向
    Left_Direction=cmd_msg.data/10000000;          // 左轮方向 
    Right_Direction=cmd_msg.data%10000000/1000000; // 右轮方向
  // 设置 左轮方向
    if(Left_Direction==1)      digitalWrite(DIR_left, LOW);   // 后退      
    else if(Left_Direction==0) digitalWrite(DIR_left, HIGH);  // 前进 
  // 设置 右轮 方向
    if(Right_Direction==1)     digitalWrite(DIR_right, LOW);              
    else if(Right_Direction==0)digitalWrite(DIR_right, HIGH); 
  // 设置左右轮子期望值         
    car_pid.Left_Speedwant = cmd_msg.data%1000000/1000; // 左轮PWM期望值
    car_pid.Right_Speedwant = cmd_msg.data%1000;        // 右轮PWM期望值
}
// 订阅 命令发布话题 
ros::Subscriber<std_msgs::UInt32> sub("motor", Motor_contrl);

void Control_Period() 
{
   car_pid.PID_cacu_control();                           
   pub_countL+=car_pid.countL;
   pub_countR+=car_pid.countR;
   car_pid.countR=car_pid.countL=0;
   timer++;                
}

// 10ms 中断函数 
void flash()
{
   Control_Period();
   timeCounts++;  // 超声波传感器 测量相关系数
   if(timeCounts==1)  left_10ms = true;
   if(timeCounts==2)  left_20ms = true;
   if(timeCounts==13) right_130ms = true;
   if(timeCounts==14) right_140ms = true;
   if(timeCounts>=24) {
      timeCounts=0; 
      adc_det = true;
   }
}
///// 右轮 编码器计数////
void interruptR()                      
{
   car_pid.countR += 1;
}
////// 左轮 编码器计数////
void interruptL()
{
   car_pid.countL += 1;
}

void setup()
{
/////初始化I2C总线 和超声传感器/////
   Wire.begin();                           // I2C总线 
   delay(5);
   KS103_command(KS103_left,reduce_noise); // 前左超声 降噪
   delay(5);  
   KS103_command(KS103_right,reduce_noise);// 前右超声 降噪
   delay(3000);                            // 等待 稳定工作
////ROS节点初始化///////////
   nh.initNode();                         //消息订初始化,会影响串口0   2560 serial1  serial2 serial3 可以用
   delay(3000);
   nh.subscribe(sub);                //消息订初始化,会影响串口
   nh.advertise(Pluse);              //消息发布初始化
////串口初始化，用于调试/////
  //Serial2.begin(9600); 
////编码器中断引脚初始化/////  
   pinMode(En_CHL, INPUT);//设置中断脚为输入状态
   pinMode(En_CHR, INPUT);// 
   // 上升沿触发 
   attachInterrupt(0, interruptL, FALLING);  // step0  input on interrupt 1 - pin 2
   attachInterrupt(1, interruptR, FALLING);  // step1  input on interrupt 1 - pin 3
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
   SetPinFrequency(PWM_right, Frequence);   //右电机控制频率初始化
   pwmWriteHR(PWM_right, 0);          //右电机控制占孔比初始化
   SetPinFrequency(PWM_left, Frequence);    //左电机控制频率初始化
   pwmWriteHR(PWM_left,0);            //左电机控制占孔比初始化
//// 两个定制器//////
  // 电机 pid 10ms 中断 外加 超声测量 编制设定 
   FlexiTimer2::set(10, 1.0/1000, flash); // call every 10 1ms "ticks" 
   FlexiTimer2::start();
  // 发送数据 定时器中断   
   Timer3.initialize( interval );
  //Timer3.attachInterrupt( send_msg_serial );
 // Timer3.attachInterrupt( send_msg_ros_serial );
 Timer3.attachInterrupt( send_msg );

/*测试代码*/
    // 设置左右轮子期望值         
   // car_pid.Left_Speedwant  = 20;        // 左轮PWM期望值  50 就很大了
   // car_pid.Right_Speedwant = 20;         // 右轮PWM期望值
    
}

// linux 测试命令/////
//   rosrun rosserial_python serial_node.py /dev/ttyUSB0
//   rostopic pub motor std_msgs/UInt32  1100100
//   rostopic echo Pluse

//// 主循环 //////
void loop()
{
   tesk(); //测量电池电压 充电电流  超声数据
}


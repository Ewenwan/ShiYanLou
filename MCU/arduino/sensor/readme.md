# 各种传感器
## 模拟量
```C
// 红外测据传感器
#include <math.h>
// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A2;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
float tmp;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:
 // Serial.print("sensor = ");
 tmp=sensorValue/1024.0*5.0;
 tmp=5.3681*pow(tmp,-1.366);//公式实际测试 有 excel 散点图 幂函数得到 单位 cm
  Serial.println(tmp);
 // Serial.print("\t output = ");
 // Serial.println(outputValue);
  delay(100);
}

```

## 串口
```C
// 读取 MPU6050 
/*
This code is used for connecting arduino to serial mpu6050 module, and test in arduino uno R3 board.
connect map:
arduino   mpu6050 module
VCC    5v/3.3v
TX     RX<-0
TX     TX->1
GND    GND
note: 
because arduino download and mpu6050 are using the same serial port, you need to un-connect 6050 module when you want to download program to arduino.
 Created 14 Nov 2013
 by Zhaowen
 
 serial mpu6050 module can be found in the link below:
 http://item.taobao.com/item.htm?id=19785706431
 */
#include <MadgwickAHRS.h>  //互补滤波算法库
Madgwick filter;          //滤波算法类对象

unsigned char Re_buf[11],counter=0;//串口接收缓冲区
unsigned char sign=0;
float a[3],w[3],angle[3],T;//三轴加速度 角速度 角度 温度

float pitch = 0;
float roll = 0;
float yaw = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);//波特率 115200 时 模块每隔 10ms 输出 1 帧数据。
  //Serial.begin(115200);
  //
  ///* 
//  delay(1000);
//  Serial.write(0xFF);
//  Serial.write(0xAA);
//  Serial.write(0x64);//设置9600波特率
//  delay(2000);
//  Serial.write(0xFF);
//  Serial.write(0xAA);
//  Serial.write(0x64);//设置9600波特率
//  
//  Serial.write(0xFF);
//  Serial.write(0xAA);
//  Serial.write(0x63);//设置115200波特率
//  //Serial.begin(115200);
  
//  Serial.write(0xFF);
//  Serial.write(0xAA);
//  Serial.write(0x65);//水平安装
  //*/
}

void loop() {
  if(sign)
  {  
   sign=0;             //清零接收完成标志
   if(Re_buf[0]==0x55) //检查帧头
   {  
  	switch(Re_buf [1])     //信息标识符
  	{
  	case 0x51://加速度标识 X Y Z轴加速度
  		a[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*16;//单位为g 9.8m/s^2
  		a[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*16;
  		a[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*16;
  		T = (short(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25;//温度
     // Re_buf [10]为校验和 
     // Re_buf [10]=Sum=0x55+0x51+AxH+AxL+AyH+AyL+AzH+AzL+TH+TL
  		break;
  	case 0x52://角速度标识  X Y Z轴角速度
  		w[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*2000;//单位 度/s
  		w[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*2000;
  		w[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*2000;
  		T = (short(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25;//温度 C摄氏度
  		break;
  	case 0x53://角度
      angle[0] = (short(Re_buf [3]<<8| Re_buf [2]))/32768.0*180;//角度
  		angle[1] = (short(Re_buf [5]<<8| Re_buf [4]))/32768.0*180;
  		angle[2] = (short(Re_buf [7]<<8| Re_buf [6]))/32768.0*180;
  		T = (short(Re_buf [9]<<8| Re_buf [8]))/340.0+36.25;
//        Serial.print("a:");//加速度
//        Serial.print(a[0]);Serial.print(" ");
//        Serial.print(a[1]);Serial.print(" ");
//        Serial.print(a[2]);Serial.print(" ");
//        Serial.print("w:");//角速度
//        Serial.print(w[0]);Serial.print(" ");
//        Serial.print(w[1]);Serial.print(" ");
//        Serial.print(w[2]);Serial.print(" ");
//        Serial.print("angle:");//角度
//        Serial.print(angle[0]);Serial.print(" ");
//        Serial.print(angle[1]);Serial.print(" ");
//        Serial.print(angle[2]);Serial.print(" ");
//        Serial.print("T:");//温度
//        Serial.println(T);
   // 调用互补滤波算法 
     filter.updateIMU(w[0], w[1], w[2], a[0], a[1], a[2]);
     yaw = filter.getYaw();
     Serial.println(yaw);
     break;
  	} 
      }
  } 
}
//串口中断
void serialEvent() {
  while (Serial.available()) {
    
    //char inChar = (char)Serial.read(); Serial.print(inChar); //Output Original Data, use this code 
  
    Re_buf[counter]=(unsigned char)Serial.read();//缓冲区
    if(counter==0&&Re_buf[0]!=0x55) return;      //第0号数据不是帧头              
    counter++;       
    if(counter==11)             //接收到11个数据
    {    
       counter=0;               //重新赋值，准备下一帧数据的接收 
       sign=1;                  //置位接收完成标识符
    }
      
  }
}
```
串口命名控制
```c
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

```
ros 收发话题格式
```c
#include <ros.h> // ros
#include <std_msgs/UInt32.h> // 话题消息类型
ros::NodeHandle   nh;        // 创建ROS句柄
std_msgs::UInt32  ENC_Pluse; // 定义编码器脉冲广播消息格式

ros::Publisher Pluse("Pluse", &ENC_Pluse); // 定义发布话题节点 发送到话题Pluse，消息为ENC_Pluse

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


void setup()
{
////ROS节点初始化///////////
// 默认串口通信 波特率为 57600，可能会通信出错，可以降低通信波特率 
// arduino-1.8.1\libraries\ros_lib\ArduinoHardware.h ---> ArduinoHardware(SERIAL_CLASS* io , long baud= 57600)
// baud_ = 57600;  修改此处的值即可，例如 9600
   nh.initNode();      //初始化节点,会影响串口0   2560 serial1  serial2 serial3 可以用
   nh.subscribe(sub);  //消息 订初 始化 
   nh.advertise(Pluse);//消息 发 布初始化
}

//// 主循环 //////
void loop()
{
 nh.spinOnce();// 需要给ros发布的权限
 // 编码器数据     
    ENC_Pluse.data=(0x00               & 0xffffffff)<<30
                   |(Left_Direction    & 0xffffffff)<<25
                   |(Right_Direction   & 0xffffffff)<<24
                   |(pub_countL        & 0xffffffff)<<12
                   |(pub_countR        & 0xffffffff);
    Pluse.publish(&ENC_Pluse);// 发布消息
 delay(1000);
}
// linux 测试命令/////
//   rosrun rosserial_python serial_node.py /dev/ttyUSB0 _baud:=57600
//   rostopic pub motor std_msgs/UInt32  1100100  // 发布话题消息
//   rostopic echo Pluse                          // 查询话题消息


```


## 脉宽
```c
#include <TimerOne.h>
/*
HC-SR04超声波测距模块可提供2cm-400cm的非接触式距离感测功能，测距精度可达高到3mm；模块包括超声波发射器、接收器与控制电路。像智能小车的测距以及转向，或是一些项目中，常常会用到。智能小车测距可以及时发现前方的障碍物，使智能小车可以及时转向，避开障碍物。

Vcc-GND 5V电源输入；Trig：触发信号输入；Echo:回响信号输出

工作原理
采用IO口TRIG触发测距，给最少10us的高电平信号。
模块自动发送8个40khz的方波，自动检测是否有信号返回。
有信号返回，通过IO口ECHO输出一个高电平，高电平持续的时间就是超声波从发射到返回的时间。
$测试距离s=\frac{高电平时间t\times 声速（v=340m/s)}{2}$

*/
//const int  trigger_io = LED_BUILTIN;
const int  trigger_io = 8;  // 触发口

//Most Arduino boards have two external interrupts: numbers 0 (on digital pin 2) and 1 (on digital pin 3).
//The Arduino Mega has an additional four: numbers 2 (pin 21), 3 (pin 20), 4 (pin 19), and 5 (pin 18).
const int  pwm_io = 2;

void setup(void)
{
  pinMode( trigger_io, OUTPUT);

  // 定时器中断
  Timer1.initialize(2000);// 2ms中断
  Timer1.attachInterrupt(sensor_trigger); // blinkLED to run every 0.15 seconds
  Serial.begin(9600);

  pinMode(pwm_io, INPUT);          // 中断输入口
  // 电平变化即触发中断
  attachInterrupt(0, DYP_plus_measure, CHANGE); // 电平改变触发, DYP_plus_measure中断触发函数
}
// 形式：uint32 mills()与uint32 micros()
//
// 用于得到从程序开始运行以来的时间，mills()函数返回以毫秒 ms 表示的时间，而micros()函数返回以微秒 us 表示的时间。
// 当计时溢出后会自动从零开始计数，mills()函数会在程序运行约50天后溢出，而micros()会在程序运行约70分钟后溢出。

unsigned long DYP_PulseStartTicks;     
volatile int DYP_val = 0 ;
volatile float DYP_distalce = 0.0;
void DYP_plus_measure(void)
{
        if (digitalRead( pwm_io ) == HIGH)// 低电平到高电平的 时间点
                DYP_PulseStartTicks = micros();    // 微秒 us  
        else// 高电平变成低电平的 时间点
                DYP_val = micros() - DYP_PulseStartTicks;
        DYP_distalce = DYP_val / 58.0;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
}

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


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
}
```
## 脉冲个数
```c
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
int countR=0, countL=0;
 ///// 右轮 编码器计数////
void interruptR()                      
{
   countR += 1;
}
////// 左轮 编码器计数////
void interruptL()
{
   countL += 1;
}
 
```
## i2c
```c
// 修改i2c 器件地址 
#include <Wire.h>
#define KS109ADDR 0x74     //老地址    // 默认地址  0xe8 右移1位 为0x74 最高位为1 表示读数据  则为 0xf4
#define NewKS109ADDR 0xea  //新地址    0xea  右移1位 为0x75 最高位为1 表示读数据  则为 0xf5
#define Reg 0x02           //寄存器地址

void setup()
{

    Wire.begin();  
    delay(100); 
    Wire.beginTransmission(KS109ADDR); 
    Wire.write(Reg);                              
    Wire.write(0x9a); 
    Wire.endTransmission();
    delay(10);  
    Wire.beginTransmission(KS109ADDR);       
    Wire.write(Reg);                              
    Wire.write(0x92); 
    Wire.endTransmission();
    delay(10); 
    Wire.beginTransmission(KS109ADDR);         
    Wire.write(Reg);                              
    Wire.write(0x9e); 
    Wire.endTransmission();
    delay(10); 
    Wire.beginTransmission(KS109ADDR);   
    Wire.write(Reg);                              
    Wire.write(NewKS109ADDR); 
    Wire.endTransmission();  
}
void loop()
{
}
```
定时器 定时测量距离
```c
// KS103  Ultrasonic Ranger Finder  for Arduino IDE 1.0
// For using KS103 through TWI(I2C) 
// Written by Yuji Sawaguchi
#include <FlexiTimer2.h> // Timer2  UN0 PWM 3 和 11用不了   2560  PWM 9和10 用不了
#include <TimerOne.h>    // Timer1  UNO PWM 9 10口 用不了   2560  PWM  11  12  13口 用不了
const long interval = 50000;// * 1000; // 50000 microseconds = 50 ms  
// Timer1.initialize( interval ); Timer1.attachInterrupt( ggyy );


#include <Wire.h>

#define KS103_left  0xe8 >> 1   // default address of KS103 is 0xe8
#define KS103_right  0xea >> 1   // default address of KS103 is 0xe8 修改为 0xea 

#define reduce_noise  0x71 // these lines define the data for commands. See data sheat.
//#define turn_off_led 0xc1
//#define turn_on_led 0xc0
#define read_distance 0xb4  // distance may be less than 5m .
//#define read_brightness 0xa0
//#define read_temperature 0xc9


word front_left=0;  // thease variables are for printing the temperture through Serial.print().
word front_right=0;

const int adc_charge_pin  = A0;    //充电电流ADC检测口
const int adc_battery_pin = A1;    //电池电压ADC检测口
int adc_charge_value = 0;       
int adc_battery_value = 0;


unsigned char timeCounts =0;
bool left_10ms = false;
bool left_20ms = false;
bool right_110ms = false;
bool right_120ms = false;
bool adc_det     = false;
const int led_pin = 13;  


void flash()
{
timeCounts++;
if(timeCounts==1) left_10ms = true;
if(timeCounts==2) left_20ms = true;
if(timeCounts==11) right_110ms = true;

if(timeCounts==20) { 
  static boolean output = HIGH;
  digitalWrite(led_pin, output);
  output = !output;
 //timeCounts=0;
 }
 if(timeCounts==12) right_120ms = true;
 
 if(timeCounts>=20) {
  timeCounts=0; 
  adc_det = true;}
 
}

void setup()
{
  Wire.begin();                    // join the TWI as the master
  Serial.begin(9600);             // start Serial transaction
  pinMode(13, OUTPUT);  
  KS103_command(KS103_left,reduce_noise);    // send the noise reduction command to KS103
  delay(5);  
  KS103_command(KS103_right,reduce_noise);   // send the noise reduction command to KS103
  delay(3000);                               // wait for being stabilized 2seconds
  
  FlexiTimer2::set(10, 1.0/1000, flash); // call every 10 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
  Timer1.initialize( interval );
  Timer1.attachInterrupt( send_msg ); 
 //Serial.begin(9600);             // start Serial transaction
  
}

void loop()
{
  task();
  /*
  while(send_msg_50ms){
 //if(send_msg_50ms){
 //Serial.println(send_msg_50ms);
  task();
  send_msg_50ms=false;
    }
    */
}

void send_msg(){
    
  Serial.print("L: ");
  Serial.print("\t");
  Serial.print(front_left/10, DEC);
  Serial.print("\t");
  Serial.print("R: ");
  Serial.print("\t");
  Serial.print(front_right/10, DEC);
  Serial.print("\t");
  Serial.print("C: ");
  Serial.print("\t");
  Serial.print(adc_charge_value, DEC);
  Serial.print("\t");
  Serial.print("B: ");
  Serial.print("\t");
  Serial.println(adc_battery_value, DEC);
  }

void task()
{  
  KS103_read_command_left(read_distance);
  //delay(75);
  KS103_read_command_right(read_distance);
  if(front_left>5110) front_left=5110;
  if(front_right>5110) front_right=5110; 
   /*
  Serial.print("L: ");
  Serial.print("\t");
  Serial.print(front_left/10, DEC);
  Serial.print("\t");
  Serial.print("R: ");
  Serial.print("\t");
  Serial.println(front_right/10, DEC);
 */
   if(adc_det)
   adc_charge_value = analogRead(adc_charge_pin);
   adc_battery_value = analogRead(adc_battery_pin);
   adc_det = false;
  }
  
void KS103_read_command_left(byte command){  // sending the command and read the data in the register
 // word temp_reading;
  // step 1: instruct sensor to read echoes
  if(left_10ms){
      left_10ms = false; 
      KS103_command(KS103_left,command);   // send the command 
  }
  // step 2: wait for readings to happen
 if(left_20ms){
  left_20ms = false;
  //delay(5);                   // wait 1 milliseconds . Without this waiting, the system will be unstable. 
  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(KS103_left); // start to transmit with KS103
  Wire.write(byte(0x02));      // register 2 is the gate of receiving the data
  Wire.endTransmission();      // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(KS103_left, 2);    // request the data in the 2nd and third register of KS103
  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // wait the register available
  {
    front_left = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
    front_left |= Wire.read(); // read the register 3 to lower byte
  }
  //return temp_reading;  // return as a 16bit data 
 // left_20ms = 0;
 }
}


void KS103_read_command_right(byte command){  // sending the command and read the data in the register
 // word temp_reading;
  // step 1: instruct sensor to read echoes
  if(right_110ms){
     right_110ms=false;
     KS103_command(KS103_right,command);   // send the command
  }
  // step 2: wait for readings to happen
  if(right_120ms){
   right_120ms = false;
  //delay(5);                   // wait 1 milliseconds . Without this waiting, the system will be unstable. 
  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(KS103_right); // start to transmit with KS103
  Wire.write(byte(0x02));      // register 2 is the gate of receiving the data
  Wire.endTransmission();      // stop transmitting
  // step 4: request reading from sensor
  Wire.requestFrom(KS103_right, 2);    // request the data in the 2nd and third register of KS103
  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // wait the register available
  {
    front_right = (Wire.read()) << 8;  // read register 2 and shift it to upper byte.
    front_right |= Wire.read(); // read the register 3 to lower byte
  }
  //return temp_reading;  // return as a 16bit data 
 }
}

void KS103_command(char addr ,byte command){  // send the command to KS103
  Wire.beginTransmission(addr); // start the transmission with KS103
  Wire.write(byte(0x02));      // register 2
  Wire.write(command);      // send the command to the register 2
  Wire.endTransmission();      // end of transmission
}
```

## 舵机
```C
#include <Servo.h>

Servo myservo0,myservo1, myservo2, myservo3, myservo4;  //舵机对象 类

int pos;         // 0~180
#define servo_pin0 2

void setup() {
//  myservo0.attach(servo_pin0);  // attaches the servo on pin 9 to the servo object
//  myservo1.attach(3);
//  myservo2.attach(4);
//  myservo3.attach(5);
  myservo4.attach(9);
  
//  //myservo0.writeMicroseconds(500);//0度
//  //myservo0.writeMicroseconds(900);//
//  myservo0.writeMicroseconds(1500);//中间值 90度
//  //myservo0.writeMicroseconds(2500);//180度
//  //delay(1000);
//  //myservo1.writeMicroseconds(1500);//中间值 90度
//  //myservo1.writeMicroseconds(1000);//45度
//  //myservo1.writeMicroseconds(900);//36度
//  myservo1.writeMicroseconds(2000);//135度
//  myservo2.writeMicroseconds(2500);
//  myservo3.writeMicroseconds(1500);
  myservo4.writeMicroseconds(1500);//90度
 //myservo0.write(0);
 // myservo0.write(180);
 //delay(1000);    
 //myservo0.writeMicroseconds(550);//一般舵机有1000到2000，或者700到2300的范围来转动，实际测试一下就行了，1500通常是中间值 2389 >>> 180
 //delay(1000); 
 // myservo0.write(0);
 //1ms 控制范围  1000~2000us 阿里风斯 ASMC-04B 1ms 2ms 控制范围 可调  0-->300 为逆时针转动  角度范围可调 最大0~300度  到最小0~150度
 //myservo0.writeMicroseconds(1000);//开始值0度
 //myservo0.writeMicroseconds(1500);//中间值150度(最大300度时)  初始化时如果不写 默认会到中间值
 //myservo0.writeMicroseconds(2000);//最大值300度(最大300度时)可调
 
// 阿里风斯 ASMC-04B跳线帽已换 为2ms控制范围
//2ms 控制范围  500~2500us  伟创的为2ms控制范围(不可调) 0-->180 为逆时针转动 角度范围0~180度(不可调)
 //myservo0.writeMicroseconds(500);//开始值0度
 //myservo0.writeMicroseconds(1500);//中间值150度(最大300度时)  初始化时如果不写 默认会到中间值
 //myservo0.writeMicroseconds(2500);//最大值300度(最大300度时)可调  
}

void loop() {
//  for(pos = 10; pos <= 170; pos += 1)  
//  {                                   
//    myservo0.write(map(pos,10,170,65,110)); 
//    delay(20);                     
//  } 
//  for(pos = 170; pos>=10; pos -= 1)    
//  {                                
//    myservo0.write(map(pos,10,170,65,110)); 
//    delay(20);                       // waits 15ms for the servo to reach the position 
//  }
  
}
```
 **定时器控制舵机正反转**
 ```c
 
 ///*
// 舵机的实现 定时器1
#include <Servo.h>

#include <FlexiTimer2.h>// 系统任务定时器 make2560可用 
//#include <MsTimer2.h>  // uno  Timer2  8bit PWM 3 和 11 用不了

Servo myservo;  // 创建 servo 对象来控制一个 servo
const int pin = 9;//舵机的控制信号引脚   timer1  PWM 9 10口
const int ang_pin = A0;//将舵机的角度输出信号连接至A0

bool zf_flag = 0;//符号标志选择正反转 0 正 1500 ->1900  1 反
int r_zl = 2;    //每次增量
int min_r = 600; //最小位置（500，若设置为1500，水平位置）//600，最小适合值
int r_current = min_r;//当前位置
int max_r =2400;  //最大角度位置//2400最大合适值
float angle;      // 舵机反馈的角度值 对应角度值
int   angle_int;
int   angle_int_100ms;//100ms时对应的角度

bool direction_change_flag = 0; // 舵机转向变化flag 向任意一个方向转动到转不动了，就需要改变方向。

// 20ms  舵机控制 任务=========================
void servo_control(){
   if(r_current <=  min_r)   zf_flag = 0;//当前位置小于等于最小位置，标志位置0
  else if(r_current >=  max_r) zf_flag = 1;//当前位置大于等于最大位置，标志位置1
  if(zf_flag) r_current -=  r_zl;//正转
  else r_current +=  r_zl;//反转
  myservo.writeMicroseconds(r_current);//向9脚写入当前状态
  angle = (analogRead(ang_pin)*(-2.132) + 2592.7)/100*15;
  angle_int = angle;
}
//*/
int servo_count=0;//子计时器
int servo_change_dir_dect_time_count=0;
void task(){
  servo_count++;
  if(servo_count >= 10)
  {
    servo_control();
    servo_count = 0;
  }
  
// 舵机调换方向判断=========转不动了改变方向==============
  servo_change_dir_dect_time_count++;
  if(servo_change_dir_dect_time_count == 50) angle_int_100ms = angle_int; // 100ms时的角度值
  if(servo_change_dir_dect_time_count >= 500){
    int temp = abs(angle_int - angle_int_100ms);// 两次 角度差值绝对值
    Serial.println(temp);// 打印看看正常情况下 会有多少的 角度差 
    //  11为阈值， 或者这里 使用 temp的平均值来作为阈值（自适应）
    if(temp>11);// 可能需要多次都小于一个值才能够判断出 舵机堵转  500ms正常差值 6~7   1s差值 11~20
    else direction_change_flag =1;// 舵机转不动了， 变换转动方向
    servo_change_dir_dect_time_count = 0;
    }
}
void setup() {
   Serial.begin(9600);//串口初始化
  // put your setup code here, to run once:
   myservo.attach(pin);
   FlexiTimer2::set(2,1.0/1000,task );//2ms执行一次定时器中断服务程序 2560
   FlexiTimer2::start();

 // MsTimer2::set(20, task); // 20ms period
 // MsTimer2::start();

   
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // 舵机切换方向============================
if(direction_change_flag) {zf_flag = !zf_flag; direction_change_flag=0;}
    
}

 ```
 **定时器控制舵机正反转 + 堵转保护 + 位置记忆**
 ```c
 ///*
// 舵机的实现 定时器1
#include <Servo.h>

#include <FlexiTimer2.h>// 系统任务定时器 make2560可用 
//#include <MsTimer2.h>  // uno  Timer2  8bit PWM 3 和 11 用不了

#include <avr/eeprom.h>


int Address = 0;
/* Write to EEPROM */
#define EP_WRITE_BLOCK(address, block) eeprom_write_block((const void*)&block, (void*)address, sizeof(block))
#define EP_WRITE_BYTE(address, b) eeprom_write_byte(&address, b)

/* Read from EEPROM */
#define EP_READ_BLOCK(address, block) eeprom_read_block((void*)&(block), (void*)address, sizeof(block))
#define EP_READ_BYTE(address) eeprom_read_byte(address)


Servo myservo;  // 创建 servo 对象来控制一个 servo
const int pin = 9;//舵机的控制信号引脚   timer1  PWM 9 10口
const int ang_pin = A0;//将舵机的角度输出信号连接至A0

bool zf_flag = 0;//符号标志选择正反转 0 正 1500 ->1900  1 反
int r_zl = 2;    //每次增量
int min_r = 600; //最小位置（500，若设置为1500，水平位置）//600，最小适合值
int r_current = min_r;//当前位置
int max_r =2400;  //最大角度位置//2400最大合适值
float angle;      // 舵机反馈的角度值 对应角度值
int   angle_int;
int   angle_int_100ms;//100ms时对应的角度

bool direction_change_flag = 0; // 舵机转向变化flag 向任意一个方向转动到转不动了，就需要改变方向。

int servo_loc_fliter=7;
int servo_loc_filter_threshold=7;



// 20ms  舵机控制 任务=========================
void servo_control(){
   if(r_current <=  min_r)   zf_flag = 0;//当前位置小于等于最小位置，标志位置0
  else if(r_current >=  max_r) zf_flag = 1;//当前位置大于等于最大位置，标志位置1
  if(zf_flag) r_current -=  r_zl;//正转
  else r_current +=  r_zl;//反转
  myservo.writeMicroseconds(r_current);//向9脚写入当前状态
  EP_WRITE_BLOCK(Address,r_current);//保存位置
  angle = (analogRead(ang_pin)*(-2.132) + 2592.7)/100*15;
  angle_int = angle;
}
//*/
int servo_count=0;//子计时器
int servo_change_dir_dect_time_count=0;
void task(){
  servo_count++;
  if(servo_count >= 10)
  {
    servo_control();
    servo_count = 0;
  }
  
// 舵机调换方向判断=========转不动了改变方向==============
  servo_change_dir_dect_time_count++;
  if(servo_change_dir_dect_time_count == 50) angle_int_100ms = angle_int; // 100ms时的角度值
  if(servo_change_dir_dect_time_count >= 500){
    int temp = abs(angle_int - angle_int_100ms);// 两次 角度差值绝对值
    Serial.print(temp);// 打印看看正常情况下 会有多少的 角度差 
    //  11为阈值， 或者这里 使用 temp的平均值来作为阈值（自适应）
    servo_loc_fliter = (servo_loc_fliter*2 + temp*3)/5;
    Serial.print('\t');   
    Serial.print(servo_loc_fliter);
    
    servo_loc_filter_threshold = (servo_loc_filter_threshold*5 + temp*5)/10;
    Serial.print('\t');   
    Serial.println(servo_loc_filter_threshold);
    
    if(temp>11);// 可能需要多次都小于一个值才能够判断出 舵机堵转  500ms正常差值 6~7   1s差值 11~20
    else direction_change_flag =1;// 舵机转不动了， 变换转动方向
    servo_change_dir_dect_time_count = 0;
    }
}
void setup() {
   Serial.begin(9600);//串口初始化
  // put your setup code here, to run once:
   myservo.attach(pin);

   int r_current_init; 
   EP_READ_BLOCK(Address, r_current_init);

   if(r_current_init>2500) r_current = 2500;
   else if(r_current_init<500) r_current = 500;  
   else r_current = r_current_init;  
   
   FlexiTimer2::set(2,1.0/1000,task );//2ms执行一次定时器中断服务程序 2560
   FlexiTimer2::start();

 // MsTimer2::set(20, task); // 20ms period
 // MsTimer2::start();

}  


void loop() {
  // put your main code here, to run repeatedly:
  
  // 舵机切换方向============================
//if(direction_change_flag) {zf_flag = !zf_flag; direction_change_flag=0;}
    
}
 
 ```

# 舵机内部控制程序   位置式控制
```c
    /* by Payne.Pan 2017.2 */
    /* only for study      */
    #define A   5
    #define B   6
    #define Delta 5 // 脉冲信号响应 灵敏度====

    long prev_time=0;
    long pwm=0;


    void rising() // 2号口  pwm信号 脉冲上升沿
    {
      prev_time = micros();// 上升沿时刻的时间
      attachInterrupt(0, falling, FALLING);// 切换为下降沿，检测脉冲信号的 下降沿
    }


    void falling() { // 检测脉冲信号的 下降沿
      pwm = micros()-prev_time; // 下降沿时间 - 上升沿时间 ====> 脉冲信号 持续时间
      attachInterrupt(0,rising, RISING);// 再次切换为 上降沿中断， 检测下一个脉冲信号的  上升沿
    }

// 控制电机 正转 反转 停止=====
    void runMotor(int dir) 
    {
      if (dir == 0) // 点击停止
      {
        digitalWrite(A,LOW);
        digitalWrite(B,LOW);
      }
      else if (dir > 0) // 电机正转
      {
        digitalWrite(A,HIGH);
        digitalWrite(B,LOW);
      } 
      else  // 电机反转
      {
        digitalWrite(A,LOW);
        digitalWrite(B,HIGH);
      }
    }

// 初始化 
    void setup() {
      pinMode(2,INPUT);   // 外接 pwm 输入信号，用来接收 转动的角度值====500～2500模式===== 
      
      pinMode(A,OUTPUT);  // 电机控制板 正反转
      pinMode(B,OUTPUT);
      
      pinMode(A0,INPUT); # 舵机 电位器反馈 模拟量测量接口
      
      attachInterrupt(0,rising, RISING); // 2号管脚对应 0号 可以检测硬件中断的端口，上升边中断 外接 PWM信号
    }

    void loop() {
      int a = analogRead(A0);     // 读取 舵机主轴连接的电位器 反馈值
      a = map(a,0,1023,500,2500); // 将0～1023的 模拟量值 映射到 500～2500范围，对应 pwm控制信号的 脉冲宽度范围
      if(pwm)>2500 pwm =2500;
      else if(pwm<500) pwm =500; // 确保 pwm 范围，保护====
      if (pwm-a > Delta)
      {
        runMotor(1);// 正转
      } 
      else if (pwm-a < -Delta)
      {
        runMotor(-1);// 反转
      } 
      else 
      {
        runMotor(0);// 停止
      }
    }


```



# 舵机内部控制程序  PID控制

```c



#if (ARDUINO >= 100)
 #include <Arduino.h>// mega 2560  
#else
 #include <WProgram.h>//
#endif

#include "pid.h"// pid

#include <FlexiTimer2.h>//  
//#include <TimerOne.h>

#define PWM_PIN 9 // PWM 管脚
#define DIR_PIN 8 // 方向 管脚
#define ANGEL_PIN A0 // 模拟量反馈 管脚

int angel_last=0;// 上次 反馈值
int angel_diff_feedback=0;// 两次反馈差值
int angel_diff_feedback_last=0;
int led_count=0;
int pid_count=0;

pidControl pidCon; // pid 控制参数结构体
PpidControl PpidCon = NULL;// 指针

// 0~360 映射到 0~1023  这里是 电位器反馈值====
int angel_down = 400;// 转动范围====
int angel_up = 950;
int jitter_threshold = 15;// 两头范围 ====模拟量值
bool enable_change =true; // 使能换向
// bool half_speed =true;    // 换向前半速，减少磨损===

#define SPEED_VALUE_GOOD 25 //  模拟量 反馈差值  25
#define PWM_VALUE_GOOD 125  //  pwm 差值   125

  // 125 pwm 值  20ms  差值范围 3~6
  //             40ms  差值范围 8~11
  //             60ms  差值范围 13~17
  //             80ms  差值范围 18~22
  //             100ms 差值范围 23~27
  // 也就是说 正常情况下 差值有波动，pwm应该不考虑小范围波动

/*
// 设置定时器频率==========2560===================
void setPwmFrequency2560(int pin, int divisor) 
{
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
*/

// 设置定时器频率==========UNO===================
void setPwmFrequencyUNO(int pin, int divisor) 
{
  byte mode;
  if((pin >= 3 && pin <= 11) ) 
  {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
      if(pin == 5 || pin == 6)//Timer0
      {
        TCCR0B = TCCR0B & 0b11111000 | mode;  
      }
      else if(pin == 9 || pin == 10 )//Timer1
      {
        TCCR1B = TCCR1B & 0b11111000 | mode;  
      }
      else if(pin == 3 || pin == 11)//Timer2
      {
        TCCR2B = TCCR2B & 0b11111000 | mode;  
      }
  }
}


bool led_on_off=false;
void task()
{
  led_count++;
  pid_count++;
  if(led_count>=500)
  {
  led_count=0; 
  led_on_off = !led_on_off;
  digitalWrite(LED_BUILTIN, led_on_off);
  }
  // 125 pwm 值  20ms  差值范围 3~6
  //             40ms  差值范围 8~11
  //             60ms  差值范围 13~17
  //             80ms  差值范围 18~22
  //             100ms 差值范围 23~27
  // 也就是说 正常情况下 差值有波动，pwm应该不考虑小范围波动=======
  
  if(pid_count==50) // *2ms  100ms周期================
  { 
   pid_count = 0; 
   int angel_read = analogRead(ANGEL_PIN);
   angel_diff_feedback = abs( angel_read - angel_last);
   angel_last = angel_read;
   Serial.print(angel_diff_feedback); // 差值
   Serial.print('\t'); //  
   Serial.println(angel_read);       // 反馈值
   //  20 到 0 到 1023 到 1000 区间 ， 差值不稳，剔除====
   // if(angel_read>20 && angel_read<1000)
   if( angel_read > (angel_down) && angel_read < (angel_up))// 在指定范围内进行pid控制
    {
     // /* // 开启pid控制
     if(abs(angel_diff_feedback -angel_diff_feedback_last) < 40)//避免过大波动======
     {
         POSITION_PID( PpidCon,angel_diff_feedback);
         SetPwm(PpidCon);
         Serial.print("piderr0"); //  
         Serial.print('\t'); //  
         Serial.print(PpidCon->s16error[0]); //  
     }
     angel_diff_feedback_last = angel_diff_feedback;
     // */
    }
    // 缓速==修改期望值===降低 换向时 齿轮损耗===
    ///*
    if((angel_read < angel_down + jitter_threshold*5 || angel_read > angel_up - jitter_threshold*5))// 80
       PpidCon->s16speedwant  = SPEED_VALUE_GOOD/6;// 期望速度变为原来的 1/6
    else 
       PpidCon->s16speedwant  = SPEED_VALUE_GOOD;// 还原
     //*/ 
     
     // 直接修改pwm输出值  切换时 震荡很明显====
   /*
  if((angel_read < angel_down+jitter_threshold*2 || angel_read > angel_up-jitter_threshold*2))
      {
       PpidCon->u16PWM = PWM_VALUE_GOOD/2;// 半速======
       SetPwm(PpidCon);
      }
   */
   
    // 换向检测===========================================
    if((angel_read<angel_down || angel_read>angel_up)&& enable_change )
    {  
       enable_change =false;
       PpidCon->bDrection = !PpidCon->bDrection;// 换向
       SetDir(PpidCon);
    }
    if(angel_down<angel_read && angel_read<angel_up) enable_change =true;// 使能换向检测
    
   Serial.print('\t'); //  
   Serial.print(PpidCon->u16PWM);          // 输出值 
   Serial.print('\t'); //  
   Serial.println(PpidCon->s16speedwant);  // 期望值  
  }
}

void setup() {
  
//  pidControl pidCon;
//  PpidControl PpidCon = &pidCon;// 指针

  PpidCon = &pidCon;// 指针
  
  PpidCon->direction_pin = DIR_PIN;// 方向口
  PpidCon->pwm_pin       = PWM_PIN;// pwm口
  PpidCon->bDrection     = true;// 正转
  PpidCon->s16speedwant  = SPEED_VALUE_GOOD;//   100ms 期望差值===25===
  
  PpidCon->s16speed_p=2.5; // pwm 目标值 125附近，反馈值 100ms差值 23~27左右
  PpidCon->s16speed_i=0.7;
  PpidCon->s16speed_d=0;
  
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  FlexiTimer2::set(2,1.0/1000,task );//2ms执行一次定时器中断服务程序
  FlexiTimer2::start();
//Timer1.initialize(150000);// 150ms
//Timer1.attachInterrupt(task); // blinkLED to run every 0.15 seconds
   Serial.begin(9600);// 

   pinMode(DIR_PIN, OUTPUT);     // 左轮方向口
   digitalWrite(DIR_PIN, HIGH);   // LOW 后退   HIGH向前  
   

   setPwmFrequencyUNO(PWM_PIN,1);//9
   pinMode(PWM_PIN, OUTPUT);   
     
   analogWrite(PWM_PIN,0);  // 半电压== PWM_VALUE_GOOD=125

  angel_last=analogRead(ANGEL_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:

}


```


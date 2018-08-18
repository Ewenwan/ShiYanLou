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

## 脉宽
```c
#include <TimerOne.h>


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

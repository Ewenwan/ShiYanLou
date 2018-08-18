# 各种传感器
## 模拟量
## 串口
```C

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
        DYP_distalce = DYP_val / 58;// 换算得到 距离  这里需要查询手册，一般是 声速*时间/2 =  340*秒时间/2
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

## 

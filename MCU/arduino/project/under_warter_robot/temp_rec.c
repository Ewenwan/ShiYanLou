/*
 DIY Arduino based RC Transmitter Project
              == Receiver Code ==
  by Dejan Nedelkovski, www.HowToMechatronics.com
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

  1.接收无线发送端发送的数据
  2.控制左右推进器电机转动
  3.控制抽水电机 抽水/排水
  4.控制灯光亮灭
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>
#include <avr/wdt.h>              //看门狗中断系统重启 


RF24 radio(7, 8);                 //无线接收模块 控制端口 nRF24L01(CE, CSN)

#define left_moto_pin 10          //左电机控制引脚
#define right_moto_pin 9          //右电机控制引脚    
#define bucket_pin 6              //排水桶电机控制引脚
#define led_pin 4                 //灯光控制引脚

bool chip_connect=false;          //nrf无线模块是否和单片机连接正确标志
bool rec_flg=false;               //无线收到信号标志
Servo left_moto;                  //左电机
Servo right_moto;                 //右电机
Servo bucket;                     //排水桶电机

int left_moto_threat=0;           //左电机油门  1500-2000后退 1500-1000前进
int right_moto_threat=0;          //右电机油门 
int left_moto_threat_last=0;      //左电机      上次的值,避免前后两次差异过大
int right_moto_threat_last=0;     //右电机

int bucket_threat=0;              //排水电机油门 90停止 0排水上浮 180抽水下潜
int last_bucket_threat=90;        //上次的值

unsigned long last_down_time = 0; //上次下潜时间
unsigned long last_up_time = 0;   //上次上浮时间
bool down_flag = false;           //下潜标志
bool up_flag = false;             //上浮标志

int cur_up_down=124;              //接收到的抽水桶电机的控制信号
int last_up_down=124;
int forward_back=129;             //前进后退控制信号 imu 138
int left_right=122;               //左右控制信号     imu 128

bool last_last_led_switch=true;   //灯控制信号
bool last_led_switch=true;        //上次灯控制信号

bool on_flag=false;               //灯状态
bool bucket_stop_flag=false;      //抽水桶电机状态

const byte address[6] = "00001";  //无线模块地址
unsigned long lastReceiveTime = 0;//上次收到无线信号的时间
unsigned long currentTime = 0;    //当前系统时间

unsigned long last_contral_time = 0;     //上次执行控制的时间
unsigned long last_check_radio_time = 0; //上次检查无线连接状态的时间
unsigned long last_alive_time = 0;        //上次打印运行信号的时间

// 无线数据包结构体 32 bytes
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte button1;
  byte button2;
  byte button3;
  byte button4;
};
//定义一个结构体
Data_Package data;

// 重置数据
void resetData() 
{
    data.j1PotX   = 127;
    data.j1PotY   = 127;
    data.j2PotX   = 127;
    data.j2PotY   = 127;
    data.j1Button = 1;
    data.j2Button = 1;
    data.pot1     = 1;
    data.pot2     = 1;
    data.tSwitch1 = 1;
    data.tSwitch2 = 1;
    data.button1  = 1;
    data.button2  = 1;
    data.button3  = 1;
    data.button4  = 1;
}

//初始化无线接收器
void init_radio()
{
    radio.begin();
    radio.openReadingPipe(0, address);
    //radio.printDetails();
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening();             //设置为receiver
    resetData();
}

//初始化电调电机
void init_moto()
{
    left_moto.writeMicroseconds(1000); 
    right_moto.writeMicroseconds(1000);
    delay(1000);
    left_moto.writeMicroseconds(1500); 
    right_moto.writeMicroseconds(1500);
    delay(1000);
}

//控制灯光
void contol_led()
{
    if((!data.tSwitch2)&&(!last_led_switch)&&(!last_last_led_switch)&&(!on_flag))
    {
        on_flag=true;
        digitalWrite(led_pin, HIGH);
    }
    else if (last_last_led_switch && last_led_switch && data.tSwitch2 && on_flag)
    {
        digitalWrite(led_pin, LOW);
        on_flag=false;
    }
    last_last_led_switch = last_led_switch;
    last_led_switch = data.tSwitch2;
}

//控制前后运动
void control_moto_forward_back()
{
    forward_back = data.j1PotY;
    if(forward_back > 134)
    {
        left_moto_threat=map(forward_back,135,255,1500,1000);
        right_moto_threat=map(forward_back,135,255,1500,1000);
    }
    else if(forward_back < 126)
    {
        left_moto_threat=map(forward_back,125,0,1500,2000);
        right_moto_threat=map(forward_back,125,0,1500,2000);
    }
    else
    {
        left_moto_threat=1500;
        right_moto_threat=1500; 
    }
}

//控制左右运动
void control_moto_left_righ()
{
    left_right=data.j1PotX;
    if(left_right > 134)
    {
        left_moto_threat  += map(left_right,135,255,0,400);
        right_moto_threat -= map(left_right,135,255,0,400);
    }
    else if(left_right < 116)
    {
        left_moto_threat  -= map(left_right,115,0,0,400);
        right_moto_threat += map(left_right,115,0,0,400);
    }
}

//抑制电机油门前后两次变化幅度
void restrain_moto_change_range()
{
    if (right_moto_threat - right_moto_threat_last > 80)
    {
        right_moto_threat = right_moto_threat_last + 80;
    }
    else if(right_moto_threat - right_moto_threat_last < -80)
    {
        right_moto_threat = right_moto_threat_last-80;
    }
    
    if (left_moto_threat - left_moto_threat_last > 80)
    {
        left_moto_threat = left_moto_threat_last+80;
    }
    else if(left_moto_threat - left_moto_threat_last < -80)
    {
        left_moto_threat = left_moto_threat_last-80;
    }
}

//电机油门阈值处理
void moto_threshold()
{
    left_moto_threat=min(left_moto_threat,2000);
    left_moto_threat=max(left_moto_threat,1000);
    right_moto_threat=min(right_moto_threat,2000);
    right_moto_threat=max(right_moto_threat,1000);
}

// 控制抽水电机 上浮/下潜 
void control_bucket_up_down()
{
    cur_up_down=data.j2PotY;
    if((last_up_down > 129) && (cur_up_down > 129))
    {
        bucket_threat=0; 
        last_up_time = millis();
        up_flag = true; 
        down_flag = false; 
        bucket_stop_flag=false;
    }
    else if((last_up_down < 121) && (cur_up_down < 121))
    {
        bucket_threat=180;
        last_down_time = millis();
        down_flag = true; 
        up_flag = false; 
        bucket_stop_flag=false;
    }

    if(bucket_threat != last_bucket_threat)
    {
       bucket.write(bucket_threat);
    }
    last_bucket_threat = bucket_threat; 
    last_up_down = cur_up_down;
}

void control_bucket_stop()
{
    if(up_flag)
    {
        if(((millis()-last_up_time) > 200)&&(!bucket_stop_flag))
        {
            bucket_threat=90; // 停止
            bucket.write(bucket_threat); 
            up_flag=false;
            bucket_stop_flag=true;
        }
    }
    else if(down_flag)
    {
        if(((millis() - last_down_time) > 200)&&(!bucket_stop_flag))
        {
            bucket_threat=90; // 停止
            bucket.write(bucket_threat); 
            down_flag=false;
            bucket_stop_flag=true;
        }
    }
}

// 1秒钟检查一次无线连接是否正常
void  check_nrf_connected()
{
    if((millis()-last_check_radio_time) > 1000)
    {
        chip_connect=radio.isChipConnected();
        if(!chip_connect)
        {
            //Serial.println("lost wifi");
            init_radio();// 重置天线   好像没有用 因为已经失去连接了，可能需要重置无线的电源
        }
        last_check_radio_time = millis();
    }
}

// 1秒钟打印一次系统正在运行
void  print_system_running()
{
    if((millis()-last_alive_time) > 1000)
    {
        Serial.println("system_running ...");
        last_alive_time = millis();
    }
}

// 系统初始化函数
void setup() {

    Serial.begin(9600);                 //串口通信频率
    Serial.print("nfr initialize...");
    init_radio();                       //初始化无线接收器
    
    bucket.attach(bucket_pin);          // 唧筒
    left_moto.attach(left_moto_pin);    //左电机控制舵机
    right_moto.attach(right_moto_pin);  //右电机控制舵机

    //灯光
    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, LOW);
  
    chip_connect=radio.isChipConnected();
    if(chip_connect)
    {
        Serial.println("nrf2401 is Connected");
    }
    else
    { 
        Serial.println("nrf2401 is not Connected");
    }

    init_moto();                        // 电调初始化
 
    wdt_enable(WDTO_2S);                //开启看门狗，并设置溢出时间为两秒
}

// 系统循环函数
void loop() {

    // 当接收间隔时间超过一定时间后重置数据
    currentTime = millis();
    if ( currentTime - lastReceiveTime > 1000 ) //超过1秒未收到无线数据，认为失去连接
    { 
        resetData();
        rec_flg = false;
    }
    // 检查无线模块是否接收到数据
    if (chip_connect && radio.available()) 
    {
        radio.read(&data, sizeof(Data_Package));//读取收到的数据
        lastReceiveTime = millis();             //本次接收到数据的时间
        rec_flg=true;
    }

    if(rec_flg)
    {
      // 定时50ms控制
      if(millis()-last_contral_time>50)
      {
        last_contral_time = millis();
        // 灯光控制
        contol_led();
        // 前后控制
        control_moto_forward_back();
        // 左右控制
        control_moto_left_righ();
        // 前后两次差异过大抑制
        restrain_moto_change_range();
        // 阈值处理
        moto_threshold();
        
        // 控制电机 
        left_moto.writeMicroseconds(left_moto_threat);   //
        right_moto.writeMicroseconds(right_moto_threat);  // 
        left_moto_threat_last = left_moto_threat;
        right_moto_threat_last= right_moto_threat;
        
        // 控制抽水电机 上浮/下潜 
        control_bucket_up_down();
      }
    }
    else
    {
        // 失去连接后 会先重置数据，之后会关闭电机
        //left_moto.writeMicroseconds(1500);   // 关闭
        //right_moto.writeMicroseconds(1500);  // 
    }
    
    // 控制抽水桶电机停止
    control_bucket_stop();
    
    // 1秒钟检查一次无线连接是否正常
    check_nrf_connected();
    
    // 1秒钟打印一次系统正在运行
    print_system_running();
    
    // 看门狗中断系统重启(单片机跑飞处理)
    wdt_reset(); //喂狗操作，使看门狗定时器复位
}


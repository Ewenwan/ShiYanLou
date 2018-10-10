#include <GPS_7M.h>                   //GPS库文件
#include <Wire.h>                     //包含I2C库文件
#include <Servo.h>                   //pwm库文件
//////////////显示器相关
#include <SS1306_Oled.h>              //显示器IIC头文件
#define mySCL 12                      //显示器时钟线
#define mySDA 13                      //显示器数据线
IIC myOLED(mySDA, mySCL);
bool dispay = false;     //  显示刷新标志   假
bool clear_flag = false; //  清屏标志   假

GPS myGPS(Serial2);                  //gps串口定义

////////////////
int bet_voltage_pin =  A0;            //电压测量管脚
int bet_current_pin =A1;              //电流测量管脚
int vol_integer = 11;                 //电压整数位
int vol_decimal = 2;                  //电压小数位
int cur_integer = 1;                  //电流整数位
int cur_decimal = 1;                  //电流小数位


uint16_t reading_cm, reading_cm_last; //深度数据
uint16_t tempread;                    //深度数据计算中间变量

int buff[4];                          //I2C数据缓冲区
bool Flag_Arm = false;                //   解锁标志   假
bool Flag_Mode = false;               //  模式标志   假
//
bool dis_5s = false;                 //  模式标志   假

//unsigned long timelast = 0;
//定时器计时
   static unsigned int aaa = 0;
   static unsigned int bbb = 0;
   static unsigned int ccc = 0;
Servo Channel3;                      //pwm电机通道
Servo Channel4;
Servo Channel5;
int Throat = 50;                      //油门
int dir=90;                           //方向
int mode=70;                          //模式
const int time_5s = 2500;             //2500*2=5000ms
//显示刷新
const int time_disp = 250;            //250*2=500ms
const int time_10s = 5000;            //150*2=300ms
int Key_Mode = 0;                     //interrupt0 p2口  模式接口
int Key_Arm = 1;                      //interrupt1 p3口  解锁接口
int Key_Up = 4;                       //interrupt4 p19口 油门增加
int Key_Dowm = 5;                     //interrupt5 p18口 油门减少
//int Key_Low = 5;
 void vol_cur_acqu()
   {
   long int vale1=0,vale2=0,cur=0,val=0;
    vale1 = analogRead(bet_voltage_pin); 
    vale2 = analogRead(bet_current_pin);
     cur=500*vale1/335+1; 
     val=110*vale2/245;
   // Serial.println(cur); 
   // Serial.println(val); 
    vol_integer = val/10;
    vol_decimal = val%10;
    cur_integer = cur/10;
    cur_decimal = cur%10;
    }
void manu_st()
{
  //列(0～),行(0～),字符串(英文),字体大小
  myOLED.OLED_ShowString(0,1,"SW:",12);
  myOLED.OLED_ShowString(65,1,"MO:",12);
  myOLED.OLED_ShowString(0,2,"TH:",12);
  myOLED.OLED_ShowString(65,2,"HI:",12);
  myOLED.OLED_ShowString(0,3,"VA:",12);
  myOLED.OLED_ShowString(65,3,"CU:",12);
  myOLED.OLED_ShowString(0,4,"LOCATION:",12); 
  }

  void manu_dyn()
  {
    if(Flag_Arm==0)
       myOLED.OLED_ShowString(25,1,"OFF",12);
   else
   myOLED.OLED_ShowString(25,1,"ON ",12);   
     
    if(Flag_Mode==0)
        myOLED.OLED_ShowString(90,1,"AUTO",12);
    else
       myOLED.OLED_ShowString(90,1,"HOLD",12);
       
     myOLED.OLED_ShowString(25,2,"   ",12);  
     myOLED.OLED_Showint(25,2,Throat,12);
    // myOLED.OLED_ShowNum(25,2,Throat,16);
     
     myOLED.OLED_ShowString(90,2,"   ",12);  
    myOLED.OLED_Showint(90,2,reading_cm,12);
   
    // vol_integer=11;
     //vol_decimal=2;
         myOLED.OLED_Showint(25,3,vol_integer,12);
    myOLED.OLED_ShowString(40,3,".",12);
    myOLED.OLED_Showint(45,3,vol_decimal,12);
    
    //cur_integer=50;
    //cur_decimal=3;
    myOLED.OLED_Showint(90,3,cur_integer,12);
    myOLED.OLED_ShowString(106,3,".",12);
    myOLED.OLED_Showint(111,3,cur_decimal,12);  
   // myOLED.OLED_ShowString(25,3,"(133,44)",12); 
   myOLED.OLED_ShowString(65,3,"CU:",12);
   myOLED.OLED_ShowString(0,4,"LOCATION:",12); 
  if(myGPS.read()&&(!myGPS.gpsAvailable()))
  {
    myOLED.OLED_ShowString(0,5,myGPS.gpsp[3],12);
    myOLED.OLED_ShowString(0,6,myGPS.gpsp[4],12);
  }
  else    
  {
    myOLED.OLED_ShowString(0,6,"GPS unavailable",12);
    myOLED.OLED_ShowString(0,5,"GPS unavailable",12);
  }
  }
//2ms中断处理函数
ISR(TIMER2_COMPA_vect){                      // triggered when Timer2 counts to 124
aaa++;
bbb++;
ccc++;
if(aaa==time_5s)
{
   dir = 93;
   Channel4.write(dir);  // 
  // Serial.println(dir); 
  //cli();                                     // 关闭定时器中断
   aaa=0;
  }

 //显示刷新标志
if(bbb==time_disp)
{ 
  dispay=true;
  bbb=0; 
 }
//显示清屏标志
if(ccc==time_10s)
{
  clear_flag =true;
  ccc=0; 
 }
 
 // sei();                                   // 再次使能定时器中断，开启采集
}// end isr

void setup()
{
	Serial.begin(9600);                       //串口0  下载口
	Serial3.begin(9600);                      //串口3  gps口
  Serial2.begin(9600);                      //串口2  mini 口 深度数据
  
	Channel3.attach(8);
	Channel4.attach(9);
	Channel5.attach(10);
   // Timer2 初始化 2ms定时中断
  TCCR2A = 0x02;                             // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;                             // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;                              // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;                             // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  //sei();                                   // 使能定时器中断    
//显示器初始化
  myOLED.OLED_Init();
  myOLED.OLED_Clear();
//显示初始菜单
 manu_st();
 manu_dyn();
	while (!(Serial& Serial3))              //串口0、3没打开时，一直循环
	{
		;
	}
	reading_cm = 0;
	Wire.begin(0x70);					                // �Wire.begin(address)  初始化wire库 ，并且加入到I2C网络， 从机
	Wire.onRequest(requestEvent);	          	//��注册主机通知从机上传数据的事件�

 
 // 外部中断attachInterrupt()
	attachInterrupt(Key_Mode	, Mode_Intpr, FALLING);  //attachInterrupt(interrupt, ISR, mode)            模式中断服务函数
	attachInterrupt(Key_Arm		, Arm_Intpr, FALLING);   //attachInterrupt(中断号，中断服务函数, 中断模式)  解锁模式中断服务函数
	attachInterrupt(Key_Up		, Up_Intpr, FALLING);    //中断服务函数,这个函数必须没有参数没有返回值；    油门增加中断服务函数
	attachInterrupt(Key_Dowm	, Dowm_Intpr, FALLING);  //                                                 油门减少中断服务函数

 // MsTimer2::set(100,turnback);                     // 中断设置函数，每 5000ms 进入一次中断 
 
//mode:定义中断触发类型，有四种形式：
// LOW：低电平触发；
// CHANGE：电平变化触发；
// RISING ：上升沿触发（由LOW变为HIGH）；
// FALLING：下降沿触发（由HIGH变为LOW）； 
// Due板子还支持高电平触发。
//detachInterrupt(interrupt)； 可以关掉中断

}

void loop()
{
	Get_Deep();                                       //获取深度数据
 if(dispay==true)
 {
  vol_cur_acqu();                                   //获取电压电流
   manu_dyn();                                      //显示动态数据
    dispay=false;  
  }
  
 if(clear_flag==true)
 {
    myOLED.OLED_Clear();                           //清屏
    manu_st();                                     //显示静态数据
    manu_dyn();                                    //显示动态数据
    clear_flag=false;  
  } 
 
 }

//I2C    将获取到的深度数据用i2c传给飞控  用于HOLD模式下保持高度一定
void requestEvent()
{
	byte sendhi;
	byte sendli;
	byte sendbyte[2];
	uint16_t tempreading_cm;
	tempreading_cm = reading_cm;
	sendhi = tempreading_cm >> 8;
	sendli = tempreading_cm & 0xff;
	sendbyte[0] = sendhi;
	sendbyte[1] = sendli;
	Wire.write(sendbyte, 2);
}

//从串口3 mini发送的深度原始数据 得到深度数据
void Get_Deep()
{
	
	if (Serial3.available() > 0)
	{
		if (Serial3.read() == '*')
		{
			for (int i = 0; i < 4; i++)
			{
				if (Serial3.available() > 0)
				{

					buff[i] = Serial3.read() - 48;  //ASCII码转化成相应字符/数字
				}
			}
			tempread = buff[0] * 1000
				+ buff[1] * 100
				+ buff[2] * 10
				+ buff[3];
			if (tempread<1000 || tempread>2000)   //0～1000 或 2000+
				reading_cm = reading_cm_last;
			else
			{
				reading_cm = tempread - 1000;
				reading_cm_last = reading_cm;      
				//Serial.println(reading_cm);      ///串口发送数据
			}		
		}
		delay(10);
	}  
}

//模式按键中断处理函数
void Mode_Intpr()
{
  delay(1000);
  if(digitalRead(2)==0)
  {
	//Serial.println("Interrupt Mode 2");
	if (!Flag_Mode)
	{
    mode=120;
		Channel5.write(mode); //�模式0    //模式口方波  100%    auto模式
	}
	else
	{
    mode=70;
		Channel5.write(mode); //�模式1    //模式口方波  50%     hold模式 
	}
 
	delay(500);
	Flag_Mode = !Flag_Mode;             //模式标志取反  
  //Serial.println(Flag_Mode);  
   //Serial.println(mode);   
  }     
}

//开关/解锁按键中断
void Arm_Intpr()
{
  delay(1000);
  if(digitalRead(3)==0)
  {
  /*
  int dt= micros() - timelast;
  if(dt>1500) 
  {
   */ 
    cli();
	//Serial.println("Interrupt Arm 3");
     Throat = 50;
   Channel3.write(Throat);  //油门最低  
    if(Flag_Arm) 
    {
   dir = 93;
   Channel4.write(dir);  // 
    }
    else
    {
   dir = 135;
   Channel4.write(dir);  //  
     aaa=0;
      bbb=0;
      ccc=0;
    sei();             // 使能定时器中断    开始5s计时   
      }
	Flag_Arm=!Flag_Arm;     //
  }
 // timelast = micros();
  //Serial.println(Flag_Arm);  
  //Serial.println(dir); 
}

//油门增加按键中断
void Up_Intpr()
{
  delay(1000);
  if(digitalRead(19)==0)
  {
	//Serial.println("Interrupt Up 19");

	Throat += 10;
	if (Throat < 121)
	{
		Channel3.write(Throat);
	}
	else
	{
		Throat = 120;
		Channel3.write(Throat);
	}
	delay(100);
 //Serial.println(Throat);
  }
}

//油门减少按键中断
void Dowm_Intpr()
{
  delay(1000);
  if(digitalRead(18)==0)
  {
	//Serial.println("Interrupt Dowm 18");
	Throat -= 10;
	if (Throat >50)
	{
		Channel3.write(Throat);
	}
	else
	{
		Throat = 50;
		Channel3.write(Throat);
	}
	delay(100);
  //Serial.println(Throat);
  }
}

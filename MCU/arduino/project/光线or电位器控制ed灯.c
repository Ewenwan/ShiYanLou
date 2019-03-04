/*

根据光线传感器 感 知外界环境亮度，并控制led的亮度。

主要内容：模拟信号读取，pwm

目标：

1.按照不同环境亮度，控制led的亮度。

2.设定阈值，控制led亮灭。

3.再加入电位器，控制亮度，电位器优先控制模式。

说明：

此次实验用到器材及对应pin口：

define LED 14 //A0模拟pin口，LED灯 define Light 15 //A1模拟pin口，光线传感器 define Rotation 16 //A2模拟pin口，旋转式电位器

LED灯亮度采用电位器优先，环境亮度辅助的控制模式

在电位器读数为0时由环境亮度控制LED灯亮度，环境亮度通过int light=analogRead(Light)读取光线传感器 PWM 输入数值确定，鉴于自己台灯的亮度数值，确定光线传感器PWM输入数值范围大致为666-921（通过串口通信获取），再借由map(light,666,921,0,255)将其转化到PWM 输出范围，作为analogWrite输出到LED灯的PWM数值；

在电位器读数不为0时由电位器控制LED灯亮度，电位器PWM输入数值由int rota=analogRead(Rotation)读取，然后同上用analogWrite(LED,map(rota,0,1023,0,255))控制LED灯亮度（电位器PWM输入范围0-1023）;

LED灯亮灭的阈值，借由电位器控制，再通过串口通信，得知大致在半值，即2.5V左右。
*/

#define LED 6        //A0
#define Light 15      //A1
#define Rotation 16   //A2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED,OUTPUT);    // led灯输出
  pinMode(Light,INPUT);   // 光纤传感器 输入
  pinMode(Rotation,INPUT);// 电位器输入
}

void loop() {
  // put your main code here, to run repeatedly:
  int rota=analogRead(Rotation);// 电位器模拟量读数
  // 电位器控制 
  if(rota != 0)
  {
    Serial.println("LED controlled by rotation!");
    Serial.println(rota);
    analogWrite(LED,map(rota,0,1023,0,255));// 电位器读数范围(0~1023) 映射成(0~255) 再写成 对应的 pwm输出(相当于映射成0~5伏)
    delay(500); 
    }
  else
  //  电位器读数为0时 由 环境亮度 控制 led亮度
  {
    Serial.println("LED effected by environment!");
    int light=analogRead(Light); // 光线传感器 读数 范围 200,1200
    //666,921是根据自己台灯亮度取的接近数值
    analogWrite(LED,map(light,200,1200,0,255));// 范围 200,1200 映射成 (0~255) 
    delay(500); 
    }
}

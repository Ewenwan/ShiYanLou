/*
        DIY Arduino based RC Transmitter
  by Dejan Nedelkovski, www.HowToMechatronics.com
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
  
 详情 https://howtomechatronics.com/projects/diy-arduino-rc-transmitter/
 开发板：Arduino Pro or Pro Mini
 处理器：ATmege328
 编程器：USBasp
*/

#include <SPI.h>        // 无线通信 SPI通信
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>       // MPU6050 i2c通信


// 按键输入接口定义
#define jB1 1  // Joystick button 1
#define jB2 0  // Joystick button 2
#define t1 7   // Toggle switch 1      imu重力感应模式
#define t2 4   // Toggle switch 1      灯光开关
#define b1 8   // Button 1
#define b2 9   // Button 2
#define b3 2   // Button 3
#define b4 3   // Button 4

// IMU 重力传感器参数定义
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float angleX, angleY;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY;
float elapsedTime, currentTime, previousTime;
unsigned long imu_beg_time;
unsigned long imu_beg_cnt=0;
int c = 0;

unsigned long t1_flag_time,t2_flag_time;
bool t1_flag=true;
bool t2_flag=true;

// 无线通信接口，SPI通信
RF24 radio(5, 6);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

// 无线通信发送信息结构体 Max size of this struct is 32 bytes - NRF24L01 buffer limit
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

Data_Package data; //Create a variable with the above structure

// 初始化
void setup() {
  // 串口初始化
  Serial.begin(9600);
  
  // 初始化IMU I2C通信接口 Initialize interface to the MPU6050
  initialize_MPU6050();

  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  
  // 初始化 无线通讯SPI接口
  radio.begin();
  radio.openWritingPipe(address);
  radio.printDetails();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);// 通信速率
  radio.setPALevel(RF24_PA_LOW);

  // add
  //radio.setRetries(15, 15);
  //radio.setChannel(25);
  
  //radio.openWritingPipe(address);
  
  // 按键输入IO口上拉 Activate the Arduino internal pull-up resistors
  pinMode(jB1, INPUT_PULLUP);
  pinMode(jB2, INPUT_PULLUP);
  pinMode(t1, INPUT_PULLUP);
  pinMode(t2, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);

  radio.printDetails();
  Serial.println("isChipConnected: ");
  Serial.println(radio.isChipConnected());
   
  // Set initial default values
  // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127.
  // We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  // 模拟量策测量数据为0～1023需要被映射到0～255来进行发送
  data.j1PotX = 127; 
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.pot1 = 1;
  data.pot2 = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.button1 = 1;
  data.button2 = 1;
  data.button3 = 1;
  data.button4 = 1;
}

// 
void loop() {
  // Read all analog inputs and map them to one Byte value
  // 读取模拟量输入的值并映射到合适区间，设置到发送数据结构体
  data.j1PotX = map(analogRead(A1), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(A0), 0, 1023, 0, 255);
  data.j2PotX = map(analogRead(A2), 0, 1023, 0, 255);
  data.j2PotY = map(analogRead(A3), 0, 1023, 0, 255);
  data.pot1 = map(analogRead(A7), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(A6), 0, 1023, 0, 255);
  // 读取数字量输入 Read all digital inputs
  data.j1Button = digitalRead(jB1);
  data.j2Button = digitalRead(jB2);

  int reading = digitalRead(t1);
  if( t1_flag != reading)
  {
    t1_flag_time=millis();
  }
  if(millis()-t1_flag_time>50)
  {
      if(!digitalRead(t1))
      {
        data.tSwitch1 = 0;
      }
      else
      {
        data.tSwitch1 = 1;
      }
  }
  t1_flag=reading;

  reading = digitalRead(t2);
  if( t2_flag != reading)
  {
    t2_flag_time=millis();
  }
  if(millis()-t2_flag_time>50)
  {
      if(!digitalRead(t2))
      {
        data.tSwitch2 = 0;
      }
      else
      {
        data.tSwitch2 = 1;
      }
  } 
  t2_flag=reading;
  
  //data.tSwitch2 = digitalRead(t2);
  //data.tSwitch1 = digitalRead(t1);
  
  data.button1 = digitalRead(b1);
  data.button2 = digitalRead(b2);
  data.button3 = digitalRead(b3);
  data.button4 = digitalRead(b4);
  // 重力感应控制开关 If toggle switch 1 is switched on
  if (data.tSwitch1 == 0) {
    //Serial.println("imu mod");
    // 一开始数据会不稳定
    imu_beg_cnt++;
    if(1 == imu_beg_cnt)
    {
          imu_beg_time=millis();
      }
    read_IMU();    // Use MPU6050 instead of Joystick 1 for controling left, right, forward and backward movements
  }
  else
  {
    imu_beg_cnt=0;
  }
  // Send the whole data from the structure to the receiver
  bool ok = radio.write(&data, sizeof(Data_Package));
  if(ok)
  {
    //Serial.println("  send ok");
  }
  else
  {
    //Serial.println("  send failed");
  }
  //Serial.println("send...  ");
  //Serial.print(data.j1PotX);
  //Serial.print('\t');
  //Serial.print(data.j2PotY);
}

// IMU端口初始化函数
void initialize_MPU6050() {
  Wire.begin();                      // Initialize comunication 初始化I2C通信
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  // Configure Accelerometer
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000dps full scale)
  Wire.endTransmission(true);
}

// 计算IMU漂移量(误差)
void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gury data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 4, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 32.8);
    GyroErrorY = GyroErrorY + (GyroY / 32.8);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
}

// 读取 IMU传感器的值 并设置 x轴和Y轴偏转角度为无线发送的偏转角度信息信息
void read_IMU() {
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-8g, we need to divide the raw values by 4096, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0; // Z-axis value

  // Calculating angle values using
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + 1.15; // AccErrorX ~(-1.15) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - 0.52; // AccErrorX ~(0.5)

  // === Read gyro data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000;   // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 4, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 32.8; // For a 1000dps range we have to divide first the raw value by 32.8, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 32.8;
  GyroX = GyroX + 1.85; //// GyroErrorX ~(-1.85)
  GyroY = GyroY - 0.15; // GyroErrorY ~(0.15)
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = GyroX * elapsedTime;
  gyroAngleY = GyroY * elapsedTime;

  // Complementary filter - combine acceleromter and gyro angle values
  angleX = 0.98 * (angleX + gyroAngleX) + 0.02 * accAngleX;
  angleY = 0.98 * (angleY + gyroAngleY) + 0.02 * accAngleY;
  // Map the angle values from -90deg to +90 deg into values from 0 to 255, like the values we are getting from the Joystick
  if( (millis() - imu_beg_time) > 2000)
  {
  data.j1PotX = map(angleX, -90, +90, 255, 0);    // 左右转
  //data.j1PotY = map(angleY, -90, +90, 0, 255);  
  }
}

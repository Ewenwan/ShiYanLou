#include "Wire.h"
#include "SPI.h"  
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

#define Gry_offset 0 
#define Gyr_Gain 0.00763358  
#define Angle_offset 0
#define RMotor_offset 20
#define LMotor_offset 20
#define pi 3.14159 

long data;
int x, y;
float kp, ki, kd; 
float r_angle, f_angle, omega;
float Turn_Speed = 0, Turn_Speed_K = 0;
float Run_Speed = 0, Run_Speed_K = 0, Run_Speed_T = 0;
float LOutput,ROutput;

unsigned long preTime = 0;
float SampleTime = 0.08;
unsigned long lastTime;
float Input, Output;
float errSum, dErr, error, lastErr;
int timeChange; 

int TN1=3;
int TN2=4;
int ENA=9;

int TN3=5;
int TN4=6;
int ENB=10;

void setup() {
  Wire.begin();
  accelgyro.initialize();
  pinMode(TN1,OUTPUT);
  pinMode(TN2,OUTPUT);
  pinMode(TN3,OUTPUT);
  pinMode(TN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  
  Mirf.spi = &MirfHardwareSpi;   
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(long);
  Mirf.config();
  Serial.begin(115200);
}

void loop() {
  Recive();
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  r_angle = (atan2(ay, az) * 180 / pi + Angle_offset);
  omega =  Gyr_Gain * (gx +  Gry_offset);  Serial.print("  omega="); Serial.print(omega);
  if (abs(r_angle)<45){
    myPID();
    PWMControl();
  }
  else{
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
}

void Recive(){
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData((byte *)&data);
    Mirf.rxFifoEmpty();
    
    x = data >> 16;
    y = data & 0x0000ffff;
    
    if(x >= 520){
      Run_Speed_K = map(x, 520, 1023, 0, 100);
      Run_Speed_K = Run_Speed_K / 50;
      Run_Speed = Run_Speed + Run_Speed_K;
    }
    else if(x <= 480){
      Run_Speed_K = map(x, 480, 0, 0, -100);
      Run_Speed_K = Run_Speed_K / 50;
      Run_Speed = Run_Speed + Run_Speed_K;
    }
    else{
      Run_Speed_K = 0;
    }
    if(y >= 520){
      Turn_Speed = map(y, 520, 1023, 0, 20);
    }
    else if(y <= 480){
      Turn_Speed = map(y,480,0,0,-20);
    }
    else{
      Turn_Speed = 0;
    }
  }
  else{
    x = y = 500;
  }
}

void myPID(){
  kp = analogRead(A0)*0.1;  Serial.print("  kp=");Serial.print(kp);
  kd = analogRead(A2)*1.0;  Serial.print("  kd=");Serial.print(kd);
  //ki = analogRead(A3)*0.001;  Serial.print("  ki=");Serial.print(ki);

  //kp = 0;  Serial.print("  kp=");Serial.print(kp);
  //kd = 0;  Serial.print("  kd=");Serial.print(kd);
  ki = 0;  Serial.print("  ki=");Serial.print(ki);
   
  unsigned long now = millis();
  float dt = (now - preTime) / 1000.0;
  preTime = now;
  float K = 0.8;
  float A = K / (K + dt);
  f_angle = A * (f_angle + omega * dt) + (1 - A) * r_angle;  Serial.print("  f_angle=");Serial.print(f_angle);
  
  timeChange = (now - lastTime);
  if(timeChange >= SampleTime){
    Input = f_angle;
    error = Input;
    errSum += error * timeChange;
    dErr = (error - lastErr) / timeChange;
    Output = kp * error + ki * errSum + kd * dErr;
    LOutput = Output + Run_Speed + Turn_Speed;  Serial.print("  LOutput=");Serial.print(LOutput);
    ROutput = Output + Run_Speed - Turn_Speed;  Serial.print("  ROutput=");Serial.println(ROutput);
    lastErr = error;
    lastTime = now;
  }
}

void PWMControl(){
  if(LOutput > 0){
    digitalWrite(TN1, HIGH);
    digitalWrite(TN2, LOW);
  }
  else if(LOutput < 0){
    digitalWrite(TN1, LOW);
    digitalWrite(TN2, HIGH);
  }
  else{
    digitalWrite(TN1, HIGH);
    digitalWrite(TN2, HIGH);
  }
  if(ROutput > 0){
    digitalWrite(TN3, HIGH);
    digitalWrite(TN4, LOW);
  }
  else if(ROutput < 0){   
    digitalWrite(TN3, LOW);
    digitalWrite(TN4, HIGH);
  }
  else{
    digitalWrite(TN3, HIGH);
    digitalWrite(TN4, HIGH);
  }
    analogWrite(ENA, min(255, abs(LOutput) + LMotor_offset));
    analogWrite(ENB, min(255, abs(ROutput) + RMotor_offset));
}



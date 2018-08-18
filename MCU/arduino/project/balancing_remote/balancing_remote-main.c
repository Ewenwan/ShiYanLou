#include "SPI.h"
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include "Wire.h" 
 
long Joystick_1_X;
long Joystick_1_Y;
long Joystick_2_X;
long Joystick_2_Y;
long Joystick_1;
long Joystick_2;

void setup(){
  Serial.begin(115200);
  Mirf.spi = &MirfHardwareSpi;  
  Mirf.init(); 
  Mirf.setTADDR((byte *)"serv1");
  Mirf.payload = sizeof(long);
  Mirf.config(); 
 }

void loop(){
  Joystick_1_X = analogRead(A0);
  Joystick_1_Y = analogRead(A1);
  //Joystick_2_X = analogRead(A2);
  //Joystick_2_Y = analogRead(A3);
  
  //Serial.print("Joystick_1_X=");Serial.print(Joystick_1_X); Serial.print("  Joystick_1_Y=");Serial.print(Joystick_1_Y);Serial.print("  Joystick_2_X=");Serial.print(Joystick_2_X); Serial.print("  Joystick_2_Y=");Serial.print(Joystick_2_Y);
  
  Joystick_1_X = Joystick_1_X << 16;
  Joystick_1 = Joystick_1_X | Joystick_1_Y;
 
  //Serial.print("  Joystick_1_X=");Serial.print(Joystick_1_X);
  Serial.print("  Joystick_1=");Serial.print(Joystick_1);
  
  //Joystick_2_X = Joystick_2_X << 16;
  //Joystick_2 = - (Joystick_2_X | Joystick_2_Y);
  
  //Serial.print("  Joystick_2_X=");Serial.print(Joystick_2_X);
  //Serial.print("  Joystick_2=");Serial.println(Joystick_2);
  
  Mirf.send((byte *)&Joystick_1);
  //delay(10);
  //Mirf.send((byte *)&Joystick_2);
  while(Mirf.isSending()){
  }
}
  

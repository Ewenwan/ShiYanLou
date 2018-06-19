#include <TimerThree.h>
 
void setup()
{
  pinMode(13, OUTPUT);
  Timer3.initialize(500000);         // initialize timer1, and set a 1/2 second period
  Timer3.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer3.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
 
void callback()
{
  digitalWrite(13, digitalRead(10) ^ 1);
}
 
void loop()
{
  // your program here...
}

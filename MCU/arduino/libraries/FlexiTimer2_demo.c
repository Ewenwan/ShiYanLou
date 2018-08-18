char dummyvar; // to get Arduinoi IDE to include core headers properly

/*
  FlexiTimer2:
  Arduino library to use timer 2 with a configurable resolution.
  Based on MsTimer2 by Javier Valencia. It is called FlexiTimer2 because it
  is based on MsTimer2, but offers more flexibility,
  since it has a configurable timer resolution.
  MsTimer2 library: http://www.arduino.cc/playground/Main/MsTimer2

For more details on FlexiTimer2 see: 
 http://www.arduino.cc/playground/Main/FlexiTimer2
 https://github.com/wimleers/flexitimer2

*/

#include <FlexiTimer2.h>

// Switch on LED on and off each half second

#if defined(ARDUINO) && ARDUINO >= 100
const int led_pin = LED_BUILTIN;	// 1.0 built in LED pin var
#else
#if defined(CORE_LED0_PIN)
const int led_pin = CORE_LED0_PIN;	// 3rd party LED pin define
#else
const int led_pin = 13;			// default to pin 13
#endif
#endif

void flash()
{
static boolean output = HIGH;
  
  digitalWrite(led_pin, output);
  output = !output;
}

void setup()
{
  pinMode(led_pin, OUTPUT);

  FlexiTimer2::set(500, 1.0/1000, flash); // call every 500 1ms "ticks"
  // FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
  FlexiTimer2::start();
}

void loop()
{
}

/*
 *  Interrupt and PWM utilities for 16 bit Timer3 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified Oct 2009 by Dan Clemens to work with timer3 of the ATMega1280 or Arduino Mega
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#include "TimerThree.h"

TimerThree Timer3;              // preinstatiate

ISR(TIMER3_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer3.isrCallback();
}

void TimerThree::initialize(long microseconds)
{
  TCCR3A = 0;                 // clear control register A 
  TCCR3B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}

void TimerThree::setPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  ICR3 = pwmPeriod = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  TCCR3B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR3B |= clockSelectBits;                                                     // reset clock select register
}

void TimerThree::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  if(pin == 5) OCR3A = dutyCycle;
  if(pin == 2) OCR3B = dutyCycle;
  if(pin == 3) OCR3C = dutyCycle;
}

void TimerThree::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  
	// sets data direction register for pwm output pin
	// activates the output pin
  if(pin == 5) { DDRE |= _BV(PORTE3); TCCR3A |= _BV(COM3A1); }
  if(pin == 2) { DDRE |= _BV(PORTE4); TCCR3A |= _BV(COM3B1); }
  if(pin == 3) { DDRE |= _BV(PORTE5); TCCR3A |= _BV(COM3C1); }
  setPwmDuty(pin, duty);
  start();
}

void TimerThree::disablePwm(char pin)
{
  if(pin == 5) TCCR3A &= ~_BV(COM3A1);   // clear the bit that enables pwm on PE3
  if(pin == 2) TCCR3A &= ~_BV(COM3B1);   // clear the bit that enables pwm on PE4
  if(pin == 3) TCCR3A &= ~_BV(COM3C1);   // clear the bit that enables pwm on PE5
}

void TimerThree::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK3 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  start();
}

void TimerThree::detachInterrupt()
{
  TIMSK3 &= ~_BV(TOIE1);                                   // clears the timer overflow interrupt enable bit 
}

void TimerThree::start()
{
  TCCR3B |= clockSelectBits;
}

void TimerThree::stop()
{
  TCCR3B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));          // clears all clock selects bits
}

void TimerThree::restart()
{
  TCNT3 = 0;
}

/*
Copyright (c) 2012 Sam Knight
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

#include "wiring_private.h"
#include "../PWM.h"

#define UINT16_MAX 65535
#define UINT8_MAX 255

//--------------------------------------------------------------------------------
//							Helper Functions
//--------------------------------------------------------------------------------

static float toBaseTwo(double baseTenNum)
{
	return log(baseTenNum + 1)/log(2);
}

//--------------------------------------------------------------------------------
//							16 Bit Timer Functions
//--------------------------------------------------------------------------------

uint32_t GetFrequency_16()
{
	return (int32_t)(F_CPU/(2 * (int32_t)GetTop_16() * GetPrescaler_16()));
}

bool SetFrequency_16(uint32_t f)
{
	if(f > 2000000 || f < 1)
	return false;
	
	//find the smallest usable multiplier
	uint16_t multiplier = (int16_t)(F_CPU / (2 * f * UINT16_MAX));
	
	uint8_t iterate = 0;
	while(multiplier > pscLst[iterate++]);
	
	multiplier = pscLst[iterate]; //multiplier holds the clock select value, and iterate holds the corresponding CS flag
	
	//getting the timer top using the new multiplier
	uint16_t timerTop = (uint16_t)(F_CPU/(2* f * multiplier));
	
	SetTop_16(timerTop);
	SetPrescaler_16((prescaler)iterate);
	
	return true;
}

uint16_t GetPrescaler_16()
{
	return pscLst[(TCCR1B & 7)];
}

void SetPrescaler_16(prescaler psc)
{
	TCCR1B = (TCCR1B & ~7) | (psc & 7);
}

void SetTop_16(uint16_t top)
{
	ICR1 = top;
}

uint16_t GetTop_16()
{
	return ICR1;
}

void Initialize_16()
{
	//setting the waveform generation mode
	uint8_t wgm = 8;
	
	TCCR1A = (TCCR1A & B11111100) | (wgm & 3);
	TCCR1B = (TCCR1B & B11100111) | ((wgm & 12) << 1);
	
	SetFrequency_16(500);
}

float GetResolution_16()
{
	return toBaseTwo(ICR1);
}

//--------------------------------------------------------------------------------
//							8 Bit Timer Functions
//--------------------------------------------------------------------------------

uint32_t GetFrequency_8(const int16_t timerOffset)
{
	return (uint32_t)(F_CPU/((uint32_t)2 * GetTop_8(timerOffset) * GetPrescaler_8(timerOffset)));
}

bool SetFrequency_8(const int16_t timerOffset, uint32_t f)
{
	if(f > 2000000 || f < 31)
		return false;
	
	//find the smallest usable multiplier
	uint16_t multiplier = (F_CPU / (2 * (uint32_t)f * UINT8_MAX));
	
	uint8_t iterate = 0;
	uint16_t timerTop;
	
	do
	{
		if(TIMER2_OFFSET != timerOffset)
		{
			while(multiplier > pscLst[++iterate]);
			multiplier = pscLst[iterate];
		}
		else
		{
			while(multiplier > pscLst_alt[++iterate]);
			multiplier = pscLst_alt[iterate];
		}
		
		//getting the timer top using the new multiplier
		timerTop = (F_CPU/(2* f * (uint32_t)multiplier));
		
	} while (timerTop > UINT8_MAX);
	
	SetTop_8(timerOffset, timerTop);
	
	if(timerOffset != TIMER2_OFFSET)
	SetPrescaler_8(timerOffset, (prescaler)iterate);
	else
	SetPrescalerAlt_8(timerOffset, (prescaler_alt)iterate);
	
	return true;
}

uint16_t GetPrescaler_8(const int16_t timerOffset)
{
	if(timerOffset != TIMER2_OFFSET)
		return pscLst[(TCCRB_8(timerOffset) & 7)];
	else
		return pscLst_alt[(TCCRB_8(timerOffset) & 7)];
}

void SetPrescaler_8(const int16_t timerOffset, prescaler psc)
{
	TCCRB_8(timerOffset) = (TCCRB_8(timerOffset) & ~7) | (psc & 7);
}

void SetPrescalerAlt_8(const int16_t timerOffset, prescaler_alt psc)
{
	TCCRB_8(timerOffset) = (TCCRB_8(timerOffset) & ~7) | (psc & 7);
}

void SetTop_8(const int16_t timerOffset, uint8_t top)
{
	OCRA_8(timerOffset) = top;
}

uint8_t	GetTop_8(const int16_t timerOffset)
{
	return OCRA_8(timerOffset);
}

void Initialize_8(const int16_t timerOffset)
{
	//setting the waveform generation mode
	uint8_t wgm = 5;
	
	TCCRA_8(timerOffset) = (TCCRA_8(timerOffset) & B11111100) | (wgm & 3);
	TCCRB_8(timerOffset) = (TCCRB_8(timerOffset) & B11110111) | ((wgm & 12) << 1);
	
	//disable timer0's interrupt when initialization is called, otherwise handler will eat
	//up processor cycles when PWM on timer0 is set to high frequencies. This will effectively disable
	//Arduino's time keeping functions, which the user should be aware of before initializing timer0
	if(timerOffset == 0) 
	{
		TIMSK0 &= B11111110;
	}
	
	SetFrequency_8(timerOffset, 500);
}

float GetResolution_8(const int16_t timerOffset)
{
	return toBaseTwo(OCRA_8(timerOffset));
}


//--------------------------------------------------------------------------------
//							Timer Independent Functions
//--------------------------------------------------------------------------------

void pwmWrite(uint8_t pin, uint8_t val)
{
	pinMode(pin, OUTPUT);
	
	//casting "val" to be larger so that the final value (which is the partially
	//the result of multiplying two potentially high value int16s) will not truncate
	uint32_t tmp = val;
	
	if (val == 0)
		digitalWrite(pin, LOW);
	else if (val == 255)
		digitalWrite(pin, HIGH);
	else
	{
		uint16_t regLoc16 = 0;
		uint16_t regLoc8 = 0;
		
		uint16_t top;
		
		switch(digitalPinToTimer(pin))
		{
			case TIMER0B:
			sbi(TCCR0A, COM0B1);
			regLoc8 = OCR0B_MEM;
			top = Timer0_GetTop();
			break;
			case TIMER1A:
			sbi(TCCR1A, COM1A1);
			regLoc16 = OCR1A_MEM;
			top = Timer1_GetTop();
			break;
			case TIMER1B:
			sbi(TCCR1A, COM1B1);
			regLoc16 = OCR1B_MEM;
			top = Timer1_GetTop();
			break;
			case TIMER2B:
			sbi(TCCR2A, COM2B1);
			regLoc8 = OCR2B_MEM;
			top = Timer2_GetTop();
			break;
			case NOT_ON_TIMER:
			default:
			if (val < 128)
			digitalWrite(pin, LOW);
			else
			digitalWrite(pin, HIGH);
			return;
		}
		
		if(regLoc16)
			_SFR_MEM16(regLoc16) = (tmp*top)/255;
		else
			_SFR_MEM8(regLoc8) = (tmp*top)/255;
		
	}		
}

//takes a 16 bit value instead of an 8 bit value for maximum resolution
void pwmWriteHR(uint8_t pin, uint16_t val)
{
	pinMode(pin, OUTPUT);
	
	uint32_t tmp = val;
	
	if (val == 0)
	digitalWrite(pin, LOW);
	else if (val == 65535)
	digitalWrite(pin, HIGH);
	else
	{
		uint16_t regLoc16 = 0;
		uint16_t regLoc8 = 0;
		
		uint16_t top;
		switch(digitalPinToTimer(pin))
		{
			case TIMER0B:
			sbi(TCCR0A, COM0B1);
			regLoc8 = OCR0B_MEM;
			top = Timer0_GetTop();
			break;
			case TIMER1A:
			sbi(TCCR1A, COM1A1);
			regLoc16 = OCR1A_MEM;
			top = Timer1_GetTop();
			break;
			case TIMER1B:
			sbi(TCCR1A, COM1B1);
			regLoc16 = OCR1B_MEM;
			top = Timer1_GetTop();
			break;
			case TIMER2B:
			sbi(TCCR2A, COM2B1);
			regLoc8 = OCR2B_MEM;
			top = Timer2_GetTop();
			break;
			case NOT_ON_TIMER:
			default:
			if (val < 128)
			digitalWrite(pin, LOW);
			else
			digitalWrite(pin, HIGH);
			return;
		}			
		
		if(regLoc16)
			_SFR_MEM16(regLoc16) = (tmp*top)/65535;
		else
			_SFR_MEM8(regLoc8) = (tmp*top)/65535;
	}
}

void InitTimers()
{
	Timer0_Initialize();
	Timer1_Initialize();
	Timer2_Initialize();
}

void InitTimersSafe()
{
	Timer1_Initialize();
	Timer2_Initialize();
}

extern bool SetPinFrequency( int8_t pin, uint32_t frequency )
{
	uint8_t timer = digitalPinToTimer(pin);
	
	if(timer == TIMER0B)
		return Timer0_SetFrequency(frequency);
	else if(timer == TIMER1A || timer == TIMER1B)
		return Timer1_SetFrequency(frequency);
	else if(timer == TIMER2B)
		return Timer2_SetFrequency(frequency);
	else
		return false;
}

bool SetPinFrequencySafe(int8_t pin, uint32_t frequency)
{
	uint8_t timer = digitalPinToTimer(pin);
	
	if(timer == TIMER1A || timer == TIMER1B)
		return Timer1_SetFrequency(frequency);
	else if(timer == TIMER2B)
		return Timer2_SetFrequency(frequency);
	else
		return false;
}

float GetPinResolution(uint8_t pin)
{
	uint8_t timer = digitalPinToTimer(pin);	
	uint16_t top;
	
	switch(timer)
	{
		case TIMER0B:
			top = Timer0_GetTop();
			break;
		case TIMER1A:
			top = Timer1_GetTop();
			break;
		case TIMER1B:
			top = Timer1_GetTop();
			break;
		case TIMER2B:
			top = Timer2_GetTop();
		default:
			return 0;
	}
	
	return toBaseTwo(top);
}

#endif
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


#ifndef BTIMERDEFS_H_
#define BTIMERDEFS_H_

#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)

//physical memory locations used in pwmWrite()
#define OCR1A_MEM	0x88
#define OCR1B_MEM	0x8A
#define OCR1C_MEM	0x8C

#define OCR0A_MEM	0x47
#define OCR0B_MEM	0x48
#define OCR2A_MEM	0xB3
#define OCR2B_MEM	0xB4

#define TCCR1A_MEM	0x80
#define TCCR1B_MEM	0x81
#define TCCR1C_MEM	0x82

#define TCCR0A_MEM	0x44
#define TCCR0B_MEM	0x45
#define TCCR2A_MEM	0xB0
#define TCCR2B_MEM	0xB1

#define ICR1_MEM	0x86

//8 bit timers
#define TIMER0_OFFSET	0x00
#define TIMER2_OFFSET	0x6C

#define TCCRA_8(tmr_offset)		_SFR_MEM8(0x44 + tmr_offset)
#define TCCRB_8(tmr_offset)		_SFR_MEM8(0x45 + tmr_offset)
#define OCRA_8(tmr_offset)		_SFR_MEM8(0x47 + tmr_offset)

static const uint16_t pscLst[] = { 0, 1, 8, 64, 256, 1024};
static const uint16_t pscLst_alt[] = {0, 1, 8, 32, 64, 128, 256, 1024};

enum prescaler
{
	ps_1	=	1,
	ps_8	=	2,
	ps_64	=	3,
	ps_256	=	4,
	ps_1024 =	5
};

//certain 8 bit timers read the CSn register differently
enum prescaler_alt
{
	psalt_1		=	1,
	psalt_8		=	2,
	psalt_32	=	3,
	psalt_64	=	4,
	psalt_128	=	5,
	psalt_256	=	6,
	psalt_1024	=	7
};

//macros for each timer 'object'
#define Timer0_GetFrequency()	GetFrequency_8(TIMER0_OFFSET)
#define Timer0_SetFrequency(x)	SetFrequency_8(TIMER0_OFFSET, x)
#define Timer0_GetPrescaler()	GetPrescaler_8(TIMER0_OFFSET)
#define Timer0_SetPrescaler(x)	SetPrescaler_8(TIMER0_OFFSET, x)
#define Timer0_GetTop()			GetTop_8(TIMER0_OFFSET)
#define Timer0_SetTop(x)		SetTop_8(TIMER0_OFFSET, x)
#define Timer0_Initialize()		Initialize_8(TIMER0_OFFSET)
#define Timer0_GetResolution()	GetResolution_8(TIMER0_OFFSET)

#define Timer1_GetFrequency()	GetFrequency_16()
#define Timer1_SetFrequency(x)	SetFrequency_16(x)
#define Timer1_GetPrescaler()	GetPrescaler_16()
#define Timer1_SetPrescaler(x)	SetPrescaler_16(x)
#define Timer1_GetTop()			GetTop_16()
#define Timer1_SetTop(x)		SetTop_16(x)
#define Timer1_Initialize()		Initialize_16()
#define Timer1_GetResolution()	GetResolution_16()

#define Timer2_GetFrequency()	GetFrequency_8(TIMER2_OFFSET)
#define Timer2_SetFrequency(x)	SetFrequency_8(TIMER2_OFFSET, x)
#define Timer2_GetPrescaler()	GetPrescaler_8(TIMER2_OFFSET)
#define Timer2_SetPrescaler(x)	SetPrescalerAlt_8(TIMER2_OFFSET, x)
#define Timer2_GetTop()			GetTop_8(TIMER2_OFFSET)
#define Timer2_SetTop(x)		SetTop_8(TIMER2_OFFSET, x)
#define Timer2_Initialize()		Initialize_8(TIMER2_OFFSET)
#define Timer2_GetResolution()	GetResolution_8(TIMER2_OFFSET)

#else
	#error "BTimerDefs.h only supports ATMega48, ATMega88, ATMega168, ATMega328"
#endif
#endif /* BTIMERDEFS_H_ */
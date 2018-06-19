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


#ifndef ATIMERDEFS_H_
#define ATIMERDEFS_H_

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)

//bit offset from timer 1 in the register
#define TIMER1_OFFSET	0x00
#define TIMER3_OFFSET	0x10
#define TIMER4_OFFSET	0x20
#define TIMER5_OFFSET	0xA0

//generic register-accessing macros (eliminates some branching)
#define TCCRA_16(tmr_offset)	_SFR_MEM8(0x80  + tmr_offset)
#define TCCRB_16(tmr_offset)	_SFR_MEM8(0x81  + tmr_offset)
#define TCCRC_16(tmr_offset)	_SFR_MEM8(0x82  + tmr_offset)
#define ICR_16(tmr_offset)		_SFR_MEM16(0x86 + tmr_offset)

//physical memory locations. Used in pwmWrite()
#define OCR1A_MEM	0x88
#define OCR1B_MEM	0x8A
#define OCR1C_MEM	0x8C
#define OCR3A_MEM	0x98
#define OCR3B_MEM	0x9A
#define OCR3C_MEM	0x9C
#define OCR4A_MEM	0xA8
#define OCR4B_MEM	0xAA
#define OCR4C_MEM	0xAC
#define OCR5A_MEM	0x128
#define OCR5B_MEM	0x12A
#define OCR5C_MEM	0x12C

#define OCR0A_MEM	0x47
#define OCR0B_MEM	0x48
#define OCR2A_MEM	0xB3
#define OCR2B_MEM	0xB4

#define TCCR1A_MEM	0x80
#define TCCR1B_MEM	0x81
#define TCCR1C_MEM	0x82
#define TCCR3A_MEM	0x90
#define TCCR3B_MEM	0x91
#define TCCR4A_MEM	0xA0
#define TCCR4B_MEM	0xA1
#define TCCR4C_MEM	0xA2
#define TCCR5A_MEM	0x120
#define TCCR5B_MEM	0x121
#define TCCR5C_MEM	0x122

#define TCCR0A_MEM	0x44
#define TCCR0B_MEM	0x45
#define TCCR2A_MEM	0xB0
#define TCCR2B_MEM	0xB1

#define ICR1_MEM	0x86
#define ICR3_MEM	0x96
#define ICR4_MEM	0xA6
#define ICR5_MEM	0x126

//8 bit timers
#define TIMER0_OFFSET	0x00
#define TIMER2_OFFSET	0x6C

#define TCCRA_8(tmr_offset)		_SFR_MEM8(0x44 + tmr_offset)
#define TCCRB_8(tmr_offset)		_SFR_MEM8(0x45 + tmr_offset)
#define OCRA_8(tmr_offset)		_SFR_MEM8(0x47 + tmr_offset)

static const uint16_t pscLst[] = { 0, 1, 8, 64, 256, 1024};
static const uint16_t pscLst_alt[] = {0, 1, 8, 32, 64, 128, 256, 1024};

struct TimerData //each instance is 4 bytes
{
	uint16_t	TimerTopRegLoc:		9;
	uint16_t	ChannelRegLoc:		9;
	uint16_t	PinConnectRegLoc:	9;
	uint8_t		PinConnectBits:		4;
	bool		Is16Bit:			1;
};

//4 bytes each, 18 elements, 72 Bytes total
const TimerData timer_to_pwm_data[] = {
	{0, 0, 0, 0},										//NOT_ON_TIMER
	{0, 0, 0, 0},										//TIMER0A	disabled when initialized
	{OCR0A_MEM, OCR0B_MEM, TCCR0A_MEM, COM0B1, false},	//TIMER0B
		
	{ICR1_MEM, OCR1A_MEM, TCCR1A_MEM, COM1A1, true},	//TIMER1A
	{ICR1_MEM, OCR1B_MEM, TCCR1A_MEM, COM1B1, true},	//TIMER1B	no C channel on timer 1?

	{0, 0, 0, 0, 0},									//TIMER2	
	{0, 0, 0, 0, 0},									//TIMER2A	disabled when initialized
	{OCR2A_MEM, OCR2B_MEM, TCCR2A_MEM, COM2B1, false},	//TIMER2B
		
	{ICR3_MEM, OCR3A_MEM, TCCR3A_MEM, COM3A1, true},	//TIMER3A
	{ICR3_MEM, OCR3B_MEM, TCCR3A_MEM, COM3B1, true},	//TIMER3B
	{ICR3_MEM, OCR3C_MEM, TCCR3A_MEM, COM3C1, true},	//TIMER3C
		
	{ICR4_MEM, OCR4A_MEM, TCCR4A_MEM, COM4A1, true},	//TIMER4A
	{ICR4_MEM, OCR4B_MEM, TCCR4A_MEM, COM4B1, true},	//TIMER4B
	{ICR4_MEM, OCR4C_MEM, TCCR4A_MEM, COM4C1, true},	//TIMER4C
	{0, 0, 0, 0, 0},									//TIMER4D	
		
	{ICR5_MEM, OCR5A_MEM, TCCR5A_MEM, COM5A1, true},	//TIMER5A
	{ICR5_MEM, OCR5B_MEM, TCCR5A_MEM, COM5B1, true},	//TIMER5B
	{ICR5_MEM, OCR5C_MEM, TCCR5A_MEM, COM5C1, true},	//TIMER5C
};

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
#define Timer0_GetFrequency()		GetFrequency_8(TIMER0_OFFSET)
#define Timer0_SetFrequency(x)		SetFrequency_8(TIMER0_OFFSET, x)
#define Timer0_GetPrescaler()		GetPrescaler_8(TIMER0_OFFSET)
#define Timer0_SetPrescaler(x)		SetPrescaler_8(TIMER0_OFFSET, x)
#define Timer0_GetTop()				GetTop_8(TIMER0_OFFSET)
#define Timer0_SetTop(x)			SetTop_8(TIMER0_OFFSET, x)
#define Timer0_Initialize()			Initialize_8(TIMER0_OFFSET)
#define Timer0_GetResolution()		GetResolution_8(TIMER0_OFFSET)

#define Timer1_GetFrequency()		GetFrequency_16(TIMER1_OFFSET)
#define Timer1_SetFrequency(x)		SetFrequency_16(TIMER1_OFFSET, x)
#define Timer1_GetPrescaler()		GetPrescaler_16(TIMER1_OFFSET)
#define Timer1_SetPrescaler(x)		SetPrescaler_16(TIMER1_OFFSET, x)
#define Timer1_GetTop()				GetTop_16(TIMER1_OFFSET)
#define Timer1_SetTop(x)			SetTop_16(TIMER1_OFFSET, x)
#define Timer1_Initialize()			Initialize_16(TIMER1_OFFSET)
#define Timer1_GetResolution()		GetResolution_16(TIMER1_OFFSET)

#define Timer2_GetFrequency()		GetFrequency_8(TIMER2_OFFSET)
#define Timer2_SetFrequency(x)		SetFrequency_8(TIMER2_OFFSET, x)
#define Timer2_GetPrescaler()		GetPrescaler_8(TIMER2_OFFSET)
#define Timer2_SetPrescaler(x)		SetPrescalerAlt_8(TIMER2_OFFSET, x)
#define Timer2_GetTop()				GetTop_8(TIMER2_OFFSET)
#define Timer2_SetTop(x)			SetTop_8(TIMER2_OFFSET, x)
#define Timer2_Initialize()			Initialize_8(TIMER2_OFFSET)
#define Timer2_GetResolution()		GetResolution_8(TIMER2_OFFSET)

#define Timer3_GetFrequency()		GetFrequency_16(TIMER3_OFFSET)
#define Timer3_SetFrequency(x)		SetFrequency_16(TIMER3_OFFSET, x)
#define Timer3_GetPrescaler()		GetPrescaler_16(TIMER3_OFFSET)
#define Timer3_SetPrescaler(x)		SetPrescaler_16(TIMER3_OFFSET, x)
#define Timer3_GetTop()				GetTop_16(TIMER3_OFFSET)
#define Timer3_SetTop(x)			SetTop_16(TIMER3_OFFSET, x)
#define Timer3_Initialize()			Initialize_16(TIMER3_OFFSET)
#define Timer3_GetResolution()		GetResolution_16(TIMER3_OFFSET)

#define Timer4_GetFrequency()		GetFrequency_16(TIMER4_OFFSET)
#define Timer4_SetFrequency(x)		SetFrequency_16(TIMER4_OFFSET, x)
#define Timer4_GetPrescaler()		GetPrescaler_16(TIMER4_OFFSET)
#define Timer4_SetPrescaler(x)		SetPrescaler_16(TIMER4_OFFSET, x)
#define Timer4_GetTop()				GetTop_16(TIMER4_OFFSET)
#define Timer4_SetTop(x)			SetTop_16(TIMER4_OFFSET, x)
#define Timer4_Initialize()			Initialize_16(TIMER4_OFFSET)
#define Timer4_GetResolution(x)		GetResolution_16(TIMER4_OFFSET)

#define Timer5_GetFrequency()		GetFrequency_16(TIMER5_OFFSET)
#define Timer5_SetFrequency(x)		SetFrequency_16(TIMER5_OFFSET, x)
#define Timer5_GetPrescaler()		GetPrescaler_16(TIMER5_OFFSET)
#define Timer5_SetPrescaler(x)		SetPrescaler_16(TIMER5_OFFSET, x)
#define Timer5_GetTop()				GetTop_16(TIMER5_OFFSET)
#define Timer5_SetTop(x)			SetTop_16(TIMER5_OFFSET, x)
#define Timer5_Initialize()			Initialize_16(TIMER5_OFFSET)
#define Timer5_GetResolution()		GetResolution_16(TIMER5_OFFSET)

#else
	#error "ATimerDefs.h only supports ATMega640, ATMega1280, ATMega1281, ATMega2560, and ATMega2561"
#endif
#endif /* ATIMERDEFS_H_ */
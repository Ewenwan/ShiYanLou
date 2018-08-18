#ifndef FlexiTimer2_h
#define FlexiTimer2_h

#ifdef __AVR__
#include <avr/interrupt.h>
#else
#error FlexiTimer2 library only works on AVR architecture
#endif


namespace FlexiTimer2 {
	extern unsigned long time_units;
	extern void (*func)();
	extern volatile unsigned long count;
	extern volatile char overflowing;
	extern volatile unsigned int tcnt2;
	
	void set(unsigned long ms, void (*f)());
	void set(unsigned long units, double resolution, void (*f)());
	void start();
	void stop();
	void _overflow();
}

#endif

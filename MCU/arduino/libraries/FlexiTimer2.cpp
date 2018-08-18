/*
  FlexiTimer2.h - Using timer2 with a configurable resolution
  Wim Leers <work@wimleers.com>

  Based on MsTimer2
  Javier Valencia <javiervalencia80@gmail.com>

  History:
    16/Dec/2011 - Added Teensy/Teensy++ support (bperrybap)
		   note: teensy uses timer4 instead of timer2
    25/April/10 - Based on MsTimer2 V0.5 (from 29/May/09)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <FlexiTimer2.h>

unsigned long FlexiTimer2::time_units;
void (*FlexiTimer2::func)();
volatile unsigned long FlexiTimer2::count;
volatile char FlexiTimer2::overflowing;
volatile unsigned int FlexiTimer2::tcnt2;

void FlexiTimer2::set(unsigned long ms, void (*f)()) {
    FlexiTimer2::set(ms, 0.001, f);
}


/**
 * @param resolution
 *   0.001 implies a 1 ms (1/1000s = 0.001s = 1ms) resolution. Therefore,
 *   0.0005 implies a 0.5 ms (1/2000s) resolution. And so on.
 */
void FlexiTimer2::set(unsigned long units, double resolution, void (*f)()) {
	float prescaler = 0.0;
	
	if (units == 0)
		time_units = 1;
	else
		time_units = units;
		
	func = f;
	
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	TIMSK2 &= ~(1<<TOIE2);
	TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
	TCCR2B &= ~(1<<WGM22);
	ASSR &= ~(1<<AS2);
	TIMSK2 &= ~(1<<OCIE2A);
	
	if ((F_CPU >= 1000000UL) && (F_CPU <= 16000000UL)) {	// prescaler set to 64
		TCCR2B |= (1<<CS22);
		TCCR2B &= ~((1<<CS21) | (1<<CS20));
		prescaler = 64.0;
	} else if (F_CPU < 1000000UL) {	// prescaler set to 8
		TCCR2B |= (1<<CS21);
		TCCR2B &= ~((1<<CS22) | (1<<CS20));
		prescaler = 8.0;
	} else { // F_CPU > 16Mhz, prescaler set to 128
		TCCR2B |= ((1<<CS22) | (1<<CS20));
		TCCR2B &= ~(1<<CS21);
		prescaler = 128.0;
	}
#elif defined (__AVR_ATmega8__)
	TIMSK &= ~(1<<TOIE2);
	TCCR2 &= ~((1<<WGM21) | (1<<WGM20));
	TIMSK &= ~(1<<OCIE2);
	ASSR &= ~(1<<AS2);
	
	if ((F_CPU >= 1000000UL) && (F_CPU <= 16000000UL)) {	// prescaler set to 64
		TCCR2 |= (1<<CS22);
		TCCR2 &= ~((1<<CS21) | (1<<CS20));
		prescaler = 64.0;
	} else if (F_CPU < 1000000UL) {	// prescaler set to 8
		TCCR2 |= (1<<CS21);
		TCCR2 &= ~((1<<CS22) | (1<<CS20));
		prescaler = 8.0;
	} else { // F_CPU > 16Mhz, prescaler set to 128
		TCCR2 |= ((1<<CS22) && (1<<CS20));
		TCCR2 &= ~(1<<CS21);
		prescaler = 128.0;
	}
#elif defined (__AVR_ATmega128__)
	TIMSK &= ~(1<<TOIE2);
	TCCR2 &= ~((1<<WGM21) | (1<<WGM20));
	TIMSK &= ~(1<<OCIE2);
	
	if ((F_CPU >= 1000000UL) && (F_CPU <= 16000000UL)) {	// prescaler set to 64
		TCCR2 |= ((1<<CS21) | (1<<CS20));
		TCCR2 &= ~(1<<CS22);
		prescaler = 64.0;
	} else if (F_CPU < 1000000UL) {	// prescaler set to 8
		TCCR2 |= (1<<CS21);
		TCCR2 &= ~((1<<CS22) | (1<<CS20));
		prescaler = 8.0;
	} else { // F_CPU > 16Mhz, prescaler set to 256
		TCCR2 |= (1<<CS22);
		TCCR2 &= ~((1<<CS21) | (1<<CS20));
		prescaler = 256.0;
	}
#elif defined (__AVR_ATmega32U4__)
	TCCR4B = 0;
	TCCR4A = 0;
	TCCR4C = 0;
	TCCR4D = 0;
	TCCR4E = 0;
	if (F_CPU >= 16000000L) {
		TCCR4B = (1<<CS43) | (1<<PSR4);
		prescaler = 128.0;
	} else if (F_CPU >= 8000000L) {
		TCCR4B = (1<<CS42) | (1<<CS41) | (1<<CS40) | (1<<PSR4);
		prescaler = 64.0;
	} else if (F_CPU >= 4000000L) {
		TCCR4B = (1<<CS42) | (1<<CS41) | (1<<PSR4);
		prescaler = 32.0;
	} else if (F_CPU >= 2000000L) {
		TCCR4B = (1<<CS42) | (1<<CS40) | (1<<PSR4);
		prescaler = 16.0;
	} else if (F_CPU >= 1000000L) {
		TCCR4B = (1<<CS42) | (1<<PSR4);
		prescaler = 8.0;
	} else if (F_CPU >= 500000L) {
		TCCR4B = (1<<CS41) | (1<<CS40) | (1<<PSR4);
		prescaler = 4.0;
	} else {
		TCCR4B = (1<<CS41) | (1<<PSR4);
		prescaler = 2.0;
	}
	tcnt2 = (int)((float)F_CPU * resolution / prescaler) - 1;
	OCR4C = tcnt2;
	return;
#else
#error Unsupported CPU type
#endif
	
	tcnt2 = 256 - (int)((float)F_CPU * resolution / prescaler);
}

void FlexiTimer2::start() {
	count = 0;
	overflowing = 0;
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	TCNT2 = tcnt2;
	TIMSK2 |= (1<<TOIE2);
#elif defined (__AVR_ATmega128__)
	TCNT2 = tcnt2;
	TIMSK |= (1<<TOIE2);
#elif defined (__AVR_ATmega8__)
	TCNT2 = tcnt2;
	TIMSK |= (1<<TOIE2);
#elif defined (__AVR_ATmega32U4__)
	TIFR4 = (1<<TOV4);
	TCNT4 = 0;
	TIMSK4 = (1<<TOIE4);
#endif
}

void FlexiTimer2::stop() {
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	TIMSK2 &= ~(1<<TOIE2);
#elif defined (__AVR_ATmega128__)
	TIMSK &= ~(1<<TOIE2);
#elif defined (__AVR_ATmega8__)
	TIMSK &= ~(1<<TOIE2);
#elif defined (__AVR_ATmega32U4__)
	TIMSK4 = 0;
#endif
}

void FlexiTimer2::_overflow() {
	count += 1;
	
	if (count >= time_units && !overflowing) {
		overflowing = 1;
		count = count - time_units; // subtract time_uints to catch missed overflows
					// set to 0 if you don't want this.
		(*func)();
		overflowing = 0;
	}
}
#if defined (__AVR_ATmega32U4__)
ISR(TIMER4_OVF_vect) {
#else
ISR(TIMER2_OVF_vect) {
#endif
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	TCNT2 = FlexiTimer2::tcnt2;
#elif defined (__AVR_ATmega128__)
	TCNT2 = FlexiTimer2::tcnt2;
#elif defined (__AVR_ATmega8__)
	TCNT2 = FlexiTimer2::tcnt2;
#elif defined (__AVR_ATmega32U4__)
	// not necessary on 32u4's high speed timer4
#endif
	FlexiTimer2::_overflow();
}


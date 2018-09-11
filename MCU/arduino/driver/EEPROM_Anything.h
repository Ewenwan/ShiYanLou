/*
 * Class for simply use EEPROM in Arduino/avr microcontrollers
 * EEPROMAnything.h
 *
 * Created: 15.12.2013
 *  Author: Miro Bozik
 *	   Web: http://mirobozik.sk/
 */ 

#ifndef EEPROMANYTHING_H_
#define EEPROMANYTHING_H_

#include <avr/eeprom.h>

/* Write to EEPROM */
#define EP_WRITE_BLOCK(address, block) eeprom_write_block((const void*)&block, (void*)address, sizeof(block))
#define EP_WRITE_BYTE(address, b) eeprom_write_byte(&address, b)

/* Read from EEPROM */
#define EP_READ_BLOCK(address, block) eeprom_read_block((void*)&(block), (void*)address, sizeof(block))
#define EP_READ_BYTE(address) eeprom_read_byte(address)


#endif /* EEPROMANYTHING_H_ */

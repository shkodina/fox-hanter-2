#ifndef MY_BOMB_USE_EEPROM_H
#define MY_BOMB_USE_EEPROM_H

#include <avr/eeprom.h>
#include "commonmakros.h"

enum EepromAddr {STARTADR = 1, SSADR = 1, MMADR = 2, HHADR = 3};

void writeInitTimeToEEPROM(char * time, char count){
	eeprom_write_block (time, STARTADR, count);
}

void readInitTimeFromEEPROM(char * time, char count){
	eeprom_read_block (time, STARTADR, count);
	for (char i = 0; i < count; i++)
		time[i] = time[i] >= 60 ? 1 : time[i]; 
}
#endif 

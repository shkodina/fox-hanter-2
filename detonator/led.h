#ifndef MY_BOMB_LED_H
#define MY_BOMB_LED_H

#include <avr/io.h>
#include "commonmakros.h"

#define LEDPORT PORTA
#define LEDDDRPORT DDRA
#define LEDPIN 5

void initLed();

inline void ledOn(){
	UPBIT(LEDPORT, LEDPIN);
}

inline void ledOFF(){
	DOWNBIT(LEDPORT, LEDPIN);
}

inline void ledTugle(){
	INVBIT(LEDPORT, LEDPIN) ;
}

void initLed(){
	LEDDDRPORT |= 1 << LEDPIN;
	ledOFF();
}

#endif 

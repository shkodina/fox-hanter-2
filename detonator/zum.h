#ifndef MY_BOMB_ZUM_H
#define MY_BOMB_ZUM_H

#include <avr/io.h>
#include "commonmakros.h"

#define ZUMPORT PORTA
#define ZUMDDRPORT DDRA
#define ZUMPIN 6

void initZum();

inline void zumOn(){
	UPBIT(ZUMPORT, ZUMPIN);
}

inline void zumOFF(){
	DOWNBIT(ZUMPORT, ZUMPIN);
}

inline void zumTugle(){
	INVBIT(ZUMPORT, ZUMPIN) ;
}

void initZum(){
	ZUMDDRPORT |= 1 << ZUMPIN;
	zumOFF();
}
#endif 

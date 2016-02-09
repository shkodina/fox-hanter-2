#ifndef MY_LIB_BOTTONS_H
#define MY_LIB_BOTTONS_H

#include "commonmakros.h"

#define BUTTONWRITEPORT PORTA
#define BUTTONREADPORT PINA
#define BUTTONCONFIGPORT DDRA

typedef struct MyBottonStruct {
	static char LONGPRESSMINTIME = 10;
	static enum ButtonsState {RELEASED = 0, PUSHED = 1};
	char bit;
	char cur_state;
	char last_state;
	char how_long_is_pressed;
} Botton;

//*********************************************************

void initButtons(Botton * bottons, char count){
	for (char i = 0; i < count)
		DOWNBIT(BUTTONCONFIGPORT, bottons[i].bit);
}

//*********************************************************

inline char buttonGetCurState(Botton * botton){
	return BITISOFF(BUTTONREADPORT, *botton.bit) ? Botton.PUSHED : Botton.RELEASED;
}

//*********************************************************


#endif
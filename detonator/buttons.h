#ifndef MY_BUTTONS_LIB_H
#define MY_BUTTONS_LIB_H

#include <avr/io.h>

#define BUTTONWRITEPORT PORTA
#define BUTTONREADPORT PINA
#define BUTTONCONFIGPORT DDRA

enum ButtonsState {RELEASED = 0, PUSHED = 1};
enum ButtonsWorkState {WPUSHED, WRELEASED, WDOUBLE, WLONGPUSH};


enum ButtonPins { BUTTONSELECT = 3, BUTTONSET = 4};
enum ButtonPosition { BPOS_SELECT = 0, BPOS_SET = 1 };
#define BUTTONSCOUNT 2

enum WorkTheame {NONE, PLUS, MINUS, BOTH, SETTIMER};

#define MAXCHECKCOUNTTONEXTPUSHED 126



void initButtons();
char buttonGetCurState(char button);
char buttonCheckOncePush(char * last_state, char button);
char getButtonWorkTheame();



#endif

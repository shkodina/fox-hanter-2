#ifndef MY_SEVEN_SEG_H
#define MY_SEVEN_SEG_H

#define SEVENSEGPORT PORTD
#define SEVENSEGCONFIGPORT DDRD
#define ENABLESEVENSEGPORT PORTC
#define ENABLESEVENSEGCONFIGPORT DDRC

enum Sleeps {SLEEPINSEVENSEG = 100, SLEEPINBLINK = 50000};
enum InBlinkState {BLINKOFF = 0, BLINKON = 1};

void initSevenSeg();
void sevenSegShowCode(char number, char code );
void sevenSegShowArrayCode(char * codes, char len );
void sevenSegShowDigit(char * digits, char len, char point_mask, char blink_mask);
void sevenSegShowGTimer(char * g_timer_time, char len, char blink_mask);
void sevenSegShowGTimerTime (Timer * g_timer);
void sevenSegShowGTimerInit (Timer * g_timer, char blink_pos);


#endif

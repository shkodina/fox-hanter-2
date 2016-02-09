#ifndef MY_SEVEN_SEG_H
#define MY_SEVEN_SEG_H

#define SEVENSEGPORT PORTD
#define SEVENSEGCONFIGPORT DDRD
#define ENABLESEVENSEGPORT PORTC
#define ENABLESEVENSEGCONFIGPORT DDRC

void initSevenSegPort();
void sevenSegShowCode(char number, char code );
void sevenSegShowDigit(char * digits, char len, char point_mask, char blink_mask);
void sevenSegShowGTimer(char * g_timer_time, char len, char blink_mask);
void sevenSegShowGTimerTime (Timer * g_timer);
void sevenSegShowGTimerInit (Timer * g_timer, char blink_pos);


#endif

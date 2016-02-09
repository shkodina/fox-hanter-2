#include "timer.h"
#include "commonmakros.h"
#include "sevenseg.h"

#include <avr/io.h>
#include <avr/delay.h>


#define SEVENSEGCOUNT 6
enum SevenSegSwiching {SHL = 0, SHR = 1, SML = 2, SMR = 3, SSL = 4, SSR = 5 };
char SevenSegSwichingAr[SEVENSEGCOUNT] = {SSR, SSL, SMR, SML, SHR, SHL};


//volatile char g_seven_time_for_sevenseg[SEVENSEGCOUNT];
//volatile char g_seven_mask_for_digit_to_blink;
//volatile char g_seven_mask_for_points;

#define SEVENSEGCODSCOUNT 10
const char SevenSegCodes[SEVENSEGCODSCOUNT] = {	0b11111010,		// 0
												0b00100010, 	// 1
												0b01111100, 	// 2
												0b01110110, 	// 3
												0b10100110, 	// 4
												0b11010110, 	// 5
												0b11011110, 	// 6
												0b01100010, 	// 7
												0b11111110, 	// 8
												0b11110110 }; 	// 9
#define POINT 0b00000001												
#define NOCODE 0												



//*************************************************************************

void initSevenSeg(){

	// enable port and turn off leds on port
	SEVENSEGCONFIGPORT = 0xFF;
	SEVENSEGPORT = 0x00;
	
	// enable port and turn off modules on port
	for (char i = 0; i < SEVENSEGCOUNT; i++){
		UPBIT(ENABLESEVENSEGCONFIGPORT, SevenSegSwichingAr[i]);
		DOWNBIT(ENABLESEVENSEGPORT, SevenSegSwichingAr[i]);
	}

	
//	DDRC = 0x3f;
//	DDRD = 0xff;
}

//*************************************************************************

void sevenSegShowCode(char number, char code ){
	SEVENSEGPORT = code;
	UPBIT(ENABLESEVENSEGPORT, number);
	_delay_us(SLEEPINSEVENSEG);
	DOWNBIT(ENABLESEVENSEGPORT, number);
}

//*************************************************************************

void sevenSegShowArrayCode(char * codes, char len ){
	char count = (len > SEVENSEGCOUNT) ? SEVENSEGCOUNT : len;
	for (char i = 0; i < count; i++)
		sevenSegShowCode(i, codes[i]);
}

//*************************************************************************

void sevenSegShowDigit(char * digits, char len, char point_mask, char blink_mask){
	static char in_blink_state = BLINKOFF;
	static int times_for_switch = SLEEPINBLINK / SLEEPINSEVENSEG;
	for (char i = 0; i < len; i++){

		char code = (point_mask & (1 << i))	? (SevenSegCodes[digits[i]] | POINT) : SevenSegCodes[digits[i]] ;

		code = ((blink_mask & (1 << i)) && (in_blink_state == BLINKOFF)) ? NOCODE : code;
		
		if  (!times_for_switch--){
			times_for_switch = SLEEPINBLINK / SLEEPINSEVENSEG;
			in_blink_state = in_blink_state == BLINKOFF ? BLINKON : BLINKOFF;
		}
		
		sevenSegShowCode(SevenSegSwichingAr[i], code);
	}
}

//*************************************************************************

void sevenSegShowGTimer(char * g_timer_time, char len, char blink_mask){
	#define TIMEARRAYLEN 6 // TIMEPOSITIONSCOUNT * 2
	char point_mask = 0b0001010; // h h. m m. s s in revert order
	char time[TIMEARRAYLEN]; // 
	for (char i = 0; i < TIMEPOSITIONSCOUNT; i++){
		time[(i * 2)] = g_timer_time[i] / 10;
		time[(i * 2) + 1] = g_timer_time[i] % 10;
	}
	sevenSegShowDigit(time, TIMEARRAYLEN, point_mask, blink_mask);
}

//*************************************************************************

void sevenSegShowGTimerTime (Timer * g_timer){
	char blink_mask = 0b00000000; // no blink
	sevenSegShowGTimer(g_timer->time, TIMEPOSITIONSCOUNT, blink_mask);
}

//*************************************************************************

void sevenSegShowGTimerInit (Timer * g_timer, char blink_pos){
	char blink_mask = 0b00000000; 
	blink_mask |= (1 << (blink_pos * 2));
	blink_mask |= (1 << (blink_pos * 2 + 1));
	sevenSegShowGTimer(g_timer->time_init, TIMEPOSITIONSCOUNT, blink_mask);
}

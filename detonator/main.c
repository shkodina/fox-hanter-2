#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 1000000UL
#include <util/delay.h>

#include "led.h"
#include "timer.h"
#include "sevenseg.h"
#include "commonmakros.h"
#include "buttons.h"


//----------------------------------------------------------------

volatile Timer g_timer;

volatile unsigned int g_timeout_in_timer_setup = 0;
#define MAXTIMEOUTINTIMERSETUP 10

//----------------------------------------------------------------

void makeBoom(){
	for (int i = 0; i < 30000; i++){
		ledTugle();
		_delay_ms(50);
	}
}

//---------------------------------------------------------------

void setupTIMER1 (void)
{
     TCCR1B = (1<<CS12);
     TCNT1 = 65536-10;        
     TIMSK |= (1<<TOIE1);
}

//---------------------------------------------------------------

void setupTIMER2 (void)
{
	TIMSK &=~(1<<OCIE2 | 1<< TOIE2);	
	ASSR  = 1<<AS2;				
	TCNT2 = 0;
	TCCR2 = 5<<CS20; 
	TIMSK |= 1<< TOIE2;
}

//---------------------------------------------------------------

ISR (TIMER2_OVF_vect)
{
	cli();

	ledTugle();
	g_timeout_in_timer_setup++;

	if (g_timer.state == ON)
		if(decTimer( & g_timer))
			makeBoom();

 	sei();
}

//---------------------------------------------------------------

void workSevenSeg(){
	sevenSegShowGTimerTime(&g_timer);
}

char workButtons(){

	
	static char cur_state = NONE;
	static char cur_time_pos = HH;

	switch (getButtonWorkTheame()) {

	case SETTIMER:
		stopTimer(&g_timer);

		cur_state = SETTIMER;
		g_timeout_in_timer_setup = 0;

		sevenSegShowGTimerInit(&g_timer, cur_time_pos);

		return TRUE;
		break;


	case INCRISEVALUE:
		if (cur_state == NONE)
			g_timer.state = g_timer.state == ON ? OFF : ON;
		else{
			g_timeout_in_timer_setup = 0;
			if (g_timer.time_init[cur_time_pos]++ == TIMEMAX)
				g_timer.time_init[cur_time_pos] = TIMEMIN;
			resetTimer(&g_timer);		
			sevenSegShowGTimerInit(&g_timer, cur_time_pos);
			return TRUE;
		}
		break;


	case NEXTPOSITION:
		if (cur_state == NONE)
			resetTimer(&g_timer);
		else{
			g_timeout_in_timer_setup = 0;
			if( cur_time_pos++ == TIMEPOSITIONSCOUNT - 1)
				cur_time_pos = HH;
			sevenSegShowGTimerInit(&g_timer, cur_time_pos);
			return TRUE;
		}
		break;


	case NONE:
		if (cur_state == SETTIMER){ 
		
			sevenSegShowGTimerInit(&g_timer, cur_time_pos);

			if (g_timeout_in_timer_setup > MAXTIMEOUTINTIMERSETUP){
				cur_state = NONE;
				return FALSE;
			}

			return TRUE;
		}
			
		break;
	default:
		break;
	}

	return FALSE;
}

//-----------------------------------------------------------------

ISR (TIMER1_OVF_vect)
{
	if(!workButtons())
		workSevenSeg();


	// run timer
	TCNT1 = 65536-10; //  31220;
    TCCR1B = (1<<CS12);
    TIMSK |= (1<<TOIE1);


}


//---------------------------------------------------------------


int main(){
	


	setTimer(&g_timer, 1, 1, 8);

	initLed();
	initSevenSeg();
	initButtons();


	setupTIMER1();
	setupTIMER2();
	sei();


	char laststate = RELEASED;

	while (1) {
//		for (char i = 0; i < 6; i++)
//			sevenSegShowCode(i, 0b11110110 );
//			for (char ii = 0; ii < 8; ii++)
//				sevenSegShowCode(i, 1 << ii );
	//sevenSegShowGTimerTime(&g_timer);
	//sevenSegShowGTimerInit(&g_timer, HH);


//		if (buttonCheckOncePush(&laststate, BUTTONSELECT))
//			g_timer.state = g_timer.state == ON ? OFF : ON;
//		if (getButtonWorkTheame() == NEXTPOSITION)
//			g_timer.state = g_timer.state == ON ? OFF : ON;
/*		if (buttonGetCurState(BUTTONSET) == PUSHED)
			g_timer.state = ON;
		else
			g_timer.state = OFF;
*/
	}

}




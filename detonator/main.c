#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 1000000UL
#include <util/delay.h>

#include "led.h"
#include "zum.h"
#include "timer.h"
#include "sevenseg.h"
#include "commonmakros.h"
#include "buttons.h"
#include "rf73_spi.h"


//----------------------------------------------------------------

#define DETONATORPOINTNUMBER 2
#define POINTNUMBERPOSITION 1
#define POINTPOWERPOSITION 2

#define STATISTICCOUNT 300 // unit sent 6 packs per sec, in 300 * 10 ms it will sent 18 packs
#define PACKSTOSTOP 2
#define PACKSTOSTART 9

#define SLIDESTATISTICPARTSCOUNT 6 

volatile Timer g_timer;

volatile unsigned int g_timeout_in_timer_setup = 0;
#define MAXTIMEOUTINTIMERSETUP 10

//----------------------------------------------------------------

void makeBoom(){
	UPBIT(DDRA, 7);
	UPBIT(PORTA, 7);
	ledOFF();
	zumOFF();

	for (int i = 0; i < 600; i++){ // 600 * 50ms = 3000ms = 3s
		ledTugle();
		_delay_ms(50);
	}
	DOWNBIT(PORTA, 7);
	ledOFF();
	zumOFF();


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
	static char was_boom = 0;
	if (was_boom)
		return;

	cli();

	g_timeout_in_timer_setup++;


enum TimerSignal {NONE = 0, SOMEMORE = 1, ELAPSED = 3};


	if (g_timer.state == ON){
		switch (decTimer( & g_timer)){

			case ELAPSED: 
				makeBoom();
				was_boom = 1;
				return;
				break;

			case SOMEMORE :
				ledOn();
				zumOn();
				break;

			default:
				ledTugle();
				zumTugle();
				break;
		}	
		

	}
 	sei();
}

//---------------------------------------------------------------

void workSevenSeg(){
	sevenSegShowGTimerTime(&g_timer);
}

char workButtons(){

	if (g_timer.state == ON)
		return FALSE;	

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



void startDelayWithAnime(unsigned int wait_time){
	#define BARLEN 6
	#define BARBLOCKCOUNT 12

	char prog_bar_1 = 0b10001000,
		 prog_bar_2 = 0b00100010;

	
	char bar[BARLEN]; 

	for (char t = 0; t < 6; t++)
		bar[t] = 0;

	unsigned int periods = wait_time / BARBLOCKCOUNT;

	for (char block = 0; block < BARBLOCKCOUNT; block++){
		bar[block / 2] |= (block % 2) ? (prog_bar_2 | prog_bar_1) : prog_bar_2;

		for (unsigned int p = 0; p < periods; p++)
			for (char i = 0; i < 2; i++){
				sevenSegShowArrayCode(bar, BARLEN);

			}
	}


}


//---------------------------------------------------------------


int main(){
	
	unsigned char rx_buf[MAX_PACKET_LEN];

	Init_Spi();
	RFM73_Initialize();
	SwitchToRxMode();


	initLed();
	initZum();
	initSevenSeg();
	initButtons();

	startDelayWithAnime(3000); // initial start

	setTimer(&g_timer, 0, 12, 23); // set timer default value

	setupTIMER1();
	setupTIMER2();
	sei();
	
	char per_part_packs[SLIDESTATISTICPARTSCOUNT];
	char per_part_len = STATISTICCOUNT / SLIDESTATISTICPARTSCOUNT;
	char per_part_cout = 0;

	for (char i = 0; i < SLIDESTATISTICPARTSCOUNT; i++ )
		per_part_packs[i] = 0;

	while (1) {

		if (per_part_cout++ >= per_part_len){

			char sum_packs = 0;
			for (char i = 0; i < SLIDESTATISTICPARTSCOUNT; i++ )
				sum_packs += per_part_packs[i];
			
			if (sum_packs >= PACKSTOSTART)
				startTimer(&g_timer);

			if (sum_packs <= PACKSTOSTOP)
				stopTimer(&g_timer);

			per_part_cout = 0;

			for (char i = 0; i < SLIDESTATISTICPARTSCOUNT - 1; i++ )
				per_part_packs[i] = per_part_packs[i+1];

			per_part_packs[SLIDESTATISTICPARTSCOUNT - 1] = 0;		
		}

		UINT8 len = Receive_Packet(rx_buf, MAX_PACKET_LEN);
		if (len > 0){
			if (rx_buf[POINTNUMBERPOSITION] == DETONATORPOINTNUMBER){
				//ledTugle();
				per_part_packs[SLIDESTATISTICPARTSCOUNT - 1]++;
			}
		}
		_delay_ms(10);

	}
	

}




#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 1000000UL
#include <util/delay.h>


#include "timer.h"
#include "sevenseg.h"
#include "commonmakros.h"


#define LEDPORT PORTA
#define LEDDDRPORT DDRA
#define LEDPIN 5

//----------------------------------------------------------------

volatile Timer g_timer;

//----------------------------------------------------------------

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
	INVBIT(LEDPORT, LEDPIN) ;
	if (decTimer( & g_timer))
		for (int i = 0; i < 30000; i++){
			INVBIT(LEDPORT, LEDPIN) ;
			_delay_ms(50);
		}
 sei();
}

//---------------------------------------------------------------





int main(){
	
	LEDDDRPORT |= 1 << LEDPIN;


	setTimer(&g_timer, 1, 1, 8);

	initSevenSegPort();


	setupTIMER2();
	sei();

	while (1) {
//		for (char i = 0; i < 6; i++)
//			sevenSegShowCode(i, 0b11110110 );
//			for (char ii = 0; ii < 8; ii++)
//				sevenSegShowCode(i, 1 << ii );
	sevenSegShowGTimerTime(&g_timer);
		
	}

}




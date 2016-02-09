#include "timer.h"

void setTimer(Timer * g_timer, char hh, char mm, char ss){
	g_timer->time[SS] = g_timer->time_init[SS] = ss;
	g_timer->time[MM] = g_timer->time_init[MM] = mm;
	g_timer->time[HH] = g_timer->time_init[HH] = hh;
	g_timer->full_time_in_seconds = hh * HHCOEF + mm * MMCOEF + ss;
	g_timer->state = OFF;
}

unsigned long decTimer(Timer * g_timer){
	g_timer->full_time_in_seconds--;
	g_timer->time[HH] = g_timer->full_time_in_seconds / HHCOEF;
	g_timer->time[SS] = g_timer->full_time_in_seconds % MMCOEF;
	g_timer->time[MM] = ( g_timer->full_time_in_seconds - g_timer->time[HH] * HHCOEF ) / MMCOEF;
	return g_timer->full_time_in_seconds;
}

void resetTimer(Timer * g_timer){
	setTimer(g_timer, g_timer->time_init[HH], g_timer->time_init[MM], g_timer->time_init[SS]);
}

void startTimer(Timer * g_timer){
	g_timer->state = ON;
}

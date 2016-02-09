#include "timer.h"

void setTimer(Timer * g_timer, char hh, char mm, char ss){
	g_timer->time[SS] = g_timer->time_init[SS] = ss;
	g_timer->time[MM] = g_timer->time_init[MM] = mm;
	g_timer->time[HH] = g_timer->time_init[HH] = hh;
	g_timer->state = OFF;
}

char decTimer(Timer * g_timer){
	if (g_timer->time[SS] == 0 &&
		g_timer->time[MM] == 0 &&
		g_timer->time[HH] == 0)
		return 1;	

	if (!g_timer->time[SS]--){
		g_timer->time[SS] = TIMEMAX;
		if (!g_timer->time[MM]--){
			g_timer->time[MM] = TIMEMAX;
			if (!g_timer->time[HH]--)
				g_timer->time[HH] = TIMEMIN;
		}
	}
	
	return 0;
}

void resetTimer(Timer * g_timer){
	setTimer(g_timer, g_timer->time_init[HH], g_timer->time_init[MM], g_timer->time_init[SS]);
}

void startTimer(Timer * g_timer){
	g_timer->state = ON;
}

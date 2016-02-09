#ifndef MY_TIMER_H
#define MY_TIMER_H

enum TimerState {ON, OFF};
enum TimerBorders {MIN = 0, MAX = 59};
enum TimePosition {SS = 2, MM = 1, HH = 0 };
#define TIMEPOSITIONSCOUNT 3

#define HHCOEF 3600
#define MMCOEF 60
#define SSCOEF 1
#define TIMEPERIODCOUNT 3

typedef struct G_Timer_Struct {
char time[TIMEPOSITIONSCOUNT];
char time_init[TIMEPOSITIONSCOUNT];
unsigned long full_time_in_seconds;
char state;
} Timer;

void setTimer(Timer * timer, char hh, char mm, char ss);
unsigned long decTimer(Timer * timer);
void resetTimer(Timer * timer);
void startTimer(Timer * timer);

#endif 
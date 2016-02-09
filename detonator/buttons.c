#include "buttons.h"
#include "commonmakros.h"

//-----------------------------------------------------------------------

void initButtons(){
	// enable buttons port to read
	DOWNBIT(BUTTONCONFIGPORT, BUTTONSELECT);
	DOWNBIT(BUTTONCONFIGPORT, BUTTONSET);
}

//-----------------------------------------------------------------------

char buttonGetCurState(char button){
	return (BUTTONREADPORT & (1 << button)) == 0 ? PUSHED : RELEASED;

}

//-----------------------------------------------------------------------

char buttonCheckOncePushOnPressed(char * last_state, char button){

	if (buttonGetCurState(button) == RELEASED){ 
		*last_state = RELEASED;
		return FALSE;
	}else{
		if (*last_state == RELEASED){
				*last_state = PUSHED;
				return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------

char buttonCheckOncePushOnReleased(char * last_state, char button){

	if (buttonGetCurState(button) == RELEASED){
		if (*last_state == PUSHED){ 
			*last_state = RELEASED;
			return TRUE;
		}
	}else{
		*last_state = PUSHED;
		return FALSE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------

char buttonCheckTwoButAtOnceTimeOncePush(char * last_state, char button1, char button2){

	static char checked_count = MAXCHECKCOUNTTONEXTPUSHED;
	checked_count++;

	if (buttonGetCurState(button1) == PUSHED &&
		buttonGetCurState(button2) == PUSHED ){
		if (*last_state == RELEASED) {
			*last_state = PUSHED;
			checked_count = 0;
			return TRUE;
		}else{
			if (checked_count >= MAXCHECKCOUNTTONEXTPUSHED){
				checked_count = 0;
				return TRUE;
			}else{
				return FALSE;
			}
		}
	}

	*last_state = RELEASED;
	return FALSE;
}

//-----------------------------------------------------------------------



char getButtonWorkTheame(){
	static char last_state[BUTTONSCOUNT];
	static char last_state_of_pair_buttons = RELEASED;
	static const char but_pos[BUTTONSCOUNT] = {BPOS_SELECT, BPOS_SET};
	static const char but[BUTTONSCOUNT] = {BUTTONSELECT, BUTTONSET};

	char once_pushed_state[BUTTONSCOUNT];

	if (buttonCheckTwoButAtOnceTimeOncePush(&last_state_of_pair_buttons, BUTTONSELECT, BUTTONSET) == TRUE)
		return BOTH;


	for (char i = 0; i < BUTTONSCOUNT; i++)
		once_pushed_state[i] = buttonCheckOncePushOnReleased( last_state + but_pos[i], but[i]);

	if (once_pushed_state[BPOS_SET])
		return MINUS;

	if (once_pushed_state[BPOS_SELECT])
		return PLUS;

	return NONE;
}


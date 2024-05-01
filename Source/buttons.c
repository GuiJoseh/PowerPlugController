#include "buttons.h"
#include "time.h"
#include <8051.h>

#define up P3_0
#define right P3_1
#define enter P3_2
#define sound P3_3

char last_up = 0;
char last_right = 0;
char last_enter = 0;

void play_sound(int delay_time){
	sound = 1;
	delay(delay_time);
	sound = 0;
}

int Dpad(void){
	sound = 0;
	if(!up){
		last_up++;
		if(last_up==1 || last_up>5){
			play_sound(500);
			return UP;
		}
	}else{
		last_up = 0;
	}
	if(!right){
		last_right++;
		if(last_right==1){
			play_sound(500);
			return RIGHT;
		}
	}else{
		last_right = 0;
	}
	if(!enter){
		last_enter++;
		if(last_enter==1){
			play_sound(500);
			return ENTER;
		}else if(last_enter==20){
			play_sound(500);
			return RESET;
		}
	}else{
		last_enter = 0;
	}
	return 0;
}
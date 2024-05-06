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

//Função para emitir som, usada quando um botão é clicado para informar a identificação do clique
void buttons_play_sound(int time_delay_time){
	sound = 1;
	time_delay(time_delay_time);
	sound = 0;
}

//Função para retornar o valor de alguma constante a depender do botão clicado (também faz o tratamento de quando um botão é segurado)
int buttons_read(void){
	sound = 0;
	if(!up){
		last_up++;
		if(last_up==1 || last_up>5){
			buttons_play_sound(500);
			return UP;
		}
	}else{
		last_up = 0;
	}
	if(!right){
		last_right++;
		if(last_right==1){
			buttons_play_sound(500);
			return RIGHT;
		}
	}else{
		last_right = 0;
	}
	if(!enter){
		last_enter++;
		if(last_enter==1){
			buttons_play_sound(500);
			return ENTER;
		}else if(last_enter==20){
			buttons_play_sound(500);
			return RESET;
		}
	}else{
		last_enter = 0;
	}
	return 0;
}
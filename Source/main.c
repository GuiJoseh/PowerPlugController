#include <8051.h>
#include "I2C.h"
#include "LCD.h"
#include "DS1307.h"
#include "buttons.h"

#define TOTAL_SCREENS 8

#define power1 P1_5
#define power2 P1_6
#define power3 P1_7

char screen = 0;
void switch_screen(void){
	LCD_Command(0x01);
	screen = (screen+1)%TOTAL_SCREENS;
}

void update_power(void){
	char start_time_1 = get_saved_time(0);
	char end_time_1 = get_saved_time(1);
	char start_time_2 = get_saved_time(2);
	char end_time_2 = get_saved_time(3);
	char start_time_3 = get_saved_time(4);
	char end_time_3 = get_saved_time(5);
	char current_time = ds1307_get_byte_time();

	if(end_time_1 > start_time_1){
		if(current_time >= start_time_1 && current_time < end_time_1){
			power1 = 1;
		}else{
			power1 = 0;
		}
	}else{
		if(current_time < end_time_1 || current_time >= start_time_1){
			power1 = 1;
		}else{
			power1 = 0;
		}
	}

	if(end_time_2 > start_time_2){
		if(current_time >= start_time_2 && current_time < end_time_2){
			power2 = 1;
		}else{
			power2 = 0;
		}
	}else{
		if(current_time < end_time_2 || current_time >= start_time_2){
			power2 = 1;
		}else{
			power2 = 0;
		}
	}

	if(end_time_3 > start_time_3){
		if(current_time >= start_time_3 && current_time < end_time_3){
			power3 = 1;
		}else{
			power3 = 0;
		}
	}else{
		if(current_time < end_time_3 || current_time >= start_time_3){
			power3 = 1;
		}else{
			power3 = 0;
		}
	}
}

//Tela de mostrar a hora
void show_time(void){
	char string[9];
	LCD_Command(0x80);
	get_time_string(string);
	LCD_String(string);
	update_power();
	if(Dpad()== ENTER)
		switch_screen();
}

void update_time(int pos){
	switch(pos){
		//Alterar Hora (Soma + 1)
		case 0:
			increase_hour();
			break;
		//Alterar Minuto (Soma + 1)
		case 1:
			increase_minute();
			break;
		//Alterar Segundo (Retorna para 0)
		case 2:
			reset_seconds();
			break;
	}
}

//Tela de alterar a hora
char pos_changing = 0;
char blink = 0;
void set_time(void){
	int dpad;
	char string[9];
	pos_changing = pos_changing%3;
	blink = (blink+1)%8;
	dpad = Dpad();
	LCD_Command(0x80);
	LCD_String("Set Hour:");
	LCD_Command(0xC0);
	get_time_string(string);
	if(blink>3){
		string[3*pos_changing] = '-';
		string[3*pos_changing+1] = '-';
	}	
	LCD_String(string);
	switch(dpad){
		case RIGHT:
			pos_changing = pos_changing+1;
			break;
		case UP:
			update_time(pos_changing);
			blink = 0;
			break;
		case ENTER:
			switch_screen();
			break;
		case RESET:
			LCD_Command(0x01);
			screen = 0;
			break;
	}
}

void set_save_time(unsigned char screen_id){
	int dpad;
	char string[6];
	char screen_start[17] 	= "Power X Start\0";
	char screen_end[17] 	= "Power X End\0";
	
	dpad = Dpad();
	pos_changing = pos_changing%2;
	blink = (blink+1)%32;
	screen_start[6] = '0'+(screen_id-1)/2+1;
	screen_end[6] = '0'+(screen_id-1)/2;
	
	LCD_Command(0x80);
	if((screen_id-1)%2){
		LCD_String(screen_start);
	}
	else{
		LCD_String(screen_end);
	}
	
	LCD_Command(0xC0);
	get_saved_time_string(screen_id-2,string);
	if(blink>15){
		string[3*pos_changing] = '-';
		string[3*pos_changing+1] = '-';
	}	
	LCD_String(string);
	switch(dpad){
		case RIGHT:
			pos_changing = pos_changing+1;
			break;
		case UP:
			increase_saved_time(screen_id-2,pos_changing);
			blink = 0;
			break;
		case ENTER:
			switch_screen();
			break;
		case RESET:
			LCD_Command(0x01);
			screen = 0;
			break;
	}
}

void main(void) {
	LCD_Init();
	
	ds1307_init();
	
	while(1){
		update_power();
		switch (screen){
			case 0:
				ds1307_update_time();
				show_time();
				break;
			case 1:
				ds1307_update_time();
				set_time();
				break;
			default:
				set_save_time(screen);
				break;
		}
	}
}

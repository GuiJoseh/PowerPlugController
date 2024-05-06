#include <8051.h>
#include "I2C.h"
#include "LCD.h"
#include "DS1307.h"
#include "buttons.h"

#define TOTAL_SCREENS 8

#define POWER P1 //Saída utilizada para controle de energia
#define START_POWER_PIN 0x04 //Pino inicial do controle de energia (Nesse caso a partir do terceiro pino)

char screen = 0; //Variavel global que registra qual tela está em exibição
//Função para trocar para a próxima tela
void switch_screen(void){
	LCD_Command(0x01);
	screen = (screen+1)%TOTAL_SCREENS;
}

//Função para controlar as saídas de energia
void update_power(void){
	int i;
	char start_time, stop_time, current_time;
	current_time = DS1307_get_byte_time();
	for(i = 0; i < TOTAL_SAVED_TIMES; i++){
		//Busca os tempos de start e stop de cada saída de energia
		start_time = DS1307_get_saved_time(2*i);
		stop_time = DS1307_get_saved_time(2*i+1);
		//Aciona o pino correspondente se o for o horário definido
		if(stop_time > start_time){
			if(current_time >= start_time && current_time < stop_time){
				POWER = POWER | (START_POWER_PIN << i);
			}else{
				POWER = POWER & (0xFF - (START_POWER_PIN << i));
			}
		}else{
			if(current_time < stop_time || current_time >= start_time){
				POWER = POWER | (START_POWER_PIN << i);
			}else{
				POWER = POWER & (0xFF - (START_POWER_PIN << i));
			}
		}
	}
}

//Tela de mostrar a hora
void show_time(void){
	char string[9];
	LCD_Command(0x80); //Cursor no inicio da primeira linha
	DS1307_get_time_string(string);
	LCD_String(string);
	if(buttons_read() == ENTER)
		switch_screen();
}

//Função para modificar a hora atual no DS1307
void update_time(int pos){
	switch(pos){
		//Alterar Hora (Soma + 1)
		case 0:
			DS1307_increase_hour();
			break;
		//Alterar Minuto (Soma + 1)
		case 1:
			DS1307_increase_minute();
			break;
		//Alterar Segundo (Retorna para 0)
		case 2:
			DS1307_reset_seconds();
			break;
	}
}

//Tela de alterar a hora
char pos_changing = 0;
char blink = 0;
void set_time(void){
	int buttons_value;
	char string_time[9];
	pos_changing = pos_changing%3;
	blink = (blink+1)%8;
	buttons_value = buttons_read();
	LCD_Command(0x80); //Cursor no inicio da primeira linha
	LCD_String("Set Hour:");
	LCD_Command(0xC0); //Cursor no inicio da segunda linha
	DS1307_get_time_string(string_time);
	if(blink>3){
		string_time[3*pos_changing] = '-';
		string_time[3*pos_changing+1] = '-';
	}	
	LCD_String(string_time);
	// Toma uma ação de acordo com o botão clicado
	switch(buttons_value){
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

//Telas de alterar os tempos de start e stop das saídas de energia
void set_save_time(unsigned char screen_id){
	int buttons_value;
	char string_time[6];
	char screen_start[17] 	= "Power X Start\0";
	char screen_stop[17] 	= "Power X Stop\0";
	
	buttons_value = buttons_read();
	pos_changing = pos_changing%2;
	blink = (blink+1)%32;
	screen_start[6] = '0'+(screen_id-1)/2+1;
	screen_stop[6] = '0'+(screen_id-1)/2;
	
	LCD_Command(0x80); //Cursor no inicio da primeira linha
	if((screen_id-1)%2){
		LCD_String(screen_start);
	}
	else{
		LCD_String(screen_stop);
	}
	
	LCD_Command(0xC0); //Cursor no inicio da segunda linha
	DS1307_get_saved_time_string(screen_id-2,string_time);
	if(blink>15){
		string_time[3*pos_changing] = '-';
		string_time[3*pos_changing+1] = '-';
	}	
	LCD_String(string_time);
	// Toma uma ação de acordo com o botão clicado
	switch(buttons_value){
		case RIGHT:
			pos_changing = pos_changing+1;
			break;
		case UP:
			DS1307_increase_saved_time(screen_id-2,pos_changing);
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
	//Inicialização dos dispositivos conectados
	LCD_Init();
	DS1307_init();
	
	while(1){
		update_power();
		switch (screen){
			case 0:
				DS1307_update_time();
				show_time();
				break;
			case 1:
				DS1307_update_time();
				set_time();
				break;
			default:
				set_save_time(screen);
				break;
		}
	}
}

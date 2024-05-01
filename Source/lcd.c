#include <8051.h>
#include "time.h"

#define rs P2_0
#define en P2_2
#define rw P2_1
#define LCD_Port P2

//Controle do LCD em modo de 4 btis
void LCD_Command(char cmnd){
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); //nibble de cima
	rs = 0;
	rw = 0;
	en = 1;
	delay(1);
	en=0;
	delay(1);
	
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4); //nibble de baixo
	en = 1;
	delay(1);
	en = 0;
	delay(10);
}

void LCD_Init(void){
	delay(20);
	LCD_Command(0x02);//FunctionSet
	LCD_Command(0x2A);//FuncS
	LCD_Command(0x06);//EntryM
	LCD_Command(0x0F);
	LCD_Command(0x0C);
}


void LCD_Char(char char_data){
	LCD_Port = (LCD_Port & 0x0F) | (char_data & 0xF0);
	rs = 1;
	rw = 0;
	en = 1;
	delay(1);
	en = 0;
	delay(1);
	
	LCD_Port = (LCD_Port & 0x0F) | (char_data << 4);
	en = 1;
	delay(1);
	en = 0;
	delay(10);
}

void LCD_String(char *str){
	int i;
	for(i = 0; str[i]!=0; i++){
		LCD_Char(str[i]);
	}
}
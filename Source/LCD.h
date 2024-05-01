#ifndef LCD
#define LCD

//Controle do LCD em modo de 4 btis
void LCD_Command(char cmnd);

void LCD_Init(void);

void LCD_Char(char char_data);

void LCD_String(char *str);

#endif /* LCD */
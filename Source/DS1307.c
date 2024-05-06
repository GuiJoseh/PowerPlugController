#include "I2C.h"
#include "DS1307.h"

#define DS1307_ADDR_W 0xD0  // stopere�o de escrita do DS1307
#define DS1307_ADDR_R 0xD1  // stopere�o de leitura do DS1307

#define SEC_ADDRESS 0x00
#define MIN_ADDRESS 0x01
#define HOUR_ADDRESS 0x02

#define RAM_START_ADDRESS 0x08

unsigned char sec, min, hour;
char savedTimes[2*TOTAL_SAVED_TIMES];
char started = 0;

//Inicializa valores do array para evitar overhead de comunicação com DS1307
void DS1307_init(void){
	int i;
	for(i = 0; i < 2*TOTAL_SAVED_TIMES; i++){
		savedTimes[i] = DS1307_get_saved_time(i);
	}
	started = 1;
}

//Função para ler um endereço no DS1307
unsigned char DS1307_read_address(unsigned char addr){
	unsigned char ret;
	i2c_start();
    i2c_write(DS1307_ADDR_W);
    i2c_write(addr);  
    i2c_start();
    i2c_write(DS1307_ADDR_R);
    ret = i2c_read(0);   // Leitura de segundos (ACK)
    i2c_stop();
	return ret;
}

//Função para escrever em um endereço no DS1307
void DS1307_write_address(unsigned char addr, unsigned char dat) {
    i2c_start();
    i2c_write(DS1307_ADDR_W);
    i2c_write(addr);
    i2c_write(dat);
    i2c_stop();
}

//Função para ler os endereços relacionados ao tempo atual no DS1307
void DS1307_read_time(unsigned char *my_sec, unsigned char *my_min, unsigned char *my_hour) {
    i2c_start();
    i2c_write(DS1307_ADDR_W);
    i2c_write(0x00); 
    i2c_start();
    i2c_write(DS1307_ADDR_R);
    *my_sec = i2c_read(1);   // Leitura de segundos (ACK)
    *my_min = i2c_read(1);   // Leitura de minutos (ACK)
    *my_hour = i2c_read(0);  // Leitura de horas (NACK)
    i2c_stop();
}


void DS1307_update_time(void) {
	DS1307_read_time(&sec, &min, &hour);
}

//Função para retornar a hora atual no formato de byte (hora aproximada)
char DS1307_get_byte_time(void){
	return ((((hour & 0x30) >> 4)*10+(hour & 0x0F))*60+(((min & 0x70) >> 4)*10+(min & 0x0F)))/SAVE_TIME_STEP;
}

//Função para transforma a hora atual no formato de string
void DS1307_time_to_string(unsigned char sec,unsigned char min,unsigned char hour, char string_out[9]){
	string_out[0] = '0'+((hour & 0x30) >> 4);
	string_out[1] = '0'+(hour & 0x0F);
	string_out[2] = ':';
	string_out[3] = '0'+((min & 0x70) >> 4);
	string_out[4] = '0'+(min & 0x0F);
	string_out[5] = ':';
	string_out[6] = '0'+((sec & 0x70) >> 4);
	string_out[7] = '0'+(sec & 0x0F);
	string_out[8] = '\0';
}

//Função para retornar a hora atual no formato de string (evita parametros internos do DS1307)
void DS1307_get_time_string(char string_out[9]){
	DS1307_time_to_string(sec, min, hour, string_out);
}

//Função para incrementar a o valor salvo em um dos endereços de tempos de start ou stop das saídas de energia
void DS1307_increase_saved_time(int id, unsigned char pos){
	unsigned char time;
	time = DS1307_read_address(RAM_START_ADDRESS+id);
	switch(pos){
		case 0:
			time = time + 60/SAVE_TIME_STEP;
			break;
		case 1:
			time = (time/(60/SAVE_TIME_STEP))*(60/SAVE_TIME_STEP) + (time+1)%(60/SAVE_TIME_STEP);
			break;
	}
	time = time % (24*60/SAVE_TIME_STEP);
	DS1307_write_address(RAM_START_ADDRESS+id, time);
	savedTimes[id] = time;
}

//Função para obter a o valor salvo em um dos endereços de tempos de start ou stop das saídas de energia
unsigned char DS1307_get_saved_time(int id){
	if(started){
		return savedTimes[id];
	}
	return DS1307_read_address(RAM_START_ADDRESS+id);
}

//Função para incrementar a o valor salvo em um dos endereços de tempos de start ou stop das saídas de energia em formato de string
void DS1307_get_saved_time_string(int id, char string_out[6]){
	unsigned int time;
	time = DS1307_get_saved_time(id);
	time = time * 15;
	string_out[0] = '0'+time/600;
	string_out[1] = '0'+(time/60)%10;
	string_out[2] = ':';
	string_out[3] = '0'+(time%60)/10;
	string_out[4] = '0'+ time%10;
	string_out[5] = '\0';
}

//Função para aumentar a hora atual em 1
void DS1307_increase_hour(void){
	unsigned char aux1, aux2, new_hour;
	DS1307_update_time();
	aux1 = hour & 0x0F;
	aux2 = (hour & 0x30)>>4;
	aux1 = aux2*10 + aux1;
	aux1 = (aux1+1)%24;
	aux2 = (aux1/10)<<4;
	aux1 = (aux1%10);
	new_hour = (hour&0xC0)|(aux2&0x30)|(aux1&0x0F);
	DS1307_write_address(HOUR_ADDRESS,new_hour);
}

//Função para aumentar o minuto atual em 1
void DS1307_increase_minute(void){
	unsigned char aux1, aux2, new_min;
	DS1307_update_time();
	aux1 = min & 0x0F;
	aux2 = (min & 0x70)>>4;
	aux1 = aux2*10 + aux1;
	aux1 = (aux1+1)%60;
	aux2 = (aux1/10)<<4;
	aux1 = (aux1%10);
	new_min =  (aux2&0x70)|(aux1&0x0F);
	DS1307_write_address(MIN_ADDRESS,new_min);
}

//Função para retornar os segundos atuais para 0
void DS1307_reset_seconds(void){
	DS1307_write_address(SEC_ADDRESS,0x00);
}
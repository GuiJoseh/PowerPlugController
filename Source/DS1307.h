#ifndef DS1307
#define DS1307

void ds1307_init(void);
unsigned char ds1307_read_address(unsigned char addr);
void ds1307_write_address(unsigned char addr,unsigned char dat);
void ds1307_read_time(unsigned char *my_sec, unsigned char *my_min, unsigned char *my_hour);
void ds1307_update_time(void);
char ds1307_get_byte_time(void);
void time_to_string(unsigned char sec,unsigned char min,unsigned char hour, char string[9]);
void get_time_string(char string[9]);
void increase_saved_time(int id, unsigned char pos);
unsigned char get_saved_time(int id);
void get_saved_time_string(int id, char string_out[9]);
void increase_hour(void);
void increase_minute(void);
void reset_seconds(void);

#endif /* DS1307 */
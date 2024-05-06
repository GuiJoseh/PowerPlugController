#ifndef DS1307
#define DS1307

#define SAVE_TIME_STEP 15
#define TOTAL_SAVED_TIMES 3

void DS1307_init(void);
unsigned char DS1307_read_address(unsigned char addr);
void DS1307_write_address(unsigned char addr,unsigned char dat);
void DS1307_read_time(unsigned char *my_sec, unsigned char *my_min, unsigned char *my_hour);
void DS1307_update_time(void);
char DS1307_get_byte_time(void);
void DS1307_time_to_string(unsigned char sec,unsigned char min,unsigned char hour, char string[9]);
void DS1307_get_time_string(char string[9]);
void DS1307_increase_saved_time(int id, unsigned char pos);
unsigned char DS1307_get_saved_time(int id);
void DS1307_get_saved_time_string(int id, char string_out[9]);
void DS1307_increase_hour(void);
void DS1307_increase_minute(void);
void DS1307_reset_seconds(void);

#endif /* DS1307 */
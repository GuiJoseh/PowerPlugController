#ifndef buttons
#define buttons

	#define UP 1
	#define LEFT 2
	#define RIGHT 3
	#define DOWN 4
	#define ENTER 5
	#define RESET 6

	void buttons_play_sound(int time_delay_time);
	int buttons_read(void);

#endif /* buttons */
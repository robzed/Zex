// real_time.h
#define MAX_AWAKE 38*60*60*13

typedef struct {
int days,hours,minutes,seconds;
} real_time_struct;


void get_real_time_struct(real_time_struct* time_struct);
void update_real_time();
void draw_real_time(int x, int y, int colour);
void init_game_time();
void add_a_minute_to_real_time();
void add_an_hour_to_real_time();
void check_sleep();
int get_hours();





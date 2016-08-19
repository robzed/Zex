//zex_misc.ext
unsigned short RangedRdm( unsigned short min, unsigned short max );
unsigned short ZexRandom();
void init_zex_random();

//void print_raw_resource(ResType the_resource, int theID, int x,int y);
void user_delay(int delay_ticks);
void user_delay_no_cursor_update_required(int delay_ticks);

void num_to_string99(int value, Ptr string);
void num_to_string999(int value, Ptr string);
void show_loading_screen();
void show_text_in_loading_screen(const char* the_text);
void show_text_in_loading_screen_nocr(const char* the_text);
void show_text_in_loading_screen_fcr(Str255 the_text);
void append_text_to_loading_screen(Str255 the_text);

void show_error_in_loading_screen(Str255 the_text);
void kill_loading_screen();
//int strlen(const char *str);
void CToPascal(const char *str, unsigned char* dest_str);
void PascalToC(const unsigned char *p_str, char *c_str);

//void strcpy(char *s, char *t);
void get_system_ticks();
unsigned int  ZexTickCount();

//standardlib funcs
short rand16();	  //get short result
//oldmac//void srand(int seed);	//int rnd gen



void internal_srand(unsigned int seed);

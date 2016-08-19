//splat_control.ext


//oldmac//#include <DrawSprocket.h>
int init_screen();
//sw splat functions
//void create_blackout_window();
//void create_game_window();
//void switch_to_256();
//void create_virtual_screen();
//void start_modifying_picture();
//void stop_modifying_picture();

//dsp and our functions
void screen_quit();
void GraphicsSelectContext();
//oldmac//void GraphicsInitAttributes(DSpContextAttributes *inAttributes);
void GraphicsActive(void);
void GraphicsInactive(void);
void GraphicsPaused(void);

void Show_it();

void splat_screen();
void deferred_vbl_proc();


void set_up_game_screen_buffers(LS_CGrafPtr screen_to_set);
LS_CGrafPtr Get_back_buffer(void);

void show_and_get_next_screen();
void display_wait();
//void my_flashy_erase_back();
void red_bar();
void green_bar();
void blue_bar();
void do_down_bars();
void do_up_bars();

//show_ready();
void load_screen_splat();

//watchdog
void reset_watchdog();
void enable_watchdog();
void disable_watchdog();

int Zex_ShowCursor(int state);
void cursor_inside_of_window(bool inside_window);

// these allow per pixel translation
void set_translation_array(unsigned char* pixels_to_copy);
void enable_pixel_translation();
void disable_pixel_translation();



void set_clut_modification(int red_level, int green_level, int blue_level);


//splat_control_new.h
#include <DrawSprocket.h>
int init_screen();
DSpContextReference		mDisplayContext;
DSpContextAttributes	mDisplayAttributes;
GDHandle				mZexGDH;
CTabHandle	 	 	    mZexCLUT;
LS_CGrafPtr	backBuff;


int init_screen();
//sw splat functions
//void create_blackout_window();
//void create_game_window();
//void switch_to_256();
//void create_virtual_screen();
//void start_modifying_picture();
//void stop_modifying_picture();

//old dsp functions
void screen_quit();
void GraphicsSelectContext();
void GraphicsInitAttributes(DSpContextAttributes *inAttributes);
void GraphicsActive(void);
void GraphicsInactive(void);
void GraphicsPaused(void);

void Show_it();


LS_CGrafPtr Get_back_buffer(void);
void show_and_get_next_screen();
void display_wait();


void switch_to_256();
void create_blackout_window();
void create_game_window();
void create_virtual_screen();
void start_modifying_picture();
void stop_modifying_picture();








//gamr pause
//SB 190199
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <mactypes.h>
//oldmac//#include <files.h>
//oldmac//#include <events.h>
//oldmac//#include <timer.h>

#include <memory.h>
#include "game_defs.h"
#include "file_io.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"
#include "zex_misc.h"
#include "3d_structures_def.h"
#include "ship_control.h"
#include "input_control.h"
#include "game_pause.h"
#include "object_control_top.h"
#include "camera.h"
#include "dlp.h"
#include "CPanel.h"
#include "handle_level.h"
#include "Error_handler.h"
#include "main_sel_screen.h"

//proto
void render_frame(int width, int height);
int handle_message_mouse(LSRAW* button_down, LSRAW* button_up);
void setup_screenQS(LSRAW* the_picture, LSRAW* the_button);
int handle_pause_mouseQS(LSRAW* button_down, LSRAW* button_up);
int handle_diff_mouse(LSRAW* button_down, LSRAW* button_up);
int handle_train_mouse(LSRAW* button_down, LSRAW* button_up);

static int button_y1_array[3] = 
    {10,47,82};

int quicksave_available,execute_quicksave;
extern int Use_Mode320;
extern int Mode320,game_running;
extern unsigned int last_pause_frame;
extern _3D light_normal;
extern int zex_alive,death_frame_counter;
extern int freeze,ask_for_load,done_load_flag,user_abort;

void do_game_pause()
{
extern FSSpec Zex_FSSpec;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int thrust;
extern int tutorial;

_3D light_normal_save;

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//extern LS_CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
int load_result,wait_flag,player_click;
    light_normal_save=light_normal;
 
 
if (level_can_quicksave()==1)
{

    prefs_screen_H=GetZexPicture ('RCZ ',145);	//in_game_prefs.rcz with QS     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"in_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
	//Get_back_buffer();
    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
    setup_screenQS(prefs_screen,button);
Show_it();	//splat 3d portion of window
    

    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
    setup_screenQS(prefs_screen,button);
Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    LS_ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
     //Get_back_buffer();
     	     reset_watchdog();

     player_click=handle_pause_mouseQS(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     freeze=0;	//unfreeze engine
     }
    else
     if (player_click==2)	//quicksave
        {
        do_level_quicksave();
        quicksave_available=1;
        wait_flag=1;	//continue		
        freeze=0;	//unfreeze engine

        }
        
    else
     if (player_click==3)	//abort - leave engine frozen
        {
        wait_flag=1;
        zex_alive=0;
        user_abort=1;
        death_frame_counter=1;
        }

    }

    if (tutorial==0) LS_HideCursor();
    LS_DisposeHandle(prefs_screen_H);
    LS_DisposeHandle(button_data_H);
    LS_DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting


}
else
{ 
    prefs_screen_H=GetZexPicture ('RCZ ',139);	//in_game_prefs.rcz     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"in_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

/*	Rob's scrap of code that's going to become the pause resolution switch...
		
	Handle checkbox_off_data_H;
	LSRAW *checkbox_off;
	Handle checkbox_on_data_H;
	LSRAW *checkbox_on;
	
	checkbox_off_data_H=GetZexPicture ('RCZ ',141);	//prefs_checkbox off   
	LS_HLock(checkbox_off_data_H);
	checkbox_off = (LSRAW*)*checkbox_off_data_H;
	
    checkbox_on_data_H=GetZexPicture ('RCZ ',142);	//prefs_checkbox on   
	LS_HLock(checkbox_on_data_H);
	checkbox_on = (LSRAW*)*checkbox_on_data_H;     
*/
	
	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
	//Get_back_buffer();
    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of window
    

    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    LS_ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
     //Get_back_buffer();
     	     reset_watchdog();

     player_click=handle_pause_mouse(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     freeze=0;	//unfreeze engine
     }
    else
     if (player_click==2)	//abort - leave engine frozen
        {
        wait_flag=1;
        zex_alive=0;
        user_abort=1;
        death_frame_counter=1;
        }
    }

    if (tutorial==0) LS_HideCursor();
//    LS_HideCursor();
    LS_DisposeHandle(prefs_screen_H);
    LS_DisposeHandle(button_data_H);
    LS_DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting
 }	//end of cant quicksave
}

int do_game_restore()
{
extern FSSpec Zex_FSSpec;
extern int zex_alive,death_frame_counter;
extern _3D light_normal;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int thrust;
extern int freeze,ask_for_load,done_load_flag,user_abort;

_3D light_normal_save;

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//extern LS_CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
int load_result,wait_flag,player_click,return_val;
    light_normal_save=light_normal;
 
 
 
    prefs_screen_H=GetZexPicture ('RCZ ',146);	//restore/quit     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"in_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
	//Get_back_buffer();
    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of window
    

    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
    setup_screen(prefs_screen,button);
Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    LS_ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
     //Get_back_buffer();
     	     reset_watchdog();

     player_click=handle_pause_mouse(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     return_val=1;	//yes do restore
     }
    else
     if (player_click==2)	//abort - leave engine frozen
        {
        wait_flag=1;
        return_val=0;
        }
    }

    LS_HideCursor();
    LS_DisposeHandle(prefs_screen_H);
    LS_DisposeHandle(button_data_H);
    LS_DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting
return return_val;
}




void do_game_dead()
{
extern FSSpec Zex_FSSpec;
extern int zex_alive,death_frame_counter;
extern _3D light_normal;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int thrust;
extern int freeze,showing_stats;
extern	DynObjectsFixedSize *ocb_ptr;


_3D light_normal_save;
//LS_UnsignedWide start_micros,end_micros;
do_fixed_frame_rate(FRAME_RATE_REINIT);

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//LS_CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
int button_state;


    light_normal_save=light_normal;
  
    prefs_screen_H=GetZexPicture ('RCZ ',2009);	//deaded.rcz     
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	

	light_normal.z=1.3;	//dim the scene
showing_stats=1;	//stop everything firing
     freeze=1;	 //in 3d_top   
    if (Mode320==1)
    {
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    print_crunched_trans_pict(prefs_screen,10,10);
    }
    else
    {
    render_frame(640,390);	//in this file - calls all the needed 3d stuff
    print_crunched_trans_pict(prefs_screen,190,100);
    }
    Show_it();
    
    if (Mode320==1)
    {
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    print_crunched_trans_pict(prefs_screen,10,10);
    }
    else
    {
    render_frame(640,390);	//in this file - calls all the needed 3d stuff
    print_crunched_trans_pict(prefs_screen,190,100);
    }
//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    Show_it();

//    if (Mode320==1)
//    {
//    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
//    print_crunched_trans_pict(prefs_screen,10,10);
//    }
//    else
//    {
//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
//    print_crunched_trans_pict(prefs_screen,190,100);
//    }
////    render_frame(640,390);	    //in this file - calls all the needed 3d stuff
//    Show_it();

//wait for fire to come up
    button_state=1;
	while (button_state==1) button_state=game_fire_button();
    //and down
	do_fixed_frame_rate(FRAME_RATE_REINIT);
    button_state=0;
    while (button_state==0) 
    {
    // Microseconds(&start_micros);	//equalise
	reset_watchdog();
    

     if (Mode320==1)
     {
     render_frame(320,390>>1);	 //in this file - calls all the needed 3d stuff
     print_crunched_trans_pict(prefs_screen,10,10);
     }
     else
     {
     render_frame(640,390);	 //in this file - calls all the needed 3d stuff
     print_crunched_trans_pict(prefs_screen,190,100);
     }
//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
 //  Show_it();
    run_camera_control();;
     do_object_control();
   Show_it();
   do_fixed_frame_rate(FRAME_RATE_DELAY);
     //Microseconds(&end_micros);
     //while (end_micros.lo-start_micros.lo <1000000/MAX_FRAME_RATE)	//fps
     //    Microseconds(&end_micros);

    button_state=game_fire_button();
    }
showing_stats=0;	//stop everything firing
    freeze=0;	//in 3d_top
    LS_DisposeHandle(prefs_screen_H);
    light_normal=light_normal_save;	//restore the lighting
    (*ocb_ptr).object_list[0].invincible_count=250;	//zex is invincible whilst this is >0

}


//displays a dialog with just OK
void do_game_alert(const char * the_string)
{
extern FSSpec Zex_FSSpec;
extern int zex_alive,death_frame_counter;
extern _3D light_normal;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int thrust;
extern int freeze,ask_for_load,done_load_flag,user_abort;

_3D light_normal_save;

Handle prefs_screen_H;
Handle button_data_H;
Handle button_down_data_H;
//extern LS_CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
int load_result,wait_flag,player_click;
int prefs_x,prefs_y;

    light_normal_save=light_normal;
  
    prefs_screen_H=GetZexPicture ('RCZ ',144);	//message     
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"in_game_prefs.RCZ",&the_file);
//	prefs_screen_H=read_file(the_file);
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',134);	//prefs_button_down1   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

	light_normal.z=1.3;	//dim the scene
    freeze=1;	//in 3d_top
	//Get_back_buffer();
    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff
    
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
//    setup_screen(prefs_screen,button);
 if (Mode320==1)
 {
    prefs_x=70; prefs_y=10;
 }
 else
 {
    prefs_x=250; prefs_y=100;
 }


//    prefs_x=250; prefs_y=100;
    
	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);
//	print_crunched_trans_pict(button,0+prefs_x, button_y1_array[0]+prefs_y);

   Show_it();	//splat 3d portion of window
    

    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
//    setup_screen(prefs_screen,button);
 if (Mode320==1)
 {
    prefs_x=70; prefs_y=10;
 }
 else
 {
    prefs_x=250; prefs_y=100;
 }

	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);

    Show_it();	//splat 3d portion of wind

//¥¥¥main title screen display loop
    LS_ShowCursor();
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
     //Get_back_buffer();
     	     reset_watchdog();

     player_click=handle_message_mouse(button_down,button);
     Show_it();	//splat 3d portion of window
     if (player_click==1) 
     {
     wait_flag=1;	 //continue		 
     freeze=0;	//unfreeze engine
     }
    }

    LS_HideCursor();
    LS_DisposeHandle(prefs_screen_H);
    LS_DisposeHandle(button_data_H);
    LS_DisposeHandle(button_down_data_H);
    
//    enable_inputs();
    light_normal=light_normal_save;	//restore the lighting
}



void setup_screen(LSRAW* the_picture, LSRAW* the_button)
{

int prefs_x,prefs_y;
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=250; prefs_y=100;
}


//    prefs_x=250; prefs_y=100;
    
	print_crunched_trans_pict(the_picture,prefs_x,prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x, button_y1_array[0]+prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x, button_y1_array[1]+prefs_y);
//	print_crunched_trans_pict(the_button,0+prefs_x, button_y1_array[2]+prefs_y);	//load gam removed - SB 4/4/99

//	Show_it();

}


void setup_screenQS(LSRAW* the_picture, LSRAW* the_button)
{

int prefs_x,prefs_y;
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=100; prefs_y=100;
}


//    prefs_x=250; prefs_y=100;
    
	print_crunched_trans_pict(the_picture,prefs_x,prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x+80, button_y1_array[0]+prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x+80, button_y1_array[1]+prefs_y);
	print_crunched_trans_pict(the_button,0+prefs_x+80, button_y1_array[2]+prefs_y);	//load gam removed - SB 4/4/99

//	Show_it();

}


int handle_pause_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;

//oldmac//LS_CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
extern int quit;
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=250; prefs_y=100;
}
//oldmac//	the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);

//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
    
    if (Mode320==1) mouse_xy.v-=80;
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<2;i++)
        print_crunched_trans_pict(button_up,prefs_x,button_y1_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<2;i++)
    {
     if (mouse_xy.v>button_y1_array[i]+prefs_y && mouse_xy.v<button_y1_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+0,button_y1_array[i]+prefs_y);
     return_val=i+1;
     }
    }
//check for keyboard shortcuts
//oldmac//if (check_key(0x35) && LS_TickCount60()>last_pause_frame) { real_return_val=1; 
if (IsKeyPressed(SDLK_ESCAPE) && LS_TickCount60()>last_pause_frame) { real_return_val=1; 
                     last_pause_frame=LS_TickCount60()+20;
                      goto end; }	//ESC key=resume
                      
//if (check_key(0xf)) { real_return_val=1; goto end; }	  //R key=resume
//oldmac//if (check_key(0x0)) { real_return_val=2; goto end; }	//A key
if (IsKeyPressed(SDLK_a)) { real_return_val=2; goto end; }	//A key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key



if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:


return real_return_val;
}




int handle_pause_mouseQS(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;

//oldmac//LS_CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
extern int quit;
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=100; prefs_y=100;
}
//oldmac//	the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);

//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
    
    if (Mode320==1) mouse_xy.v-=80;
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<3;i++)
        print_crunched_trans_pict(button_up,prefs_x+80,button_y1_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<3;i++)
    {
     if (mouse_xy.v>button_y1_array[i]+prefs_y && mouse_xy.v<button_y1_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+80,button_y1_array[i]+prefs_y);
     return_val=i+1;
     }
    }
//check for keyboard shortcuts
//oldmac//if (check_key(0x35) && LS_TickCount60()>last_pause_frame) { real_return_val=1; 
if (IsKeyPressed(SDLK_ESCAPE) && LS_TickCount60()>last_pause_frame) { real_return_val=1; 
                         last_pause_frame=LS_TickCount60()+20;
                      goto end; }	//ESC key=resume
                      
//if (check_key(0xf)) { real_return_val=1; goto end; }	  //R key=resume
//oldmac//if (check_key(0x0)) { real_return_val=2; goto end; }	//A key
if (IsKeyPressed(SDLK_a)) { real_return_val=2; goto end; }	//A key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key



if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:


return real_return_val;
}

int handle_message_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
#define button_x 74
#define button_y 100

//oldmac//LS_CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
extern int quit;
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=250; prefs_y=100;
}
//oldmac//	the_drawing_buffer=Get_back_buffer();

    ZexGetMouse (&mouse_xy);
    
    if (Mode320==1) mouse_xy.v-=80;
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
        print_crunched_trans_pict(button_up,prefs_x+button_x,button_y+prefs_y);
     if (mouse_xy.v>button_y+prefs_y && mouse_xy.v<button_y+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+button_x,button_y+prefs_y);
     return_val=1;
     }
    

if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;

old_button_state=button_state;

//oldmac//end:


return real_return_val;
}

// **********************************************************************************

//called from level 4 to see if we are registered and can proceed
void check_balls()
{
extern prefs gzex_prefs;

Handle prefs_screen_H;
//Handle button_data_H;
//Handle button_down_data_H;

//extern LS_CGrafPtr the_drawing_buffer;
//GDHandle	saveGDH;
//GrafPtr		savePort;

//FSSpec the_file;
LSRAW *prefs_screen;
LSRAW *button;
LSRAW *button_down;
int wait_flag,button_state;
int prefs_x,prefs_y;

Mode320=0;


if ( gzex_prefs.registered==1)	//yes
{
/*
   prefs_screen_H=GetZexPicture ('RCZ ',5001);	//registered.rcz     
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	

//	light_normal.z=1.3;	//dim the scene
//    freeze=1;	//in 3d_top
	//Get_back_buffer();

//¥¥¥main title screen display loop
    LS_ShowCursor();
    button_state=1;
    while (button_state==1) button_state=game_fire_button();
    
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
    //and down
    wait_flag=game_fire_button();
    
     //Get_back_buffer();
     	     reset_watchdog();

    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
//    do_control_panel_control();
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=120; prefs_y=60;
}


//    prefs_x=250; prefs_y=100;
    
	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);
//     do_object_control();

        Show_it();	//splat 3d portion of wind

    }

        wait_flag=1;
        
    LS_HideCursor();
    LS_DisposeHandle(prefs_screen_H);
    //LS_DisposeHandle(button_data_H);
    //LS_DisposeHandle(button_down_data_H);
*/
}
else
{
//else show not registered screen and quit game
   prefs_screen_H=GetZexPicture ('RCZ ',5000);	//no passage.rcz     
	LS_HLock(prefs_screen_H);
	prefs_screen = (LSRAW*)*prefs_screen_H;
	

	light_normal.z=1.3;	//dim the scene
//    freeze=1;	//in 3d_top
	//Get_back_buffer();

//¥¥¥main title screen display loop
    LS_ShowCursor();
    button_state=1;
    while (button_state==1) button_state=game_fire_button();
    
    wait_flag=0;
	while (wait_flag==0)	//until mouse control says play
    {
    //and down
    wait_flag=game_fire_button();
    
     //Get_back_buffer();
     	     reset_watchdog();

    if (Mode320==1)
    render_frame(320,390>>1);	//in this file - calls all the needed 3d stuff
    else
    render_frame(640,390);	//in this file - calls all the needed 3d stuff

//    render_frame(640,390);	  //in this file - calls all the needed 3d stuff
    run_dlp();
//    run_dlp_text();
    do_control_panel_control();
if (Mode320==1)
{
    prefs_x=70; prefs_y=10;
}
else
{
    prefs_x=120; prefs_y=60;
}


//    prefs_x=250; prefs_y=100;
    
	print_crunched_trans_pict(prefs_screen,prefs_x,prefs_y);
//     do_object_control();

        Show_it();	//splat 3d portion of wind

    }

        wait_flag=1;
        zex_alive=0;
        user_abort=1;
        death_frame_counter=1;
        
    LS_HideCursor();
    LS_DisposeHandle(prefs_screen_H);
    //LS_DisposeHandle(button_data_H);
    //LS_DisposeHandle(button_down_data_H);
}
if (Use_Mode320==1) Mode320=1;

}

//sets globals int difficulty,tutorial;
void get_level()
{
extern int difficulty,tutorial,training_mission,next_level;
Handle picture_H;
LSRAW* the_picture;

Handle training_picture_H;
LSRAW* training_picture;

Handle button_data_H;
Handle button_down_data_H;
LSRAW *button;
LSRAW *button_down;
int player_click,player_clickd;


game_running=0; 
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;


 picture_H=GetZexPicture ('RCZ ',3202);	//diff pict   
 if (picture_H==0) report_error_c("Resource missing: RCZ 3202","",4);
 LS_HLock(picture_H);

 training_picture_H=GetZexPicture ('RCZ ',3204);	//train pict   
 if (training_picture_H==0) report_error_c("Resource missing: RCZ 3204","",4);
 LS_HLock(training_picture_H);
 training_picture=(LSRAW*)*training_picture_H;
     fade_main_screen();
 
 the_picture = (LSRAW*)*picture_H;
    erase_zex_rect(640,480);	//in pictures
 
 print_crunched_pict(the_picture,150,120);
 Show_it();
    erase_zex_rect(640,480);	//in pictures
 print_crunched_pict(the_picture,150,120);
 Show_it();
 
player_click=0;
while (player_click==0)
{
 player_click=handle_diff_mouse(button_down,button);

 if (player_click==1) //tutorial
 { 
//see if tutorial or training mission
    print_crunched_pict(training_picture,150,120);
    Show_it();
    erase_zex_rect(640,480);	//in pictures
    print_crunched_pict(training_picture,150,120);
    Show_it();
 
    player_clickd=0;
    while (player_clickd==0)
     {
      player_clickd=handle_train_mouse(button_down,button);
      if (player_clickd==1)
      {
      tutorial=1; 
      difficulty=16;
      training_mission=0; 
      }
      else
      if (player_clickd==2)	//training miss 1
      {
      difficulty=16;
      training_mission=1;
      }
      else
      if (player_clickd==3)	//training miss 2
      {
      difficulty=16;
      training_mission=2;
      }
//      else
//      if (player_clickd==4)	//training miss 3
//      {
//      difficulty=16;
//      training_mission=3;
//      }
      Show_it();
     }
  
 }	//end of tutorial selected
 
 else	//difficulty rating
 if (player_click==2) { game_running=0; next_level=6; tutorial=0; training_mission=0; difficulty=16;}
 else
 if (player_click==3) { game_running=0; next_level=6; tutorial=0; training_mission=0;  difficulty=8;}
 else
 if (player_click==4) { game_running=0; next_level=6; tutorial=0; training_mission=0;  difficulty=1;}
 
 Show_it();
}
 LS_DisposeHandle(button_data_H);	//Was a leak prior to 27/10/05
 LS_DisposeHandle(button_down_data_H);	//Was a leak prior to 27/10/05
 LS_DisposeHandle(picture_H);
 LS_DisposeHandle(training_picture_H);
}

#define FRIG1 27
static int button_y2_array[4] = 
    {10+FRIG1,47+FRIG1,82+FRIG1,119+FRIG1};

#define FRIG2 40
static int button_y3_array[4] = 
    {10+FRIG2,47+FRIG2,82+FRIG2,119+FRIG2};

int handle_diff_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;

//oldmac//LS_CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
extern int quit;

    prefs_x=120; prefs_y=140;
//	the_drawing_buffer=Get_back_buffer();

    ZexGetMouse (&mouse_xy);
    
    if (Mode320==1) mouse_xy.v-=80;
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<4;i++)
        print_crunched_pict(button_up,prefs_x+80,button_y2_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<4;i++)
    {
     if (mouse_xy.v>button_y2_array[i]+prefs_y && mouse_xy.v<button_y2_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+80,button_y2_array[i]+prefs_y);
     return_val=i+1;
     }
    }
                      


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;



return real_return_val;
}


int handle_train_mouse(LSRAW* button_down, LSRAW* button_up)
{
//extern	  DSpContextReference		      mDisplayContext;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;

//oldmac//LS_CGrafPtr the_drawing_buffer;

int prefs_x,prefs_y;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;
extern int quit;

    prefs_x=120; prefs_y=140;
//oldmac//	the_drawing_buffer=Get_back_buffer();

    ZexGetMouse (&mouse_xy);
    
    if (Mode320==1) mouse_xy.v-=80;
    
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<3;i++)
        print_crunched_pict(button_up,prefs_x+80,button_y3_array[i]+prefs_y);
//now draw the down one
    for (i=0; i<3;i++)
    {
     if (mouse_xy.v>button_y3_array[i]+prefs_y && mouse_xy.v<button_y3_array[i]+36+prefs_y )	//controls
     {
     print_crunched_trans_pict(button_down,prefs_x+80,button_y3_array[i]+prefs_y);
     return_val=i+1;
     }
    }
                      


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;



return real_return_val;
}




// do_intro.cpp

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <files.h>
//oldmac//#include <memory.h>
//oldmac//#include <events.h>

#include "file_io.h"
#include "do_intro.h"
#include "sound.h"
#include "drawing.h"
#include "pictures.h"
#include	"splat_control.h"
#include	"txtsplat.h"
#include "ship_control.h"
#include "zex_misc.h"
#include "input_control.h"
#include "Error_handler.h"

//do intro
void do_intro()
{

//FSSpec the_file;
Handle the_file_data_H;
LSRAW *the_picture;
extern FSSpec Zex_FSSpec;
int timer_end,wait_flag;
LS_CGrafPtr the_drawing_buffer;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int music_volume;
//oldmac//extern GDHandle				mZexGDH;

int yaccel=30;
int ypos=0;
int xpos=680;
int memsize,i,j,y_correct,x_correct,new_x,new_y;
float mag;
LSRAW* pictbuffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
int frame,movie_index,mdelay;

typedef struct
{
int the_frame,the_delay;
} stumovie;

stumovie frames [] = 
{
	{ 128,2},{200,1},{129,2},{200,1},{130,2},{200,1},{131,2},{200,1},{132,2},{200,1},{133,2},{200,1},{134,2},{200,1},
	{135,1},{200,1},{136,1},{135,1},{134,1},{133,1},{132,1},
	{131,1},{130,1},{129,1},{128,1},
	{137,2},{139,2},{140,2},{141,2},{142,3},{143,3},{144,3},{145,50},
	{146,2},{147,2},{148,2},{149,2},{150,2},{151,2},{152,2},{153,2},{154,100},
	{-1,-1},{-1,-1},{-1,-1}
};

int top,bottom;

//    goto fred;
//load and display title screen
//    enable_inputs();
//    show_ready();
    show_text_in_loading_screen("Zex: Loading Intro                                ");	   

	reset_watchdog();
	music_set_vol(music_volume);
//cache intro movie
    frame=128;
    movie_index=0;
    while (frame!=153)
    {
    frame=frames[movie_index].the_frame;
    movie_index++;
	reset_watchdog();

    the_file_data_H=GetZexPicture ('RCZM',frame);	//lsg    
    LS_DisposeHandle(the_file_data_H);
	// just for testing - we could pre-cache as a resource (you will need to increase the number
	// of handles) but it appears that this doesn't fix the interframe delay anyway
	//LS_GetResource('RCZM',frame);
    }

    kill_loading_screen();

    play_tune(129);

//play intro movie
    frame=128;
    movie_index=0;
    while (frame!=153)
    {
    if (game_fire_button())
	{
		break;
	}
	reset_watchdog();

    frame=frames[movie_index].the_frame;
    mdelay=frames[movie_index].the_delay;
    movie_index++;
    if (frame==137) simple_play_sound(2000);
    
    the_file_data_H=GetZexPicture ('RCZM',frame);	//lsg    
	the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
    //oldmac//GetPort (&savePort);
	//oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
	//oldmac//SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
    erase_zex_rect(640,480);	//in pictures
//	EraseRect(&the_drawing_buffer->portRect);
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
	the_picture = (LSRAW*)*the_file_data_H;
	print_crunched_pict_C(the_picture,0,0,80,400);
	show_and_get_next_screen();
    LS_DisposeHandle(the_file_data_H);
    user_delay_no_cursor_update_required(mdelay);		// no show_it() here...
    }
    simple_stop_sound();

//now widen the screen up  
    top=75;
    bottom=405;
    the_file_data_H=GetZexPicture ('RCZM',152);	//lsg 
	the_picture = (LSRAW*)*the_file_data_H;
    wait_flag=0;
    while (top>6 && wait_flag==0)
    {   
    if (game_fire_button()) wait_flag=1;
	reset_watchdog();

	the_drawing_buffer=Get_back_buffer();
	print_crunched_pict_C(the_picture,0,0,top,bottom);
	show_and_get_next_screen();
    top=top-2;
    bottom=bottom+2;
    user_delay(1);
    }
    LS_DisposeHandle(the_file_data_H);
  
////Show LSG
//    the_file_data_H=GetZexPicture ('RCZ ',129);	//lsg    
//	  the_drawing_buffer=Get_back_buffer();
//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
//	  the_picture = (LSRAW*)*the_file_data_H;
//	  print_crunched_pict(the_picture,30,30);
//
//
////	print_pict(the_picture,210,150);
//	   show_and_get_next_screen();
////    play_zsound(LS, sound_high_pri, twin1_chan, sound_vol_7);
//    wait_flag=0;
//   
//    timer_end=LS_TickCount60()+50;
//	  while (wait_flag==0)
//    {
//	  if (LS_TickCount60()>timer_end) wait_flag=1;
//	    if (game_fire_button()) wait_flag=1;
////	if (LS_Left_Button()) wait_flag=1;
//
//	  }
////
//	  LS_DisposeHandle(the_file_data_H);
	   
//show zex screen

    the_file_data_H=GetZexPicture ('RCZ ',130);	//zex logo   
    LS_HLock(the_file_data_H);
   	the_picture = (LSRAW*)*the_file_data_H;
//new code
//we need to decompress this picture using my ever-so-useful picture utilities
	memsize=(*the_picture).pwidth;
	memsize*=(*the_picture).pheight;
	memsize+=80;
	pictbuffer=(LSRAW*)NewPtr(memsize);
 	if (pictbuffer==0) report_error_c("Memory Error:do_intro","",-1);
 	decrunch_pict(the_picture,pictbuffer);

    music_set_vol(music_volume);
    play_tune(131);	//131 - LS theme

 	mag=4.0;
 	y_correct=0;
    x_correct=0;
 	for (i=0;i<80;i++)
 	{
		reset_watchdog();
		the_drawing_buffer=Get_back_buffer();
		erase_zex_rect(640,480);	//in pictures

		print_pict_magnified(pictbuffer,0,0,y_correct,0,mag);
		Show_it();	//show us what happened!!!
		if (game_fire_button()) 
		{
			break;
		}
		mag-=0.05;
		SDL_Delay(5); 
    }

end_zex_pict: {}
    blank_screen_now();
	DisposePtr((Ptr) pictbuffer);
	LS_DisposeHandle(the_file_data_H);

//old code
    #if 0
    yaccel=35;
    while (yaccel>0)
    {
	if (game_fire_button()) goto end_zex_pict;
     xpos-=yaccel;
	 yaccel--;
	 print_crunched_pict(the_picture,xpos,170);
	 show_and_get_next_screen();
	reset_watchdog();
     user_delay(1);	//wait 1 ticks
    }

//now up to middle of screen    
    while (yaccel<15)
    {
    	if (LS_Left_Button()) goto end_zex_pict;

     xpos+=yaccel;
	 yaccel++;
	 print_crunched_pict(the_picture,xpos,170);
	show_and_get_next_screen();
	user_delay(1);	//wait 1 ticks
	reset_watchdog();
    }
//draw (c) text
	print_crunched_pict(the_picture,xpos,170);
//     SplatTextLarge_Transparent ("! #$,-./159AFMXZ",10,355);
	 show_and_get_next_screen();
	LS_DisposeHandle(the_file_data_H);

    #endif
    
//    wait_flag=0;
//   
//    timer_end=LS_TickCount60()+60;
//	  while (wait_flag==0)
//    {
////	if (LS_Left_Button()) wait_flag=1;
//	  if (game_fire_button()) wait_flag=1;
//	  
//	  if (LS_TickCount60()>timer_end) wait_flag=1;
//	  }

    disable_inputs();

}

void blank_screen_now()
{
LS_CGrafPtr the_drawing_buffer;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
	show_and_get_next_screen();
}


void blank_screen_60()
{
unsigned int timer_end;
int wait_flag;
LS_CGrafPtr the_drawing_buffer;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;

//blank out screen for a second
	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
	show_and_get_next_screen();
    wait_flag=0;
   
    timer_end=LS_TickCount60()+50;
	while (wait_flag==0)
    {
	if (LS_TickCount60()>timer_end) wait_flag=1;
	}
}

void do_exit(void)
{

//    fade_out_music(32);

}


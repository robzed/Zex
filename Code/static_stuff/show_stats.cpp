// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2005  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// *************************************MODULE HEADER*********************************
// * FILENAME           : show_stats.c
// * MODULE TITLE       : show_stats.c
// * PROJECT            : Zex
// * DATE STARTED       : 14:21:40 on 21st June 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <NumberFormatting.h>
#include "show_stats.h"
//oldmac//#include <memory.h>
//oldmac//#include <timer.h>
//oldmac//#include <files.h>
#include "sound.h"
#include "pictures.h"
#include	"splat_control.h"
#include "3d_structures_def.h"
#include "load_objects.h"
#include "objects_controls.h"
#include "fpoly4.h"
#include "ship_control.h"
#include "input_control.h"
#include "animation.h"
#include "zex_misc.h"
#include "txtsplat.h"
#include "camera.h"
#include "game_defs.h"
#include "MeshToZex.h"
#include "compile_zmap.h"
#include "Z_init.h"
#include "Error_handler.h"
#include "object_control_top.h"
#include "zex_misc.h"
#include "hiscore.h"
#include "dlp.h"
#include "drawing.h"
#include "OGL.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int handle_stats_mouse(LSRAW* button_down, LSRAW* button_up);

int accumulated_game_time;	//from load game else 0
int start_time;	//time this session started
int stop_time;
int showing_stats;	//if 1 stops all monster controllers from firing
int show_stats_frame_counter=0;
static int reached_end_of_epilogue;
static int epilogue_colour,epilogue_colour_delta;
//credits vars
extern int real_frame_counter;				// changed from int real_frame_counter=0; - I (Rob) figure this this is better since sub-systems rely on this being common (?)

extern int user_abort;
extern const RGBColor	rgbBlack;
extern int docked_at;
extern int score,credits,start_time;
extern int do_launch;
extern int ship_z_ang;
extern int launch_timer;
extern int camera_rotation_z;	//obviously the camera rot
extern int camera_rotation_x;
extern int camera_rotation_y;

int score_this_level,credits_this_level,time_this_level,monsters_killed_this_level;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void render_frame(int width, int height);

void ShowWhyWeDied();

void setup_stats_screen(LSRAW* logo,int dock_fee);


extern char end_of_level_reason_string[512];	//we can print this if eolc is not zero
extern int	dynamic_object_index, game_over, can_do_next_level;
extern int end_of_level_code,level_has_camera_control,disable_camera_control, training_mission_complete;


void show_stats()
{
Handle button_data_H;
LSRAW *button;
Handle logo_H;
LSRAW *logo;
Handle button_down_data_H;
LSRAW *button_down;
extern Ptr screenaddr;
extern int screenwidth;

extern int gLaser;
extern int gCannon;

extern	float SinTable[360], CosTable[360];
extern const RGBColor	rgbBlack;
extern int zex_alive;
extern	DynObjectsFixedSize *ocb_ptr;
extern int level_complete,thrust;
extern int zex_alive,current_level;
//oldmac//extern DSpContextReference		mDisplayContext;
int dust_count=0;

//oldmac//LS_UnsignedWide start_micros,end_micros;

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
load_dyn_object_pb the_params;
int i,fade_percent;
extern int dock_fee,global_show_status,gShowGalacticMap;


global_show_status=0;
gShowGalacticMap=0;

ogl_inactive();

if (docked_at!=-1) credits=credits-dock_fee;

clear_all_cont_sound_chans();	//clear out all continuous channels
level_has_camera_control=0;
disable_camera_control=0;

clear_dlp();	
init_dlp();	//clear it all

show_stats_frame_counter=0;
reached_end_of_epilogue=0;
epilogue_colour=226;
epilogue_colour_delta=1;
//real_frame_counter=0;

//make zex dead so we dont get shot at
//copy zex
	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy;
	the_params.world_z=MONSTER_Z;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=270;
	the_params.controller_ref=zex;
	the_params.control_use_1=(*ocb_ptr).object_list[0].control_use_1;
	the_params.control_use_2=(*ocb_ptr).object_list[0].control_use_2;
	the_params.control_use_3=(*ocb_ptr).object_list[0].control_use_3;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=(*ocb_ptr).object_list[0].shield_value;
     the_params.has_roll=0;
	the_params.mass=100;
	the_params.dq=0;
	the_params.object_category=zex_object;
	 the_params.laser_cat=(*ocb_ptr).object_list[0].laser_category;
	 the_params.cannon_cat=(*ocb_ptr).object_list[0].cannon_category;
	 the_params.missile_cat=(*ocb_ptr).object_list[0].missile_category;
	 the_params.number_of_missiles=(*ocb_ptr).object_list[0].number_of_missiles;
	 the_params.unique_id=0;
kill(zex);

while (game_fire_button()==1);

showing_stats=1;	//stop everything firing

stop_time=LS_TickCount60();

    if (game_over==1)
    logo_H=GetZexPicture ('RCZ ',143);	//zex logo with game over over it.    
    else
    logo_H=GetZexPicture ('RCZ ',130);	//zex logo.    
    
    LS_HLock (logo_H);
    logo=(LSRAW*)*logo_H;
    

//set up drawing env.
    set_poly_clipping(0,639,0,479);	//set poly clip rect - sb 181298
    //oldmac//DSpContext_FadeGamma ( mDisplayContext, 100,0);
	LS_NormalGamma();

    kill_all_dust();
    
	do_fixed_frame_rate(FRAME_RATE_REINIT);
    while(game_fire_button()==0)
    {

      //oldmac//Microseconds(&start_micros);	 //equalise
    (*ocb_ptr).object_list[0].Omovedy=0;
    (*ocb_ptr).object_list[0].Omovedx=0;	//stop zex from moving
    thrust=0;	//just in case

	the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
    do_light_control();	//found in object_control_top

    run_camera_control();;
    render_frame(639,479);	//calls all the needed 3d stuff

//print the strings
    setup_stats_screen(logo,dock_fee);

dust_count--;
if (dust_count<0 && game_over==1)
 {
 dust_count=3;	//every 300 frames, gen a new dust
 gen_dust_for_end();
 } 

    do_object_control();
       Show_it();
    
     //oldmac//Microseconds(&end_micros);
     //oldmac//while (end_micros.lo-start_micros.lo <1000000/MAX_FRAME_RATE)	//fps
     //oldmac//    Microseconds(&end_micros);
	do_fixed_frame_rate(FRAME_RATE_DELAY);
	   
    }	//end of gcannon - mouse clicked
    
//Now if can_do_next_level is 1, we can set game over
if (can_do_next_level) 
{	
	game_over=1;
	training_mission_complete=1; 
}	//rather than docking, end game if it was a mission


//now we do a fade out
    fade_percent=100;
	do_fixed_frame_rate(FRAME_RATE_REINIT);
    while(fade_percent>10)
    {

      //oldmac//Microseconds(&start_micros);	 //equalise
     (*ocb_ptr).object_list[0].Omovedy=0;
    (*ocb_ptr).object_list[0].Omovedx=0;	//stop zex from moving
    thrust=0;	//just in case
    //oldmacDSpContext_FadeGamma ( mDisplayContext, fade_percent, 0);
	LS_FadeGamma(fade_percent, 0);
    fade_percent-=5;

	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
    do_light_control();	//found in object_control_top

    run_camera_control();
    render_frame(639,479);	//calls all the needed 3d stuff

//print the strings
//    setup_stats_screen(logo);

dust_count--;
if (dust_count<0 && game_over==1)
 {
 dust_count=8;	//every 300 frames, gen a new dust
 gen_dust_for_end();
 } 

    do_object_control();
       Show_it();
    
     //oldmac//Microseconds(&end_micros);
     //oldmac//while (end_micros.lo-start_micros.lo <1000000/MAX_FRAME_RATE)	//fps
     //oldmac//    Microseconds(&end_micros);
	do_fixed_frame_rate(FRAME_RATE_DELAY);

    }	//end of while fade-percent

//get balck screen


//	reset_objects();	
	
	
//    setup_stats_screen(button,logo);
//    setup_stats_screen(button,logo);
//    setup_stats_screen(button,logo);
//    while (handle_stats_mouse(button_down, button)==0);

    LS_DisposeHandle(logo_H);
  while (game_fire_button()==1);


	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
       Show_it();

//now we have to load zex back in
     the_params.who_spawned_this_object=-1;
	load_dyn_object(zex,0,&the_params,-1);	//the object, the position (-1=next free)

    //oldmac//DSpContext_FadeGamma ( mDisplayContext, 100, 0);
	LS_NormalGamma();
}


void ShowWhyWeDied()
{
char the_char=-1;
int count=0;
int text_x=0;

  while(the_char!=0 && count<255)
  {
    the_char=end_of_level_reason_string[count];
    count++;
  }
//calc text x - center the string
//the font is 8 pixels wide
  text_x=(count>>1)*8;
  text_x=320-text_x;
  if (text_x<0) text_x=0;

//       SplatTextLarge_Transparent (end_of_level_reason_string,120,160);
       SplatText_Transparent (end_of_level_reason_string,text_x,20,10);;
}



char epilogue[] =
"Incoming message. Callsign not known...\n\n\
And so it was that Mankind succumbed to the evil that was the R'Mparians.\n\
Children cried, women prayed and men fought but it was to no avail. Eventually the\n\
R'Mparians wore down Earth's resources. Slowly but surely over a period exceeding\n\
one hundred Earth years, as more Aliens arrived, the Human population dwindled.\n\
\nEventually, in 2112 the last few Humans were brought before the Grand R'Mparian\n\
'Zandar'. Medieval torture and gross humiliation preceded the final Human death.\n\n\
Zex had failed.";

char epilogue_win[] =
"Incoming message from Fleet Admiral M.J. Fredricks...                                             \n\n\
                                                                               \n\
 Congratulations Captain! You have saved us and planet Earth from the R'Mparians.\n\
 Unfortunately because you are, to all intents and purposes, dead we can not bring you\n\
 home but please accept our eternal thanks and gratitude!                               \n\n\
 If it's any consolation you will be given our highest award posthumously. Your wife and\n\
 child will want for nothing.\n\n\
 Until we need you again, this is Fredricks out!\n\
";

void setup_stats_screen(LSRAW* logo,int dock_fee)
{


unsigned char number_string[12];
char number_string1[12];
int alive_time,hours,mins,secs;
int i;
char line_str[256];
 char the_char;
 int linepos;
 int ypos=280;
 extern int do_next_level;
 extern int surrendered_flag;
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
	print_crunched_trans_pict(logo,160, 0);
//now print the epilogue text



if ((game_over==1 && user_abort!=1) || (surrendered_flag && game_over))	//we died for real and didn't abort OR the aliens had previously surrendered 
{
   i=0;
   next_line: linepos=0;
   copy_line:
   //copy each line then splat it
   if (surrendered_flag)
   {
    the_char=epilogue_win[i];
   }
   else
   {
    the_char=epilogue[i];
   }
    if (the_char==0)
    {
    reached_end_of_epilogue=1;
    }
    
    if (i==show_stats_frame_counter) goto eof;
    line_str[linepos]=the_char;
    i++;
    linepos++;
    
    if (the_char=='\n') goto eol;
    goto copy_line;
    
   eol:
    line_str[linepos-1]=0;	//delete the cr
    line_str[linepos]=0;
    SplatText_Transparent_Fade ((char *)line_str,0,ypos,246);
    ypos+=14;
    goto next_line;
    
   eof:
    line_str[linepos]=0;
    SplatText_Transparent_Fade ((char *)line_str,0,ypos,216);
    
    if (reached_end_of_epilogue==0)
    show_stats_frame_counter++;
        
   //	   print_crunched_trans_pict(button,260, 400);
   //	   SplatTextLarge_Transparent ("OK",320,400,255);
   //show vital stats
}

real_frame_counter++;
//epilogue_colour+=epilogue_colour_delta;
//if (epilogue_colour>230) epilogue_colour_delta=-epilogue_colour_delta;
//else
//if (epilogue_colour<226) epilogue_colour_delta=-epilogue_colour_delta;

       if (end_of_level_code!=0 && game_over==1) ShowWhyWeDied();

#define SCOREY 150
#define CREDY 190
#define TIMEY 230
//#define SCOREX 120
#define SCOREX 10
#define DATAX 120

	SplatTextLarge_Transparent ("SCORE",32,SCOREY);
	SplatTextLarge_Transparent ("CREDITS",0,CREDY);
	SplatTextLarge_Transparent ("TIME",54,TIMEY);

ZexNumToString(score,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatTextLarge_Transparent ((char *)number_string,150,SCOREY);


    ZexNumToString((int)(score-score_this_level),number_string);
    number_string[number_string[0]+1]=0;	//quick p to c hack
    number_string[0]=' ';
    SplatTextLarge_Transparent ((char *)number_string,360,SCOREY);

ZexNumToString(credits,number_string);
number_string[number_string[0]+1]=0;	//quick p to c hack
number_string[0]=' ';
SplatTextLarge_Transparent ((char *)number_string,150,CREDY);

    ZexNumToString((int)(credits-credits_this_level),number_string);
    number_string[number_string[0]+1]=0;	//quick p to c hack
    number_string[0]=' ';
    SplatTextLarge_Transparent ((char *)number_string,360,CREDY);

alive_time=(stop_time-start_time)+accumulated_game_time;

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;
#define TIMEX 173
num_to_string99(hours,number_string1);	//hours
SplatTextLarge_Transparent ((char *)number_string1,TIMEX,TIMEY);
//
	SplatTextLarge_Transparent (":",TIMEX+40,TIMEY-3);
//	  
num_to_string99(mins,number_string1);	//mins
SplatTextLarge_Transparent ((char *)number_string1,TIMEX+50,TIMEY);

	SplatTextLarge_Transparent (":",TIMEX+90,TIMEY-3);
	
num_to_string99(secs,number_string1);	//secs
SplatTextLarge_Transparent ((char *)number_string1,TIMEX+100,TIMEY);


alive_time=(stop_time-start_time);

hours=alive_time/(60*60*60);
alive_time-=hours*(60*60*60);

mins=alive_time/(60*60);
alive_time-=mins*(60*60);

secs=alive_time/60;
#undef TIMEX
#define TIMEX 384
num_to_string99(hours,number_string1);	//hours
SplatTextLarge_Transparent ((char *)number_string1,TIMEX,TIMEY);
//
	SplatTextLarge_Transparent (":",TIMEX+40,TIMEY-3);
//	  
num_to_string99(mins,number_string1);	//mins
SplatTextLarge_Transparent ((char *)number_string1,TIMEX+50,TIMEY);

	SplatTextLarge_Transparent (":",TIMEX+90,TIMEY-3);
	
num_to_string99(secs,number_string1);	//secs
SplatTextLarge_Transparent ((char *)number_string1,TIMEX+100,TIMEY);


//show this level stats
// if (can_do_next_level==1 || game_over==1)
//    {
//    SplatText_Transparent ("THIS MISSION STATS: Score: ",0,12,228);
//    ZexNumToString((int)(score-score_this_level),number_string);
//    number_string[number_string[0]+1]=0;	  //quick p to c hack
//    number_string[0]=' ';
//    SplatText_Transparent ((char *)number_string,270,12,10);
//
//    }

if (game_over==0)
{

  if (docked_at!=-1) 
  {

  
  SplatText_Transparent ("DOCKING TOLL:",248,440,33);
  //calc docking toll
    ZexNumToString((int)(dock_fee),number_string);
    number_string[number_string[0]+1]=0;	//quick p to c hack
    number_string[0]=' ';
    SplatText_Transparent ((char *)number_string,332,440,33);
  
  
  }
  if (can_do_next_level==1) { SplatText_Transparent ("MISSION COMPLETE",230,126,228);}	

}


SplatText_Transparent ("PRESS FIRE TO CONTINUE",225,460,33);


//	Show_it();

}


int handle_stats_mouse(LSRAW* button_down, LSRAW* button_up)
{
//oldmac//extern	  DSpContextReference		          mDisplayContext;

extern Ptr screenaddr;
extern int screenwidth;

LS_CGrafPtr the_drawing_buffer;


int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
unsigned char number_string[2] = {0,0};
extern int old_button_state;
extern int quit;
//copy bg for draw_bg
//	GetMouse(&mouse_xy);
	the_drawing_buffer=Get_back_buffer();
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

    ZexGetMouse (&mouse_xy);
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=LS_Left_Button();
        print_crunched_pict(button_up,260,400);
//now draw the down one
     if (mouse_xy.v>400 && mouse_xy.v<400+36 )	  //controls
     {
     print_crunched_pict(button_down,260,400);
     return_val=1;
     }
    
////check for keyboard shortcuts
if (IsKeyPressed(SDLK_RETURN)) { real_return_val=1; goto end; }	//return key
if (IsKeyPressed(SDLK_KP_ENTER)) { real_return_val=1; goto end; }	//enter key
//if (check_key(0x08)) { real_return_val=6; goto end; }	//C key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:
Show_it();
return real_return_val;
}




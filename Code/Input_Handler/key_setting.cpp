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
// * FILENAME           : key_setting.cpp
// * MODULE TITLE       : key_setting
// * PROJECT            : Zex
// * LANGUAGE           : ANSI C
// * DATE STARTED       : 20:33 on 17th December 2001
// * FIRST AUTHOR       : Rob
// *
// * COPYRIGHT (c)        LS 2001
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

#include "Error_handler.h"

#include "key_setting.h"
#include "key_setup_display.h"

#include "pictures.h"
#include "OGL.h"
#include "olg_text.h"
#include "splat_control.h"
#include "fpoly4.h"
#include "input_control.h"
#include "sound.h"
#include "txtsplat.h"

#include "load_level.h"		// for make_data_fsspec()
#include "file_io.h"		// WriteZexFile(), etc
#include "timing.h"

// ***********************************************************************************
// * CONSTANTS 
// *

#define DEBUG_MOUSE_POSITION_FRAME 0

// ***********************************************************************************
// * TYPE DEFINITIONS
// *


// ***********************************************************************************
// * DATA STRUCTURE DECLARATIONS 
// *

#define DISABLED 0	// disabled keys are missing from the game (no duplicate checking, or saving)
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7

#define SPACE 8

#define BRIGHT 16


static struct { int enabled; const char *string; int keynumber; const char *interpreter_string; } key_names[]={

// Enabled Name
	{ BRIGHT+YELLOW, "Left Turn", turn_left_key,"TRNL" },
	{ BRIGHT+YELLOW+SPACE, "Right Turn", turn_right_key,"TRNR", },
	{ BRIGHT+YELLOW, "Thrusters Up", thrust_up_key,"THRU" } ,
	{ BRIGHT+YELLOW+SPACE, "Thrusters Down", thrust_down_key,"THRD" } ,
		
	{ YELLOW, "Cannon", cannon_key,"CANN" },
	{ YELLOW, "Missile", missile_key,"MISS" },
	{ YELLOW+SPACE, "Laser", laser_key,"LASR" },

	{ YELLOW, "Retro Thruster", retro_key,"RETR" },
	{ YELLOW, "Warp Drive", warp_drive_key,"WARP" },
	{ YELLOW, "Shockwave", shockwave_key,"SHKW" },
	{ YELLOW+SPACE, "Abort Game", abort_key,"ABRT" },

	{ CYAN, "Music Volume Up", music_up_key,"MUSU" },
	{ CYAN, "Music Volume Down", music_down_key,"MUSD" },
	{ CYAN, "Sound Volume Up", sound_up_key,"SONU" },
	{ CYAN+SPACE, "Sound Volume Down", sound_down_key,"SOND" },

	{ GREEN, "Radar Range", radar_range_key,"RDAR" },
	{ GREEN, "Camera Zoom In", camera_zoom_in_key,"CMIN" },
	{ GREEN, "Comera Zoom Out", camera_zoom_out_key,"CMOT" },

	{ CYAN, "Missle Toggle", missle_toggle_key,"MTOG" },
	{ CYAN, "Show Inventory", show_inventory_key,"SHOI" },

	{ GREEN, "Eject", eject_key,"EJCT" },
	{ GREEN, "Dock", dock_request_key,"DOCK" },
	{ GREEN, "Camera Mode", camera_mode_key,"CAMM" },
	{ GREEN, "Control Panel Toggle", control_panel_key,"CPTG" },

	{ CYAN, "Velocity Limiter", velocity_limter_key,"VELL" },
	{ CYAN, "Command", command_and_control_key,"COMK" },

	{ GREEN+SPACE, "Galactic Map",galaxy_map_key,"GALM" },
	{ GREEN, "Switch Res", res_switch_key, "SRES" },
	
	{ -1, "End Marker",0,"" }
};



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

static int mx, my, mvalid=0, mkeyprogram=-1;
static Uint32 mkeyflash;
static int keys_changed_flag=0;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

int handle_key_mouse(LSRAW* button_down, LSRAW* button_up);
void print_keys(void);
bool handle_key_entry_mouse(int x1, int y1, int x2, int y2, int i);
void handle_key_entry_click(int x, int y);
void handle_actual_key_entry(void);
int find_duplicated_key(SDLKey keycode);
void accept_keys(void);
void revert_keys(void);
void load_default_keys(void);
char *get_output_key_name(SDLKey keycode);
void kwrite(char **output, const char *string);

int load_and_interpret(const char* file_name, const char* file_path);
void key_interpret(const char *string, int input_length);
void get_word(const char**input, int *in_len, const char **word, int *word_len);
void find_and_execute_word(const char**input, int *in_len, const char *word_string, int word_length);
bool interpreter_string_compare(const char *string, int len, const char *c_string);
bool convert_from_ascii_decimal(const char *string, int len, int* decimal);

#if DEBUG_MOUSE_POSITION_FRAME
	static void draw_debug_frame(int x1, int y1, int x2, int y2);
#else
	#define draw_debug_frame(a,b,c,d)			// define as blank frame
#endif
// ***********************************************************************************
// * ACTUAL CODE
// *

// +--------------------------------+-------------------------+-----------------------
// | TITLE: do_key_setting          | AUTHOR(s): Rob Probin   | DATE STARTED: 2Jan02
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

#define CONTROL_PLATE_X (340)
#define CONTROL_PLATE_Y (280)

void do_key_setting(void)
{
#if PORTABLE_FILESYSTEM
ZexPicture* picture_pic_obj;
LSRAW* the_picture;

ZexPicture* button_data_pic_obj;
ZexPicture* button_down_data_pic_obj;
#else
Handle picture_H;
LSRAW* the_picture;

Handle button_data_H;
Handle button_down_data_H;
#endif
LSRAW *button;
LSRAW *button_down;
int player_click;

// initilisation block
//freeze_physics();
init_screen_stars();

Rect saved_clipping;
get_poly_clipping(&saved_clipping);
set_poly_clipping(0,WIND_WIDTH,0,WIND_HEIGHT);	//set poly clip rect - sb 181298	
erase_zex_rect(WIND_WIDTH,WIND_HEIGHT);	//in pictures
//oldmac//DSpContext_FadeGamma ( mDisplayContext, 100, 0);	//clear any sun glare
LS_NormalGamma();

LS_ShowCursor();
mvalid=0; mkeyprogram=-1;
keys_changed_flag=0;	// initially no change


// set up secreen
    #if PORTABLE_FILESYSTEM
    button_data_pic_obj= new ZexPicture ('RCZ ',132);	//prefs_button_up   
    button = button_data_pic_obj->GetPictureRef();
	
    button_down_data_pic_obj= new ZexPicture ('RCZ ',133);	//prefs_button_down   
    button_down = button_down_data_pic_obj->GetPictureRef();

    picture_pic_obj= new ZexPicture ('RCZ ',201);	//diff pict   
    if (picture_pic_obj==0) report_error_c("Resource missing: RCZ 00200","",2047);
    the_picture = picture_pic_obj->GetPictureRef();
 
    #else

    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;


 picture_H=GetZexPicture ('RCZ ',201);	//diff pict   
 if (picture_H==0) report_error_c("Resource missing: RCZ 00200","",2047);
 LS_HLock(picture_H);

// fade_main_screen();
 
 the_picture = (LSRAW*)*picture_H;
    #endif
    
 player_click=0;

do_fixed_frame_rate(FRAME_RATE_REINIT);
while (player_click==0)
 {
 erase_zex_rect(WIND_WIDTH,WIND_HEIGHT);	 //in pictures
 show_screen_stars();

 print_keys();
 
 print_crunched_trans_pict(the_picture,CONTROL_PLATE_X+65,CONTROL_PLATE_Y+15);

 player_click=handle_key_mouse(button_down,button);

 // Button 1: Accept -- saves in forth file (if changed) & exits
 if (player_click==1) { accept_keys(); }

 else // Button 2: Cancel/Revert -- causes forth to reload from file
 if (player_click==2) { revert_keys(); player_click=0; }

 else // Button 3: Defaults -- loads hardcoded defaults as keyset
 if (player_click==3) { load_default_keys(); player_click=0; }
 
 Show_it();
 
 //thread_yield();
 //update_microsecs_timer();
 do_fixed_frame_rate(FRAME_RATE_DELAY);
 handle_actual_key_entry();
 }

#if PORTABLE_FILESYSTEM
delete picture_pic_obj;
delete button_data_pic_obj;
delete button_down_data_pic_obj;
#else	// resource memory leak?
LS_DisposeHandle(picture_H);
LS_DisposeHandle(button_down_data_H);
LS_DisposeHandle(button_data_H);
#endif

// disposal cleanup block
destroy_screen_stars();
//unfreeze_physics();
LS_HideCursor();

set_poly_clipping(saved_clipping.left, saved_clipping.right, saved_clipping.left, saved_clipping.bottom);	// restore original clipping area
} // end of do_key_setting()





// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

static int button_y2_array[4] = {0,34,68,102};

int handle_key_mouse(LSRAW* button_down, LSRAW* button_up)
{
int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i;
extern int old_button_state;

    ZexGetMouse (&mouse_xy);

    button_state=game_fire_button();
    
//first draw all buttons as off
    for (i=0; i<3;i++)
        print_crunched_trans_pict(button_up,CONTROL_PLATE_X,button_y2_array[i]+CONTROL_PLATE_Y);
//now draw the down one
    for (i=0; i<3;i++)
    {
		int top = button_y2_array[i]+CONTROL_PLATE_Y;
		int bottom = button_y2_array[i+1]+CONTROL_PLATE_Y;
		draw_debug_frame(CONTROL_PLATE_X, top, 639, bottom);
     if(mouse_xy.h > CONTROL_PLATE_X && mouse_xy.v >= top && mouse_xy.v < bottom)	//controls
     {
     print_crunched_trans_pict(button_down,CONTROL_PLATE_X,button_y2_array[i]+CONTROL_PLATE_Y);
     return_val=i+1;
     }
    }

if (button_state==1 && old_button_state==0) 	//if down and was up
  {
  handle_key_entry_click(mouse_xy.h,mouse_xy.v);
  if(return_val!=0)
    {
    play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
    }
  }
  
if (button_state==0 && old_button_state==1)	//if up and was down
  { 
    real_return_val = return_val; }
else
  { real_return_val=0; }
  
old_button_state=button_state;

return real_return_val;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

#define PK_START_X 40
#define PK_START_Y 40

#define PK_STEP_X 310
#define PK_STEP_Y 15

#define PK_COLUMN_SPACE 150
#define PK_LIMIT_Y 440

#define DIM 128

#define FLASH_TIME 250		// ms

void print_keys(void)
{
 int x,y,i;
 int enabled;
 
 x=PK_START_X; y=PK_START_Y; i=0;
 
 enabled=key_names[i].enabled;
 while(enabled != -1)
   {
   if(enabled>=1)
     {
     // we change the colour to a flashing colour if this function is selected
     if(mkeyprogram==i)
       {
	   Uint32 flash_diff = LS_GetTicks()-mkeyflash;
       if(flash_diff<FLASH_TIME)
          { enabled&=~WHITE; enabled|=RED+BRIGHT; }
       else
          { 
          if(flash_diff>(2*FLASH_TIME)) { mkeyflash=LS_GetTicks(); } // reset
          enabled|=WHITE+BRIGHT;
          }
       }
       
	   
	   draw_debug_frame(x,y,x+PK_STEP_X,y+PK_STEP_Y);
     // highlight it instantly it is clicked, to provide instant feedback
     if(handle_key_entry_mouse(x,y,x+PK_STEP_X,y+PK_STEP_Y,i))
       { enabled|=WHITE+BRIGHT; }

     // select the colour, as per the table (or overridden).
	 SDL_Surface *screen = Get_back_buffer();
     int colour;
	 
     if(enabled&BRIGHT)
       { colour = SDL_MapRGB(screen->format, (enabled&RED)?255:0, (enabled&GREEN)?255:0, (enabled&BLUE)?255:0); }
     else
       { colour = SDL_MapRGB(screen->format, (enabled&RED)?DIM:0, (enabled&GREEN)?DIM:0, (enabled&BLUE)?DIM:0); }
	 
	 
     if(mkeyprogram==i)
	 {
		 SplatText(">>>", x-30, y, colour, 255);			// marker for those with colour recognition problems
	 }
     SplatText(key_names[i].string, x, y, colour, 255);
     SplatText(SDL_GetKeyName(get_programmed_keycode(key_names[i].keynumber)), x+PK_COLUMN_SPACE, y, colour, 255);
     
     if(enabled&SPACE) y+=PK_STEP_Y;
     y+=PK_STEP_Y;
     } // end of if enabled
   if(y>=PK_LIMIT_Y) { x+=PK_STEP_X; y=PK_START_Y; }
   
   i++;
   enabled=key_names[i].enabled;
   } // end of while

if(mvalid==1) { mkeyprogram=-1; }	// unrecognised click - deselect
mvalid=0;		// make mouse no longer valid
   
} // end of print keys


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

//#define FONT_SHIFT_Y -10
//#define WINDOW_SHIFT_X -40
// no longer valid under sdl
#define FONT_SHIFT_Y 0
#define WINDOW_SHIFT_X 0

bool handle_key_entry_mouse(int x1, int y1, int x2, int y2, int i)
{
if(mvalid!=0)
  {
  if(mx>=(x1+WINDOW_SHIFT_X) && mx<(x2+WINDOW_SHIFT_X) && my>=(y1+FONT_SHIFT_Y) && my<(y2+FONT_SHIFT_Y))	// click is in this box
    {
    play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
    mvalid=0;		// can only be valid for one
    mkeyprogram=i; 	// store the chosen one
    mkeyflash=0;	// always start flash from beginning
    return true;
    }
  }
return false;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Pass a mouse click message to be handled next frame
// +----------------------------------------------------------------ROUTINE HEADER----

void handle_key_entry_click(int x, int y)
{
mvalid=1;
mx=x;
my=y;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void handle_actual_key_entry(void)
{
	SDLKey keycode;
	int duplicate_slot;
	bool single_key;
	static SDLKey rejected_keycode;	// don't play sound twice
	static bool rejected_keycode_valid = false;
	
	if(mkeyprogram!=-1)	// i.e. we want to enter a key
	{
		single_key = is_one_key_pressed(&keycode);
		if(single_key)
		{
			duplicate_slot=find_duplicated_key(keycode);
			if(duplicate_slot!=-1 && duplicate_slot!=mkeyprogram) // check if it is a repeat (but not on SAME key)
			{
				if(rejected_keycode_valid && rejected_keycode!=keycode)	// don't play sound twice
				{
					play_zsound(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
				}
				rejected_keycode=keycode;
				rejected_keycode_valid = true;
			}
			else	// not repeat key
			{
				rejected_keycode_valid = false;
				if(duplicate_slot==-1) { keys_changed_flag=1; }	// keys changed, except for same key in same slot...
				load_single_key_def(key_names[mkeyprogram].keynumber, keycode); // store in real key map
				mkeyprogram=-1;	// only a valid key cancels entry
			}
			
		}	// end of if(there is a key)
		else
		{
			rejected_keycode_valid = false;
		}
		
	} // end of if (we are in key change mode)
	
} // end of handle_actual_key_entry()


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Searches down all non-disabled keys looking for duplicate
// +----------------------------------------------------------------ROUTINE HEADER----

int find_duplicated_key(SDLKey keycode)
{
 int i;
 int enabled;
 
 i=0;
 
 enabled=key_names[i].enabled;
 while(enabled != -1)
   {
   if(enabled>=1)
     {
     if(keycode==get_programmed_keycode(key_names[i].keynumber))
       {
       return i;
       }
     } // end of if enabled   
   i++;
   enabled=key_names[i].enabled;
   } // end of while
   
return -1;	// key not found
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Accept -- saves in forth file (if changed) 
// |
// | Converted from old forth interpreter version - hence reason it looks "weird"
// +----------------------------------------------------------------ROUTINE HEADER----

const unsigned int MAX_KEY_FILE_LENGTH = 2048;
const char keys_filename[] = "keys.def";
const char keys_path[] = "P";

void accept_keys(void)
{
	if(!keys_changed_flag) { return; } // if no change, return

	const int OVERFLOW_SIZE = 1000;			// we have an overflow for writing - because of the horrid code...
	char output_string[MAX_KEY_FILE_LENGTH+OVERFLOW_SIZE];
	
	char *output=output_string;
	
	kwrite(&output,"# Zex's Current Keys - Autogenerated File\x0d\x0d");
	
	int i=0;
	int enabled=key_names[i].enabled;
	while(enabled != -1)
	{
		if(enabled>=1)
		{
			kwrite(&output,key_names[i].interpreter_string);
			kwrite(&output,"  ");
			kwrite(&output,get_output_key_name(get_programmed_keycode(key_names[i].keynumber)));
			kwrite(&output,"\x0d");
		} // end of if enabled
		
		i++;
		enabled=key_names[i].enabled;
	} // end of while
	
	unsigned int length = output - output_string;
	if(length > MAX_KEY_FILE_LENGTH) report_error_c("Zex: Key save data too long","", 0);
	
	FSSpec file;
	make_data_fsspec(keys_path, keys_filename, &file);
	i = WriteZexFile(file, output_string, length);
	if(i) report_error_c("Zex: Problem writing keys to file","", i); 
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

void kwrite(char **output, const char *string)
{
char *dest;

dest=*output;	// copy address of pointer to string for easier use

while(*string)	// while not at end of string
  {
  *dest=*string;	// copy the characters
  dest++; string++;	// and carry on
  }
  
*output=dest;	// update end address / address of next append
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:  
// +----------------------------------------------------------------ROUTINE HEADER----

static char temp_number[20];

char *get_output_key_name(SDLKey keycode)
{
	// output the decimal keycode
	sprintf(temp_number, "%i", keycode);
	return temp_number;    
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: Cancel/Revert -- causes forth to reload from file 
// +----------------------------------------------------------------ROUTINE HEADER----

void revert_keys(void)
{
	load_and_interpret(keys_path, keys_filename);		// ignore read errors
	keys_changed_flag=0;	// now definitely hasn't changed!
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void load_saved_keys(void)
{
	load_and_interpret(keys_path, keys_filename);		// ignore read errors
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s):              | DATE STARTED:
// +
// | DESCRIPTION:Defaults -- loads hardcoded defaults as keyset  
// +----------------------------------------------------------------ROUTINE HEADER----

//const char default_keys_path[] = "P";	// store them in the prefs directory for the moment
//const char default_keys_filename[] = "default_keys.def";

void load_default_keys(void)
{
	// we could load from a file - but for the moment we are not going to...
	//load_and_interpret(default_keys_path, default_keys_filename);

	set_internal_default_keys();
	keys_changed_flag=1;	// too much effort to check if really changed, so just assume
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 13-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----

int load_and_interpret(const char* file_name, const char* file_path)
{
	FSSpec file;
	make_data_fsspec(file_name, file_path, &file);
	char file_buffer[MAX_KEY_FILE_LENGTH];
	for(unsigned int i=0; i<MAX_KEY_FILE_LENGTH; i++)
	{
		file_buffer[i]=0;		// ensure zero terminated
	}
	int i = ReadZexFile(file, file_buffer, MAX_KEY_FILE_LENGTH);
	if(i) return i;
	
	key_interpret(file_buffer, MAX_KEY_FILE_LENGTH);
	
	return 0;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 13-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----

void key_interpret(const char *string, int input_length)
{
	const char *word;
	int wlen;
	
	while(input_length > 0 && *string!=0)
	{
		get_word(&string, &input_length, &word, &wlen);
		if(wlen != 0) find_and_execute_word(&string, &input_length, word, wlen);
	}
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 13-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----

void get_word(const char**input, int *in_len, const char **word, int *word_len)
{
	// remove front space
	while(**input <= 32 && **input != 0 && *in_len > 0)
	{
		(*input)++;
		(*in_len)--;
	}
	
	*word = *input;		// front of the word
	*word_len = 0;
		
	// now skip actual word (watching for end of input)
	while(**input > 32 && *in_len > 0)
	{
		(*input)++;
		(*in_len)--;
		(*word_len)++;
	}
	
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 13-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----

void find_and_execute_word(const char**input, int* in_len, const char *word_string, int word_length)
{
	const char *word;
	int wlen;
	int decimal;
	// comment line?
	if(*word_string == '#')
	{
		while(**input != 13 && **input != 10 && **input != 0 && *in_len > 0)
		{
			(*input)++;
			(*in_len)--;
		}
		return;
	}
	
	// look for keyame token
	int i=0;
	int enabled=key_names[i].enabled;
	while(enabled != -1)
	{
		if(enabled>=1)
		{
			if(interpreter_string_compare(word_string, word_length, key_names[i].interpreter_string))
			{
				get_word(input, in_len, &word, &wlen);
				if(wlen == 0) return;
				if(convert_from_ascii_decimal(word, wlen, &decimal) == false) return;
				load_single_key_def(key_names[i].keynumber, static_cast<SDLKey>(decimal));
				return;
			}
		} // end of if enabled
		
		i++;
		enabled=key_names[i].enabled;
	} // end of while
	
	// if we get here we haven't got a match - we have a syntax error
	// but we ignore errors from here...
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 13-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----
bool interpreter_string_compare(const char *string, int len, const char *c_string)
{
	while(len != 0 && *c_string != 0)
	{
		if(*string != *c_string)
		{
			return false;
		}
		string++;
		c_string++;
		
	}
	
	return true;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 13-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----

bool convert_from_ascii_decimal(const char *string, int len, int* decimal)
{
	bool neg = false;
	if(len == 0) return false;		// blank string is invalid
	
	if(*string == '-')
	{
		neg = true;
		len--;
		string++;
	}
	
	int num = 0;
	if(len == 0) return false;
	
	while(len)
	{
		if(*string < '0' && *string > '9')
		{
			return false;	// found illegal characters
		}
		
		num *= 10;				// scale by 10
		num += *string - '0';
		
		len--;
		string++;
	}
	
	if(neg) num = -num;
	
	*decimal = num;
	return true;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob          | DATE STARTED: 19-6-2005
// +
// | DESCRIPTION:
// +----------------------------------------------------------------ROUTINE HEADER----

#if DEBUG_MOUSE_POSITION_FRAME
static void draw_debug_frame(int x1, int y1, int x2, int y2)
{
	Rect r;
	set_foreground_colour(0);
	r.left = x1; 
	r.right = x2;
	r.top = y1 ;
	r.bottom = y2;
	
	FrameRect(&r);
	
}
#endif
// ***********************************************************************************
// * DESIGN NOTES
// *





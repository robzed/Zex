//hiscore.c
//SB 070299
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <mactypes.h>
//oldmac//#include <resources.h>
//oldmac//#include <NumberFormatting.h>
#include <string.h>

#include "3d_structures_def.h"
#include "objects_controls.h"
#include "fpoly4.h"
#include "load_objects.h"
#include "compile_zmap.h"
#include "zex_misc.h"

#include "ship_control.h"
#include "input_control.h"
#include "splat_control.h"
#include "animation.h"
#include "Error_handler.h"
#include "object_control_top.h"
#include "hiscore.h"
#include "file_io.h"
#include	"txtsplat.h"
#include "dlp.h"
#include "zex_misc.h"
#include "pictures.h"
#include	"camera.h"
#include "splat_control.h"
#include "sound.h"
#include "obj_control_misc_routines.h"

#include <string>
using namespace std;

void render_frame(int width, int height);
void draw_hiscores();
//sub funcs of dlp
void draw_line(int start_x, int start_y, int end_x, int end_y, int colour);


void do_hiscore_screen()
{

extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern UInt32 gLaser;
extern UInt32 gCannon;
extern	float SinTable[360], CosTable[360];
extern int camera_rotation_z;	//obviously the camera rot
extern int camera_rotation_delta_z; //set this to !0 and caamera rots around z

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
load_dyn_object_pb the_params;

int i;	//good old i
//long last_button_press_time=0;
Handle shield_data_H;

LSRAW *the_picture_shield;
int y_inc=-1;
int x1=220,y1=160;	//circle center
int y=0;
int picture_x,picture_y;
int r=52,r_inc=0;	//radius
int scaling_factor=2;
float scale_count=40;

clear_pb(&the_params);	//saves loads of zero code

    camera_rotation_z=0;
    camera_rotation_delta_z=1;

	extern _3D camera_pos;
	camera_pos.x=0;
	camera_pos.y=0;
	camera_pos.z=-1600;
	
while (game_fire_button()==1);
//    reset_camera();
//    
////load in 100 stars
    reset_objects();	//clear out ocb's
//    load_object ("shield1.z",0,'ZEXS',no_colldet,'MZ01');
    load_object ("star.z",0,'ZEXS',no_colldet,0,0);
////load in the shield
//	  the_params.world_x=0;
//	  the_params.world_y=0;
//	  the_params.world_z=-1000;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=1;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=0;	  //these zoom towards us
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=0;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=0;
//	      the_params.object_category=baddie_object;
//
//	  load_dyn_object(0,0,&the_params);	//the object, the position (-1=next free)    


    for (i=0;i<50;i++)
    {
	the_params.world_x=RangedRdm(1,4000)-2000;
	the_params.world_y=RangedRdm(1,4000)-2000;
	the_params.world_z=RangedRdm(1000,3000);
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
	the_params.Omovedz=-RangedRdm(15,50);
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
	the_params.rot_z=RangedRdm(1,359);
	the_params.controller_ref=star_for_story;	//these zoom towards us
//	the_params.control_use_1=0;
//	the_params.control_use_2=0;
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	the_params.shield_value=0;
//     the_params.has_roll=0;
	the_params.mass=100;
//	the_params.dq=0;
	the_params.no_trivial_rejection=1;
	    the_params.object_category=baddie_object;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(0,i,&the_params,-1);	//the object, the position (-1=next free)    
    }

//set up drawing env.
    set_poly_clipping(0,639,0,460);	//set poly clip rect - sb 181298

    shield_data_H=GetZexPicture ('RCZA',128);	//shield pict   
	LS_HLock(shield_data_H);
	the_picture_shield = (LSRAW*)*shield_data_H;

	do_fixed_frame_rate(FRAME_RATE_REINIT);
    while (game_fire_button()==0)
    {
      camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
      if (camera_rotation_z > 359) camera_rotation_z = camera_rotation_z - 359;	//check if >255

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
 //   screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
 //   screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

    render_frame(639,460);	//calls all the needed 3d stuff
    scaling_factor=static_cast<int>(scale_count);
    if (scale_count>0)
    scale_count-=0.5;
    picture_x=static_cast<int>(x1+r*COS(y));
    picture_y=static_cast<int>(y1+r*SIN(y));
    print_pict_scaled(the_picture_shield,picture_x,picture_y,scaling_factor,0);
    y+=y_inc;
    if (y>359) y-=360; 
    if (y<0) y+=360;
//    r=r+r_inc;
//    if (r>80) 
//    {
//    r_inc=-r_inc;
//    y_inc=-y_inc;
//    }
//    if (r<10) r_inc=-r_inc;
    
//    print_pict(the_picture_shield,130,80);

    draw_hiscores();	//show the high scores
//print the strings
    do_object_control();
	Show_it();
	do_fixed_frame_rate(FRAME_RATE_DELAY);
    }	//end of gcannon
    
   
   
    set_poly_clipping(0,356,0,440);	//set poly clip rect - sb 181298
    gLaser=0;
    gCannon=0;
    reset_objects();	//clear out ocb's

	 LS_DisposeHandle(shield_data_H);
    while (game_fire_button()==1);


}


void draw_hiscores()
{
extern prefs gzex_prefs;
extern int weird_count,wc_inc;
extern int weird_countb,wc_incb;
int i,line;
unsigned char number_string[12];

weird_count+=wc_inc;
if (weird_count>210) 
    { 
    weird_count=209;
    wc_inc=-wc_inc;
    }
    
if (weird_count<1) 
   {
   weird_count=1;
   wc_inc=-wc_inc;
   }
   
weird_countb+=wc_incb;

if (weird_countb>210) 
    {
    wc_incb=-wc_incb;
    weird_countb=209;
    }
    
if (weird_countb<1) 
   {
   weird_countb=1;
   wc_incb=-wc_incb;
   }
line=30;

for (i=0; i<10; i++)
	{
	 ZexNumToString(gzex_prefs.the_scores[i].score,number_string);
	 number_string[number_string[0]+1]=0;	//quick p to c hack
	 number_string[0]=' ';
//	   SplatTextLarge_weird ((char *) number_string,390,line);
//
//     SplatTextLarge_weirdB (gzex_prefs.the_scores[i].name,100,line);


	 SplatTextLarge_Transparent ((char *) number_string,390,line);

     SplatTextLarge_Transparent (gzex_prefs.the_scores[i].name,100,line);

     line+=40;

	}
SplatText_Transparent ("PRESS FIRE TO CONTINUE",220,20,33);

}

//this routine examines a score and enters it if it's higher than any of the current top ten
//SB 8/2/99
void handle_hiscore()
{
extern prefs gzex_prefs;
extern int score,tutorial,training_mission, user_abort;

int i;

if (tutorial || training_mission || user_abort==1) return;	//NO, don't do it if tutorial or training

for (i=0;i<10;i++)
   {
   if (score>gzex_prefs.the_scores[i].score)
     {
     insert_score(score,i);
     do_hiscore_screen();
     break;
     }
   }

}


class hiscore_input_processing {
	
public:
	hiscore_input_processing();
	~hiscore_input_processing();
	bool driver();
	string get_name() { return hiscore_name_edit; }
private:
		bool hiscore_process_key(SDLKey key, Uint16 unicode_key);
	
	string hiscore_name_edit;
	int previous_unicode;
};

hiscore_input_processing::hiscore_input_processing()
{
	zex_polling_switch(false);
	int previous_unicode = SDL_EnableUNICODE(1); // turn on unicode translation
	hiscore_name_edit.erase();		// ensure name start blank
}

hiscore_input_processing::~hiscore_input_processing()
{
	zex_polling_switch(true);
	SDL_EnableUNICODE(previous_unicode);	// turn unicode off (if off previously)
}

bool hiscore_input_processing::driver()
{
	SDL_Event event;
	
	while ( SDL_PollEvent(&event) ) {
		
		update_zex_main_event_state(event);	// keep Zex's state up to date...
											// (Zex is entirely state polled)
											// also process for the 
		
		switch (event.type) {
			
			case SDL_KEYDOWN:
			{
				//printf("Keydown %i %i %c\n", event.key.keysym.sym, event.key.keysym.unicode, event.key.keysym.unicode);
				bool stop_flag = hiscore_process_key(event.key.keysym.sym, event.key.keysym.unicode);
				if(stop_flag) return true;
			}
					break;
				
			case SDL_QUIT:
				//printf("Quit\n");
				// this is what the Zex event processing does - it cleans up SDL via atexit().
				exit(0);			// quit immediately
									//				request_quit();
				break;
				
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			   break;
			case SDL_KEYUP:
				//printf("Keyup %i\n", event.key.keysym.sym);
				break;
			default:
				//printf("other \n");
				break;
		} // switch(event.type)
		
	} // while(SDL_PollEvent())
	
	return false;
	
}

bool hiscore_input_processing::hiscore_process_key(SDLKey key, Uint16 unicode_key)
{
	bool finished = false;		// we haven't finished by default
	
	if(key==SDLK_RETURN || key==SDLK_KP_ENTER)
	{
		finished = true;	// finished and accept
	}
	else if(key==SDLK_ESCAPE)
	{
		hiscore_name_edit.erase();			// delete name entirely
		finished = true;	// finished and cancel
	}
	else if(key==SDLK_BACKSPACE)
	{
		int len = hiscore_name_edit.length();
		if(len)
		{
			hiscore_name_edit.erase(len-1,1);	// remove last character
		}
	}
	else // must be character for input stream
	{
		if(unicode_key >= 'a' && unicode_key <= 'z')
		{
			unicode_key -= 'a';
			unicode_key += 'A';
		}
		
		if((unicode_key & 0xff80) == 0 && unicode_key>=' ' && unicode_key<='Z')	// ensure only printable characters
		{
			if (hiscore_name_edit.length() >= NAME_LEN) 
			{
				play_zsound_always(low_beep, sound_high_pri, ls1_chan, sound_vol_7);
			}
			else
			{
				hiscore_name_edit += unicode_key;
			}
		}
	}
	
	return finished;		
}



void draw_box(int new_pos);
void draw_box(int x, int y);
void insert_score_display_screen(string name_so_far);
void insert_score_add_score(string name, int the_score, int score_pos);

//
// Here we do the "you've got a high score"
//
void insert_score(int the_score, int score_pos)
{
	extern int weird_count,wc_inc;
	
	set_poly_clipping(0,639,0,479);
	
	while (game_fire_button()==1);	//wait for button up
	
	hiscore_input_processing name_input;
	
	bool finished=false;
	while(!finished)
	{
		weird_count+=wc_inc;
		if (weird_count>330) wc_inc=-wc_inc;
		if (weird_count<1) wc_inc=-wc_inc;

		insert_score_display_screen(name_input.get_name());
		
		user_delay(2);				// make approx 30 fps
		
		finished = name_input.driver();
		
	}
	
	insert_score_add_score(name_input.get_name(), the_score, score_pos);
	
}


void insert_score_display_screen(string name_so_far)
{
	static int flying_character = 0;
	static bool flying_character_setup_required = 0;
	static int flying_character_x, flying_character_y;
	static int flying_character_step_x, flying_character_step_y;
	static int previous_length = 0;
	static int flying_character_count;
	//
	// first we figure out if we need a flying character
	//
	int len = name_so_far.length();
	if(len != previous_length)
	{
		// if we've added to the string?
		if(len > previous_length)
		{
			flying_character = name_so_far[len-1];
			flying_character_setup_required = true;
		}
		else	// it got shorter ... disable flying character
		{
			flying_character = 0;
		}
		previous_length = len;
	}

	//
	// now start to draw the screen
	//
	char my_string[2];
	extern const RGBColor	rgbBlack;
	
	my_string[1]='\0';
	RGBBackColor (&rgbBlack);

    erase_zex_rect(640,480);	//in pictures
		
	do_down_bars();
	
	SplatTextLarge_Transparent("NEW HIGH SCORE!!",140,10);
	
	//
	//now draw the name string	
	//
	string name_to_draw = name_so_far;
	if(flying_character && len > 0)
	{
		name_to_draw.erase(len-1, 1);
	}
	SplatTextLarge_Transparent (name_to_draw.c_str(),120,340);
	int text_pixel_width = PixelWidthof_SplatTextLarge_Transparent(name_to_draw.c_str());	
	int dest_x = 124+text_pixel_width; int dest_y = 340;
	draw_box(dest_x, dest_y);
	
	//print char set
    int x=80; int y=80;
    for (int i=' '; i<'[';i++)
	{
		if (x>540)
        {
			x=80;
			y=y+40;
        }
		my_string[0]=i;
		if(i == flying_character)
		{
			if(flying_character_setup_required)
			{
				const int steps_to_arrive = 7;
				flying_character_x = x; flying_character_y = y;
				flying_character_step_x = (dest_x - x) / steps_to_arrive;
				flying_character_step_y = (dest_y - y) / steps_to_arrive;
				flying_character_setup_required = false;
				flying_character_count = steps_to_arrive+1;
			}
			SplatTextLarge_Transparent(my_string,flying_character_x,flying_character_y);
		}
		else
		{
			SplatTextLarge_Transparent(my_string,x,y);
		}
		
		x=x+40;
	} 
	//y+=20;
	
	//SplatText_Transparent ("DEL",x,y,10);
	//x=80;
	//y+=40;
	//SplatText_Transparent ("OK",x,y,10);

	
	SplatText_Transparent ("Type your name and hit [return] or [escape] to cancel.",130,420,249);
	
		
	do_up_bars();
	
	Show_it();
	
	// update the flying character movement
	flying_character_x += flying_character_step_x;
	flying_character_y += flying_character_step_y;
	flying_character_count--;
	if(flying_character_count == 0) flying_character = 0;		// disable when finished
}




void insert_score_add_score(string name, int the_score,int score_pos)
{
	extern prefs gzex_prefs;
	int i;

	// don't record an empty name (can be generated by cancel or immediate return)
	if(name.length() == 0) return;
	
	//now insert the name into the hiscores array and save 'em
	//copy from 10 to score_pos
	for (i=9;i>score_pos;i--)
	{
		strcpy(gzex_prefs.the_scores[i].name,gzex_prefs.the_scores[i-1].name);
		gzex_prefs.the_scores[i].score=gzex_prefs.the_scores[i-1].score;
	}
	strcpy(gzex_prefs.the_scores[score_pos].name,name.c_str());
	gzex_prefs.the_scores[i].score=the_score;
}



void draw_box(int x, int y)
{
	static int box_colour=215;

	draw_line(x,y,x+40,y,box_colour);	//dlp function
	draw_line(x+40,y,x+40,y+40,box_colour);	//dlp function
	draw_line(x,y+40,x+40,y+40,box_colour);	//dlp function
	draw_line(x,y,x,y+40,box_colour);	//dlp function
	
	box_colour++;
	if (box_colour>=245) box_colour=215;
	
	if (box_colour==223 ||
		box_colour==233 ||
		box_colour==243) box_colour+=2;	//stop from getting too dark	
}


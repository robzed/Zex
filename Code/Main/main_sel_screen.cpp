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

//do_main_sel_screen
//SB 1801298
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <files.h>
//oldmac//#include <memory.h>
//oldmac//#include <events.h>
//oldmac//#include <timer.h>
//oldmac//#include <resources.h>

#include "file_io.h"
#include "do_intro.h"
#include "sound.h"
#include "drawing.h"
#include "pictures.h"
#include	"txtsplat.h"
#include "ship_control.h"
#include "zex_misc.h"
#include "main_sel_screen.h"
#include "MeshToZex.h"
#include "3d_structures_def.h"
#include "load_objects.h"
#include "objects_controls.h"
#include "compile_zmap.h"
#include "Z_init.h"
#include "fpoly4.h"
#include "ship_control.h"
#include "input_control.h"
#include "splat_control.h"
#include "animation.h"
#include "Error_handler.h"
#include "object_control_top.h"
#include "hiscore.h"
#include "game_pause.h"
#include "game_defs.h"
#include "main.h"
#include "register_game.h"

#include <stdio.h>

//proto
void render_frame(int width, int height);
void render_frame_no_erase(int width, int height);
void render_frame_colour_bg(int width, int height, int colour);

void setup_main_screen(LSRAW* the_picture, LSRAW* the_button);
void load_ship(int static_obj);
void load_animated_ship(int static_obj, unsigned long frame1_id, unsigned long frame2_id, 
                   unsigned long frame3_id, unsigned long frame4_id);

void setup_tables();	//actually register game
void fade_main_screen();
void fade_up_main_screen();


//oldmac//draw_mouse_pointer();
int handle_mouse(LSRAW* button_down, LSRAW* button_up, 
                 LSRAW* the_picture,LSRAW* arrow_up,LSRAW* arrow_down,
                 LSRAW* arrow_up_down,LSRAW* arrow_down_down,
                 LSRAW* checkbox_off,LSRAW* checkbox_on);
                                  
//oldmac//get_bg (short x,short y);
int last_click_time;
static int button_has_been_up=0;


extern const RGBColor	rgbBlack;
extern const RGBColor	rgbWhite;
extern const RGBColor	rgbRed;
extern const RGBColor	rgbOrange;
extern const RGBColor	rgbBlue		;
extern const RGBColor	rgbDarkBlue;
extern const RGBColor	rgbYellow;
extern const RGBColor	rgbPurple;
extern const RGBColor	rgbGreen;
extern const RGBColor	rgbLightBlue;
extern const RGBColor	rgbGrey;
extern const RGBColor	rgbDarkGrey;

extern prefs gzex_prefs;
//oldmac//extern	  DSpContextReference		      mDisplayContext;

void do_creds_screen();

static int vbl_altered;
extern int gMusicPlaying;
extern _3D camera_pos;
extern int camera_rotation_z;	//obviously the camera rot
extern int camera_rotation_delta_z; //set this to !0 and caamera rots around z
extern _3D camera_pos_delta;

void do_main_sel_screen()
{
extern FSSpec Zex_FSSpec;
extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern LSRAW *mouse_grafic;
extern int camera_rotation_x;
extern int camera_rotation_y;
extern	DynObjectsFixedSize *ocb_ptr;
extern prefs gzex_prefs;
extern int music_volume;
extern int sound_volume;
extern int freeze,done_load_flag,start_time;
extern int accumulated_game_time;

//FSSpec the_file;
//Handle the_file_data_H;
//Handle object_data_H;
Handle button_data_H;
Handle select_data_H;

LSRAW *the_picture;
LSRAW *button;

Handle button_down_data_H;
LSRAW *button_down;
Handle arrow_down_data_H;
Handle arrow_up_data_H;
LSRAW *arrow_down;
LSRAW *arrow_up;
Handle arrow_up_down_data_H;
LSRAW *arrow_up_down;
Handle arrow_down_down_data_H;
LSRAW *arrow_down_down;

Handle checkbox_off_data_H;
LSRAW *checkbox_off;
Handle checkbox_on_data_H;
LSRAW *checkbox_on;

int wait_flag;
//LS_CGrafPtr the_drawing_buffer;
int yaccel=20;
int ypos=0;
//GDHandle	saveGDH;
//GrafPtr		savePort;
//int i;
int change_ship_counter,ship_counter;
//oldmac//LS_UnsignedWide start_micros,end_micros;
//load_dyn_object_pb the_params;
//Handle mouse_grafic_H;
typedef struct
{
 int anim,slot;
 unsigned long f1_id,f2_id,f3_id,f4_id;
} loader_info;

//list of animation and slot infos for ship loader
//anim flag, slot
#define NUMBER_OF_SHIPS 14
loader_info loader_list[NUMBER_OF_SHIPS]={ 
	{ 0,0,0,0,0,0 },	//zex
	{ 0,14,0,0,0,0 },	//falcon
	{ 0,17,0,0,0,0 },	//shuttle
	{ 0,18,0,0,0,0 },	//alien shuttle
	{ 0,19,0,0,0,0 },	//alien transporter
	{ 0,1,0,0,0,0 },
	{ 0,2,0,0,0,0 },
	{ 1,3,'MN31','MN32','MN33','MN34' },
	{ 0,13,'MN61','MN62','MN63','MN64' },
	{ 0,15,0,0,0,0 },	//warper
	{ 0,16,0,0,0,0 },	//excaliber
	{ 0,7,0,0,0,0 },
	{ 0,8,0,0,0,0 },
	{ 1,9,'MN41','MN42','MN43','MN44' },
};

int player_click,i,load_result;
int mode=0;	//0=main prefs, 1=sound prefs
extern int just_installed;
//extern CCrsrHandle our_cursor;

load_dyn_object_pb the_params;
int is_faded;

//еееEND of vars
is_faded=0;

last_click_time=LS_TickCount60()+2;
vbl_altered=0;

freeze=0;	//unfreeze engine in case we quit a previous game
done_load_flag=0;
change_ship_counter=300;
ship_counter=0;
//load in main picture to the 3 buffers

//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"prefs_bg.RCZ",&the_file);
//	select_data_H=read_file(the_file);

	 #if OGL==1
    select_data_H=GetZexPicture ('RCZ ',147);	//prefs_bg_ogl   
    #else
    select_data_H=GetZexPicture ('RCZ ',131);	//prefs_bg   
    #endif
	LS_HLock(select_data_H);
	the_picture = (LSRAW*)*select_data_H;

    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;

    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;

    arrow_down_data_H=GetZexPicture ('RCZ ',135);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"arrow_down.RCZ",&the_file);
//	arrow_down_data_H=read_file(the_file);
	LS_HLock(arrow_down_data_H);
	arrow_down = (LSRAW*)*arrow_down_data_H;

    arrow_up_data_H=GetZexPicture ('RCZ ',137);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"arrow_up.RCZ",&the_file);
//	arrow_up_data_H=read_file(the_file);
	LS_HLock(arrow_up_data_H);
	arrow_up = (LSRAW*)*arrow_up_data_H;

    arrow_up_down_data_H=GetZexPicture ('RCZ ',138);	//prefs_button_down   
//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"arrow_up_down.RCZ",&the_file);
//	arrow_up_down_data_H=read_file(the_file);
	LS_HLock(arrow_up_down_data_H);
	arrow_up_down = (LSRAW*)*arrow_up_down_data_H;

    arrow_down_down_data_H=GetZexPicture ('RCZ ',136);	//prefs_button_down   
	LS_HLock(arrow_down_down_data_H);
	arrow_down_down = (LSRAW*)*arrow_down_down_data_H;
    
    checkbox_off_data_H=GetZexPicture ('RCZ ',141);	//prefs_checkbox off   
	LS_HLock(checkbox_off_data_H);
	checkbox_off = (LSRAW*)*checkbox_off_data_H;

    checkbox_on_data_H=GetZexPicture ('RCZ ',142);	//prefs_checkbox on   
	LS_HLock(checkbox_on_data_H);
	checkbox_on = (LSRAW*)*checkbox_on_data_H;     
    
    reset_objects();	//clear out ocb's


//load zex for title screen    
//read an object
    load_object ("zex_title.z",0,'ZEXS',no_colldet,'T010',0);
    load_object ("m1.z",1,'ZEXS',no_colldet,0,0);
    load_object ("m3.z",2,'ZEXS',no_colldet,0,0);
    load_object ("ejectcap.z",7,'ZEXS',no_colldet,'TB00',0);
    load_object ("ast2.z",8,'ZEXS',no_colldet,'T002',0);

    load_object ("phoenix11.z",3,'MN31',yes_colldet,0,0);
    load_object ("phoenix12.z",4,'MN32',yes_colldet,0,0);
    load_object ("phoenix13.z",5,'MN33',yes_colldet,0,0);
    load_object ("phoenix14.z",6,'MN34',yes_colldet,0,0);

    load_object ("SC1a.z",9,'MN41',yes_colldet,'T11A',0);
    load_object ("SC1b.z",10,'MN42',yes_colldet,'T11B',0);
    load_object ("SC1c.z",11,'MN43',yes_colldet,'T11C',0);
    load_object ("SC1d.z",12,'MN44',yes_colldet,'T11D',0);

    load_object ("m6a.z",13,'MN60',no_colldet,0,0);

    load_object ("m7.z",14,'ZEXS',no_colldet,0,0);	//wingman
    load_object ("warper.z",15,'ZEXS',no_colldet,'WPR1',0);	//warper
    load_object ("excalibur.z",16,'ZEXS',no_colldet,'EXCA',0);	//warper
    load_object ("shut2.z",17,'ZEXS',no_colldet,'SHT1',0);	//earth shuttle
    load_object ("ashut2.z",18,'ZEXS',no_colldet,0,0);	//alien shuttle
    load_object ("atran1.z",19,'ZEXS',no_colldet,'TP01',0);	//alien transporter


    load_object ("star.z",35,'STAR',no_colldet,0,0);	//warper



    set_poly_clipping(0,356,0,419);	//set poly clip rect - sb 181298
    if (loader_list[ship_counter].anim==0)
    {
    load_ship(loader_list[ship_counter].slot);  
	}
	else
	load_animated_ship(loader_list[ship_counter].slot,loader_list[ship_counter].f1_id,
	                   loader_list[ship_counter].f2_id,loader_list[ship_counter].f3_id,
	                   loader_list[ship_counter].f4_id);
	
	camera_pos.x=0;
	camera_pos.y=0;
	camera_pos.z=-1000;
//	camera_pos.z=-800;
    camera_rotation_delta_z=0;

    camera_rotation_z=0;
    camera_rotation_x=0;
    camera_rotation_y=0;


	LS_FadeGamma(0,0);		// black out screen
    setup_main_screen(the_picture,button);
    setup_main_screen(the_picture,button);
      fade_up_main_screen();

//    setup_main_screen(the_picture,button);
//    setup_main_screen(the_picture,button);
//    setup_main_screen(the_picture,button);

//    load_object ("star.z",20,'ZEXS',no_colldet,0);
//    for (i=10;i<80;i++)
//    {
//	  the_params.world_x=RangedRdm(1,4000)-2000;
//	  the_params.world_y=RangedRdm(1,4000)-2000;
//	  the_params.world_z=RangedRdm(1000,3000);
//	  the_params.Omovedx=2;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;	//-RangedRdm(15,50);
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=RangedRdm(1,4);
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=RangedRdm(1,359);
//	  the_params.controller_ref=dust;	//these zoom towards us
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
//	  load_dyn_object(20,i,&the_params);	//the object, the position (-1=next free)    
//    }

    wait_flag=0;
    camera_pos_delta.z=10;
    LS_ShowCursor();
   
	do_fixed_frame_rate(FRAME_RATE_REINIT);

//еееmain title screen display loop
	while (wait_flag==0)	//until mouse control says play
     {
      if (LS_Left_Button()==0) button_has_been_up=1;
      
      //oldmac//Microseconds(&start_micros);	 //equalise
//    setup_main_screen(the_picture,button);

      change_ship_counter--;
      if (change_ship_counter<0)
      {
        change_ship_counter=260;
        ship_counter++;
        if (ship_counter>NUMBER_OF_SHIPS-1) ship_counter=0;
        if (loader_list[ship_counter].anim==0)
        {
         load_ship(loader_list[ship_counter].slot);  
	    }
	    else
 	     load_animated_ship(loader_list[ship_counter].slot,loader_list[ship_counter].f1_id,
	                   loader_list[ship_counter].f2_id,loader_list[ship_counter].f3_id,
	                   loader_list[ship_counter].f4_id);
      }
      camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
      if (camera_rotation_z > 359) camera_rotation_z = camera_rotation_z - 359;	//check if >255

      camera_pos.z=camera_pos.z+camera_pos_delta.z;
      if (camera_pos.z>-950) camera_pos_delta.z=-camera_pos_delta.z;
      if (camera_pos.z<-2900) camera_pos_delta.z=-camera_pos_delta.z;      
 	  run_animations();	//in animation.c
      render_frame_colour_bg(360,420,242);	//calls all the needed 3d stuff

#define showships 1
//#define textcol 181
#define textcol 246

#if showships==1
      if (ship_counter==0)
      {
        SplatText_Transparent_Fade ("ZEX",156,100,textcol);
        SplatText_Transparent_Fade ("Upgradeable attack craft",80,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY:   -100/1000+",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS:    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER:      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES:   3",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT:      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE: Earth",20,300,textcol);
        SplatText_Transparent_Fade ("Look after this!",20,320,textcol);

        
      }
      else
      if (ship_counter==1)
      {
        SplatText_Transparent_Fade ("FALCON",136,100,textcol);
        SplatText_Transparent_Fade ("VELOCITY:   -100/900",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS:    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER:      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES:   Not as standard",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT:      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE: Earth",20,300,textcol);
        SplatText_Transparent_Fade ("Standard Earth attack craft.",20,320,textcol);

        
      }
      else
      if (ship_counter==2)
      {
        SplatText_Transparent_Fade ("SHUTTLE",140,100,textcol);
        SplatText_Transparent_Fade ("Short range transport craft",80,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -100/420",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    No",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   No",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Earth",20,300,textcol);
        SplatText_Transparent_Fade ("These guys live to trade. Not a combat craft",20,320,textcol);
        SplatText_Transparent_Fade ("but usually fitted with high-end shields and",20,334,textcol);
        SplatText_Transparent_Fade ("lasers.",20,348,textcol);
        SplatText_Transparent_Fade ("The backbone of Earth's presence in this galaxy,",20,366,textcol);
        SplatText_Transparent_Fade ("these guys should be protected at all costs.",20,380,textcol);

      }
      else
      if (ship_counter==3)
      {
        SplatText_Transparent_Fade ("ALIEN SHUTTLE",140,100,textcol);
        SplatText_Transparent_Fade ("Short range transport craft",80,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -100/580",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    No",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   No",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("Again, these guys aren't poor. Get too close",20,320,textcol);
        SplatText_Transparent_Fade ("and you'll know about it.",20,334,textcol);
        SplatText_Transparent_Fade ("Command looks very highly on the destruction of",20,352,textcol);
        SplatText_Transparent_Fade ("Alien shuttles.",20,366,textcol);

      }

      else
      if (ship_counter==4)
      {
        SplatText_Transparent_Fade ("ALIEN TRANSPORTER",130,100,textcol);
        SplatText_Transparent_Fade ("Long range transport craft",100,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -100/230",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   Yes",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes - Crew of 40+",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("There is no data available for this vessel.",20,360,textcol);

      }

      else

      if (ship_counter==5)
      {
        SplatText_Transparent_Fade ("TYPE M2",140,100,textcol);
        SplatText_Transparent_Fade ("Short range defence craft",70,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY   350+",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   No",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      No",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);

        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("An annoyance.",20,360,textcol);
      }

      else    
      if (ship_counter==6)
      {
        SplatText_Transparent_Fade ("TYPE M3",140,100,textcol);
        SplatText_Transparent_Fade ("Heavy defence craft",90,120,textcol);
        SplatText_Transparent_Fade("VELOCITY   500",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   No",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("Avoid in groups!",20,360,textcol);
        SplatText_Transparent_Fade ("Avoid ramming.",20,380,textcol);
        }
      else
      if (ship_counter==7)
      {
        SplatText_Transparent_Fade ("TYPE M4A - A.K.A. PHOENIX",90,100,textcol);
        SplatText_Transparent_Fade ("Fast attack/defence class",90,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -200/+800",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   2 Pylons",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("Highly feared! Good shields.",20,360,textcol);


      }
      else

      if (ship_counter==8)
      {
        SplatText_Transparent_Fade ("TYPE M6 - PYTHON",90,100,textcol);
        SplatText_Transparent_Fade ("Fast attack/defence class",90,120,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -200/+1800",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    ???",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      ???",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   ???",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("There is no data available for this vessel.",20,360,textcol);

      }
      else
      if (ship_counter==9)
      {
        SplatText_Transparent_Fade ("WARPER",150,100,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -200/+1375",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   2",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes - Crew of 2",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("Cruiser class - fast with medium maneuverability.",20,360,textcol);
        SplatText_Transparent_Fade ("Powerful forward firing weapons.",20,380,textcol);

      }

      else
      if (ship_counter==10)
      {
        SplatText_Transparent_Fade ("EXCALIBUR",150,100,textcol);
        SplatText_Transparent_Fade ("VELOCITY   -20/+320",20,200,textcol);
        SplatText_Transparent_Fade ("CANNONS    Yes",20,220,textcol);
        SplatText_Transparent_Fade ("LASER      Yes",20,240,textcol);
        SplatText_Transparent_Fade ("MISSILES   8 Pylons. Possible on-ship stock.",20,260,textcol);
        SplatText_Transparent_Fade ("PILOT      Yes - Crew of 5",20,280,textcol);
        SplatText_Transparent_Fade ("ALLEGIANCE Alien",20,300,textcol);
        SplatText_Transparent_Fade ("RECON",20,340,textcol);
        SplatText_Transparent_Fade ("Area denial role. Heavy and slow.",20,360,textcol);
        SplatText_Transparent_Fade ("Heavyweight patrol/guard vessel.",20,380,textcol);

      }


      else
      if (ship_counter==11)
      {
        SplatText_Transparent_Fade ("EJECTION CAPSULE",90,130,textcol);
        SplatText_Transparent_Fade ("VELOCITY >1000",20,200,textcol);
        SplatText_Transparent_Fade ("You may need this!",20,360,textcol);

      }
      else
      if (ship_counter==12)
      {
        SplatText_Transparent_Fade ("ASTEROID",120,130,textcol);
        SplatText_Transparent_Fade ("These can possess strong magnetic fields!",20,360,textcol);

      }

      else
      if (ship_counter==13)
      {
        SplatText_Transparent_Fade ("SPACE COW",120,130,textcol);
        SplatText_Transparent_Fade ("These organic creatures are harmless.",20,360,textcol);
        SplatText_Transparent_Fade ("Can be a great source of raw fuel.",20,380,textcol);
 
      }
      
#endif	//of showships
     
     player_click=handle_mouse(button_down,button,the_picture,arrow_up,arrow_down,
                               arrow_up_down,arrow_down_down,checkbox_off,checkbox_on);
                               
     if (player_click==2)	//load game 
     {
	 debug_printf(debug_overview, "---- LOAD GAME ---- \n");

	untake_mouse_keyboard_as_input();
//     load_result=load_game();	//1=ok, 0=canx
     load_result=1;
     take_mouse_keyboard_as_input();
     if (load_result==1) 
     {
     fade_main_screen();
     is_faded=1;
     wait_flag=1;
     done_load_flag=1;	//want to load a game
//     start_time=LS_TickCount60();
     }
     else done_load_flag=0;
     
     if (is_faded==0)
      {
       setup_main_screen(the_picture,button);
       setup_main_screen(the_picture,button);
//    setup_main_screen(the_picture,button);
      }
       LS_ShowCursor();
     }


     if (player_click==5)	//register game 
     {
     fade_main_screen();
//     is_faded=1;

     untake_mouse_keyboard_as_input();
	 
	 // disabled temporarily...
     //oldmac//setup_tables();	//1=ok, 0=canx
     take_mouse_keyboard_as_input();
	 handle_register_game();	//in register_game.cpp
 //DSpContext_FadeGamma ( mDisplayContext, 0, 0);	//black
	 LS_FadeGamma(0,0);
	 
    setup_main_screen(the_picture,button);
    setup_main_screen(the_picture,button);
      fade_up_main_screen();
    set_poly_clipping(0,356,0,419);	//set poly clip rect - sb 181298

//    setup_main_screen(the_picture,button);
    LS_ShowCursor();

     }
 

  
     if (player_click==3 || player_click==7) 
     {
     if (player_click==3) 
	 {
		 debug_printf(debug_overview, "---- NEW GAME ---- \n");
		 get_level();	//get difficulty level
	 }
     else
	 {
		 fade_main_screen();
		 debug_printf(debug_overview, "---- QUIT GAME ---- \n");
	 }
     
     is_faded=1;
     start_time=LS_TickCount60();
     wait_flag=1;	//play level, quit game
     accumulated_game_time=0;	//reset accum time on new game
     }
     
     
     if (player_click==6 || player_click==4)	//been doing the story/hiscore screen
     {
         load_object ("zex_title.z",0,'ZEXS',no_colldet,'T010',IS_NOT_TRANSLUCENT);
         ship_counter=0;	//reset back to zex on next frrame
         load_ship(loader_list[ship_counter].slot);  
         set_poly_clipping(0,356,0,419);	//fixes not clipping correctly in < 0.88

     }
     
     if (vbl_altered==1)
     {
     vbl_altered=0;
    setup_main_screen(the_picture,button);
    setup_main_screen(the_picture,button);
     
     }
//     do_object_control();
        MoveTo(0,10);
        TextFont(21);
        TextSize(9);
        TextFace(normal);
        RGBForeColor(&rgbBlue);

        if ( gzex_prefs.registered==1)
        {
        DrawString_c(VERS_STRING);
	}
        else
        {
		DrawString_c(VERS_STRING);

#if FLASH_MAIN_MENU_UNREG
		static unsigned int counter = 0;
		static bool state = false;
		if(state) { RGBForeColor(&rgbWhite); }
		counter++;
		if(counter > 20) { counter = 0; state = !state; }
#else
		RGBForeColor(&rgbWhite);				// make sign nice and visible
#endif
        DrawString_c(" *** UNREGISTERED ***");
	}

     if (is_faded==0) Show_it();	//splat 3d portion of window
    
 
     //oldmac//Microseconds(&end_micros);
     //oldmac//while (end_micros.lo-start_micros.lo <1000000/38)	//fps
     //oldmac//    Microseconds(&end_micros);
	 do_fixed_frame_rate(FRAME_RATE_DELAY);
	 
	 }	//end of while

//fade_out_music();	

	 LS_DisposeHandle(select_data_H);
     LS_DisposeHandle(button_data_H);
     LS_DisposeHandle(button_down_data_H);
     LS_DisposeHandle(arrow_up_data_H);
     LS_DisposeHandle(arrow_down_data_H);
     LS_DisposeHandle(arrow_up_down_data_H);
     LS_DisposeHandle(arrow_down_down_data_H);
     LS_DisposeHandle(checkbox_off_data_H);
     LS_DisposeHandle(checkbox_on_data_H);
  
    
    LS_HideCursor();
     display_wait();	//

gzex_prefs.sound_volume=sound_volume;
gzex_prefs.music_volume=music_volume;
write_prefs();
freeze=0;	//game is not frozen
}

static int old_button_state=0;
//int button_y_array[7] = 
//    {133,170,207,244,281,318,354};
int button_y_array[7] = 
    {113,150,187,224,261,298,334};
    

int handle_mouse(LSRAW* button_down, LSRAW* button_up, 
                 LSRAW* the_picture,LSRAW* arrow_up,LSRAW* arrow_down,
                 LSRAW* arrow_up_down,LSRAW* arrow_down_down,
                 LSRAW* checkbox_off,LSRAW* checkbox_on)
{

extern int button_y_array[7];
extern int music_volume;
extern int sound_volume,Use_Mode320, Use_SpeedLimit, Use_VBL, Change_res;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i,mvolume;
unsigned char number_string[2] = {0,0};
//oldmac//extern int old_button_state;
extern int last_click_time;
extern int quit;
//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=LS_Left_Button();
//    button_state=game_fire_button();

//draw lores checkbox
//    #if OGL==0
//    if (Use_Mode320==0)
//    print_crunched_pict(checkbox_off,415,436);
//    else
//    print_crunched_pict(checkbox_on,415,436);
//    #endif
    
//draw speedlimit checkbox
//    if (Use_SpeedLimit==0)
//    print_crunched_pict(checkbox_off,415,456);
//    else
//    print_crunched_pict(checkbox_on,415,456);
    
//draw vbl checkbox
    #if OGL==0

//    if (Use_VBL==0)
//    print_crunched_pict(checkbox_off,380,456);
//    else
//    print_crunched_pict(checkbox_on,380,456);


//draw change res checkbox
    if (Change_res==0)
    print_crunched_pict(checkbox_off,415,437);
    else
    print_crunched_pict(checkbox_on,415,437);
    #endif


//draw sound and music arrows
    print_crunched_pict(arrow_up,380,393);
    print_crunched_pict(arrow_up,380,415);

    print_crunched_pict(arrow_down,415,394);
    print_crunched_pict(arrow_down,415,416);
//draw volumes
//music
    mvolume=music_volume;
    mvolume=mvolume>>3;
    number_string[0]=mvolume+0x30;	//make ascii 1-8
    SplatText_Fade ((char *)number_string,403,419,textcol,255);

//sounds
    mvolume=sound_volume;
    mvolume=mvolume>>3;	//div 8 (0-63)
    number_string[0]=mvolume+0x30;	//make ascii 1-8
    SplatText_Fade ((char *)number_string,403,396,textcol,255);
//draw arrow under mouse
//d8 - check for keys
//low res vid
//     #if OGL==0
//    if (mouse_xy.h>415 && mouse_xy.h<415+120 && mouse_xy.v>435 && mouse_xy.v<435+16)
//    {
//       if (LS_Left_Button() && button_has_been_up)
//       {
//       button_has_been_up=0;
//       Use_Mode320=Use_Mode320^1;
//       }
//    }  
//    #endif

//Use speed limiter
//    if (mouse_xy.h>415 && mouse_xy.h<415+140 && mouse_xy.v>456 && mouse_xy.v<456+16)
//    {
//       if (LS_Left_Button() && button_has_been_up)
//       {
//       button_has_been_up=0;
//       Use_SpeedLimit=Use_SpeedLimit^1;
//       }
//    }  

//Use vbl
      #if OGL==0

//    if (mouse_xy.h>250 && mouse_xy.h<380+16 && mouse_xy.v>456 && mouse_xy.v<456+16)
//    {
//       if (LS_Left_Button() && button_has_been_up)
//       {
//       button_has_been_up=0;
//       Use_VBL=Use_VBL^1;
//       vbl_altered=1;
//       }
//    }  

//Change res
    if (mouse_xy.h>415 && mouse_xy.h<415+16 && mouse_xy.v>435 && mouse_xy.v<435+16)
    {
       if (LS_Left_Button() && button_has_been_up)
       {
		button_has_been_up=0;
		Change_res=Change_res^1;
		toggle_video();
		setup_main_screen(the_picture,button_up);
		setup_main_screen(the_picture,button_up);

       }
    }  
     #endif

    //up arrows
 if (LS_Left_Button() && button_has_been_up)
   {
    button_has_been_up=0;
    if (mouse_xy.h>380 && mouse_xy.h<396 && mouse_xy.v>393 && mouse_xy.v<409)
    {
       if (LS_Left_Button())
       {
       sound_volume+=8;
       if (sound_volume>63) 
       {
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, ls1_chan, sound_vol_7);
       }
       else
        {
        print_crunched_pict(arrow_up_down,380,393);
        play_zsound_always(setup_sound, sound_high_pri, c1_chan1, sound_vol_7);
        } 
       
       }
    }  

    //music up
    if (mouse_xy.h>380 && mouse_xy.h<396 && mouse_xy.v>416 && mouse_xy.v<432)
    {
       if (LS_Left_Button())
       {
       music_volume+=8;
       
       if (gMusicPlaying==0)     play_tune(131);
        
       if (music_volume>63) 
       {
        int old_volume;
        music_volume=63;
        old_volume=sound_volume;
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        sound_volume=old_volume;
       }
       else
       print_crunched_pict(arrow_up_down,380,416);
             
       music_set_vol(music_volume);
       }
    }  
    
    //down arrows
    if (mouse_xy.h>415 && mouse_xy.h<431 && mouse_xy.v>393 && mouse_xy.v<409)
    {
       if (LS_Left_Button())
       {
       sound_volume-=8;
       if (sound_volume<0) 
       {
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        sound_volume=0;
       }
       else
        {
        print_crunched_pict(arrow_down_down,415,394);
        play_zsound_always(setup_sound, sound_high_pri, c1_chan1, sound_vol_7);
        }
       }

    }
    //music down
    if (mouse_xy.h>415 && mouse_xy.h<431 && mouse_xy.v>417 && mouse_xy.v<433)
    {
       if (LS_Left_Button())
       {
       music_volume-=8;
       if (music_volume<8) 
       {
        int old_volume;
        music_volume=0;
        old_volume=sound_volume;
        sound_volume=63;
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        sound_volume=old_volume;       
       }
       else
       print_crunched_pict(arrow_down_down,415,417);
       
       if (music_volume<6)	//bit of a bug in the mod player wont let one set volume 0
        {
//        music_set_vol(1);
        stop_music_now();
        }
       else
        music_set_vol(music_volume);
       
       }

    }
  }
//first draw all buttons as off
    for (i=0; i<7;i++)
        print_crunched_pict(button_up,380,button_y_array[i]);
//now draw the down one
    for (i=0; i<7;i++)
    {
     if (mouse_xy.v>button_y_array[i] && mouse_xy.v<button_y_array[i]+36 && mouse_xy.h>390
         )	//controls
     {
     print_crunched_pict(button_down,380,button_y_array[i]);
     return_val=i+1;
     }
    }
//check for keyboard shortcuts
if (IsKeyPressed(SDLK_n)) { real_return_val=3; goto end; }	 //N key
if (IsKeyPressed(SDLK_s)) { real_return_val=1; goto end; }	 //S key
if (IsKeyPressed(SDLK_c)) { real_return_val=6; goto end; }	 //C key
if (IsKeyPressed(SDLK_q)) { real_return_val=7; goto end; }	 //Q key
if (IsKeyPressed(SDLK_h)) { real_return_val=4; goto end; }	 //H key

//    else
//    print_crunched_pict(button_up,380,145);	//setup controls
//
//    if (mouse_xy.v>195 && mouse_xy.v<235 )	//sound
//    {
//    print_crunched_pict(button_down,380,195);
//    return_val=2;
//    }
//    else
//    print_crunched_pict(button_up,380,195);	//load
//
//    if (mouse_xy.v>243 && mouse_xy.v<283)
//    {
//    print_crunched_pict(button_down,380,243);
//    return_val=3;
//    }
//    else
//    print_crunched_pict(button_up,380,243);	//play
//
//    if (mouse_xy.v>370 && mouse_xy.v<410 )
//    {
//    print_crunched_pict(button_down,380,370);
//    return_val=4;
//    }
//    else
//    print_crunched_pict(button_up,380,370);	//credits
//  
//    if (mouse_xy.v>420 && mouse_xy.v<460)
//    {
//    print_crunched_pict(button_down,380,420);
//    return_val=20;
//    }
//    else
//    print_crunched_pict(button_up,380,420);	//quit
 

if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:

if (LS_Left_Button())        last_click_time=LS_TickCount60()+10;

//process real_return_val
if (real_return_val==1)
{
LS_HideCursor();
     fade_main_screen();

GraphicsPaused();
LS_FlushEvents(-1,0);	//flush all

configure_input_device();
LS_FlushEvents(-1,0);	//flush all

GraphicsActive();
 //oldmac//DSpContext_FadeGamma ( mDisplayContext, 0, 0);	//black
	LS_FadeGamma(0,0);

    setup_main_screen(the_picture,button_up);
    setup_main_screen(the_picture,button_up);
    fade_up_main_screen();
//    setup_main_screen(the_picture,button_up);

    LS_ShowCursor();
}

if (real_return_val==4)	//hiscore
{
	Show_it();
     display_wait();	//
//    fade_out_music();
 //   music_set_vol(music_volume);
//    play_tune(128);
    do_hiscore_screen();
//    fade_out_music();
//    play_tune(131);
//    music_set_vol(music_volume);

    setup_main_screen(the_picture,button_up);
    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);

real_return_val=4;
}


if (real_return_val==6)
{
	Show_it();
//     display_wait();	  //
//    fade_out_music();
 //   music_set_vol(music_volume);
//    play_tune(128);
    do_creds_screen();
//    fade_out_music();
//    play_tune(131);
//    music_set_vol(music_volume);

    setup_main_screen(the_picture,button_up);
    setup_main_screen(the_picture,button_up);
 //   setup_main_screen(the_picture,button_up);

real_return_val=6;
}

//quit game
if (real_return_val==7)
{
quit=1;
}

return real_return_val;
}
//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее



void do_creds_screen()
{
	camera_pos.x=0;
	camera_pos.y=0;
	camera_pos.z=-960;
fade_and_play_zex_resource(134);	//Should be 134

show_story_text();

fade_and_play_zex_resource(131);

}

void setup_main_screen(LSRAW* the_picture, LSRAW* the_button)
{
extern int button_y_array[7];

LS_CGrafPtr the_drawing_buffer;

	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
	print_crunched_pict(the_picture,0,0);
	print_crunched_trans_pict(the_button,380, button_y_array[0]);
	print_crunched_trans_pict(the_button,380, button_y_array[1]);
	print_crunched_trans_pict(the_button,380, button_y_array[2]);
	print_crunched_trans_pict(the_button,380, button_y_array[3]);
	print_crunched_trans_pict(the_button,380, button_y_array[4]);
	print_crunched_trans_pict(the_button,380, button_y_array[5]);
	print_crunched_trans_pict(the_button,380, button_y_array[6]);
        
        	
	Show_it();

}

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
// not used from this file
void load_star(int dyn_slot)
{
load_dyn_object_pb the_params;
int drot_z;
extern int dynamic_object_index;

//    kill(0);	  //clear out old ship

//    reset_objects();
//set up ship for title screen
	 the_params.unique_id=0;
	the_params.world_x=RangedRdm(0,2000)-1000;
	the_params.world_y=RangedRdm(0,2000)-1000;
	the_params.world_z=RangedRdm(10,300);
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	drot_z=RangedRdm(0,10)-5;
	if (drot_z==0) drot_z=5;
	
	the_params.delta_rot_z=drot_z;
//	the_params.delta_rot_z=0;	//230299

	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
     the_params.has_roll=0;
	the_params.mass=100;
	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=zex_object;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;

	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(35,dyn_slot,&the_params,-1);	//the object, the position (-1=next free)
}
#endif

void load_ship(int static_obj)
{
load_dyn_object_pb the_params;
int drot_z,i;
extern int dynamic_object_index;

    kill(0);	//clear out old ship

//    reset_objects();
//set up ship for title screen
	the_params.world_x=00;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	drot_z=RangedRdm(0,10)-5;
	if (drot_z==0) drot_z=5;
	
	the_params.delta_rot_z=drot_z;
//	the_params.delta_rot_z=0;	//230299

	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_7=0;
	the_params.control_use_4=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
     the_params.has_roll=0;
	the_params.mass=100;
	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=zex_object;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_obj,0,&the_params,-1);	//the object, the position (-1=next free)
    


}

void load_animated_ship(int static_obj, unsigned long frame1_id, unsigned long frame2_id, 
                   unsigned long frame3_id, unsigned long frame4_id)
{
load_dyn_object_pb the_params;
int drot_z,i;
extern int dynamic_object_index;
anim_pb anim;
    kill(0);	//clear out old ship
    kill(1);	//clear out old ship
    kill(2);	//clear out old ship
    kill(3);	//clear out old ship
    kill(4);	//clear out old ship
    kill(5);	//clear out old ship

//    reset_objects();
    anim.object_id[0]=frame1_id;
    anim.object_id[1]=frame2_id;
    anim.object_id[2]=frame3_id;
    anim.object_id[3]=frame4_id;

//set up ship for title screen
	the_params.world_x=00;
	the_params.world_y=0;
	the_params.world_z=0;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	drot_z=RangedRdm(0,8)-4;
	if (drot_z==0) drot_z=4;
	
	the_params.delta_rot_z=drot_z;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	the_params.shield_value=1000;
	the_params.mass=100;
	the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=zex_object;
     the_params.who_spawned_this_object=-1;

//	load_dyn_object(static_obj,0,&the_params);	//the object, the position (-1=next free)
    load_animated_object (&anim,4,3,1,1,&the_params,-1);


}



//еееееееееееееееееееееееее
//display the story and credits text
void show_story_text()
{

extern Ptr screenaddr;
extern int screenwidth;
extern const RGBColor	rgbBlack;
extern int gLaser;
extern int gCannon;

extern	float SinTable[360], CosTable[360];


LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
load_dyn_object_pb the_params;

Handle text_h;
Ptr text;
int text_size=0;
int text_pos;
int i,temp_y,text_line_index,text_current_pos;	//good old i
int end=0;
char the_char;
char text_line[1024];
static unsigned long last_button_press_time=0;
Handle picture_H;
LSRAW* the_picture;
LSRAW* pictbuffer;
int memsize;
int scaling_factor;
//array of x coords for creds picture
int pict_x[30]= {
120,150,180,210,230,238,245,238,212,200,
190,165,150,140,120,100,90,78,63,51,
45,40,37,36,35,34,33,32,21,21};


int text_scaled_x[17] = {
    300,299,299,297,295,293,291,287,285,281,
    276,271,265,229,200,160,0
};

int text_scaled_y[17] = {
    240,239,239,237,235,233,231,227,215,201,
    176,171,165,129,100,100,0
};


int wobble_count=00;
int wobble_inc=1;
int wobble_x[20]={
120,115,110,105,100,105,110,115,120,125,
130,135,140,145,150,145,140,135,130,125};

int wobble_y[20]={
120,115,110,105,100,105,110,115,120,125,
130,135,140,145,150,145,140,135,130,125};

//variables for moving picture in a circle
int r=10;	//radius
int r_inc=1;	//radius inc/dec
int y_inc=3;
int x1=120,y1=100;	//circle center
int y=0;
int picture_x,picture_y;
int click;
int scroll_pos,scroll_delta,colour;

//get picture
    picture_H=GetZexPicture ('RCZ ',2001);	//info pict   
 if (picture_H==0) report_error_c("Resource missing: RCZ 2001","",4);
	LS_HLock(picture_H);
	the_picture = (LSRAW*)*picture_H;

//get text

 text_h = (Handle) LS_GetResource('STRY',128);  //Get the Handle to the Resource 
 if (text_h==0 || ResError()!=0) report_error_c("Resource missing: STRY 128","",4);
 LS_HLock(text_h);
 text_size=GetHandleSize(text_h);
 text=*text_h;
 
    enable_inputs();

//load in 100 stars
    reset_objects();	//clear out ocb's
    load_object ("star.z",0,'ZEXS',no_colldet,0,IS_NOT_TRANSLUCENT);
    for (i=0;i<70;i++)
    {
	the_params.world_x=RangedRdm(1,4000)-2000;
	the_params.world_y=RangedRdm(1,4000)-2000;
	the_params.world_z=RangedRdm(1000,3000);
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=-RangedRdm(15,50);
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=RangedRdm(1,359);
	the_params.controller_ref=star_for_story;	//these zoom towards us
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=0;
	the_params.mass=100;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=baddie_object;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(0,i,&the_params,-1);	   //the object, the position (-1=next free)    
    }

//set up drawing env.
    set_poly_clipping(0,639,0,479);	//set poly clip rect - sb 181298
    last_button_press_time=LS_TickCount60()+10;
    click=0;    
    text_current_pos=0;
#define SCROLLMAX 450;

    scroll_pos=SCROLLMAX;
    scroll_delta=-3;
    colour=0;
//change 200699 - we now print ot a pict and then zoom that on and off
    pictbuffer=(LSRAW*)NewPtr(640*500);
    pictbuffer->pwidth=640;
    pictbuffer->pheight=480;
    
	int page = 1;
	char pagenum[80];

  while(end==0)
  {  
//set up to draw into our picture buffer
    screenaddr=(char*)pictbuffer+4;
    screenwidth=640;
    erase_zex_rect(640,480);	//in pictures


//print the strings
    text_pos=text_current_pos;
    temp_y=50;
    for (i=0;i<24;i++)
    {
     text_line_index=0;
     the_char=0;
     while (the_char!=13 && text_line_index<1023)
      {
      the_char=text[text_pos];
      text_line[text_line_index]=the_char;
      text_pos++;
      text_line_index++;
      if (text_pos==text_size) //end of file
      end=1;
      }
      text_line[text_line_index-1]=0;	//make c string
    
      if (temp_y<460 && end==0) SplatText_Transparent_Fade (text_line,30,temp_y,244);
      temp_y+=14;
    }	//end of for
    
//set up for zoom on
scaling_factor=16;  
    while(click==0)
    {
     int scale_x_index,scale_x,scale_y;

    user_delay(1);

    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures

    render_frame(639,479);	//calls all the needed 3d stuff
    
	
    do_object_control();
//drae the picture of the text
    scale_x_index=16-scaling_factor;
    scale_x=text_scaled_x[scale_x_index];
    scale_y=text_scaled_y[scale_x_index];
    print_pict_scaled(pictbuffer,scale_x,scale_y,scaling_factor,1);
    if (scaling_factor>0) scaling_factor--;

	sprintf(pagenum,"%i of 16", page);
	SplatText_Transparent_Fade (pagenum,580,460,224);
    SplatText_Transparent ("Press Fire To Continue",240,460,colour);
    colour++;
    if (colour>210) colour=0;

	Show_it();
    
    if (LS_TickCount60()>last_button_press_time)	//else it gets silly
    click=game_fire_button();
    else    
     {
     gLaser=0;
     gCannon=0;
     }
    }	//end of gcannon
    
    last_button_press_time=LS_TickCount60()+10;
//now we need to get the next set of text
    text_current_pos=text_pos;
    gLaser=0;
    gCannon=0;
    click=0;

	page++;
   }	//end of while (end)
DisposePtr((char*)pictbuffer);

//wait for final click
//now show info pict
//get memory to decompress picture
scaling_factor=25;
memsize=(*the_picture).pwidth;
memsize*=(*the_picture).pheight;
memsize+=80;
pictbuffer=(LSRAW*)NewPtr(memsize);
 if (pictbuffer==0) report_error_c("Memory Error","",-1);
 decrunch_pict(the_picture,pictbuffer);

   click=0;
   while(click==0)
   {
    user_delay(1); 

	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
    render_frame(639,479);	//calls all the needed 3d stuff
    if (scaling_factor>0)
    print_pict_scaled(pictbuffer,pict_x[scaling_factor],123,scaling_factor,1);
    else
    {
    picture_x=static_cast<int>(x1+r*COS(y));
    picture_y=static_cast<int>(y1+r*SIN(y));
    print_pict_scaled(pictbuffer,picture_x,picture_y,scaling_factor,1);
    y+=y_inc;
    if (y>359)  { y-=360; }
//    if (y<0)  { y+=360; }
    
//    r+=r_inc;
//    if (r>60)  r_inc=-r_inc; 
//    else
//    if (r<0) r_inc=-r_inc;
    
    }
    
    
    if (scaling_factor>0) scaling_factor--;
    
    do_object_control();	//do the stars
	Show_it();
    
    if (LS_TickCount60()>last_button_press_time)	//else it gets silly
    click=game_fire_button();
    else     gLaser=0;
   }	//end of gcannon
   
  
//zoom picture out
   while(scaling_factor<25)
   {
	user_delay(1);

	the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures

    render_frame(639,479);	//calls all the needed 3d stuff
    print_pict_scaled(pictbuffer,pict_x[scaling_factor],123,scaling_factor,1);
    if (scaling_factor<40) scaling_factor++;
    
    do_object_control();	//do the stars
	Show_it();
    
    if (LS_TickCount60()>last_button_press_time)	//else it gets silly
    poll_input_device();
    else     
     {
     gLaser=0;
     gCannon=0;
     }
   }	//end of gcannon
   
   
    set_poly_clipping(0,356,0,440);	//set poly clip rect - sb 181298
    gLaser=0;
   disable_inputs();
    reset_objects();	//clear out ocb's
    ReleaseResource(text_h);
    DisposePtr((Ptr) pictbuffer);
    LS_DisposeHandle(picture_H);
}


void fade_main_screen()
{
int fade_percent;
	do_fixed_frame_rate(FRAME_RATE_REINIT);
	
    fade_percent=100;
    while(fade_percent>10)

     {
	  LS_FadeGamma(fade_percent, 0);
      fade_percent-=5;

      camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
      if (camera_rotation_z > 359) camera_rotation_z = camera_rotation_z - 359;	//check if >255

      camera_pos.z=camera_pos.z+camera_pos_delta.z;
      if (camera_pos.z>-950) camera_pos_delta.z=-camera_pos_delta.z;
      if (camera_pos.z<-2900) camera_pos_delta.z=-camera_pos_delta.z;      
 	  run_animations();	//in animation.c
      render_frame_colour_bg(360,420,242);	//calls all the needed 3d stuff

#define textcol 246

     
     Show_it();	//splat 3d portion of window
	 do_fixed_frame_rate(FRAME_RATE_DELAY);
	 
    }	//end of while


 Get_back_buffer();
 erase_zex_rect(640,480);	//in pictures
 Show_it();
 Get_back_buffer();
 erase_zex_rect(640,480);	//in pictures
 Show_it();
 
 LS_NormalGamma();
}

void fade_up_main_screen()
{
int fade_percent;

	do_fixed_frame_rate(FRAME_RATE_REINIT);
	
    fade_percent=10;
    while(fade_percent<100)

     {
	  LS_FadeGamma(fade_percent, 0);
      fade_percent+=10;

      camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
      if (camera_rotation_z > 359) camera_rotation_z = camera_rotation_z - 359;	//check if >255

      camera_pos.z=camera_pos.z+camera_pos_delta.z;
      if (camera_pos.z>-950) camera_pos_delta.z=-camera_pos_delta.z;
      if (camera_pos.z<-2900) camera_pos_delta.z=-camera_pos_delta.z;      
 	  run_animations();	//in animation.c
      render_frame_colour_bg(360,420,242);	//calls all the needed 3d stuff

#define textcol 246

     Show_it();	//splat 3d portion of window
     
	 do_fixed_frame_rate(FRAME_RATE_DELAY);

    }	//end of while

	
	LS_NormalGamma();
}



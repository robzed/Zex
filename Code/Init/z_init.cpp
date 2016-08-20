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

/*game
init
*/

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include <DrawSprocket.h>
//oldmac//#include <ConditionalMacros.h>
//oldmac//#include <Dialogs.h>
//oldmac//#include <Types.h>
//oldmac//#include <Quickdraw.h>
//oldmac//#include	<fonts.h>
#include	"sound.h"	//sound public defs
#include	"splat_control.h"
#include	"Z_init.h"
#include	"matrix_primitives.h"
#include	"file_io.h"
#include "load_objects.h"
#include	"ship_control.h"
#include "map_control.h"
#include "camera.h"
#include "ship_control.h"
#include "object_control_top.h"
#include "explode.h"
#include "load_level.h"
#include "Damage.h"
#include "input_control.h"
#include "zex_misc.h"
#include "texture_management.h"
#include "dlp.h"
#include "handle_level.h"
#include "buy.h"
//#include "processor.h"
#include "Batcom.h"
#include "light_control.h"
#include "handle_level.h"
#include "instrum.h"
#include "instrum_int.h"
#include "pictures.h"
#include "buy_int.h"
#include "Error_handler.h"
#include "cargo.h"
#include "real_time.h"
#include "command_and_control.h"
#include "OGL.h"
#include "main.h"
//#include "fgouraud.h"

#ifdef fpoly
#include "fpoly4.h"
#endif
#include "txtsplat.h"
#include "CPanel.h"

//int the_level;
int added_nav;

void setup_fgouraud(void);
void OGL_load_control_panel();

//oldmac//CCrsrHandle our_cursor;


extern int Use_Mode320;
extern int Mode320,game_over;
extern int  level_has_control,level_has_camera_control,disable_camera_control;
extern int can_gen_dust;
extern int fastdock_in_progress,fastdock_fade_value;
extern int in_ramparian_space;
extern kit_desc ship_items[ITEMS_COUNT];
extern int show_control_panel,global_request_dock;
extern int global_show_target_vector,thrust_lockout;
extern int end_of_level_code;	//if not zero, we display end_of_level_reason_string
extern int can_gen_cargo,gRank,kills,promotion_points,tutorial;

#if OGL
extern int OGL_active;
#endif

int init_mac(void)
{
//oldmac//extern QDGlobals qd;
//oldmac//	MaxApplZone();				// Grab application memory.

//oldmac//	InitGraf(&qd.thePort);		// Initialize QuickDraw variables for our program.
//oldmac//	InitFonts();				// Initialize fonts.
//oldmac//	InitWindows();				// Initialize the Window Manager.
//oldmac//	InitMenus();				// Ditto for the Menu Manager.
//oldmac//	TEInit();					// blah, blah Text Edit.
//oldmac//	InitDialogs(0L);			// blah, blah Dialog Manager.
//oldmac//	InitCursor();				// Set the cursor to the arrow cursor and init.


//	MoreMasters();				// Allocate a block of master pointers.
//	MoreMasters();				// And allocate more.

//oldmac//	GetDateTime((unsigned long *)&qd.randSeed);		// Randomize random seed.
//	LS_FlushEvents(everyEvent, 0);	// Clear event queue of any pending events.
	LS_FlushEvents(-1, 0);		// this used to be... LS_FlushEvents(mDownMask | keyDownMask | autoKeyMask, 0);

	#if OGL
	OGL_active=0;
	#endif

	return 0;
}

Handle float_dialog_H;
LSRAW *float_dialog;
Handle small_flare_H;
LSRAW *small_flare_picture;
Handle large_flare_H;
LSRAW *large_flare_picture;
Handle sleep_dialog_H;
LSRAW *sleep_dialog;
Handle order_dialog_H;
LSRAW *order_dialog;
LSRAW *map_cursor_picture;
Handle map_cursor_H;

//inv
LSRAW *inv_picture;
Handle inv_H;
LSRAW *up_picture;
Handle up_H;
LSRAW *down_picture;
Handle down_H;
LSRAW *armed_picture;
Handle armed_H;

LSRAW *inv_cursor_picture;
Handle inv_cursor_H;



//ogl 32 bit textures
LSRAW *inv_picture_rgba;
LSRAW *inv_cursor_picture_rgba;
LSRAW* raw_rgba_data_CP;	//ptr to the rgba data for the control panel
LSRAW* short_range_ret_rgba;
LSRAW* long_range_ret_rgba;

int inv_pict_reference;	//from constant images or -1 if not set
int inv_cursor_reference;	//same
int cp_reference;
int long_range_reticule_ref;
int short_range_reticule_ref;	//radar reticules

int	init_game(void)
{
int	return_val=0;
//oldmac//LSRAW* pictbuffer;
//oldmac//LSRAW* pictbuffer_rgba;
//oldmac//UInt32 picture_w,picture_h;
//oldmac//Ptr raw_rgba_data;
//oldmac//Ptr raw_data;

//oldmac//Handle the_file_data_H;
//oldmac//LSRAW *the_picture;


//oldmac//extern FSSpec Zex_FSSpec;
extern int monsters_can_fire,text_display_timer1,text_display_timer2;
extern Handle bpic;
extern int have_bpic,Change_res;

//oldmac//extern Ptr screenaddr;		// for cache line stuff

        init_wingmen_under_command();

        float_dialog_H=GetZexPicture ('RCZ ',3203);	//floating dialog pict for tutorial
        if (float_dialog_H==0) report_error_c("Resource missing: RCZ 3203","",4);
	LS_HLock(float_dialog_H);
	float_dialog = (LSRAW*)*float_dialog_H;


        order_dialog_H=GetZexPicture ('RCZ ',3116);	//floating dialog pict for tutorial
        if (order_dialog_H==0) report_error_c("Resource missing: RCZ 3116 - order box top","",4);
	LS_HLock(order_dialog_H);
	order_dialog = (LSRAW*)*order_dialog_H;


        map_cursor_H=GetZexPicture ('RCZ ',3304);	//cross hair pict
	LS_HLock(map_cursor_H);
	map_cursor_picture = (LSRAW*)* map_cursor_H;



//inventory screens
    inv_H=GetZexPicture ('RCZ ',4000);	//inventory pict
	LS_HLock(inv_H);
	inv_picture = (LSRAW*)*inv_H;

    up_H=GetZexPicture ('RCZ ',3301);	//jett up pict
	LS_HLock(up_H);
	up_picture = (LSRAW*)*up_H;

    down_H=GetZexPicture ('RCZ ',3300);	//jett down pict
	LS_HLock(down_H);
	down_picture = (LSRAW*)*down_H;

    inv_cursor_H=GetZexPicture ('RCZ ',3302);	    //arrow pict
	LS_HLock(inv_cursor_H);
	inv_cursor_picture = (LSRAW*)* inv_cursor_H;

    armed_H=GetZexPicture ('RCZ ',3303);	//arrow pict
	LS_HLock(armed_H);
	armed_picture = (LSRAW*)* armed_H;

load_32bit_graphics();


        init_dynamic_textures();


global_request_dock=-1;

thrust_lockout=0;
global_show_target_vector=1;
fastdock_in_progress=0; fastdock_fade_value=100;
 can_gen_dust=1;
 level_has_control=0;
 level_has_camera_control=0;
 disable_camera_control=0;
 end_of_level_code=0;
  have_bpic=0;
 bpic=0;
 game_over=0;
 level_has_control=0;
//get files
//    our_cursor=GetCCursor(128);
//    SetCCursor(our_cursor);

return_val |= init_screen();
show_text_in_loading_screen("Exec: Screen Init Completed");

// print the SDL versions
SDL_version compile_version;
SDL_VERSION(&compile_version);
const SDL_version *link_version=SDL_Linked_Version();
char version_string[200];
sprintf(version_string, "SDL: compiled - %d.%d.%d / running - %d.%d.%d\n",
		compile_version.major, compile_version.minor, compile_version.patch,
		link_version->major, link_version->minor, link_version->patch);
show_text_in_loading_screen(version_string);



show_text_in_loading_screen("Exec: DLP Init I                             ");
    init_dlp();
show_text_in_loading_screen("Exec: Reading Prefs...");

	get_zex_fsspec();
    text_display_timer1=0;
    text_display_timer2=0;
    read_prefs();	//in file io
	if (Change_res)	//Set screen res as soon as we've read the prefs
	{
		full_screen_video();
	}
	else
	{
		windowed_video();
	}


show_text_in_loading_screen("Exec: Initialising F4 sound system");


	return_val |= init_zsound();

	SDL_ShowCursor(SDL_DISABLE);		// notice: initialising sound mixer turns it on, sometimes, therefore do turn it off here
	Zex_ShowCursor(SDL_ENABLE);			// let's have the software cursor

	//    play_zsound_always(sound_test, sound_high_pri, right_chan, sound_vol_7);
show_text_in_loading_screen("Zex: Setting up player");


ship_init();
show_text_in_loading_screen("F4: Maths Init");
	Z3D_TrigInit();	//set up our sin/cos tables


#ifdef fpoly
show_text_in_loading_screen("F4: Backend Init 1");
    init_polygon();
#endif
//oldmac//show_text_in_loading_screen("F4: Text Init");
//oldmac//    InitText();
show_text_in_loading_screen("F4: Overlay Init");

    InitControlPanel();
    init_zex_random();
show_text_in_loading_screen("F4: Loading renderer");
init_ogl();

show_text_in_loading_screen("F4: Loading textures");
        load_misc_graphics();
//        ogl_active();
	load_textures();
//	ogl_inactive();
show_text_in_loading_screen("F4: Backend Init 2");
    setup_fgouraud();
    monsters_can_fire=1;
show_text_in_loading_screen("F4: DLP Init 2");

    clear_dlp();	//clear the 2d display processor
show_text_in_loading_screen("F4:Input System Init");

   init_input_device();
show_text_in_loading_screen("F4: Engine Loaded - EOT 0");

show_text_in_loading_screen("Zex: Loading stocks                         ");

    load_up_stock_strings();
    init_batcom();
    read_in_cargo();
//    play_zsound_always(sound_test, sound_high_pri, left_chan, sound_vol_7);



	return return_val;


}


void load_32bit_graphics()
{
//LSRAW* pictbuffer;
//LSRAW* pictbuffer_rgba;
//UInt32 picture_w,picture_h;
//Ptr raw_rgba_data;
//Ptr raw_data;

//Handle the_file_data_H;
//LSRAW *the_picture;

        //load up 32 bit pictures
	#if OGL==1
	//inv pict 32 pict into some memory
       picture_w=	(*inv_picture).pwidth;
       picture_h=	(*inv_picture).pheight;	//now pointing to data

       pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
       if (pictbuffer==0) report_error_c("Memory Error in init_game(OGL),ID=1","",-1);
       decrunch_pict(inv_picture,pictbuffer);
       invert_y_pict(pictbuffer);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error_c("Memory Error in print_crunched_trans_pict(OGL),ID=2","",-1);
   //now load up the texture
       raw_data=(char *)&pictbuffer->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       inv_picture_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

        DisposePtr((Ptr)pictbuffer);

	//mouse cursor pict 32 pict into some memory
       picture_w=	(*inv_cursor_picture).pwidth;
       picture_h=	(*inv_cursor_picture).pheight;	//now pointing to data

       pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
       if (pictbuffer==0) report_error_c("Memory Error in zinit(OGL),ID=2","",-1);
       decrunch_pict(inv_cursor_picture,pictbuffer);
       invert_y_pict(pictbuffer);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error_c("Memory Error in zinit(OGL),ID=3","",-1);
   //now load up the texture
       raw_data=(char *)&pictbuffer->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       inv_cursor_picture_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

        DisposePtr((Ptr)pictbuffer);
//load up control panel pict
    the_file_data_H=GetZexPicture ('ZRAW',400);	//control panel
    LS_HLock(the_file_data_H);

//	FSMakeFSSpec(Zex_FSSpec.vRefNum,Zex_FSSpec.parID,"zcp1.RAW",&the_file);
//	the_file_data_H=read_file(the_file);
	the_picture = (LSRAW*)*the_file_data_H;


	//mouse cursor pict 32 pict into some memory
       picture_w=	(*the_picture).pwidth;
       picture_h=	(*the_picture).pheight;	//now pointing to data

//       pictbuffer=(LSRAW*)NewPtr(picture_w*picture_h+80);
//       if (pictbuffer==0) report_error_c("Memory Error in zinit(OGL),ID=2","",-1);
//       decrunch_pict(the_picture,pictbuffer);
       invert_y_pict(the_picture);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error_c("Memory Error in zinit(OGL),ID=3","",-1);
   //now load up the texture
       raw_data=(char *)&the_picture->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       raw_rgba_data_CP=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

LS_DisposeHandle(the_file_data_H);


//load short range radar reticules

     the_file_data_H=GetZexPicture ('RCZA',130);	//short range radar pict
    LS_HLock(the_file_data_H);

	the_picture = (LSRAW*)*the_file_data_H;


       picture_w=(*the_picture).pwidth;;	// Ian hard coded these rather than stick a header
                                //on his raw files!! SO I had to redo them
       picture_h=(*the_picture).pheight;;	//

       invert_y_pict(the_picture);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error_c("Memory Error in zinit(OGL),ID=3","",-1);
   //now load up the texture
       raw_data=(char *)&the_picture->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       short_range_ret_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

LS_DisposeHandle(the_file_data_H);

//long range reticules
    the_file_data_H=GetZexPicture ('RCZA',131);
    LS_HLock(the_file_data_H);

	the_picture = (LSRAW*)*the_file_data_H;


       picture_w=	(*the_picture).pwidth;
       picture_h=	(*the_picture).pheight;	//now pointing to data

       invert_y_pict(the_picture);
       pictbuffer_rgba=(LSRAW*)NewPtr((picture_w*picture_h*4)+80);
       if (pictbuffer_rgba==0) report_error_c("Memory Error in zinit(OGL),ID=3","",-1);
   //now load up the texture
       raw_data=(char *)&the_picture->raw_data;
       raw_rgba_data=(char *)&pictbuffer_rgba->raw_data;
   //
       convert_to_rgba_wh(raw_data,raw_rgba_data,2,picture_w,picture_h);
       long_range_ret_rgba=pictbuffer_rgba;
	pictbuffer_rgba->pwidth=picture_w;
	pictbuffer_rgba->pheight=picture_h;

LS_DisposeHandle(the_file_data_H);
	#endif
}

int	reinit_game(int the_level)
{
int	return_val=0;
//oldmac//extern FSSpec Zex_FSSpec;
//oldmac//extern int	zex_alive
extern int monsters_can_fire,game_pause;
extern int frame_counter,real_frame_counter,objs_displayed,number_of_stars;
//extern int the_level;
extern int freeze,day_night_timer,score,credits, start_time;
extern int showing_stats,level_complete,old_mothership_stock_slot_number,done_load_flag;
extern int docked_at;
extern int gRadarRange,quicksave_available;

if (quicksave_available)
{
	reinit_game_for_next_level(the_level);
	return return_val;
}
#if ZEX_LIVES_FOR_EVER
rank=3;
kills=50; promotion_points=500;
#else
gRank=0;
kills=promotion_points=0;
#endif
init_game_time();
global_request_dock=-1;
show_control_panel=0;
can_gen_cargo=1;
show_control_panel=0;
thrust_lockout=0;
global_show_target_vector=1;
fastdock_in_progress=0; fastdock_fade_value=100;
 can_gen_dust=1;
in_ramparian_space=0;
 end_of_level_code=0;
	game_over=0;
	docked_at=-1;	//not docked
    old_mothership_stock_slot_number=-1;
    showing_stats=0;
    monsters_can_fire=1;
    game_pause=0;
    if (done_load_flag==0)	//if we haven't loaded
    {
     score=0;
     if (tutorial==1)
	 {
		credits=300;
	 }
	 else
	 {
		credits=1000;
	 }
	}


    start_time=LS_TickCount60();
	ship_init();
//get files
	reset_camera();
//	the_level=2;
	level_complete=0;

    load_level(the_level);	//load up all objects etc.

    if(done_load_flag==0)
    show_level_text();	//dont show the text on a load


	#if ZEX_LIVES_FOR_EVER
	credits=3000000;
	#endif

    if (Use_Mode320==1) Mode320=1;	  //set up 320*240 screen
    init_level();
//	return_val |= load_objects();	//load objects into static arrays
	reset_ship();
	frame_counter=real_frame_counter=objs_displayed=0;
    reset_kill_list();
    reset_explode_list();
    clear_dlp();	//clear the 2d display processor
    init_zex_random();
    number_of_stars=0;
    //set camera to zex
    camera_to_zex();
    freeze=0;	//game is not frozen
    day_night_timer=60;	//set timer for day
    init_batcom();
    init_light();
	return return_val;
}

int	reinit_game_for_next_level(int the_level)
{
int	return_val=0;
//oldmac//extern FSSpec Zex_FSSpec;
extern int	zex_alive, monsters_can_fire,game_pause;
extern int frame_counter,real_frame_counter,objs_displayed,number_of_stars;
//extern int the_level;
extern int freeze,day_night_timer;
//oldmac//extern int score,start_time;
extern int credits,last_bullet_fired_frame;
extern int showing_stats,level_complete,old_mothership_stock_slot_number,last_laser_fired_frame;
extern int have_ejected;	//1 if so, keeps game alive!
extern int hide_zex,thrust,death_frame_counter,tractor,launch_timer,attack_message_timer;
extern int missile_flip;
extern int docked_at,quicksave_available;

extern _3D ship_normal;

        init_wingmen_under_command();

#if ZEX_LIVES_FOR_EVER
rank=3; kills=30; promotion_points=500;
#else
gRank=0; kills=0; promotion_points=0;
#endif
global_request_dock=-1;
show_control_panel=0;
can_gen_cargo=1;

global_show_target_vector=1;
thrust_lockout=0;
fastdock_in_progress=0; fastdock_fade_value=100;
in_ramparian_space=0;
 can_gen_dust=1;
 disable_camera_control=0;
 level_has_control=0;
 level_has_camera_control=0;
 end_of_level_code=0;
	game_over=0;
	docked_at=-1;	//not docked
    last_bullet_fired_frame=0;
    last_laser_fired_frame=0;
    old_mothership_stock_slot_number=-1;
    showing_stats=0;
    monsters_can_fire=1;
    game_pause=0;
//    score=0;
//    credits=2000;
//    alive_timer=LS_TickCount60();
//	ship_init();
//get files
	reset_camera();
//	the_level=2;
	level_complete=0;
    load_level(the_level);	//load up all objects etc.
    if (quicksave_available==0)
    show_level_text();


   if (Use_Mode320==1) Mode320=1;	 //set up 320*240 screen
    init_level();

    if (quicksave_available==1)
    {
    do_level_quicksave_restore();
    }

//	return_val |= load_objects();	//load objects into static arrays
//	reset_ship();
have_ejected=0;	//have not ejected

hide_zex=0;	//zex invisible counter
ship_normal.x=1;
ship_normal.y=0;
ship_normal.z=0;
thrust=0;
//shields=1000;
//hull=25;
zex_alive=1;
death_frame_counter=100;	//frames of circling camera - crucial we do this so ship can get killed
last_bullet_fired_frame=0;
last_laser_fired_frame=0;
tractor=0;
launch_timer=0;
attack_message_timer=0;
missile_flip=0;

	frame_counter=real_frame_counter=objs_displayed=0;
    reset_kill_list();
    reset_explode_list();
    clear_dlp();	//clear the 2d display processor
    init_zex_random();
    number_of_stars=0;
    //set camera to zex
    camera_to_zex();
    freeze=0;	//game is not frozen
    day_night_timer=60;	//set timer for day
init_light();

//check if player has a nav computer
    if (ship_items[KNAV_COMPUTER].status<=0)
    {
		added_nav=1;
     if (credits>400) credits-=400;
     else credits=0;
     ship_items[KNAV_COMPUTER].status=1;
     ship_items[KNAV_COMPUTER].obj_class=EQUIPMENT;
     ship_items[KNAV_COMPUTER].modifier=1;
    }
    else
    added_nav=0;


	return return_val;
}

//reset important engine vars
void reset_3D()
{
extern  _3D *points_list_ptr_reset;
extern  _3D *points_list_ptr;
extern textured_connections *connection_list_ptr;	//global connection list ptr
extern textured_connections *connection_list_ptr_reset;	//global connection list ptr
extern	_ZPoly *Spoly_list_ptr;
extern	_ZPoly *Spoly_list_ptr_reset;
extern int connection_list_index;
extern int points_list_index;
extern int world_points_list_index;
extern int dynamic_object_index;
extern int poly_list_index;

	connection_list_index=0;
	points_list_index=0;
	world_points_list_index=0;
	dynamic_object_index=0;
	poly_list_index=0;

	points_list_ptr=points_list_ptr_reset;	//reset static object points ptr
    connection_list_ptr=connection_list_ptr_reset;
    Spoly_list_ptr=Spoly_list_ptr_reset;

}


//¥¥must be called before loading a new level
void clear_out_dyn()
{
extern int	dynamic_object_index;
extern	DynObjectsFixedSize *ocb_ptr;
int dest_ocb;
		for (dest_ocb=0;dest_ocb<MAX_NUMBER_OF_DYN_OBJECTS;dest_ocb++)
		{
		kill(dest_ocb);
    (*ocb_ptr).object_list[dest_ocb].crashable=1;
	(*ocb_ptr).object_list[dest_ocb].in_use=0;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oworldx=0;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=0;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=0;
	(*ocb_ptr).object_list[dest_ocb].Orotdx=0;
 	(*ocb_ptr).object_list[dest_ocb].Orotdy=0;
	(*ocb_ptr).object_list[dest_ocb].Orotdz=0;
	(*ocb_ptr).object_list[dest_ocb].Orotx=0;
	(*ocb_ptr).object_list[dest_ocb].Oroty=0;
	(*ocb_ptr).object_list[dest_ocb].Orotz=0;
	(*ocb_ptr).object_list[dest_ocb].Omovedx=0;
	(*ocb_ptr).object_list[dest_ocb].Omovedy=0;
	(*ocb_ptr).object_list[dest_ocb].Omovedz=0;
	(*ocb_ptr).object_list[dest_ocb].controller=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_1=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_2=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_3=0;
	(*ocb_ptr).object_list[dest_ocb].control_use_4=0;
	(*ocb_ptr).object_list[dest_ocb].current_velocity=0;
	(*ocb_ptr).object_list[dest_ocb].wanted_velocity=0;
	(*ocb_ptr).object_list[dest_ocb].attack_mode=0;

	(*ocb_ptr).object_list[dest_ocb].object_normal.x=0;
	(*ocb_ptr).object_list[dest_ocb].object_normal.y=0;
	(*ocb_ptr).object_list[dest_ocb].object_normal.z=0;
	(*ocb_ptr).object_list[dest_ocb].shield_value=0;
	(*ocb_ptr).object_list[dest_ocb].mass=0;
//	(*ocb_ptr).object_list[dest_ocb].thrust_x=0;
//	(*ocb_ptr).object_list[dest_ocb].thrust_y=0;
	(*ocb_ptr).object_list[dest_ocb].extern_movedx=0;
	(*ocb_ptr).object_list[dest_ocb].extern_movedy=0;
	(*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;

	(*ocb_ptr).object_list[dest_ocb].just_hit=0;
	(*ocb_ptr).object_list[dest_ocb].no_trivial_rejection=0;
	(*ocb_ptr).object_list[dest_ocb].object_category=0;
	(*ocb_ptr).object_list[dest_ocb].laser_category=0;
	(*ocb_ptr).object_list[dest_ocb].cannon_category=0;
	(*ocb_ptr).object_list[dest_ocb].missile_category=0;
	(*ocb_ptr).object_list[dest_ocb].number_of_missiles=0;

	(*ocb_ptr).object_list[dest_ocb].anim_frame_rate=0;
	(*ocb_ptr).object_list[dest_ocb].anim_counter=0;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=0;	//not animated

		}
    dynamic_object_index=0;

}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//resets static ocbs


void reset_static_slots()
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	static_object_index;	//how many static objects

int i;
		for (i=0;i<MAX_NUMBER_OF_STAT_OBJECTS;i++)
		{
		(*ocb_Sptr).object_list[i].in_use=0;
		}
    static_object_index=0;

}


void load_misc_graphics()
{
//flares for galactic map screen
    small_flare_H=GetZexPicture ('RCZA',3000);	//star pict
	if (small_flare_H!=0)
	{
	LS_HLock(small_flare_H);
	small_flare_picture = (LSRAW*)*small_flare_H;
    }
    else
        report_error_c("Not enough memory or resource missing.","RCZA",3000);

    large_flare_H=GetZexPicture ('RCZA',3001);	//star pict
	if (large_flare_H!=0)
	{
	LS_HLock(large_flare_H);
	large_flare_picture = (LSRAW*)*large_flare_H;
    }
    else
        report_error_c("Not enough memory or resource missing.","RCZA",3001);

}





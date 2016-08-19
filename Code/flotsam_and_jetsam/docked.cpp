//docked.c
//SB 300199
/*$Id*/

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <memory.h>
//oldmac//#include <timer.h>
//oldmac//#include <files.h>
//oldmac//#include <NumberFormatting.h>
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
#include "drawing.h"
#include "buy.h"
#include "inventory.h"
#include	"buy_int.h"
#include "instrum_int.h"
#include "file_io.h"
#include "game_defs.h"
#include "Batcom.h"
#include "light_control.h"
#include "obj_control_misc_routines.h"
#include "handle_level.h"
#include	"txtsplat.h"
#include "docked.h"
#include "Galaxy.h"
#include "real_time.h"
#include "map_control.h"
#include "command_and_control.h"
#include "Error_handler.h"
#include "compile_zmap.h"
#include "3d_top.h"
#include	"object_control_top.h"
#include "OGL.h"
#include "texture_management.h"
#include "game_save.h"
#include "CPanel.h"

int comms_will_fail,docked_at_time;

void render_frame(int width, int height);
void setup_docked_screen(LSRAW* the_picture, LSRAW* the_button, LSRAW* the_button_off);
int handle_docked_mouse(LSRAW* button_down, LSRAW* button_up);
void docked_load_ship(int static_obj, int dyn_slot, float x, float y, float z, int rotz);
void show_ratings();
void process_sleeping();

//oldmac//extern DSpContextReference		mDisplayContext;
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

extern int warphole_slot;

extern int hull_strength,do_next_level,level_complete,can_do_next_level;
extern int wingmen_guarding_what_object,training_mission,tutorial;
extern int docked_at,docked_at_shields;	//slot number
extern float docked_at_power_rating;
extern	DynObjectsFixedSize *ocb_ptr;

void do_docked(int mothership_cat)	 //0 for earth, 1 for alien
{
	extern int music_volume;
	extern _3D camera_pos;
	extern int camera_rotation_z;	//obviously the camera rot
	extern int camera_rotation_x;
	extern int camera_rotation_y;
	extern int camera_rotation_delta_z; //set this to !0 and caamera rots around z
	extern _3D camera_pos_delta;
	extern	float SinTable[360], CosTable[360];
	extern _3D light_normal;
	extern int level_complete,thrust,retro;
	extern	DynObjectsFixedSize *ocb_ptr;
	extern int do_launch;
	extern int dynamic_object_index;
	extern int tractor,showing_stats,level_complete;
	extern int launch_timer;
	extern int ship_z_ang;
	extern kit_desc ship_items[ITEMS_COUNT];
	extern int camera_mode,fastdock_in_progress,fastdock_fade_value, global_show_status;
	
	
	
	_3D light_normal_save;
	Handle button_data_H;
	Handle select_data_H;
	Handle button_off_H;
	Handle really_quit_H;
	Handle quit_H;
	
	LSRAW *the_picture;
	LSRAW *button;
	LSRAW *button_off;
	LSRAW *really_quitp;
	LSRAW *quitp;
	
	Handle button_down_data_H;
	LSRAW *button_down;
	int wait_flag,player_click;
	int ship_dy=0;
	int inc=1,really_quit=0;
	unsigned char world_objects_in_use_state [MAX_NUMBER_OF_DYN_OBJECTS];
	int i,old_hull_strength;
	int docked_objects_slot1,docked_objects_slot2;	//we use two objects
	int saved_dynamic_object_index;
	float ship_x,ship_y;
	//LS_UnsignedWide start_micros,end_micros;
	int saved_light_amplitude;
	int ms_shields;
	int sleeping_flag;
	Rect top_sleep_rect = {0,0,0,639};
	Rect bottom_sleep_rect= {478,0,479,639};
	int ship_slot,bg_slot;
	int err;
	//first setup the stock
	//first do busy network
	//show_text_in_loading_screen("Zex: do_docked");
	
	fastdock_in_progress=0; fastdock_fade_value=100;
	
    play_simple_continuous_sound(tractor_beam_loop, 44, 0.25);
	kill_all_dust();
	
	//stand down any wingmen
	stand_down_wingmen();	//in C&C
	
	sleeping_flag=0;
	
	//now do mothership damage
	ms_shields=docked_at_shields;
	if (docked_at_shields<8000 && RangedRdm(0,docked_at_shields)<3000) comms_will_fail=2;	//cant connect because of damage to mship

	//if we are within 1 minute of the last dock, then the network will be busy <G>
	//if (LS_TickCount60()<docked_at_time+(60*60)) comms_will_fail=1;
	//else	//this prevents the guy docking every minute reestting the timer
	docked_at_time=LS_TickCount60();	//we also use this to determine whether if failed, we can now connect (5 mins)
	if (training_mission!=0 || tutorial!=0)
	{
		//do nothing if we aren't running for real (Bug found on Intel build, PPC is fine writing to 0!)
	}
	else
	{
		(*ocb_ptr).object_list[docked_at].mship_ptr->visited=1;
	}
	setup_stock(docked_at);
	remove_batcom_message();
	
	tractor=0;	//not tractoring anymore
	do_launch=0;	//zex needs this. Set to 1 if relaunch ship
	
	saved_light_amplitude=get_light_amplitude();
	
	//reset all controllers
    for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
    {
		if ((*ocb_ptr).object_list[i].in_use==1)
		{
			(*ocb_ptr).object_list[i].control_use_3=0;
			(*ocb_ptr).object_list[i].control_use_4=0;
			(*ocb_ptr).object_list[i].current_velocity=0;
			(*ocb_ptr).object_list[i].wanted_velocity=0;
		}
    }
	
	//make a note of which objects are in use
    for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
    {
		world_objects_in_use_state[i]=(*ocb_ptr).object_list[i].in_use;
    }
	saved_dynamic_object_index=dynamic_object_index;	//VERY important we save and restore this
														//if we want to run the old objects again
	
	//now load objects making up scene
    level_complete=0;	//reset engine
	
    light_normal_save=light_normal;
	light_normal.z=0.0;	//dim the scene
    light_normal.x=0;
    
//    if (training_mission==0 && tutorial==0)
		select_data_H=GetZexPicture ('RCZ ',2002);	//docked_bg   
//	else
//		select_data_H=GetZexPicture ('RCZ ',2114);	//docked_bg no next mission   
	
	LS_HLock(select_data_H);
	the_picture = (LSRAW*)*select_data_H;
	
    button_data_H=GetZexPicture ('RCZ ',132);	//prefs_button_up   
	LS_HLock(button_data_H);
	button = (LSRAW*)*button_data_H;
	
    button_off_H=GetZexPicture ('RCZ ',140);	//prefs_button_up_off   
	LS_HLock(button_off_H);
	button_off = (LSRAW*)*button_off_H;
	
    really_quit_H=GetZexPicture ('RCZ ',3200);	//really quit grafic   
	LS_HLock(really_quit_H);
	really_quitp = (LSRAW*)*really_quit_H;
	
    quit_H=GetZexPicture ('RCZ ',3201);	//quit grafic   
	LS_HLock(quit_H);
	quitp = (LSRAW*)*quit_H;
	
	
    button_down_data_H=GetZexPicture ('RCZ ',133);	//prefs_button_down   
	LS_HLock(button_down_data_H);
	button_down = (LSRAW*)*button_down_data_H;
	
	
	ship_slot=get_static_object_slot('ZEXD');
	
	if (mothership_cat==zex_object)
	{
        bg_slot=get_static_object_slot('BGTE');
	}
	else
	{
        bg_slot=get_static_object_slot('BGTA');
	}
	
	//find out where these objects are going
    docked_objects_slot1=find_vacant_dynamic_object_slot();
    if (docked_objects_slot1==-1) report_error_c("Internal Failure during dock. Please Contact LSG","",-4);
    docked_load_ship(ship_slot,docked_objects_slot1,0,-170,-280,-90);	//Zex
    docked_objects_slot2=find_vacant_dynamic_object_slot();
    if (docked_objects_slot2==-1) report_error_c("Internal Failure during dock. Please Contact LSG","",-4);
	docked_load_ship(bg_slot,docked_objects_slot2,0,0,1,0);	//docking bay floor
    dynamic_object_index=2;	//we havetwo objects in docked view
    
	//now mark all those in the wolrd_objects_in_use array as not in_use
    for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
    {
		if (world_objects_in_use_state[i]==1) (*ocb_ptr).object_list[i].in_use=0;
    }
	
	
	//now render the scene
	ship_y=-170;	
	ship_x=0;
	
	camera_pos.x=0;
	camera_pos.y=320;	//300	
	camera_pos.z=-950;	//-950
    camera_rotation_delta_z=0;
	
    camera_rotation_z=00;
    camera_rotation_x=18;
    camera_rotation_y=00;
	
    setup_docked_screen(the_picture,button,button_off);
    setup_docked_screen(the_picture,button,button_off);
	//    setup_docked_screen(the_picture,button,button_off);
	
    wait_flag=0;
    set_poly_clipping(0,364,0,479);	//set poly clip rect - sb 181298
    camera_pos_delta.z=2;
	
    LS_ShowCursor();
    set_light_amplitude(32000);
    
	do_fixed_frame_rate(FRAME_RATE_REINIT);
	
	//¥¥¥main title screen display loop
	while (wait_flag==0)	//until mouse control says play
    {
		int hour;
		
		if (really_quit>0) really_quit--;
		
		if (really_quit==1)	//put back End Game text
		{
			print_crunched_pict(quitp,438, 326);
			Show_it();
			Get_back_buffer();
			print_crunched_pict(quitp,438, 326);
			
		}
		
		//      Microseconds(&start_micros);	 //equalise
		//      if (RangedRdm(1,100)>95 && docked_at_shields<500)
		if (training_mission==0 && tutorial==0)
		{ 
			hour=get_hours();
			if (hour>20 || hour<7)
				light_normal.z=3.9;	//dim the scene
			else
				light_normal.z=0.55;	//normal
		}
		else
			light_normal.z=0.55;	//normal
		
		ship_dy+=inc;
		if (ship_dy>359) 
		{
			ship_dy-=360;
			if (RangedRdm(0,10)>5) inc=-inc;
		}
		if (ship_dy<0) 
		{
			ship_dy+=360;
			if (RangedRdm(0,10)>5) inc=-inc;
		}
		
		ship_y+=SIN(ship_dy)*.7;
		ship_x+=COS(ship_dy)*.7;
		(*ocb_ptr).object_list[docked_objects_slot1].Oworldy=ship_y;
		(*ocb_ptr).object_list[docked_objects_slot1].Oworldx=ship_x;
		
		camera_pos.x= 	 (*ocb_ptr).object_list[docked_objects_slot1].Oworldx;
		//      camera_pos.z+=camera_pos_delta.z;
		
		if (camera_pos.z>-950) camera_pos_delta.z=-camera_pos_delta.z;
		if (camera_pos.z<-1000) camera_pos_delta.z=-camera_pos_delta.z;      
		
		// 	(*ocb_ptr).object_list[0].Orotx=SIN(ship_dy)*7;
		
		if (training_mission==0 && tutorial==0)
		{
			if (sleeping_flag<200)
			{
				run_animations();	//in animation.c
				render_frame(364,480);	//calls all the needed 3d stuff
			}
		}
		else
		{
			run_animations();	//in animation.c
			render_frame(364,480);	//calls all the needed 3d stuff
			
		}
		//show ship callsign and nearest planet
		//      if (training_mission==0)
		//         {
		//         //get callsign
		//         unsigned long callsign;
		//         char cs_string[32];
		//         
		////         get_callsign_and_name(cs_string,docked_at);
		////             #define textcol 246
		////             SplatText_Transparent_Fade ((char *)cs_string,380,115,textcol);
		//
		//         }
		
		
		if (sleeping_flag==0)
		{
			player_click=handle_docked_mouse(button_down,button);
			
			if (player_click==1)	//sleep
			{
				//     fade_out_music();
				sleeping_flag=1;
			}
			else
				if (player_click==5)	//save game 
				{
					untake_mouse_keyboard_as_input();
					save_game_as();
					setup_docked_screen(the_picture,button,button_off);
					setup_docked_screen(the_picture,button,button_off);
					//    setup_docked_screen(the_picture,button,button_off);
					take_mouse_keyboard_as_input();
					LS_ShowCursor();
				}
			else
				
				if (player_click==6)	//abort game, change to Really? and prime
				{
					if (really_quit==0)
					{
						really_quit=100;
						print_crunched_pict(really_quitp,438, 326);
						Show_it();
						Get_back_buffer();
						print_crunched_pict(really_quitp,438, 326);
						
					}
					else
					{
					extern int zex_alive, user_abort, show_control_panel;
//change
					wingmen_guarding_what_object=0;
					LS_HideCursor();
					wait_flag=1;	//local terminate loop variable
					
					
					do_launch=1;	//tell zex to carry on...
					zex_alive=0;	//but that we have died
					user_abort=1;	//and it was a user end game
					
					//kill these objects and re-enable those that were active when we docked
					kill(docked_objects_slot1);
					kill(docked_objects_slot2);
					//re-enable the world
					//    load_object ("zex_game.z",zex,'ZEXS',yes_colldet,'T009',0);
					for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
					{
						if (world_objects_in_use_state[i]==1) (*ocb_ptr).object_list[i].in_use=1;
					}
					//240699 - set zex not being docked...
					
					dynamic_object_index=saved_dynamic_object_index;	//restore how many were active
					calc_current_quad();
					
					
					thrust=0;
					ship_z_ang=90;	//facing up
					(*ocb_ptr).object_list[0].extern_movedy=0;
					(*ocb_ptr).object_list[0].docked_object_slot=-1;	//deff. not docked
					(*ocb_ptr).object_list[0].docked=0;	//deff. not docked
					(*ocb_ptr).object_list[0].request_dock=0;	//deff. not docked
					(*ocb_ptr).object_list[0].eng_invisible=1;	//don't want to see the ship
					
					//     (*ocb_ptr).object_list[zex].Oworldy=(*ocb_ptr).object_list[zex].Oworldy-60000;
					
					old_hull_strength=hull_strength;
					reinit_screen();	//resets hull strength...
					hull_strength=old_hull_strength;
					set_poly_clipping(0,639,0,388);	//set poly clip rect - sb 181298
													// 	(*ocb_ptr).object_list[zex].Oworldy=ship_y;
					showing_stats=0;	//reenable monster control
										//    if (level_complete==3) level_complete=2;
										//     i=tractor;
					launch_timer=400;	//for 100 frames disable docking. Dec'd in ship_control
					 tractor=0;

					ship_items[KSHIELDS].status=1;
					ship_items[KSHIELDS].value1=1000;
					do_level_reinit();

					camera_rotation_z=3;
					camera_rotation_x=0;
					camera_rotation_y=00;
					camera_pos.z=-2000;
					 set_light_amplitude(65530);	//32767 is max
					 fade_and_play_zex_resource(132);	 //Spring by Purple Motion

					 camera_rotation_delta_z=3;

#if OGL==1
					ogl_active();
					err=aglGetError();
					if (err!=0) report_error_c ("do_docked(1): AGL error.","",err);
					
					load_ogl_textures();
					err=aglGetError();
					if (err!=0) report_error_c ("do_docked(2): AGL error.","",err);
					load_ogl_dynamic_textures();
					if (err!=0) report_error_c ("do_docked(3): AGL error.","",err);
					user_delay(70);
#endif
					
					if (warphole_slot!=-1)           insert_continuous_sound(warphole_slot, rumble_loop,65);
					
					(*ocb_ptr).object_list[0].Oworldy+=5000;
					(*ocb_ptr).object_list[docked_at].control_use_9+=100+RangedRdm(0,500);	//set launch timer
					
					docked_at=-1;
					
						LS_HideCursor();
						wait_flag=1;
					}
				}
			else
				if (player_click==4)	//buy 
				{
					set_poly_clipping(0,639,0,479);

					buy(docked_at);	//file buy.c
					setup_docked_screen(the_picture,button,button_off);
					setup_docked_screen(the_picture,button,button_off);
					//    setup_docked_screen(the_picture,button,button_off);
				}
			
			else
				if (player_click==2)	//leave dock
				{
					wingmen_guarding_what_object=0;
					//     camera_mode=0;
					//     fade_out_music();
					//     play_tune(130);	  //main theme - starshine by Purple Motion
					//     music_set_vol(music_volume);
					LS_HideCursor();
					wait_flag=1;	//local terminate loop variable
					
					
					do_launch=1;	//tell zex to carry on...
									//kill these objects and re-enable those that were active when we docked
					kill(docked_objects_slot1);
					kill(docked_objects_slot2);
					//re-enable the world
					//    load_object ("zex_game.z",zex,'ZEXS',yes_colldet,'T009',0);
					for (i=0;i<MAX_NUMBER_OF_DYN_OBJECTS;i++)
					{
						if (world_objects_in_use_state[i]==1) (*ocb_ptr).object_list[i].in_use=1;
					}
					//240699 - set zex not being docked...
					
					dynamic_object_index=saved_dynamic_object_index;	//restore how many were active
					calc_current_quad();
					
					force_fuel_update();
					play_level_music();
					
					thrust=0;
					//     retro=1100;
					ship_z_ang=90;	//facing up
					(*ocb_ptr).object_list[0].extern_movedy=600;	//eject ship
					(*ocb_ptr).object_list[0].docked_object_slot=-1;	//deff. not docked
					(*ocb_ptr).object_list[0].docked=0;	//deff. not docked
					(*ocb_ptr).object_list[0].request_dock=0;	//deff. not docked
					
					//     (*ocb_ptr).object_list[zex].Oworldy=(*ocb_ptr).object_list[zex].Oworldy-60000;
					
					old_hull_strength=hull_strength;
					reinit_screen();	//resets hull strength...
					hull_strength=old_hull_strength;
					set_poly_clipping(0,639,0,388);	//set poly clip rect - sb 181298
													// 	(*ocb_ptr).object_list[zex].Oworldy=ship_y;
					showing_stats=0;	//reenable monster control
										//    if (level_complete==3) level_complete=2;
										//     i=tractor;
					launch_timer=400;	//for 100 frames disable docking. Dec'd in ship_control
					camera_rotation_z=00;
					camera_rotation_x=0;
					camera_rotation_y=00;
					do_level_reinit();
#if OGL==1
					ogl_active();
					err=aglGetError();
					if (err!=0) report_error_c ("do_docked(1): AGL error.","",err);
					
					load_ogl_textures();
					err=aglGetError();
					if (err!=0) report_error_c ("do_docked(2): AGL error.","",err);
					load_ogl_dynamic_textures();
					if (err!=0) report_error_c ("do_docked(3): AGL error.","",err);
					user_delay(70);
#endif
					
					play_zsound_always(launch, sound_high_pri, c1_chan1, sound_vol_7);
					if (warphole_slot!=-1)           insert_continuous_sound(warphole_slot, rumble_loop,65);
					
					//     (*ocb_ptr).object_list[docked_at].launching_zex=100;	  //set launch timer
					(*ocb_ptr).object_list[0].Oworldy+=2000;
					(*ocb_ptr).object_list[docked_at].control_use_9+=100+RangedRdm(0,500);	//set launch timer
					
					docked_at=-1;
					
					goto get_out_no_render;
				}	//end of launch
			else
				if (player_click==3)	//inventory 
				{
					
					set_poly_clipping(0,639,0,479);	//set poly clip rect - rp 01-08-2005
					
					do_inventory();	//file buy.c - wait for click
					global_show_status=0;
					bool done=false;
					bool button_down=false;
					while(LS_Left_Button()==1);	//wait for mouse to come up
					while(done==false)
					{
						if (LS_Left_Button()==1) button_down=true;

						if (button_down==true) //if button has been down
						{
							if (LS_Left_Button()==0) done=true;
						}
							do_inventory_single_frame(true);
							Show_it();
							SDL_Delay(20);
						
					}
					global_show_status=0;
					setup_docked_screen(the_picture,button,button_off);
					setup_docked_screen(the_picture,button,button_off);
					

					//    setup_docked_screen(the_picture,button,button_off);
				}
			
		}	//end of sleeping flag==0    
		
		if (sleeping_flag>0)
		{
			if (sleeping_flag<240)	//rectangles
			{
				RGBForeColor(&rgbBlack);
				top_sleep_rect.bottom=sleeping_flag;
				PaintRect(&top_sleep_rect);
				bottom_sleep_rect.top=479-sleeping_flag;
				PaintRect(&bottom_sleep_rect);
				
				sleeping_flag+=10;
			}
			else
				if (sleeping_flag>=240 && sleeping_flag<299)	//D'OH!
				{
					process_sleeping();
					sleeping_flag=300;	//exit sleep
										//     sleeping_flag++;
										//       top_sleep_rect.bottom=240;
										//       PaintRect(&top_sleep_rect);
										//       bottom_sleep_rect.top=240;
										//       PaintRect(&bottom_sleep_rect);
					
				}
			
			else if (sleeping_flag==300)
			{
				setup_docked_screen(the_picture,button,button_off);
				setup_docked_screen(the_picture,button,button_off);
				sleeping_flag=0;
			}
		}
		
		Show_it();	//splat 3d portion of window
		
		//Microseconds(&end_micros);
		//while (end_micros.lo-start_micros.lo <1000000/MAX_FRAME_RATE)	//fps
		//    Microseconds(&end_micros);
		do_fixed_frame_rate(FRAME_RATE_DELAY);
    }
get_out_no_render:
		
		stop_simple_continuous_sound();
	
    set_light_amplitude(saved_light_amplitude);
	
	LS_DisposeHandle(select_data_H);
	LS_DisposeHandle(button_data_H);
	LS_DisposeHandle(button_off_H);
	LS_DisposeHandle(really_quit_H);
	LS_DisposeHandle(quit_H);
	LS_DisposeHandle(button_down_data_H);
	
	return;
}



void process_sleeping()
{
//oldmac//extern GDHandle				mZexGDH;
extern int sleep_timer;

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
int quit;
            #define textcol 246	//white
int button_down;
int rand_val;

rand_val=RangedRdm(0,100);

button_down=0;

while (LS_Left_Button()) { SDL_Delay(10); Show_it(); } // give computer back time and allow mouse to move (if on screen)


if (sleep_timer > MAX_AWAKE-(38*60*60*4) )	 //been awake for less than 4 hours?
{
quit=0;
while (quit==0)
 {
    //oldmac//GetPort (&savePort);
	//oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
	//oldmac//SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures

	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
        if (rand_val>50)
	SplatText_Transparent_Fade ("You are not tired enough to sleep.",208,184,textcol);
	else
	SplatText_Transparent_Fade ("You try hard to fall asleep, but you simply are not tired.",120,184,textcol);
        draw_real_time(280,200,textcol);            
        sleep_timer+=38*60;

	SplatText_Transparent_Fade ("Click Mouse Button To Continue.",220,284,textcol);
        
if (LS_Left_Button()==1) button_down=1;

if (button_down==1) 
     {
     if (LS_Left_Button()==0) quit=1;
     }
     
    Show_it();
  }

}
else
{
//do sleep, show time incrementing very fast
quit=0;
while (quit==0)
 {
    //oldmac//GetPort (&savePort);
	//oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
	//oldmac//SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures

	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
	add_a_minute_to_real_time();
	SplatText_Transparent_Fade ("You are asleep.",270,184,textcol);
        draw_real_time(280,200,textcol);            
        sleep_timer+=38*60;

        if (sleep_timer > MAX_AWAKE ) quit=1;
	SplatText_Transparent_Fade ("Click Mouse Button To Wake Up.",220,284,textcol);
        
if (LS_Left_Button()==1) button_down=1;

if (button_down==1) 
     {
     if (LS_Left_Button()==0) quit=1;
     }
     
    Show_it();
  }
}

}


int old_button_state=0;
int button_y2_array[6] = 
    {135,	//next mission
    172,	//Leave dock
    209,	//show status
    246,	//buy
    285,	//save
    322,	//quit
    };
    
void setup_docked_screen(LSRAW* the_picture, LSRAW* the_button, LSRAW* the_button_off)
{
extern int button_y2_array[6];
extern const RGBColor	rgbBlack;
extern Ptr screenaddr;
extern int screenwidth;
//oldmac//extern GDHandle				mZexGDH;

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
	set_poly_clipping(0,364,0,479);	//set poly clip rect - sb 181298
         
	Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
//oldmac//    GetPort (&savePort);
//oldmac//	saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//oldmac//	SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures

//oldmac//	SetGDevice(saveGDH);	//these are here so I can SEE them!
//oldmac//	SetPort(savePort);
//    screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
//    screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;
	print_crunched_pict(the_picture,0,0);
	
	if (can_do_next_level==1)	//if we can go to next mission
	print_crunched_pict(the_button,380, button_y2_array[0]);
    else
	print_crunched_pict(the_button_off,380, button_y2_array[0]);
	
	print_crunched_pict(the_button,380, button_y2_array[1]);
	print_crunched_pict(the_button,380, button_y2_array[2]);
	print_crunched_pict(the_button,380, button_y2_array[3]);
	print_crunched_pict(the_button,380, button_y2_array[5]);
//	print_crunched_pict(the_button,380, button_y2_array[5]);
	if (can_do_next_level==1)	//if we can go to next mission
	print_crunched_pict(the_button,380, button_y2_array[4]);
    else
	print_crunched_pict(the_button_off,380, button_y2_array[4]);

        show_ratings();             

//	print_crunched_pict(the_button,380, button_y2_array[6]);

	Show_it();

}

void show_ratings()
{
char cs_string[32];
int temp;
unsigned char number_string[12]; 
galactic_stats the_stats;
extern int gRank,promotion_points,kills,tutorial, credits;

             #define textcol 246

if (training_mission!=0 || tutorial!=0)
{
             SplatText_Transparent_Fade ("Save Game, Rest and Galactic",410,380+20,textcol);
             SplatText_Transparent_Fade ("Stats not available in",430,393+20,textcol);
             SplatText_Transparent_Fade ("training missions.",450,405+20,textcol);

}
else
{
         get_callsign_and_name(cs_string,docked_at);
             #define textcol 246
             SplatText_Transparent_Fade ((char *)cs_string,380,115,textcol);
//show stats
             get_galactic_stats(&the_stats);


             SplatText_Transparent_Fade ("Motherships:",376,380,textcol);
             SplatText_Transparent_Fade ("Supply Rating:",376,393,textcol);
             SplatText_Transparent_Fade ("Power Rating:",376,406,textcol);
             
			 #undef textcol
             #define textcol 225	//earth green
             ZexNumToString(the_stats.number_of_earth_mships,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack
             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,500,380,textcol);

			 #undef textcol
             #define textcol 215	//alien red
             ZexNumToString(the_stats.number_of_alien_mships,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack
             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,560,380,textcol);

			 #undef textcol
             #define textcol 225	//earth green
             ZexNumToString(the_stats.total_earth_supply_rating,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack
             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,500,393,textcol);

             #undef textcol
             #define textcol 215	//alien red
             ZexNumToString(the_stats.total_alien_supply_rating,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack
             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,560,393,textcol);

             #undef textcol
             #define textcol 225	//earth green
             ZexNumToString(the_stats.total_earth_power_rating,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack
             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,500,406,textcol);

			 #undef textcol
			 #define textcol 215	//alien red
             ZexNumToString(the_stats.total_alien_power_rating,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack
             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,560,406,textcol);

			 #undef textcol
             #define textcol 246	//white
             //print rank
             SplatText_Transparent_Fade ("Rank:",376,424,textcol);
			 SplatText_Transparent_Fade (rank_to_string(gRank),500,424,textcol);

             SplatText_Transparent_Fade ("Promotion Points:",376,437,textcol);
             
             ZexNumToString(promotion_points,number_string);
             number_string[number_string[0]+1]='/';	//quick p to c hack
             
             if (gRank==ENS)	//these values must match those in map_control, run_map_control
             {
             number_string[number_string[0]+2]='3';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }
             else
             if (gRank==LT)
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='5';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]=0;	//quick p to c hack
             }
            
             else
             if (gRank==LTCR)
             {
             number_string[number_string[0]+2]='6';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]=0;	//quick p to c hack
             }
             else
             if (gRank==CDR)
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='8';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==CAPT)
             {
             number_string[number_string[0]+2]='3';	//quick p to c hack
             number_string[number_string[0]+3]='5';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==RADM)
             {
             number_string[number_string[0]+2]='6';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==VADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='8';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]=0;	//quick p to c hack
             }
             else
             if (gRank==ADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='1';	//quick p to c hack
             number_string[number_string[0]+3]='0';	//quick p to c hack
             number_string[number_string[0]+4]='0';	//quick p to c hack
             number_string[number_string[0]+5]='0';	//quick p to c hack
             number_string[number_string[0]+6]='0';	//quick p to c hack
             number_string[number_string[0]+7]='0';	//quick p to c hack
             number_string[number_string[0]+8]=0;	//quick p to c hack
             }
             else
             if (gRank==FADM)	//as high as it goes baby
             {
             number_string[number_string[0]+2]='N';	//quick p to c hack
             number_string[number_string[0]+3]='/';	//quick p to c hack
             number_string[number_string[0]+4]='A';	//quick p to c hack
             number_string[number_string[0]+5]=0;	//quick p to c hack
             }




             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,500,437,textcol);

             SplatText_Transparent_Fade ("Kills:",376,450,textcol);
             
             ZexNumToString(kills,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack

             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,500,450,textcol);
//display time
             SplatText_Transparent_Fade ("Credits:",376,463,textcol);
             ZexNumToString(credits,number_string);
             number_string[number_string[0]+1]=0;	//quick p to c hack

             number_string[0]=' ';
             SplatText_Transparent_Fade ((char *)number_string,500,463,textcol);

//             draw_real_time(507,463,textcol);            
             
 }
}

int handle_docked_mouse(LSRAW* button_down, LSRAW* button_up)
{
//oldmac//extern	  DSpContextReference		          mDisplayContext;
extern int button_y2_array[6];
extern int music_volume;
extern int sound_volume;

int button_state;
Point mouse_xy;
int return_val=0;
int real_return_val,i,k;
unsigned char number_string[2] = {0,0};
extern int old_button_state;
extern int quit;
//copy bg for draw_bg
//	GetMouse(&mouse_xy);
    ZexGetMouse (&mouse_xy);
//    DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
    button_state=LS_Left_Button();
//first draw all buttons as off
    if ((training_mission==0) && (tutorial==0)) k=0; else k=1;
    for (i=k; i<6;i++)
    if ((training_mission==0) && (tutorial==0))	//if we can go to next mission
	print_crunched_pict(button_up,380, button_y2_array[i]);
    else
    {
    if(i!=4)	print_crunched_pict(button_up,380, button_y2_array[i]);
    }
//        print_crunched_pict(button_up,380,button_y2_array[i]);
//now draw the down one
    for (i=k; i<6;i++)
    {
     if (i==4 && (training_mission || tutorial==1)) i=5;
     if (mouse_xy.v>button_y2_array[i] && mouse_xy.v<button_y2_array[i]+36 )	//controls
     {
     print_crunched_pict(button_down,380,button_y2_array[i]);
     return_val=i+1;
     }
    }
////check for keyboard shortcuts
//if (check_key(0x2d)) { real_return_val=3; goto end; }	//N key
//if (check_key(0x01)) { real_return_val=1; goto end; }	//S key
//if (check_key(0x08)) { real_return_val=6; goto end; }	//C key
//if (check_key(0x0C)) { real_return_val=7; goto end; }	//Q key


if (button_state==1 && old_button_state==0)	//if up and was down
real_return_val = return_val;
else
real_return_val=0;
old_button_state=button_state;

end:
//process real_return_val
//if (real_return_val==1)
//{
//LS_HideCursor();
//
//GraphicsPaused();
//LS_FlushEvents(-1,0);	//flush all
//
//configure_input_device();
//LS_FlushEvents(-1,0);	//flush all
//
//GraphicsActive();
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//
//    LS_ShowCursor();
//}
//
//if (real_return_val==6)
//{
//	  Show_it();
//     display_wait();	  //
//    do_creds_screen();
//
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//    setup_main_screen(the_picture,button_up);
//
//real_return_val=6;
//}
//
////quit game
//if (real_return_val==7)
//{
//quit=1;
//}

return real_return_val;
}


void docked_load_ship(int static_obj, int dyn_slot, float x, float y, float z, int rotz)
{
load_dyn_object_pb the_params;
//int drot_z;
extern int dynamic_object_index;

clear_pb(&the_params);	//saves loads of zero code
//    reset_objects();
//set up ship for title screen
	the_params.world_x=x;
	the_params.world_y=y;
	the_params.world_z=z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
	the_params.rot_z=rotz;
//	the_params.controller_ref=0;
//	the_params.control_use_1=0;
//	the_params.control_use_2=0;
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.attack_mode=0;
//	the_params.normal.x =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
  //   the_params.has_roll=0;
  	the_params.mass=100;
//		the_params.dq=0;
//	the_params.no_trivial_rejection=0;
	    the_params.object_category=zex_object;
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
//	the_params.unique_id=0;

     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_obj,dyn_slot,&the_params,-1);	//the object, the position (-1=next free)
}

//load_animated_ship(int static_obj, unsigned long frame1_id, unsigned long frame2_id, 
//                   unsigned long frame3_id, unsigned long frame4_id)
//{
//load_dyn_object_pb the_params;
//int drot_z;
//extern int dynamic_object_index;
//anim_pb anim;
//    kill(0);	  //clear out old ship
//
////    reset_objects();
//    anim.object_id[0]=frame1_id;
//    anim.object_id[1]=frame2_id;
//    anim.object_id[2]=frame3_id;
//    anim.object_id[3]=frame4_id;
//
////set up ship for title screen
//	  the_params.world_x=00;
//	  the_params.world_y=0;
//	  the_params.world_z=0;
//	  the_params.Omovedx=0;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=0;
//	  the_params.delta_rot_y=0;
//	  drot_z=RangedRdm(0,8)-4;
//	  if (drot_z==0) drot_z=4;
//	  
//	  the_params.delta_rot_z=drot_z;
//	  the_params.rot_x=0;
//	  the_params.rot_y=0;
//	  the_params.rot_z=0;
//	  the_params.controller_ref=0;
//	  the_params.control_use_1=0;
//	  the_params.control_use_2=0;
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.control_use_5=0;
//	  the_params.wanted_velocity=0;
//	   the_params.attack_mode=0;
//	  the_params.normal.x =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  -1;	//objects are ALWAYS defined facing right
//	  the_params.shield_value=1000;
//	  the_params.mass=100;
//	  the_params.no_trivial_rejection=0;
//	      the_params.object_category=zex_object;
//
////	load_dyn_object(static_obj,0,&the_params);	//the object, the position (-1=next free)
//    load_animated_object (&anim,4,3,1,1,&the_params);
//
//}


void get_callsign_and_name(char * cs_string, int which_ocb)
{
unsigned long callsign;
int supply_planet;
extern int current_quad;
unsigned char * src_name;
int src_name_len;
extern galactic_map *GM_ptr;	//ptr to the galactic map
int i;

         callsign=(*ocb_ptr).object_list[which_ocb].mship_ptr->callsign;
             
//              cs_string[4]=0;
		 safe_number_id_to_4_character_string(callsign, cs_string);
              cs_string[4]='-';
              supply_planet=(*ocb_ptr).object_list[which_ocb].mship_ptr->supply_planet_index; 
              src_name=GM_ptr->the_quadrant[current_quad].quadrant_planets[supply_planet].name;
              src_name_len=src_name[0];
for(i=5;i<src_name_len+5;i++)
   {
   cs_string[i]=src_name[i-4];
   }
   cs_string[i]=0;
}



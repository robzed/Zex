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

/*
Level #:5 Warphole
Auth: Rob n Stu
Date:
Rev:
 */
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "3d_structures_def.h"
#include "objects_controls.h"	//contains monster defs
#include "load_objects.h"
#include "object_misc.h"
#include "compile_zmap.h"
#include "Error_handler.h"
#include "zex_misc.h"
#include "dlp.h"
#include "instrum_int.h"
#include "wingman.h"
#include "monster_control_misc.h"
#include "level_control.h"
#include "ship_control.h"
#include	"camera.h"
#include "explode.h"
#include "object_control_top.h"
//oldmac//#include <NumberFormatting.h>
#include "angular_stuff.h"
#include "Batcom.h"
#include "pictures.h"
#include "game_pause.h"
#include "sound.h"
#include "object_misc.h"
#include	"drawing.h"
#include "buy_int.h"
#include "obj_control_misc_routines.h"
#include "motherships.h"

extern int can_gen_dust;
extern int done_monsters;	//used to control when we set off monsters
extern int LFrame_counter,total_frame_counter;	//timer
extern int next_ast_time;
extern int interval;	//how long before we gen a monster
extern	DynObjectsFixedSize *ocb_ptr;
extern	float SinTable[360], CosTable[360];	//fancy see this here!!!
extern int var_a,tractor;
extern int wingmen_guarding_what_object;	//the object wingmen are guarding
extern int gRadarRange,thrust;
extern int    global_open_doors;
extern int dynamic_object_index;
extern int level_has_control,level_has_camera_control;
extern float camera_zoom;
extern _3D camera_pos;
extern int camera_rotation_z;	//obviously the camera rot
extern int camera_rotation_x;
extern int camera_rotation_y;
extern int can_do_next_level;
extern int in_wormhole;
extern int in_ramparian_space;
extern int mothership_core_slot;
extern int wingmen_guarding_what_object,game_running;	//the object wingmen are guarding

int warphole_core_slot,warphole_trunk;
int entered_hole;
unsigned int obc_warning_timer;
int gen_dust_timer;
int over_hole;
int vib;
int roty;
int shooter1_slot,shooter2_slot,star_slot,mon2_slot,ball1_slot,gala_slot,ring_slot;
int timer_5,timer_6,cy_delta;
int frame_count,cam_rot_d;
int star_rot,sphere_rot;
int rumble_volume;
int midi_note,inc_midi;
int end_scene;
int bar_slot,bar_alive;

unsigned char bg_colour;

void Level5();
void init_level5();
void quicksave_level5();
void restore_quicksave_level5();
static void kill_all_things();
static void load_shooter_1(int static_slot);
static void load_star(int static_slot);
static void load_sphere(int static_slot, int rot_z);
static void load_galaxy(int static_slot);
static void load_ring(int static_slot);
void reinit_level5();

void init_level5()
{


//find warphole and set up sound for it
 warphole_core_slot=find_dynamic_object_slot('WH1Z');
 warphole_trunk=find_dynamic_object_slot('WH2&');
 if (warphole_core_slot==-1) report_error_c("Level4: Couldn't find warphole.","",-3);
//set up the sound
 insert_continuous_sound(warphole_core_slot, rumble_loop,65);
  mothership_core_slot=find_dynamic_object_slot('MS02');

  if (mothership_core_slot==-1) report_error_c("Level4: Couldn't find mothership.","",-3);

   global_open_doors=1;
   entered_hole=0;
   obc_warning_timer=0;
   gen_dust_timer=0;
   over_hole=0;
   vib=30;
   roty=0;
   shooter1_slot=get_static_object_slot('SHT1');	//go fetch fido (the object)
   shooter2_slot=get_static_object_slot('SHT2');	//go fetch fido (the object)
   star_slot=get_static_object_slot('STAR');	//go fetch fido (the object)
   mon2_slot=get_static_object_slot('MON2');	//go fetch fido (the object)
   ball1_slot=get_static_object_slot('BLL1');	//go fetch fido (the object)
   gala_slot=get_static_object_slot('GALA');	//go fetch fido (the object)
   ring_slot=get_static_object_slot('RING');	//go fetch fido (the object)
//   bar_slot=find_dynamic_object_slot('BAR1');	//go fetch fido (the object)

   if (shooter1_slot==-1) report_error_c ("Level4: Couldn't find sht1 ","",-3);
    if (shooter2_slot==-1) report_error_c ("Level4: Couldn't find sht2 ","",-3);
    if (star_slot==-1) report_error_c ("Level4: Couldn't find STAR ","",-3);
    if (mon2_slot==-1) report_error_c ("Level4: Couldn't find MON2 ","",-3);
    if (ball1_slot==-1) report_error_c ("Level4: Couldn't find BLL1 ","",-3);
    if (gala_slot==-1) report_error_c ("Level4: Couldn't find GALA ","",-3);
    if (ring_slot==-1) report_error_c ("Level4: Couldn't find RING ","",-3);
//    if (bar_slot==-1) report_error_c ("Level4: Couldn't find BAR ","",-3);
   timer_5=0;
   cy_delta=10;
   frame_count=0;
   cam_rot_d=1;
   star_rot=0;
   bg_colour=255;
   timer_6=0;
   sphere_rot=0;
   rumble_volume=100;
   midi_note=42;
   inc_midi=7;
   end_scene=0;
in_ramparian_space=0;
bar_alive=0;
}


void Level5()
{
extern int level_complete; 	//game var - 0,1 or 2
extern float dest_x,dest_y;	//from level file
extern	DynObjectsFixedSize *ocb_ptr;
extern int camera_rotation_z;
extern float depth_of_field;
int ship_x, ship_y;	//from map_control

int hole_dist;
  int obj_x,obj_y;
  int camy,camx,delta_x,delta_y;
load_dyn_object_pb the_params;


 ship_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
 ship_y=static_cast<int>((*ocb_ptr).object_list[0].Oworldy);


//see if we can find any excaliburs
 if (bar_alive==1 && find_dynamic_object_slot('EXC1')==-1)
 {
 int wm;
 
 add_to_kill_list(bar_slot);
 add_to_explode_list(bar_slot,5);
 bar_alive=0;
      wingmen_guarding_what_object=mothership_core_slot;
//      wm=find_a_wingman();
//          if (wm!=-1) 
//           {
//            do_callsign_text("     GOOD LUCK BOSS, I'M OFF. BYE!",wm,1);
//           }
 }
 
 
////control the bar
//if (bar_alive==1)
//{
//   (*ocb_ptr).object_list[bar_slot].Oworldy=ship_y;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+1].Oworldy=ship_y+5400;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+2].Oworldy=ship_y-5400;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+3].Oworldy=ship_y+5400*2;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+4].Oworldy=ship_y-5400*2;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+5].Oworldy=ship_y+5400*3;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+6].Oworldy=ship_y-5400*3;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+7].Oworldy=ship_y+5400*4;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+8].Oworldy=ship_y-5400*4;	  //stop zex getting past
//}
//else	  //oh bugger! That's the laziest thing I've ever done!
//{
//   (*ocb_ptr).object_list[bar_slot+1].Oworldy=ship_y+5400;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+2].Oworldy=ship_y-5400;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+3].Oworldy=ship_y+5400*2;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+4].Oworldy=ship_y-5400*2;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+5].Oworldy=ship_y+5400*3;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+6].Oworldy=ship_y-5400*3;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+7].Oworldy=ship_y+5400*4;	  //stop zex getting past
//   (*ocb_ptr).object_list[bar_slot+8].Oworldy=ship_y-5400*4;	  //stop zex getting past
//}
//




//gen shooting things and rotate camera
if (end_scene==1)
{
int slot,it;
//load up required objects which are warphole, zex and alien mothership
 clear_all_cont_sound_chans();
 play_zsound_always(explosion4, sound_high_pri, stereo1_chan, sound_vol_7);
 play_zsound_always(missile_explode, sound_high_pri, c1_chan1, sound_vol_7);
 play_zsound_always(screech, sound_high_pri, c1_chan2, sound_vol_7);
//kill everything
 kill_all_things();
//load Zex for game
 slot=get_static_object_slot('ZEXS');	//go fetch fido (the object)
 if (slot==-1) report_error_c ("Level4: Couldn't find ZEXS???","",-3);
 replace_dyn_object(slot,0);	//the object, the position (-1=next free)
//load in warphole mouth
 slot=get_static_object_slot('WH1Z');	//go fetch fido (the object)
 if (slot==-1) report_error_c ("Level4: Couldn't find WH1Z","",-3);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);

	the_params.world_x=0;
	
	the_params.world_y=0;
	the_params.world_z=35000;	//RangedRdm(2000,6000);
	the_params.delta_rot_z=-1;

	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
         the_params.who_spawned_this_object=-1;
	load_dyn_object(slot,it,&the_params,-1);	//the object, the position (-1=next free)
 }


//load in warphole core
 slot=get_static_object_slot('WH2&');	//go fetch fido (the object)
 if (slot==-1) report_error_c ("Level4: Couldn't find WH2&","",-3);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
	the_params.world_x=0;
	
	the_params.world_y=0;
	the_params.world_z=45000;	//RangedRdm(2000,6000);
	the_params.delta_rot_z=-1;
	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
         the_params.who_spawned_this_object=-1;
	load_dyn_object(slot,it,&the_params,-1);	//the object, the position (-1=next free)
 }

//set camera
camera_pos.x=camera_pos.y=0;
camera_pos.z=0;

//set zex vel in z
(*ocb_ptr).object_list[0].Omovedz=-400;
(*ocb_ptr).object_list[0].Oworldz=35000;
(*ocb_ptr).object_list[0].no_trivial_rejection=1;

//level_has_camera_control=0;	//this'll cause much trashing in the camera controller no doubt...

 in_ramparian_space=1;	//global for engine (dust etc.)
 end_scene=2;
 return;
}




if (end_scene==2)
{
int slot,it;
//flip zex if gone past
if ((*ocb_ptr).object_list[0].Oworldz<400)
 {
 kill_all_things();
(*ocb_ptr).object_list[0].Omovedz=400;
(*ocb_ptr).object_list[0].Oroty=90;
(*ocb_ptr).object_list[0].is_visible=1;

(*ocb_ptr).object_list[0].Orotz=0;
(*ocb_ptr).object_list[0].Orotx=0;
camera_rotation_z=camera_rotation_y=camera_rotation_x=0;
//load in excalibur <G>
//load in warphole core
 slot=get_static_object_slot('EXC1');	//go fetch fido (the object)
 if (slot==-1) report_error_c ("Level4, ENDSCENE=2: Couldn't find EXC1","",-3);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
        clear_pb(&the_params);
//load in object
	the_params.world_x=30000;
	
	the_params.world_y=30000;
	the_params.world_z=0;	//RangedRdm(2000,6000);
	the_params.object_category=ALIEN;
        the_params.controller_ref=excalabur;
	 the_params.no_trivial_rejection=1;	//controlled when not in view
         the_params.who_spawned_this_object=-1;
	load_dyn_object(slot,it,&the_params,-1);	//the object, the position (-1=next free)
 }

end_scene=3;
 }
 

return;
}

if (end_scene==3)
 {
 load_shooter_1(star_slot);
  if ((*ocb_ptr).object_list[0].Omovedz>0) (*ocb_ptr).object_list[0].Omovedz-=4;
  if ((*ocb_ptr).object_list[0].Oroty!=0) (*ocb_ptr).object_list[0].Oroty--;
  if ((*ocb_ptr).object_list[0].Omovedz<=0)
      {
      end_scene=4;
      (*ocb_ptr).object_list[0].Omovedz=0;
      (*ocb_ptr).object_list[0].Oworldx=0;
      (*ocb_ptr).object_list[0].Oworldy=0;
      (*ocb_ptr).object_list[0].Oworldz=-100;
      level_has_camera_control=0;	//this'll cause much trashing in the camera controller no doubt...
      level_has_control=0;

      }
return;
 }

if (end_scene==4)
 {
  add_to_text_display("OBC> Please wait. rebooting...",0);
  play_zsound_always(mac_startup, sound_high_pri, c1_chan1, sound_vol_7);
  
  can_gen_dust=1;
  thrust=0;
  timer_5=0;
  end_scene=5;
  return;
 }  
 
if (end_scene==5)
 {
 int slot,it;
  timer_5++;
  if (timer_5>190)
  { 
  add_to_text_display("OBC> Regained ship control!",0);
  add_to_text_display("OBC> Found alien mothership. looks like we made it!",0);
  add_to_text_display("OBC> Dock with mothership to end mission.",0);
//load alien mothership
 slot=get_static_object_slot('MS10');	//go fetch fido (the object)
 if (slot==-1) report_error_c ("Level4: Couldn't find MS10","",-3);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=300000;
	
	the_params.world_y=-200000;
	the_params.world_z=0;	//RangedRdm(2000,6000);
	the_params.delta_rot_y=2;
	
	the_params.controller_ref=earth_mothership;

	the_params.shield_value=11170;
	the_params.mass=90;
	the_params.object_category=baddie_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
         the_params.who_spawned_this_object=-1;
	load_dyn_object(slot,it,&the_params,-1);	//the object, the position (-1=next free)
 }


 slot=get_static_object_slot('MS11');	//go fetch fido (the object)
 if (slot==-1) report_error_c ("Level4: Couldn't find MS11","",-3);

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=300000;
	
	the_params.world_y=-200000;
	the_params.world_z=0;	//RangedRdm(2000,6000);
	the_params.delta_rot_y=-2;
	
	the_params.controller_ref=earth_mothership;

	the_params.shield_value=11170;
	the_params.mass=90;
	the_params.object_category=baddie_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
         the_params.who_spawned_this_object=-1;
	load_dyn_object(slot,it,&the_params,-1);	//the object, the position (-1=next free)

load_motherships_into_nav();

 }
level_complete=2;
  dest_x=300000;
  dest_y=-200000;
       insert_batcom_message("MISSION COMPLETE","                ");
   can_do_next_level=1;
  game_running=1;
  end_scene=6;
  in_wormhole=0;	//set main render loop back to control panel
  reinit_screen();	//get cp back up
  }
 return;
 }
    
//that's it folks! All done. Lashings of ginger ale all round!

if (end_scene==6)
{
return;
}


if (entered_hole==5)
{
frame_count++;

timer_6++;

timer_5++;

if (timer_5>2)
  {
  timer_5=0;
camera_rotation_z+=cam_rot_d;
  }

  if (frame_count<400)
  load_shooter_1(shooter1_slot);
  
  if (frame_count>300 && frame_count<600)
  {
    if (rumble_volume>0)
    {
    rumble_volume-=1;
    set_cont_sound_chan_volume(warphole_core_slot,rumble_volume);	//fade out the rumble
    if (rumble_volume==1) clear_all_cont_sound_chans();	//so we know what state we're in
    }

  cam_rot_d=-cam_rot_d;
  load_shooter_1(shooter2_slot);
  }
  
//stars and lines
  if (frame_count>600 && frame_count<1200)
  {
     if (timer_6>4)	//flash screen
     { 
        timer_6=0;
        if (RangedRdm(0,100)>50)
        set_erase_colour(255);
        else
         set_erase_colour(246);
  
//  bg_colour--;
//  if (bg_colour<244) bg_colour=255;
     }

  load_star(star_slot);
//  load_star(star_slot);

 load_shooter_1(shooter1_slot);
// if (RangedRdm(0,100)>95) load_shooter_1(shooter1_slot);
  }
  
if (frame_count>1199 && frame_count<1212)
     {
//       load_circle(galaxy_slot);
   set_erase_colour(255);
  load_sphere(ball1_slot,sphere_rot);
  sphere_rot+=33;
     }
if (frame_count==1212) load_galaxy(gala_slot);

if (frame_count==1220) check_balls();	//check if registered - will end the game if not

if (frame_count>1280)
{
 int slot;
  slot=find_dynamic_object_slot('BLL1');
   if (slot!=-1)
    {
    (*ocb_ptr).object_list[slot].Omovedz=-1200;
    }
if (RangedRdm(0,100)>94) load_ring(ring_slot);

}

if (frame_count==1330) play_simple_continuous_sound(tractor_beam_loop, 40, 1);
if (frame_count>1332) 
 {
 inc_midi--;
 if (inc_midi<0)
  {
  inc_midi=17;
  set_cont_sound2_note(midi_note);
  midi_note++;
  if (midi_note>68) end_scene=1;	//get out
  }
 }
//camera_pos.y=0; camera_pos.x=0;

if (frame_count<1000) camera_pos.y+=cy_delta;
else
{
 if (camera_pos.y>0) camera_pos.y--;
 if (camera_pos.y<0) camera_pos.y++;
 
}
if (camera_pos.y>1000) cy_delta=-cy_delta; 
if (camera_pos.y<-1000) cy_delta=-cy_delta; 

//if (cy_delta<60) cy_delta++;

if (camera_rotation_z>359) camera_rotation_z-=360;
if (camera_rotation_z<0) camera_rotation_z+=360;

return;
}

//zoom zex off int odistance and kill it
if (entered_hole==4)
{
        (*ocb_ptr).object_list[0].Oworldz+=220;
 if ((*ocb_ptr).object_list[0].Oworldz>6000)
       {
       entered_hole=5;
       kill(0);
       
       }

return;
}


if (entered_hole==3)
{
int static_slot;

//we are now through the mouth and tunnel and in blackness
//lets get everything back to normal
depth_of_field=256;	//back to normal on the camera lens
(*ocb_ptr).object_list[0].Oworldz=-100;
(*ocb_ptr).object_list[0].Oworldx=0;
(*ocb_ptr).object_list[0].Oworldy=0;

camera_pos.z=-1500;	//move cam back
camera_pos.x=0;
camera_pos.y=0;

        (*ocb_ptr).object_list[0].Oworldz+=220;

//    static_slot=get_static_object_slot('ZEX1');	//go fetch fido (the object)
//    if (static_slot==-1) report_error ("Level4: Couldn't find ZEX1.","",-3);
//        
//    replace_dyn_object(static_slot,0);	//the object, the position (-1=next free)

entered_hole=4;
return;
}

if (entered_hole==1)
{
stop_music_now();
//clear_all_cont_sound_chans();
play_zsound_always(space_synth, sound_high_pri, c1_chan1, sound_vol_7);
entered_hole=2;
}

if (entered_hole==2)
{
  int mouth_core_slot;

//(*ocb_ptr).object_list[0].Orotdz=-15;
(*ocb_ptr).object_list[warphole_trunk].Orotdz=7;

//make the mouth invisible so we don't get funny old artifacts
//  mouth_core_slot=find_dynamic_object_slot('WH1&');
//  if (mouth_core_slot!=-1) 
//  {
//  (*ocb_ptr).object_list[mouth_core_slot].eng_invisible=1;
//  }
(*ocb_ptr).object_list[0].Oworldz+=100;

  camy=static_cast<int>(-camera_pos.y);
  camx=static_cast<int>(-camera_pos.x);

  if ( camy < ship_y) camera_pos.y-=vib;
  else camera_pos.y+=vib;
  
  
  if ( camx < ship_x) camera_pos.x-=vib;
  else camera_pos.x+=vib;

  if (vib>0) vib-=3;
  
        (*ocb_ptr).object_list[0].Oworldz+=220;
        if (camera_pos.z<-100) 
        {
        camera_pos.z+=20;
        }
        else
        {
         if (depth_of_field<3000) depth_of_field+=100;
         else	//we are now completely black
         {
          entered_hole=3;
          //kill the warp hole entrance
          //we have to move the sound from a 3d one to a cont one
          
          kill_all_things();
          camera_pos.y-=30;
//          add_to_kill_list(warphole_core_slot);
//          add_to_kill_list(warphole_trunk);
          
         }
        } 
}

else	//haven't entered yet
{
 //first, if zex is close enough to the target we start generating stars affected by the wh
 hole_dist = get_distance_to_from(warphole_core_slot,0);
 
 if (hole_dist<10000 && over_hole<2)
 over_hole=2;
 else
 if (hole_dist<30000 && over_hole<1)
 over_hole=1;
 if (hole_dist>30000)  over_hole=0; 

 if (over_hole==1)
   {
   //OBC warnings
   if (ZexTickCount()>obc_warning_timer)
     {
     //audible warning
      play_zsound(danger, sound_high_pri, speech_chan, sound_vol_7);	   //do computer danger voice
      obc_warning_timer=ZexTickCount()+60*(hole_dist/10000+3);
     }
   
   gen_dust_timer++;
   if (gen_dust_timer>2)
       {
       //gen a piece of dust that flies into hole
       gen_dust_timer=0;
       gen_dust_for_warphole();
       }
   } 
   
 if (over_hole==2)	//err we go..
  {
   int static_slot;
    can_gen_dust=0;
    add_to_text_display("OBC> LOST SHIP CONTROL!",0);
//    in_wormhole=1;	  //set main render loop to not show control panel
//    reinit_screen();
    fade_out_music();
    over_hole=3;
    level_has_control=1;
    level_has_camera_control=1;
   //swap zex for zexc
    static_slot=get_static_object_slot('ZEX1');	//go fetch fido (the object)
    if (static_slot==-1) report_error_c ("Level4: Couldn't find ZEXC.","",-3);
        
    replace_dyn_object(static_slot,0);	//the object, the position (-1=next free)

  }
  
if (over_hole==3)	//move ship over hole whilst zooming camera in
  {
  int centered;
  LSRect hole_box;


   gen_dust_timer++;
   if (gen_dust_timer>2)
       {
       //gen a piece of dust that flies into hole
       gen_dust_timer=0;
       gen_dust_for_warphole();
       }
  if ((*ocb_ptr).object_list[0].Omovedx>0) (*ocb_ptr).object_list[0].Omovedx--;
  if ((*ocb_ptr).object_list[0].Omovedx<0) (*ocb_ptr).object_list[0].Omovedx++;
  if ((*ocb_ptr).object_list[0].Omovedy>0) (*ocb_ptr).object_list[0].Omovedy--;
  if ((*ocb_ptr).object_list[0].Omovedy<0) (*ocb_ptr).object_list[0].Omovedy++;
  
  obj_x=static_cast<int>((*ocb_ptr).object_list[warphole_core_slot].Oworldx);
  obj_y=static_cast<int>((*ocb_ptr).object_list[warphole_core_slot].Oworldy);

  //see if we're centered
  hole_box.top=obj_y+200;
  hole_box.left=obj_x-200;
  hole_box.bottom=obj_y-200;
  hole_box.right=obj_x+200;
  
  if (point_in_rect(ship_x,ship_y,hole_box)==1) 
  {
  centered=1;
  }
  else centered=0;


  delta_x=delta_y=0;
  
//  if (ship_x<obj_x) 
    delta_x=(obj_x-ship_x);
//  if (ship_x>obj_x) delta_x=(ship_x-obj_x);

//  if (ship_y<obj_y) 
    delta_y=(obj_y-ship_y);
//  if (ship_y>obj_y) delta_y=-(ship_y-obj_y);
  
  delta_x/=16;
  delta_y/=16;
  
  (*ocb_ptr).object_list[0].Omovedx=delta_x;
  (*ocb_ptr).object_list[0].Omovedy=delta_y;
  camy=static_cast<int>(-camera_pos.y);
  camx=static_cast<int>(-camera_pos.x);
  
  if (camera_pos.z<-800) camera_pos.z+=60;
  
  if ( camy < ship_y) camera_pos.y-=40;
  else camera_pos.y+=40;
  
  
  if ( camx < ship_x) camera_pos.x-=40;
  else camera_pos.x+=40;
  

  //if centered then rotate Zex to 90 degrees
  if (centered==1)
    {
    int ship_ang,ship_x_ang=0;




    ship_ang=(*ocb_ptr).object_list[0].Orotz;
    
    if (ship_ang!=90) 
     {
     ship_ang++;
     if (ship_ang<0) ship_ang+=360;
     if (ship_ang>359) ship_ang-=360;
     (*ocb_ptr).object_list[0].Orotz=ship_ang;
     }
     
    if (ship_ang==90)
     {
    ship_x_ang=(*ocb_ptr).object_list[0].Oroty;
    
    if (ship_x_ang!=270) 
     {
     ship_x_ang--;
     if (ship_x_ang<0) ship_x_ang+=360;
     if (ship_x_ang>359) ship_x_ang-=360;
     (*ocb_ptr).object_list[0].Oroty=ship_x_ang;
     }
     
     }
     
    //we are now facing down the hole
    if (ship_x_ang==270 && ship_ang==90)
        {

        (*ocb_ptr).object_list[0].Oworldz+=70;
        if (camera_pos.z<-80) 
        {
        camera_pos.z+=40;
        }
        else 
        {
        int static_slot;
        
        add_to_text_display("OBC> SUFFERING A MAJOR ANXIETY ATTACK!",0);

        over_hole=-1;	//ok, we're in
	entered_hole=1;
   //swap zex1 for zex2 - brighter engines
    static_slot=get_static_object_slot('ZEX2');	//go fetch fido (the object)
    if (static_slot==-1) report_error_c ("Level4: Couldn't find ZEXC.","",-3);
        
    replace_dyn_object(static_slot,0);	//the object, the position (-1=next free)

//	kill_all_things();	//kill all but zex and wormhole
        }
        }
     
    }
  
  }
  
}

}

void kill_all_things()
{
int i;

for (i=1;i<MAX_NUMBER_OF_DYN_OBJECTS-1;i++)
   {
    if ((*ocb_ptr).object_list[i].in_use==1) kill(i);
   }
}

void load_shooter_1(int static_slot)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status!=0) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
        if (RangedRdm(0,100)>50)
	the_params.world_x=-300;
	else
	the_params.world_x=300;
	
	the_params.world_y=RangedRdm(1,4000)-2000;
	the_params.world_z=5000;	//RangedRdm(2000,6000);
	the_params.Omovedz=-RangedRdm(95,290);
	
	the_params.rot_y=90;
	
	the_params.controller_ref=STAR_FOR_WARPHOLE;

	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}

void load_sphere(int static_slot, int rot_z)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status==1) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=300;
	
	the_params.world_y=0;
	the_params.world_z=38000;	//RangedRdm(2000,6000);
	the_params.delta_rot_z=3;
	the_params.rot_z=rot_z;
	
	the_params.controller_ref=STAR_FOR_WARPHOLE;
	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}

void load_galaxy(int static_slot)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status==1) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=0;
	
	the_params.world_y=0;
	the_params.world_z=228000;	//RangedRdm(2000,6000);
	the_params.delta_rot_z=1;
	
	the_params.controller_ref=STAR_FOR_WARPHOLE;
	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}

void load_ring(int static_slot)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status==1) return;
 
it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=0;
	
	the_params.world_y=0;
	the_params.world_z=88000;	//RangedRdm(2000,6000);
	the_params.Omovedz=-500;
	
	the_params.delta_rot_z=1;
	
	the_params.controller_ref=STAR_FOR_WARPHOLE;
	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}


void load_star(int static_slot)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars,global_show_status;
extern _3D camera_pos;
int camera_z;

load_dyn_object_pb the_params;

int it;
float dust_random_x,dust_random_y;
//float ship_dir_x,ship_dir_y;
//oif (global_show_status==1) return;
star_rot+=5;
if (star_rot>359) star_rot=0;

it=find_vacant_dynamic_object_slot();
if (it!=-1)
 {   
//load in object
        clear_pb(&the_params);
	the_params.world_x=COS(star_rot)*500;
	
	the_params.world_y=SIN(star_rot)*500;
	the_params.world_z=7000;	//RangedRdm(2000,6000);
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=-RangedRdm(25,110);
	
	the_params.delta_rot_y=5;
	the_params.rot_y=90;
	
	the_params.controller_ref=STAR_FOR_WARPHOLE;
	the_params.object_category=scenary_object;
		
	 the_params.no_trivial_rejection=1;	//controlled when not in view
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,it,&the_params,-1);	//the object, the position (-1=next free)


 }
}



//no quicksave or restore on level 5
void quicksave_level5()
{
}

void restore_quicksave_level5()
{
}


void reinit_level5()
{
 insert_continuous_sound(warphole_core_slot, rumble_loop,65);

}



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

//mothership controller
/*
 * $Log: motherships.cpp,v $
 * Revision 1.36  2006/09/09 09:53:51  stu
 * Zex2005: Code: Mothership lasers now take into account LASER_RANGE_MULTIPLIER)
 *
 * Revision 1.35  2006/07/09 23:59:33  stu
 * Zex2005: Code: Fixed a problem with ships items not being properly maintained and setup when flatpack is used.
 *
 * Fixed problem whereby even though you had no flatpacks you could sell one.
 *
 * Fixed a problem whereby you would be fined 50 credits twice for attacking a mothership
 *
 * Fixed a problem whereby if showing GM or status when docked then interior of MS would be blank.
 *
 * Revision 1.34  2006/07/09 03:05:09  stu
 * Zex2005: Code: Ship items available for sale are now affected by the mships supply rating and tech level. So on a mship on a poor supply route supplied by an agri planet there won't be much but on a mship on close to a tech planet the range will be much wider (honouring what must and must not be avail).
 *
 * Revision 1.33  2006/07/06 20:06:59  stu
 * Zex2005: Code: Change heading of ships launching from 90 to 70 degrees so they don't hang about the exit if they've nothing to do.
 *
 * Revision 1.32  2006/07/03 23:56:52  stu
 * Zex2005: Code: not possible to destroy Mothership shield object any more.
 *
 * Revision 1.31  2006/05/28 01:31:51  stu
 * Zex2005: Code: Fix a problem whereby you can't connect to galanet on the first few attempts after zex has been booted.
 *
 * Revision 1.30  2006/03/04 18:00:25  stu
 * Zex2005: Code: Fix for motherships not attacking immediately (until they have launched something)
 *
 * Revision 1.29  2006/03/02 22:16:39  stu
 * Zex2005: Code: Checked hit_by_slot, docking_what and docked_object_slot. Also checked freeze in ship_control for various problems when GM is displayed on Space Cadet level.
 *
 * Revision 1.28  2006/02/27 01:42:56  stu
 * Zex2005: Code: A few changes to Motherships. 1. Can't connect to GalaNet if power rating is too low - typical after forcing a mship to surrender. Wait and try again. 2. You now get score and credits for forcing a surrender.
 * 3. Motherships surrender later.
 *
 * Revision 1.27  2006/02/21 21:15:24  stu
 * Zex: Code: Fix for #116
 *
 * Revision 1.26  2006/02/05 19:30:16  rob
 * Zex2005: Debug stuff, currently disabled.
 *
 * Revision 1.25  2006/02/05 13:20:05  rob
 * Zex2005: PC build crashing because object_aimed_at_slot is out of range.
 *
 * Revision 1.24  2006/01/14 00:00:46  stu
 * Zex2005: Code: Some important bug fixes. 1. Tasers didn't work - they fired like lasers (single beam). 2. Finally found the crash in motherships where my_mem would be zero - only the controller parts of a mothership has the aux mem - if something hit another part of a mothership then we'd crash (found whilst fixing the tasers). Also fixes a few gameplay bugs.
 *
 * Revision 1.23  2006/01/08 21:53:24  stu
 * Zex2005: Code: Change so if a mothership is docking someone when it surrenders the docking cancelled.
 *
 * Revision 1.22  2006/01/08 00:40:40  rob
 * Zex2005: Only fastdock zex, because otherwise another ship docking can cause Zex to end up in the wrong place.
 *
 * Revision 1.21  2006/01/05 01:50:24  stu
 * Zex2005: Code: Just make sure that a human motehrship can't hate us if we're a little too enthusiastic with a big laser when making an lien mothership surrender
 *
 * Revision 1.20  2006/01/04 23:11:37  stu
 * Zex2005: A variety of mothership bugs fixed and some other stuff - object movement now frozen when displaying GM if Space Cadet, trinary mode GM (off as booked in), ZEX_LIVES_FOREVER now gives you everything and rank 3.
 *
 * Revision 1.19  2006/01/01 17:49:19  rob
 * Zex2005: Added back in screen fading, especially for fast dock.
 *
 * Revision 1.18  2005/12/28 23:47:59  stu
 * Zex2005:Code: Slight boo-boo on the last check-in of this file.
 *
 * Revision 1.16  2005/12/20 20:45:18  rob
 * Zex2005: Various signed/unsigned comparison things plus one might be used uninitialised (not really - but got rid of warning).
 *
 * Revision 1.15  2005/12/20 20:32:47  stu
 * Zex2005:Code: All object search loops now check they haven't run off the end of the dynamic OCB's
 *
 * Revision 1.14  2005/12/18 23:28:09  stu
 * Zex2005: Code: FIx for game motherships crashing when docking - would set docking_what to -1 and then go and use it in the ocbs. Fixes 62, 68 probably.
 *
 * Revision 1.13  2005/12/13 23:55:37  rob
 * Zex2005: Got rid of the last few int->float conversion warnings.
 *
 * Revision 1.12  2005/12/13 00:08:08  stu
 * Zex2005: Code: Updates for final music and some changes so for final ejection music (so the music doesn't change when docking the players ejection pod).
 *
 * Revision 1.11  2005/11/29 21:12:53  stu
 * Zex2005: Code: Tutorial: Muzzle tuner no longer fitted until time. No ejection pod fitted in tutorial. General: Fixes a problem where a mothership could time-out an ejection pod (this may fix other reported bugs).
 *
 * Revision 1.9  2005/11/27 00:29:43  stu
 * Zex2005: Code: Fixed a variety of bugs on training missions; crash when docked, radar now low range on tutorial, No longer show hi-score if player achieves a high score during tutorial; Journey - pod can't dock - mothership now stops if player ejects; Trade on tutorial borken; graphics during train missions whilst docked borked.
 *
 * Revision 1.8  2005/11/08 22:46:03  stu
 * Zex2005: Code: Various bug fixes as per the TODO
 *
 * Revision 1.7  2005/10/12 18:33:55  stu
 * Zex2005: Code: Changed docking music as old didn't work
 *
 * Revision 1.6  2005/10/10 21:29:08  rob
 * Zex2005: Added newline at end to stop compiler whinging.
 *
 * Revision 1.5  2005/10/08 13:13:37  rob
 * Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.
 *
 * Revision 1.4  2005/05/17 13:51:18  rob
 * Zex2005: More file changes.
 *
 * Revision 1.3  2005/05/02 10:30:40  rob
 * Zex2005: More Updates (code)
 *
 * Revision 1.2  2005/04/23 20:12:09  rob
 * ZEX: First set of changes for SDL build. (remainder of files)
 *
 * Revision 1.27  1999/09/26 18:34:45  stu
 * 260999
 *
 * Revision 1.26  1999/09/20 19:03:33  stu
 * 0.93.3
 *
 * Revision 1.25  1999/09/19 05:38:58  stu
 * 0.93 as posted
 *
 * Revision 1.24  1999/09/14 15:06:26  stu
 * 140999
 *
 * Revision 1.23  1999/09/12 00:56:19  stu
 * 120999
 *
 * Revision 1.22  1999/08/30 22:23:04  stu
 * 300899
 *
 * Revision 1.21  1999/08/20 09:57:35  stu
 * 20 Aug 99
 *
 * Revision 1.20  1999/08/15 22:17:02  stu
 * Cleanup 150899
 *
 * Revision 1.19  1999/08/03 18:25:55  stu
 * *** empty log message ***
 *
 * Revision 1.18  1999/07/29 05:09:39  stu
 * Version 0.87 changes - minot bug fixes etc.
 *
 * Revision 1.17  1999/07/26 21:35:55  stu
 * Pre test 2 27 July 99
 *
 * Revision 1.16  1999/07/26 19:08:46  stu
 * Pre 0.86 test 27 July 99
 *
 * Revision 1.15  1999/07/13 04:08:02  stu
 * Clean up 130799
 *
 * Revision 1.14  1999/07/09 06:03:08  stu
 * Clean up 09-7-99
 *
 * Revision 1.13  1999/07/04 00:21:34  stu
 * Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86
 *
 * Revision 1.12  1999/06/23 06:08:17  stu
 * More fixes for docking.
 *
 * Revision 1.11  1999/06/23 04:54:13  stu
 * More fixes related to motherships and weapons - still ongoing. Still docking sometimes dodgy.
 *
 * Revision 1.10  1999/06/22 15:31:53  stu
 * Level 2 work
 *
 * Revision 1.9  1999/06/22 10:57:34  stu
 * Lurkers clean up
 *
 * Revision 1.8  1999/06/14 07:25:42  stu
 * *** empty log message ***
 *
 * Revision 1.7  1999/05/25 04:49:35  stu
 * *** empty log message ***
 *
 * Revision 1.6  1999/05/21 03:31:25  stu
 * 1. New screen driver.
2. VBL Driven music fade and play
 *
 * Revision 1.5  1999/05/17 04:34:31  stu
 * Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c
 *
 * Revision 1.4  1999/05/16 19:15:00  stu
 * 1.3 fixes motherships not docking Zex

Added $Log to top of file
 *
 * Revision 1.2  1999/05/12 15:20:13  stu
 * Added $Log to top of file
 *
*/
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
  
//oldmac//#include <fp.h>
//oldmac//#include <QuickDraw.h>
#include	"3d_structures_def.h"
#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include	"zex_misc.h"
#include "explode.h"
#include "sound.h"
#include "fire_weapon.h"
#include "monster1.h"
#include "compile_zmap.h"
#include "Damage.h"
#include "3d_top.h"
#include "dlp.h"
#include "aim.h"
#include "object_misc.h"
#include "ship_control.h"
#include "check_bounding_boxes.h"
#include "score_and_credits.h"
#include "instrum_int.h"
#include "dlp_explosions.h"
#include "boid.h"
#include "misc_controllers.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include "wingman.h"
#include "obj_control_misc_routines.h"
#include "motherships.h"
#include "handle_level.h"
#include	"Error_handler.h"
#include "map_control.h"
#include "game_defs.h"
//������������������������������������������������������
int dock_timer;	//watchdog timer
int dock_fee;
int mothership_cat, docked_at, docked_at_shields;	//from mothership_category, slot of mothership we are docked at
float docked_at_power_rating=30;	//make sure comms won't fail when we first dock when zex first booted
int fastdock_in_progress,fastdock_fade_value;
int global_request_dock;	//item number of object requesting dock
void tell_engine_we_have_docked(void);
extern kit_desc ship_items[ITEMS_COUNT];
//oldmac//extern DSpContextReference		mDisplayContext;
extern int wingmen_guarding_what_object;

extern int tractor,zex_alive;

void handle_earth_mothership(int it)	//tractor and laser - stationary position
{
extern	DynObjectsFixedSize *ocb_ptr;

float shot_x,shot_y,shot_z;
int slot,object_aimed_at_slot;
load_dyn_object_pb the_params;
_2D start_coord,end_coord;
extern int thrust,level_complete;
extern int launch_timer;
extern float dest_x,dest_y;
extern int docked_at,docked_at_shields;
extern int have_ejected;
extern int score,current_level,showing_stats;

int ship_z_ang,angle_to_zex;
int tractor_scan_object,tractor_type;
LSRect pick_up_box;

float dx,dy,dock_offset;

register ZObject * object_ptr;
extern int credits;

//cu1=laser fire interval
//cu2=free
//cu3=phase of tractor
//cu4=tractoring someone
//cu7=object aimed at from collision (or -1)
//cu8=object aimed at from collision active if > 0
//cu9=free
//cu10=free
//

//if paused then return!!!!
if (showing_stats==1) return;

    object_ptr=&ocb_ptr->object_list[it];

//Find something to kill with the laser!!!
//alien motherships dont attack their own kind...
 object_ptr->control_use_1--;
 if (object_ptr->control_use_1<0 && object_ptr->is_armed==1)
 // && (*ocb_ptr).object_list[it].object_category==zex_object) 
 {
 object_ptr->control_use_1=12;
 //fire laser
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
        if (object_ptr->object_category==baddie_object)
	object_aimed_at_slot=aim_from_object_no_limit(it,zex_object,24000);	//pass the object, category to aim at and distance limit
        else
	object_aimed_at_slot=aim_from_object_no_limit(it,baddie_object,24000);	//pass the object, category to aim at and distance limit
        
        #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	
	if (object_aimed_at_slot==0) object_aimed_at_slot=-1;	//dont target zex!
	
	if (object_aimed_at_slot==-1)
	{ 
	//����this is a NOP in C -- might need to do something here...���� 
	}
	else
	{
	clear_pb(&the_params);
//FRIG
//we no laonger use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	
	 play_zsound_relative_to_zex(object_aimed_at_slot,laser1);

	 shot_x=(*ocb_ptr).object_list[it].Oworldx;
	 shot_x=shot_x+(400*(*ocb_ptr).object_list[it].object_normal_translated.x);
	 shot_y=(*ocb_ptr).object_list[it].Oworldy;
	 shot_y=shot_y+(400*(*ocb_ptr).object_list[it].object_normal_translated.y);
	 shot_z=(*ocb_ptr).object_list[it].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
	 the_params.controller_ref=ship_laser1;	//controller
	 the_params.control_use_1=10;	//frames to live counter
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
	 the_params.control_use_3=it;	//who fired in ctrl 3
	 the_params.control_use_4=(*ocb_ptr).object_list[it].laser_category;	//laser class


//	the_params.shield_value=0;
//     the_params.has_roll=0;
	 the_params.mass=0;	//should be 0
//		 	 the_params.dq=0;
	 the_params.object_category=zex_object;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=it;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)

//now set the poly to a zero shape
  
	}	//end of good slot
	}	//end of slot -1

}	//end of time to fire


////handle fire laser becuase something shot us!
////first see if we have an attack in progress
if (object_ptr->control_use_8>0 && object_ptr->is_armed==1)
{
object_ptr->control_use_8--;	 //timer for attack - stops at zero, target in cu7

 object_ptr->control_use_1--;
 if (object_ptr->control_use_1<0)
 // && (*ocb_ptr).object_list[it].object_category==zex_object) 
 {
 object_ptr->control_use_1=12;
 //fire laser
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	object_aimed_at_slot = object_ptr->control_use_10;
        #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	
	
	if (object_aimed_at_slot==-1)
	{ 
	//����this is a NOP in C -- might need to do something here...���� 
	}
	else
	{
	
//FRIG
//we no laonger use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	
	clear_pb(&the_params);
	 play_zsound_relative_to_zex(object_aimed_at_slot,laser1);

	 shot_x=object_ptr->Oworldx;
	 shot_x=shot_x+(400*object_ptr->object_normal_translated.x);
	 shot_y=object_ptr->Oworldy;
	 shot_y=shot_y+(400*object_ptr->object_normal_translated.y);
	 shot_z=object_ptr->Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
	 the_params.controller_ref=ship_laser1;	//controller
	 the_params.control_use_1=10;	//frames to live counter
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
	 the_params.control_use_3=it;	//who fired in ctrl 3
	 the_params.control_use_4=object_ptr->laser_category;	 //laser class

	 the_params.mass=0;	//should be 0 laser
//		 	 the_params.dq=0;
	 the_params.object_category=zex_object;
	 the_params.no_trivial_rejection=1;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=it;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)

//now set the poly to a zero shape
  
	}
    }	//end of good slot
 

  }	//end of control_use_1<0

}	//end of control_use_8>0

//first see if ship is "in-range" and start tractoring it if so,
//after that look for enemies chasing ship to laser with a top notch, hunky laser (as set in the level file)

if (zex_alive==0) return;	//prevent any embrarassing crashes...
if (launch_timer>0) return;

if (tractor==0)
	{
	object_ptr->control_use_4=0;
	object_ptr->control_use_3=0;
	}
//stop zex from running right through us!
//are we tractoring?
if (object_ptr->control_use_4==1)
{

	if(have_ejected==0)	//we don't toll ejection pods and we don't time them out
	{
		dock_timer++;
		if (dock_timer>1300)
		{
		cancel_dock(0);
		 return;
		}

		 dock_fee=(credits/100)*(3+current_level);
		 if (dock_fee<200) { dock_fee=200; }

		if (credits<dock_fee)
		{
			cancel_dock(0);
			add_to_text_display("OBC> Not enough credits to pay dock toll!",1);

			return;
			
		}
	}


 //do fade if fastdock in progress
 if (fastdock_in_progress==1)
    {
    RGBColor blue = {0,0,0xffff};
    
    if (fastdock_fade_value<-95)	//to red to zero and then to black below zero
        {
             tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
     tractor=0;
     mothership_cat=object_ptr->object_category;	//so we get the right docked bg
     docked_at=it;	//save slot of this mothership
     docked_at_shields=object_ptr->shield_value;
	 clear_cont_sound(it);	//stop tractor sound
        return;	//we're all docked up mama.
        }
    //fade by another 5 %
   //oldmac// DSpContext_FadeGamma ( mDisplayContext, fastdock_fade_value, &blue);
	LS_FadeGamma(fastdock_fade_value, &blue);
    fastdock_fade_value-=2;

    }
//first draw a tractor beam - we let the dlp do this
//    if (find_dynamic_object_slot('ZEXS')==-1)
//       (*ocb_ptr).object_list[it].control_use_4=0;	//zex is dead
    if(object_ptr->is_visible==0) return;	 //not visible so we cant calc coords
    if((*ocb_ptr).object_list[zex].is_visible==0) return;	//same
      
    start_coord=get_screen_center_coord(it);	//get screen coord is in 3d_top
    end_coord=get_screen_center_coord(zex);
    if (object_ptr->object_category==zex_object) 
    {
    	tractor_type=0;
//    dock_offset=2500;
    	dock_offset=0;
    }
    else
    {
    	tractor_type=1;	//alien tractor beam
    	dock_offset=0;
    }
    add_to_dlp(DRAW_TRACTOR,start_coord.x, start_coord.y,end_coord.x,end_coord.y,tractor_type,0);
    

//This switch statement controls itself to guide the ship in by altering control_use_3
///cases 1,2,3 are used when above mother ship, 5,6,7 below
    switch (object_ptr->control_use_3)
    {
//case 1 - ship somewhere above us - we want it at an angle of 45 to mothership
     case 1:
     {
     thrust=120;
     
//carry out tractor mission
//first we want zex above us
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[0].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[0].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     if (angle_to_zex<45 || angle_to_zex>135)	//to our right or left, so go up until 45 degrees is reached
      {
      if((*ocb_ptr).object_list[0].Orotz!=90) (*ocb_ptr).object_list[0].Orotz+=1;
      }
     else
      {
      object_ptr->control_use_3=2;
      }
     break;
    }
    
    case 2:
     {
     thrust=60;
//
//now go left or right until angle==90
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[0].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[0].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     if (angle_to_zex>92)	//to our left, so go right
     {
     if((*ocb_ptr).object_list[0].Orotz!=0) (*ocb_ptr).object_list[0].Orotz--;
     }
     else
     if (angle_to_zex<88)	//to our right, so go left
     {
     if((*ocb_ptr).object_list[0].Orotz!=180) (*ocb_ptr).object_list[0].Orotz++;
     }
     else	//somewhere between 93 and 87 which is almost directly aboe
     {
     object_ptr->control_use_3=3;
     }
     break;
     }


     case 3:	//now all we do is turn to 90
     {
     thrust=40;


//now bring zex in
//so, if zrot isn't 270 make it so
     ship_z_ang=(*ocb_ptr).object_list[0].Orotz;
     if(ship_z_ang!=270) ship_z_ang-=1;
     (*ocb_ptr).object_list[0].Orotz=ship_z_ang;
     if ((*ocb_ptr).object_list[0].Oworldx<object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[0].Oworldx+=30;
     }
     if ((*ocb_ptr).object_list[0].Oworldx>object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[0].Oworldx-=30;
     }
     //see if we've landed
  pick_up_box.top= static_cast<int>(object_ptr->Oworldy+1000);
  pick_up_box.bottom= static_cast<int>(object_ptr->Oworldy-1000);
  pick_up_box.left= static_cast<int>(object_ptr->Oworldx-3000);
  pick_up_box.right= static_cast<int>(object_ptr->Oworldx+3000);

  //check whether we are in the end zone
  if (point_in_rect( static_cast<int>((*ocb_ptr).object_list[0].Oworldx),
                   static_cast<int>((*ocb_ptr).object_list[0].Oworldy),
                  pick_up_box)) 
  {
     tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
     tractor=0;
     mothership_cat=object_ptr->object_category;	//so we get the right docked bg
     docked_at=it;	//save slot of this mothership
     docked_at_shields=(*ocb_ptr).object_list[it].shield_value;
     clear_cont_sound(it);	//stop tractor sound
  }
     break;
     }


//ship below
//case 1 - ship somewhere above us - we want it at an angle of 45 to mothership
     case 5:
     {
     thrust=120;
   
//carry out tractor mission
//first we want zex above and to our left
//so, if zrot isn't 150 make it so
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[0].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[0].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     if (angle_to_zex<215 || angle_to_zex>315)	//to our right or left, so go down until 45 degrees is reached
      {
      if((*ocb_ptr).object_list[0].Orotz!=270) (*ocb_ptr).object_list[0].Orotz+=1;
      }
     else
      {
      object_ptr->control_use_3=6;
      }
     break;
    }
    
    case 6:
     {
     thrust=60;
//
//now go left or right until angle==270
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[0].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[0].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     if (angle_to_zex>272)	//to our left, so go right
     {
     if((*ocb_ptr).object_list[0].Orotz!=180) (*ocb_ptr).object_list[0].Orotz--;
     }
     else
     if (angle_to_zex<268)	//to our right, so go left
     {
     if((*ocb_ptr).object_list[0].Orotz!=0) (*ocb_ptr).object_list[0].Orotz++;
     }
     else	//somewhere between 267 and 273 which is almost directly below
     {
     (*ocb_ptr).object_list[it].control_use_3=7;
     }
     break;
     }


     case 7:	//now all we do is turn to 90
     {
     thrust=40;


//now bring zex in
//so, if zrot isn't 90 make it so
     ship_z_ang=(*ocb_ptr).object_list[0].Orotz;
     if(ship_z_ang!=90) ship_z_ang++;
     (*ocb_ptr).object_list[0].Orotz=ship_z_ang;
     if ((*ocb_ptr).object_list[0].Oworldx<object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[0].Oworldx+=30;
     }
     if ((*ocb_ptr).object_list[0].Oworldx>object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[0].Oworldx-=30;
     }

		 //see if we've landed
	  pick_up_box.top= static_cast<int>(object_ptr->Oworldy+1000);
	  pick_up_box.bottom= static_cast<int>(object_ptr->Oworldy-1000);
	  pick_up_box.left= static_cast<int>(object_ptr->Oworldx-3000);
	  pick_up_box.right= static_cast<int>(object_ptr->Oworldx+3000);

	  //check whether we are in the end zone
	  if (point_in_rect( static_cast<int>((*ocb_ptr).object_list[0].Oworldx),
					   static_cast<int>((*ocb_ptr).object_list[0].Oworldy),
					  pick_up_box)) 
	  {
		 tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
		 tractor=0;
		 mothership_cat=object_ptr->object_category;	//so we get the right docked bg
		 docked_at=it;	//save slot of this mothership
		 docked_at_shields=object_ptr->shield_value;
		 clear_cont_sound(it);	//stop tractor sound

	  }

     break;
     }

   }	//end of switch

   dy=(*ocb_ptr).object_list[0].object_normal_translated.y*thrust;
   dx=(*ocb_ptr).object_list[0].object_normal_translated.x*thrust;
//use and adjust extern_movedx and y

   (*ocb_ptr).object_list[0].Omovedy=dy;
   (*ocb_ptr).object_list[0].Omovedx=dx;
   
}
else	//cu4!=1
 {
//is the ship close enough to get a tractor lock?
if ((*ocb_ptr).object_list[0].request_dock!=0)
{
 tractor_scan_object=aim_from_object_no_limit_wep(it,zex_object,15000);

 if (tractor_scan_object!=-1 && ((*ocb_ptr).object_list[tractor_scan_object].Oid=='ZEXS' || 
                 (*ocb_ptr).object_list[tractor_scan_object].Oid=='EC01') &&
                 (*ocb_ptr).object_list[tractor_scan_object].request_dock!=0 &&
                 docked_at==-1)	//zex found in range?
  {
  //see if we have a fast dock...
  (*ocb_ptr).object_list[0].Orotdy=0;	//fix for eject after running out of fuel
  (*ocb_ptr).object_list[0].Oroty=0;	//fix for eject after running out of fuel

  if (ship_items[KFAST_DOCK].status==1) fastdock_in_progress=1;
  else fastdock_in_progress=0;
  
  wingmen_guarding_what_object=it;	//make wingmen guard ship
        insert_continuous_sound(it, tractor_beam_loop,56);
        
 	object_ptr->control_use_4=1;	 //start tractoring
 	(*ocb_ptr).object_list[tractor_scan_object].docked_object_slot=it;	//poke our ID into the object so we don't switch our shields on!

      if (!are_we_running_ejection_music())
	  { fade_and_play_zex_resource(130); }

 //    	play_tune(128);	//docking theme - Full Circle by Renn
     	dock_timer=0;	   //start watchdog
     
 	if (object_ptr->object_category==zex_object)
  	add_to_text_display("HUMAN MOTHERSHIP HAS ATTAINED TRACTOR LOCK. DOCKING...",0);
  	else
  	add_to_text_display("ALIEN MOTHERSHIP HAS ATTAINED TRACTOR LOCK. DOCKING...",0);
  
//now is zex above or below us? It affects the way we bring here in
     	angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[0].Oworldx), 
										 static_cast<int>((*ocb_ptr).object_list[0].Oworldy), 
										 static_cast<int>(object_ptr->Oworldx), 
										 static_cast<int>(object_ptr->Oworldy));

 	if (angle_to_zex>=0 && angle_to_zex<=180) object_ptr->control_use_3=1;	 //above indicator
 	else
 	object_ptr->control_use_3=5;	 //below

 	tractor=1;	//extern used in ship_control to disbale player input while we ponce aabout
   	(*ocb_ptr).object_list[0].extern_movedx=0;   
   	(*ocb_ptr).object_list[0].extern_movedy=0;	//stop all this slidy stuff

  }	//end of request dock logic
 
 }	//end of request_dock!=0; 
  
 }	//end of else
} //end of tractor



// ***********************************************
// The Sim MotherShip controller follows...

//  *****Active mothership - the simulated one, the above ones are training mships only
// one routine handles both earth and alien (I hope)

void handle_active_mothership(int it)	  //tractor and laser - stationary position - gens bad guys
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int tractor,zex_alive;
extern	float SinTable[360], CosTable[360];

float shot_x,shot_y,shot_z;
int slot,object_aimed_at_slot;
load_dyn_object_pb the_params;
_2D start_coord,end_coord;
extern int thrust,level_complete;
extern int launch_timer;
extern float dest_x,dest_y;
extern int docked_at,docked_at_shields;
extern int have_ejected;
extern int score,current_level,showing_stats;

int ship_z_ang,angle_to_zex;
int tractor_scan_object,tractor_type;
LSRect pick_up_box;

float dx,dy,dock_offset,pos_y;
int sin1,sin2;

register ZObject * object_ptr;
extern int credits;
mothership_block * our_memory;

//cu1=laser fire interval
//cu2=free
//cu3=phase of tractor
//cu4=tractoring someone
//cu7=object aimed at from collision (or -1)
//cu8=object aimed at from collision active if > 0
//cu9=how many ships on board
//cu10=object that hit this mothership
//cu11=object just launched
//cu12=how many ships in this mothership
//cu13=ring position counter
//if paused then return!!!!
if (showing_stats==1) return;
    object_ptr=&ocb_ptr->object_list[it];
//check if initialised
if (object_ptr->mship_ptr==0)
       {
        object_ptr->mship_ptr=mship_get_mem();
          if (object_ptr->mship_ptr==0) 
           {
           report_error_c("handle_active_mothership: mship_get_mem failed! Please report this to LSG. Thanks","",-3);
           return;	//cant init
           }
       }
our_memory=object_ptr->mship_ptr;
//move the two rings
//located at slot+2, slot+3

//if (object_ptr->Oid=='MS10')
//{
// object_ptr->time_out+=2;
// if (object_ptr->time_out>359) object_ptr->time_out-=360;
// sin1=object_ptr->time_out;
// sin2=sin1-90;
// if (sin2<0) sin2+=360;
// 
// pos_y=(object_ptr->Oworldy); //-1000;
// (*ocb_ptr).object_list[it+2].Oworldy=pos_y+(SIN(sin1)*1000);
//// (*ocb_ptr).object_list[it+1].Oworldy=pos_y+(COS(sin1)*1000);
// pos_y=(object_ptr->Oworldy); //+1000;
// (*ocb_ptr).object_list[it+3].Oworldy=pos_y+(SIN(sin2)*1000);
//
//}
//
//initted how many ships held here?
if(object_ptr->cu12==-1) 
{
object_ptr->cu12=5;	//this mothership holds 5 aliens
}
//if shields less than 11700 recharge
if (object_ptr->shield_value<5800)
object_ptr->shield_value+=6;
else
if (object_ptr->shield_value<15800)
object_ptr->shield_value+=4;
else
if (object_ptr->shield_value<25800)
object_ptr->shield_value+=2;

//have we initialised how many ships we have on board?

object_ptr->control_use_9--;
if (object_ptr->control_use_9<0 && object_ptr->cu12>0)
{
object_ptr->control_use_9=RangedRdm(600,4000);	//frames to next ship

//call right npc generator
if (object_ptr->object_category==baddie_object)
{
int type=NPC_CLASS_FIGHTER;
 object_ptr->cu11=GenRandomMonsterWithVelocity( static_cast<int>(object_ptr->Oworldx),  static_cast<int>(object_ptr->Oworldy+300),500,70,it,type);	 //velocity, heading 
}
else
{
int type=NPC_CLASS_FIGHTER;
object_ptr->cu11=GenRandomHumanNPCWithVelocity( static_cast<int>(object_ptr->Oworldx),  static_cast<int>(object_ptr->Oworldy+300),500,70,it,type);	//velocity, heading
}

object_ptr->control_use_1=12;
//set invincible on mothership to avoid collision with launcher
//set invincible count on all abjects making up this mothership - 2 for alien, 3 for earth
if (object_ptr->Oid=='MS10')	//alien
{
 object_ptr->invincible_count=120;
 (*ocb_ptr).object_list[it+1].invincible_count=120;
}
else	//earth, ms01 and 2 ms02's
{
 object_ptr->invincible_count=120;
 (*ocb_ptr).object_list[it+1].invincible_count=120;
 (*ocb_ptr).object_list[it+2].invincible_count=120;
}

if(object_ptr->cu12>0)
object_ptr->cu12--;
 
}


 if (object_ptr->control_use_1<0) object_ptr->cu11=-1;	//not launching any more
 
 
//Find something to kill with the laser!!!
//alien motherships dont attack their own kind...
 object_ptr->control_use_1--;
 if (object_ptr->control_use_1<0 && object_ptr->is_armed==1)
 // && (*ocb_ptr).object_list[it].object_category==zex_object) 
 {
 object_ptr->control_use_1=12;
 //fire laser
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
//        if (object_ptr->object_category==baddie_object)
//	object_aimed_at_slot=aim_from_object_no_limit(it,zex_object,24000);	//pass the object, category to aim at and distance limit
	object_aimed_at_slot=find_something_to_attack(it,0,static_cast<int>(24000*LASER_RANGE_MULTIPLIER));	//pass the object, category to aim at and distance limit
//        else
//	object_aimed_at_slot=aim_from_object_no_limit(it,0,24000);	//pass the object, category to aim at and distance limit
        
        #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	if (object_aimed_at_slot==0) object_aimed_at_slot=object_ptr->attack_what;
	
	if (object_aimed_at_slot==0) 
	{
         //if we like zex we do not fire
         if (our_memory->like_zex>3)	//allowed to shot us 3 times before becomes enemy
            {
           }
           else
           {
 	    object_aimed_at_slot=-1;	//dont target zex!
	    object_ptr->attack_what=-1;
	   }
	}
	if (object_aimed_at_slot==-1 || object_aimed_at_slot==object_ptr->control_use_10)
	{ 
	//do nothing if the thing we targetted is being shot at becuase it shot us
	//or we didn't find anything suitable to shoot 
	}
	else
	{
	clear_pb(&the_params);
//FRIG
//we no laonger use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	
	 play_zsound_relative_to_zex(it,laser3);

	 shot_x=(*ocb_ptr).object_list[it].Oworldx;
	 shot_x=shot_x+(400*(*ocb_ptr).object_list[it].object_normal_translated.x);
	 shot_y=(*ocb_ptr).object_list[it].Oworldy;
	 shot_y=shot_y+(400*(*ocb_ptr).object_list[it].object_normal_translated.y);
	 shot_z=(*ocb_ptr).object_list[it].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
	 the_params.controller_ref=ship_laser1;	//controller
	 the_params.control_use_1=10;	//frames to live counter
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
	 the_params.control_use_3=it;	//who fired in ctrl 3
	 the_params.control_use_4=3;	//laser class


//	the_params.shield_value=0;
//     the_params.has_roll=0;
	 the_params.mass=0;	//should be 0
//		 	 the_params.dq=0;
	 the_params.object_category=zex_object;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=it;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)

//now set the poly to a zero shape
  
	}	//end of good slot
	}	//end of slot -1

}	//end of time to fire


////handle fire laser becuase something shot us!
////first see if we have an attack in progress
if (object_ptr->control_use_8>0 && object_ptr->is_armed==1)
{
object_ptr->control_use_8--;	 //timer for attack - stops at zero, target in cu7
if(object_ptr->control_use_8==3 || object_ptr->control_use_8==12)
   {
   //gen two nasties
if (object_ptr->Oid=='MS10')	//alien
{
 object_ptr->invincible_count=120;
 (*ocb_ptr).object_list[it+1].invincible_count=120;
}
else	//earth, ms01 and 2 ms02's
{
 object_ptr->invincible_count=120;
 (*ocb_ptr).object_list[it+1].invincible_count=120;
 (*ocb_ptr).object_list[it+2].invincible_count=120;
}


    object_ptr->cu11=GenRandomMonsterWithVelocity( static_cast<int>(object_ptr->Oworldx),  static_cast<int>(object_ptr->Oworldy+300),500,90,it,NPC_CLASS_FIGHTER);	//velocity, heading
    object_ptr->control_use_1=12;

   }
   
 object_ptr->control_use_1--;
 if (object_ptr->control_use_1<0)
 // && (*ocb_ptr).object_list[it].object_category==zex_object) 
 {
 object_ptr->control_use_1=12;
 //fire laser
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	object_aimed_at_slot = object_ptr->control_use_10;
	//check distance and if greater than 24000 cancel the attack
	if (object_aimed_at_slot!=-1)
	{
	int dt;
	  dt=get_distance_to_from(object_aimed_at_slot,it);
	  if (dt>24000)
	    {
	      object_ptr->control_use_10=-1;
	      object_aimed_at_slot=-1;
	    }
	}  
        #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	
	
	if (object_aimed_at_slot==-1)
	{ 
	//����this is a NOP in C -- might need to do something here...���� 
	}
	else
	{
	if (ocb_ptr->object_list[object_aimed_at_slot].in_use==1)
	{
 //FRIG
 //we no laonger use the engine to draw the laser - it's done with teh dlp,
 //but we still need a laser object in there for the controller, so we insert
 //a basic and very small triangle so as not to upset the renderer	 
 	 clear_pb(&the_params);
 	  play_zsound_relative_to_zex(it,laser3);
 
 	  shot_x=object_ptr->Oworldx;
 	  shot_x=shot_x+(400*object_ptr->object_normal_translated.x);
 	  shot_y=object_ptr->Oworldy;
 	  shot_y=shot_y+(400*object_ptr->object_normal_translated.y);
 	  shot_z=object_ptr->Oworldz;
 	 	 
 	  the_params.world_x=shot_x;
 	  the_params.world_y=shot_y;
 	  the_params.world_z=shot_z;
 	  the_params.controller_ref=ship_laser1;	//controller
 	  the_params.control_use_1=10;	 //frames to live counter
 	  the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
 	  the_params.control_use_3=it;	 //who fired in ctrl 3
 	  the_params.control_use_4=3;	//laser class
 
 	  the_params.mass=0;	 //should be 0 laser
 //		  	  the_params.dq=0;
 	  the_params.object_category=zex_object;
 	  the_params.no_trivial_rejection=1;
 	  the_params.unique_id=0;
      the_params.who_spawned_this_object=it;
 	  load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)
           }
//now set the poly to a zero shape
  
	}
    }	//end of good slot
 

  }	//end of control_use_1<0

}	//end of control_use_8>0

if (object_ptr->control_use_8<=0 ) object_ptr->control_use_10=-1;	//not attacking thing that shot us any more



//BIG difference between a real mothership and the training ones above is that this real one will dock
//anything requesting it and not just Zex...
//If you're not a complete and utter genius don't even bother reading past this line :)

//first see if ship is "in-range" and start tractoring it if so,
//after that look for enemies chasing ship to laser with a top notch, hunky laser 

//if (zex_alive==0) return;	//prevent any embrarassing crashes...
//if (launch_timer>0) return;

if (object_ptr->docking_what==-1)
	{
	object_ptr->control_use_4=0;
	object_ptr->control_use_3=0;
	}
//stop zex from running right through us!
//are we tractoring?

if (object_ptr->control_use_4==1)	//we wont run this if docking what is -1
{
if ((*ocb_ptr).object_list[object_ptr->docking_what].in_use==0)	//prevent anything embarrasing to my rep
								//as a quite superb coder happening... (object has been killed)
    {
    cancel_dock(object_ptr->docking_what);	//docking_what is OK here
    return;
    }
    
//    dock_timer++;
//    if (dock_timer>3300)
//    {
//    cancel_dock(ocb_ptr->docking_what);
//     return;
//    }
//see if player has enough credit to dock
if (object_ptr->docking_what==zex)	//docking_what is OK here
{
 if(have_ejected==0)	//we don't toll ejection pods
 {
     dock_fee=(credits/100)*(3+current_level);
     if (dock_fee<200) dock_fee=200;

     if (credits<dock_fee)
     {
       cancel_dock(0);
       add_to_text_display("OBC> Not enough credits to pay dock toll!",1);
       play_level_music();

       return;
     }

 }
}

 //do fade if fastdock in progress
 if (fastdock_in_progress==1)
 {
    RGBColor blue = {0,0,0xffff};
    
    if (fastdock_fade_value<-95)	//to red to zero and then to black below zero
        {
            tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
//     tractor=0;
     mothership_cat=object_ptr->object_category;	//so we get the right docked bg
     docked_at=it;	//save slot of this mothership
     docked_at_shields=object_ptr->shield_value;
     clear_cont_sound(it);	//stop tractor sound
        return;	//we're all docked up mama.
        }
    //fade by another 5 %
    //oldmac//DSpContext_FadeGamma ( mDisplayContext, fastdock_fade_value, &blue);
	LS_FadeGamma(fastdock_fade_value, &blue);

    fastdock_fade_value-=2;		//this is supposed to fade the screen out, but it looks like we can't do that now
    fastdock_fade_value-=10;	//so we'll just speed the whole thing up

 }

//first draw a tractor beam - we let the dlp do this
//    if (find_dynamic_object_slot('ZEXS')==-1)
//       (*ocb_ptr).object_list[it].control_use_4=0;	//zex is dead
    if(object_ptr->is_visible==0) return;	 //not visible so we cant calc coords
    if((*ocb_ptr).object_list[object_ptr->docking_what].is_visible==0) return;	     //docking what is OK here
      
    start_coord=get_screen_center_coord(it);	//get screen coord is in 3d_top
    end_coord=get_screen_center_coord(object_ptr->docking_what);	//docking what is OK here
    if (object_ptr->object_category==zex_object) 
    {
    	tractor_type=0;
//    dock_offset=2500;
    	dock_offset=0;
    }
    else
    {
    	tractor_type=1;	//alien tractor beam
    	dock_offset=0;
    }
    add_to_dlp(DRAW_TRACTOR,start_coord.x, start_coord.y,end_coord.x,end_coord.y,tractor_type,0);
    

//This switch statement controls itself to guide the ship in by altering control_use_3
///cases 1,2,3 are used when above mother ship, 5,6,7 below
    switch (object_ptr->control_use_3)
    {
//case 1 - ship somewhere above us - we want it at an angle of 45 to mothership
     case 1:
     {
     if (object_ptr->docking_what==0)	//docking_what is OK here
     thrust=120;
     else
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=120;	//docking_what is OK here
     (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=120;	//docking_what is OK here
     }
//carry out tractor mission
//first we want zex above us
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy)); //docking_what is OK here
     
     if (angle_to_zex<45 || angle_to_zex>135)	//to our right or left, so go up until 45 degrees is reached
      {
      if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=90) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz+=1;
      }
     else
      {
      object_ptr->control_use_3=2;
      }
     break;
    }
    
    case 2:
     {
     if (object_ptr->docking_what==0)
     thrust=60;
     else
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=60;
     (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=60;
     }

//
//now go left or right until angle==90
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     
     if (angle_to_zex>92)	//to our left, so go right
     {
     if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=0) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz--;
     }
     else
     if (angle_to_zex<88)	//to our right, so go left
     {
     if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=180) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz++;
     }
     else	//somewhere between 93 and 87 which is almost directly aboe
     {
     object_ptr->control_use_3=3;
     }
     break;
     }


     case 3:	//now all we do is turn to 90
     {
     if (object_ptr->docking_what==0)
     thrust=40;
     else
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=40;
     (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=40;
     }


//now bring zex in
//so, if zrot isn't 270 make it so
     ship_z_ang=(*ocb_ptr).object_list[object_ptr->docking_what].Orotz;
     if(ship_z_ang!=270) ship_z_ang-=1;
     
     (*ocb_ptr).object_list[object_ptr->docking_what].Orotz=ship_z_ang;
     
     if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx<object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx+=30;
     }
     if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx>object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx-=30;
     }
     //see if we've landed
  pick_up_box.top= static_cast<int>(object_ptr->Oworldy+1000);
  pick_up_box.bottom= static_cast<int>(object_ptr->Oworldy-1000);
  pick_up_box.left= static_cast<int>(object_ptr->Oworldx-3000);
  pick_up_box.right= static_cast<int>(object_ptr->Oworldx+3000);

  //check whether we are in the end zone
  if (point_in_rect( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx),
                   static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy),
                  pick_up_box)) 
  {
  
     if (object_ptr->docking_what==0)
     {  
     tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
     mothership_cat=object_ptr->object_category;	//so we get the right docked bg
     docked_at=it;	//save slot of this mothership
     docked_at_shields=(*ocb_ptr).object_list[it].shield_value;
     tractor=0;
     }
     else
     {
     object_ptr->cu12++;	//add one ship to mothership
     add_to_kill_list(object_ptr->docking_what);
     object_ptr->control_use_9=RangedRdm(700,1300);	//frames to next ship
     }
     object_ptr->docking_what=-1;
     clear_cont_sound(it);	//stop tractor sound

  }
     break;
     }


//ship below
//case 1 - ship somewhere above us - we want it at an angle of 45 to mothership
     case 5:
     {
     if (object_ptr->docking_what==0)
     thrust=120;
     else
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=120;
     (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=120;
     }
   
//carry out tractor mission
//first we want zex above and to our left
//so, if zrot isn't 150 make it so
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     if (angle_to_zex<215 || angle_to_zex>315)	//to our right or left, so go down until 45 degrees is reached
      {
      if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=270) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz+=1;
      }
     else
      {
      object_ptr->control_use_3=6;
      }
     break;
    }
    
    case 6:
     {
     if (object_ptr->docking_what==0)
     thrust=60;
     else
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=60;
     (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=60;
     }
//now go left or right until angle==270
     angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
									  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
									  static_cast<int>(object_ptr->Oworldx), 
									  static_cast<int>(object_ptr->Oworldy));
     if (angle_to_zex>272)	//to our left, so go right
     {
     if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=180) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz--;
     }
     else
     if (angle_to_zex<268)	//to our right, so go left
     {
     if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=0) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz++;
     }
     else	//somewhere between 267 and 273 which is almost directly below
     {
     (*ocb_ptr).object_list[it].control_use_3=7;
     }
     break;
     }


     case 7:	//now all we do is turn to 90
     {
     if (object_ptr->docking_what==0)
     thrust=40;
     else
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=40;
     (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=40;
     }


//now bring zex in
//so, if zrot isn't 90 make it so
     ship_z_ang=(*ocb_ptr).object_list[object_ptr->docking_what].Orotz;
     if(ship_z_ang!=90) ship_z_ang++;
     (*ocb_ptr).object_list[object_ptr->docking_what].Orotz=ship_z_ang;
     if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx<object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx+=30;
     }
     if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx>object_ptr->Oworldx+dock_offset) 
     {
     (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx-=30;
     }

     //see if we've landed
  pick_up_box.top= static_cast<int>(object_ptr->Oworldy+1000);
  pick_up_box.bottom= static_cast<int>(object_ptr->Oworldy-1000);
  pick_up_box.left= static_cast<int>(object_ptr->Oworldx-3000);
  pick_up_box.right= static_cast<int>(object_ptr->Oworldx+3000);

  //check whether we are in the end zone
  if (point_in_rect( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx),
                   static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy),
                  pick_up_box)) 
  {

     if (object_ptr->docking_what==0)
     {
     tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
     mothership_cat=object_ptr->object_category;	//so we get the right docked bg
     docked_at=it;	//save slot of this mothership
     docked_at_shields=object_ptr->shield_value;
     tractor=0;
     }
     else
     {
     object_ptr->cu12++;	//add one ship to mothership
     object_ptr->control_use_9=RangedRdm(700,1300);	//frames to next ship
     add_to_kill_list(object_ptr->docking_what);
     }
     object_ptr->docking_what=-1;
     clear_cont_sound(it);	//stop tractor sound

  }

     break;
     }

   }	//end of switch

  if (object_ptr->docking_what!=1)	//this is redundant, here we know docking what isn't -1
	{
     if (object_ptr->docking_what==0)
     {

       dy=(*ocb_ptr).object_list[0].object_normal_translated.y*thrust;
       dx=(*ocb_ptr).object_list[0].object_normal_translated.x*thrust;
//use and adjust extern_movedx and y

       (*ocb_ptr).object_list[0].Omovedy=dy;
       (*ocb_ptr).object_list[0].Omovedx=dx;
     } 
     else
     {

       dy=(*ocb_ptr).object_list[object_ptr->docking_what].object_normal_translated.y
           *(*ocb_ptr).object_list[object_ptr->docking_what].current_velocity;
      
       dx=(*ocb_ptr).object_list[object_ptr->docking_what].object_normal_translated.x
       *(*ocb_ptr).object_list[object_ptr->docking_what].current_velocity;
//use and adjust extern_movedx and y

       (*ocb_ptr).object_list[object_ptr->docking_what].Omovedy=dy;
       (*ocb_ptr).object_list[object_ptr->docking_what].Omovedx=dx;
     } 
	}	//docking_what!=-1
       
}	//end of docking_what checked
else	//cu4!=1
 {
//is a ship close enough to get a tractor lock?
//global_request_doc is set to the objects number when an object wants dock
 if (global_request_dock!=-1)
//now we have to see if the object is close enough to get a lock on
 {
  if (get_distance_to_from(global_request_dock, it)<15000) tractor_scan_object=global_request_dock;
  else tractor_scan_object=-1;

  if (tractor_scan_object!=-1 && (
                 (*ocb_ptr).object_list[tractor_scan_object].request_dock!=0 
                 ))	//object in range?
   {
   
   //check if it is zex do we like zex?
    if ((*ocb_ptr).object_list[tractor_scan_object].Oid=='ZEXS' && our_memory->like_zex>3)
    {
  	 add_to_text_display("OBC> Mothership is refusing dock request.",0);
         global_request_dock=1;	//cancel dock from zex
         goto nodock;
    }
   
  //see if we have a fast dock...
   (*ocb_ptr).object_list[tractor_scan_object].Orotdy=0;	//fix for eject after running out of fuel
   (*ocb_ptr).object_list[tractor_scan_object].Oroty=0;	//fix for eject after running out of fuel
   global_request_dock=-1;	//noted, number 1.
   if (tractor_scan_object==0)
    {
    if (ship_items[KFAST_DOCK].status==1) fastdock_in_progress=1;
    else fastdock_in_progress=0;
    }
   else
   fastdock_in_progress=0;
  
   if (tractor_scan_object==0)
    {
    wingmen_guarding_what_object=it;	//make wingmen guard ship
          if (!are_we_running_ejection_music())
			{ fade_and_play_zex_resource(130);	}
			
 	if (object_ptr->object_category==zex_object)
  	add_to_text_display("HUMAN MOTHERSHIP HAS ATTAINED TRACTOR LOCK. DOCKING...",0);
  	else
  	add_to_text_display("ALIEN MOTHERSHIP HAS ATTAINED TRACTOR LOCK. DOCKING...",0);
    }
  
   insert_continuous_sound(it, tractor_beam_loop,56);
        
 	object_ptr->control_use_4=1;	 //start tractoring
 	(*ocb_ptr).object_list[tractor_scan_object].docked_object_slot=it;	//poke our ID into the object so we don't switch our shields on!


//now is zex above or below us? It affects the way we bring here in
     	angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[tractor_scan_object].Oworldx), 
										 static_cast<int>((*ocb_ptr).object_list[tractor_scan_object].Oworldy), 
										 static_cast<int>(object_ptr->Oworldx), 
										 static_cast<int>(object_ptr->Oworldy));

 	if (angle_to_zex>=0 && angle_to_zex<=180) object_ptr->control_use_3=1;	 //above indicator
 	else
 	object_ptr->control_use_3=5;	 //below

 	 object_ptr->docking_what=tractor_scan_object;	 //extern used in ship_control to disbale player input while we ponce aabout
	if (object_ptr->docking_what==0) tractor=1;	//extern used in ship_control to disbale player input while we ponce aabout
    	 (*ocb_ptr).object_list[tractor_scan_object].extern_movedx=0;   
   	(*ocb_ptr).object_list[tractor_scan_object].extern_movedy=0;	//stop all this slidy stuff

  }	//end of request dock logic
 
 }	//end of global_request_dock!=0; 
  
 }	//end of else

nodock: {}
} //end of tractor






// ***********************************************
// The game MotherShip controller follows...

//  *****game mothership - the real one, the above ones are training mships only
// one routine handles both earth and alien (I hope)

void handle_game_mothership(int it)	//tractor and laser - stationary position - gens bad guys
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int tractor,zex_alive;
extern	float SinTable[360], CosTable[360];

float shot_x,shot_y,shot_z;
int slot,object_aimed_at_slot;
load_dyn_object_pb the_params;
_2D start_coord,end_coord;
extern int thrust,level_complete;
extern int launch_timer;
extern float dest_x,dest_y;
extern int docked_at,docked_at_shields;
extern int have_ejected;
extern int score,current_level,showing_stats,promotion_points;

int ship_z_ang,angle_to_zex;
int tractor_scan_object,tractor_type;
LSRect pick_up_box;

float dx,dy,dock_offset,pos_y;
int sin1,sin2;

register ZObject * object_ptr;
extern int credits;
mothership_block * our_memory;

//cu1=laser fire interval
//cu2=free
//cu3=phase of tractor
//cu4=tractoring someone
//cu7=object aimed at from collision (or -1)
//cu8=object aimed at from collision active if > 0
//cu9=how many ships on board
//cu10=object that hit this mothership
//cu11=object just launched
//cu12=how many ships in this mothership
//cu13=ring position counter
//if paused then return!!!!
if (showing_stats==1) return;

    object_ptr=&ocb_ptr->object_list[it];
    
//    if (object_ptr->object_category==HUMAN)	  //debug
//    {	//debug
//    int a;	  //debug
//    a++;	  //debug
//    }	//debug
    
//check if initialised
if (object_ptr->mship_ptr==0 )
       {

           report_error_c("handle_game_mothership: mship_ptr not initialised! Please report this to LSG. Thanks","",-3);
           return;	//cant init
           
       }

       
our_memory=object_ptr->mship_ptr;



if (our_memory->surrender_timer>0)
 our_memory->surrender_timer--;
 

if (object_ptr->shield_value<object_ptr->original_shield_value)
{
int shield_add,surrender;

surrender=0;
shield_add= static_cast<int>(our_memory->power_rating/10);
if (shield_add<1) shield_add=1;	//recharge MS shields
object_ptr->shield_value+=shield_add;
our_memory->power_rating-=0.02;	//decrement some power to recharge the shields
if (our_memory->power_rating<0) our_memory->power_rating=0;	//can't go negative on the power

//check for surrender conditions
   if (object_ptr->shield_value<7000 && our_memory->surrender_timer==0)
    {
	extern int gRank;
    surrender=1;
	score+=2000*gRank;
	credits+=200*gRank;
    }
//   if (our_memory->power_rating<1)
//    {
//    surrender=1;
//    }
    
   if (surrender==1)
    {
		 if (our_memory->allegiance==ALIEN)
		 {
			our_memory->allegiance=HUMAN;
			promotion_points+=RangedRdm(200,400);
			add_to_text_display("OBC> Congratulations! Mothership has surrendered!",2);

		 }
		 else
		 {
			our_memory->allegiance=ALIEN;
		 }

		 our_memory->callsign=make_callsign(our_memory->allegiance);
		 our_memory->surrender_timer=10000;	//cant surrender for 1000 frames
		 our_memory->like_zex=1;	//make sure after surrender player can dock
		 
		//make sure we stop tractoring
		if (object_ptr->docking_what!=-1) cancel_dock(object_ptr->docking_what);
		object_ptr->docking_what=-1;	//and we are no longer docking it
		clear_cont_sound(it);	//so stop tractor sound
		
		 //object_ptr->shield_value=20000;	//Leave the shields recharge normally
		 our_memory->power_rating=2;
		 force_ocb_update();	//reflect this change on screen
    }
}

if(our_memory->power_rating<our_memory->max_power_rating)
our_memory->power_rating+=0.01;

if (our_memory->power_rating>5)	//enough power to construct a ship
{
 object_ptr->control_use_9--;
 if (object_ptr->control_use_9<0)	//time to launch something?
 {
  object_ptr->control_use_9= static_cast<int>((4000-(our_memory->power_rating/*100*/+our_memory->production_rating/*100*/)*20));	//frames to next ship
  if (object_ptr->control_use_9<100) object_ptr->control_use_9=100;
//decrease the power rating
  our_memory->power_rating-=5;

        play_zsound_relative_to_zex(it,launch);


//call right npc generator
 if (object_ptr->object_category==baddie_object)
 {
 int type=-1;	//gen anything
  object_ptr->cu11=GenRandomMonsterWithVelocity( static_cast<int>(object_ptr->Oworldx), static_cast<int>(object_ptr->Oworldy+300),500,70,it,type);	  //velocity, heading 
 }
 else
 {
 int type=-1;
 object_ptr->cu11=GenRandomHumanNPCWithVelocity( static_cast<int>(object_ptr->Oworldx), static_cast<int>(object_ptr->Oworldy+300),500,70,it,type);	 //velocity, heading
 }

 object_ptr->control_use_1=12;
//set invincible on mothership to avoid collision with launcher
//set invincible count on all abjects making up this mothership - 2 for alien, 3 for earth
 if (object_ptr->Oid=='MS10')	 //alien
 {
  object_ptr->invincible_count=120;
  (*ocb_ptr).object_list[it+1].invincible_count=120;
 }
 else	//earth, ms01 and 2 ms02's
 {
  object_ptr->invincible_count=120;
  (*ocb_ptr).object_list[it+1].invincible_count=120;
  (*ocb_ptr).object_list[it+2].invincible_count=120;
 }

 if(object_ptr->cu12>0)
 object_ptr->cu12--;
 
 }
}	//end of enough power to gen a ship

//check we haven't gone below 0 on the power

if (our_memory->power_rating<0) our_memory->power_rating=0;


 if (object_ptr->control_use_1<0) object_ptr->cu11=-1;	//not launching any more
 
 
//Find something to kill with the laser!!!
//alien motherships dont attack their own kind...
 object_ptr->control_use_1--;
 if (object_ptr->control_use_1<0 && object_ptr->is_armed==1)
 // && (*ocb_ptr).object_list[it].object_category==zex_object) 
 {
 object_ptr->control_use_1=12;
 //fire laser
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	  OSType ob_type; 
          Byte *bp;
//        if (object_ptr->object_category==baddie_object)
//	object_aimed_at_slot=aim_from_object_no_limit(it,zex_object,24000);	//pass the object, category to aim at and distance limit
	object_aimed_at_slot=find_something_to_attack(it,0,static_cast<int>(24000*LASER_RANGE_MULTIPLIER));	//pass the object, category to aim at and distance limit
//        else
//	object_aimed_at_slot=aim_from_object_no_limit(it,0,24000);	//pass the object, category to aim at and distance limit
        
        #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	if (object_aimed_at_slot==0) object_aimed_at_slot=object_ptr->attack_what;
	
	if (object_aimed_at_slot >= 0)
	{		
		//change for 0.92, if it's a missle there's a chance we won't target it,depending on it's
		//class - hiogher class, more chance we won't target it.
       	ob_type = (*ocb_ptr).object_list[object_aimed_at_slot].Oid;  
        bp = (Byte *)&ob_type;
		//
        if (bp[0]=='Z' && bp[1]=='Z')	//missile we're aiming at?
        {
			//it's a missile
			if (bp[2]!='3') //clas 1 or 2
			{
				if (RangedRdm(0,100)>50) object_aimed_at_slot=-1;	//50/50 miss
			}
			else
			{
				if (bp[2]=='3') //clas
					object_aimed_at_slot=-1;	//dont target class 3 missiles
			}
		}
	}
	
	if (object_aimed_at_slot==0) 
	{
         //if we like zex we do not fire
         if (our_memory->like_zex>3)	//allowed to shot us 3 times before becomes enemy
            {
           }
           else
           {
 	    object_aimed_at_slot=-1;	//dont target zex!
	    object_ptr->attack_what=-1;
	   }
	}
	
	if (object_aimed_at_slot==-1 || object_aimed_at_slot==object_ptr->control_use_10)
	{ 
	//do nothing if the thing we targetted is being shot at becuase it shot us
	//or we didn't find anything suitable to shoot 
	}
	else
	{
	clear_pb(&the_params);
//FRIG
//we no longer use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	
	 play_zsound_relative_to_zex(it,laser3);

	 shot_x=(*ocb_ptr).object_list[it].Oworldx;
	 shot_x=shot_x+(400*(*ocb_ptr).object_list[it].object_normal_translated.x);
	 shot_y=(*ocb_ptr).object_list[it].Oworldy;
	 shot_y=shot_y+(400*(*ocb_ptr).object_list[it].object_normal_translated.y);
	 shot_z=(*ocb_ptr).object_list[it].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
	 the_params.controller_ref=ship_laser1;	//controller
	 the_params.control_use_1=10;	//frames to live counter
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
	 the_params.control_use_3=it;	//who fired in ctrl 3
	 the_params.control_use_4=3;	//laser class


//	the_params.shield_value=0;
//     the_params.has_roll=0;
	 the_params.mass=0;	//should be 0
//		 	 the_params.dq=0;
	 the_params.object_category=zex_object;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=it;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)

//now set the poly to a zero shape
  
	}	//end of good slot
	}	//end of slot -1

}	//end of time to fire


////handle fire laser becuase something shot us!
////first see if we have an attack in progress
if (object_ptr->control_use_8>0 && object_ptr->is_armed==1)
{
object_ptr->control_use_8--;	 //timer for attack - stops at zero, target in cu7
if(object_ptr->control_use_8==3 || object_ptr->control_use_8==12)	//send out ships at 12 and 3 in attack timer
   {
	   //if we are human and the thing that shot us was Zex, do nothing else
	   if (object_ptr->Oid!='MS10' && object_ptr->control_use_10==0)
	   {
			if (object_ptr->control_use_8==3)	//only fine once, at the end of the attack timer
			{
			   //fine 50 credits
			   credits-=50;
				if (credits<0) 
				{
				   credits=0;
				   add_to_text_display("Attacking our Mothership has cost you all your credits!",0);
				}
				else
				{
					add_to_text_display("Attacking our Mothership has cost you 50 credits!",0);
				}
			}
	   }
	   else	//not (human ms and zex that hit us)
	   {
			play_zsound_relative_to_zex(it,launch);

		   //gen two NPC's 
			if (object_ptr->Oid=='MS10')	//alien
			{
				object_ptr->invincible_count=120;
				(*ocb_ptr).object_list[it+1].invincible_count=120;	//for launch
				object_ptr->cu11=GenRandomMonsterWithVelocity( static_cast<int>(object_ptr->Oworldx), static_cast<int>(object_ptr->Oworldy+300),500,90,it,NPC_CLASS_FIGHTER);	//velocity, heading
				object_ptr->control_use_1=12;
			}
			else	//earth, ms01 and 2 ms02's
			{
				object_ptr->invincible_count=120;	//for launch
				(*ocb_ptr).object_list[it+1].invincible_count=120;
				(*ocb_ptr).object_list[it+2].invincible_count=120;
				object_ptr->cu11=GenRandomHumanNPCWithVelocity( static_cast<int>(object_ptr->Oworldx), static_cast<int>(object_ptr->Oworldy+300),500,90,it,NPC_CLASS_FIGHTER);	//velocity, heading
				object_ptr->control_use_1=12;
			}
	   }
   }	//end of cu3 is about to expire
   
 object_ptr->control_use_1--;
 if (object_ptr->control_use_1<0)
 // && (*ocb_ptr).object_list[it].object_category==zex_object) 
 {
 object_ptr->control_use_1=12;
 //fire laser
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	object_aimed_at_slot = object_ptr->control_use_10;
	//check distance and if greater than 24000 cancel the attack
	if (object_aimed_at_slot!=-1)
	{
	int dt;
	  dt=get_distance_to_from(object_aimed_at_slot,it);
	  if (dt>24000)
	    {
	      object_ptr->control_use_10=-1;
	      object_aimed_at_slot=-1;
	    }
	}  
        #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	
	
	if (object_aimed_at_slot==-1)
	{ 
	//����this is a NOP in C -- might need to do something here...���� 
	}
	else
	{
	if (ocb_ptr->object_list[object_aimed_at_slot].in_use==1)
	{
 //FRIG
 //we no laonger use the engine to draw the laser - it's done with teh dlp,
 //but we still need a laser object in there for the controller, so we insert
 //a basic and very small triangle so as not to upset the renderer	 
 	 clear_pb(&the_params);
 	  play_zsound_relative_to_zex(it,laser3);
 
 	  shot_x=object_ptr->Oworldx;
 	  shot_x=shot_x+(400*object_ptr->object_normal_translated.x);
 	  shot_y=object_ptr->Oworldy;
 	  shot_y=shot_y+(400*object_ptr->object_normal_translated.y);
 	  shot_z=object_ptr->Oworldz;
 	 	 
 	  the_params.world_x=shot_x;
 	  the_params.world_y=shot_y;
 	  the_params.world_z=shot_z;
 	  the_params.controller_ref=ship_laser1;	//controller
 	  the_params.control_use_1=10;	 //frames to live counter
 	  the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
 	  the_params.control_use_3=it;	 //who fired in ctrl 3
 	  the_params.control_use_4=3;	//laser class
 
 	  the_params.mass=0;	 //should be 0 laser
 //		  	  the_params.dq=0;
 	  the_params.object_category=zex_object;
 	  the_params.no_trivial_rejection=1;
 	  the_params.unique_id=0;
      the_params.who_spawned_this_object=it;
 	  load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)
           }
//now set the poly to a zero shape
  
	}
    }	//end of good slot
 

  }	//end of control_use_1<0

}	//end of control_use_8>0

if (object_ptr->control_use_8<=0 ) object_ptr->control_use_10=-1;	//not attacking thing that shot us any more



//BIG difference between a real mothership and the training ones above is that this real one will dock
//anything requesting it and not just Zex...
//If you're not a complete and utter genius don't even bother reading past this line :)

//first see if ship is "in-range" and start tractoring it if so,
//after that look for enemies chasing ship to laser with a top notch, hunky laser 

//if (zex_alive==0) return;	//prevent any embrarassing crashes...
//if (launch_timer>0) return;

if (object_ptr->docking_what==-1)	//game motherships continue to run whilst the player is docked, so lets first check if we docked something
	{
	object_ptr->control_use_4=0;	//and if so tell ourselves we no longer are docking. cu4 is docking
	object_ptr->control_use_3=0;	//cu3 is docking state control
	}
//stop zex from running right through us!
//are we tractoring?

if (object_ptr->control_use_4==1)
{
if ((*ocb_ptr).object_list[object_ptr->docking_what].in_use==0)	//prevent anything embarrasing to my rep
								//as a quite superb coder happening... docking_what is checked above by setting cu4 to 0
    {
    cancel_dock(object_ptr->docking_what);
    return;
    }
    
//    dock_timer++;	//Taken out Dec 2005
//    if (dock_timer>3300)
//    {
//    cancel_dock(ocb_ptr->docking_what);
//     return;
//    }
//see if player has enough credit to dock
if (object_ptr->docking_what==zex)
{
 if(have_ejected==0)	//we don't toll ejection pods
 {
 extern int current_quad,difficulty;	//in map_control
     //calc dock fee
     if (difficulty==1) dock_fee=30;
     else
     if (difficulty==8) dock_fee=20;
     else dock_fee=10;
     
     dock_fee*=current_quad+1;

     if (credits<dock_fee)
     {
       cancel_dock(0);
       add_to_text_display("OBC> Not enough credits to pay docking toll!",1);
       play_level_music();

       return;
     }

 }
}

 //do fade if fastdock in progress
 if (fastdock_in_progress==1 && object_ptr->docking_what==zex)
    {
    RGBColor blue = {0,0,0xffff};
    
    if (fastdock_fade_value<-95)	//to red to zero and then to black below zero
        {
             tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
     tractor=0;
     mothership_cat=object_ptr->object_category;	//so we get the right docked bg
	 
	 object_ptr->docking_what=-1;
    docked_at=it;	//save slot of this mothership
     docked_at_shields=object_ptr->shield_value;
     clear_cont_sound(it);	//stop tractor sound
        return;	//we're all docked up mama.
        }
    //fade by another 5 %
    //oldmac//DSpContext_FadeGamma ( mDisplayContext, fastdock_fade_value, &blue);
	LS_FadeGamma(fastdock_fade_value, &blue);
    fastdock_fade_value-=10;	//We should fade the screen out here

    }

//first draw a tractor beam - we let the dlp do this
    if(object_ptr->is_visible==0) return;	 //not visible so we cant calc coords
    if((*ocb_ptr).object_list[object_ptr->docking_what].is_visible==0) return;	     //same
      
    start_coord=get_screen_center_coord(it);	//get screen coord is in 3d_top
    end_coord=get_screen_center_coord(object_ptr->docking_what);
    if (object_ptr->object_category==zex_object) 
    {
    	tractor_type=0;
//    dock_offset=2500;
    	dock_offset=0;
    }
    else
    {
    	tractor_type=1;	//alien tractor beam
    	dock_offset=0;
    }
    add_to_dlp(DRAW_TRACTOR,start_coord.x, start_coord.y,end_coord.x,end_coord.y,tractor_type,0);
    

//This switch statement controls itself to guide the ship in by altering control_use_3
///cases 1,2,3 are used when above mother ship, 5,6,7 below
    switch (object_ptr->control_use_3)
    {
//case 1 - ship somewhere above us - we want it at an angle of 45 to mothership
		 case 1:
		 {
			 if (object_ptr->docking_what==0)
			 thrust=120;
			 else
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=120;
			 (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=120;
			 }
		//carry out tractor mission
		//first we want zex above us
			 angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
											  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
											  static_cast<int>(object_ptr->Oworldx), 
											  static_cast<int>(object_ptr->Oworldy));
			 
			 if (angle_to_zex<45 || angle_to_zex>135)	//to our right or left, so go up until 45 degrees is reached
			  {
			  if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=90) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz+=1;
			  }
			 else
			  {
			  object_ptr->control_use_3=2;
			  }
			 break;
		}
		
		case 2:
		 {
			 if (object_ptr->docking_what==0)
			 thrust=60;
			 else
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=60;
			 (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=60;
			 }

		//
		//now go left or right until angle==90
			 angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
											  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
											  static_cast<int>(object_ptr->Oworldx), 
											  static_cast<int>(object_ptr->Oworldy));
			 
			 if (angle_to_zex>92)	//to our left, so go right
			 {
			 if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=0) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz--;
			 }
			 else
			 if (angle_to_zex<88)	//to our right, so go left
			 {
			 if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=180) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz++;
			 }
			 else	//somewhere between 93 and 87 which is almost directly aboe
			 {
			 object_ptr->control_use_3=3;
			 }
			 break;
		 }


		 case 3:	//now all we do is turn to 90
		 {
			 if (object_ptr->docking_what==0)
			 thrust=40;
			 else
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=40;
			 (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=40;
			 }


		//now bring zex in
		//so, if zrot isn't 270 make it so
			 ship_z_ang=(*ocb_ptr).object_list[object_ptr->docking_what].Orotz;
			 if(ship_z_ang!=270) ship_z_ang-=1;
			 
			 (*ocb_ptr).object_list[object_ptr->docking_what].Orotz=ship_z_ang;
			 
			 if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx<object_ptr->Oworldx+dock_offset) 
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx+=30;
			 }
			 if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx>object_ptr->Oworldx+dock_offset) 
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx-=30;
			 }
			 //see if we've landed
			  pick_up_box.top= static_cast<int>(object_ptr->Oworldy+1000);
			  pick_up_box.bottom= static_cast<int>(object_ptr->Oworldy-1000);
			  pick_up_box.left= static_cast<int>(object_ptr->Oworldx-3000);
			  pick_up_box.right= static_cast<int>(object_ptr->Oworldx+3000);

			  //check whether we are in the end zone
			  if (point_in_rect( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx),
							   static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy),
							  pick_up_box)) 
			  {
				 if (object_ptr->docking_what==0)
				 {  
				 tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
				 mothership_cat=object_ptr->object_category;	//so we get the right docked bg
				 
				 docked_at=it;	//save slot of this mothership
				 docked_at_shields=(*ocb_ptr).object_list[it].shield_value;
				 tractor=0;
				 }
				 else
				 {
				 object_ptr->cu12++;	//add one ship to mothership
				 add_to_kill_list(object_ptr->docking_what);
				 object_ptr->control_use_9=RangedRdm(700,1300);	//frames to next ship
				 }
				 object_ptr->docking_what=-1;
				 clear_cont_sound(it);	//stop tractor sound

			}
			 break;
		 }


	//ship below
	//case 1 - ship somewhere above us - we want it at an angle of 45 to mothership
		 case 5:
		 {
			 if (object_ptr->docking_what==0)
			 thrust=120;
			 else
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=120;
			 (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=120;
			 }
		   
		//carry out tractor mission
		//first we want zex above and to our left
		//so, if zrot isn't 150 make it so
			 angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
											  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
											  static_cast<int>(object_ptr->Oworldx), 
											  static_cast<int>(object_ptr->Oworldy));
			 if (angle_to_zex<215 || angle_to_zex>315)	//to our right or left, so go down until 45 degrees is reached
			  {
			  if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=270) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz+=1;
			  }
			 else
			  {
			  object_ptr->control_use_3=6;
			  }
			 break;
		}
		
		case 6:
		 {
			 if (object_ptr->docking_what==0)
			 thrust=60;
			 else
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=60;
			 (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=60;
			 }
		//now go left or right until angle==270
			 angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx), 
											  static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy), 
											  static_cast<int>(object_ptr->Oworldx), 
											  static_cast<int>(object_ptr->Oworldy));
			 if (angle_to_zex>272)	//to our left, so go right
			 {
			 if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=180) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz--;
			 }
			 else
			 if (angle_to_zex<268)	//to our right, so go left
			 {
			 if((*ocb_ptr).object_list[object_ptr->docking_what].Orotz!=0) (*ocb_ptr).object_list[object_ptr->docking_what].Orotz++;
			 }
			 else	//somewhere between 267 and 273 which is almost directly below
			 {
			 (*ocb_ptr).object_list[it].control_use_3=7;
			 }
			 break;
		 }


		 case 7:	//now all we do is turn to 90
		 {
			 if (object_ptr->docking_what==0)
			 thrust=40;
			 else
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].wanted_velocity=40;
			 (*ocb_ptr).object_list[object_ptr->docking_what].current_velocity=40;
			 }


		//now bring zex in
		//so, if zrot isn't 90 make it so
			 ship_z_ang=(*ocb_ptr).object_list[object_ptr->docking_what].Orotz;
			 if(ship_z_ang!=90) ship_z_ang++;
			 (*ocb_ptr).object_list[object_ptr->docking_what].Orotz=ship_z_ang;
			 if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx<object_ptr->Oworldx+dock_offset) 
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx+=30;
			 }
			 if ((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx>object_ptr->Oworldx+dock_offset) 
			 {
			 (*ocb_ptr).object_list[object_ptr->docking_what].Oworldx-=30;
			 }

			 //see if we've landed
		  pick_up_box.top= static_cast<int>(object_ptr->Oworldy+1000);
		  pick_up_box.bottom= static_cast<int>(object_ptr->Oworldy-1000);
		  pick_up_box.left= static_cast<int>(object_ptr->Oworldx-3000);
		  pick_up_box.right= static_cast<int>(object_ptr->Oworldx+3000);

		  //check whether we are in the end zone
		  if (point_in_rect( static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldx),
						   static_cast<int>((*ocb_ptr).object_list[object_ptr->docking_what].Oworldy),
						  pick_up_box)) 
		  {

			 if (object_ptr->docking_what==0)
			 {
			 tell_engine_we_have_docked();	//Thank You, Thank You. Thank You..
			 mothership_cat=object_ptr->object_category;	//so we get the right docked bg
			 
			 docked_at=it;	//save slot of this mothership
			 docked_at_shields=object_ptr->shield_value;
			 tractor=0;
			 }
			 else
			 {
			 object_ptr->cu12++;	//add one ship to mothership
			 object_ptr->control_use_9=RangedRdm(700,1300);	//frames to next ship
			 add_to_kill_list(object_ptr->docking_what);
			 }
			 object_ptr->docking_what=-1;
			 clear_cont_sound(it);	//stop tractor sound

		  }

	break;
	}

   }	//end of switch


 if (object_ptr->docking_what!=-1)
 {
     if (object_ptr->docking_what==0)
     {

       dy=(*ocb_ptr).object_list[0].object_normal_translated.y*thrust;
       dx=(*ocb_ptr).object_list[0].object_normal_translated.x*thrust;
//use and adjust extern_movedx and y

       (*ocb_ptr).object_list[0].Omovedy=dy;
       (*ocb_ptr).object_list[0].Omovedx=dx;
     } 
     else
     {

       dy=(*ocb_ptr).object_list[object_ptr->docking_what].object_normal_translated.y
           *(*ocb_ptr).object_list[object_ptr->docking_what].current_velocity;
      
       dx=(*ocb_ptr).object_list[object_ptr->docking_what].object_normal_translated.x
       *(*ocb_ptr).object_list[object_ptr->docking_what].current_velocity;
//use and adjust extern_movedx and y

       (*ocb_ptr).object_list[object_ptr->docking_what].Omovedy=dy;
       (*ocb_ptr).object_list[object_ptr->docking_what].Omovedx=dx;
     } 
   }  
       
}	//end of docking_what is safe
else	//cu4!=1
 {
//is a ship close enough to get a tractor lock?
//global_request_doc is set to the objects number when an object wants dock
 if (global_request_dock!=-1)
//now we have to see if the object is close enough to get a lock on
 {
  if (get_distance_to_from(global_request_dock, it)<15000) tractor_scan_object=global_request_dock;
  else tractor_scan_object=-1;

  if (tractor_scan_object!=-1 && (
                 (*ocb_ptr).object_list[tractor_scan_object].request_dock!=0 
                 ))	//object in range?
   {
   
   //check if it is zex do we like zex?
    if ((*ocb_ptr).object_list[tractor_scan_object].Oid=='ZEXS' && our_memory->like_zex>3)
    {
  	add_to_text_display("OBC> Mothership is refusing our dock request.",0);
       global_request_dock=-1;
        goto nodock;
    }
   
  //see if we have a fast dock...
   (*ocb_ptr).object_list[tractor_scan_object].Orotdy=0;	//fix for eject after running out of fuel
   (*ocb_ptr).object_list[tractor_scan_object].Oroty=0;	//fix for eject after running out of fuel
   global_request_dock=-1;	//noted, number 1.
   if (tractor_scan_object==0)
    {
    if (ship_items[KFAST_DOCK].status==1) fastdock_in_progress=1;
    else fastdock_in_progress=0;
    }
   else
   fastdock_in_progress=0;
  
   if (tractor_scan_object==0)
    {
    wingmen_guarding_what_object=it;	//make wingmen guard ship
          if (!are_we_running_ejection_music())
			{ fade_and_play_zex_resource(130);	} 
 	if (object_ptr->object_category==zex_object)
  	add_to_text_display("HUMAN MOTHERSHIP HAS ATTAINED TRACTOR LOCK. DOCKING...",0);
  	else
  	add_to_text_display("ALIEN MOTHERSHIP HAS ATTAINED TRACTOR LOCK. DOCKING...",0);
    }
  
   insert_continuous_sound(it, tractor_beam_loop,56);
        
 	object_ptr->control_use_4=1;	 //start tractoring
 	(*ocb_ptr).object_list[tractor_scan_object].docked_object_slot=it;	//poke our ID into the object so we don't switch our shields on!


//now is zex above or below us? It affects the way we bring here in
     	angle_to_zex=get_angle_to_from ( static_cast<int>((*ocb_ptr).object_list[tractor_scan_object].Oworldx), 
										 static_cast<int>((*ocb_ptr).object_list[tractor_scan_object].Oworldy), 
										 static_cast<int>(object_ptr->Oworldx), 
										 static_cast<int>(object_ptr->Oworldy));

 	if (angle_to_zex>=0 && angle_to_zex<=180) object_ptr->control_use_3=1;	 //above indicator
 	else
 	object_ptr->control_use_3=5;	 //below

 	 object_ptr->docking_what=tractor_scan_object;	 //extern used in ship_control to disbale player input while we ponce aabout
	if (object_ptr->docking_what==0) tractor=1;	//extern used in ship_control to disbale player input while we ponce aabout
    	 (*ocb_ptr).object_list[tractor_scan_object].extern_movedx=0;   
   	(*ocb_ptr).object_list[tractor_scan_object].extern_movedy=0;	//stop all this slidy stuff

  }	//end of request dock logic
 
 }	//end of global_request_dock!=0; 
  
 }	//end of else

nodock: {}
} //end of tractor





void tell_engine_we_have_docked(void)
{
extern int level_complete;
	level_complete=3;	//this is the flag that tells the main loop we have docked
	//clear dlp so any lasers that were active are cleared as engine will stop running NPC's, weapons etc when we dock
	clear_dlp();
}






// *******************************************************
int thing_that_hit=-1;
unsigned int last_hit_time=0;
extern int training_mission,tutorial;
extern	DynObjectsFixedSize *ocb_ptr;
extern int launch_timer;

void do_mothership_hit(int object_slot, int mothership_slot)
{
extern	float SinTable[360], CosTable[360];
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;

load_dyn_object_pb the_params;
int slot;
float shot_x,shot_y,shot_z;

register ZObject * mothership_ptr;
register ZObject * object_ptr;



object_ptr=&ocb_ptr->object_list[object_slot];
mothership_ptr=&ocb_ptr->object_list[mothership_slot];

if ((mothership_ptr->docking_what!=-1) && (object_slot==mothership_ptr->docking_what)) return;	//we are tractoring this thing, so go away!
if (mothership_ptr->invincible_count>0) return;	//mships set this when they launch summat

//this does two things:
//1. repels the thing that hit the motehrship with a suitable sound
//2. Inserts the mothership shield dome into the ocb's
     object_ptr->hit_by_slot=-1;	//we've registered the hit


    
if (mothership_ptr->controller!=GAME_MOTHERSHIP) return; //is it a mothership controller? (Those crashes where my_mem would be 0 were 
															//where non-controller parts of a mothership were being hit)


//but first, find out who fired the thing that hit us and ATTACK THE F*****S
//if who_spawned_this_object is -1 then it's likely a ship has run into our shields - we forgive them.
//if however it is not, take the spawner as someone who fired and "lock them in" to our rather large laser..
if (object_ptr->who_spawned_this_object!=-1)
{
 int attacker;
 
 attacker=object_ptr->who_spawned_this_object;
 mothership_ptr->control_use_10=attacker;
 mothership_ptr->control_use_8=100;	//attack active counter, dec'd in mothership
 
 //if the attacker was Zex and we are an alien mship, we inc the like_zex bit in our memory
 //if it gets > 3 we can attack it
if (training_mission==0 && tutorial==0)
{
 if (attacker==zex)
    {
	 mothership_block * my_mem;
     if (mothership_ptr->object_category==ALIEN)
          {
			  my_mem=mothership_ptr->mship_ptr;
			  my_mem->like_zex++;
          }
		  else	//it's a human mothership
		  {
				my_mem=mothership_ptr->mship_ptr;
				my_mem->like_zex=1;	//this is a reset if we're a human mothership; it must dock zex
		  }
    }
}
}

//if we are docking the thing that has collided then run the mothership and return
if ((object_ptr->docked_object_slot!=-1) && (object_ptr->docked_object_slot==mothership_slot))
{
 if (tutorial==0)
 {
  if (training_mission!=0)
  handle_earth_mothership(mothership_slot);	//keep on tractoring baby - because it wont be called because of collision
  else
  handle_game_mothership(mothership_slot);
 }
 else
 handle_earth_mothership(mothership_slot);
 
 return;
}
//and it the thing that collided is being docked, return anyway
if (object_ptr->docked_object_slot!=-1) return;
//and if cu11==this objects slot (we just launched it) return;
    if (mothership_ptr->cu11==object_slot) return;
    
//right if all that's OK, repell the thing.

    if (ZexTickCount()>last_hit_time)
    {
		thing_that_hit=-1;
		last_hit_time=ZexTickCount()+30;
    }
    
    if (object_slot==thing_that_hit) return;	//only do it once
    thing_that_hit=object_slot;

if (launch_timer>0) return;	//no shields when launch in progress

//repel the thing that hit
   //calc reverse thrust
//     if ( (*ocb_ptr).object_list[object_slot].current_velocity>0)
//     {
//     (*ocb_ptr).object_list[object_slot].extern_movedx= -600* (*ocb_ptr).object_list[object_slot].object_normal_translated.x;
//     (*ocb_ptr).object_list[object_slot].extern_movedy= -600* (*ocb_ptr).object_list[object_slot].object_normal_translated.y;
//     (*ocb_ptr).object_list[object_slot].Omovedx= -600* (*ocb_ptr).object_list[object_slot].object_normal_translated.x;
//     (*ocb_ptr).object_list[object_slot].Omovedy= -600* (*ocb_ptr).object_list[object_slot].object_normal_translated.y;
//    (*ocb_ptr).object_list[object_slot].current_velocity = -600;
//     }
//     else
     //the thrust vector of the motherhsip is the angle to from :) (FRIG)
     
     {
     int angle_to_hitter,new_dx,new_dy,delta_rotz,vel;
     
//     vel=(*ocb_ptr).object_list[object_slot].current_velocity;
     
 //    if (object_slot==zex) 
 //    {
     delta_rotz=RangedRdm(60,140);
     if (RangedRdm(0,10)>5) delta_rotz=-delta_rotz;
     object_ptr->extern_rotdz=delta_rotz;	//+-70
 //    }
     angle_to_hitter=get_angle_toobject_fromobject(object_slot,mothership_slot);
     new_dx= static_cast<int>((700)*COS(angle_to_hitter));	//get vector components assuming new thrust is 600
     new_dy= static_cast<int>((700)*SIN(angle_to_hitter));
     
     object_ptr->extern_movedx= new_dx;
     object_ptr->extern_movedy= new_dy;
    object_ptr->current_velocity = 700;
//    (*ocb_ptr).object_list[object_slot].current_velocity = 700+vel;
     }
     
//dec shields
if (object_slot==zex) 
{
     if (ship_items[KSHIELDS].value1>0)
     {
     play_zsound(thud, sound_low_pri, c1_chan1, sound_vol_7);
     ship_items[KSHIELDS].value1=ship_items[KSHIELDS].value1-150;
     }
     else
     {	//we're taking hull damage - possible systems damage too!
     damage_me (150<<2);
     damage_something();
     if (hull_strength<=0)
     kill_zex();	//in ship control
     }

}
else 
damage_object(object_slot,150);


//if it's a wingman that has hit, issue a message
if (object_ptr->controller==WINGMAN_FOR_GAME) wingman_bounced_text(object_slot);

//(*ocb_ptr).object_list[object_slot].shield_value-=150;

if (mothership_ptr->showing_shield!=0) return;	    //we are showing the shield
mothership_ptr->showing_shield=1;


//insert the shield object

  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
        clear_pb(&the_params);
        
	shot_x=mothership_ptr->Oworldx;
	shot_y=mothership_ptr->Oworldy;
	shot_z=mothership_ptr->Oworldz;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;
	the_params.shield_value=30000;
	the_params.shields_class=5;
	the_params.delta_rot_z=RangedRdm(0,4)-2;
	if (the_params.delta_rot_z==0) 
	{
		if (RangedRdm(0,99)>50) 
		{
			the_params.delta_rot_z=3;
		}
		else
		{
			the_params.delta_rot_z=-3;
		}
	}
	the_params.controller_ref=large_shield;	//controller
	the_params.control_use_1=30;	//frames to live counter

	the_params.control_use_7=mothership_slot;	//set owner in cu7 so shield when killed can reset showing_shield

	the_params.mass=20;
  //      the_params.dq=0;
	the_params.object_category=mothership_ptr->object_category;
	the_params.no_trivial_rejection=1;

     the_params.who_spawned_this_object=-1;
	load_dyn_object(large_shield,slot,&the_params,-1);	//the static object, the position (-1=next free)
	}
     play_zsound_relative_to_zex(mothership_slot,large_shield_sound);


}








void cancel_dock(int object_being_docked)
{
int who_was_docking;
	if (object_being_docked!=-1)	//we've had a few problems here...
	{
	//clear sound of mothership doing the tractoring
		clear_cont_sound((*ocb_ptr).object_list[object_being_docked].docked_object_slot);	//stop tractor sound
		launch_timer=20;
		dock_timer=0;	//start watchdog
		(*ocb_ptr).object_list[object_being_docked].request_dock=0;
		who_was_docking=(*ocb_ptr).object_list[object_being_docked].docked_object_slot;
		if ((who_was_docking<0) || (who_was_docking>=MAX_NUMBER_OF_DYN_OBJECTS))
		{
			return;	//Cancel dock when no mothership in range.
		}
		
		(*ocb_ptr).object_list[who_was_docking].docking_what=-1;
		(*ocb_ptr).object_list[who_was_docking].control_use_3=0;
		(*ocb_ptr).object_list[object_being_docked].docked_object_slot=-1;
		global_request_dock=-1;
		
		if (object_being_docked==0)    
		{
			tractor=0;	    //extern used in ship_control to disbale player input while we ponce aabout
			play_level_music();

		}
	}
}


//--------motherships misc
//list of motherships calculated at level load time
int number_of_motherships;
mothership_nav_entry mothership_nav [10];
//scans active objects for motherships and adds their slot numbers to mothership_nav[]
//valid mothership object types are:
//MS02 - earth1
//MS10 - alien1
void load_motherships_into_nav()
{
extern DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;  
int active_object_counter,slot_counter;

active_object_counter = slot_counter = 0;
number_of_motherships=0;

while (active_object_counter<dynamic_object_index  && slot_counter<MAX_NUMBER_OF_DYN_OBJECTS)	//process all "in use" objects
 {

  if ((*ocb_ptr).object_list[slot_counter].in_use==1) //if object is alive
    {    
     if ((*ocb_ptr).object_list[slot_counter].Oid=='MS02')
     {
      mothership_nav[number_of_motherships].slot=slot_counter;
      mothership_nav[number_of_motherships].allegiance=zex_object;
      mothership_nav[number_of_motherships].x=(*ocb_ptr).object_list[slot_counter].Oworldx;
      mothership_nav[number_of_motherships].y=(*ocb_ptr).object_list[slot_counter].Oworldy;
      number_of_motherships++;
     }
     else
     if ((*ocb_ptr).object_list[slot_counter].Oid=='MS10')
     {
      mothership_nav[number_of_motherships].slot=slot_counter;
      mothership_nav[number_of_motherships].allegiance=baddie_object;
      mothership_nav[number_of_motherships].x=(*ocb_ptr).object_list[slot_counter].Oworldx;
      mothership_nav[number_of_motherships].y=(*ocb_ptr).object_list[slot_counter].Oworldy;
     number_of_motherships++;
     }
     
     active_object_counter++;  
    }
   slot_counter++;
  }  //end of while

}

//updates the master array of positions once every few frames
static int um_count=-1;
void update_motherships()
{
um_count--;
if (um_count<0)
  {
  um_count=38*2;	//frames to next run
  load_motherships_into_nav();
  }
  
}



// ********************Memory management for motherships ****************
#define NUMBER_OF_MSHIP_MEM_BLOCKS 30
mothership_block mship_blocks [NUMBER_OF_MSHIP_MEM_BLOCKS];

//must be called from game_init or reinit game
void init_mship_mem()
{
int i;
for (i=0;i<NUMBER_OF_MSHIP_MEM_BLOCKS;i++)
     mship_blocks[i].in_use=0;
}


//returns a ptr to a mothership_block or zero if none available
mothership_block * mship_get_mem()
{
int i;
for (i=0;i<NUMBER_OF_MSHIP_MEM_BLOCKS;i++)
  {
     if (mship_blocks[i].in_use==0) 
     {
      mship_blocks[i].in_use=1;
      mship_blocks[i].like_zex=1;
      return &mship_blocks[i];
     }
  }
return 0;	//failed
}

//returns 0 if OK, else -1
int mship_release_mem(mothership_block * the_block)
{
int i;
for (i=0;i<NUMBER_OF_MSHIP_MEM_BLOCKS;i++)
  {
     if (&mship_blocks[i]==the_block) 
     {
      mship_blocks[i].in_use=0;
      return 0;
     }
  }
return -1;	//failed
}

int is_a_mothership(int slot)
{
 if ((*ocb_ptr).object_list[slot].Oid=='MS02') return 1;
 else
 if ((*ocb_ptr).object_list[slot].Oid=='MS01') return 1;
 else
 if ((*ocb_ptr).object_list[slot].Oid=='MS10') return 1;
 else
 if ((*ocb_ptr).object_list[slot].Oid=='MS11') return 1;
 else
 if ((*ocb_ptr).object_list[slot].Oid=='MS12') return 1;
 else
 if ((*ocb_ptr).object_list[slot].Oid=='MS13') return 1;

 //else
 return 0;
}


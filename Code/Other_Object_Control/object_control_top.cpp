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

//object control top
/*
$Log: object_control_top.cpp,v $
Revision 1.26  2006/09/09 09:49:14  stu
Zex2005: Code: Static lasers now affected by the laser range multiplier. Default thrust mode set back to combat (since we display a message about switching thrust modes shortly after starting).

Revision 1.25  2006/06/25 11:16:05  rob
Zex2005: Game has now got a blacklist list which can be used to remove rogue serial numbers flying around the internet when we update the application.

Revision 1.24  2006/06/20 21:53:28  stu
1. Changed distribution of monsters depending on difficulty
2. Reduced shields of many monsters
3. Monsters now decelerate earlier so rams are less forceful.
4. Increased range of lasers, increased fire rate of lasers.
5. Phoenix now has level 1 cannon instead of 2.
6. If difficulty is Space Cadet then damage to Zex is divided by 2.
7. Damage to hull was rounded up to 1; it's now rounded down to 0.
8. kills now more selective - previously if you destroyed anything (including cannon
shells then kills would increment)

All these changes are labelled with //Beta 5

Revision 1.23  2006/05/25 20:47:27  rob
Zex2005: Added shockwave effect. Certainly makes shockwave more interesting.

Revision 1.22  2006/03/02 22:17:03  stu
Zex2005: Code: Checked hit_by_slot, docking_what and docked_object_slot. Also checked freeze in ship_control for various problems when GM is displayed on Space Cadet level.

Revision 1.21  2006/02/26 23:24:15  rob
Zex2005: Needed to clear out automatic variable that is used when inserting objects.

Revision 1.20  2006/02/26 00:08:41  rob
Zex2005: Added callsign debug and fixed init_memory.cpp build problem (oops!).

Revision 1.19  2006/02/25 21:43:27  rob
Zex2005: Corrected callsign dump. Also included write callsign write.

Revision 1.18  2006/02/05 19:30:25  rob
Zex2005: Debug stuff, currently disabled.

Revision 1.17  2006/01/22 12:39:26  rob
Zex2005: Switch res in game with a key plus optional callsign check (both UNFINISHED).

Revision 1.16  2006/01/07 13:56:24  stu
Zex2005: Code: A change in options.h to make buy show all ships items. As booked in this is on for items 0-14.

Revision 1.15  2006/01/04 23:49:07  stu
Zex2005:Code: Fixes a bug whereby you could ALWAYS hear ejection pods, not matter how far away they were from you.

Revision 1.14  2005/12/30 10:24:24  stu
Zex2005:Code: Should now be much more difficult to drill through walls as any collision has a constant damage quotient as well as the exisiting dynamic damage. Maybe with really big shields you can drill through a wall...(I didn't make it a function of the shields)

Revision 1.13  2005/12/20 20:45:23  rob
Zex2005: Various signed/unsigned comparison things plus one might be used uninitialised (not really - but got rid of warning).

Revision 1.12  2005/12/20 20:32:57  stu
Zex2005:Code: All object search loops now check they haven't run off the end of the dynamic OCB's

Revision 1.11  2005/12/16 22:01:46  stu
Zex2005: Code: more work

Revision 1.10  2005/12/13 23:56:00  rob
Zex2005: Got rid of the last few int->float conversion warnings.

Revision 1.9  2005/10/10 21:29:17  rob
Zex2005: Added newline at end to stop compiler whinging.

Revision 1.8  2005/10/08 13:14:33  rob
Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.

Revision 1.7  2005/09/29 00:49:53  stu
Zex2005: Code: Basic x86 port

Revision 1.6  2005/05/17 13:50:48  rob
Zex2005: More file changes.

Revision 1.5  2005/05/17 13:11:00  rob
Zex2005: Various assorted conversion fixes.

Revision 1.4  2005/05/15 21:40:12  rob
Zex2005: gcc4 fixes - Command and control bug and init memory.

Revision 1.3  2005/05/02 10:30:47  rob
Zex2005: More Updates (code)

Revision 1.2  2005/04/23 20:12:23  rob
ZEX: First set of changes for SDL build. (remainder of files)

Revision 1.40  1999/10/01 04:11:52  stu
0.94.2 011099 SB

Revision 1.39  1999/09/26 20:24:05  stu
First persistent special - cargo dump now OK

Revision 1.38  1999/09/26 18:34:49  stu
260999

Revision 1.37  1999/09/20 19:03:35  stu
0.93.3

Revision 1.36  1999/09/19 05:39:06  stu
0.93 as posted

Revision 1.35  1999/09/14 15:06:32  stu
140999

Revision 1.34  1999/09/12 00:56:21  stu
120999

Revision 1.33  1999/08/30 22:23:10  stu
300899

Revision 1.32  1999/08/20 09:57:41  stu
20 Aug 99

Revision 1.31  1999/08/15 22:17:07  stu
Cleanup 150899

Revision 1.30  1999/08/03 18:24:47  stu
*** empty log message ***

Revision 1.29  1999/07/26 21:35:59  stu
Pre test 2 27 July 99

Revision 1.28  1999/07/26 19:08:52  stu
Pre 0.86 test 27 July 99

Revision 1.27  1999/07/19 17:42:27  stu
200799

Revision 1.26  1999/07/17 18:35:58  stu
180799 - level 4 changes

Revision 1.25  1999/07/13 04:08:08  stu
Clean up 130799

Revision 1.24  1999/07/09 06:03:14  stu
Clean up 09-7-99

Revision 1.23  1999/07/04 18:37:23  stu
Clean up, Sunday 4th july

Revision 1.22  1999/07/04 00:21:39  stu
Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86

Revision 1.21  1999/06/27 19:27:53  stu
Clean up 27 June 99

Revision 1.20  1999/06/23 06:08:19  stu
Mothership collisions

Revision 1.19  1999/06/23 04:54:14  stu
More fixes related to motherships and weapons - still ongoing. Still docking sometimes dodgy.

Revision 1.18  1999/06/22 15:31:55  stu
Level 2 work

Revision 1.17  1999/06/22 10:57:39  stu
Lurkers clean up

Revision 1.16  1999/06/19 16:53:51  stu
This change ensures we don't flash the screen  red if there is a planet dipslayed.

Revision 1.15  1999/06/07 05:41:41  stu
Changes for tumbling_to_explode_object

Revision 1.14  1999/06/05 19:54:53  stu
*** empty log message ***

Revision 1.13  1999/06/05 17:23:00  stu
Major control change:
Objects now are controlled even if they are in collision. Previously they weren't.

Revision 1.12  1999/06/05 01:01:34  stu
Many minor object control problems fixed. See todo

Revision 1.11  1999/05/27 04:48:48  stu
Adds visible shield to Zex when hit (if shields are charged that is....)

Revision 1.10  1999/05/25 04:49:37  stu
Two changes:
1. Mothership hit code not descriminating between missiles and motherships (missiles not working)
2. Kill dust routine linked to camera z

Revision 1.9  1999/05/17 04:34:32  stu
Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c

Revision 1.8  1999/05/14 17:42:36  stu
Added Log to comments

Update: Adds who_spawned_this_object to an ocb, Used for finding out who fired a weapon mainly. Used by motherships now found in a separate file

Revision 1.7  1999/05/12 22:49:58  stu
Added Log to comments

Update: Adds who_spawned_this_object to an ocb, Used for finding out who fired a weapon mainly. Used by motherships now found in a separate file

Revision 1.6  1999/05/12 15:18:43  stu
Added Log to comments

Update: Adds who_spawned_this_object to an ocb, Used for finding out who fired a weapon mainly. Used by motherships now found in a separate file

*/

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <stdlib.h>
#include <string.h>		// for memcpy

//oldmac//#include <fp.h>
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
#include "cargo.h"
#include "transporters.h"
#include "NPC_Control.h"
//oldmac//#include "processor.h"
#include "register_game.h"
#include "game_defs.h"


int kill_list[10];
int kill_list_index=0;
int number_of_stars;	//how many stars displayed?
int monsters_can_fire;	//if 1, monsters can fire
//int difficulty;	//loaded from level loader
int planet_on_screen;
int global_open_doors;	//if zero, doors don't open
int global_control_monsters;	//if 0, monsters are frozen
extern int credits,score;
extern _3D camera_pos;
extern int warphole_core_slot;
void handle_door_vertical(int it);	//control vertical door
void handle_door_vertical_switched(int it);
void check_collision_physics(int current_object);
void handle_door_switch(int it);

void add_to_kill_list (int current_object)
{
extern int kill_list[10];
extern int kill_list_index;

     kill_list[kill_list_index]=current_object;
     if (kill_list_index<10)
        kill_list_index++;
}

void reset_kill_list()
{
extern int kill_list[10];
extern int kill_list_index;

 kill_list_index=0;
}

void kill_kill_list()
{
extern int kill_list[10];
extern int kill_list_index;
int i;
for (i=0;i<kill_list_index;i++)
   {
   kill(kill_list[i]);
   }
}


//еееееееееееееееееееееееееееееееееееееееееееееееееееее
//Note about lasers
//These are targetted devices and as such the controller for the laser knows the target
//and calculates the shield damage to the object on each frame the laser controller is run.
//All other weapons rely on collisions to do their damage.
//SB 30/12/98
extern	DynObjectsFixedSize *ocb_ptr;
extern int thrust,shields,hull;
extern int tractor,showing_stats;
extern int camera_rock;	//obviously the camera rot
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;
extern int launch_timer,attack_message_timer;

void do_object_control()
{
extern int	dynamic_object_index;	//how many dynamic objects to look at
extern int dynamic_object_total;
extern int ship_z_ang;
extern _3D ship_rotated_normal; //from engine
extern int frame_counter;	//bit 8 used as a little slow pseudo randomness
extern int camera_rotation_delta_z,zex_alive;
extern kit_desc instruments[ITEMS_COUNT];
extern int planet_on_screen;
extern	float SinTable[360], CosTable[360];
extern int jettison_timer;

int i;

int number_to_examine,current_object,hit_by_what_slot,hit_by_what_energy,current_object_pe;
int total_energy_in_coll,temp_velocity,t1;
short hit_by_what_mass;
int shield_damage,it2;
  OSType ob_type; 
  Byte *bp;
  OSType co_type; 
  short *cop;

//if (launch_timer>0) return;	//we are launching so dont check anything...
planet_on_screen=0;

if (jettison_timer>0) jettison_timer--;

number_to_examine=dynamic_object_index;	//we copy this in case we kill any objects
current_object=0;

for (i=0;i<number_to_examine;i++)
{
	while((*ocb_ptr).object_list[current_object].in_use==0)
	{
	current_object++;	//empty object
//	if (current_object>MAX_NUMBER_OF_DYN_OBJECTS) return;
	}
	
//	debug_printf(debug_verbose, "do_object_control - i = %i\n", i);
//	debug_printf(debug_verbose, "do_object_control - current_object = %i\n", current_object);

if ((*ocb_ptr).object_list[current_object].invincible_count>0)
    (*ocb_ptr).object_list[current_object].invincible_count--;
     
//cancel external rot z for object
if ((*ocb_ptr).object_list[current_object].extern_rotdz!=0 && current_object!=0)
{
 if ((*ocb_ptr).object_list[current_object].extern_rotdz>0) (*ocb_ptr).object_list[current_object].extern_rotdz--;
 else
 (*ocb_ptr).object_list[current_object].extern_rotdz++;
 
 
 (*ocb_ptr).object_list[current_object].Orotz+=(*ocb_ptr).object_list[current_object].extern_rotdz;
 
 if ((*ocb_ptr).object_list[current_object].Orotz>359) (*ocb_ptr).object_list[current_object].Orotz-=359;
 else
 if ((*ocb_ptr).object_list[current_object].Orotz<0) (*ocb_ptr).object_list[current_object].Orotz+=359;

}

//run collision detection for object
	if ((*ocb_ptr).object_list[current_object].crashable && showing_stats==0 && (*ocb_ptr).object_list[current_object].is_visible==1) 
	{
		int our_mass,thing_that_hit_us_mass,thing_that_hit_us_slot;
		
		our_mass=(*ocb_ptr).object_list[current_object].mass;

		check_collision(current_object);

		thing_that_hit_us_slot=(*ocb_ptr).object_list[current_object].hit_by_slot;
		thing_that_hit_us_mass = 1;		// just to avoid warning: 'thing_that_hit_us_mass' may be used uninitialized in this function
		if (thing_that_hit_us_slot!=-1)
		{
			thing_that_hit_us_mass=(*ocb_ptr).object_list[thing_that_hit_us_slot].mass;
		}
		if ((*ocb_ptr).object_list[current_object].object_category!=2 && thing_that_hit_us_slot!=-1 && our_mass<1000 && thing_that_hit_us_mass>100)	//it's moveable
		 {
		 
	//	    if ((*ocb_ptr).object_list[current_object].object_category!=2)	  //we dont move scenary objects
	//	      {
				 //we dont move bullets or missiles back
	//	               ob_type = (*ocb_ptr).object_list[hit_by_what_slot].Oid;  
	//                     bp = (Byte *)&ob_type;
	//
	//                     if (bp[0]=='M' && bp[1]=='I' && bp[2]=='S')
	//                     {
	//                     }
	//                     else
	//                     {
	//     	(*ocb_ptr).object_list[current_object].Oworldx=(*ocb_ptr).object_list[current_object].previous_x;
	//	                (*ocb_ptr).object_list[current_object].Oworldy=(*ocb_ptr).object_list[current_object].previous_y;
	//	               }
					 
				 if (current_object==zex)
				 {
					thrust=thrust/2;
				 }
				 else
				 {
					ob_type = (*ocb_ptr).object_list[thing_that_hit_us_slot].Oid;	//we know that thing_that_hit_us_slot is good here
							bp = (Byte *)&ob_type;

					 //we don't stop bullets
					 if (bp[0]=='M' && bp[1]=='I' && bp[2]=='S')
					 {
						//do nowt
					 }
					 else
					 {
						(*ocb_ptr).object_list[current_object].current_velocity=-((*ocb_ptr).object_list[current_object].current_velocity+30);
						(*ocb_ptr).object_list[current_object].wanted_velocity=-((*ocb_ptr).object_list[current_object].current_velocity+30);
				 
						//(*ocb_ptr).object_list[current_object].Oworldx=(*ocb_ptr).object_list[current_object].previous_x;
						//(*ocb_ptr).object_list[current_object].Oworldy=(*ocb_ptr).object_list[current_object].previous_y;
					 }
				 }
				 
		   
	//	}	//end of cat not 2
		 
		 
		 }	//end of hit and mass<1000
	}	//end of crashable and showing stats
        
//	if (current_object==zex) check_collision (current_object);	//Sets the hit_by_id field
	
//run controller for object

//store old x and y in case of collision
(*ocb_ptr).object_list[current_object].previous_x=(*ocb_ptr).object_list[current_object].Oworldx;
(*ocb_ptr).object_list[current_object].previous_y=(*ocb_ptr).object_list[current_object].Oworldy;


//is state 3?
if ((*ocb_ptr).object_list[current_object].no_trivial_rejection!=3)
{
ZObject * object_ptr;
    
    object_ptr=&ocb_ptr->object_list[current_object];
    
//    switch ((*ocb_ptr).object_list[current_object].controller)
    switch (object_ptr->controller)
     {
     case zex:
         {
//   	  current_object++;
   	break;
         }
     case player_bullet_weeny:
         {
 	      handle_weeny_bullet(current_object);
//	current_object++;
          break;
         }
     case player_bullet_not_so_weeny:
         {
 	      handle_not_so_weeny_bullet(current_object);
//	current_object++;
          break;
         }
     case dust:
         {
 	      handle_dust(current_object);
//	current_object++;
          break;
         }

     case dust_for_end:
         {
         float dist;
         
         dist=(*ocb_ptr).object_list[current_object].Oworldz-camera_pos.z;
          if (dist>10000)	//monsters do NOT run if not visible!!!
             {
             kill(current_object);

             }
                       break;
         }
         
     case DUST_FOR_HOLE:
         {
         int dist,dx,dy,ang_to;
         float z_dist,z_acc;
         
         int hole_x,hole_y,obj_x,obj_y,delta_x,delta_y;
         
         dist= static_cast<int>(object_ptr->Oworldz-camera_pos.z);
          if (dist>30000)	//monsters do NOT run if not visible!!!
             {
             kill(current_object);

             }
//move dust to hole accelerating towards center in x y and z...
//calc distance in z from z=0 (the mouth)
//            if ((*ocb_ptr).object_list[current_object].Oworldz>0) z_dist=0;
//            else
//            z_dist=0-(*ocb_ptr).object_list[current_object].Oworldz;
//            z_acc=10000-z_dist;
//            z_acc/=100;
//            (*ocb_ptr).object_list[current_object].Omovedz++;
//            (*ocb_ptr).object_list[current_object].Oworldz+=(*ocb_ptr).object_list[current_object].Omovedz;
             dist=get_distance_to_from(warphole_core_slot,current_object);
             if (dist<800)
            object_ptr->Oworldz+=220;
            else
            object_ptr->Oworldz+=3;
            
//first, move dust in general direction of warp hole
//            hole_x=(*ocb_ptr).object_list[warphole_core_slot].Oworldx;
//            hole_y=(*ocb_ptr).object_list[warphole_core_slot].Oworldy;
//
//            obj_x=(*ocb_ptr).object_list[current_object].Oworldx; 
//            obj_y=(*ocb_ptr).object_list[current_object].Oworldy;
//            
//            delta_x=ABS(hole_x-obj_x);
//            delta_x=delta_x/500;	  //
//            if (delta_x<1) delta_x=1;
//
//
//            delta_y=ABS(hole_y-obj_y);
//            delta_y=delta_y/500;	  //
//            if (delta_y<1) delta_y=1;
//            
////  	  	  ast_mass=(*ocb_ptr).object_list[it].shield_value/2000;
//            if (hole_x<obj_x) (*ocb_ptr).object_list[current_object].extern_movedx-=delta_x;
//            else (*ocb_ptr).object_list[current_object].extern_movedx+=delta_x;
//
//            if (hole_y<obj_y) (*ocb_ptr).object_list[current_object].extern_movedy-=delta_y;
//            else (*ocb_ptr).object_list[current_object].extern_movedy+=delta_y;

//         dist=(*ocb_ptr).object_list[current_object].Oworldz-camera_pos.z;
        ang_to=get_angle_toobject_fromobject(warphole_core_slot,current_object);
//         ang_to=(*ocb_ptr).object_list[warphole_core_slot].Orotz;
//         ang_to=offset_ang(ang_to,100);
         object_ptr->Orotz=ang_to;
//         
   if (dist>800)
   {
   dy= static_cast<int>(object_ptr->object_normal_translated.y*120);	//current velocity
   dx= static_cast<int>(object_ptr->object_normal_translated.x*120);
   }
   else
   {
   dy=1;
   dx=1;
   } 
//   dz=0-(*ocb_ptr).object_list[current_object].Oworldz;
//   dz=ABS(1000-dz);
//   if (dz>50) dz=50;
//   dz=0-(*ocb_ptr).object_list[current_object].Oworldz;	//acc faster as we get closer in z
//   dz=600-dz;
//   if (dz<0) dz=25;
//   (*ocb_ptr).object_list[current_object].Omovedz=dz;
   
//   dx+=(*ocb_ptr).object_list[it].extern_movedx;
//   dy+=(*ocb_ptr).object_list[it].extern_movedy;
//check if outside of play area and adjust dx and dy accordingly
   object_ptr->Oworldy+=dy;
   object_ptr->Oworldx+=dx;
//
                      break;
         }


     case exploding_object:
         {
 	      handle_explode(current_object);
//	current_object++;
          break;
         }

     case burning_particle:
         {
 	      handle_burning_particle(current_object);
//	current_object++;
          break;
         }

     case monster1:
         {
          if (object_ptr->is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_monster1(current_object);
//	current_object++;
          break;
         }


     case monster2:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_monster2(current_object);
//	current_object++;
          break;
         }

     case planet_rot_y:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_planet_roty(current_object);
//	current_object++;
          break;
         }

     case planet_rot_z:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_planet_rotz(current_object);
//	current_object++;
          break;
         }

     case monster3:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_monster3(current_object);
//	current_object++;
          break;
         }

     case monster4:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_monster4(current_object);
//	current_object++;
          break;
         }

     case monster6:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_monster6(current_object);
//	current_object++;
          break;
         }

     case warper:
         {
//always active
 	      handle_warper(current_object);
//	current_object++;
          break;
         }

     case excalabur:
         {
//always active
 	      handle_excalabur(current_object);
//	current_object++;
          break;
         }
     case DOOR_VERTICAL_SLIDING_DOWN_SHOT:
         {
//always active
 	      handle_door_vertical(current_object);
//	current_object++;
          break;
         }

     case boid:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_boid(current_object);
//	current_object++;
          break;
         }


     case ejection_cap:
         {
 	      handle_ejection(current_object);
//	current_object++;
          break;
         }
     
     case ship_laser1:
         {
 	      handle_laser_weeny(current_object,0);
//	current_object++;
          break;
         }

     case rear_ship_laser:
         {
 	      handle_laser_weeny(current_object,1);
//	current_object++;
          break;
         }

     case missile1:
         {
 	      handle_missile1(current_object);
//	current_object++;
          break;
         }

     case gun1:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
 //            current_object++;
             break;
             }
 	      handle_gun1(current_object);
//	current_object++;
          break;
         }

     case static_laser1:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_static_laser1(current_object);
//	current_object++;
          break;
         }


     case asteroid1:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_asteroid1(current_object);
//	current_object++;
          break;
         }
             
     case persistent_asteroid:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_persistent_asteroid(current_object);
//	current_object++;
          break;
         }

     case dlp_exploding_object:
         {
//          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
//             {
//             current_object++;
//             break;
//             }
          process_dlp_explosion(current_object);
// 	handle_asteroid1(current_object);
//	current_object++;
          break;
         }


//     case dlp_exploding_object:
//         {
//          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
//             {
//             current_object++;
//             break;
//             }
// 	handle_dlp_exploding_object(current_object);
//	        current_object++;
//          break;
//         }
//
     case space_cow:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_space_cow(current_object);
//	current_object++;
          break;
         }
      case earth_mothership:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_earth_mothership(current_object);
//	current_object++;
          break;
         }


      case ALIEN_ACTIVE_MOTHERSHIP: case EARTH_ACTIVE_MOTHERSHIP:
         {
 	      handle_active_mothership(current_object);
//	current_object++;
          break;
         }

      case GAME_MOTHERSHIP:
         {
 	      handle_game_mothership(current_object);
//	current_object++;
          break;
         }

      case ALIEN_SHUTTLE:
         {
 	      handle_alien_shuttle(current_object);
//	current_object++;
          break;
         }

      case NPC_CONTROL:
         {
 	      NPC_Control(current_object);
//	current_object++;
          break;
         }



      case large_shield:
         {
          object_ptr->control_use_1--;
          if (object_ptr->control_use_1<0) 
          {
          int owner;
          
          owner=object_ptr->control_use_7;	//owner of the large shield in cu7
          kill(current_object);
	  (*ocb_ptr).object_list[owner].showing_shield=0;
	  }
//	current_object++;
          break;
         }

		  
      case SHOCKWAVE_CONTROLLER:
	  {
		  const int max_num_points = 72;
		  static _3D shockwave_copy_buff[max_num_points];
		  static bool shockwave_copied = false;

          object_ptr->control_use_1 ++;
		  if(object_ptr->control_use_1 == 1 && !shockwave_copied)
		  {
			  // copy the number of points
			  _3D *shockwave_points_ptr = object_ptr->points_ptr;
			  int num_points = object_ptr->number_of_points;

			  if(num_points > max_num_points) { num_points = max_num_points; }
			  
			  for(int i=0; i < num_points; i++)
			  {
				  shockwave_copy_buff[i].x = shockwave_points_ptr[i].x;
				  shockwave_copy_buff[i].y = shockwave_points_ptr[i].y;
			  }

			  shockwave_copied = true;
		  }
          else if (object_ptr->control_use_1 > 1) 
          {
			  _3D *shockwave_points_ptr = object_ptr->points_ptr;
			  int num_points = object_ptr->number_of_points;
			  const double expansion_multiplier = 1.2;

			  for(int i=0; i < num_points; i++)
			  {
				  shockwave_points_ptr[i].x *= expansion_multiplier;
				  shockwave_points_ptr[i].y *= expansion_multiplier;
				  float y = shockwave_points_ptr[i].y;
				  
				  // check for last frame
				  if(y > object_ptr->control_use_2)
				  {
					  object_ptr->control_use_1  = -1;		// last frame
				  }

				  // check for specific damage size
				  if(object_ptr->control_use_7 == 0 && y > object_ptr->control_use_4)
				  {
					  object_ptr->control_use_7 = 1;
				  }
			  }
			  
			  // do damage if past specific size
			  if(object_ptr->control_use_7 == 1)
			  {
				  object_ptr->control_use_7 = 2;
				  damage_object_due_to_shockwave(object_ptr->control_use_2, object_ptr->control_use_3);
			  }
		  }
		  else if (object_ptr->control_use_1 <= 0)
		  {	
			  // make the shockwave the right size for the next time
			  // copy the number of points back from the copy buffer
			  _3D *shockwave_points_ptr = object_ptr->points_ptr;
			  int num_points = object_ptr->number_of_points;
			  
			  if(num_points > max_num_points) { num_points = max_num_points; }
			  
			  for(int i=0; i < num_points; i++)
			  {
				  shockwave_points_ptr[i].x = shockwave_copy_buff[i].x;
				  shockwave_points_ptr[i].y = shockwave_copy_buff[i].y;
			  }
			  
			  //
			  // Delete shockwave object
			  kill(current_object);
		  }

		  break;
	  }

      case zex_shield:
         {

          object_ptr->control_use_1--;
          if (object_ptr->control_use_1<0 || 
               ship_items[KSHIELDS].value1<1) 
           {
           kill(current_object);
	  (*ocb_ptr).object_list[object_ptr->who_spawned_this_object].showing_shield=0;	//owner is not showing shield anymore
	   }
	   else
	   {
	   //update shields position to that of Zex. Bit if a mouthful in C, but there ya go...
	   object_ptr->Oworldx=(*ocb_ptr).object_list[object_ptr->who_spawned_this_object].Oworldx;
	   object_ptr->Oworldy=(*ocb_ptr).object_list[object_ptr->who_spawned_this_object].Oworldy;
	   object_ptr->Oworldz=(*ocb_ptr).object_list[object_ptr->who_spawned_this_object].Oworldz;
	   //and rot
	   object_ptr->Orotx=(*ocb_ptr).object_list[object_ptr->who_spawned_this_object].Orotx;
	   object_ptr->Oroty=(*ocb_ptr).object_list[object_ptr->who_spawned_this_object].Oroty;
	   object_ptr->Orotz=(*ocb_ptr).object_list[object_ptr->who_spawned_this_object].Orotz;

	   }
//	current_object++;
          break;
         }




     case exploding_zex:
         {
          if ((*ocb_ptr).object_list[current_object].is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_exploding_zex(current_object);
//	current_object++;
          break;
         }

     case wingman1:
         {
 	      handle_wingman1(current_object);	//in monster1.c
//	current_object++;
          break;
         }

     case WINGMAN_FOR_GAME:
         {
 	      handle_wingman_for_game(current_object);	//in monster1.c
//	current_object++;
          break;
         }



     case DOOR_SWITCH:
         {
 	      handle_door_switch(current_object);	//in monster1.c
//	current_object++;
          break;
         }

     case CARGO_CONTAINER:
         {
         object_ptr->control_use_1++;
         
          if (object_ptr->control_use_1>500)	//its tumbled enough - time to kill it off...
             {
              add_to_explode_list(current_object,1);	//these draw rings
              add_to_kill_list(current_object);
              play_zsound_relative_to_zex(current_object, short_explosion);
            }
          break;
         }


     case DOOR_VERTICAL_SLIDING_DOWN_SWITCHED:
         {
           handle_door_vertical_switched(current_object);
          break;
         }

     case tumbling_to_explode_object:
         {
          if (object_ptr->control_use_1<1)	//its tumbled enough - time to kill it off...
             {
              int temp;
              add_to_explode_list(current_object,3);	//these draw rings

//if the thing that destroyed us was fired by zex then score etc.
             if (object_ptr->callsign!=0)  
             {
             do_callsign_text("     DESTROYED!",current_object,2);	//callsign goes in first 4 chars of string
//             if (score>1000) score-=1000;
//	        else score=0;
	     }
             temp=object_ptr->hit_by_slot; //what object hit this object
       
             if (temp!=-1 && ((*ocb_ptr).object_list[temp].object_category==zex_object))	//was the thing that hit this object a zex object
             { 
				add_to_score_and_credits(current_object); 
			 }
       
             add_to_kill_list (current_object);
             play_zsound_relative_to_zex(current_object, short_explosion);
             break;
             }
          else 	//not time to die
             {
//              if ((*ocb_ptr).object_list[current_object].control_use_1==8)
//              cause_dlp_explosion(current_object);

              object_ptr->control_use_1--;
              break;
             }
         }

    
     case player_ejection_pod:
         {
          if (object_ptr->is_visible==0)	//monsters do NOT run if not visible!!!
             {
//             current_object++;
             break;
             }
 	      handle_player_ejection_pod(current_object);
//	current_object++;
          break;
         }

     case star_for_story:
         {
          if (object_ptr->Oworldz<-800)	//monsters do NOT run if not visible!!!
             {
             object_ptr->Oworldz=3000;
//             (*ocb_ptr).object_list[current_object].Oworldx=RangedRdm(1,4000)-2000;
//			 (*ocb_ptr).object_list[current_object].Oworldy=RangedRdm(1,4000)-2000;

//             current_object++;
             break;
             }
         }

     case STAR_FOR_WARPHOLE:
         {
          if (object_ptr->Oworldz<-1200)	//monsters do NOT run if not visible!!!
             {
               kill(current_object);
//             (*ocb_ptr).object_list[current_object].Oworldx=RangedRdm(1,4000)-2000;
//			 (*ocb_ptr).object_list[current_object].Oworldy=RangedRdm(1,4000)-2000;

//             current_object++;
             break;
             }
         }

     default:
//          current_object++;	  //what to do with unknown controllers?
          break;
    }	//end of switch
  
 } //end of is state 3
  
//   } //end of if not collided

//   else //object collided
//   {


check_collision_physics(current_object);

     current_object++;



 }	//end of for i

	check_game_param_slowly();		// another blacklist check here...
}


//
//Check if collision
void check_collision_physics(int current_object)
{
extern int tutorial, difficulty;

int hit_by_what_slot,hit_by_what_mass,hit_by_what_energy,current_object_pe,total_energy_in_coll;
int shield_damage,it2;
  OSType ob_type; 
  Byte *bp;
  OSType co_type; 
  short *cop;
int temp_velocity,t1,hull_damage;
register ZObject * hit_by_ptr;
register ZObject * current_object_ptr;

     current_object_ptr=&ocb_ptr->object_list[current_object];

    //if this is a mothership return; it;s handled by the object hitting the mothership
    co_type = current_object_ptr->Oid;
    if (co_type=='MS02' || co_type=='MS01' || co_type=='MS11' || co_type=='MS10' || co_type=='MS12')	//the object was hit by a mothership
    return;

if (current_object_ptr->hit_by_slot!=-1)
  {
     if (current_object==zex) check_if_wingman_hit_zex();
     
     hit_by_what_slot=current_object_ptr->hit_by_slot;
	 
     hit_by_ptr=&ocb_ptr->object_list[hit_by_what_slot];

     hit_by_what_mass=hit_by_ptr->mass;
//process being hit by a mothership - 110599
    co_type = hit_by_ptr->Oid;
//    cop = (short *)&co_type;
//    if (co_type=='MS01' || co_type=='MS02' || co_type=='MS11' || co_type=='MS10')	//the object was hit by a mothership
    if (co_type=='MS02' || co_type=='MS01' || co_type=='MS11' || co_type=='MS10' || co_type=='MS12')	//the object was hit by a mothership
    {
        do_mothership_hit(current_object,hit_by_what_slot);
//        current_object++;
    }
    else
    {   
    
////special case for zex which has shields and HULL details, all the others just have shields
////rewritten 12/12/98 for hull
     if (current_object==zex)
     {
//check for cargo container
      if (hit_by_ptr->controller==CARGO_CONTAINER || hit_by_ptr->controller==PERSISTENT_CARGO_CONTAINER)
      {
      if (handle_pickup(hit_by_what_slot)==1);
       {
       current_object_ptr->hit_by_slot=-1;	//clear down hit
       return;
       }
      }
//calculate damage to zex
//calc mass*velocity=total kinetic energy of object that hit us
    
     hit_by_what_energy=hit_by_what_mass*abs_nobranch(hit_by_ptr->current_velocity);
//calc zex's pe
//     current_object_pe=(*ocb_ptr).object_list[zex].mass*ABS((*ocb_ptr).object_list[zex].Omovedx);
     current_object_pe=current_object_ptr->mass*abs_nobranch(thrust*2);
     total_energy_in_coll=abs_nobranch(-current_object_pe+hit_by_what_energy);
	 
	 total_energy_in_coll+=4000;	//for people trying to drill through walls...
     if (tractor==0 && hit_by_what_mass!=0) 
     {
     shield_damage=(total_energy_in_coll>>9);	 //no damage when being tractored
     shield_damage+=hit_by_ptr->dq;	//090499
     }
     else 
	 {
		shield_damage=0;
     }
     if (shield_damage>35) camera_rock=11;	//start rocking motion

//additional damage if it was a missile
    ob_type = hit_by_ptr->Oid;
    
    bp = (Byte *)&ob_type;
     
     if (ob_type=='AST1') shield_damage+=10;	//dont hit asteroids
     
     if (bp[0]=='Z'&& bp[2]=='1')
     {
     shield_damage+=300;
     hit_by_ptr->control_use_1=-1;	//ensure missile is destroyed
     add_to_kill_list(hit_by_what_slot);
     add_to_explode_list(hit_by_what_slot,3);	//these draw rings

     play_zsound_relative_to_zex(hit_by_what_slot,missile_explode);
     }
     else
     if (bp[0]=='Z'  && bp[2]=='2')
     {
     shield_damage+=600;
     hit_by_ptr->control_use_1=-1;	//ensure missile is destroyed
     add_to_kill_list(hit_by_what_slot);
     add_to_explode_list(hit_by_what_slot,5);	//these draw rings
     play_zsound_relative_to_zex(hit_by_what_slot,missile_explode);
     }

     else
     if (bp[0]=='Z' && bp[2]=='3')
     {
     shield_damage+=1200;
     hit_by_ptr->control_use_1=-1;	//ensure missile is destroyed
     add_to_kill_list(hit_by_what_slot);
     add_to_explode_list(hit_by_what_slot,5);	//these draw rings
     play_zsound_relative_to_zex(hit_by_what_slot,missile_explode);
     }
     else
//refuel and shield damage if space cow
     if (bp[2]=='4' && bp[0]=='M' && bp[1]=='N' )
     {
     shield_damage+=1;
     if (ship_items[KNORMAL_FUEL_TANK].value1<181)	//this stops us filling the lower half of the screen
         ship_items[KNORMAL_FUEL_TANK].value1+=1;  

     }

     //if Zex invincible and hit by cannon, no damage
     if ( current_object_ptr->invincible_count>0)
     {
//      if (hit_by_ptr->Oid=='MIS1' || hit_by_ptr->Oid=='MIS2' 
//       || hit_by_ptr->Oid=='MIS3') 
       shield_damage=0;
     }

//Reduce damage as per difficulty
	 if (difficulty==16)	//beta 5
	 {
		shield_damage/=2;
	 }
	 
//calc hull damage as damage-shield strength
     hull_damage=ship_items[KSHIELDS].value1-shield_damage;
     if (hull_damage>0) hull_damage=0;
     else
     hull_damage=-hull_damage;
     
     if (ship_items[KSHIELDS].value1>0)
     {
     play_zsound(thud, sound_low_pri, c1_chan1, sound_vol_7);
     ship_items[KSHIELDS].value1=ship_items[KSHIELDS].value1-shield_damage;
     //Gen ships shield
     GenZexShield();	//in object_control_misc_routines.c
     }
     
     if (hull_damage>0 && tutorial==0)
     {
     damage_me (hull_damage<<2);	//beta 5
     damage_something();
     }
//calculate directional change which is stored in extern_movedx and y and moved and decremented in engine
//control
//after many moons cocking about I finally realised all we had to do was copy delta_x and y
//out of the object that hit us and adjust for our mass!
//this is a long way from being perfect...

  if (current_object_ptr->mass<hit_by_what_mass) t1=1;	//prevent a Velocity overflow
  else
  t1=current_object_ptr->mass/hit_by_what_mass;	//difference in mass


//another tweak - 200199
//if the object we hit has no velocity but it's mass is great, then we reverse our direction by a given amount
   if (hit_by_ptr->Omovedx==0 && hit_by_ptr->Omovedy==0)
   {
   //calc reverse thrust
	 current_object_ptr->extern_movedx+= -current_object_ptr->Omovedx;
     current_object_ptr->extern_movedy+= -current_object_ptr->Omovedy;
     current_object_ptr->Omovedx=-current_object_ptr->Omovedx/3;
     current_object_ptr->Omovedy=-current_object_ptr->Omovedy/3;
   }
   else	//object that hit had some velocity
   {
//  (*ocb_ptr).object_list[zex].extern_movedx=(*ocb_ptr).object_list[hit_by_what_slot].Omovedx/t1;
//  (*ocb_ptr).object_list[zex].extern_movedy=(*ocb_ptr).object_list[hit_by_what_slot].Omovedy/t1;
  current_object_ptr->extern_movedx=hit_by_ptr->Omovedx/t1;
  current_object_ptr->extern_movedy=hit_by_ptr->Omovedy/t1;
//  (*ocb_ptr).object_list[hit_by_what_slot].Omovedx=0;
//  (*ocb_ptr).object_list[hit_by_what_slot].Omovedy=0;
  }
//check if we're destroyed
     if (hull_strength>0)
     {
//      (*ocb_ptr).object_list[zex].Omovedx=-(*ocb_ptr).object_list[zex].Omovedx;
      current_object_ptr->hit_by_slot=-1;	//clear down hit
//      if (shield_damage>1) play_zsound(shield_scratch, sound_low_pri, twin2_chan, sound_vol_7);
     }
     else	//ship detroyed
     {
     kill_zex();	//in ship control
     }
//     current_object++;
     }
     



    else	//not zex that was hit



     {
 
 
     if (hit_by_what_mass!=0)
     {
//calculate damage to object
//calc mass*velocity=total kinetic energy of object that hit us
     temp_velocity=abs_nobranch(hit_by_ptr->current_velocity);
     hit_by_what_energy=hit_by_what_mass * temp_velocity;
//calc zex's pe
     temp_velocity=1 + abs_nobranch(current_object_ptr->current_velocity);
     current_object_pe=current_object_ptr->mass * temp_velocity;
     
//     current_object_pe=(*ocb_ptr).object_list[zex].mass*ABS(thrust);
     total_energy_in_coll=abs_nobranch(-current_object_pe+hit_by_what_energy);
     shield_damage=total_energy_in_coll>>9;
     shield_damage+=hit_by_ptr->dq;	//090499

  if (current_object_ptr->mass<hit_by_what_mass) t1=1;	     //prevent a Velocity overflow
  else
  t1=current_object_ptr->mass/hit_by_what_mass;	//difference in mass

     
//additional damage if it was a missile that hit the object
    ob_type = hit_by_ptr->Oid;  
    bp = (Byte *)&ob_type;

     if (ob_type=='AST1') shield_damage+=10;	//dont hit asteroids

     if (bp[0]=='Z' && bp[2]=='1')
     {
     //if the thing hit was a boid and we have boid_addition then destroy boid
      ob_type = current_object_ptr->Oid;  
      bp = (Byte *)&ob_type;
      if (bp[0]=='B' && bp[1]=='I' && bp[2]=='D' && ship_items[KBOID_ADDITION].status==1)
      shield_damage+=20000;
      else
      shield_damage+=300;
//if the thing the missile hit was its target, then destroy it
     if (hit_by_ptr->control_use_2==current_object)
      {
      hit_by_ptr->control_use_1=-1;	//ensure missile is destroyed
          add_to_explode_list(hit_by_what_slot,3);	//these draw rings

       play_zsound_relative_to_zex(hit_by_what_slot,missile_explode);
      }
     }
     else
     if (bp[0]=='Z' && bp[2]=='2')
     {
     //if the thing hit was a boid and we have boid_addition then destroy boid
      ob_type = current_object_ptr->Oid;  
      bp = (Byte *)&ob_type;
      if (bp[0]=='B' && bp[1]=='I' && bp[2]=='D' && ship_items[KBOID_ADDITION].status==1)
      shield_damage+=20000;
      else
      shield_damage+=600;

     if (hit_by_ptr->control_use_2==current_object)
      {

      add_to_explode_list(hit_by_what_slot,5);	//these draw rings
       hit_by_ptr->control_use_1=-1;	  //ensure missile is destroyed
  
      play_zsound_relative_to_zex(hit_by_what_slot,missile_explode);
      }
     }
     else
     if (bp[0]=='Z' && bp[2]=='3')
     {
     //if the thing hit was a boid and we have boid_addition then destroy boid
      ob_type = current_object_ptr->Oid;  
      bp = (Byte *)&ob_type;
      if (bp[0]=='B' && bp[1]=='I' && bp[2]=='D' && ship_items[KBOID_ADDITION].status==1)
      shield_damage+=20000;
      else
      shield_damage+=1200;

     if (hit_by_ptr->control_use_2==current_object)
      {

          add_to_explode_list(hit_by_what_slot,5);	//these draw rings
     hit_by_ptr->control_use_1=-1;	//ensure missile is destroyed

      play_zsound_relative_to_zex(hit_by_what_slot,missile_explode);
      }
     }

     //additional damage if the thing that was hit is an asteroid and the thing that
     //hit it was a shell and we have the asteroid addition
     
     if (current_object_ptr->Oid=='AST1')
     {
     
       if (ship_items[KASTEROID_ADDITION].status==1)
       {
        unsigned long hitter_oid;
         hitter_oid=hit_by_ptr->Oid;
         if (hitter_oid=='MIS1' || hitter_oid=='MIS2' || hitter_oid=='MIS3')
         shield_damage*=128; 
       }
     } 
     
     
     
     damage_object(current_object,shield_damage);
             
        
     current_object_ptr->hit_by_slot=-1;	//we've registered the hit
   
    }	//end of it had some mass
   }	//end of else not zex


    }	//end of else not mothership that hit object
  } //end of collided
}
//ееееееееееееееееееееееееееееееееееееееееееееееееееееее

void handle_laser_weeny (int it, int rear)	 //SB 301298
{
extern	DynObjectsFixedSize *ocb_ptr;
//_ZPoly *the_polys;
//textured_connections *pconnections;
//_3D p3;	//our three points
//_3D *this_object_points;
//float target_y;
//signed long temp_colour;
//cu7=laser experience
int aimed_at,who_fired;

//these for the little explosion
extern _ZPoly_objs *dynamic_polys_ptr;
extern int	dynamic_object_index,tractor;	//how many dynamic objects
extern int flash_screen,planet_on_screen;
extern textured_connections fixed_triag_connection [3];
extern _3D fixed_triag_points[3];

//_3D *points_list_ptr_base;
ZObject *source_object;
int i,dest_ocb,poly_slot, size_of_polys;
_ZPoly *source_object_polys;
_ZPoly *new_polys_address;
_ZPoly *old_polys_address;
//_ZPoly *new_poly;
//textured_connections *new_connection;
_2D start_coord,end_coord;
int laser_cat;
int doing_a_miss;	//from laser experience

laser_cat=(*ocb_ptr).object_list[it].control_use_4;

(*ocb_ptr).object_list[it].control_use_1--;
    aimed_at=(*ocb_ptr).object_list[it].control_use_2;
    who_fired=(*ocb_ptr).object_list[it].control_use_3;
	if ((aimed_at==-1) || (who_fired==-1)) return;

if ((*ocb_ptr).object_list[it].control_use_1<1 || (*ocb_ptr).object_list[who_fired].in_use==0
     || (*ocb_ptr).object_list[aimed_at].in_use==0)
 { 
 kill(it);
 return;
 }
else
 { 
 (*ocb_ptr).object_list[it].control_use_1--;	//dec frame count

//these laser controllers are tricky in that we have to twiddle the points of the poly
//laser controllers assume the poly is a simple three pointed triangle
//and that points 1 and 3 originaate at the firer and point to is on the target
//the slot of which is in control_use_2, so we make sure points 1 and 3 remain at the firer 
//and point 2 on the target
//control_use_3 is the colour which is inc'd, wrapped and poked into the poly as well. Deep Joy.
    
//    if (aimed_at<0 || aimed_at>500) return;	 //quick error check in case a monster controller has really cocked up
//laser revved 18/1/99 to use dlp
//draw a laser (line) from it to aimed_at
//note the drastic reduction in code!
    if((*ocb_ptr).object_list[aimed_at].is_visible==0) return;	  //not visible so we cant calc coords
    if((*ocb_ptr).object_list[who_fired].is_visible==0) return;	//same

    if (rear==0) start_coord=get_screen_front_coord(who_fired);	//get screen coord is in 3d_top
    else
    start_coord=get_screen_rear_coord(who_fired);
    
    end_coord=get_screen_center_coord(aimed_at);
//if doing a miss
//    doing_a_miss=(*ocb_ptr).object_list[it].control_use_7;
//    if (doing_a_miss==1)
//    {
//    end_coord.x+=RangedRdm(0,40)-20;
//    end_coord.y+=RangedRdm(0,40)-20;
//    }
//
    
///debug
if (start_coord.x<-2000 || start_coord.x>2000) return; //DebugStr("laser error");
if (end_coord.x<-2000 || end_coord.x>2000) return; //DebugStr("laser error");
if (start_coord.y<-2000 || start_coord.y>2000) return; //DebugStr("laser error");
if (end_coord.y<-2000 || end_coord.y>2000) return; //DebugStr("laser error");

    if (laser_cat==1)
    add_to_dlp(DRAW_LASER1,start_coord.x, start_coord.y,end_coord.x,end_coord.y,0,0);
	else
    if (laser_cat==2)
    add_to_dlp(DRAW_LASER2,start_coord.x, start_coord.y,end_coord.x,end_coord.y,0,0);
    else    
    if (laser_cat==3)
    add_to_dlp(DRAW_LASER3,start_coord.x, start_coord.y,end_coord.x,end_coord.y,0,0);

   
//    if (doing_a_miss==1) return;
    
//throw a few polys off target - we just do one per frame
  source_object=&(*ocb_ptr).object_list[aimed_at];	//saves lots of possible code
  source_object_polys=(*source_object).polys_ptr;


if ((*ocb_ptr).object_list[aimed_at].docked_object_slot!=-1) return;	//it is being docked and hence is protected
  
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
  {
    int i;
//create a new object from each poly
    i=0;
//	(*ocb_ptr).object_list[dest_ocb]=(*ocb_ptr).object_list[static_slot];	//copy the object
	(*ocb_ptr).object_list[dest_ocb].in_use=1;	//Let's not "where do you want to crash today?" Stuforgot this and hence receives the WOW award.
	(*ocb_ptr).object_list[dest_ocb].Oworldx=(*source_object).Oworldx;
	(*ocb_ptr).object_list[dest_ocb].Oworldy=(*source_object).Oworldy;
	(*ocb_ptr).object_list[dest_ocb].Oworldz=(*source_object).Oworldz;
	(*ocb_ptr).object_list[dest_ocb].Orotdx=RangedRdm(0,20);
	(*ocb_ptr).object_list[dest_ocb].Orotdy=RangedRdm(0,20);
	(*ocb_ptr).object_list[dest_ocb].Orotdz=RangedRdm (0,20);
	(*ocb_ptr).object_list[dest_ocb].Orotx=(*source_object).Orotx;
	(*ocb_ptr).object_list[dest_ocb].Oroty=(*source_object).Oroty;
	(*ocb_ptr).object_list[dest_ocb].Orotz=(*source_object).Orotz;
	(*ocb_ptr).object_list[dest_ocb].Omovedx=RangedRdm(0,60)-30;
	(*ocb_ptr).object_list[dest_ocb].Omovedy=RangedRdm(0,60)-30;
	(*ocb_ptr).object_list[dest_ocb].Omovedz=RangedRdm(0,20)-10;
	(*ocb_ptr).object_list[dest_ocb].controller=exploding_object;
	(*ocb_ptr).object_list[dest_ocb].object_category=scenary_object;
	(*ocb_ptr).object_list[dest_ocb].control_use_1=RangedRdm(20,50);	//controller counts this down
	(*ocb_ptr).object_list[dest_ocb].control_use_2=0;
	  (*ocb_ptr).object_list[dest_ocb].hit_by_slot=-1;
	  (*ocb_ptr).object_list[dest_ocb].crashable=no;	//this object does not need col_detect
//	(*ocb_ptr).object_list[dest_ocb].number_owned=0;
//	(*ocb_ptr).object_list[dest_ocb].Oreference=1;
//	(*ocb_ptr).object_list[dest_ocb].immediate_child=NULL;
	  (*ocb_ptr).object_list[dest_ocb].number_of_polys=1;
	  (*ocb_ptr).object_list[dest_ocb].number_of_points=3;
	(*ocb_ptr).object_list[dest_ocb].anim_frame=0;
	(*ocb_ptr).object_list[dest_ocb].anim_max_object=1;	//not animated

//now copy polys out of static into vacant dynamic
	poly_slot=find_vacant_dyn_poly_slot();
	if	(poly_slot!=-1)
 	 {
	  dynamic_polys_ptr[poly_slot].in_use=1;	//prevent a crash - this poly slot is in use!
	  //copy the polys across
	  new_polys_address=&dynamic_polys_ptr[poly_slot].the_obj_polys[0];	//so we can poke the new object
	  old_polys_address=source_object_polys+i;	//point to right poly!
	  size_of_polys=sizeof(_ZPoly);	//we have just one poly to copy	
	  BlockMoveData	(old_polys_address,new_polys_address,size_of_polys);

	  //poke the address of the polys into the new object
      (*ocb_ptr).object_list[dest_ocb].polys_ptr=new_polys_address;
	  (*ocb_ptr).object_list[dest_ocb].active_polys_slot=poly_slot;	//so we can quickly kill these polys

	  (*new_polys_address).poly_connections_ptr=fixed_triag_connection;	//poke our new connections in - 1,2,3
  	  (*ocb_ptr).object_list[dest_ocb].points_ptr=fixed_triag_points;	//now originated at 0

	}	//end of poly slot
	else	//oops, couldn't get any polys! (Found on intel port)
	 {
		(*ocb_ptr).object_list[dest_ocb].in_use=0;
		return;
	 }
	dynamic_object_index++;	//we've added an object that needs to be processed
   } //end of good slot for poly


//now do damage to ship
//    if ((*ocb_ptr).object_list[aimed_at].crashable==0) return;	//can't kill things not crashable! (Shields for example)
  if ((*ocb_ptr).object_list[aimed_at].Oid=='SHL1' || (*ocb_ptr).object_list[aimed_at].Oid=='SHLZ') return;	//dont shoot shields!

    if (aimed_at!=0)
    {
    (*ocb_ptr).object_list[aimed_at].hit_by_slot=it;
    
    if (  ((*ocb_ptr).object_list[aimed_at].Oid=='MS01' || (*ocb_ptr).object_list[aimed_at].Oid=='MS02' ) && attack_message_timer==0) 
    {
    add_to_text_display("HUMAN MOTHERSHIP UNDER ATTACK!!!!",1);
    attack_message_timer=200;
    }
    
     
       if (damage_object(aimed_at,(*ocb_ptr).object_list[it].control_use_4*30)==1) 
       {
       kill(it);	//very important we kill lasers if the target is dead
      if ((*ocb_ptr).object_list[who_fired].in_use==1) (*ocb_ptr).object_list[who_fired].attack_what=-1;
   
       }
     //special case for motherships v0.92
     if (is_a_mothership(aimed_at)==1)
      do_mothership_hit(it,aimed_at);
          
     }
     else	//zex that was hit by a laser
     {
     
      if ((*ocb_ptr).object_list[aimed_at].invincible_count==0)
      {
        if (ship_items[KSHIELDS].value1>0)
        {
         if (tractor==0) ship_items[KSHIELDS].value1=ship_items[KSHIELDS].value1-((*ocb_ptr).object_list[it].control_use_4*12);
         GenZexShield();	//in object_control_misc_routines.c

        }
      
      else
       {	//we're taking hull damage - possible systems damage too!
       damage_me ((*ocb_ptr).object_list[it].control_use_4*6);
//     instruments[RangedRdm(0,5)]=2;	//kill a system at random
	  damage_something();
       }
     
     
//check if we're destroyed
      if (hull_strength<=0)
      kill_zex();
      
     }	//end of not invincible
     
    }	//end of else must've been zex hit
 }	//end of control_use_1>0

}


//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_weeny_bullet(int it)	//lets the bullet live for 60 frames
{
extern	DynObjectsFixedSize *ocb_ptr;


(*ocb_ptr).object_list[it].control_use_1+=2;
if ((*ocb_ptr).object_list[it].control_use_1>60)
 { 
 kill(it);
 }
else
 { 
// (*ocb_ptr).object_list[it].control_use_1++;	  //inc frame count
// (*ocb_ptr).object_list[it].Omovedx-=4*(*ocb_ptr).object_list[it].control_use_2;	  //decelerate
 //(*ocb_ptr).object_list[it].thrust_y= (*ocb_ptr).object_list[it].Omovedy;	//update thrust y
 //(*ocb_ptr).object_list[it].thrust_x= (*ocb_ptr).object_list[it].Omovedx;	//update thrust x

 }
}


void handle_planet_roty(int it)	//rotate slowly every control_1 frames
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int planet_on_screen;
planet_on_screen=1;

////do planet z which varies between 100000 and 200000
//  if ((*ocb_ptr).object_list[it].control_use_4==0) (*ocb_ptr).object_list[it].control_use_4=10;
//  (*ocb_ptr).object_list[it].Oworldz+=(*ocb_ptr).object_list[it].control_use_4;
//  if ((*ocb_ptr).object_list[it].Oworldz<40000) (*ocb_ptr).object_list[it].control_use_4=-(*ocb_ptr).object_list[it].control_use_4;
//  else
//  if ((*ocb_ptr).object_list[it].Oworldz>230000) (*ocb_ptr).object_list[it].control_use_4=-(*ocb_ptr).object_list[it].control_use_4;

  (*ocb_ptr).object_list[it].control_use_2--;

  if ((*ocb_ptr).object_list[it].control_use_2<=0)
  {
   (*ocb_ptr).object_list[it].control_use_2=(*ocb_ptr).object_list[it].control_use_1;
   (*ocb_ptr).object_list[it].Oroty+=(*ocb_ptr).object_list[it].control_use_3;
   
   if ((*ocb_ptr).object_list[it].Oroty>359) (*ocb_ptr).object_list[it].Oroty-=360;
   else
   if ((*ocb_ptr).object_list[it].Oroty<0) (*ocb_ptr).object_list[it].Oroty+=360;
  }
}

void handle_planet_rotz(int it)	//rotate slowly every control_1 frames
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int planet_on_screen;
planet_on_screen=1;
 return;
//do planet z which varies between 100000 and 200000
//  if ((*ocb_ptr).object_list[it].control_use_4==0) (*ocb_ptr).object_list[it].control_use_4=10;
//  (*ocb_ptr).object_list[it].Oworldz+=(*ocb_ptr).object_list[it].control_use_4;
//  if ((*ocb_ptr).object_list[it].Oworldz<40000) (*ocb_ptr).object_list[it].control_use_4=-(*ocb_ptr).object_list[it].control_use_4;
//  else
//  if ((*ocb_ptr).object_list[it].Oworldz>230000) (*ocb_ptr).object_list[it].control_use_4=-(*ocb_ptr).object_list[it].control_use_4;


//  (*ocb_ptr).object_list[it].control_use_2--;
//
//  if ((*ocb_ptr).object_list[it].control_use_2<=0)
//  {
//   (*ocb_ptr).object_list[it].control_use_2=(*ocb_ptr).object_list[it].control_use_1;
//   (*ocb_ptr).object_list[it].Orotz+=(*ocb_ptr).object_list[it].control_use_3;
//   
//   if ((*ocb_ptr).object_list[it].Orotz>359) (*ocb_ptr).object_list[it].Orotz-=360;
//   else
//   if ((*ocb_ptr).object_list[it].Orotz<0) (*ocb_ptr).object_list[it].Orotz+=360;
//  }
}


//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_space_cow(int it)	 //lets the bullet live for 60 frames
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
int object_count,active_object_count;
//search for objects close by and if their mass is < 900 attract them towards us
int ast_x,ast_y,obj_x,obj_y,distance_to;

(*ocb_ptr).object_list[it].control_use_8=(*ocb_ptr).object_list[it].hit_by_slot;	//who hit sc? (So it can try ram the hitter)

if ((*ocb_ptr).object_list[it].just_hit!=0 )	//&& (*ocb_ptr).object_list[it].control_use_8==zex) 
{
(*ocb_ptr).object_list[it].control_use_2+=10;	//we use this as a counter of how long cow chases player
if ((*ocb_ptr).object_list[it].control_use_3==0) 
              play_zsound_relative_to_zex(it, mono_roar);

//    play_zsound(mono_roar, sound_high_pri, speech_chan, sound_vol_3);
(*ocb_ptr).object_list[it].control_use_3=1;
 
//(*ocb_ptr).object_list[it].Orotdz=83;
}

if ((*ocb_ptr).object_list[it].control_use_2==0) 
{
(*ocb_ptr).object_list[it].control_use_3=0;	//sound control
}

if ((*ocb_ptr).object_list[it].control_use_2>0)
{
(*ocb_ptr).object_list[it].control_use_2--;	//chase counter
 


(*ocb_ptr).object_list[it].current_velocity=100;
if ((*ocb_ptr).object_list[it].control_use_8!=-1)
{
	ram_object((*ocb_ptr).object_list[it].control_use_8,it,1);
}
if ((*ocb_ptr).object_list[it].control_use_2==0)
{
 (*ocb_ptr).object_list[it].Omovedx=0;
 (*ocb_ptr).object_list[it].Omovedy=0;	//stop
}
//suck in objects
ast_x= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
ast_y= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

 object_count=active_object_count=0;
 while (active_object_count<dynamic_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
 {
  if ((*ocb_ptr).object_list[object_count].in_use==1 && object_count!=it)
  {
    distance_to=get_distance_to_from(object_count,it);
    if (distance_to<13000 && (*ocb_ptr).object_list[object_count].mass<1000) 
      {
        obj_x= static_cast<int>((*ocb_ptr).object_list[object_count].Oworldx); 
        obj_y= static_cast<int>((*ocb_ptr).object_list[object_count].Oworldy);
//		ast_mass=(*ocb_ptr).object_list[it].shield_value/2000;
        if (ast_x<obj_x) (*ocb_ptr).object_list[object_count].extern_movedx-=4;
        else (*ocb_ptr).object_list[object_count].extern_movedx+=4;

        if (ast_y<obj_y) (*ocb_ptr).object_list[object_count].extern_movedy-=4;
        else (*ocb_ptr).object_list[object_count].extern_movedy+=4;
        
        
      }
   active_object_count++;
  } //end of if on
   object_count++;
 }	//next object - end of while
 


}
//else
//if ((*ocb_ptr).object_list[it].control_use_2>0) (*ocb_ptr).object_list[it].control_use_2--;
//
//(*ocb_ptr).object_list[it].Orotdz=(*ocb_ptr).object_list[it].control_use_2/20;


if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	//clear down just hit for next frame

// (*ocb_ptr).object_list[it].thrust_x= (*ocb_ptr).object_list[it].Omovedx;	//update thrust x

}


//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
//Moded 040699 SB
void handle_asteroid1(int it)	 //control asteroids magnetism
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
int object_count,active_object_count;
//search for objects close by and if their mass is < 900 attract them towards us
int ast_x,ast_y,obj_x,obj_y,distance_to;


//change 8/3/99 - if distance to zex>200000 then kill the asteroid

ast_x= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
ast_y= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);


if (get_distance_to_zex(it)>200000) add_to_kill_list(it);

 object_count=active_object_count=0;
 while (active_object_count<dynamic_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
 {
  if ((*ocb_ptr).object_list[object_count].in_use==1  ) 
  {
    if ((*ocb_ptr).object_list[object_count].crashable==1 && object_count!=it)
    {
    distance_to=get_distance_to_from(object_count,it);
    if (distance_to<11000 && distance_to>900 && (*ocb_ptr).object_list[object_count].mass<1000 ) 
      {
        obj_x= static_cast<int>((*ocb_ptr).object_list[object_count].Oworldx); 
        obj_y= static_cast<int>((*ocb_ptr).object_list[object_count].Oworldy);
//		ast_mass=(*ocb_ptr).object_list[it].shield_value/2000;
        if (ast_x<obj_x) (*ocb_ptr).object_list[object_count].extern_movedx-=6;
        else (*ocb_ptr).object_list[object_count].extern_movedx+=6;

        if (ast_y<obj_y) (*ocb_ptr).object_list[object_count].extern_movedy-=6;
        else (*ocb_ptr).object_list[object_count].extern_movedy+=6;
     }   
        
   }
   active_object_count++;
  } //end of if on
   object_count++;
 }	//next object - end of while
}


void handle_persistent_asteroid(int it)	//control asteroids magnetism
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
int object_count,active_object_count;
//search for objects close by and if their mass is < 900 attract them towards us
int ast_x,ast_y,obj_x,obj_y,distance_to;


//change 8/3/99 - if distance to zex>200000 then kill the asteroid

ast_x= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
ast_y= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);


//if (get_distance_to_zex(it)>200000) add_to_kill_list(it);

 object_count=active_object_count=0;
 while (active_object_count<dynamic_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
 {
  if ((*ocb_ptr).object_list[object_count].in_use==1  ) 
  {
    if ((*ocb_ptr).object_list[object_count].crashable==1 && object_count!=it)
    {
    distance_to=get_distance_to_from(object_count,it);
    if (distance_to<11000 && distance_to>900 && (*ocb_ptr).object_list[object_count].mass<1000 ) 
      {
        obj_x= static_cast<int>((*ocb_ptr).object_list[object_count].Oworldx); 
        obj_y= static_cast<int>((*ocb_ptr).object_list[object_count].Oworldy);
//		ast_mass=(*ocb_ptr).object_list[it].shield_value/2000;
        if (ast_x<obj_x) (*ocb_ptr).object_list[object_count].extern_movedx-=6;
        else (*ocb_ptr).object_list[object_count].extern_movedx+=6;

        if (ast_y<obj_y) (*ocb_ptr).object_list[object_count].extern_movedy-=6;
        else (*ocb_ptr).object_list[object_count].extern_movedy+=6;
     }   
        
   }
   active_object_count++;
  } //end of if on
   object_count++;
 }	//next object - end of while
}


//void handle_dlp_exploding_object(int it)	  //control asteroids magnetism
//{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern int dynamic_object_index;
//int object_count,active_object_count;
////search for objects close by and if their mass is < 900 attract them towards us
//int ast_x,ast_y,obj_x,obj_y,distance_to;
//
//
////change 8/3/99 - if distance to zex>200000 then kill the asteroid
//
//ast_x=(*ocb_ptr).object_list[it].Oworldx;
//ast_y=(*ocb_ptr).object_list[it].Oworldy;
//
//
//if (get_distance_to_zex(it)>200000) add_to_kill_list(it);
//
// object_count=active_object_count=0;
// while (active_object_count<dynamic_object_index)
// {
//  if ((*ocb_ptr).object_list[object_count].in_use==1 && object_count!=it)
//  {
//    distance_to=get_distance_to_from(object_count,it);
//    if (distance_to<11000 && (*ocb_ptr).object_list[object_count].mass<1000) 
//      {
//        obj_x=(*ocb_ptr).object_list[object_count].Oworldx; 
//        obj_y=(*ocb_ptr).object_list[object_count].Oworldy;
////		  ast_mass=(*ocb_ptr).object_list[it].shield_value/2000;
//        if (ast_x<obj_x) (*ocb_ptr).object_list[object_count].extern_movedx-=4;
//        else (*ocb_ptr).object_list[object_count].extern_movedx+=4;
//
//        if (ast_y<obj_y) (*ocb_ptr).object_list[object_count].extern_movedy-=4;
//        else (*ocb_ptr).object_list[object_count].extern_movedy+=4;
//        
//        
//      }
//   active_object_count++;
//  } //end of if on
//   object_count++;
// }	//next object - end of while
//}
//
//


//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_ejection(int it)	//control ejection pod
{
extern	DynObjectsFixedSize *ocb_ptr;


(*ocb_ptr).object_list[it].control_use_1++;
 (*ocb_ptr).object_list[it].Orotdx+=1;
if ((*ocb_ptr).object_list[it].control_use_1>75)
 { 
  (*ocb_ptr).object_list[it].Omovedx-=30;
//   (*ocb_ptr).object_list[it].thrust_x=-30;
     if ((*ocb_ptr).object_list[it].control_use_2==0)
     {
      play_zsound_relative_to_zex(it, zip);
      (*ocb_ptr).object_list[it].control_use_2=1;	//set lock
     }	//only play the sound once
 }


if ((*ocb_ptr).object_list[it].control_use_1>160)
 { 
 kill(it);
 }
}

//DOOR STAYS OPEN for 30 secs then shuts
//cu1=frame counter
//cu2=doing_open
//cu3=1=open
//cu4=close timer
//cu7=open now (if 1)
extern int real_frame_counter;

void handle_door_vertical(int it)	//control vertical door
{
extern	DynObjectsFixedSize *ocb_ptr;

if (global_open_doors==0) 	//cant open doors
{
    (*ocb_ptr).object_list[it].just_hit=0;	//registered
    return;
}

//check if doing close
if ((*ocb_ptr).object_list[it].control_use_2==2 && (*ocb_ptr).object_list[it].control_use_3!=0) //doing close and not fully close?
{

(*ocb_ptr).object_list[it].Oworldy+=30;	//close
(*ocb_ptr).object_list[it].control_use_1++;
if ((*ocb_ptr).object_list[it].Oworldy>(*ocb_ptr).object_list[it].Ostarty) 
 {
 (*ocb_ptr).object_list[it].Oworldy=(*ocb_ptr).object_list[it].Ostarty;
 (*ocb_ptr).object_list[it].control_use_2=0;	//not doing open
 (*ocb_ptr).object_list[it].control_use_3=0;	//open 
 }
    (*ocb_ptr).object_list[it].just_hit=0;	//registered

return;
}

//check if doing open
if ((*ocb_ptr).object_list[it].control_use_2==1 && (*ocb_ptr).object_list[it].control_use_3!=1) //doing open and not fully open?
{

(*ocb_ptr).object_list[it].Oworldy-=30;
(*ocb_ptr).object_list[it].control_use_1++;
if ((*ocb_ptr).object_list[it].Oworldy<(*ocb_ptr).object_list[it].Ostarty-8000) 
 {
 (*ocb_ptr).object_list[it].control_use_2=0;	//not doing open
 (*ocb_ptr).object_list[it].control_use_3=1;	//open
 (*ocb_ptr).object_list[it].control_use_4=real_frame_counter+(38*5);	//open timer
 
 }
    (*ocb_ptr).object_list[it].just_hit=0;	//registered
return;
}


//check if time to close
if ( (*ocb_ptr).object_list[it].control_use_3==1)	//if open
{
if (real_frame_counter > (*ocb_ptr).object_list[it].control_use_4)	//time to close
   {
    (*ocb_ptr).object_list[it].control_use_2=2;	//doing close
   (*ocb_ptr).object_list[it].control_use_1=0;	//reset frame counter
   }
  
}	//open



//see if just shot
if ((*ocb_ptr).object_list[it].just_hit>0) 	//door just shot
{
 int slot_that_hit;
 slot_that_hit=(*ocb_ptr).object_list[it].hit_by_slot;
 if (slot_that_hit!=-1)
  {
  if ((*ocb_ptr).object_list[slot_that_hit].Oid=='MIS1' || (*ocb_ptr).object_list[slot_that_hit].Oid=='MIS2' || (*ocb_ptr).object_list[slot_that_hit].Oid=='MIS3')

     (*ocb_ptr).object_list[it].control_use_7=1;
  }
}

if ((*ocb_ptr).object_list[it].control_use_7>0)	//door just shot
 {
   (*ocb_ptr).object_list[it].control_use_7=0;
    (*ocb_ptr).object_list[it].control_use_2=1;	//doing open
    (*ocb_ptr).object_list[it].just_hit=0;	//registered
   (*ocb_ptr).object_list[it].control_use_1=0;	//reset frame counter
 }



}




//DOOR STAYS OPEN for 30 secs then shuts
//cu1=frame counter
//cu2=doing_open
//cu3=1=open
//cu4=close timer
//cu7=open now (if 1)
void handle_door_vertical_switched(int it)	 //control vertical door - switch operated
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int real_frame_counter;

if (global_open_doors==0) 	//cant open doors
{
    (*ocb_ptr).object_list[it].just_hit=0;	//registered
    return;
}

//check if doing close
if ((*ocb_ptr).object_list[it].control_use_2==2 && (*ocb_ptr).object_list[it].control_use_3!=0) //doing close and not fully close?
{

(*ocb_ptr).object_list[it].Oworldy+=90;	//close
(*ocb_ptr).object_list[it].control_use_1++;
if ((*ocb_ptr).object_list[it].Oworldy>(*ocb_ptr).object_list[it].Ostarty) 
 {
  clear_cont_sound(it);	//stop tractor sound

 (*ocb_ptr).object_list[it].Oworldy=(*ocb_ptr).object_list[it].Ostarty;
 (*ocb_ptr).object_list[it].control_use_2=0;	//not doing open
 (*ocb_ptr).object_list[it].control_use_3=0;	//open 
 }
    (*ocb_ptr).object_list[it].just_hit=0;	//registered

return;
}

//check if doing open
if ((*ocb_ptr).object_list[it].control_use_2==1 && (*ocb_ptr).object_list[it].control_use_3!=1) //doing open and not fully open?
{

(*ocb_ptr).object_list[it].Oworldy-=130;
(*ocb_ptr).object_list[it].control_use_1++;
if ((*ocb_ptr).object_list[it].Oworldy<(*ocb_ptr).object_list[it].Ostarty-8000) 
 {
  clear_cont_sound(it);	//stop tractor sound

 (*ocb_ptr).object_list[it].control_use_2=0;	//not doing open
 (*ocb_ptr).object_list[it].control_use_3=1;	//open
// (*ocb_ptr).object_list[it].control_use_4=real_frame_counter+(38*3);	  //open timer
 
 }
    (*ocb_ptr).object_list[it].just_hit=0;	//registered
return;
}


//check if time to close
if ( (*ocb_ptr).object_list[it].control_use_3==1)	//if open
{
if (real_frame_counter > (*ocb_ptr).object_list[it].control_use_4)	//time to close
   {
    insert_continuous_sound(it, rumble_loop,53);
    (*ocb_ptr).object_list[it].control_use_3=3;	//doing close
    (*ocb_ptr).object_list[it].control_use_2=2;	//doing close
   (*ocb_ptr).object_list[it].control_use_1=0;	//reset frame counter
   }
  
}	//open



if ((*ocb_ptr).object_list[it].control_use_7>0 && (*ocb_ptr).object_list[it].control_use_3!=1)	//door just shot
 {
    insert_continuous_sound(it, rumble_loop,56);

   (*ocb_ptr).object_list[it].control_use_7=0;
    (*ocb_ptr).object_list[it].control_use_2=1;	//doing open
    (*ocb_ptr).object_list[it].just_hit=0;	//registered
   (*ocb_ptr).object_list[it].control_use_1=0;	//reset frame counter
 }



}


void handle_door_switch(int it)
{
if ((*ocb_ptr).object_list[it].just_hit>0) 	//door just shot
 {
 int the_obj;
 
 (*ocb_ptr).object_list[it].just_hit=0;
 
 //find the object with the unique ID in cu1 and set it's cu7
 the_obj=find_unique_id((*ocb_ptr).object_list[it].control_use_1);
 if (the_obj!=-1)
     {
     (*ocb_ptr).object_list[the_obj].control_use_7=1;
     (*ocb_ptr).object_list[the_obj].control_use_4=real_frame_counter+(28*(*ocb_ptr).object_list[it].control_use_2);	//time to stay open
     }
 }
}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_missile1(int it)	//control missile1
{
extern	DynObjectsFixedSize *ocb_ptr;
int aimed_at,wangle,zangle,d1,d2;
//float min_distance;


(*ocb_ptr).object_list[it].control_use_1--;	//dec frame count


if ((*ocb_ptr).object_list[it].control_use_1<1 || (*ocb_ptr).object_list[it].shield_value<0)
 { 
 add_to_explode_list(it,5);
 add_to_kill_list (it);	//if run out of time, explode it
 //mplay_zsound(short_explosion_with_echo, sound_high_pri, twin1_chan, sound_vol_7);
 play_zsound_relative_to_zex(it,missile_explode);

 return;
 }
else
 { 
//turn missile to target
//see if we have a target
if ((*ocb_ptr).object_list[it].control_use_2==-1)	//who the missile is fired at - nothing!
{
if (find_something_to_attack(it,1,96000)==0)
(*ocb_ptr).object_list[it].control_use_2=(*ocb_ptr).object_list[it].attack_what;
}


//first check if target is alive and we haven't lost lock
    aimed_at=(*ocb_ptr).object_list[it].control_use_2;	//who the missile is fired at
    if (aimed_at!=-1 && (*ocb_ptr).object_list[aimed_at].in_use==1 && (*ocb_ptr).object_list[it].control_use_3==0)
    {
     (*ocb_ptr).object_list[it].control_use_10=1;	//we have found a target

        wangle=get_angle_to_from( static_cast<int>((*ocb_ptr).object_list[aimed_at].Oworldx),
								  static_cast<int>((*ocb_ptr).object_list[aimed_at].Oworldy),
								 static_cast<int>((*ocb_ptr).object_list[it].Oworldx),
								  static_cast<int>((*ocb_ptr).object_list[it].Oworldy));
//

        zangle=(*ocb_ptr).object_list[it].Orotz;
//   wangle=(*ocb_ptr).object_list[it].wanted_velocity;
   
        if (wangle!=zangle)
        {

        d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
        d2=get_diff(wangle,zangle);
        if (d1>6 || d2>6)
          {
           if (d1>d2) zangle+=6;
            else
            zangle-=6;
           if (zangle>359) zangle-=360;
           if (zangle<0) zangle+=360;
          }
        (*ocb_ptr).object_list[it].Orotz=zangle;
        }  //end of wangle!=zangle
//now calc thrust components of missile
        (*ocb_ptr).object_list[it].Omovedy=
        (*ocb_ptr).object_list[it].object_normal_translated.y*700;	//current velocity
        (*ocb_ptr).object_list[it].Omovedx=
        (*ocb_ptr).object_list[it].object_normal_translated.x*700;
      }	//end oftarget is alive
      
      else	//target is not alive so explode missile
      {
 //now calc thrust components of missile
        (*ocb_ptr).object_list[it].Omovedy=
        (*ocb_ptr).object_list[it].object_normal_translated.y*700;	//current velocity
        (*ocb_ptr).object_list[it].Omovedx=
        (*ocb_ptr).object_list[it].object_normal_translated.x*700;

      if ((*ocb_ptr).object_list[it].control_use_10==1)
       {
       add_to_explode_list(it,5);
       add_to_kill_list (it);	  //if run out of time, explode it
       play_zsound_relative_to_zex(it,missile_explode);
  
       }
     }
 }

}




//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_not_so_weeny_bullet(int it)	//lets the bullet live for 50 frames
{
extern	DynObjectsFixedSize *ocb_ptr;
int attacking_this;
//int zangle,wangle,d1,d2;


(*ocb_ptr).object_list[it].control_use_1++;
if ((*ocb_ptr).object_list[it].control_use_1>120)
 { 
 kill(it);
 }
else
 { 
// if (find_something_to_attack(it)==-1) return;	//didn't find anything
// if ((*ocb_ptr).object_list[it].Oworldx>1000000) Debugger();
// if ((*ocb_ptr).object_list[it].Oworldx<-1000000) Debugger();
 
// attacking_this=(*ocb_ptr).object_list[it].attack_what;

// if (RangedRdm(1,10)>7) ram_object(attacking_this,it,2);
 
//      (*ocb_ptr).object_list[it].thrust_y= (*ocb_ptr).object_list[it].Omovedy;	//update thrust y
//       (*ocb_ptr).object_list[it].thrust_x=(*ocb_ptr).object_list[it].Omovedx;
 }
}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
//these are cool. When their shields get down to below 50 they release between 2 and 6
//easy monsters from x+15000, y.
//control_use_2 is the coun for this
void handle_gun1(int it)	//static gun cointroller
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int monsters_can_fire;

int zex_x,zex_y,gun_x,gun_y,wangle,zangle;
int attacking_this;
int d1,d2,dest_ocb,static_slot;
load_dyn_object_pb the_params;
//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,0,20000)==-1) return;	//didn't find anything
attacking_this=(*ocb_ptr).object_list[it].attack_what;

(*ocb_ptr).object_list[it].control_use_1--;

//check shields
if ((*ocb_ptr).object_list[it].shield_value<50 && (*ocb_ptr).object_list[it].control_use_3==0)	 //trigger monsters
{
 (*ocb_ptr).object_list[it].control_use_3=1;
 (*ocb_ptr).object_list[it].control_use_2=RangedRdm(0,2);
}


if ((*ocb_ptr).object_list[it].control_use_2>0)
{
(*ocb_ptr).object_list[it].control_use_2--;	//dec gen monster count
//gen monster

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
  {

        clear_pb(&the_params);
        
	the_params.world_x=(*ocb_ptr).object_list[it].Oworldx+16000+(2000*(*ocb_ptr).object_list[it].control_use_2);
	the_params.world_y=(*ocb_ptr).object_list[it].Oworldy+(2000*(*ocb_ptr).object_list[it].control_use_2);	
	the_params.world_z=MONSTER_Z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
//	the_params.rot_z=0;
//	if (RangedRdm(1,10)>5)	//select either controller 1 or 2
//	the_params.controller_ref=monster1;
//	else
	the_params.controller_ref=monster2;
	
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
  //   the_params.has_roll=0;
	the_params.shield_value=370;
	the_params.mass=100;
//		 	 the_params.dq=0;
	the_params.no_trivial_rejection=1;
	    the_params.object_category=baddie_object;
     static_slot=get_static_object_slot('MON2');	//go fetch fido (the object)
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
   }
}

//rotate to face zex
zex_x= static_cast<int>((*ocb_ptr).object_list[attacking_this].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[attacking_this].Oworldy);
gun_x= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
gun_y= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);
wangle=get_angle_to_from (zex_x, zex_y, gun_x, gun_y);

zangle=(*ocb_ptr).object_list[it].Orotz;

//rotate to wanted angle
   if (wangle!=zangle)
   {

   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) zangle+=1;
   else
   zangle-=1;
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   (*ocb_ptr).object_list[it].control_use_4=zangle;
   }
   else	//we are not turning so can fire
   {
    if ((*ocb_ptr).object_list[it].control_use_1<0)
     {
       (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,30);	//set time to fire next cannon
       if(get_distance_to_from(attacking_this,it)<14000 )
        { 
         //first, start off animation
         (*ocb_ptr).object_list[it].anim_state=1;
	     play_zsound_relative_to_zex(it,short_explosion);
         if (monsters_can_fire) fire_weeny_cannon(it);
        }
     }
   }
}


//040699 SB
void handle_static_laser1(int it)	 //static laser cointroller
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int monsters_can_fire;

int zex_x,zex_y,gun_x,gun_y,wangle,zangle;
int attacking_this;
int d1,d2,dest_ocb,static_slot;
load_dyn_object_pb the_params;
//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,0,static_cast<int>(20000*LASER_RANGE_MULTIPLIER))==-1) return;	//didn't find anything
attacking_this=(*ocb_ptr).object_list[it].attack_what;

(*ocb_ptr).object_list[it].control_use_1--;

//check shields
if ((*ocb_ptr).object_list[it].shield_value<50 && (*ocb_ptr).object_list[it].control_use_3==0)	 //trigger monsters
{
 (*ocb_ptr).object_list[it].control_use_3=1;
 (*ocb_ptr).object_list[it].control_use_2=RangedRdm(0,2);
}


if ((*ocb_ptr).object_list[it].control_use_2>0)
{
(*ocb_ptr).object_list[it].control_use_2--;	//dec gen monster count
//gen monster

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
  {
       clear_pb(&the_params);
       
	the_params.world_x=(*ocb_ptr).object_list[it].Oworldx+16000+(2000*(*ocb_ptr).object_list[it].control_use_2);
	the_params.world_y=(*ocb_ptr).object_list[it].Oworldy+(2000*(*ocb_ptr).object_list[it].control_use_2);	
	the_params.world_z=MONSTER_Z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
//	the_params.rot_z=0;
//	if (RangedRdm(1,10)>5)	//select either controller 1 or 2
//	the_params.controller_ref=monster1;
//	else
	the_params.controller_ref=monster2;
	
//	the_params.control_use_1=0;
//	the_params.control_use_2=0;
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.current_velocity=0;
//	the_params.control_use_7=0;
//	the_params.wanted_velocity=0;
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
  //   the_params.has_roll=0;
	the_params.shield_value=370;
	the_params.mass=100;
//		 	 the_params.dq=0;
	the_params.no_trivial_rejection=1;
	    the_params.object_category=baddie_object;
     static_slot=get_static_object_slot('MON2');	//go fetch fido (the object)
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
   }
}

//rotate to face zex
zex_x= static_cast<int>((*ocb_ptr).object_list[attacking_this].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[attacking_this].Oworldy);
gun_x= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
gun_y= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);
wangle=get_angle_to_from (zex_x, zex_y, gun_x, gun_y);

zangle=(*ocb_ptr).object_list[it].Orotz;

//rotate to wanted angle
   if (wangle!=zangle)
   {

   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) zangle+=1;
   else
   zangle-=1;
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   (*ocb_ptr).object_list[it].control_use_4=zangle;
   }
   else	//we are not turning so can fire
   {
    if ((*ocb_ptr).object_list[it].control_use_1<0)
     {
       (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,30);	//set time to fire next cannon
         //first, start off animation
         (*ocb_ptr).object_list[it].anim_state=1;
//	play_zsound_relative_to_zex(it,short_explosion, sound_low_pri, sound_vol_3);
         if (monsters_can_fire) fire_laser_at(attacking_this,it,1000);
     }
   }
}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_burning_particle(int it)	//lets the shard live for a few frames and changes it's colour
{
extern	DynObjectsFixedSize *ocb_ptr;
_ZPoly *the_polys;
signed long temp_colour;

(*ocb_ptr).object_list[it].control_use_1--;	//explode sets this to a count
if ((*ocb_ptr).object_list[it].control_use_1<1)
 { 
 kill(it);
 }
else
{
//if not time to kill, is this the first time we've been run?
//if so, set the colour to white
    if ((*ocb_ptr).object_list[it].control_use_1<15)
    {
    the_polys=(*ocb_ptr).object_list[it].polys_ptr;
//alter laser colour
//we need to get blue down to zero
    temp_colour=(*the_polys).Prgb.blue;
    temp_colour-=9000;
    if (temp_colour<0) temp_colour=0;
    (*the_polys).Prgb.blue=temp_colour;

    temp_colour=(*the_polys).Prgb.green;
    temp_colour-=4000;
    if (temp_colour<0) temp_colour=0;
    (*the_polys).Prgb.green=temp_colour;

    temp_colour=(*the_polys).Prgb.red;
    temp_colour-=2000;
    if (temp_colour<0) temp_colour=0;
    (*the_polys).Prgb.red=temp_colour;
    }
}
}


//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
void handle_explode(int it)	//lets the shard live for a few frames
{
extern	DynObjectsFixedSize *ocb_ptr;


(*ocb_ptr).object_list[it].control_use_1--;	//explode sets this to a count
if ((*ocb_ptr).object_list[it].control_use_1<1)
 { 
 kill(it);
 }
}



//еееееееееееееееееееееееееееееееееееееееееееееее
void handle_dust(int it)	//check if too far behind (or infront!) and regen if so
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int thrust,number_of_stars;
extern _3D camera_pos;

int dust_distance,camera_z;
int diff_angle,angle_to_dust,behind;
float	zex_x,zex_y,it_x,it_y, it_z;

//see if too close to camera z, kill if so - 0.94
it_z=(*ocb_ptr).object_list[it].Oworldz;
if (it_z<camera_pos.z)
 {
  kill(it);
   if (number_of_stars>0) number_of_stars--;
  return;
 }
//if too far away in z kill it
if (camera_pos.z-it_z<-15000)
 {
  kill(it);
   if (number_of_stars>0) number_of_stars--;
  return;

 }
//find out if dust is behind us
behind=0;	//defaault in front

zex_x=(*ocb_ptr).object_list[zex].Oworldx;
zex_y=(*ocb_ptr).object_list[zex].Oworldy;

it_x=(*ocb_ptr).object_list[it].Oworldx;
it_y=(*ocb_ptr).object_list[it].Oworldy;

angle_to_dust=get_angle_to_from ((int)it_x, (int)it_y, (int)zex_x, (int)zex_y);

diff_angle= get_diff((*ocb_ptr).object_list[zex].Orotz, angle_to_dust);

if (diff_angle>270 || diff_angle<90) behind=0;
else
behind=1;

if (thrust<0) behind=!behind;	//takes care of that little problem


//get distance from ship
 dust_distance=get_distance_to_zex(it);

  camera_z=abs_nobranch( static_cast<int>(camera_pos.z) );
  camera_z*=4;

if (behind==1)
 {
 if (dust_distance>camera_z)
  {
   kill(it);	//stars are gen'd as a function of frame rate
   if (number_of_stars>0) number_of_stars--;
  }
 }

else	//not behind - d12
 {
 if (dust_distance>camera_z)
  {
   kill(it);	//stars are gen'd as a function of frame rate
   if (number_of_stars>0) number_of_stars--;
  }
 }


}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
void check_collision(int current_object)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int	dynamic_object_index;	//how many dynamic objects to look at

float temp_x,temp_y,temp_z;
float distance,min_distance;

int i,number_to_examine,compare_object,in;
register ZObject * current_object_ptr;
register ZObject * compare_object_ptr;
    
//ееееееееее
    current_object_ptr=&ocb_ptr->object_list[current_object];

if (current_object_ptr->invincible_count>0) return;

number_to_examine=dynamic_object_index;	//we copy this in case we kill any objects
compare_object=0;
in=1;



if (current_object_ptr->colltype==3)	//sphere and box

{
for (i=0;i<number_to_examine;i++)
 {
	while((*ocb_ptr).object_list[compare_object].in_use==0) //search for !empty objects
    {
	compare_object++;	//empty object
    if (compare_object>MAX_NUMBER_OF_DYN_OBJECTS-1) return;	//oops check
    }
    
    compare_object_ptr=&ocb_ptr->object_list[compare_object];

//dont compare object to itself!
    //change 27/12/98 - if the objects are the same then they don't collide
 if (compare_object_ptr->crashable==yes && compare_object_ptr->invincible_count<=0 && compare_object!=current_object )
 
//     && (*ocb_ptr).object_list[current_object].object_category != (*ocb_ptr).object_list[compare_object].object_category)
 {
//now get distance between objects and if less than the two radii added, it's a crash
//Get AB
	temp_x=compare_object_ptr->Oworldx-current_object_ptr->Oworldx;
	temp_y=compare_object_ptr->Oworldy-current_object_ptr->Oworldy;
	temp_z=compare_object_ptr->Oworldz-current_object_ptr->Oworldz;
//get sqrt(x^2+y^2+z^2)
//	distance=sqrt(temp_x*temp_x+temp_y*temp_y+temp_z*temp_z);
	distance=(temp_x*temp_x+temp_y*temp_y+temp_z*temp_z);
	min_distance=current_object_ptr->radius + compare_object_ptr->radius;
	min_distance=min_distance*min_distance;
//if objects are in collision then crash it
//    if (distance < (*ocb_ptr).object_list[current_object].radius + (*ocb_ptr).object_list[compare_object].radius)
    if (distance < min_distance)

    {
    //check bounding boxes...(only if both objects are in view!!!!)
      if (current_object_ptr->is_visible==1 && compare_object_ptr->is_visible==1)
      {
      	if (check_bounding_box(current_object,compare_object))	//file
       	{
       	current_object_ptr->hit_by_slot=compare_object;
       	current_object_ptr->just_hit=20;
       	}
      }	//end of in-view check
      
    }	//end of radius check
  }
  compare_object++;	//next obj
 }	///end of full monty check
 return;
}

else	//not sphere and box, try for sphere
if (current_object_ptr->colltype==1)	//sphere only

{
for (i=0;i<number_to_examine;i++)
 {
	while((*ocb_ptr).object_list[compare_object].in_use==0) //search for !empty objects
	compare_object++;	//empty object

    compare_object_ptr=&ocb_ptr->object_list[compare_object];
//dont compare object to itself!
    //change 27/12/98 - if the objects are the same then they don't collide
 if (compare_object_ptr->crashable==yes && compare_object_ptr->invincible_count<=0 && compare_object!=current_object )
//     && (*ocb_ptr).object_list[current_object].object_category != (*ocb_ptr).object_list[compare_object].object_category)
 {
 //check if the other object is box type, if so we MUST DO box coll
 if (compare_object_ptr->colltype==2)	     //box only
    {
       //debug
//   if (current_object_ptr->controller==persistent_asteroid && (*ocb_ptr).object_list[compare_object].Oid=='DRV8')
//   {
//   int a;
//   a++;
//   }
   //end debug

         if (check_bounding_box(current_object,compare_object))	//file
       	{
       	current_object_ptr->hit_by_slot=compare_object;
       	current_object_ptr->just_hit=20;

       	}
    }
    else	//compare obj is not box type
    {
//now get distance between objects and if less than the two radii added, it's a crash
//Get AB
	temp_x=compare_object_ptr->Oworldx-current_object_ptr->Oworldx;
	temp_y=compare_object_ptr->Oworldy-current_object_ptr->Oworldy;
	temp_z=compare_object_ptr->Oworldz-current_object_ptr->Oworldz;
//get sqrt(x^2+y^2+z^2)
//	distance=sqrt(temp_x*temp_x+temp_y*temp_y+temp_z*temp_z);
	distance=(temp_x*temp_x+temp_y*temp_y+temp_z*temp_z);
	min_distance=current_object_ptr->radius + compare_object_ptr->radius;
	min_distance=min_distance*min_distance;
//if objects are in collision then crash it
//    if (distance < (*ocb_ptr).object_list[current_object].radius + (*ocb_ptr).object_list[compare_object].radius)
    if (distance < min_distance)

    {
      OSType co_type; 
      OSType cu_type; 

	//first make sure we are not checking components of a mothership
    	co_type = compare_object_ptr->Oid;
    	cu_type = current_object_ptr->Oid;
		co_type&=0xffff0000; cu_type&=0xffff0000;
		co_type|=0x2020; cu_type|=0x2020;

	if (co_type=='MS  ' && cu_type=='MS  ')
        {
        }
        else	//not two components of a mothership
        {
       	current_object_ptr->hit_by_slot=compare_object;
       	current_object_ptr->just_hit=20;
        }
    }	//end of radius check
    
   }	//end of compare is not box type
   
  }
  compare_object++;	//next obj
 }	
 return;
} ///end of sphere only check



else
if (current_object_ptr->colltype==2)	//box only

{
for (i=0;i<number_to_examine;i++)
 {
	while((*ocb_ptr).object_list[compare_object].in_use==0) //search for !empty objects
	compare_object++;	//empty object

//dont compare object to itself!
////debug code
//if ((*ocb_ptr).object_list[current_object].Oid=='DRV8'
//&& (*ocb_ptr).object_list[compare_object].Oid=='MIS1')
//{
//int z;
// z++;
//}
////end debug
   compare_object_ptr=&ocb_ptr->object_list[compare_object];

    //change 27/12/98 - if the objects are the same then they don't collide
  if (compare_object_ptr->crashable==yes && compare_object_ptr->invincible_count<=0 && compare_object!=current_object )
 //     && (*ocb_ptr).object_list[current_object].object_category != (*ocb_ptr).object_list[compare_object].object_category)
  {
       if (current_object_ptr->is_visible==1 && compare_object_ptr->is_visible==1)
       {
       	if (check_bounding_box(current_object,compare_object))	//file
        	{
        	current_object_ptr->hit_by_slot=compare_object;
        	current_object_ptr->just_hit=20;
        	}
        	else
        	{
       	if (check_bounding_box(compare_object,current_object))	//file
        	 {
        	 current_object_ptr->hit_by_slot=compare_object;
        	current_object_ptr->just_hit=20;
        	 }
        	}
       }
   }
  compare_object++;	//next obj
 }	///end of box only
 return;
}
 
 
 

}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 18 Jan 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----


#if CALLSIGN_DEBUG_MODE

static bool callsign_is_bad(unsigned long callsign)
{
	if(callsign == 0) return false; // might not have a callsign...
	
	char s[5];
	safe_number_id_to_4_character_string(callsign, s);
	for(int i=0; i<4; i++)
	{
		if(s[i]=='?')
		{
			return true;
		}
	}
	return false;
}

#endif
// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 18 Jan 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
#if CALLSIGN_DEBUG_MODE

#include "Error_handler.h"

void parse_ocb_for_bad_callsigns()
{
	int max_objects = MAX_NUMBER_OF_DYN_OBJECTS;
	int bad_callsigns = 0;
	static bool written_once = false;
	
	if(written_once) { return; }
	
	for(int i=0; i < max_objects; i++)
	{
		if((*ocb_ptr).object_list[i].in_use)
		{
			if(callsign_is_bad(ocb_ptr->object_list[i].callsign)) 
			{ 
				bad_callsigns++;
			}
			if((*ocb_ptr).object_list[i].mship_ptr)
			{
				if(callsign_is_bad((*ocb_ptr).object_list[i].mship_ptr->callsign))
				{
					bad_callsigns++;
				}
			}
		}
	}
	
	if(bad_callsigns)
	{
		write_callsigns();
		report_error_id_non_fatal("I've written bad callsigns to callsigns.txt", bad_callsigns);
		written_once = true;
	}
}


#endif

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 Jan 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

#if CALLSIGN_DEBUG_MODE
void write_callsigns()
{
	FILE* f = fopen("callsigns.txt","w");
	int max_objects = MAX_NUMBER_OF_DYN_OBJECTS;
	
	extern int dynamic_object_index;
	fprintf(f, "Active objects = %i out of %i possible\n\n", dynamic_object_index, MAX_NUMBER_OF_DYN_OBJECTS);
	
	if(f)
	{
		for(int i=0; i < max_objects; i++)
		{
			if((*ocb_ptr).object_list[i].in_use == 0)
			{
				//fprintf(f, "%i [NOT IN USE]\n", i);
			}
			else
			{
				unsigned long this_callsign = ocb_ptr->object_list[i].callsign;
				char id[5]; id[4]=0;
				safe_number_id_to_4_character_string(ocb_ptr->object_list[i].Oid, id);
				fprintf(f, "%i (object = %s) = %lx", i, id, this_callsign);

				if(callsign_is_bad(this_callsign)) 
				{ 
					fprintf(f, " - BAD CALLSIGN!"); 
				}
				
				char s[5]; s[4]=0;
				safe_number_id_to_4_character_string(this_callsign, s);
				fprintf(f, " %s",s);
								
				if((*ocb_ptr).object_list[i].mship_ptr)
				{
					char s[5]; s[4]=0;
					safe_number_id_to_4_character_string((*ocb_ptr).object_list[i].mship_ptr->callsign, s);
					fprintf(f," - Mothership callsign = %x %s", (*ocb_ptr).object_list[i].mship_ptr->callsign, s);
					if(callsign_is_bad((*ocb_ptr).object_list[i].mship_ptr->callsign))
					{
						fprintf(f, " - BAD CALLSIGN!"); 
					}
					
				}
				fprintf(f, "\n");
			}
		}
		fclose(f);
	}
}
#endif



// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 Jan 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

#if CALLSIGN_DEBUG_MODE

void robs_callsign_hack()
{
	// rob's debug code...
	for(int i=0; i<MAX_NUMBER_OF_DYN_OBJECTS; i++)
	{
		if(i&1)
			(*ocb_ptr).object_list[i].callsign = 9876; // 'LOVE';
		else
			(*ocb_ptr).object_list[i].callsign = 9875; // 'HATE';
	}
	
	extern	StatObjectsFixedSize *ocb_Sptr;
	for(int j=0; j<MAX_NUMBER_OF_STAT_OBJECTS; j++)
	{
		if(j&1)
			(*ocb_Sptr).object_list[j].callsign = 1234; // 'ZOOM';
		else
			(*ocb_Sptr).object_list[j].callsign = 1235; // 'ZIPP';
	}
}
#endif


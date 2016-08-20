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

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

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

//object_control_misc_routines

extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength,credits,score;
//ееееееее















//еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее


extern	DynObjectsFixedSize *ocb_ptr;

//used for any object to damage shields and check for death
//returns 1 if object killed
int damage_object (int current_object, int shield_damage)
{
extern int flash_screen,planet_on_screen;
int hit_by_what,who_fired,explosion_class;
int return_val=0;


//modify for Zex's shield strength
     if (current_object==zex)
     {
      int a;
      a=ship_items[KSHIELDS].modifier-1;	//1,2,3,4,5
      shield_damage-=a*6;
      if (shield_damage<1) shield_damage=1;
     }
     
     (*ocb_ptr).object_list[current_object].shield_value-=shield_damage;

     if ((*ocb_ptr).object_list[current_object].shield_value<0 && 
     (*ocb_ptr).object_list[current_object].Oid!='LAS1')	//we dont explode lasers!
       {
       int temp,id;
       
       return_val=1;
//change 070699
//randomly we sometimes send these to tumble rather than immediately destroyed
         id=(*ocb_ptr).object_list[current_object].Oid;
         if (RangedRdm(0,10)>5)
         {
            if (id!='MIS1' && id!='MIS2' && id!='MIS3' && id!='IODA')	//we do not tumble bullets OR iodevices!
             {
              add_to_score_and_credits(current_object);
              tumble_object(current_object);
              flash_screen=2;
              return 1;
             }
         }
         //calc explosion effect
//         if ((*ocb_ptr).object_list[current_object].object_category==baddie_object)	  //score and credits
//          add_to_score_and_credits(current_object);

         switch (id)
         {
          case 'ZZ1A': case 'ZZ1B' : case 'ZZ1C': case 'WARP': case 'MON6': case 'IODA':
          {
          explosion_class=5;
          }


          case 'MON2': case 'MON3':
          {
          explosion_class=3;
          }
          
          default:
          {
          Byte *bp;
          int temp_id=id;
		  temp_id&=0xffff0000;
		  temp_id|=0x2020;
		  if (temp_id=='IO  ') explosion_class=5;
		  else
		  if (temp_id=='MN  ') explosion_class=4;
		  else
		  explosion_class=0;
		  /*
             bp = (Byte *)&id;
          //see if ionic device
          if (bp[0]=='I' && bp[1]=='O') explosion_class=5;
          else
          if (bp[0]=='M' && bp[1]=='N') explosion_class=4;
          else
          explosion_class=0;
          */
		  }
         }
         
         add_to_explode_list(current_object,explosion_class);


        
  //if the thing that destroyed us was fired by zex then score etc.
         if ((*ocb_ptr).object_list[current_object].callsign!=0) 
         {
          do_callsign_text("     DESTROYED!",current_object,2);	//callsign goes in first 4 chars of string
          
//          if (score>1000) score-=1000;
//	else score=0;

          }
          
          
		temp=(*ocb_ptr).object_list[current_object].hit_by_slot; //what object hit this object
		if (temp!=-1 && ((*ocb_ptr).object_list[temp].object_category==zex_object))	//was the thing that hit this object a zex object
		{ 
			add_to_score_and_credits(current_object); 
		}
		add_to_kill_list (current_object);
          
		if ((*ocb_ptr).object_list[current_object].Oid=='IODA')
		{
          play_zsound_relative_to_zex(current_object, explosion4);
          play_zsound_relative_to_zex(current_object, screech);
		}
		else          
		{
          play_zsound_relative_to_zex(current_object, short_explosion);
		}
		//find out if it was a wingman that killed object and issue message if so
		hit_by_what=(*ocb_ptr).object_list[current_object].hit_by_slot;
		if (hit_by_what!=-1)	//180699 - fix crash bug
		{
           who_fired=(*ocb_ptr).object_list[hit_by_what].who_spawned_this_object;
           if (who_fired!=-1)
            {
             if ((*ocb_ptr).object_list[who_fired].controller==WINGMAN_FOR_GAME) wingman_diealien_text(who_fired);
            }
		}
	}

return return_val;

}


void GenZexShield()
{
int slot;
float shot_x,shot_y,shot_z;
load_dyn_object_pb the_params;


if ((*ocb_ptr).object_list[zex].showing_shield==0)
 {
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
        clear_pb(&the_params);
        
	(*ocb_ptr).object_list[zex].showing_shield=1;
	
	shot_x=(*ocb_ptr).object_list[zex].Oworldx;
	shot_y=(*ocb_ptr).object_list[zex].Oworldy;
	shot_z=(*ocb_ptr).object_list[zex].Oworldz-100;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
	the_params.rot_x=(*ocb_ptr).object_list[zex].Orotx;
	the_params.rot_y=(*ocb_ptr).object_list[zex].Oroty;
	   
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	the_params.rot_z=0;
	the_params.controller_ref=zex_shield;	//controller
	the_params.control_use_1=20;	//frames to live counter
//	the_params.control_use_2=0;	//use to control deceleration.
//	the_params.control_use_3=0;
//     the_params.has_roll=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.control_use_7=0;	//set owner in cu7 so shield when killed can reset showing_shield
//	the_params.attack_mode=0;
//	the_params.shield_value=0;
	the_params.mass=20;
  //      the_params.dq=0;
	the_params.object_category=(*ocb_ptr).object_list[zex].object_category;
	the_params.no_trivial_rejection=1;
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
        the_params.who_spawned_this_object=0;
//	the_params.unique_id=0;
	load_dyn_object(zex_shield,slot,&the_params,-1);	//the static object, the position (-1=next free)
	}

 }	//end of not showing shield
}


void gen_shockwave_object(int distance, int damage, int damage_happens_distance)
{
	int slot;
	float shot_x,shot_y,shot_z;
	load_dyn_object_pb the_params;
	
	slot=find_vacant_dynamic_object_slot();
	if (slot!=-1)
	{
		clear_pb(&the_params);
		
		//(*ocb_ptr).object_list[zex].showing_shield=1;
		
		shot_x=(*ocb_ptr).object_list[zex].Oworldx;
		shot_y=(*ocb_ptr).object_list[zex].Oworldy;
		shot_z=(*ocb_ptr).object_list[zex].Oworldz-100;
		
		the_params.world_x=shot_x;
		the_params.world_y=shot_y;
		the_params.world_z=shot_z;
		//	the_params.Omovedx=0;
		//	the_params.Omovedy=0;
		//	the_params.Omovedz=0;
		//	the_params.delta_rot_x=0;
		//	the_params.delta_rot_y=0;
		//	the_params.delta_rot_z=0;
		//the_params.rot_x=(*ocb_ptr).object_list[zex].Orotx;
		//the_params.rot_y=(*ocb_ptr).object_list[zex].Oroty;
		if(RangedRdm(0,20) < 10)
		{
			the_params.rot_x=40;
		}
		
		//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
		//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
		//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
		//	the_params.rot_z=0;
		the_params.controller_ref=SHOCKWAVE_CONTROLLER;	//controller
		the_params.control_use_1=0;		//frames to live counter
		the_params.control_use_2=distance;	//use to control deceleration.
		the_params.control_use_3=damage;
										//     the_params.has_roll=0;
		the_params.control_use_4=damage_happens_distance;
		the_params.control_use_7=0;
										//	the_params.control_use_7=0;
										//	the_params.current_velocity=0;
										//	the_params.wanted_velocity=0;
										//	the_params.control_use_7=0;	//set owner in cu7 so shield when killed can reset showing_shield
										//	the_params.attack_mode=0;
										//	the_params.shield_value=0;
		the_params.mass=20;
		//      the_params.dq=0;
		the_params.object_category=2; // scenary object now ... was (*ocb_ptr).object_list[zex].object_category;
		the_params.no_trivial_rejection=1;
		//	the_params.laser_cat=0;
		//	the_params.cannon_cat=0;
		//	the_params.missile_cat=0;
		//	the_params.number_of_missiles=0;
		the_params.who_spawned_this_object=0;
		//	the_params.unique_id=0;
		load_dyn_object(31,slot,&the_params,-1);	//the static object, the position (-1=next free)
	}
	
}





//this routine takes an object and changes its controller to
//tumbling_to_explode_object which will ultimately destroy it
//in the meantime however it is assigned a tumble direction and speed
//and sent tumbling off into space
void tumble_object(int the_object)
{
int temp,delta_x,delta_y,hit_by;
add_to_explode_list(the_object,5);
hit_by=(*ocb_ptr).object_list[the_object].hit_by_slot;
if (hit_by!=-1)
 {
    delta_x= static_cast<int>((*ocb_ptr).object_list[hit_by].Omovedx/5);
    delta_y= static_cast<int>((*ocb_ptr).object_list[hit_by].Omovedy/5);
 }
else
{
    temp=RangedRdm(0,80);
    if (RangedRdm(0,10)>5) temp=-temp;
    delta_x=temp;

    temp=RangedRdm(0,80);
    if (RangedRdm(0,10)>5) temp=-temp;
    delta_y=temp;

}


(*ocb_ptr).object_list[the_object].controller=tumbling_to_explode_object;
(*ocb_ptr).object_list[the_object].crashable=0;
(*ocb_ptr).object_list[the_object].control_use_1=30;	//frames to live
(*ocb_ptr).object_list[the_object].Omovedx=delta_x;
(*ocb_ptr).object_list[the_object].Omovedy=delta_y;
(*ocb_ptr).object_list[the_object].Omovedz=-60;
(*ocb_ptr).object_list[the_object].Orotdz=RangedRdm(30,60);
(*ocb_ptr).object_list[the_object].Orotdy=RangedRdm(10,40);

        play_zsound_relative_to_zex(the_object, short_explosion);

}

//find the object with the unique id
int find_unique_id(int the_id)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
extern int dynamic_object_index;

int i,active_object_count;

i=active_object_count=0;
while (active_object_count<dynamic_object_index  && i<MAX_NUMBER_OF_DYN_OBJECTS)
{
  if ((*ocb_ptr).object_list[i].in_use==1)
  {
   if ((*ocb_ptr).object_list[i].unique_id==the_id)
   {
     return i;
     break;
   }
   active_object_count++;
 }
 i++;
}
return -1;

}


//given a pointer to a pb, clear it
//saves loads of code!
void clear_pb(load_dyn_object_pb* the_pb)
{

 the_pb->Omovedx=the_pb->Omovedy=the_pb->Omovedz=0;
 the_pb->delta_rot_x=the_pb->delta_rot_y=the_pb->delta_rot_z=0;
 the_pb->rot_x=the_pb->rot_y=the_pb->rot_z=0;
 the_pb->controller_ref=0;
 
 the_pb->control_use_1=0;
 the_pb->control_use_2=0;
 the_pb->control_use_3=0;
 the_pb->control_use_4=0;
 the_pb->control_use_7=0;
 
 the_pb->normal.y=the_pb->normal.z=0;
 the_pb->normal.x=1;
 
 the_pb->going_to=-1;
 the_pb->aggressive=1;
 the_pb->obj_class=0;
 the_pb->sub_controller=255;	// used to be -1;
 the_pb->shields_class=1;
 the_pb->max_vel=0;
 the_pb->min_vel=0;
 the_pb->go_home_time=-1;
 
 the_pb->current_velocity=0;
 the_pb->wanted_velocity=0;
 the_pb->attack_mode=0;
 the_pb->who_spawned_this_object=0;
 
 the_pb->shield_value=the_pb->mass=0;
 the_pb->dq=0;
 the_pb->no_trivial_rejection=0;
 the_pb->object_category=0;
 the_pb->has_roll=0;
 the_pb->laser_cat=0;
 the_pb->cannon_cat=0;
 the_pb->missile_cat=0;
 the_pb->number_of_missiles=0;
 the_pb->unique_id=0;


}


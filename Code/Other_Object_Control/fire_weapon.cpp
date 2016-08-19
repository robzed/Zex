#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include	"3d_structures_def.h"
#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include	"zex_misc.h"
#include "explode.h"
#include "fire_weapon.h"
#include "aim.h"
#include "sound.h"
#include "monster1.h"
#include "weapons_damage.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include "obj_control_misc_routines.h"
#include "game_defs.h"

void fire_missile_at(int it, int at_what, int obj_class)
{
	extern	DynObjectsFixedSize *ocb_ptr;
	int slot,object_aimed_at_slot;
	float shot_x,shot_y,shot_z;
	anim_pb anim;	//animation pb
	load_dyn_object_pb the_params;
	//int dest_ocb;
	
	
	clear_pb(&the_params);	//saves loads of zero code
	slot=find_vacant_dynamic_object_slot();
	if (slot!=-1)
	{
		object_aimed_at_slot=at_what;	//pass the distance limit
										//	distance_from_zex=get_distance_to_zex(object_aimed_at_slot);
		if (object_aimed_at_slot==-1)
		{ 
			//¥¥¥¥this is a NOP in C -- might need to do something here...¥¥¥¥ 
		}
		else
		{
			if (at_what==zex) play_zsound(danger, sound_low_pri, speech_chan, sound_vol_7);	  //do computer danger voice
			
			(*ocb_ptr).object_list[it].number_of_missiles--;	//monster just fired a missile
			play_zsound_relative_to_zex(it,missile_explode);
			if((*ocb_ptr).object_list[it].missile_category==1)
			{
				anim.object_id[0]='ZZ1A';
				anim.object_id[1]='ZZ1B';
				anim.object_id[2]='ZZ1C';
				the_params.dq=MISSILE_DAMAGE1;	//about 300 for a type 1
			}
			else
				if((*ocb_ptr).object_list[it].missile_category==2)
				{
					anim.object_id[0]='ZZ2A';
					anim.object_id[1]='ZZ2B';
					anim.object_id[2]='ZZ2C';
					the_params.dq=MISSILE_DAMAGE2;	//about 300 for a type 1
				}
			else
				if((*ocb_ptr).object_list[it].missile_category>2)
				{
					anim.object_id[0]='ZZ3A';
					anim.object_id[1]='ZZ3B';
					anim.object_id[2]='ZZ3C';
					the_params.dq=MISSILE_DAMAGE3;	//about 300 for a type 1
				}
			
			
			shot_x=(*ocb_ptr).object_list[it].Oworldx;
			shot_x=shot_x+((1500+(*ocb_ptr).object_list[it].radius)*(*ocb_ptr).object_list[it].object_normal_translated.x);
			shot_y=(*ocb_ptr).object_list[it].Oworldy;
			shot_y=shot_y+((1500+(*ocb_ptr).object_list[it].radius)*(*ocb_ptr).object_list[it].object_normal_translated.y);
			shot_z=(*ocb_ptr).object_list[it].Oworldz;
			
			the_params.world_x=shot_x;
			the_params.world_y=shot_y;
			the_params.world_z=shot_z;
			the_params.Omovedx=200*(*ocb_ptr).object_list[it].object_normal_translated.x;
			the_params.Omovedy=200*(*ocb_ptr).object_list[it].object_normal_translated.y;
			//	the_params.Omovedz=0;
			//	the_params.delta_rot_x=0;
			//	the_params.delta_rot_y=0;
			//	the_params.delta_rot_z=0;
			//	the_params.rot_x=0;
			
			//	the_params.rot_y=0;
			
			//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
			//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
			//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
			the_params.rot_z=(*ocb_ptr).object_list[it].Orotz;
			the_params.controller_ref=missile1;	//controller
			the_params.control_use_1=300;	//250*(*ocb_ptr).object_list[it].missile_category;	//frames to live counter
			the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
			the_params.control_use_3=it;
			//	the_params.control_use_4=0;
			//	the_params.control_use_7=0;
			//	the_params.current_velocity=0;
			//	the_params.wanted_velocity=0;
			//	the_params.attack_mode=0;
			//	the_params.laser_cat=0;
			//	the_params.cannon_cat=0;
			//	the_params.missile_cat=0;
			//	the_params.number_of_missiles=0;
			
			the_params.shield_value=800;
			//   the_params.has_roll=0;
			the_params.mass=10;	//large for a missile
			the_params.object_category=(*ocb_ptr).object_list[it].object_category;
			the_params.no_trivial_rejection=1;
			
			the_params.who_spawned_this_object=it;
			//	load_dyn_object(ship_laser1,slot,&the_params);	//the static object, the position (-1=next free)
			//dest_ocb=
			load_animated_object (&anim,3,1,1,1,&the_params,-1);
			
		} // else if (object_aimed_at_slot==-1)
	} // if (slot!=-1)
}




extern int	dynamic_object_index,showing_stats;


void fire_not_so_weeny_cannon(int it)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

float shot_x,shot_y,shot_z;
int slot,attack_cat;
load_dyn_object_pb the_params;
_3D object_rotated_normal;

if (showing_stats==1) return;

clear_pb(&the_params);	//saves loads of zero code

 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

   slot=find_vacant_dynamic_object_slot();
//   slot=-1;	//dont fire - testing
   if (slot!=-1)
 	{

//debug code
//    test=dynamic_object_index;
//get object rotated normal into object-rotated_normal;
	shot_x=(*ocb_ptr).object_list[it].Oworldx;
	shot_x=shot_x+(1200*object_rotated_normal.x);	//position shot left or right of object
//	shot_x=shot_x+(((*ocb_ptr).object_list[it].radius+1200)*object_rotated_normal.x);	//position shot left or right of object
	shot_y=(*ocb_ptr).object_list[it].Oworldy;
	shot_y=shot_y+(1200*object_rotated_normal.y);	//position shot left or right of object
//	shot_y=shot_y+(((*ocb_ptr).object_list[it].radius+1200)*object_rotated_normal.y);	//position shot left or right of object
	shot_z=(*ocb_ptr).object_list[it].Oworldz;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;
//	the_params.Omovedx=((*ocb_ptr).object_list[it].Omovedx+490)*object_rotated_normal.x;
//	the_params.Omovedy=((*ocb_ptr).object_list[it].Omovedy+490)*object_rotated_normal.y;
	the_params.Omovedx=(490+(*ocb_ptr).object_list[it].current_velocity)*object_rotated_normal.x;
	the_params.Omovedy=(490+(*ocb_ptr).object_list[it].current_velocity)*object_rotated_normal.y;

//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
	the_params.delta_rot_x=10;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
	   	
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right

	the_params.rot_z=(*ocb_ptr).object_list[it].Orotz;
	the_params.controller_ref=player_bullet_not_so_weeny;	//controller
//	the_params.control_use_1=0;	//frames to live counter
	the_params.control_use_2=static_cast<int>((*ocb_ptr).object_list[it].object_normal_translated.x);	//use to control deceleration.
//	the_params.shield_value=0;
//     the_params.has_roll=0;
	the_params.mass=20;
	the_params.dq=GUN_DAMAGE2;
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
	the_params.current_velocity=490;	//320+(*ocb_ptr).object_list[it].current_velocity;	//launchers thrust + 220
	the_params.wanted_velocity=490;	//320+(*ocb_ptr).object_list[it].current_velocity;
//	the_params.attack_mode=0;
	the_params.no_trivial_rejection=1;
    the_params.object_category=(*ocb_ptr).object_list[it].object_category;

//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
     the_params.who_spawned_this_object=it;
//	the_params.unique_id=0;
	load_dyn_object(player_bullet_not_so_weeny,slot,&the_params,-1);	//the static object, the position (-1=next free)

    }	//end of good slot
}	//end of fire_not_so_weeny_cannon


void fire_weeny_cannon(int it)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index;
float shot_x,shot_y,shot_z;
int slot,attack_cat;
load_dyn_object_pb the_params;
_3D object_rotated_normal;

if (showing_stats==1) return;
clear_pb(&the_params);	//saves loads of zero code
 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

//fire cannon is angle this frame is same as last frame (i.e. not turning)
   slot=find_vacant_dynamic_object_slot();
//   slot=-1;	//dont fire - testing
   if (slot!=-1 && (*ocb_ptr).object_list[it].control_use_4==(*ocb_ptr).object_list[it].Orotz)
 	{
//debug code
//    test=dynamic_object_index;
//get object rotated normal into object-rotated_normal;
	shot_x=(*ocb_ptr).object_list[it].Oworldx;
	shot_x=shot_x+(((*ocb_ptr).object_list[it].radius+200)*object_rotated_normal.x);	//position shot left or right of object
	shot_y=(*ocb_ptr).object_list[it].Oworldy;
	shot_y=shot_y+(((*ocb_ptr).object_list[it].radius+200)*object_rotated_normal.y);	//position shot left or right of object
	shot_z=(*ocb_ptr).object_list[it].Oworldz;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;
	the_params.Omovedx=(390+(*ocb_ptr).object_list[it].current_velocity)*object_rotated_normal.x;
	the_params.Omovedy=(390+(*ocb_ptr).object_list[it].current_velocity)*object_rotated_normal.y;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
	the_params.delta_rot_x=10;
//	the_params.Omovedz=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
//	the_params.rot_y=0;
	   	
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right

	the_params.rot_z=(*ocb_ptr).object_list[it].Orotz;
	the_params.controller_ref=player_bullet_weeny;	//controller
//	the_params.control_use_1=0;	//frames to live counter
	the_params.control_use_2=static_cast<int>(object_rotated_normal.x);	//use to control deceleration.
//     the_params.has_roll=0;
//	the_params.shield_value=0;
	the_params.mass=10;
		the_params.dq=GUN_DAMAGE1;
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
	the_params.current_velocity=390;	//thrust
	the_params.wanted_velocity=390;
//	the_params.attack_mode=0;
	the_params.no_trivial_rejection=1;
    the_params.object_category=(*ocb_ptr).object_list[it].object_category;
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;
//	the_params.unique_id=0;
     the_params.who_spawned_this_object=it;
	load_dyn_object(player_bullet_weeny,slot,&the_params,-1);	//the static object, the position (-1=next free)

    }	//end of good slot
}	//end of fire_wweeny_cannon


////monster fires a laser
//void fire_laser(int it)	//class is 1,2 or 3
//{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_rotated_normal; //from engine
//
//extern int	dynamic_object_index,showing_stats;
//float shot_x,shot_y,shot_z;
//int slot,object_aimed_at_slot;
//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
//
//if (showing_stats==1) return;
//if ((*ocb_ptr).object_list[it].laser_category==0) return;
//
// object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;
//
////fire cannon is angle this frame is same as last frame (i.e. not turning)
//   slot=find_vacant_dynamic_object_slot();
////   slot=-1;	//dont fire - testing
//   if (slot!=-1)
// 	{
//	    object_aimed_at_slot=aim_from_object(it,zex_object,12000);	  //pass the distance limit
//	    if (object_aimed_at_slot==-1)
//	    { 
//	            //¥¥¥¥this is a NOP in C -- might need to do something here...¥¥¥¥ 
//	    }
//	    else
//	   {
//	   
//     play_zsound(laser1, sound_high_pri, twin1_chan, sound_vol_7);
//
//	   shot_x=(*ocb_ptr).object_list[it].Oworldx;
//	   shot_x=shot_x+(400*(*ocb_ptr).object_list[it].object_normal_translated.x);
//	   shot_y=(*ocb_ptr).object_list[it].Oworldy;
//	   shot_y=shot_y+(400*(*ocb_ptr).object_list[it].object_normal_translated.y);
//	   shot_z=(*ocb_ptr).object_list[it].Oworldz;
//	  	  
//	   the_params.world_x=shot_x;
//	   the_params.world_y=shot_y;
//	   the_params.world_z=shot_z;
//	   the_params.Omovedx=0;
//	   the_params.Omovedy=0;
//	   the_params.Omovedz=0;
//	   the_params.delta_rot_x=0;
//	   the_params.delta_rot_y=0;
//	   the_params.delta_rot_z=0;
//	   the_params.rot_x=0;
//	  
//	   the_params.rot_y=0;
//	     
//	   the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	   the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	   the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	   the_params.rot_z=0;
//	   the_params.controller_ref=ship_laser1;	//controller
//	   the_params.control_use_1=10;	//frames to live counter
//	   the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
//	   the_params.control_use_3=it;	//who fired
//	   the_params.control_use_4=(*ocb_ptr).object_list[it].laser_category;
//	   the_params.current_velocity=0;
//	   the_params.wanted_velocity=0;
//	   the_params.attack_mode=0;
//	   the_params.laser_cat=0;
//	   the_params.cannon_cat=0;
//	   the_params.missile_cat=0;
//	   the_params.number_of_missiles=0;
//
//	  the_params.shield_value=0;
//	   the_params.mass=8;	//should be 8 for weeny laser
//	   the_params.object_category=baddie_object;
//	   the_params.no_trivial_rejection=1;
//
//	   load_dyn_object(ship_laser1,slot,&the_params);	//the static object, the position (-1=next free)
//    }	//end of good aimed at slot
//    }	//end of good slot
//}	//end of fire_weeny_laser
//
//

//monster fires a laser at something
void fire_laser_at(int aimed_at_object,int it, int users_laser_experience)	//class is 1,2 or 3
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index;
float shot_x,shot_y,shot_z;
int slot,object_aimed_at_slot;
load_dyn_object_pb the_params;
_3D object_rotated_normal;
float examine_x,examine_y;	//x and y of object being examined
float zex_x,zex_y;	//relative to zex
int angle_to_object,angular_diff,attack_cat;
int doing_a_miss;	//from laser experience
int dist;

if (showing_stats==1) return;
if ((*ocb_ptr).object_list[it].is_armed==0) return;	//no can do if this is clear
if ((*ocb_ptr).object_list[it].laser_category==0) return;

dist=static_cast<int>((14000.0*(*ocb_ptr).object_list[it].laser_category)*LASER_RANGE_MULTIPLIER+3000.0);


if (get_distance_to_from(aimed_at_object,it)>dist) return;

clear_pb(&the_params);	//saves loads of zero code

     examine_x=(*ocb_ptr).object_list[aimed_at_object].Oworldx;
     examine_y=(*ocb_ptr).object_list[aimed_at_object].Oworldy;
//from the "angle to" we can compute whether these are in front or behind us.
//use of some nifty routines coming up...
     zex_x=(*ocb_ptr).object_list[it].Oworldx;
     zex_y=(*ocb_ptr).object_list[it].Oworldy;
     angle_to_object=get_angle_to_from ((int)examine_x, (int)examine_y, (int)zex_x, (int)zex_y);

angular_diff=get_diff(angle_to_object,(*ocb_ptr).object_list[it].Orotz);
if (angular_diff<25 || angular_diff>335)	//good enough for me matey
{
 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

//fire cannon is angle this frame is same as last frame (i.e. not turning)
   slot=find_vacant_dynamic_object_slot();
//   slot=-1;	//dont fire - testing
   if (slot!=-1)
 	{
	  object_aimed_at_slot=aimed_at_object;
	  if (object_aimed_at_slot==-1)
	  { 
	          //¥¥¥¥this is a NOP in C -- might need to do something here...¥¥¥¥ 
	  }
	  else
	 {
	 if ((*ocb_ptr).object_list[it].laser_category==1)
     play_zsound_relative_to_zex(it,laser1);
     else
	 if ((*ocb_ptr).object_list[it].laser_category==2)
     play_zsound_relative_to_zex(it,laser2);
     else
     play_zsound_relative_to_zex(it,laser3);


	 shot_x=(*ocb_ptr).object_list[it].Oworldx;
	 shot_x=shot_x+(400*(*ocb_ptr).object_list[it].object_normal_translated.x);
	 shot_y=(*ocb_ptr).object_list[it].Oworldy;
	 shot_y=shot_y+(400*(*ocb_ptr).object_list[it].object_normal_translated.y);
	 shot_z=(*ocb_ptr).object_list[it].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
	
//	the_params.rot_y=0;
	   
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	the_params.rot_z=0;
	 the_params.controller_ref=ship_laser1;	//controller
	 the_params.control_use_1=10;	//frames to live counter
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
	 the_params.control_use_3=it;	//who fired
	 the_params.control_use_4=(*ocb_ptr).object_list[it].laser_category;
//	the_params.control_use_7=0;

//if experience is < 2000 then we may miss
    doing_a_miss=0;
    
//    if (users_laser_experience<2000)
//    {
//     doing_a_miss=RangedRdm(0,2000);
//     if (doing_a_miss>users_laser_experience)
//         
//         doing_a_miss=1;
//         else
//         doing_a_miss=0;
//         
//    }
	the_params.control_use_7=doing_a_miss;
//
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.attack_mode=0;
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//	the_params.number_of_missiles=0;

  //   the_params.has_roll=0;
//	the_params.shield_value=0;
	 the_params.mass=0;	//should be 0
//		the_params.dq=0;
    the_params.object_category=(*ocb_ptr).object_list[it].object_category;

     the_params.who_spawned_this_object=it;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)
    }	//end of good aimed at slot
    }	//end of good slot
 }	//end of angular diff OK

}	//end of fire_weeny_laser




void fire_ejection_pod(int it)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index;
float shot_x,shot_y,shot_z;
int slot,attack_cat;
load_dyn_object_pb the_params;
_3D object_rotated_normal;

clear_pb(&the_params);	//saves loads of zero code

 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

   	slot=find_vacant_dynamic_object_slot();
//   slot=-1;	//dont fire - testing
   	if (slot!=-1)
 		{
//get object rotated normal into object-rotated_normal;
    	object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;
		shot_x=(*ocb_ptr).object_list[it].Oworldx;
		shot_y=(*ocb_ptr).object_list[it].Oworldy;
		shot_z=(*ocb_ptr).object_list[it].Oworldz;
		shot_z=shot_z-700;
		
		the_params.world_x=shot_x;
		the_params.world_y=shot_y;
		the_params.world_z=shot_z;
		the_params.Omovedx=-7;
//		the_params.Omovedy=0;
		the_params.Omovedz=-10;	//start at low speed
//		the_params.delta_rot_x=0;
		the_params.delta_rot_y=1;
		the_params.delta_rot_z=1;
//		the_params.rot_x=0;
//		the_params.rot_y=0;
	   	
//		the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//		the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//		the_params.normal.z =  0;	//objects are ALWAYS defined facing right

//		the_params.rot_z=0;
		the_params.controller_ref=ejection_cap;	//controller
//		the_params.control_use_1=0;	//frames to live counter
///		the_params.control_use_2=0;	//use to control deceleration.
//		the_params.control_use_3=0;
//		the_params.control_use_4=0;
//	the_params.control_use_7=0;
//		the_params.current_velocity=0;
//		the_params.wanted_velocity=0;
//	the_params.attack_mode=0;
//		the_params.shield_value=0;
    the_params.object_category=(*ocb_ptr).object_list[it].object_category;
//	the_params.laser_cat=0;
  //   the_params.has_roll=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
     the_params.who_spawned_this_object=-1;
//	the_params.number_of_missiles=0;
		the_params.mass=12;
//	the_params.unique_id=0;
//			 	 the_params.dq=0;
		load_dyn_object(ejection_cap,slot,&the_params,-1);	//the static object, the position (-1=next free)

    	}	//end of good slot for ejection pod
} 	//end of fire ejection pod


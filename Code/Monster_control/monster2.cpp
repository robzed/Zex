#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <fp.h>
#include	"3d_structures_def.h"
#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include	"zex_misc.h"
#include "explode.h"
#include "sound.h"
#include "monster1.h"
#include "fire_weapon.h"
#include "aim.h"
#include "compile_zmap.h"
#include "Error_handler.h"
#include "instrum_int.h"
#include "weapons_damage.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"


//monster controller 2
//This one always tries to ram zex whilst firing
//SB 13/12/98
//ееееееееееееееееееееееееееееееееееееееееееееееееееееее
//void handle_monster2(int it)	  //
//{
////control 1 = refire timer
////control 2= ejected
////control 4 = last rot z
////control 5 = real velocity
////control 6 = wanted velocity
//extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_rotated_normal; //from engine
//
//extern int	  dynamic_object_index,monsters_can_fire;
//
////load_dyn_object_pb the_params;
//_3D object_rotated_normal;
//int obj_x_ang,go_up,h_direction,obj_z_ang,zex_distance,damp;
//int facing_up,attacking_this;
//float shot_x,shot_y,shot_z,dx,dy;
//
//
//  process_extern_movements(it);	//handles external forces
//
////change for 0.85 - monsters must now avoid things before anything else
//if (do_avoid(it,0)==true) return;	  //did avoidance action
//
////change for d18 for all monsters
////they must now find something to attack and check the thing is still alive
////before attacking
////fills in attack_what
//
//if (find_something_to_attack(it,1,20000)==-1)
//{
////go into a large circle
//   (*ocb_ptr).object_list[it].Orotz--;
////calc thrust components   
//
//   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	  //current velocity
//   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
////check if outside of play area and adjust dx and dy accordingly
//   (*ocb_ptr).object_list[it].Omovedy=dy;
//   (*ocb_ptr).object_list[it].Omovedx=dx;
//
// return;	  //didn't find anything
//}
//
//
//attacking_this=(*ocb_ptr).object_list[it].attack_what;
//
//
//zex_distance=get_distance_to_from(attacking_this,it);
//if (zex_distance <4000  && facing_towards_target(attacking_this,it)==true)
//(*ocb_ptr).object_list[it].wanted_velocity=-60;
//else
//(*ocb_ptr).object_list[it].wanted_velocity=zex_distance/15;
//
//if (zex_distance >75000) (*ocb_ptr).object_list[it].wanted_velocity=0;	//if too far away, stop
//
//if ((*ocb_ptr).object_list[it].wanted_velocity>200) (*ocb_ptr).object_list[it].wanted_velocity=230;	  //limit top speed
////(*ocb_ptr).object_list[it].wanted_velocity=0;
//if (zex_distance<1000) (*ocb_ptr).object_list[it].wanted_velocity=0;
//if (zex_distance>350000) kill(it);	  //if too far away kill it
////if (damp==1) (*ocb_ptr).object_list[it].wanted_velocity=1;
//
//   if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
//      (*ocb_ptr).object_list[it].current_velocity-=9;
//   else
//      if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
//      (*ocb_ptr).object_list[it].current_velocity+=1;
//
////directional and fire control   
// object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;
//
// if ((*ocb_ptr).object_list[it].shield_value>10 && zex_distance>3000)	//if we have enough power for control
// {
// if (RangedRdm(1,10)>4)
//   {
//  ram_object(attacking_this,it,3);	  //this one nearly always aims for zex
//   }
//  } //end of enough power for control
//else
//  turn_away_from_object(attacking_this,it,3);
//
////check cannon fire time
//if ((*ocb_ptr).object_list[it].control_use_1<0) //last cannon timer expired
// {
// 
//  if ((*ocb_ptr).object_list[it].shield_value>45)	  //we have enough strength to fire and move
//  {
//  (*ocb_ptr).object_list[it].control_use_1=RangedRdm(6,40);	  //set time to fire next cannon
//   if (monsters_can_fire==1 && RangedRdm(1,10)>5) fire_laser_at(attacking_this,it);
//   if (monsters_can_fire==1  && is_aimed_at(attacking_this,it)==true) fire_weeny_cannon(it);	  //commented out for debug
//
//   }	  //end of enough shields
//
//   
//  }	  //end of time to fire 
// else	//not time to fire
//
//   //go out of control if we're too lame to control
//   if ((*ocb_ptr).object_list[it].shield_value<43)
//   {
////generate ejection pod
//
//   if ((*ocb_ptr).object_list[it].control_use_2==0)	  //havent ejected yet
//   {
//    (*ocb_ptr).object_list[it].Orotdx=1;
//    (*ocb_ptr).object_list[it].Orotdy=1;
//    (*ocb_ptr).object_list[it].Orotdz=1;
//    (*ocb_ptr).object_list[it].Omovedx=RangedRdm(0,3)-2;
//    (*ocb_ptr).object_list[it].Omovedy=RangedRdm(0,3)-2;	  //shipp loses control and rotates
//   	  
//   	  (*ocb_ptr).object_list[it].control_use_2=1;	  //have now!
//    fire_ejection_pod(it);
// 
//   	  }	  //end of haven't ejected yet
//   }
//  (*ocb_ptr).object_list[it].control_use_1-=1;	//dec time to fire
//
//(*ocb_ptr).object_list[it].control_use_4=(*ocb_ptr).object_list[it].Orotz;	  //save this rot for next frame
//  if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	  //clear down just hit for next frame
//  
//  
//}

extern int global_control_monsters;

//monster 2 replaced 050699 SB
void handle_monster2(int it)	//
{
//control 1 = refire timer
//control 2= ejected
//        3= roll direction
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire;

//load_dyn_object_pb the_params;
_3D object_rotated_normal;
int obj_x_ang,go_up,h_direction,obj_z_ang,zex_distance,roll,object_roll;
int attacking_this,zangle;
float shot_x,shot_y,shot_z,dx,dy;
int avoid_mode;
register ZObject * object_ptr;





  process_extern_movements(it);	//handles external forces
if (global_control_monsters==0) return;

    object_ptr=&ocb_ptr->object_list[it];
//change for 0.85 - monsters must now avoid things before anything else
//if ((*ocb_ptr).object_list[it].attack_what==-1)
//{
//if (do_avoid(it,0)==true) return;	  //did avoidance action/
//}

  if (do_avoid(it,0,0,150,100,3)==true) 
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  object_ptr->attack_what=-1;
  return;	//did avoidance action
  }


//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,160000)==-1)
{
//go into a large circle
   object_ptr->Orotz--;
//calc thrust components   

   dy=object_ptr->object_normal_translated.y*object_ptr->current_velocity;	//current velocity
   dx=object_ptr->object_normal_translated.x*object_ptr->current_velocity;
	   dy+=object_ptr->extern_movedy;
	   dx+=object_ptr->extern_movedx;

//check if outside of play area and adjust dx and dy accordingly
   object_ptr->Omovedy=dy;
   object_ptr->Omovedx=dx;

 return;	//didn't find anything
}



attacking_this=object_ptr->attack_what;

//calc wanted velocity
zex_distance=get_distance_to_from(attacking_this,it);
if (zex_distance<4000) object_ptr->attack_mode=0;	 //run
if (zex_distance>14000) object_ptr->attack_mode=1;	 //attack
//if ((*ocb_ptr).object_list[it].shield_value>100) (*ocb_ptr).object_list[it].attack_mode=1;
//else (*ocb_ptr).object_list[it].attack_mode=0;

if (zex_distance <4500 && facing_towards_target(attacking_this,it)==true)
object_ptr->wanted_velocity=-105;
else
object_ptr->wanted_velocity=280;

if (zex_distance >95000) object_ptr->wanted_velocity=0;	//if too far away, stop

//calc thrust from wanted - cu5=thrust
   if (object_ptr->wanted_velocity<object_ptr->current_velocity)
      object_ptr->current_velocity-=8;	 //brakes
   else
      if (object_ptr->wanted_velocity>object_ptr->current_velocity)
      object_ptr->current_velocity+=2;	 //throttle

//directional and fire control   
 object_rotated_normal=object_ptr->object_normal_translated;

//if (avoid_mode==false)
//{
 if (object_ptr->attack_mode==1) ram_object(attacking_this,it,2);
 else
 turn_away_from_object(attacking_this,it,3);
// ram_object(attacking_this,it,2);

//}

//check cannon fire time
if (object_ptr->control_use_1<0 //fire timer
    || object_ptr->just_hit>0 ) //OR monster was just hit
 {
 
  if (object_ptr->shield_value>15 && zex_distance < 12500)	//we have enough strength to fire and move
  {
  object_ptr->control_use_1=RangedRdm(10,40);	 //set time to fire next cannon
//fire cannon is angle this frame is same as last frame (i.e. not turning)
//   if (monsters_can_fire==1 && RangedRdm(1,10)>3) fire_laser_at(attacking_this,it);
   if (monsters_can_fire==1 && is_aimed_at(attacking_this,it)==true) fire_weeny_cannon(it); 
   }	//end of enough shields
   
   else	//not enough power left for control


   //go out of control
   if (object_ptr->shield_value<50)
   {
//generate ejection pod

   if (object_ptr->control_use_2==0 && RangedRdm(0,100>90))	//havent ejected yet
   {
    object_ptr->Orotdx=1;
    object_ptr->Orotdy=1;
    object_ptr->Orotdz=1;
    object_ptr->Omovedx=3;
    object_ptr->Omovedy=1;	 //shipp loses control and rotates
   	
   	object_ptr->control_use_2=1;	 //have now!
   
    fire_ejection_pod(it);
   	}	//end of haven't ejected yet
   }
   
  }	//end of time to fire 
 else	//not time to fire
  object_ptr->control_use_1-=1;	//dec time to fire

object_ptr->control_use_4=object_ptr->Orotz;	  //save this rot for next frame

  if (object_ptr->just_hit>0) 
  {
  object_ptr->just_hit-=20;	 //clear down just hit for next frame
  }


}

//еееееееееееееееееееееееееееееееееее
//same as #2 but slower accel,better brakes and weapons,heavier, can fire when turning, bigger shields
//typically blue uns
void handle_monster3(int it)	//
{
//control 1 = refire timer
//control 2= ejected
//        3= roll direction
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire;

//load_dyn_object_pb the_params;
_3D object_rotated_normal;
int obj_x_ang,go_up,h_direction,obj_z_ang,zex_distance,roll,object_roll;
int attacking_this,zangle;
float shot_x,shot_y,shot_z,dx,dy;
int avoid_mode;
register ZObject * object_ptr;


  process_extern_movements(it);	//handles external forces

    object_ptr=&ocb_ptr->object_list[it];

//change for 0.85 - monsters must now avoid things before anything else
//if ((*ocb_ptr).object_list[it].attack_what==-1)
//{
//if (do_avoid(it,0)==true) return;	  //did avoidance action
//}
if (global_control_monsters==0) return;


  if (do_avoid(it,0,0,200,0,4)==true) 
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  object_ptr->attack_what=-1;
  return;	//did avoidance action
  }

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,460000)==-1)
{
//go into a large circle
   object_ptr->Orotz--;
//calc thrust components   

   dy=object_ptr->object_normal_translated.y*object_ptr->current_velocity;	  //current velocity
   dx=object_ptr->object_normal_translated.x*object_ptr->current_velocity;
	   dy+=object_ptr->extern_movedy;
	   dx+=object_ptr->extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
   object_ptr->Omovedy=dy;
   object_ptr->Omovedx=dx;

 return;	//didn't find anything
}



attacking_this=object_ptr->attack_what;

//calc wanted velocity
zex_distance=get_distance_to_from(attacking_this,it);
if (zex_distance<4000) object_ptr->attack_mode=0;	 //run
if (zex_distance>14000) object_ptr->attack_mode=1;	 //attack
//if ((*ocb_ptr).object_list[it].shield_value>100) (*ocb_ptr).object_list[it].attack_mode=1;
//else (*ocb_ptr).object_list[it].attack_mode=0;

if (zex_distance <4500 && facing_towards_target(attacking_this,it)==true)
object_ptr->wanted_velocity=-105;
else
object_ptr->wanted_velocity=330;

if (zex_distance >295000) object_ptr->wanted_velocity=0;	//if too far away, stop

//calc thrust from wanted - cu5=thrust
   if (object_ptr->wanted_velocity<object_ptr->current_velocity)
      object_ptr->current_velocity-=8;	 //brakes
   else
      if (object_ptr->wanted_velocity>object_ptr->current_velocity)
      object_ptr->current_velocity+=2;	 //throttle

//directional and fire control   
 object_rotated_normal=object_ptr->object_normal_translated;

//if (avoid_mode==false)
//{
 if (object_ptr->attack_mode==1) ram_object(attacking_this,it,2);
 else
 turn_away_from_object(attacking_this,it,3);
// ram_object(attacking_this,it,2);

//}

// if ((*ocb_ptr).object_list[it].shield_value>10 && zex_distance>2500)	//if we have enough power for control
// {
// if (RangedRdm(1,10)>2)
//   {
//  ram_object(attacking_this,it,2);	//this one nearly always aims for zex
//   }
//  } //end of enough power for control
//else
//  turn_away_from_object(attacking_this,it,3);
  

// if ((*ocb_ptr).object_list[it].shield_value>10 )	//if we have enough power for control
// {
//   if (RangedRdm(1,10)>3)
//    ram_object(attacking_this,it,3);	  //this one nearly always aims for zex
// } //end of enough power for control

// roll=((*ocb_ptr).object_list[it].Orotz-(*ocb_ptr).object_list[it].control_use_4)*4;	  //change in angle
//   if (roll>359) roll-=359;
//   if (roll<0) roll+=359;
if (RangedRdm(10,100)>98 && object_ptr->Orotx<10) //if not rolling
{
 object_ptr->Orotx=12;	 //do a roll
 object_ptr->control_use_3=9;	 //+-9
}

if (RangedRdm(10,100)>98 && object_ptr->Orotx>10)	 //if already rolling 
 object_ptr->control_use_3=-object_ptr->control_use_3;	  //roll the other way

object_roll=object_ptr->Orotx;
if (object_roll>10)
{
// if (object_roll>roll) object_roll--;
// else
 object_roll+=object_ptr->control_use_3;
}
   if (object_roll>359) object_roll-=359;
   if (object_roll<0) object_roll+=359;
   
object_ptr->Orotx=object_roll;
//check cannon fire time
if (object_ptr->control_use_1<0 //fire timer
    || object_ptr->just_hit>0 ) //OR monster was just hit
 {
 
  if (object_ptr->shield_value>15 && zex_distance < 8500)	 //we have enough strength to fire and move
  {
  object_ptr->control_use_1=RangedRdm(10,40);	 //set time to fire next cannon
//fire cannon is angle this frame is same as last frame (i.e. not turning)
   if (monsters_can_fire==1 && RangedRdm(1,10)>3) fire_laser_at(attacking_this,it,200);
   if (monsters_can_fire==1 && is_aimed_at(attacking_this,it)==true) fire_not_so_weeny_cannon(it); 
   }	//end of enough shields
   
   else	//not enough power left for control


   //go out of control
   if (object_ptr->shield_value<50)
   {
//generate ejection pod

   if (object_ptr->control_use_2==0)	 //havent ejected yet
   {
    object_ptr->Orotdx=1;
    object_ptr->Orotdy=1;
    object_ptr->Orotdz=1;
    object_ptr->Omovedx=3;
    object_ptr->Omovedy=1;	 //shipp loses control and rotates
   	
   	object_ptr->control_use_2=1;	 //have now!
   
    fire_ejection_pod(it);
   	}	//end of haven't ejected yet
   }
   
  }	//end of time to fire 
 else	//not time to fire
  object_ptr->control_use_1-=1;	//dec time to fire

object_ptr->control_use_4=object_ptr->Orotz;	  //save this rot for next frame

  if (object_ptr->just_hit>0) 
  {
  object_ptr->just_hit-=20;	 //clear down just hit for next frame
  }


}



//m4=phoenix
void handle_monster4(int it)	//
{
//control 1 = refire timer
//control 2= ejected
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
//these cheat in that if shields drop below 250 they may eject their ejector pod but not eject!
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire;

load_dyn_object_pb the_params;
_3D object_rotated_normal;
int slot,h_direction,obj_z_ang,zex_distance,damp;
int facing_up,attacking_this;
float shot_x,shot_y,shot_z,dy,dx;
int object_aimed_at_slot,distance_from_zex;
anim_pb anim;	//animation pb


  process_extern_movements(it);	//handles external forces
if (global_control_monsters==0) return;

  if (do_avoid(it,0,1,200,200,5)==true) 
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  (*ocb_ptr).object_list[it].attack_what=-1;
  return;	//did avoidance action
  }

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,570000)==-1)
{
//go into a large circle
   (*ocb_ptr).object_list[it].Orotz--;
//calc thrust components   

   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
	   dy+=(*ocb_ptr).object_list[it].extern_movedy;
	   dx+=(*ocb_ptr).object_list[it].extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

 return;	//didn't find anything
}
attacking_this=(*ocb_ptr).object_list[it].attack_what;

//m4 has rechargeable shields
if ((*ocb_ptr).object_list[it].shield_value<600) (*ocb_ptr).object_list[it].shield_value++;

//calc attack/defense mode
if ((*ocb_ptr).object_list[it].attack_mode==0 && (*ocb_ptr).object_list[it].shield_value<120)
(*ocb_ptr).object_list[it].attack_mode=1;	//go to defense

if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].shield_value>550)
(*ocb_ptr).object_list[it].attack_mode=0;	//go to attack


zex_distance=get_distance_to_from(attacking_this,it);
//calc wanted velocity
if ((*ocb_ptr).object_list[it].attack_mode==0)
{
if (zex_distance >3000)
(*ocb_ptr).object_list[it].wanted_velocity=370;	//faster than zex when attackingo
else
(*ocb_ptr).object_list[it].wanted_velocity=-140;	//better reverse thrust
}
else	//not attacking
(*ocb_ptr).object_list[it].wanted_velocity=280;	//faster than m2, slightly slower than zex in retreat

//(*ocb_ptr).object_list[it].wanted_velocity=0;
if (zex_distance<1000) (*ocb_ptr).object_list[it].wanted_velocity=0;

//if (damp==1) (*ocb_ptr).object_list[it].wanted_velocity=1;
//acceleration
   if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity-=14;
   else
      if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity+=4;


//directional and fire control   
 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

 if ((*ocb_ptr).object_list[it].attack_mode==0)
   {
  ram_object(attacking_this,it,4);	//this one nearly always aims for zex
  (*ocb_ptr).object_list[it].control_use_3=0;   }
   else
   if ((*ocb_ptr).object_list[it].control_use_3<12)	//we turn away for 6 frames
   {
   turn_away_from_object(attacking_this,it,4);	//turn away
   (*ocb_ptr).object_list[it].control_use_3++;
   }

   if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].just_hit>0)
      turn_away_from_object(attacking_this,it,2);
      
//check cannon fire time
if ((*ocb_ptr).object_list[it].control_use_1<0) //last cannon timer expired
 {
   (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,20);	//set time to fire next cannon
  if (monsters_can_fire==1 && RangedRdm(1,10)>5) fire_laser_at(attacking_this,it,400);

  if ((*ocb_ptr).object_list[it].shield_value>45 && zex_distance <20000 && (*ocb_ptr).object_list[it].attack_mode==0)	 //we have enough strength to fire and move
   {
      if (monsters_can_fire==1) fire_not_so_weeny_cannon(it);	//commented out for debug
//decide whether to fire a missile
    if ( RangedRdm(10,110)>105 &&
        (*ocb_ptr).object_list[it].number_of_missiles>0
        && (*ocb_ptr).object_list[it].shield_value<650
        )
        {
         fire_missile_at(it,attacking_this,1);
       
      } 
   }	//end of enough shields
   
   //go out of control
   if ((*ocb_ptr).object_list[it].shield_value<250 && RangedRdm(1,10>7))
   {
//generate ejection pod
     if ((*ocb_ptr).object_list[it].control_use_2==0)	//havent ejected yet
     {
//    (*ocb_ptr).object_list[it].Orotdx=1;
//    (*ocb_ptr).object_list[it].Orotdy=1;
//    (*ocb_ptr).object_list[it].Orotdz=1;
//    (*ocb_ptr).object_list[it].Omovedx=3;
//    (*ocb_ptr).object_list[it].Omovedy=1;	//shipp loses control and rotates
//   	
   	(*ocb_ptr).object_list[it].control_use_2=1;	//have now!
      fire_ejection_pod(it);
   	}	//end of haven't ejected yet
    }	//end of decide if to eject
   
  }	//end of time to fire 

(*ocb_ptr).object_list[it].control_use_1-=1;	//dec time to fire

(*ocb_ptr).object_list[it].control_use_4=(*ocb_ptr).object_list[it].Orotz;	//save this rot for next frame
if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	//clear down just hit for next frame


}

#define WARPER_MAX_SHIELDS 2000

unsigned int warper_growl=0;
//WARPER
void handle_warper(int it)	  //
{
//control 1 = refire timer
//control 2= ejected
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
//control 8 - invisible timer

//these cheat in that if shields drop below 250 they may eject their ejector pod but not eject!
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire;

load_dyn_object_pb the_params;
_3D object_rotated_normal;
int slot,h_direction,obj_z_ang,zex_distance,damp;
int facing_up,attacking_this;
float shot_x,shot_y,shot_z,dy,dx;
int object_aimed_at_slot,distance_from_zex;
anim_pb anim;	//animation pb


//do growl
  process_extern_movements(it);	//handles external forces
if (global_control_monsters==0) return;


if (ZexTickCount()>warper_growl)
 {
 warper_growl=ZexTickCount()+50;
  if (RangedRdm(0,10)>5)         play_zsound_relative_to_zex(it,growl);

 }

//change for 0.85 - monsters must now avoid things before anything else
//if (do_avoid(it,0)==true) return;	  //did avoidance action

  if (do_avoid(it,0,0,200,0,3)==true) 
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  (*ocb_ptr).object_list[it].attack_what=-1;
  return;	//did avoidance action
  }

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,290000)==-1)
{
//go into a large circle
   (*ocb_ptr).object_list[it].Orotz--;
//calc thrust components   

   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
	   dy+=(*ocb_ptr).object_list[it].extern_movedy;
	   dx+=(*ocb_ptr).object_list[it].extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

 return;	//didn't find anything
}
attacking_this=(*ocb_ptr).object_list[it].attack_what;

////m4 has rechargeable shields
if ((*ocb_ptr).object_list[it].shield_value<WARPER_MAX_SHIELDS-1) (*ocb_ptr).object_list[it].shield_value+=2;

//calc attack/defense mode
if ((*ocb_ptr).object_list[it].attack_mode==0 && (*ocb_ptr).object_list[it].shield_value<WARPER_MAX_SHIELDS/4)
(*ocb_ptr).object_list[it].attack_mode=1;	//go to defense

if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].shield_value>800)
(*ocb_ptr).object_list[it].attack_mode=0;	//go to attack


zex_distance=get_distance_to_from(attacking_this,it);
//calc wanted velocity
if ((*ocb_ptr).object_list[it].attack_mode==0)
{
if (zex_distance >3000)
(*ocb_ptr).object_list[it].wanted_velocity=370;	//faster than zex when attackingo
else
(*ocb_ptr).object_list[it].wanted_velocity=-50;	//better reverse thrust
}
else	//not attacking
(*ocb_ptr).object_list[it].wanted_velocity=280;	//faster than m2, slightly slower than zex in retreat

//(*ocb_ptr).object_list[it].wanted_velocity=0;
if (zex_distance<1000) (*ocb_ptr).object_list[it].wanted_velocity=0;

//if (damp==1) (*ocb_ptr).object_list[it].wanted_velocity=1;
//acceleration
   if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity-=14;
   else
      if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity+=4;


//directional and fire control   
 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

 if ((*ocb_ptr).object_list[it].attack_mode==0)
   {
  ram_object(attacking_this,it,4);	//this one nearly always aims for zex
  (*ocb_ptr).object_list[it].control_use_3=0;   }
   else
   if ((*ocb_ptr).object_list[it].control_use_3<12)	//we turn away for 6 frames
   {
   turn_away_from_object(attacking_this,it,4);	//turn away
   (*ocb_ptr).object_list[it].control_use_3++;
   }

   if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].just_hit>0)
      turn_away_from_object(attacking_this,it,2);
      
//check cannon fire time
if ((*ocb_ptr).object_list[it].control_use_1<0) //last cannon timer expired
 {
   (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,40);	//set time to fire next cannon
  if (monsters_can_fire==1 && RangedRdm(1,10)>3) fire_laser_at(attacking_this,it,1000);

  if ((*ocb_ptr).object_list[it].shield_value>45 && zex_distance <26000 && (*ocb_ptr).object_list[it].attack_mode==0)	 //we have enough strength to fire and move
   {
      if (monsters_can_fire==1) fire_not_so_weeny_cannon(it);	//commented out for debug
//decide whether to fire a missile
    if ( RangedRdm(10,110)>74 &&
        (*ocb_ptr).object_list[it].number_of_missiles>0
        //&& (*ocb_ptr).object_list[it].shield_value<350
        )
        {
        //fire off a missile
         fire_missile_at(it,attacking_this,1);
	
       
        } 
   }	//end of enough shields
   
   //go out of control
   if ((*ocb_ptr).object_list[it].shield_value<250 && RangedRdm(1,10>7))
   {
//generate ejection pod
     if ((*ocb_ptr).object_list[it].control_use_2==0)	//havent ejected yet
     {
//    (*ocb_ptr).object_list[it].Orotdx=1;
//    (*ocb_ptr).object_list[it].Orotdy=1;
//    (*ocb_ptr).object_list[it].Orotdz=1;
//    (*ocb_ptr).object_list[it].Omovedx=3;
//    (*ocb_ptr).object_list[it].Omovedy=1;	//shipp loses control and rotates
//   	
   	(*ocb_ptr).object_list[it].control_use_2=1;	//have now!
      fire_ejection_pod(it);
   	}	//end of haven't ejected yet
    }	//end of decide if to eject
   
  }	//end of time to fire 

(*ocb_ptr).object_list[it].control_use_1-=1;	//dec time to fire

(*ocb_ptr).object_list[it].control_use_4=(*ocb_ptr).object_list[it].Orotz;	//save this rot for next frame
if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	//clear down just hit for next frame

//invisibility

//visibile if fully charged
if ((*ocb_ptr).object_list[it].shield_value>WARPER_MAX_SHIELDS-100)  (*ocb_ptr).object_list[it].eng_invisible=0;
else
//visible if < half
if ((*ocb_ptr).object_list[it].shield_value<WARPER_MAX_SHIELDS/2)  (*ocb_ptr).object_list[it].eng_invisible=0;
else
(*ocb_ptr).object_list[it].eng_invisible=1;
//
// (*ocb_ptr).object_list[it].control_use_8--;
// 
//if ((*ocb_ptr).object_list[it].control_use_8<0) 
// {
// (*ocb_ptr).object_list[it].control_use_8=400;
// (*ocb_ptr).object_list[it].control_use_9^=1;
// if ((*ocb_ptr).object_list[it].control_use_9==1)
// (*ocb_ptr).object_list[it].eng_invisible=1;
// else
// (*ocb_ptr).object_list[it].eng_invisible=0;
 
// }
}

//m6=python
void handle_monster6(int it)	//
{
//control 1 = refire timer
//control 2= ejected
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
//these cheat in that if shields drop below 250 they may eject their ejector pod but not eject!
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire;

//load_dyn_object_pb the_params;
_3D object_rotated_normal;
int slot,h_direction,obj_z_ang,zex_distance,damp;
int facing_up,attacking_this;
float shot_x,shot_y,shot_z,dy,dx;
int object_aimed_at_slot,distance_from_zex;
//anim_pb anim;	//animation pb


  process_extern_movements(it);	//handles external forces
if (global_control_monsters==0) return;

//  if (do_avoid(it,0)==true) 
//  {
////if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
//  (*ocb_ptr).object_list[it].attack_what=-1;
//  return;	  //did avoidance action
//  }


//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,190000)==-1)
{
//go into a large circle
   (*ocb_ptr).object_list[it].Orotz--;
//calc thrust components   

   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
	   dy+=(*ocb_ptr).object_list[it].extern_movedy;
	   dx+=(*ocb_ptr).object_list[it].extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

 return;	//didn't find anything
}
attacking_this=(*ocb_ptr).object_list[it].attack_what;

//m4 has rechargeable shields
if ((*ocb_ptr).object_list[it].shield_value<900) (*ocb_ptr).object_list[it].shield_value++;

//calc attack/defense mode
if ((*ocb_ptr).object_list[it].attack_mode==0 && (*ocb_ptr).object_list[it].shield_value<160)
(*ocb_ptr).object_list[it].attack_mode=1;	//go to defense

if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].shield_value>550)
(*ocb_ptr).object_list[it].attack_mode=0;	//go to attack


zex_distance=get_distance_to_from(attacking_this,it);
//calc wanted velocity
if ((*ocb_ptr).object_list[it].attack_mode==0)
{
if (zex_distance >6000)
(*ocb_ptr).object_list[it].wanted_velocity=320;	//faster than zex when attackingo
else
(*ocb_ptr).object_list[it].wanted_velocity=-190;	//better reverse thrust
}
else	//not attacking
(*ocb_ptr).object_list[it].wanted_velocity=300;	//faster than m2, slightly slower than zex in retreat

//(*ocb_ptr).object_list[it].wanted_velocity=0;
if (zex_distance<1000) (*ocb_ptr).object_list[it].wanted_velocity=0;

//if (damp==1) (*ocb_ptr).object_list[it].wanted_velocity=1;
//acceleration
   if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity-=19;
   else
      if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity+=2;


//directional and fire control   
 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

 if ((*ocb_ptr).object_list[it].attack_mode==0)
   {
  ram_object_with_roll(attacking_this,it,4);	//this one nearly always aims for zex
  (*ocb_ptr).object_list[it].control_use_3=0;   }
   else
   if ((*ocb_ptr).object_list[it].control_use_3<12)	//we turn away for 6 frames
   {
   turn_away_from_object(attacking_this,it,4);	//turn away
   (*ocb_ptr).object_list[it].control_use_3++;
   }

   if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].just_hit>0)
      turn_away_from_object(attacking_this,it,2);
      
//check cannon fire time
if ((*ocb_ptr).object_list[it].control_use_1<0) //last cannon timer expired
 {
   (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,20);	//set time to fire next cannon
  if (monsters_can_fire==1 && RangedRdm(1,10)>5) fire_laser_at(attacking_this,it,1200);

  if ((*ocb_ptr).object_list[it].shield_value>45 && zex_distance <25000 && (*ocb_ptr).object_list[it].attack_mode==0)	 //we have enough strength to fire and move
   {
      if (monsters_can_fire==1) fire_not_so_weeny_cannon(it);	//commented out for debug
//decide whether to fire a missile
    if ( RangedRdm(10,110)>105 &&
        (*ocb_ptr).object_list[it].number_of_missiles>0
//        && (*ocb_ptr).object_list[it].shield_value<650
        )
        {
         fire_missile_at(it,attacking_this,1);
       
      } 
   }	//end of enough shields
   
   //go out of control
   if ((*ocb_ptr).object_list[it].shield_value<200 && RangedRdm(1,10>8))
   {
//generate ejection pod
     if ((*ocb_ptr).object_list[it].control_use_2<3)	//havent ejected yet
     {
//    (*ocb_ptr).object_list[it].Orotdx=1;
//    (*ocb_ptr).object_list[it].Orotdy=1;
//    (*ocb_ptr).object_list[it].Orotdz=1;
//    (*ocb_ptr).object_list[it].Omovedx=3;
//    (*ocb_ptr).object_list[it].Omovedy=1;	//shipp loses control and rotates
//   	
   	(*ocb_ptr).object_list[it].control_use_2++;	//have now!
      fire_ejection_pod(it);
   	}	//end of haven't ejected yet
    }	//end of decide if to eject
   
  }	//end of time to fire 

(*ocb_ptr).object_list[it].control_use_1-=1;	//dec time to fire

(*ocb_ptr).object_list[it].control_use_4=(*ocb_ptr).object_list[it].Orotz;	//save this rot for next frame
if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	//clear down just hit for next frame


}



//EXCALABUR
//The idea with excaliber is that it just sits there and fires missiles at you.
#define EXCALABUR_MAX_SHIELDS 2000

void handle_excalabur(int it)	     //
{
//control 1 = refire timer
//control 2= ejected
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
//these cheat in that if shields drop below 250 they may eject their ejector pod but not eject!
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire;

//load_dyn_object_pb the_params;
_3D object_rotated_normal;
int slot,h_direction,obj_z_ang,zex_distance,damp;
int facing_up,attacking_this;
float shot_x,shot_y,shot_z,dy,dx;
int object_aimed_at_slot,distance_from_zex;
//anim_pb anim;	//animation pb
register ZObject * object_ptr;


  process_extern_movements(it);	//handles external forces
if (global_control_monsters==0) return;

    object_ptr=&ocb_ptr->object_list[it];
//change for 0.85 - monsters must now avoid things before anything else

  if (do_avoid_scenary(it,0,0,60,0,6)==true) 
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  object_ptr->attack_what=-1;
  return;	//did avoidance action
  }

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,90000)==-1)
{
//just stop
if (object_ptr->wanted_velocity<0) object_ptr->wanted_velocity++;
if (object_ptr->wanted_velocity>0) object_ptr->wanted_velocity--;

//calc thrust components   
   if (object_ptr->wanted_velocity<object_ptr->current_velocity)
      object_ptr->current_velocity-=14;
   else
      if (object_ptr->wanted_velocity>object_ptr->current_velocity)
      object_ptr->current_velocity+=4;

   dy=object_ptr->object_normal_translated.y*object_ptr->current_velocity;	  //current velocity
   dx=object_ptr->object_normal_translated.x*object_ptr->current_velocity;
	   dy+=object_ptr->extern_movedy;
	   dx+=object_ptr->extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
   object_ptr->Omovedy=dy;
   object_ptr->Omovedx=dx;

 return;	//didn't find anything
}

attacking_this=object_ptr->attack_what;

////m4 has rechargeable shields
if ((*ocb_ptr).object_list[it].shield_value<EXCALABUR_MAX_SHIELDS) (*ocb_ptr).object_list[it].shield_value+=2;
//if ((*ocb_ptr).object_list[it].shield_value<600) (*ocb_ptr).object_list[it].shield_value++;
//
////calc attack/defense mode
//if ((*ocb_ptr).object_list[it].attack_mode==0 && (*ocb_ptr).object_list[it].shield_value<120)
//(*ocb_ptr).object_list[it].attack_mode=1;	//go to defense
//
//if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].shield_value>550)
//(*ocb_ptr).object_list[it].attack_mode=0;	//go to attack


zex_distance=get_distance_to_from(attacking_this,it);
//calc wanted velocity
if (object_ptr->attack_mode==0)
{
if (zex_distance >12000)
object_ptr->wanted_velocity=40;	//don't move
else
object_ptr->wanted_velocity=-20;	//better reverse thrust
}
else	//not attacking
object_ptr->wanted_velocity=0;	 //faster than m2, slightly slower than zex in retreat

//(*ocb_ptr).object_list[it].wanted_velocity=0;
if (zex_distance<1000) object_ptr->wanted_velocity=0;

//if (damp==1) (*ocb_ptr).object_list[it].wanted_velocity=1;
//acceleration
   if (object_ptr->wanted_velocity<object_ptr->current_velocity)
      object_ptr->current_velocity-=14;
   else
      if (object_ptr->wanted_velocity>object_ptr->current_velocity)
      object_ptr->current_velocity+=4;


//directional and fire control   
 object_rotated_normal=object_ptr->object_normal_translated;

 if (object_ptr->attack_mode==0)
   {
  ram_object(attacking_this,it,4);	//this one nearly always aims for zex
  object_ptr->control_use_3=0;   
  }
   else
   if (object_ptr->control_use_3<12)	 //we turn away for 6 frames
   {
   turn_away_from_object(attacking_this,it,4);	//turn away
   object_ptr->control_use_3++;
   }

//   if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].just_hit>0)
//      turn_away_from_object(attacking_this,it,2);
      
//check cannon fire time
//if ((*ocb_ptr).object_list[it].control_use_1<0) //last cannon timer expired
// {
//   (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,40);	//set time to fire next cannon
//  if (monsters_can_fire==1 && RangedRdm(1,10)>3) fire_laser_at(attacking_this,it,2000);
//
//  if ((*ocb_ptr).object_list[it].shield_value>45 && zex_distance <26000 && (*ocb_ptr).object_list[it].attack_mode==0)	//we have enough strength to fire and move
//   {
//      if (monsters_can_fire==1) fire_not_so_weeny_cannon(it);	//commented out for debug
//decide whether to fire a missile
    if (object_ptr->control_use_1<0)
    {
    if ( RangedRdm(10,110)>106 )
    {
         fire_missile_at(it,attacking_this,1);
         object_ptr->control_use_1=180;
    }
    }
    else
    object_ptr->control_use_1--;
//   }	  //end of enough shields
   
   //go out of control
   if (object_ptr->shield_value<250 && RangedRdm(1,10>7))
   {
//generate ejection pod
     if (object_ptr->control_use_2==0)	 //havent ejected yet
     {
//    (*ocb_ptr).object_list[it].Orotdx=1;
//    (*ocb_ptr).object_list[it].Orotdy=1;
//    (*ocb_ptr).object_list[it].Orotdz=1;
//    (*ocb_ptr).object_list[it].Omovedx=3;
//    (*ocb_ptr).object_list[it].Omovedy=1;	//shipp loses control and rotates
//   	
   	object_ptr->control_use_2=1;	 //have now!
      fire_ejection_pod(it);
   	}	//end of haven't ejected yet
    }	//end of decide if to eject
   
//  }	  //end of time to fire 

object_ptr->control_use_1-=1;	 //dec time to fire

object_ptr->control_use_4=object_ptr->Orotz;	  //save this rot for next frame
if (object_ptr->just_hit>0) object_ptr->just_hit--;	  //clear down just hit for next frame


}


//Ok, these are the real controllers :)



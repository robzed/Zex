// *************************************MODULE HEADER*********************************
// * FILENAME           : NPC_Control.c
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 10:02:14 on 18th August 1999
// * FIRST AUTHOR       : Stu
// *
// * COPYRIGHT (c)        
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *

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
#include "NPC_Control.h"
#include "level_control.h"
#include "motherships.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void run_fighter(int it);
void run_traveller(int it);
void run_guard(ZObject * object_ptr);

extern	DynObjectsFixedSize *ocb_ptr;
//called from object_control_top

void NPC_Control(int it)
{
extern mothership_nav_entry mothership_nav [];
extern int global_control_monsters,global_request_dock;

int closest_mship;
int dx,dy;

register ZObject * object_ptr;

if ((*ocb_ptr).object_list[it].docked_object_slot!=-1) return;	//being tractored

    process_extern_movements(it);	//handles external forces
    object_ptr=&ocb_ptr->object_list[it];
if (global_control_monsters==0) return;

if (RangedRdm(0,100)>10)
{
  if (do_avoid(it,1,1,150,100,3)==true) //(int it, int with_roll, int can_reverse, int fwd_vel, int rev_vel, int max_angle)
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  object_ptr->attack_what=-1;
  goto check_return;	//did avoidance action
  }
}

//standard thrust calcs
if (object_ptr->current_velocity<object_ptr->wanted_velocity)
    object_ptr->current_velocity+=10;
else
if (object_ptr->current_velocity>object_ptr->wanted_velocity)
    object_ptr->current_velocity-=10;


   dy=static_cast<int>(object_ptr->object_normal_translated.y*object_ptr->current_velocity);	//current velocity
   dx=static_cast<int>(object_ptr->object_normal_translated.x*object_ptr->current_velocity);
   dy=static_cast<int>(dy+object_ptr->extern_movedy);
   dx=static_cast<int>(dx+object_ptr->extern_movedx);

   object_ptr->Omovedy=dy;
   object_ptr->Omovedx=dx;

//decode class
    if (object_ptr->obj_class==NPC_CLASS_FIGHTER) run_fighter(it);
    else
    if (object_ptr->obj_class==NPC_CLASS_TRAVELLER) run_traveller(it);
    else
    if (object_ptr->obj_class==NPC_CLASS_GUARD) run_guard(object_ptr);

//common stuff
//check if time to go home and set up if so
check_return: 
if (object_ptr->time_out>0)
object_ptr->time_out++;

if (object_ptr->time_out>object_ptr->go_home_time /*&& object_ptr->obj_class!=NPC_CLASS_TRAVELLER*/) 
 {
  //find a mothership
     closest_mship=find_nearest_mothership_to_object(it,object_ptr->object_category);
     //found one?
     if (closest_mship!=-1)
      {
      object_ptr->control_use_10=closest_mship;
      object_ptr->go_home_time=-2;	//set dock flag
      object_ptr->time_out=0;
      }
 }
//end

//check if we are supposed to be heading home
if (object_ptr->go_home_time==-2)	//yup
   {
   int zex_wx,zex_wy,obj_wx,obj_wy;
   int zangle,wangle,d1,d2,dist;
     //calc angle to mship
     
       zex_wx=static_cast<int>(mothership_nav[object_ptr->control_use_10].x);	//x and y of mothership
       zex_wy=static_cast<int>(mothership_nav[object_ptr->control_use_10].y);

       obj_wx=static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
       obj_wy=static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

       zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);

   		//request dock - we have to keep shoving this in until the mothership is free to dock us
   		
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
       wangle=zangle;
//calc whether to turn left or right
        zangle=(*ocb_ptr).object_list[it].Orotz;
   
        if (wangle!=zangle)
         {

          d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
          d2=get_diff(wangle,zangle);
          if (d1>d2) zangle+=1;
          else
          zangle-=1;
          if (zangle>359) zangle-=360;
          if (zangle<0) zangle+=360;

          object_ptr->Orotz=zangle;
         }
      //calc thrust
      //want thrust at max if outside 30000 units else thurst at 50
      dist=get_distance_to_from_point(obj_wx, obj_wy, zex_wx, zex_wy);
      if (dist>17000) object_ptr->wanted_velocity=object_ptr->max_vel;	
      else 
      {
       global_request_dock=it;
       object_ptr->request_dock=1;

      if (object_ptr->cu13==-1)  
	  {
        do_callsign_text("     REQUESTING DOCK.",it,2);   
        object_ptr->cu13=1;
	  }
      else
	  {
        if (object_ptr->cu13>0) object_ptr->cu13++;
	  }   
      if (object_ptr->cu13==300 && object_ptr->Oid=='FALC')  
      {
      int temp;
      temp=RangedRdm(0,3);
      if (temp==0)
      do_callsign_text("     DOCK ME! I'M KNACKERED!",it,2);   
      else
      if (temp==1)
      do_callsign_text("     DOCK ME! YOU REALLY CAN'T GET THE SERVICE THESE DAYS!",it,2);   
      else
      if (temp==2)
      do_callsign_text("     I'M SURE I JUST REQUESTED TO BE DOCKED!?!",it,2);   
      else
      if(temp==3)
      do_callsign_text("     POLITELY REQUESTING DOCK.",it,2);   
      }
       else
      if (object_ptr->cu13==700 && object_ptr->Oid=='FALC')  
      {
      int temp;
      temp=RangedRdm(0,3);
      if (temp==0)
      do_callsign_text("     COME ON, COME ON - BET YOU'RE ALL NICE AND WARM IN THERE...",it,2);   
      else
      if (temp==1)
      do_callsign_text("     I'M ON MY KNEES - PLEASE, PLEASE PLEASE DOCK ME. NOW!",it,2);   
      else
      if (temp==2)
      do_callsign_text("     COME ON! DOCK ME YOU PILE OF JUNK",it,2);   
      else
      if (temp==3)
      do_callsign_text("     GREAT! LIEUTENANT SPEEDY GONZALEZ ON THE CONTROLS TODAY.",it,2);   
      } 
      object_ptr->wanted_velocity=0;
      }
     }	//end of dock

  
//end

}
extern int	dynamic_object_index,monsters_can_fire;

void run_fighter(int it)
{
//control 1 = refire timer
//control 2= ejected
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
//these cheat in that if shields drop below 250 they may eject their ejector pod but not eject!
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine


load_dyn_object_pb the_params;
_3D object_rotated_normal;
int slot,h_direction,obj_z_ang,zex_distance,damp;
int facing_up,attacking_this;
float shot_x,shot_y,shot_z,dy,dx;
int object_aimed_at_slot,distance_from_zex;
anim_pb anim;	//animation pb


if ((*ocb_ptr).object_list[it].sub_controller==warper) 
{
handle_warper(it);
return;
}
//generic fighter

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,1,SECTION_SIZE)==-1)
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

//handle shields
if ((*ocb_ptr).object_list[it].shield_value<(*ocb_ptr).object_list[it].original_shield_value) 
    (*ocb_ptr).object_list[it].shield_value+=(*ocb_ptr).object_list[it].shields_class;

//calc attack/defense mode
if ((*ocb_ptr).object_list[it].attack_mode==0 && (*ocb_ptr).object_list[it].shield_value<(*ocb_ptr).object_list[it].original_shield_value/3)
(*ocb_ptr).object_list[it].attack_mode=1;	//go to defense

if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].shield_value>(*ocb_ptr).object_list[it].original_shield_value/2)
(*ocb_ptr).object_list[it].attack_mode=0;	//go to attack


zex_distance=get_distance_to_from(attacking_this,it);
//calc wanted velocity
if ((*ocb_ptr).object_list[it].attack_mode==0)
{
if (zex_distance >26000) //Beta5 - was 3000
(*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].max_vel;	//max speed
else
(*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].min_vel;	//reverse thrust
}
else	//not attacking
(*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].max_vel;	//run away max speed

//(*ocb_ptr).object_list[it].wanted_velocity=0;
if (zex_distance<16000) (*ocb_ptr).object_list[it].wanted_velocity=0; //Beta5 - dist was 1000

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
   if ((*ocb_ptr).object_list[it].control_use_3<12)	//we turn away for 12 frames
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
  if (monsters_can_fire==1 && RangedRdm(1,10)>6) fire_laser_at(attacking_this,it,400);

  if ((*ocb_ptr).object_list[it].shield_value>45 && zex_distance <20000 && (*ocb_ptr).object_list[it].attack_mode==0)	 //we have enough strength to fire and move
   {
      if (monsters_can_fire==1) 
       {
       if ((*ocb_ptr).object_list[it].cannon_category==1)
       fire_weeny_cannon(it);	//commented out for debug
       if ((*ocb_ptr).object_list[it].cannon_category==2)
       fire_not_so_weeny_cannon(it);	//commented out for debug
       if ((*ocb_ptr).object_list[it].cannon_category==3)
       fire_not_so_weeny_cannon(it);	//commented out for debug
      
       }
//decide whether to fire a missile
    if ( RangedRdm(10,110)>85 &&	//Beta5 - was 75
        (*ocb_ptr).object_list[it].number_of_missiles>0
        && (*ocb_ptr).object_list[it].shield_value<1000
        && (*ocb_ptr).object_list[it].attack_mode==0
        )
        {
         fire_missile_at(it,attacking_this,1);
       
      } 
   }	//end of enough shields
   
   //go out of control
   if ((*ocb_ptr).object_list[it].shield_value<350 && RangedRdm(1,10>7))	//Beta5 - shield value was 250
   {
	   if ((*ocb_ptr).object_list[it].Oid!='MON2')	//mon2's do not eject - they have no pilot
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
		}
	}	//end of decide if to eject
   
  }	//end of time to fire 

(*ocb_ptr).object_list[it].control_use_1-=1;	//dec time to fire

(*ocb_ptr).object_list[it].control_use_4=(*ocb_ptr).object_list[it].Orotz;	//save this rot for next frame
if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	//clear down just hit for next frame
}

//we use cu3 to indicate whether we have initted
void run_traveller(int it)
{
int furthest_mship;
//   if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
//      (*ocb_ptr).object_list[it].current_velocity-=14;
//   else
//      if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
//      (*ocb_ptr).object_list[it].current_velocity+=4;
_3D object_rotated_normal;
int slot,h_direction,obj_z_ang,zex_distance,damp;
int facing_up,attacking_this;
float shot_x,shot_y,shot_z,dy,dx;
int object_aimed_at_slot,distance_from_zex;
anim_pb anim;	//animation pb
extern int difficulty;


if (find_something_to_attack(it,1,25000)==-1)
goto nothing_to_fire_at;

attacking_this=(*ocb_ptr).object_list[it].attack_what;

//handle shields
//if ((*ocb_ptr).object_list[it].shield_value<(*ocb_ptr).object_list[it].original_shield_value) 
//    (*ocb_ptr).object_list[it].shield_value+=(*ocb_ptr).object_list[it].shields_class;

//calc attack/defense mode
if ((*ocb_ptr).object_list[it].attack_mode==0 && (*ocb_ptr).object_list[it].shield_value<220)
(*ocb_ptr).object_list[it].attack_mode=1;	//go to defense

if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].shield_value>600)
(*ocb_ptr).object_list[it].attack_mode=0;	//go to attack


zex_distance=get_distance_to_from(attacking_this,it);
//calc wanted velocity
if ((*ocb_ptr).object_list[it].attack_mode==0)
{
if (zex_distance >3000)
(*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].max_vel;	//max speed
else
(*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].min_vel;	//reverse thrust
}
else	//not attacking
(*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].max_vel;	//run away max speed

//(*ocb_ptr).object_list[it].wanted_velocity=0;
if (zex_distance<4000) (*ocb_ptr).object_list[it].wanted_velocity=0;

//if (damp==1) (*ocb_ptr).object_list[it].wanted_velocity=1;
//acceleration
   if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity-=6;
   else
      if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
      (*ocb_ptr).object_list[it].current_velocity+=4;


//directional and fire control   
 object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;

 if ((*ocb_ptr).object_list[it].attack_mode==0)
   {
   if (difficulty==16)
  ram_object(attacking_this,it,3);	//this one nearly always aims for zex
  else
   if (difficulty==8)
  ram_object(attacking_this,it,4);	//this one nearly always aims for zex
  else
  ram_object(attacking_this,it,5);	//this one nearly always aims for zex

  
  (*ocb_ptr).object_list[it].control_use_3=0;   }
   else
   if ((*ocb_ptr).object_list[it].control_use_3<12)	//we turn away for 6 frames
   {
   turn_away_from_object(attacking_this,it,2);	//turn away
   (*ocb_ptr).object_list[it].control_use_3++;
   }

   if ((*ocb_ptr).object_list[it].attack_mode==1 && (*ocb_ptr).object_list[it].just_hit>0)
      turn_away_from_object(attacking_this,it,2);
      
//check cannon fire time
if ((*ocb_ptr).object_list[it].control_use_1<0) //last cannon timer expired
 {
   (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,20);	//set time to fire next cannon
  if (monsters_can_fire==1 && RangedRdm(1,10)>2) fire_laser_at(attacking_this,it,400);

  if ((*ocb_ptr).object_list[it].shield_value>45 && zex_distance <20000 && (*ocb_ptr).object_list[it].attack_mode==0)	 //we have enough strength to fire and move
   {
      if (monsters_can_fire==1) 
       {
       if ((*ocb_ptr).object_list[it].cannon_category==1)
       fire_weeny_cannon(it);	//commented out for debug
       if ((*ocb_ptr).object_list[it].cannon_category==2)
       fire_not_so_weeny_cannon(it);	//commented out for debug
       if ((*ocb_ptr).object_list[it].cannon_category==3)
       fire_not_so_weeny_cannon(it);	//commented out for debug
      
       }
//decide whether to fire a missile
    if ( RangedRdm(10,110)>100 &&
        (*ocb_ptr).object_list[it].number_of_missiles>0
        /*&& (*ocb_ptr).object_list[it].shield_value<650*/
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




nothing_to_fire_at:    if ((*ocb_ptr).object_list[it].go_home_time==-2) return;	//we are going 
  //find a mothership to travel to
/*     furthest_mship=find_furthest_mothership_from_object(it,-1);	//returns OCB slot
     //found one?
     if (furthest_mship!=-1 && furthest_mship!=(*ocb_ptr).object_list[it].control_use_7)	//not the mship that launched it!
      {
      (*ocb_ptr).object_list[it].control_use_10=furthest_mship;
      (*ocb_ptr).object_list[it].go_home_time=-2;	//set dock flag
      (*ocb_ptr).object_list[it].time_out=0;
      }
      else
*/
      (*ocb_ptr).object_list[it].wanted_velocity=(*ocb_ptr).object_list[it].max_vel;
}

void run_guard(ZObject * object_ptr)
{
}



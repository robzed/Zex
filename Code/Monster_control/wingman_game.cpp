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

// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 1
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 02:49:43 on 16th September 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
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

#include "wingman.h"
#include	"3d_structures_def.h"
#include "fire_weapon.h"
#include "Error_handler.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include "dlp.h"
#include	"objects_controls.h"
#include	"zex_misc.h"
// ***********************************************************************************
// * CONSTANTS 
// *

#define WM_MAX_SPEED 360

// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern int player_input_counter;
extern int wingmen_guarding_what_object;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE              : 
// | AUTHOR(s)          :
// | DATE STARTED       : 
// +----------------------------------------------------------------------------------
// | DESCRIPTION:  
// | 
// | 
// |
// |
// +----------------------------------------------------------------------------------
// | PARAM IN   : none
// |
// | PARAM OUT  : none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial                Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// | 

void handle_wingman_for_game(int it)	//
{
//control 1 = refire timer
//control 2= gone for repairs to mothership
//        3= roll direction
//control 4 = last rot z
//control 5 = real velocity
//control 6 = wanted velocity
//control 7 - message transmitted re: waiting for zex to undock
//10 = hysterisis
//9=radio init and ctrl
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine

extern int	dynamic_object_index,monsters_can_fire,thrust;

//load_dyn_object_pb the_params;
_3D object_rotated_normal;
int obj_x_ang,go_up,h_direction,obj_z_ang,zex_distance,roll,object_roll;
int attacking_this,zangle;
float shot_x,shot_y,shot_z,dx,dy;
int avoid_mode,turn_delta;
register ZObject * current_object_ptr;


    current_object_ptr=&ocb_ptr->object_list[it];

//check whether initted
//if ((*ocb_ptr).object_list[it].control_use_9==0 && (*ocb_ptr).object_list[it].control_use_2==0)	//not innited
if (current_object_ptr->control_use_9==0)	 //not innited
{
    if (current_object_ptr->callsign==0)	//if it hasn't been filled already
    gen_callsign(it);	//fills control_use_8 with 4 char callsign
//    do_callsign_text("     READY!",it);	//callsign goes in first 4 chars of string
    wingman_ready_text(it);
//    add_to_text_display("NAME READY",1);
    current_object_ptr->control_use_9=1;
    player_input_counter=0;	//inc'd every frame
}

//  process_extern_movements(it);	//handles external forces
// if control_use_2 is set we do not have enough power for control

//if ((*ocb_ptr).object_list[it].extern_rotdz!=0) 
//{
////just process spin
//	     dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	  //current velocity
//	     dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
//	     dy+=(*ocb_ptr).object_list[it].extern_movedy;
//	     dx+=(*ocb_ptr).object_list[it].extern_movedx;
////check if outside of play area and adjust dx and dy accordingly
//	     (*ocb_ptr).object_list[it].Omovedy=dy;
//	     (*ocb_ptr).object_list[it].Omovedx=dx;
//return;	//we are spinning
//}

//check whether player has moved in the last 300 frames and issue a sarcy message if not..
if (player_input_counter==2000)
    {
    wingman_heyboss_text(it);
    }

if (player_input_counter==5000)
    {
    wingman_heyboss1_text(it);
    }
    
//can be used by level controller to ge na nasty...
if (player_input_counter==6000)     player_input_counter=0;	//inc'd every frame



if (thrust>=-30 && thrust<40)    
player_input_counter++;



//check command
if (current_object_ptr->command!=0x8000)
{
 //new command has arrived
 //set act on counter
 current_object_ptr->command_timer=RangedRdm(30,100);
 current_object_ptr->command_memory=current_object_ptr->command;	//remember the command
}
current_object_ptr->command = current_object_ptr->command = 0x8000;	//set bit 15 to indicate command ack'd

//now see if the command timer is about to expire

if ( current_object_ptr->command_timer>0)
 current_object_ptr->command_timer--;
 
if (current_object_ptr->command_timer==1)
{
 //process the command
 if ( current_object_ptr->command_memory==SEND_STATUS)
    {
    if (current_object_ptr->shield_value>600)
                do_callsign_text("     STATUS: ALL NORMAL.",it,2);   
    else
    if (current_object_ptr->shield_value>300)
                do_callsign_text("     STATUS: SHIELDS ARE LOW.",it,2);   
    else
                do_callsign_text("     STATUS: GONNA DIE!",it,2);   
    
    }

else
 if ( current_object_ptr->command_memory==STAND_DOWN)
    {
     (*ocb_ptr).object_list[it].controller=NPC_CONTROL;	//is this self modifying code??? :)
     (*ocb_ptr).object_list[it].current_mode=1;

     do_callsign_text("     STANDING DOWN.",it,2);    

    }

else
 if ( current_object_ptr->command_memory==FOLLOW_ME)
    {
     (*ocb_ptr).object_list[it].current_mode=FOLLOW_ME;

     do_callsign_text("     COMING ON YOUR TAIL.",it,2);    

    }
else
 if ( current_object_ptr->command_memory==HOLD_POSITION)
    {
     (*ocb_ptr).object_list[it].current_mode=HOLD_POSITION;

     do_callsign_text("     WAITING HERE.",it,2);    

    }

}



//HOLD_POSITION code
if (current_object_ptr->current_mode==HOLD_POSITION)
{
    current_object_ptr->wanted_velocity=0;
   	
   	if (current_object_ptr->wanted_velocity<current_object_ptr->current_velocity)
      current_object_ptr->current_velocity-=8;	 //brakes
   	else
      if (current_object_ptr->wanted_velocity>current_object_ptr->current_velocity)
      current_object_ptr->current_velocity+=20;	//throttle
	   dy=current_object_ptr->object_normal_translated.y*current_object_ptr->current_velocity;	  //current velocity
	   dx=current_object_ptr->object_normal_translated.x*current_object_ptr->current_velocity;
	   dy+=current_object_ptr->extern_movedy;
	   dx+=current_object_ptr->extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
	   current_object_ptr->Omovedy=dy;
	   current_object_ptr->Omovedx=dx;

goto wingman_recharge;

}


else

//NORMAL MODE
if (current_object_ptr->current_mode==FOLLOW_ME)
{
//normal modus operandi
if (current_object_ptr->control_use_2==1)
{
//reduce speed
   	if (current_object_ptr->wanted_velocity<current_object_ptr->current_velocity)
      current_object_ptr->current_velocity-=8;	 //brakes
   	else
      if (current_object_ptr->wanted_velocity>current_object_ptr->current_velocity)
      current_object_ptr->current_velocity+=20;	//throttle

//calc thrust components   


	   dy=current_object_ptr->object_normal_translated.y*current_object_ptr->current_velocity;	  //current velocity
	   dx=current_object_ptr->object_normal_translated.x*current_object_ptr->current_velocity;
	   dy+=current_object_ptr->extern_movedy;
	   dx+=current_object_ptr->extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
	   current_object_ptr->Omovedy=dy;
	   current_object_ptr->Omovedx=dx;

goto wingman_recharge;
}


if (do_avoid(it,2,1,150,150,5)) goto wingman_recharge;
//  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
//  (*ocb_ptr).object_list[it].attack_what=-1;
//  return;	//did avoidance action
//  }
 
 //if (do_avoid(it,1)==true) return;	    //did avoidance action

//change for 0.85 - monsters must now avoid things before anything else
 zex_distance=get_distance_to_from(wingmen_guarding_what_object,it);

//wait about if zex is docking
//if ((*ocb_ptr).object_list[it].control_use_2==0)
//{


//check if zex is docking, and if so stop
//	  if ((*ocb_ptr).object_list[zex].docked_object_slot!=-1) 
//	  {
//	  	  if ( zex_distance<12000) (*ocb_ptr).object_list[it].wanted_velocity=0;
//	  	  else 
//	  	  if ( zex_distance>16000)
//	  	  (*ocb_ptr).object_list[it].wanted_velocity=WM_MAX_SPEED;
//    
//	    	  (*ocb_ptr).object_list[it].Orotx=0;
//   	  	  if ((*ocb_ptr).object_list[it].wanted_velocity<(*ocb_ptr).object_list[it].current_velocity)
//     	(*ocb_ptr).object_list[it].current_velocity-=8;	//brakes
//   	  	  else
//      	if ((*ocb_ptr).object_list[it].wanted_velocity>(*ocb_ptr).object_list[it].current_velocity)
//      	(*ocb_ptr).object_list[it].current_velocity+=1;	//throttle
//	     	  dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
//	     	  dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
//	     dy+=(*ocb_ptr).object_list[it].extern_movedy;
//	     dx+=(*ocb_ptr).object_list[it].extern_movedx;
////check if outside of play area and adjust dx and dy accordingly
//	     	  (*ocb_ptr).object_list[it].Omovedy=dy;
//	     	  (*ocb_ptr).object_list[it].Omovedx=dx;
//
//	  	    	  turn_delta=ram_object(zex,it,3);	  //this one nearly always aims for zex
//
//
//    	  if ((*ocb_ptr).object_list[it].control_use_7==0)
//    	  {
////    		    int temp;
////    		    temp=RangedRdm(0,10);
////    		    if (temp<3)
////    		    do_callsign_text("     Taking a coffee break...",it);	//callsign goes in first 4 chars of string
////    		    else if (temp<6)
////    		    do_callsign_text("     Waiting...",it);	//callsign goes in first 4 chars of string
////    		    else
////    		    do_callsign_text("     Standing Guard.",it);	//callsign goes in first 4 chars of string
//
//    	  	  (*ocb_ptr).object_list[it].control_use_7=1;
//     	}
//	  return;
//	  }
//
	current_object_ptr->control_use_7=0;	 //not taking a coffee break

 	if (zex_distance<8000) current_object_ptr->control_use_10=0;

 	if (zex_distance>60000 || current_object_ptr->control_use_10==1)
 	{
	  current_object_ptr->wanted_velocity=WM_MAX_SPEED;
	  current_object_ptr->Orotx=0;
   		if (current_object_ptr->wanted_velocity<current_object_ptr->current_velocity)
      		current_object_ptr->current_velocity-=14;	 //brakes
   		else
      		if (current_object_ptr->wanted_velocity>current_object_ptr->current_velocity)
      		current_object_ptr->current_velocity+=2;	//throttle

  		current_object_ptr->control_use_10=1;
  		turn_delta=ram_object(wingmen_guarding_what_object,it,3);	 //this one nearly always aims for zex

  		current_object_ptr->attack_what=-1;
 		goto wingman_recharge;
 	}
//}	//end of check if zex is docking

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
//if (find_something_to_attack(it,1,30000)==-1 && (*ocb_ptr).object_list[it].control_use_2==0)
if (find_something_to_attack(it,1,30000)==-1)
{
	int wangle,angular_velocity,d1,d2,zangle;

	if (current_object_ptr->control_use_9==2 )	 //set to 2 in attack
	{
	current_object_ptr->control_use_9=1;
//    add_to_text_display("NAME RETURNING",1);
//    do_callsign_text("     RETURNING",it);	  //callsign goes in first 4 chars of string
        wingman_returning_text(it);
	}

//return to zex if nothing found
	if (zex_distance>12000) 
	  {
	  current_object_ptr->wanted_velocity=WM_MAX_SPEED;
	  current_object_ptr->Orotx=0;
   if (current_object_ptr->wanted_velocity<current_object_ptr->current_velocity)
      current_object_ptr->current_velocity-=8;	 //brakes
   else
      if (current_object_ptr->wanted_velocity>current_object_ptr->current_velocity)
      current_object_ptr->current_velocity+=3;	 //throttle

   turn_delta=ram_object(wingmen_guarding_what_object,it,4);	 //this one nearly always aims for zex

   goto wingman_recharge;
	  }


//track zex
//get wanted heading
wangle=(*ocb_ptr).object_list[wingmen_guarding_what_object].Orotz;
//	angular_velocity=3;	//max degree per frame change
//adjust heading
zangle=current_object_ptr->Orotz;
if (wangle!=zangle)
	   {
   
	    d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
	    d2=get_diff(wangle,zangle);
        
        if (d1<d2) angular_velocity=d1/15;
        else
        angular_velocity=d2/15;
        
        if (angular_velocity<1) angular_velocity=1;
 	    if (d1>d2) 
	    {
	     if (d2<angular_velocity) angular_velocity=d2;	  //so we don't overshoot
	     zangle+=angular_velocity;
	    }
	    else
	    {
	     if (d1<angular_velocity) angular_velocity=d1;	  //so we don't overshoot
	     angular_velocity=-angular_velocity;
	     zangle+=angular_velocity;
	    }
	    
	    if (zangle>359) zangle-=360;
	    if (zangle<0) zangle+=360;

	    current_object_ptr->Orotz=zangle;
	   }
   

 
	current_object_ptr->wanted_velocity=thrust;	//wanted velocity
//set max/min vels
	if (current_object_ptr->wanted_velocity>WM_MAX_SPEED) current_object_ptr->wanted_velocity=WM_MAX_SPEED;
	if (current_object_ptr->wanted_velocity<-100) current_object_ptr->wanted_velocity=-100;

//do thrust
   	if (current_object_ptr->wanted_velocity<current_object_ptr->current_velocity)
      current_object_ptr->current_velocity-=8;	 //brakes
   	else
      if (current_object_ptr->wanted_velocity>current_object_ptr->current_velocity)
      current_object_ptr->current_velocity+=20;	//throttle

//calc thrust components   


	   dy=current_object_ptr->object_normal_translated.y*current_object_ptr->current_velocity;	  //current velocity
	   dx=current_object_ptr->object_normal_translated.x*current_object_ptr->current_velocity;
	   dy+=current_object_ptr->extern_movedy;
	   dx+=current_object_ptr->extern_movedx;
//check if outside of play area and adjust dx and dy accordingly
	   current_object_ptr->Omovedy=dy;
	   current_object_ptr->Omovedx=dx;
	
 goto wingman_recharge;	//didn't find anything
}

//got something to attack
//if ((*ocb_ptr).object_list[it].control_use_9==1 && (*ocb_ptr).object_list[it].control_use_2==0)
if (current_object_ptr->control_use_9==1)
{
//add_to_text_display("NAME GOING FOR ATTACK!",1);
//    do_callsign_text("     GOING FOR ATTACK",it);	//callsign goes in first 4 chars of string
 wingman_attacking_text(it);
 current_object_ptr->control_use_9=2;
}

attacking_this=current_object_ptr->attack_what;

//calc wanted velocity
zex_distance=get_distance_to_from(attacking_this,it);
if (zex_distance<3000) current_object_ptr->attack_mode=0;	 //run
if (zex_distance>5000) current_object_ptr->attack_mode=1;	 //attack
//if ((*ocb_ptr).object_list[it].shield_value>100) (*ocb_ptr).object_list[it].attack_mode=1;
//else (*ocb_ptr).object_list[it].attack_mode=0;

if (current_object_ptr->attack_mode==0)
{
 if (facing_towards_target(attacking_this,it)==true)
 current_object_ptr->wanted_velocity=-105;
 else
 current_object_ptr->wanted_velocity=WM_MAX_SPEED;
}
else
 current_object_ptr->wanted_velocity=WM_MAX_SPEED;

//if (zex_distance >95000) (*ocb_ptr).object_list[it].wanted_velocity=0;	//if too far away, stop

//calc thrust from wanted - cu5=thrust
   if (current_object_ptr->wanted_velocity<current_object_ptr->current_velocity)
      current_object_ptr->current_velocity-=10;	//brakes
   else
      if (current_object_ptr->wanted_velocity>current_object_ptr->current_velocity)
      current_object_ptr->current_velocity+=4;	 //throttle

//directional and fire control   
 object_rotated_normal=current_object_ptr->object_normal_translated;

 if (current_object_ptr->attack_mode==1) turn_delta=ram_object(attacking_this,it,3);
 else
 turn_delta=turn_away_from_object(attacking_this,it,4);

//check cannon fire time
if (current_object_ptr->control_use_1<0 //fire timer
    || current_object_ptr->just_hit>0 ) //OR monster was just hit
 {
 
  if (current_object_ptr->shield_value>15 && zex_distance < 12500)	 //we have enough strength to fire and move
  {
  current_object_ptr->control_use_1=RangedRdm(7,20);	//set time to fire next cannon
//fire cannon is angle this frame is same as last frame (i.e. not turning)
   if (RangedRdm(1,10)>3) fire_laser_at(attacking_this,it,500);
   if (is_aimed_at(attacking_this,it)==true) fire_not_so_weeny_cannon(it); 
   }	//end of enough shields
   
   else	//not enough power left for control


   //go out of control
   if (current_object_ptr->shield_value<300)
   {
//generate ejection pod

   if (current_object_ptr->control_use_2==0)	 //havent ejected yet
   {
   current_object_ptr->control_use_2=1;	//have now!
   current_object_ptr->wanted_velocity=0;	 //lost engines
   current_object_ptr->Orotdz=1;	//lost engines
  
   //gen a message
    wingman_mayday_text(it);
//    fire_ejection_pod(it);
   	}	//end of haven't ejected yet
   }
   
  }	//end of time to fire 
 else	//not time to fire
  current_object_ptr->control_use_1-=1;	//dec time to fire

current_object_ptr->control_use_4=current_object_ptr->Orotz;	  //save this rot for next frame

  if (current_object_ptr->just_hit>0) 
  {
  current_object_ptr->just_hit-=20;	//clear down just hit for next frame
  }
goto wingman_recharge;
}	//end of FOLLOW_ME
 
//finally recharge shileds and check if we need to toggle the disabled flag
wingman_recharge:
if (current_object_ptr->shield_value<1000) current_object_ptr->shield_value++;

if (current_object_ptr->shield_value>500 && current_object_ptr->control_use_2==1)
   {
   current_object_ptr->control_use_2=0;	//all back to normal
    wingman_repaired_text(it);
   }
}





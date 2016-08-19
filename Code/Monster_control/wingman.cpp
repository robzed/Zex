// *************************************MODULE HEADER*********************************
// * FILENAME           : wingman.c
// * MODULE TITLE       : wingman.c
// * PROJECT            : Zex
// * DATE STARTED       : 16:15:21 on 15th June 1999
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

#define WM_MAX_SPEED 380

// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

int player_input_counter;
int wingmen_guarding_what_object;	//the object wingmen are guarding

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

/*
$Log
*/



extern	DynObjectsFixedSize *ocb_ptr;
extern int	dynamic_object_index,monsters_can_fire,thrust;


void handle_wingman1(int it)	//
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
extern _3D ship_rotated_normal; //from engine


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


if (do_avoid(it,2,1,150,150,5)) return;
//  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
//  (*ocb_ptr).object_list[it].attack_what=-1;
//  return;	  //did avoidance action
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
////    		    do_callsign_text("     Waiting...",it);	  //callsign goes in first 4 chars of string
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
 		return;
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

   return;
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
   

 
	current_object_ptr->wanted_velocity=thrust;	   //wanted velocity
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
	
 return;	//didn't find anything
}

//got something to attack
//if ((*ocb_ptr).object_list[it].control_use_9==1 && (*ocb_ptr).object_list[it].control_use_2==0)
if (current_object_ptr->control_use_9==1)
{
//add_to_text_display("NAME GOING FOR ATTACK!",1);
//    do_callsign_text("     GOING FOR ATTACK",it);	  //callsign goes in first 4 chars of string
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
  current_object_ptr->just_hit-=20;	 //clear down just hit for next frame
  }
  
//finally recharge shileds and check if we need to toggle the disabled flag
wingman_recharge:
if (current_object_ptr->shield_value<1000) current_object_ptr->shield_value++;

if (current_object_ptr->shield_value>500 && current_object_ptr->control_use_2==1)
   {
   current_object_ptr->control_use_2=0;	//all back to normal
    wingman_repaired_text(it);
   }
}


// ****************************************************************************** 

int last_mayday_text_id=0;

void wingman_mayday_text(int it)
{
int temp;

temp=last_mayday_text_id;

while (temp==last_mayday_text_id)
temp=RangedRdm(1,6);


   switch (temp)
        {
        case 1:
           do_callsign_text("     MAYDAY! MAYDAY! HAVE EXTENSIVE STRUCTURAL DAMAGE!",it,2);    
           break;

        case 2:
           do_callsign_text("     ANYONE! HELP! I'M LOSING SHIP CONTROL...",it,2);   
           break;

        case 3:
           do_callsign_text("     I'M UNDER HEAVY ATTACK... BREAKING UP!",it,2);   
           do_callsign_text("     I CAN'T GET AWAY FROM THESE SLIME BAGS...",it,2);   
           break;

        case 4:
           do_callsign_text("     MY PRIMARY SYSTEMS FAILING. NEED HELP NOW!",it,2);   
           break;

        case 5:
           do_callsign_text("     I'M IN SOME SERIOUS MESS HERE...HELP!",it,2);   
           break;

        case 6:
           do_callsign_text("     SHE'S BREAKING UP...I CAN'T HOLD IT... HELP!",it,2);   
           break;

       default:
           do_callsign_text("     BADLY DAMAGED!",it,2);
           break;
        }
 
last_mayday_text_id=temp;


}

int last_repaired_text_id=0;

void wingman_repaired_text(int it)
{
int temp;

temp=last_repaired_text_id;

while (temp==last_repaired_text_id)
temp=RangedRdm(1,6);


   switch (temp)
        {
        case 1:
           do_callsign_text("     RECOVERED FROM DAMAGE.",it,2);   
           break;

        case 2:
           do_callsign_text("     MANAGED TO FIX THE PROBLEM -- OPERATIONAL AGAIN",it,2);   
           break;

        case 3:
           do_callsign_text("     PHEW, THOUGHT I'D BIT IT THERE! RECOVERED",it,2);   
           break;

        case 4:
           do_callsign_text("     OPERATIONAL AGAIN.",it,2);   
           break;

        case 5:
           do_callsign_text("     SHIELDS BACK UP. CANCEL EMERGENCY!",it,2);   
           break;

        case 6:
           do_callsign_text("     FIXED IT! READY TO ROCK AGAIN",it,2);   
           break;

       default:
           do_callsign_text("     REPAIRED",it,2);
           break;
        }
 
last_repaired_text_id=temp;


}




int last_ret_text_id=0;

void wingman_returning_text(int it)
{
int temp;

if (get_distance_to_from(wingmen_guarding_what_object,it) < 30000) return;	//dont do it if close enough

temp=last_ret_text_id;


if (wingmen_guarding_what_object==0)
{	//these are if we are guarding zex
while (temp==last_ret_text_id)
temp=RangedRdm(1,16);

   switch (temp)
        {
        case 1:
           do_callsign_text("     RETURNING TO LEADER",it,2);   
           break;

        case 2:
           do_callsign_text("     COMING HOME",it,2);   
           break;

        case 3:
           do_callsign_text("     CHECKING IN",it,2);   
           break;

        case 4:
           do_callsign_text("     COMING RIGHT BACK ON YOUR TAIL BOSS",it,2);   
           break;

        case 5:
           do_callsign_text("     CHILLING OUT",it,2);   
           break;

        case 6:
           do_callsign_text("     COOLING OFF...",it,2);   
           break;

        case 7:
           do_callsign_text("     COOLING OFF AND COMING IN",it,2);   
           break;

        case 8:
           do_callsign_text("     JOINING YOU - HANG ON",it,2);   
           break;

        case 9:
           do_callsign_text("     WEAPONS INTO CHILL MODE.",it,2);   
           break;

       default:
           break;
        }
 

last_ret_text_id=temp;
}

else	//guarding something else

{
while (temp==last_ret_text_id)
temp=RangedRdm(1,7);

   switch (temp)
        {
        case 1:
           do_callsign_text("     RETURNING BACK TO MOTHERSHIP",it,2);   
           break;

        case 2:
           do_callsign_text("     STANDING GUARD",it,2);   
           break;

        case 3:
           do_callsign_text("     CHECKING IN",it,2);   
           break;

        case 4:
           do_callsign_text("     SNOOZING",it,2);   
           break;

        case 5:
           do_callsign_text("     CHILLING OUT",it,2);   
           break;

        case 6:
           do_callsign_text("     SNORE",it,2);   
           break;

       default:
           do_callsign_text("     JUST WAITING...",it,2);   
           break;
        }
 

last_ret_text_id=temp;
}


}


int last_ready_text_id=0;
void wingman_ready_text(int it)
{
int temp;


temp=last_ready_text_id;

while (temp==last_ready_text_id)
temp=RangedRdm(1,13);

   switch (temp)
        {
        case 1:
           do_callsign_text("     SIR! ALL SYSTEMS READY TO ROCK BABY, ERR, SIR!",it,2);   
           break;

        case 2:
           do_callsign_text("     PLEASED TO MEET YOU SIR! LET'S BOOGIE!",it,2);   
           break;

        case 3:
           do_callsign_text("     PLEASED TO MEET YOUR SIR! I WAS BORN FOR THIS MOMENT!",it,2);   
           break;

        case 4:
           do_callsign_text("     DO I NOT LIKE ALIENS. I'M READY BOSS",it,2);   
           break;

        case 5:
           do_callsign_text("     SO, THIS IS THE FAMOUS ZEX! PLEASED TO WORK WITH YOU SIR.",it,2);   
           break;

        case 6:
           do_callsign_text("     AN HONOUR TO SERVE WITH YOU SIR! A WALK IN THE PARK, DUDE",it,2);   
           break;

        case 7:
           do_callsign_text("     GOODDAY BOSS! THIS SHOULD BE FUN!",it,2);   
           break;

        case 8:
           do_callsign_text("     HOWDY SIR! LET'S DO IT!",it,2);   
           break;

        case 9:
           do_callsign_text("     NICE TO MEET YOU BOSS! NOW WHERE'S THAT CEEGAR GONE...",it,2);
           break;

        case 10:
           do_callsign_text("     SIR! ALL SYSTEMS OPERATIONAL. WEAPONS HUMMING.",it,2);
           break;

        case 11:
           do_callsign_text("     MISSION TIME! YEEHAH.",it,2);
           break;

        case 12:
           do_callsign_text("     READY SIR!.",it,2);
           break;



       default:
           do_callsign_text("     ALL WARMED UP AND READY SIR!",it,2);
           break;
        }
 
last_ready_text_id=temp;


}




int last_attack_text_id=0;

void wingman_attacking_text(int it)
{
int temp;
temp=last_attack_text_id;

while (temp==last_attack_text_id)
temp=RangedRdm(1,37);	//>number of case statements so we dont gen a message every single bloody time...

   switch (temp)
        {
        case 1:
           do_callsign_text("     HEY I FOUND ONE. DIE ALIEN SLIME!",it,2);   
           break;

        case 2:
           do_callsign_text("     I'M GOING FOR IT... YEEHAWWW!",it,2);   
           break;

        case 3:
           do_callsign_text("     ANOTHER DAY, ANOTHER R'MPARIAN TO KICK!",it,2);
           break;

        case 4:
           do_callsign_text("     MEET DOCTOR DEATH YOU ALIEN *#$&+%!",it,2);
           break;

        case 5:
           do_callsign_text("     I'M GOING TO KICK SOME SLIME BUTT.",it,2);   
           break;

        case 6:
           do_callsign_text("     I KNOW IT'S A CLICHE BUT... I'M GOING IN!",it,2);   
           break;

        case 7:
           do_callsign_text("     IS THIS A DAGGER I SEE BEFORE ME? OH NO, IT'S",it,2);   
           do_callsign_text("     A 45 MEGAWATT LASER. AND THAT'S AN ALIEN!",it,2);   
           break;

        case 8:
           do_callsign_text("     TIME FOR ALIEN PIE FOR DINNER...",it,2);   
           break;

        case 9:
           do_callsign_text("     DO WE LEAVE THEM ALONE OR VAPOURISE THEM?",it,2);   
           do_callsign_text("     I VOTE FOR THE TOAST!",it,2);   
           break;


        case 10:
           do_callsign_text("     ALIENS! GOOD. DON'T WANT MY LASER GETTING COLD!",it,2);   
           break;


        case 11:
           do_callsign_text("     WEAPONS ARMED... ALIEN IN SIGHTS.. GOODBYE!",it,2);   
           break;

        case 12:
           do_callsign_text("     ALIENS TO THE LEFT OF ME, ALIENS TO THE RIGHT...",it,2);   
           break;

        case 13:
           do_callsign_text("     FIGHT ON BRO!",it,2);   
           break;

        case 14:
           do_callsign_text("     DO I LIKE R'MPARIANS? ERR, NO.",it,2);   
           break;

        case 15:
           do_callsign_text("     DO I LIKE MY LASER? JUST A BIT! INCOMING ALIEN!",it,2);   
           break;

        case 16:
           do_callsign_text("     I CAN'T BELIEVE WE GET PAID FOR THIS!",it,2);   
           break;

        case 17:
           do_callsign_text("     HEY! GOT ANYTHING NASTY YOU WANT TO THROW AT ME 'MR BIG ALIEN'?.",it,2);   
           break;

        case 18:
           do_callsign_text("     INCOMING!",it,2);   
           break;

        case 19:
           do_callsign_text("     DO WE GET A MEDAL FOR THIS KIND OF PSYCHOTIC HEROISM?",it,2);   
           break;




       default:
//           do_callsign_text("     GOING FOR ATTACK",it);
           break;
        }


last_attack_text_id=temp;
}



//checks to see if the object that hit zex was or owned by a wingman and issues a radio message if so
unsigned int next_message_time=0;

void check_if_wingman_hit_zex()
{
int item_that_hit_zex;

//we only do one per second roughly...

if (LS_TickCount60()<next_message_time) return;
next_message_time=LS_TickCount60()+30;

item_that_hit_zex=(*ocb_ptr).object_list[zex].hit_by_slot;
if (item_that_hit_zex!=-1)
    {
     if ((*ocb_ptr).object_list[item_that_hit_zex].controller==WINGMAN_FOR_GAME)
     
     wingman_hit_text(item_that_hit_zex);
     else
     if ((*ocb_ptr).object_list[item_that_hit_zex].Oid=='MIS2')
       {
       int owner_slot;
       //get the slot of the owner of the cannon shell
        owner_slot=(*ocb_ptr).object_list[item_that_hit_zex].who_spawned_this_object;
         if ((*ocb_ptr).object_list[owner_slot].controller==WINGMAN_FOR_GAME)
            wingman_shot_text(owner_slot);

       }
    }
}


int last_hit_text_id=0;

void wingman_hit_text(int it)
{
int temp;

temp=last_hit_text_id;

while (temp==last_hit_text_id)
temp=RangedRdm(1,4);


   switch (temp)
        {
        case 1:
           do_callsign_text("     SORRY BOSS! DIDN'T MEAN TO HIT YOU.",it,2);    
           break;

        case 2:
           do_callsign_text("     EXCUSE ME!",it,2);   
           break;

        case 3:
           do_callsign_text("     WATCH OUT!!",it,2);   
           break;

       default:
           do_callsign_text("     HEY! WATCH WHERE YOU'RE DRIVING!",it,2);
           break;
        }
 
last_mayday_text_id=temp;


}


int last_bounced_text_id=0;
int last_bounced_id=0;
unsigned int last_bounced_message_time=0;

void wingman_bounced_text(int it)
{
int temp;
//We don't report a bounce message from the same slot if it comes in within a second of the last bounced 
//message
if (LS_TickCount60()>last_bounced_message_time)
{ 
 last_bounced_id=0;
 last_bounced_message_time=LS_TickCount60()+120;
}
if (last_bounced_id==it) return;


temp=last_bounced_text_id;

while (temp==last_bounced_text_id)
temp=RangedRdm(1,3);


   switch (temp)
        {
        case 1:
           do_callsign_text("     SHIIIIIIT!",it,2);    
           break;

        case 2:
           do_callsign_text("     BUGGER! I HIT THE MOTHERSHIP SHIELDS.",it,2);   
           break;

       default:
           do_callsign_text("     WAAAAAH!!!! I HATE THOSE SHIELDS!",it,2);
           break;
        }
 
last_bounced_text_id=temp;
last_bounced_id=it;

}

int last_hey_text_id=0;

void wingman_heyboss_text(int it)
{
int temp;

temp=last_hey_text_id;

while (temp==last_hey_text_id)
temp=RangedRdm(1,5);


   switch (temp)
        {
        case 1:
           do_callsign_text("     HEY BOSS! YOU GONNA DO ANY WORK TODAY????",it,2);    
           break;

        case 2:
           do_callsign_text("     NICE TO SEE OUR LEADER SO BUSY, ERRM,  LEADING...",it,2);   
           break;

        case 3:
           do_callsign_text("     DON'T WE HAVE A MISSION TO BE GETTING ON WITH??",it,2);   
           break;

        case 4:
           do_callsign_text("     QUIET. DON'T WANT TO WAKE OUR CAPTAIN.",it,2);   
           break;



       default:
           do_callsign_text("     I THINK THE BOSS HAS FALLEN ASLEEP.",it,2);
           break;
        }
 
last_hey_text_id=temp;

 }

int last_hey1_text_id=0;

void wingman_heyboss1_text(int it)
{
int temp;

temp=last_hey1_text_id;

while (temp==last_hey1_text_id)
temp=RangedRdm(1,3);


   switch (temp)
        {
        case 1:
           do_callsign_text("     HEY BOSS! YOU REALLY NEED TO BE DOING SOMETHING.",it,2);    
           break;

        case 2:
           do_callsign_text("     HAS OUR ESTEEMED LEADER DIED?",it,2);   
           break;

       default:
           do_callsign_text("     HELLO ZEX! YOU'RE SUPPOSED TO BE FLYING THAT THING!.",it,2);
           break;
        }
 
last_hey1_text_id=temp;

 }

int last_die_text_id=0;

void wingman_diealien_text(int it)
{
int temp;

temp=last_die_text_id;

while (temp==last_die_text_id)
temp=RangedRdm(1,11);


   switch (temp)
        {
        case 1:
           do_callsign_text("     YEAH! SUCK ON THAT RAMPY SLIME!",it,2);    
           break;

        case 2:
           do_callsign_text("     YEEHAAAAH! EARTH 1. ALIENS NIL.",it,2);   
           break;

        case 3:
           do_callsign_text("     DIE SLIME.",it,2);   
           do_callsign_text("     AWWW GOSH! I GOT GREEN STUFF ALL OVER MY WINDSHIELD.",it,2);
           break;

        case 4:
           do_callsign_text("     AND ANOTHER ONE BITES THE DUST.",it,2);   
           break;

        case 5:
           do_callsign_text("     GOODNIGHT PAL! HEY, HOW COME IT'S THE BOSS THAT GETS THE CREDITS?",it,2);   
           break;

        case 6:
           do_callsign_text("     SHREDDED!",it,2);   
           break;


        case 7:
           do_callsign_text("     YEEHAH! WHAT A SHOT! AM I GOOD OR AM I GOOD?",it,2);   
           break;

        case 8:
           do_callsign_text("     OUCH! MY CANNON IS SOOOO H O T!",it,2);   
           break;

        case 9:
           do_callsign_text("     HMMMM, IS THAT TWO OR THREE HUNDRED NOW?? HEEHEE.",it,2);   
           break;

        case 10:
           do_callsign_text("     YUP. YOU'RE DEAD MR ALIEN. IN FACT, WHERE'D IT GO???",it,2);   
           break;

        case 11:
           do_callsign_text("     STRIKE ONE! THIS IS SO EASY.",it,2);   
           break;



       default:
           do_callsign_text("     LIKE TAKING CANDY FROM A KID.",it,2);
           break;
        }
 
last_die_text_id=temp;

 }



int last_shot_text_id=0;

void wingman_shot_text(int it)
{
int temp;

temp=last_shot_text_id;

while (temp==last_shot_text_id)
temp=RangedRdm(1,4);


   switch (temp)
        {
        case 1:
           do_callsign_text("     SORRY BOSS! DIDN'T MEAN TO SHOOT YOU.",it,2);    
           break;

        case 2:
           do_callsign_text("     DOH! I SHOT THE BOSS! NO PROMOTION FOR ME THEN...",it,2);
           break;

        case 3:
           do_callsign_text("     SORRY I SHOT YOU CAP'N - JUST GOT A LITTLE EXCITED.",it,2);   
           break;

       default:
           do_callsign_text("     OOPS - I SHOT YOU - SORRY.",it,2);
           break;
        }
 
last_mayday_text_id=temp;


}



int last_diebb_text_id=0;

void wingman_diealienbyboss_text(int it)
{
int temp;

if (it==-1) return;

temp=last_diebb_text_id;

while (temp==last_diebb_text_id)
temp=RangedRdm(1,8);


   switch (temp)
        {
        case 1:
           do_callsign_text("     GET 'EM BOSS!!",it,2);    
           break;

        case 2:
           do_callsign_text("     WAY TO GO DUDE!",it,2);    
           break;

        case 3:
           do_callsign_text("     HEY! THE BOSS GOT ONE! I'LL BE...",it,2);
           break;


       default:
           break;
        }
 
last_diebb_text_id=temp;

 }


int last_listen_text_id=0;

void wingman_listen_to_them_text(int it)
{
int temp;

if (it==-1) return;

temp=last_listen_text_id;

while (temp==last_listen_text_id)
temp=RangedRdm(1,4);


   switch (temp)
        {
        case 1:
           do_callsign_text("     I THINK YOU SHOULD LISTEN TO THEM BOSS!",it,2);    
           break;

        case 2:
           do_callsign_text("     BETTER DO IT DUDE.",it,2);    
           break;

        case 3:
           do_callsign_text("     ARE YOU GOING TO IGNORE THAT ORDER BOSS?",it,2);    
           break;


       default:
           break;
        }
 
last_listen_text_id=temp;

 }
 
//searches dyn ocb's for first wingman and returns its slot else -1
int find_a_wingman()
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
int i,active_object_count;

i=active_object_count=0;
while (active_object_count<dynamic_object_index  && i<MAX_NUMBER_OF_DYN_OBJECTS)
{
  if ((*ocb_ptr).object_list[i].in_use==1)
  {
   if ((*ocb_ptr).object_list[i].Oid=='WMAN')
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


//nifty routine that searches dyn ocb's for nth wingman and returns its slot else -1
int find_nth_wingman(int n)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
int i,active_object_count;
int found_index;

found_index=i=active_object_count=0;
while (active_object_count<dynamic_object_index  && i<MAX_NUMBER_OF_DYN_OBJECTS)
{
  if ((*ocb_ptr).object_list[i].in_use==1)
  {
   if ((*ocb_ptr).object_list[i].Oid=='WMAN')
   {
     if (found_index==n)
      {
       return i;
       break;
      }
     found_index++;
     
   }
   active_object_count++;
 }
 i++;
}
return -1;

}



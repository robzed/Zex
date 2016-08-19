// *************************************MODULE HEADER*********************************
// * FILENAME           : level3.c
// * MODULE TITLE       : level3.c
// * PROJECT            : Zex
// * DATE STARTED       : 02:37:09 on 18th June 1999
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
#include "obj_control_misc_routines.h"
#include "buy_int.h"
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

//these are defined in level1
extern int done_monsters;	//used to control when we set off monsters
extern int LFrame_counter,total_frame_counter;	//timer
extern int next_ast_time;
extern int interval;	//how long before we gen a monster
extern mothership_nav_entry mothership_nav [10];
extern	DynObjectsFixedSize *ocb_ptr;
extern	float SinTable[360], CosTable[360];	//fancy see this here!!!
extern int var_a,tractor;
extern int wingmen_guarding_what_object;	//the object wingmen are guarding
extern int gRadarRange;
extern int    global_open_doors;
extern int dynamic_object_index;

extern int quicksave_available;
extern int training_complete;

int mothership_core_slot;
static int far_ahead_message_counter;
static int far_ahead_message_counter_dec_timer;	//when we dec the ahead message counter
static unsigned int  last_ahead_message_timer;	//when we last moaned about zex being too far ahead
static float ms_dx,ms_dy,ms_thrust;
static int done_stop_message, done_start_message;
static int done_ast_message;
static int mission_phase;

static unsigned int next_status_time;
static int asteroid_counter,check_for_asteroid_counter;
static int phase0_complete,phase1_complete,phase2_complete,phase3_complete,phase4_complete;
static unsigned int muzzle_tuner_message_timer;
static int number_of_monsters_generated_on_2,number_of_monsters_generated_on_1;
static int done_sarcasm;
static int do_asteroid_count=60;
static int done_phase_0_message,done_phase_2_message,done_phase_3_message;
static int mission_complete;
static int done_enable_monsters;
static int end_game,old_phase;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

void Level3();
void init_level3();
void GenRandomMonster();
void control_mothership();
void issue_status_message();
void generate_asteroid();
void generate_asteroid_to_left();
int count_asteroids();
void quicksave_level3();
void restore_quicksave_level3();
void reinit_level3();
int can_level3_quicksave();


void init_level3()
{
  add_to_text_display("FOLLOW GREEN RADAR VECTOR TO TARGET.",0);
  done_monsters=0;
  LFrame_counter=0;
  total_frame_counter=0;
  next_ast_time=1000;
  interval=RangedRdm(600,800);	//how long before we gen a monster
//Find MS02 (center of motehrship) so we can log it's dynamic OCB slot and hence the other two MS01's
  mothership_core_slot=find_dynamic_object_slot('MS02');

  if (mothership_core_slot==-1) report_error_c("Level2: Couldn't find mothership.","",-3);
  (*ocb_ptr).object_list[mothership_core_slot].is_armed=0;	//not armed on this level
  (*ocb_ptr).object_list[mothership_core_slot].shield_value=10000;
  far_ahead_message_counter=0;
  far_ahead_message_counter_dec_timer=1;
  last_ahead_message_timer=LS_TickCount60()+10*60;
  ms_thrust=0;
//  GenRandomNastyMonster();
//  GenRandomNastyMonster();
//  GenRandomNastyMonster();
//  GenRandomNastyMonster();
//  GenRandomMonster();
  done_start_message=done_stop_message=0;
  next_status_time=LS_TickCount60()+30*60;
  mission_phase=0;
  asteroid_counter=0; 
  done_ast_message=0;
  check_for_asteroid_counter=20;
  phase0_complete=phase1_complete=phase2_complete=phase3_complete=phase4_complete=0;
  muzzle_tuner_message_timer=0;
  number_of_monsters_generated_on_2=number_of_monsters_generated_on_1=0;
  done_sarcasm=0;
  do_asteroid_count=60;	//how often we count asteroids  
  done_phase_0_message=done_phase_2_message=done_phase_3_message=0;
  mission_complete=0;
//      add_to_text_display( ( char *) "SS01: GOOD AFTERNOON GENTLEMEN. IN THE NEXT FEW HOURS THE FATE",1);
//      add_to_text_display( ( char *) "SS01: OF THE HUMAN RACE AS WE KNOW IT COULD BE DECIDED. WE HAVE",1);
//      add_to_text_display( ( char *) "SS01: PICKED WHAT WE BELIEVE TO BE THE BEST AND NOW WE PLACE OUR",1);
//      add_to_text_display( ( char *) "SS01: FATE IN YOUR HANDS. GOOD LUCK AND MAY GOD BLESS US ALL.",1);
      add_to_text_display( ( char *) "SS01: WHEN YOU ARE READY CAPTAIN...",1);

   global_open_doors=1;
   done_enable_monsters=0;
   end_game=0;
   old_phase=0;

     if (gRadarRange==0)
      {
      gRadarRange=1;
      add_to_text_display( ( char *) "OBC> Radar auto-switched to long range mode",1);
      }

}

extern int score,credits;
extern _3D ship_rotated_normal; //from engine
extern kit_desc ship_items[ITEMS_COUNT];	//ships systems

void Level3()
{
extern int level_complete,can_do_next_level; 	  //game var - 0,1 or 2
extern float dest_x,dest_y;	//from level file
extern	DynObjectsFixedSize *ocb_ptr;
extern int launch_timer, have_ejected;
LSRect corral_box= { 28000,-28000,-28000,-2000 } ;

int i,zex_x;
int ship_x,ship_y,distance;
static int done_debug=0;
//anim_pb anim;

//Debug
	#if 0
	if (done_debug==0)	// - set up at Corral for debug
	{
		(*ocb_ptr).object_list[zex].Oworldx-=2000000;
		(*ocb_ptr).object_list[zex].Oworldy+=400000;

		(*ocb_ptr).object_list[mothership_core_slot].Oworldx-=2000000;
		(*ocb_ptr).object_list[mothership_core_slot+1].Oworldx-=2000000;
		(*ocb_ptr).object_list[mothership_core_slot+2].Oworldx-=2000000;

		(*ocb_ptr).object_list[mothership_core_slot].Oworldy+=400000;
		(*ocb_ptr).object_list[mothership_core_slot+1].Oworldy+=400000;
		(*ocb_ptr).object_list[mothership_core_slot+2].Oworldy+=400000;
		mission_phase=3;
		number_of_monsters_generated_on_2=35;
		phase1_complete=1; phase2_complete=1;
		done_debug=1;
	}
	#endif
//EndDebug

if (mission_phase>old_phase)
 {
  old_phase=mission_phase;
  if (mission_phase==1)
  add_to_text_display( ( char *) "MISSION PHASE 1 COMPLETED. GAME QUICKSAVED.",1);
  if (mission_phase==2)
  add_to_text_display( ( char *) "MISSION PHASE 2 COMPLETED. GAME QUICKSAVED.",1);
  if (mission_phase==3)
  add_to_text_display( ( char *) "MISSION PHASE 3 COMPLETED. GAME QUICKSAVED.",1);
  if (mission_phase==4)
  add_to_text_display( ( char *) "MISSION PHASE 4 COMPLETED. GAME QUICKSAVED.",1);
  
  quicksave_level3();
 }


if( have_ejected==1) //modded 261105 SB - if player has ejected, mothership stops so escape pod can dock.
{
	(*ocb_ptr).object_list[mothership_core_slot].Omovedx=0;
	(*ocb_ptr).object_list[mothership_core_slot].Omovedy=0;

	(*ocb_ptr).object_list[mothership_core_slot+1].Omovedx=0;
	(*ocb_ptr).object_list[mothership_core_slot+1].Omovedy=0;

	(*ocb_ptr).object_list[mothership_core_slot+2].Omovedx=0;
	(*ocb_ptr).object_list[mothership_core_slot+2].Omovedy=0;

	mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
	mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;
	return;	//in this case, the script exits here
}


if (mission_complete==1)	
{
level_complete=2;
dest_x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
dest_y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;
	insert_batcom_message("MISSION COMPLETE","                ");
   can_do_next_level=1;
//stop da mothership
    ms_thrust=0;
    
	(*ocb_ptr).object_list[mothership_core_slot].Omovedx=ms_dx*ms_thrust;
	(*ocb_ptr).object_list[mothership_core_slot].Omovedy=ms_dy*ms_thrust;

	(*ocb_ptr).object_list[mothership_core_slot+1].Omovedx=ms_dx*ms_thrust;
	(*ocb_ptr).object_list[mothership_core_slot+1].Omovedy=ms_dy*ms_thrust;

	(*ocb_ptr).object_list[mothership_core_slot+2].Omovedx=ms_dx*ms_thrust;
	(*ocb_ptr).object_list[mothership_core_slot+2].Omovedy=ms_dy*ms_thrust;

	mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
	mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;


return;
}


ship_x=static_cast<int>((*ocb_ptr).object_list[mothership_core_slot].Oworldx);
ship_y=static_cast<int>((*ocb_ptr).object_list[mothership_core_slot].Oworldy);
distance=get_distance_to_from_point(static_cast<int>(dest_x),static_cast<int>(dest_y),ship_x,ship_y);

if (distance<200000 && end_game==0) 
{
int wm;
     do_game_alert("");
      add_to_text_display( ( char *) "SS01: SUCCESS! WELL DONE CAPTAIN, WE HAVE ARRIVED.",1);
      add_to_text_display( ( char *) "SS01: PLEASE CLEAR THE CORRAL OF ALIEN SHIPS THEN",1);
      add_to_text_display( ( char *) "SS01: DOCK WITH US TO END THE MISSION.",1);
      
      wm=find_a_wingman();
          if (wm!=-1) 
           {

           if (RangedRdm(0,10)>5)
           do_callsign_text("     HEY BOSS! BETCHA GET PROMOTED FOR THIS!",wm,1);
           else
           {
           do_callsign_text("     WELL IF THEY DONT PROMOTE THE BOSS FOR THIS",wm,1);
           do_callsign_text("     I'LL EAT MY REMAINING CANNON SHELLS...",wm,1);
           }

           }
     
      wingmen_guarding_what_object=mothership_core_slot;

dest_x=-100000;
dest_y=0;

end_game=1;
}
//seee if we can do monsters - between 4 and 3 million is asteroids
zex_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
if (zex_x<400000 && phase3_complete==0 && number_of_monsters_generated_on_2>20) { mission_phase=3; phase2_complete=1; phase1_complete=1; }	  //nasty monsters
else
if (zex_x<1000000 && phase2_complete==0) { mission_phase=2; phase1_complete=1; }	//nasty monsters
else
if (zex_x<1500000 && phase1_complete==0) 
{
 
 mission_phase=1; phase0_complete=1; 
 interval=1; 
}    	     //asteroids
else
if (phase0_complete==0)
mission_phase=0;	//easy monsters


//var_a=mission_phase;

//check mothership is stil alive


if ((*ocb_ptr).object_list[mothership_core_slot].in_use==0)	//ooh, it's dead
{
int kill_bits;
//end game
      point_camera_at_coords((*ocb_ptr).object_list[mothership_core_slot].Oworldx, 
                             (*ocb_ptr).object_list[mothership_core_slot].Oworldy);
                             
      end_game_right_here_right_now(-1,"SS01 Destroyed!");	

      kill_bits = (find_dynamic_object_slot('MS01'));
      if (kill_bits!=-1) 
       {
        add_to_explode_list(kill_bits,5);
        add_to_kill_list(kill_bits);
       }

      kill_bits = (find_dynamic_object_slot('MS01'));
      if (kill_bits!=-1) 
       {
        add_to_explode_list(kill_bits,3);
        add_to_kill_list(kill_bits);
       }

return;
}

control_mothership();	//follow Zex and do radio messages etc. In this file.

//we also maintain a timer that gens a new monster every n frames
//if player has more than 25% shields
//and asteroids every n frames
LFrame_counter++;
total_frame_counter++;


//random asteroids
//if (total_frame_counter>next_ast_time+5 && mission_phase==2)
//{
// total_frame_counter=0;
// if (ship_items[KNAV_COMPUTER].status==1)
// {
//  if (RangedRdm(1,10)>5)
//  add_to_text_display("WARNING: MAGNETIC ANOMALIES DETECTED",1);
//  else
//  add_to_text_display("WARNING: ABNORMAL NATURAL MAGNETIC FLUX VARIATIONS",1);
// }
// 
// next_ast_time=RangedRdm(8000,25000);	//number of frames to next asteroid set
//}
//
//if (total_frame_counter>next_ast_time && total_frame_counter<next_ast_time+4 && mission_phase>1) 
//{
//
////gen an asteroid
//generate_asteroid();
//
//}
//

//gen a monster at random
if (LFrame_counter>interval)
{
  
  LFrame_counter=0;
  
  if (mission_phase==0)
  {
    if (ship_items[KSHIELDS].value1>50)
    {
     number_of_monsters_generated_on_1++;
     
     if (number_of_monsters_generated_on_1>35)
     {
     }
     else
     if (number_of_monsters_generated_on_1>8)
     {
     GenM2();
     GenM2();
     }
     else
     {
     GenM1WithRoll();
     GenM1WithRoll();
     GenM1WithRoll();
     }
      interval=400+RangedRdm(100,200);
    
      
     
    }	//end of shields>150
    
  }	//end of mission_phase==0
  else
  if (mission_phase==1)	//asteroids?
  {
  //count the asteroids
  
  do_asteroid_count--;
  
  if (do_asteroid_count<0)
  {
  int n=0;
  n=count_asteroids();
  if (n!=0)
     {
      if (n==1)      add_to_text_display( ( char *) "SS01: ONE ASTEROID TO GO...",1);
      else
      if (n==2)      add_to_text_display( ( char *) "SS01: TWO ASTEROIDS LEFT",1);
      else
      if (n==4)      add_to_text_display( ( char *) "SS01: FOUR ASTEROIDS TO DESTROY",1);
      else
      if (n==8)      add_to_text_display( ( char *) "SS01: EIGHT ASTEROIDS LEFT",1);
      else
      if (n==12)      add_to_text_display( ( char *) "SS01: WE COUNT TWELVE ASTEROIDS REMAINING",1);
      else
      if (n==15)      add_to_text_display( ( char *) "SS01: WELL DONE CAPTAIN. KEEP BLASTING THEM...",1);
      
     }
  do_asteroid_count=180;
  }
  //first a bit of sarcasm from wingmen...
     if (tractor==1 && (*ocb_ptr).object_list[0].Oid=='EC01' && done_sarcasm==0)
       {
        int wm;
        
          done_sarcasm=1;
          wm=find_a_wingman();
          if (wm!=-1) 
           {
           if (RangedRdm(0,10)>5)
           do_callsign_text("     THERE'S THE BOSS GETTING A TOW HOME. SNIGGER..",wm,1);
           else
           do_callsign_text("     IS THAT OUR TOP GUN IN THAT EJECTION POD. COUGH.",wm,1);
           
           }
       }
       
     if(ship_items[KASTEROID_ADDITION].status<1 && ZexTickCount()>muzzle_tuner_message_timer)
        {
        int wm;
          muzzle_tuner_message_timer=ZexTickCount()+1000;
          wm=find_a_wingman();
          if (wm!=-1) 
           {
           if (RangedRdm(0,10)>5)
           do_callsign_text("     GET A MUZZLE TUNER FOR THOSE ASTEROIDS, BOSS",wm,1);
           else
           do_callsign_text("     YOU NEED TO BUY A MUZZLE TUNER FOR THOSE ASTEROIDS, MON CAPITAN",wm,1);
           
           }
        }


  if (done_ast_message==0)
     {
//     reset_dlp_text_display();
     add_to_text_display( ( char *) "SS01: WE DETECT AN ASTEROID FIELD AHEAD AND FEAR DAMAGE. PROCEED FORWARD",1);
     add_to_text_display( ( char *) "SS01: AND DESTROY ALL THE ASTEROIDS. WE WILL WAIT HERE.",1);
     add_to_text_display( ( char *) "SS01: GOOD LUCK. SS01 OUT.",1);
     done_ast_message=1;
     do_game_alert("");
     
     wingmen_guarding_what_object=mothership_core_slot;	//the object wingmen are guarding
     //switch to long range radar if not already
     if (gRadarRange==0)
      {
      gRadarRange=1;
      add_to_text_display( ( char *) "OBC> Radar switched to long range mode",1);
      }
     }	     //end of done_ast_message

//if player does not have the asteroid addition, get winfman to issue a message
     
//generate some asteroids over a period of time
     if (asteroid_counter<16)
     {
     generate_asteroid_to_left();
     asteroid_counter++;
     }
     interval=1;     
     LFrame_counter=0;
   //now see if all asteroids are clear
     if (phase1_complete==0)
     {
        if (find_dynamic_object_slot('AST1')==-1)
       {
        add_to_text_display( ( char *) "SS01: THANK YOU CAPTAIN. WE ARE MOVING TO YOUR POSITION TO CONTINUE WITH THE MISSION",1);
        add_to_text_display( ( char *) "SS01: TAKE A SHORT BREAK. WE'LL BE THERE IN A FEW SECONDS.",1);
        phase1_complete=1;
        last_ahead_message_timer=LS_TickCount60()+(60*45);
        wingmen_guarding_what_object=0;	//the object wingmen are guarding

       }
     }
     
     
   }	   //end of phase==1
   
  else	//gen max 15 monsters
  if (mission_phase==2)
  {
    if (done_phase_2_message==0)
     {
      add_to_text_display( ( char *) "SS01: WARNING! LONG RANGE SCANNERS DETECT MAJOR INCOMING ENEMY.",1);
      done_phase_2_message=1;
      run_level_aux_music(4);	//play second moddette
      wingmen_guarding_what_object=0;
     }
    if (ship_items[KSHIELDS].value1>50)
    {
     number_of_monsters_generated_on_2++;
     
     if (number_of_monsters_generated_on_2>20)
     {
     GenRandomMonster();
     }
     else
     if (number_of_monsters_generated_on_2>18)
     {
     GenRandomNastyMonster();
     //GenRandomNastyMonster();
     }
     else
     {
     GenRandomMonster();
     //GenRandomMonster();
     }
     interval=100+RangedRdm(100,200);
     
     if (score>29000 && RangedRdm(10,100)>40) 
     {
     number_of_monsters_generated_on_2++;
     interval+=200;
     GenRandomMonster();
     }
  
     if (score>112000 && RangedRdm(10,100)>40) 
     {
     number_of_monsters_generated_on_2++;
     interval+=300;
     GenRandomMonster();
     }
  
     if (score>200000 && RangedRdm(10,100)>40)
     {
     interval+=300;
     GenRandomMonster();
     }
     
     if (score>250000 && RangedRdm(10,100)>60)
     {
     interval+=300;
     GenRandomMonster();
     }
     if (score>300000 && RangedRdm(10,100)>80)
     {
     interval+=100;
     GenRandomNastyMonster();
     }
    }	//end of shields>150
    
  }	//end of mission_phase==2


  else	//gen max 15 monsters
  if (mission_phase==3 && done_enable_monsters==0)
  {
  interval=5;
  
  if (point_in_rect(static_cast<int>((*ocb_ptr).object_list[0].Oworldx),
                  static_cast<int>((*ocb_ptr).object_list[0].Oworldy),
                  corral_box)) 
                  {
                  int current_object,i,number_to_examine;
                  done_enable_monsters=1;
                  level_complete=1;
                  mission_complete=1;
					//training_complete=1;
//find all monsters with (*ocb_ptr).object_list[current_object].no_trivial_rejection==3
//and make a 2
                   number_to_examine=dynamic_object_index;	//we copy this in case we kill any objects
                   current_object=0;

                   for (i=0;i<number_to_examine;i++)
                   {
	            while((*ocb_ptr).object_list[current_object].in_use==0)
	            current_object++;	//empty object
	           
	           if ((*ocb_ptr).object_list[current_object].no_trivial_rejection==3)
	           (*ocb_ptr).object_list[current_object].no_trivial_rejection=2;
	           
	           current_object++;
	           }
	           
                  }

  //end game
//     GenRealNastyMonster();
//     interval=500+RangedRdm(100,200);

  }	//end of mission_phase==3


    
 }	//end of LFrame_counter>interval



}


//I Love my job...
void control_mothership()
{
int zex_distance,angle_to_zex;
int zex_x,zex_y,ms_x,ms_y;
extern int zex_alive;


if (zex_alive==0 ) return;


 if ((mission_phase==1 && phase1_complete==0) || end_game==1)	//just stop
    {
    if (ms_thrust>1) ms_thrust-=0.5;
    else
    if (ms_thrust<-1) ms_thrust+=0.5;
    else
    ms_thrust=0;
    
    	(*ocb_ptr).object_list[mothership_core_slot].Omovedx=ms_dx*ms_thrust;
	(*ocb_ptr).object_list[mothership_core_slot].Omovedy=ms_dy*ms_thrust;

	(*ocb_ptr).object_list[mothership_core_slot+1].Omovedx=ms_dx*ms_thrust;
	(*ocb_ptr).object_list[mothership_core_slot+1].Omovedy=ms_dy*ms_thrust;

	(*ocb_ptr).object_list[mothership_core_slot+2].Omovedx=ms_dx*ms_thrust;
	(*ocb_ptr).object_list[mothership_core_slot+2].Omovedy=ms_dy*ms_thrust;

	mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
	mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;

    return;
    }
    
    
//first, see if it's time to issue a status report - we do this every 30 seconds or so
if (LS_TickCount60()>next_status_time)
   {
   issue_status_message();
   next_status_time=LS_TickCount60()+60*30;
   
   }
//now move the mothership to get close to Zex
//get angle to zex
 angle_to_zex=get_angle_toobject_fromobject(0,mothership_core_slot);
 ms_dx=COS(angle_to_zex);
 ms_dy=SIN(angle_to_zex);
 

zex_distance=get_distance_to_from(zex,mothership_core_slot);

if (zex_distance>46000)	//move to zex
{
 if (ms_thrust<240) ms_thrust+=0.25;	 //slow acceleration
 if (done_start_message==0)
  {
  add_to_text_display( ( char *) "SS01: ENGAGING THRUST.",1);
  done_start_message=1;
  done_stop_message=0;
  }
}
else
 if (ms_thrust>0) 
 {
  if (done_stop_message==0)
  {
   done_start_message=0;
   add_to_text_display( ( char *) "SS01: DIS-ENGAGING THRUST.",1);
   done_stop_message=1;
  }
 if (tractor==1) 
 ms_thrust-=2.00;	//fast decceleration if tractoring    
 else
 ms_thrust-=0.25;	 //slow deceleration

 if (ms_thrust<0) ms_thrust=0;
 }

//now calc move in x and y
(*ocb_ptr).object_list[mothership_core_slot].Omovedx=ms_dx*ms_thrust;
(*ocb_ptr).object_list[mothership_core_slot].Omovedy=ms_dy*ms_thrust;

(*ocb_ptr).object_list[mothership_core_slot+1].Omovedx=ms_dx*ms_thrust;
(*ocb_ptr).object_list[mothership_core_slot+1].Omovedy=ms_dy*ms_thrust;

(*ocb_ptr).object_list[mothership_core_slot+2].Omovedx=ms_dx*ms_thrust;
(*ocb_ptr).object_list[mothership_core_slot+2].Omovedy=ms_dy*ms_thrust;

mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;

//radio zex if too far infront
//the far-ahead_message_counter is incremented each time we have to send a message, and decremented
//every 200 calls.
// if(far_ahead_message_counter_dec_timer!=0)
// {
// far_ahead_message_counter_dec_timer--;
//  if(far_ahead_message_counter_dec_timer==0)
//   {
//      far_ahead_message_counter_dec_timer=400;
//      if (far_ahead_message_counter>0) far_ahead_message_counter--;
//   }
// }

//the last_ahead_message_timer is set to LS_TickCount60+30*60 on a message.
//We dont allow another message until the time exceeds it then we reload it
 if (LS_TickCount60()>last_ahead_message_timer)
 {
  if (zex_distance>120000)
   { 
//     if (far_ahead_message_counter==0)
     switch (far_ahead_message_counter)
     {
       case 0:
       {
       if (RangedRdm(0,10)>5)
       add_to_text_display( ( char *) "SS01: YOU ARE TOO FAR AHEAD. WAIT!",1);
       else
       add_to_text_display( ( char *) "SS01: WAIT FOR US TO CATCH UP",1);
       break;
       }
       case 1:
       {
       add_to_text_display( ( char *) "SS01: WE REPEAT, YOU ARE TOO FAR AHEAD. WAIT FOR US!",1);
       break;
       }
       case 2:
       {
       add_to_text_display( ( char *) "SS01: RETURN TO VICINITY OF SS01. THIS IS A DIRECT ORDER CAPTAIN!",1);
       break;
       }

       case 3:
       {
       int the_wingman;
       add_to_text_display( ( char *) "SS01: ZEX RETURN TO VICINITY OF SS01. YOU FACE BEING FINED FOR NON-COMPLIANCE.",1);
       the_wingman=find_a_wingman();
       if (the_wingman!=-1)
       wingman_listen_to_them_text(the_wingman);
       break;
       }

       case 4:
       {
       if (credits>500)
       {
        add_to_text_display( ( char *) "SS01: ZEX RETURN TO VICINITY OF SS01. YOU HAVE BEEN FINED 500 CREDITS.",1);
        credits-=500;
       }
       else
       {
        add_to_text_display( ( char *) "SS01: ZEX RETURN TO VICINITY OF SS01 IMMEDIATELY. YOU HAVE RECEIVED A REPRIMAND.",1);
        if (score>1000)
           score-=1000;
       }
       break;
       }
       
       
       default:
       {
       if (credits>1000)
       {
        add_to_text_display( ( char *) "SS01: ZEX RETURN TO VICINITY OF SS01. YOU HAVE BEEN FINED 1000 CREDITS.",1);
        credits-=1000;
       }
       else
       {
        add_to_text_display( ( char *) "SS01: RETURN TO VICINITY OF SS01 IMMEDIATELY. SEVERE REPRIMAND.",1);
        if (score>2000)
           score-=2000;
       }
       
       break;
       }

     }
     far_ahead_message_counter++;
   }
   else	//zex is within range of the mothership
   {
    far_ahead_message_counter=0;
   }
  last_ahead_message_timer=LS_TickCount60()+30*60;
 }
 
 

}

char status_start[] = "SS01: WE ARE ";
char from_clark[] = " KM'S FROM CLARKE CORRAL. OUR SHIELDS ARE AT ";
char percent[] = "%";

void issue_status_message()
{
extern float dest_x,dest_y;

char status_message[256];
char temp_string[24];
int sm_index=0;
int source_index;
char the_char;
int distance;
int ship_x,ship_y,shields;

//copy start to status message
        the_char=255;
        source_index=0;
        while (the_char!=0)
        {
        the_char=status_start[source_index];
        status_message[sm_index]=the_char;
        sm_index++;
        source_index++;
        }
	
	sm_index--;	//back past terminator char
//print he distance into temp-string
ship_x=static_cast<int>((*ocb_ptr).object_list[mothership_core_slot].Oworldx);
ship_y=static_cast<int>((*ocb_ptr).object_list[mothership_core_slot].Oworldy);
distance=get_distance_to_from_point(static_cast<int>(dest_x),static_cast<int>(dest_y),ship_x,ship_y);
if (distance<300000) 
{
return;	//stop issuing status messages
}

distance*=16;

ZexNumToString(distance,(unsigned char*) temp_string);
temp_string[temp_string[0]+1]=0;	//quick p to c hack

//copy temp_string to status message
        the_char=255;
        source_index=1;	//pascal length at start
        while (the_char!=0)
        {
        the_char=temp_string[source_index];
        status_message[sm_index]=the_char;
        sm_index++;
        source_index++;
        }
	
	sm_index--;	//back past terminator char

//copy clark to status message
        the_char=255;
        source_index=0;
        while (the_char!=0)
        {
        the_char=from_clark[source_index];
        status_message[sm_index]=the_char;
        sm_index++;
        source_index++;
        }
	
	sm_index--;	//back past terminator char

//calc shield strength
        shields=(*ocb_ptr).object_list[mothership_core_slot].shield_value/10;	//max is 1000
        

	ZexNumToString(shields,(unsigned char*) temp_string);
	temp_string[temp_string[0]+1]=0;	//quick p to c hack

//copy temp_string to status message
        the_char=255;
        source_index=1;	//pascal length at start
        while (the_char!=0)
        {
        the_char=temp_string[source_index];
        status_message[sm_index]=the_char;
        sm_index++;
        source_index++;
        }
	
	sm_index--;	//back past terminator char

//copy clark to status message
        the_char=255;
        source_index=0;
        while (the_char!=0)
        {
        the_char=percent[source_index];
        status_message[sm_index]=the_char;
        sm_index++;
        source_index++;
        }
	
	sm_index--;	//back past terminator char



//finally, terminate
        status_message[sm_index]=0;
      
        add_to_text_display( ( char *) status_message,1);

}

void generate_asteroid()
{
int dest_ocb,static_slot;
float ast_x,ast_y;
load_dyn_object_pb the_params;

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
           clear_pb(&the_params);

   	ast_x=(*ocb_ptr).object_list[0].Oworldx+40000;
//	ast_x=ast_x+(40000*ship_rotated_normal.x);
	   ast_y=(*ocb_ptr).object_list[0].Oworldy;
	   ast_y=ast_y+(40000*ship_rotated_normal.y);

	the_params.world_x=ast_x+RangedRdm(0,20000)-10000;
	the_params.world_y=ast_y+(RangedRdm(0,20000)-10000);	
	the_params.world_z=MONSTER_Z;
	the_params.Omovedx=RangedRdm(0,120)-140;
	the_params.delta_rot_y=RangedRdm(1,10)-5;
	the_params.delta_rot_z=RangedRdm(1,10)-5;
	the_params.controller_ref=asteroid1;
	the_params.shield_value=31170;
	the_params.mass=11000;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=scenary_object;
     static_slot=get_static_object_slot('AST1');	//go fetch fido (the object)
	 if (static_slot==-1) 
	     report_error_c("Level1: get_static_object_slot failed.","",-3);
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
    }
}




void generate_asteroid_to_left()
{
int dest_ocb,static_slot;
float ast_x,ast_y;
load_dyn_object_pb the_params;

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
        clear_pb(&the_params);

   	ast_x=(*ocb_ptr).object_list[0].Oworldx-(160000+RangedRdm(100,30000));
//	ast_x=ast_x+(40000*ship_rotated_normal.x);
	   ast_y=(*ocb_ptr).object_list[0].Oworldy+(RangedRdm(0,40000)*3)-60000;
//	ast_y=ast_y+(40000*ship_rotated_normal.y);

	the_params.world_x=ast_x;
	the_params.world_y=ast_y;	
	the_params.world_z=MONSTER_Z;
	the_params.Omovedx=5;
	the_params.delta_rot_y=RangedRdm(1,10)-5;
	the_params.delta_rot_z=RangedRdm(1,10)-5;
	the_params.controller_ref=persistent_asteroid;
	the_params.shield_value=31170;
     the_params.has_roll=0;
	the_params.mass=11000;
	 the_params.object_category=scenary_object;
     static_slot=get_static_object_slot('AST1');	//go fetch fido (the object)
	 if (static_slot==-1) 
	     report_error_c("Level1: get_static_object_slot failed.","",-3);
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
    }
}

int count_asteroids()
{
int active_object_counter;
int object_counter;
int ast_count;

object_counter=active_object_counter=ast_count=0;
while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("triv_rejection looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
   if ((*ocb_ptr).object_list[object_counter].Oid=='AST1')ast_count++;
   
   active_object_counter++;
   }
 object_counter++;
  
 }
return ast_count;
}


//data for quicksave
int wingman1_sign;	//so we can restore them on quickload
int wingman2_sign;
int wingman_count;
int level_phase;
int qs_hull_strength;
int qs_score;
int qs_credits;
int qs_missile_flip;
int qs_time;
kit_desc qs_ship_items[ITEMS_COUNT];

extern int hull_strength,missile_flip,stop_time,start_time,accumulated_game_time;

void quicksave_level3()
{
extern kit_desc ship_items[ITEMS_COUNT];
int temp,i;
//count number of wingmen alive
//max of two
quicksave_available=1;

temp=find_nth_wingman(0);
wingman1_sign=-1;
if (temp!=-1) 
  {
  wingman_count++;
  wingman1_sign=(*ocb_ptr).object_list[temp].callsign;
  }

temp=find_nth_wingman(1);
wingman2_sign=-1;
if (temp!=-1) 
  {
  wingman_count++;
  wingman2_sign=(*ocb_ptr).object_list[temp].callsign;
  }
//save phase
level_phase=mission_phase;

//save ships items

 qs_hull_strength=hull_strength;
 qs_score=score;
 qs_credits=credits;
 qs_missile_flip=missile_flip;
 
 
qs_time=(stop_time-start_time)+accumulated_game_time;	//little error here in that on load we are rewound to the start of the phase


 for (i=0;i<ITEMS_COUNT;i++)
 qs_ship_items[i]=ship_items[i];

//qs_ship_items=ship_items;
// for (i=0;i<ITEMS_COUNT;i++)
// ship_items[i]=my_save_file.save_items[i];
//}

}


void restore_quicksave_level3()
{
int temp,i;
extern int hull_damage;

mission_phase=level_phase;
if (mission_phase==0)
      {
      //set Zex's x
      (*ocb_ptr).object_list[0].Oworldx=2200000;
      (*ocb_ptr).object_list[0].Oworldy=-400000;
      //if wingman1_sign set wman1
            temp=find_nth_wingman(0);
      if (wingman1_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=2228000;
            (*ocb_ptr).object_list[temp].Oworldy=-40000;
            (*ocb_ptr).object_list[temp].callsign=wingman1_sign;

            } 
            else kill(temp);     

            temp=find_nth_wingman(1);
      if (wingman2_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=2228000;
            (*ocb_ptr).object_list[temp].Oworldy=-40000;
            (*ocb_ptr).object_list[temp].callsign=wingman2_sign;

            }      
            else kill(temp);

      //set mothership
        temp=find_dynamic_object_slot('MS02');
	(*ocb_ptr).object_list[temp].Oworldx=2202500;
	(*ocb_ptr).object_list[temp].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+1].Oworldx=2200000;
	(*ocb_ptr).object_list[temp+1].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+2].Oworldx=2205100;
	(*ocb_ptr).object_list[temp+2].Oworldy=-375000;
      }	//end of mission phase=0
else

if (mission_phase==1)
      {
      phase0_complete=1;
      //set Zex's x
      (*ocb_ptr).object_list[0].Oworldx=1499998;
      (*ocb_ptr).object_list[0].Oworldy=-400000;
      //if wingman1_sign set wman1
            temp=find_nth_wingman(0);
      if (wingman1_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=1528000;
            (*ocb_ptr).object_list[temp].Oworldy=40000;
            (*ocb_ptr).object_list[temp].callsign=wingman1_sign;

            } 
            else kill(temp);     

            temp=find_nth_wingman(1);
      if (wingman2_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=1528000;
            (*ocb_ptr).object_list[temp].Oworldy=-40000;
            (*ocb_ptr).object_list[temp].callsign=wingman2_sign;

            }      
            else kill(temp);

      //set mothership
        temp=find_dynamic_object_slot('MS02');
	(*ocb_ptr).object_list[temp].Oworldx=1502500;
	(*ocb_ptr).object_list[temp].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+1].Oworldx=1500000;
	(*ocb_ptr).object_list[temp+1].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+2].Oworldx=1505100;
	(*ocb_ptr).object_list[temp+2].Oworldy=-375000;
	
	mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
	mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;

      }	//end of mission phase=1

else

if (mission_phase==2)
      {
      phase0_complete=1;phase1_complete=1;
      //set Zex's x
      (*ocb_ptr).object_list[0].Oworldx=999998;
      (*ocb_ptr).object_list[0].Oworldy=-400000;
      //if wingman1_sign set wman1
            temp=find_nth_wingman(0);
      if (wingman1_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=1028000;
            (*ocb_ptr).object_list[temp].Oworldy=40000;
            (*ocb_ptr).object_list[temp].callsign=wingman1_sign;

            } 
            else kill(temp);     

            temp=find_nth_wingman(1);
      if (wingman2_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=1028000;
            (*ocb_ptr).object_list[temp].Oworldy=-40000;
            (*ocb_ptr).object_list[temp].callsign=wingman2_sign;

            }      
            else kill(temp);

      //set mothership
        temp=find_dynamic_object_slot('MS02');
	(*ocb_ptr).object_list[temp].Oworldx=1002500;
	(*ocb_ptr).object_list[temp].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+1].Oworldx=1000000;
	(*ocb_ptr).object_list[temp+1].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+2].Oworldx=1005100;
	(*ocb_ptr).object_list[temp+2].Oworldy=-375000;
	
	mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
	mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;

      }	//end of mission phase=2

else

if (mission_phase==3)
      {
      phase0_complete=1;phase1_complete=1;phase2_complete=1;
      //set Zex's x
      (*ocb_ptr).object_list[0].Oworldx=499998;
      (*ocb_ptr).object_list[0].Oworldy=-400000;
      //if wingman1_sign set wman1
            temp=find_nth_wingman(0);
      if (wingman1_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=528000;
            (*ocb_ptr).object_list[temp].Oworldy=40000;
            (*ocb_ptr).object_list[temp].callsign=wingman1_sign;

            } 
            else kill(temp);     

            temp=find_nth_wingman(1);
      if (wingman2_sign!=-1)
            {
            (*ocb_ptr).object_list[temp].Oworldx=528000;
            (*ocb_ptr).object_list[temp].Oworldy=-40000;
            (*ocb_ptr).object_list[temp].callsign=wingman2_sign;

            }      
            else kill(temp);

      //set mothership
        temp=find_dynamic_object_slot('MS02');
	(*ocb_ptr).object_list[temp].Oworldx=502500;
	(*ocb_ptr).object_list[temp].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+1].Oworldx=500000;
	(*ocb_ptr).object_list[temp+1].Oworldy=-375000;

	(*ocb_ptr).object_list[temp+2].Oworldx=505100;
	(*ocb_ptr).object_list[temp+2].Oworldy=-375000;
	
	mothership_nav[0].x=(*ocb_ptr).object_list[mothership_core_slot].Oworldx;
	mothership_nav[0].y=(*ocb_ptr).object_list[mothership_core_slot].Oworldy;

      }	//end of mission phase=2


     //reset score
      score=  qs_score;
      credits=qs_credits;
      missile_flip = qs_missile_flip;
     hull_strength = qs_hull_strength;
     hull_damage=0;

 for (i=0;i<ITEMS_COUNT;i++)
 ship_items[i]=qs_ship_items[i];

}


void reinit_level3()
{
if (mission_phase==1 || end_game==1)
wingmen_guarding_what_object=mothership_core_slot;
else
wingmen_guarding_what_object=0;
}


int can_level3_quicksave()
{
if (mission_phase>0) return 1;
else return 0;
}


// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 1
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 13:52:26 on 17th August 1999
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
#include "transporters.h"



// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

extern int global_control_monsters;

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


//cu7=who spawned us
void handle_alien_shuttle(int it)	 //
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
//if (do_avoid(it,0)==true) return;	//did avoidance action
//}
if (global_control_monsters==0) return;


  if (do_avoid(it,0,0,200,0,4)==true) 
  {
//if we were attacking something and are about to hit something clear out attack_what as we HAVE to avoid
  object_ptr->attack_what=-1;
  }

if (object_ptr->control_use_1<0 //fire timer
    || object_ptr->just_hit>0 ) //OR monster was just hit
 {
 



   //go out of control
 if (object_ptr->shield_value<50)
 {
//generate ejection pod

   if (object_ptr->control_use_2==0)	 //havent ejected yet
   {
    object_ptr->Orotdx=1;
    object_ptr->Orotdy=1;
    object_ptr->Orotdz=1;
    object_ptr->Omovedx=-3;
    object_ptr->Omovedy=1;	 //shipp loses control and rotates
   	
   	object_ptr->control_use_2=1;	 //have now!
   
    fire_ejection_pod(it);
   	}	//end of haven't ejected yet
   }
   
  }	//end of time to fire 
 else	//not time to check for eject
  object_ptr->control_use_1-=1;	//dec time to check for eject

object_ptr->control_use_4=object_ptr->Orotz;	  //save this rot for next frame

  if (object_ptr->just_hit>0) 
  {
  object_ptr->just_hit-=20;	//clear down just hit for next frame
  }


}



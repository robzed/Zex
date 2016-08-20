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
// * FILENAME           : monster_control_misc.c
// * MODULE TITLE       : monster_control_misc.c
// * PROJECT            : Zex
// * DATE STARTED       : 14:41:09 on 22nd June 1999
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

#include <string.h>

//oldmac//#include <fp.h>
#include	"3d_structures_def.h"
#include "monster_control_misc.h"
#include	"objects_controls.h"	//controller definitions
#include	"zex_misc.h"
#include "angular_stuff.h"
#include "dlp.h"
#include "instrum_int.h"
#include	"load_objects.h"
#include	"object_control_top.h"
#include "compile_zmap.h"
#include "obj_control_misc_routines.h"
#include "game_defs.h"
// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
extern	float SinTable[360], CosTable[360];
extern	DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
extern _3D ship_rotated_normal; //from engine

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


int generate_sensible_rank();

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
$Header: /home/ls_cvs/Zex2005/Code/Monster_control/monster_control_misc.cpp,v 1.20 2006/06/20 21:58:52 stu Exp $
$Log: monster_control_misc.cpp,v $
Revision 1.20  2006/06/20 21:58:52  stu
Ops, booked in debug code

Revision 1.19  2006/06/20 21:53:38  stu
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

Revision 1.18  2006/05/30 21:37:16  stu
Zex2005: Code: No more greenies in alpha quad

Revision 1.17  2006/04/09 01:10:00  stu
Zex2005: Code: Human and Alien shuttles (traders), Faster Falcons, changes to when Pythons and Excalibers appear, bit more chat from Falcons when waiting to dock (if they have to wait a while).

Revision 1.16  2006/04/05 00:12:15  stu
Zex2005: Code: Updated monster stats and added Python and Excalibur

Revision 1.15  2006/01/22 12:36:16  rob
Zex2005: Modify 4 digit callsign print to be safe even if callsign is not ASCII.

Revision 1.14  2006/01/14 03:41:00  stu
Zex2005: Code: Change in the distribution of wingmen ranks to be easier to set.

Revision 1.13  2006/01/14 03:07:35  stu
Zex2005: Code: wingmen now have a rank and their cannons, lasers and shields are a function of that rank.

Revision 1.12  2006/01/08 03:07:00  stu
Zex2005: Code: Removed all remaining warnings. There should now be no compile warnings

Revision 1.11  2005/12/20 20:40:31  stu
Zex2005: Code: All dynamic object search loops modified such that if they don't pull out in time, at least they are protected.

Revision 1.10  2005/12/14 23:25:40  rob
Zex2005: Got rid of other warnings in source.

Revision 1.9  2005/12/13 23:55:33  rob
Zex2005: Got rid of the last few int->float conversion warnings.

Revision 1.8  2005/12/02 19:24:09  stu
Zex2005: Code: simple_play_sound now honours the game volume; had a go at some of the blank lines; these were possibly callsigns.

Revision 1.7  2005/10/10 21:30:43  rob
Zex2005: Added newline at end to stop compiler whinging.

Revision 1.5  2005/06/27 19:55:56  rob
Zex2005: Files changed so that use of a string constant (aka a string literal) is only used by const char * routines - this way we can alter the compiler to warning about access by char *. Real reason for altering this is because we had a case of altering these string constants (to insert call signs) which caused a bus error. This has been corrected by copying the string before modification.

Revision 1.4  2005/05/26 19:47:56  rob
Zex2005: Basic build complete.

Revision 1.3  2005/05/02 10:30:38  rob
Zex2005: More Updates (code)

Revision 1.2  2005/04/23 20:12:04  rob
ZEX: First set of changes for SDL build. (remainder of files)

Revision 1.27  1999/09/27 14:49:28  stu
Fixes NPC's bust on 260999

Revision 1.26  1999/09/26 18:34:41  stu
260999

Revision 1.25  1999/09/19 05:38:52  stu
0.93 as posted

Revision 1.24  1999/09/14 15:07:47  stu
14/9/99

Revision 1.23  1999/09/12 00:56:05  stu
120999

Revision 1.22  1999/08/30 22:22:52  stu
300899

Revision 1.21  1999/08/20 09:57:27  stu
20 Aug 99

Revision 1.20  1999/08/03 18:25:55  stu
*** empty log message ***

Revision 1.19  1999/07/26 22:22:22  stu
Pre test 3 270799

Revision 1.18  1999/07/13 04:07:47  stu
Clean up 130799

Revision 1.17  1999/07/09 06:03:08  stu
Clean up 09-7-99

Revision 1.16  1999/07/04 00:21:23  stu
Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86

Revision 1.15  1999/06/25 01:13:29  stu
Mods to accomodate level 2

Revision 1.14  1999/06/23 04:54:10  stu
More fixes related to motherships and weapons - still ongoing. Still docking sometimes dodgy.

Revision 1.13  1999/06/23 00:51:49  stu
Changes for new lighting control

Revision 1.12  1999/06/22 15:31:55  stu
Level 2 work

Revision 1.11  1999/06/20 05:40:07  stu
Changes for Level 2

Revision 1.10  1999/06/16 00:25:58  stu
*** empty log message ***

Revision 1.9  1999/06/15 19:34:16  stu
1. Big attack if player just sits there with no thrust.
2. Improvements to wingmen messages for when they shoot something.

Revision 1.8  1999/06/05 19:54:53  stu
Mods to find_something_to_aim_at to improve results - don't attack spacecows for example!

Revision 1.7  1999/06/05 17:25:57  stu
Lurkers clear up commit.

Revision 1.6  1999/06/05 01:01:34  stu
Many minor object control problems fixed. See todo

Revision 1.5  1999/05/21 03:31:22  stu
1. New screen driver.
2. VBL Driven music fade and play

*/
//monster_control_misc.c
//SB 11/4/99


void GenM1()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
   int bearing;


   bearing=(*ocb_ptr).object_list[0].Orotz;
   bearing=bearing+RangedRdm(0,20);
   if (bearing>359) bearing-=360;
        clear_pb(&the_params);
  
	the_params.world_x=(COS(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(SIN(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;
      static_slot=get_static_object_slot('MON2');	 //go fetch fido (the object)
      if (static_slot==-1) 
      return;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
	     the_params.controller_ref=monster2;
    	
	     the_params.shield_value=370;
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=baddie_object;

     the_params.who_spawned_this_object=-1;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
        }
}

void GenM1WithRoll()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
   int bearing;


   bearing=(*ocb_ptr).object_list[0].Orotz;
   bearing=bearing+RangedRdm(0,20);
   if (bearing>359) bearing-=360;
   
        clear_pb(&the_params);
  
	the_params.world_x=(COS(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(SIN(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;
      static_slot=get_static_object_slot('MON2');	 //go fetch fido (the object)
      if (static_slot==-1) 
      return;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
	     the_params.controller_ref=monster2;
     the_params.has_roll=1;
    	
	     the_params.shield_value=470;
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=baddie_object;

     the_params.who_spawned_this_object=-1;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
        }
}

void GenM2()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
   int bearing;


   bearing=(*ocb_ptr).object_list[0].Orotz;
   bearing=bearing+RangedRdm(0,20);
   if (bearing>359) bearing-=360;
   
        clear_pb(&the_params);
  
	the_params.world_x=(COS(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(SIN(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;
             static_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
             if (static_slot==-1) 
	         return;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
             {
	           the_params.controller_ref=monster3;
	           the_params.shield_value=570;
	           the_params.mass=100;
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;

	           the_params.laser_cat=1;
     the_params.who_spawned_this_object=-1;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
              }

}


void GenPhoenix()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
   int bearing;


   bearing=(*ocb_ptr).object_list[0].Orotz;
   bearing=bearing+RangedRdm(0,20);
   if (bearing>359) bearing-=360;
   
        clear_pb(&the_params);
  
	the_params.world_x=(COS(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(SIN(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;
             static_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
             if (static_slot==-1) 
	         return;	//not found

    anim.object_id[0]='MN31';
    anim.object_id[1]='MN32';
    anim.object_id[2]='MN33';
    anim.object_id[3]='MN34';
    anim.object_id[4]=0;
    anim.object_id[5]=0;

             
             static_slot=get_static_object_slot('MN31');	//go fetch fido (the object)
             if (static_slot==-1) 
	         return;	//not found

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
	the_params.controller_ref=monster4;
	the_params.shield_value=750;
	the_params.mass=100;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=baddie_object;

	 the_params.laser_cat=1;
     the_params.who_spawned_this_object=-1;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=1;
    load_animated_object (&anim,4,3,1,1,&the_params,-1);

//	load_dyn_object(static_slot,dest_ocb,&the_params);	//the object, the position (-1=next free)
    }
}



void GenRandomMonster()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
int monst_select;
int k,j;
int done_phoenix=0;

  j=1;
  if (current_level>1) j=2;
  if (current_level>6) j=3;

for (k=0;k<j;k++)
  {
   int bearing;
   //calc bearing of monster
   bearing=(*ocb_ptr).object_list[0].Orotz;
   bearing=bearing+RangedRdm(0,20);
   if (bearing>359) bearing-=360;
        clear_pb(&the_params);
   
   monst_select=RangedRdm(0,4);
  
	the_params.world_x=(COS(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(SIN(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;
 	 switch (monst_select)
	     {

       case 0:
      {
      static_slot=get_static_object_slot('MON2');	 //go fetch fido (the object)
      if (static_slot==-1) 
      break;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
	     the_params.controller_ref=monster2;
     the_params.has_roll=0;
    	
	     the_params.shield_value=170;
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=baddie_object;

     the_params.who_spawned_this_object=-1;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
        }
                   break;
      }
            case 1:
            {
             static_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
     the_params.has_roll=0;
             {
	           the_params.controller_ref=monster3;
	           the_params.shield_value=290;
	           the_params.mass=100;
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;

	           the_params.laser_cat=1;
     the_params.who_spawned_this_object=-1;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
              }
             break;
           }

            case 2:	//phoenix
            {
            if (RangedRdm(0,10)>6) break;
            if (ship_items[KLASER].modifier<2 || done_phoenix==1) break; //need level 2 laser before we get phoenix
            done_phoenix=1;
             k=50;	 //only gen one monster
    anim.object_id[0]='MN31';
    anim.object_id[1]='MN32';
    anim.object_id[2]='MN33';
    anim.object_id[3]='MN34';
    anim.object_id[4]=0;
    anim.object_id[5]=0;

            if (current_level<2) break;
             
             static_slot=get_static_object_slot('MN31');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
     the_params.has_roll=0;
  if (dest_ocb!=-1)
   {
	the_params.controller_ref=monster4;
	the_params.shield_value=1650;
	the_params.mass=100;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=baddie_object;

	 the_params.laser_cat=1;
     the_params.who_spawned_this_object=-1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=1;
         load_animated_object (&anim,4,3,1,1,&the_params,-1);
 
//	load_dyn_object(static_slot,dest_ocb,&the_params);	//the object, the position (-1=next free)
    }
 

                   break;
    }

            case 3:	//space cow
            {
            k=50;	//only gen one monster
            if (RangedRdm(0,10)>4) break;
    anim.object_id[0]='MN41';
    anim.object_id[1]='MN42';
    anim.object_id[2]='MN43';
    anim.object_id[3]='MN44';
    anim.object_id[4]='MN43';
    anim.object_id[5]='MN42';
           
 static_slot=get_static_object_slot('MN41');	//go fetch fido (the object)
 if (static_slot==-1) 
	break;	//not found

     the_params.has_roll=0;
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
	the_params.controller_ref=space_cow;
	the_params.shield_value=13070;
	the_params.mass=50;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=2;

     the_params.who_spawned_this_object=-1;
    load_animated_object (&anim,4,3,1,1,&the_params,-1);

//	load_dyn_object(static_slot,dest_ocb,&the_params);	//the object, the position (-1=next free)
    }
 

                   break;
    }

            default :
                   break;
    }
 } //end of for k

}






void GenRandomNastyMonster()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
int monst_select;
int k,j;
int done_phoenix=0;

  j=1;
  if (current_level>2) j=2;
  if (current_level>6) j=3;

for (k=0;k<j;k++)
  {
   int bearing;
   //calc bearing of monster
   bearing=(*ocb_ptr).object_list[0].Orotz;
   bearing=bearing+RangedRdm(0,30);
   if (bearing>359) bearing-=360;
        clear_pb(&the_params);

  monst_select=RangedRdm(0,3);

	the_params.world_x=(COS(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(SIN(bearing)*50000)+(*ocb_ptr).object_list[0].Oworldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;

 switch (monst_select)
 {
         case 0:	//phoenix
           {
         anim.object_id[0]='MN31';
         anim.object_id[1]='MN32';
         anim.object_id[2]='MN33';
         anim.object_id[3]='MN34';
         anim.object_id[4]=0;
         anim.object_id[5]=0;
     
                  
                  static_slot=get_static_object_slot('MN31');	     //go fetch fido (the object)
                  if (static_slot==-1) 
     	              break;	//not found
     
          dest_ocb=find_vacant_dynamic_object_slot();	     //find a slot for this object
          if (dest_ocb!=-1)
            {
       	the_params.controller_ref=monster4;
       	the_params.shield_value=1550;
       	the_params.mass=100;
       	the_params.no_trivial_rejection=1;
       	the_params.object_category=baddie_object;
       
       	the_params.laser_cat=1;
            the_params.who_spawned_this_object=-1;
       	the_params.missile_cat=1;
       	the_params.number_of_missiles=1;
           load_animated_object (&anim,4,3,1,1,&the_params,-1);
       
       //	load_dyn_object(static_slot,dest_ocb,&the_params);	//the object, the position (-1=next free)
           }
      
     
                        break;      
         }


            case 1:	//python
            {
             static_slot=get_static_object_slot('MON6');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
             the_params.has_roll=1;
             {
	           the_params.controller_ref=monster6;
	           the_params.shield_value=1700;
	           the_params.mass=100;
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;

	           the_params.laser_cat=1;
	           the_params.cannon_cat=2;
	           the_params.missile_cat=1;
	           the_params.number_of_missiles=2;
                   the_params.who_spawned_this_object=-1;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
              }
             break;
           }

            case 2:	//Warper
            {
             static_slot=get_static_object_slot('WARP');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
             the_params.has_roll=1;
             {
	           the_params.controller_ref=warper;
	           the_params.shield_value=2400;
	           the_params.mass=100;
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;

	           the_params.laser_cat=1;
	           the_params.cannon_cat=2;
	           the_params.missile_cat=1;
	           the_params.number_of_missiles=2;
                   the_params.who_spawned_this_object=-1;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
              }
             break;
           }



            default :
                   break;
    }
 } //end of for k

}


int do_avoid(int it, int with_roll, int can_reverse, int fwd_vel, int rev_vel, int max_angle)
{
int active_object_counter,i,zangle,dist,dx,dy,look_out_dist;
//search ocb's finding first object closer than 3000 units
//if found take avoiding action
register ZObject * object_ptr;
//search ocb's finding first object closer than 3000 units
//if found take avoiding action
    object_ptr=&ocb_ptr->object_list[it];

active_object_counter=0;

i = 0;

while (active_object_counter<dynamic_object_index  && i<MAX_NUMBER_OF_DYN_OBJECTS)	//process all "in use" objects
 {

  if ((*ocb_ptr).object_list[i].in_use==1) //if object is alive
    {    
     //now check if coll_det is set
      if((*ocb_ptr).object_list[i].colltype!=0 && (*ocb_ptr).object_list[i].mass>40)	//change 170699 - we only avoid things over a certain mass
       {
        //calc look out distance from speed
        look_out_dist=ABS(object_ptr->wanted_velocity)*20+2000;
//        look_out_dist=8000;
        //get distance to
        dist=get_distance_to_from_all_types(i,it);
        if (dist<look_out_dist)
            {
             //take avoiding action - swerve direction is used to maintain a persistent direction during swerve
                if (object_ptr->swerve_direction==0)	 //not swerving
                {
                int adir;
                adir=left_or_right(object_ptr->Orotz,(*ocb_ptr).object_list[i].Orotz);
		if (adir<0) adir=-max_angle;
		else
		if (adir>=0) adir=max_angle;
		object_ptr->swerve_direction=adir;
//                (*ocb_ptr).object_list[it].swerve_direction=turn_left_or_right(i,it,ABS((*ocb_ptr).object_list[it].wanted_velocity)); //+-ve angular change
                }
                zangle=object_ptr->Orotz;
                zangle+=object_ptr->swerve_direction;
                
                if (zangle>359) zangle-=360;
                if (zangle<0) zangle+=360;
                object_ptr->Orotz=zangle;
                
                //override any thrust set in controller which may be slow or negative
                //calc thrust from wanted - cu5=thrust - we accelerate away at a standard rate of 100 to avoid a circle
                if (can_reverse==1)
                {
                if (facing_towards_target(i,it))
                object_ptr->wanted_velocity=-rev_vel;
                else
                object_ptr->wanted_velocity=fwd_vel;
                }
                else
                object_ptr->wanted_velocity=fwd_vel;
                
                                
                if (object_ptr->wanted_velocity<object_ptr->current_velocity)
                object_ptr->current_velocity-=9;	//brakes
                else
                if (object_ptr->wanted_velocity>object_ptr->current_velocity)
                object_ptr->current_velocity+=6;	//throttle

                //calc thrust components   
                dy= static_cast<int>(object_ptr->object_normal_translated.y*object_ptr->current_velocity);	//current velocity
                dx= static_cast<int>(object_ptr->object_normal_translated.x*object_ptr->current_velocity);
                	   dy+= static_cast<int>(object_ptr->extern_movedy);
	                   dx+= static_cast<int>(object_ptr->extern_movedx);

                object_ptr->Omovedy=dy;
                object_ptr->Omovedx=dx;

             return true;
            }
        else //no need to swerve so reset
        object_ptr->swerve_direction=0;
       }	//end of colltype!=0
    active_object_counter++;  
    }  //end of in_use
i++;           
 }	//end of while

return false;
}

//same as above but only avoids scenary
int do_avoid_scenary(int it, int with_roll, int can_reverse, int fwd_vel, int rev_vel, int max_angle)
{
int active_object_counter,i,zangle,dist,dx,dy,look_out_dist;
register ZObject * object_ptr;
//search ocb's finding first object closer than 3000 units
//if found take avoiding action
    object_ptr=&ocb_ptr->object_list[it];
active_object_counter=0;

i = 0;

while (active_object_counter<dynamic_object_index  && i<MAX_NUMBER_OF_DYN_OBJECTS)	//process all "in use" objects
 {

  if ((*ocb_ptr).object_list[i].in_use==1) //if object is alive
    {    
     //now check if coll_det is set
      if((*ocb_ptr).object_list[i].colltype!=0 && (*ocb_ptr).object_list[i].mass>40 && (*ocb_ptr).object_list[i].object_category==SCENARY)	//change 170699 - we only avoid things over a certain mass
       {
        //calc look out distance from speed
        look_out_dist=ABS(object_ptr->wanted_velocity)*10+2000;
//        look_out_dist=8000;
        //get distance to
        dist=get_distance_to_from_all_types(i,it);
        if (dist<look_out_dist)
            {
             //take avoiding action - swerve direction is used to maintain a persistent direction during swerve
                if (object_ptr->swerve_direction==0)	 //not swerving
                {
                int adir;
                adir=left_or_right(object_ptr->Orotz,(*ocb_ptr).object_list[i].Orotz);
		if (adir<0) adir=-max_angle;
		else
		if (adir>=0) adir=max_angle;
		object_ptr->swerve_direction=adir;
//                (*ocb_ptr).object_list[it].swerve_direction=turn_left_or_right(i,it,ABS((*ocb_ptr).object_list[it].wanted_velocity)); //+-ve angular change
                }
                zangle=object_ptr->Orotz;
                zangle+=object_ptr->swerve_direction;
                
                if (zangle>359) zangle-=360;
                if (zangle<0) zangle+=360;
                object_ptr->Orotz=zangle;
                
                //override any thrust set in controller which may be slow or negative
                //calc thrust from wanted - cu5=thrust - we accelerate away at a standard rate of 100 to avoid a circle
                if (can_reverse==1)
                {
                if (facing_towards_target(i,it))
                object_ptr->wanted_velocity=-rev_vel;
                else
                object_ptr->wanted_velocity=fwd_vel;
                }
                else
                object_ptr->wanted_velocity=fwd_vel;
                
                                
                if (object_ptr->wanted_velocity<object_ptr->current_velocity)
                object_ptr->current_velocity-=9;	//brakes
                else
                if (object_ptr->wanted_velocity>object_ptr->current_velocity)
                object_ptr->current_velocity+=6;	//throttle

                //calc thrust components   
                dy= static_cast<int>(object_ptr->object_normal_translated.y*object_ptr->current_velocity);	//current velocity
                dx= static_cast<int>(object_ptr->object_normal_translated.x*object_ptr->current_velocity);
                	   dy+= static_cast<int>(object_ptr->extern_movedy);
	                   dx+= static_cast<int>(object_ptr->extern_movedx);

                object_ptr->Omovedy=dy;
                object_ptr->Omovedx=dx;

             return true;
            }
        else //no need to swerve so reset
        object_ptr->swerve_direction=0;
       }	//end of colltype!=0
    active_object_counter++;  
    }  //end of in_use
i++;           
 }	//end of while

return false;
}


int turn_left_or_right(int object_to_avoid, int it, int velocity)
{
//if diff between angle-to and heading is > 180 turn left (+ve)
//else right (-ve)
int ang_to,heading,diff;

ang_to=get_angle_toobject_fromobject(object_to_avoid,it);
heading=(*ocb_ptr).object_list[it].Orotz;
if (heading>ang_to) diff=heading-ang_to;
else diff=ang_to-heading;
if (diff>180) 
{
 if (velocity<40) return 1;
 else
 return velocity/40;
}
else 
 {
 if (velocity>-40) return -1;
 else
 return -(velocity/40);
 }
}

//this routine finds the antithesis of this objects category and fills in what_slot
//if what_slot is filled it checks if object is still in use and if not finds something else to attack
//returns 0 if all ok, else -1 if it couldn't find something to attack
//the thing found to attack will be the closest object 
//can_do_escape_caps tells the routine whether it can target escape capsules
//vers 2 - return first target in range - NOT THE CLOSEST
//this is ditched because they always pick on Zex!

//int find_something_to_attack(int it, int can_do_escape_caps, int max_distance)
//{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern int dynamic_object_index;
//int attack_cat;
//int attack_slots[10];	//array of possible slots to attack
//int attack_slots_index=0;	//index into above array
//int active_object_counter,object_counter;
//int closest_distance, closest_slot, i, dt;
//int attacking_this,id;
//
//int debug_loop_count;
//register ZObject * object_ptr;
//
//    object_ptr=&ocb_ptr->object_list[it];
//
////debug_loop_count=0;
//attacking_this=object_ptr->attack_what;
//if (attacking_this!=-1)
// {
// //introduce a little randomness to reselect another target
// if ((*ocb_ptr).object_list[attacking_this].in_use==1 && RangedRdm(1,1000)<990) return 0;	//the object is alive
// }
// 
////find something to attack
//if (object_ptr->object_category==baddie_object) attack_cat=zex_object;
//else attack_cat=baddie_object;
//
////find up to 10 objects then pick one at random
//active_object_counter=object_counter=0;
//while (active_object_counter<dynamic_object_index)
// {
////Debug code
////  debug_loop_count++;
////  if (debug_loop_count>10000) DebugStr("find_something_to_attack looped");
////end debug code
//  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
//   {
//   if ((*ocb_ptr).object_list[object_counter].object_category==attack_cat )
//      {
//      id=(*ocb_ptr).object_list[object_counter].Oid;
////we dont target the following id's
//      if (id=='DST2' || id=='MIS2' || id=='MIS1' || id=='LAS1' || id=='EX01' ||
//      id=='MN41' || id=='MN42' || id=='MN43' || id=='MN44')
//       {
//       }
//      else
//      if (id=='EC01' && can_do_escape_caps==0)
//      {
//      }
//      else
//       {
//          dt=get_distance_to_from(object_counter,it);
//          if (dt<max_distance)
//          {
//           if (object_counter==0)
//              {
//              (if RangedRdm(0,100)<50)
//              {
//            }
//            else
//            {
//           object_ptr->attack_what=object_counter;
//           return 0;
//            }
//           }
//              
//         }
//       }
//      }
//   active_object_counter++;
//   }
//  object_counter++;
// }	  //end of while
// object_ptr->attack_what=-1;
// return -1;
//}

//old slow code

int find_something_to_attack(int it, int can_do_escape_caps, int max_distance)
{
extern	      DynObjectsFixedSize *ocb_ptr;
extern int dynamic_object_index;
int attack_cat;
int attack_slots[10];	//array of possible slots to attack
int attack_slots_index=0;	//index into above array
int active_object_counter,object_counter;
int closest_distance, closest_slot=0, i, dt;
int attacking_this,id;

int debug_loop_count;

//debug_loop_count=0;
attacking_this=(*ocb_ptr).object_list[it].attack_what;
//if (attacking_this!=-1)
// {
// //introduce a little randomness to reselect another target
//// if ((*ocb_ptr).object_list[attacking_this].in_use==1 && RangedRdm(1,1000)<990) 
////  {
////   if (get_distance_to_from(attacking_this,it)<max_distance)
////  return 0;	//the object is alive
////  }
// }
 
//find something to attack
if ((*ocb_ptr).object_list[it].object_category==baddie_object) attack_cat=zex_object;
else attack_cat=baddie_object;

//find up to 10 objects then pick one at random
active_object_counter=object_counter=attack_slots_index=0;
while (active_object_counter<dynamic_object_index && attack_slots_index<9  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("find_something_to_attack looped");
//end debug code
  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
   if ((*ocb_ptr).object_list[object_counter].object_category==attack_cat &&
   (*ocb_ptr).object_list[object_counter].crashable!=0)
      {
      id=(*ocb_ptr).object_list[object_counter].Oid;
//we dont target the following id's
      if (id=='DST2' || id=='MIS2' || id=='MIS1' || id=='LAS1' || id=='EX01' ||
          id=='MN41' || id=='MN42' || id=='MN43' || id=='MN44' 
          || id=='CAR1' || id=='CAR2' ||id=='CAR3' ||id=='CAR4'
          || id=='MS10' || id=='MS11' || id=='MS01' || id=='MS02' || id=='MS12'
          )
       {
       }
      else
      if (id=='EC01' && can_do_escape_caps==0)
      {
      }
      else
       {
        attack_slots[attack_slots_index]=object_counter;
        attack_slots_index++;
       }
      }
   active_object_counter++;
   }
  object_counter++;
 }	//end of while
//now pick the nearest one

if (attack_slots_index==0)
{
 (*ocb_ptr).object_list[it].attack_what=-1;
 return -1;	//did not find anything to attack
} 

closest_distance=10000000;
for (i=0;i<attack_slots_index;i++)
  {
  dt=get_distance_to_from(attack_slots[i],it);
  if (dt<closest_distance) 
    {
    closest_distance=dt;
    closest_slot=i;
    }
  }


if (closest_distance<max_distance)
{
 (*ocb_ptr).object_list[it].attack_what=attack_slots[closest_slot];
 //if this is a mothership and it has targetted a trader then dont as they are now independent
 if ((*ocb_ptr).object_list[it].controller==GAME_MOTHERSHIP && 
 (*ocb_ptr).object_list[attack_slots[closest_slot]].controller==NPC_CONTROL &&
 (*ocb_ptr).object_list[attack_slots[closest_slot]].obj_class==NPC_CLASS_TRAVELLER)
 {
	(*ocb_ptr).object_list[it].attack_what=-1;
	return -1;
 }
 
 return 0;
}
else
{
 (*ocb_ptr).object_list[it].attack_what=-1;
 return -1;
}
}



// length of line=(sqrt(a^2+b^2)

int get_distance_to_from(int obj_to, int obj_from)
{
int zex_x,zex_y,obj_x,obj_y,dist_x,dist_y;
int temp;
//for Newtons sqrt method
float R,x_0,x_1,x_2,x_3,x_4;

if (obj_to==obj_from) return 10000000;	//hmmm...
//if ((*ocb_ptr).object_list[obj_to].Oid==(*ocb_ptr).object_list[obj_from].Oid) return 1000000;

zex_x= static_cast<int>((*ocb_ptr).object_list[obj_to].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[obj_to].Oworldy);
obj_x= static_cast<int>((*ocb_ptr).object_list[obj_from].Oworldx);
obj_y= static_cast<int>((*ocb_ptr).object_list[obj_from].Oworldy);

if (zex_x<obj_x)
dist_x=obj_x-zex_x;
else
dist_x=zex_x-obj_x;

if (zex_y<obj_y)
dist_y=obj_y-zex_y;
else
dist_y=zex_y-obj_y;

//if (dist_x>100000) return 10000000;
//if (dist_y>100000) return 10000000;
//Newtons method for sqrt
//	   R=dist_x*dist_x+dist_y*dist_y;
//	   x_0=dist_x+dist_y;
//         x_1 = 0.5*(x_0 + R/x_0);
//         x_2 = 0.5*(x_1 + R/x_1);
//         x_3 = 0.5*(x_2 + R/x_2);
//         x_4 = 0.5*(x_3 + R/x_3);

return dist_x+dist_y;	//just a quick frig
//temp = sqrt(dist_x*dist_x+dist_y*dist_y);	//just a quick frig
//return temp;

}


int get_distance_to_from_all_types(int obj_to, int obj_from)
{
int zex_x,zex_y,obj_x,obj_y,dist_x,dist_y;

if (obj_to==obj_from) return 10000000;	//hmmm...
zex_x= static_cast<int>((*ocb_ptr).object_list[obj_to].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[obj_to].Oworldy);
obj_x= static_cast<int>((*ocb_ptr).object_list[obj_from].Oworldx);
obj_y= static_cast<int>((*ocb_ptr).object_list[obj_from].Oworldy);

if (zex_x<obj_x)
dist_x=obj_x-zex_x;
else
dist_x=zex_x-obj_x;

if (zex_y<obj_y)
dist_y=obj_y-zex_y;
else
dist_y=zex_y-obj_y;

return dist_x+dist_y;	//just a quick frig

}


int get_distance_to_from_point(int zex_x, int zex_y, int obj_x, int obj_y)
{

int dist_x, dist_y;
if (zex_x<obj_x)
dist_x=obj_x-zex_x;
else
dist_x=zex_x-obj_x;

if (zex_y<obj_y)
dist_y=obj_y-zex_y;
else
dist_y=zex_y-obj_y;

return dist_x+dist_y;	//just a quick frig

}


int get_distance_to_zex(int it)
{
int zex_x,zex_y,obj_x,obj_y,dist_x,dist_y;

zex_x= static_cast<int>((*ocb_ptr).object_list[zex].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[zex].Oworldy);
obj_x= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
obj_y= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

if (zex_x<obj_x)
dist_x=obj_x-zex_x;
else
dist_x=zex_x-obj_x;

if (zex_y<obj_y)
dist_y=obj_y-zex_y;
else
dist_y=zex_y-obj_y;

return dist_x+dist_y;	//just a quick frig

}


void process_extern_movements(int it)
{
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


//   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	  //current velocity
//   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
//   dx+=(*ocb_ptr).object_list[it].extern_movedx;
//   dy+=(*ocb_ptr).object_list[it].extern_movedy;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy+=(*ocb_ptr).object_list[it].extern_movedy;
   (*ocb_ptr).object_list[it].Omovedx+=(*ocb_ptr).object_list[it].extern_movedx;
}


//еееееееееееееееееееееееееееееееееее
void ram_zex(int it, int angular_velocity)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern	float SinTable[360], CosTable[360];


//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
int slot,obj_x_ang,v_diff,h_diff,h_direction,obj_z_ang;
float shot_x,shot_y,shot_z,mturn,dx,dy;
int zex_wx,zex_wy,obj_wx,obj_wy,cosapb,cosamb;
int zangle,wangle,delta_rot,d1,d2;

//try to ram zex
//calculate angle (Zrot) between monster and zex,
  zex_wx= static_cast<int>((*ocb_ptr).object_list[zex].Oworldx);
  zex_wy= static_cast<int>((*ocb_ptr).object_list[zex].Oworldy);

  obj_wx= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
  obj_wy= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

  zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);

   
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
   wangle=zangle;
//calc whether to turn left or right
   zangle=(*ocb_ptr).object_list[it].Orotz;
   
   if (wangle!=zangle)
   {

   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) zangle+=angular_velocity;
   else
   zangle-=angular_velocity;
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   }
   
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
   dx+=(*ocb_ptr).object_list[it].extern_movedx;
   dy+=(*ocb_ptr).object_list[it].extern_movedy;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

}	//end of ram zex



int ram_object(int ramee,int it, int angular_velocity)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern	float SinTable[360], CosTable[360];


//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
int slot,obj_x_ang,v_diff,h_diff,h_direction,obj_z_ang;
float shot_x,shot_y,shot_z,mturn,dx,dy;
int zex_wx,zex_wy,obj_wx,obj_wy,cosapb,cosamb;
int zangle,wangle,delta_rot,d1,d2;

//try to ram zex
//calculate angle (Zrot) between monster and zex,
  zex_wx= static_cast<int>((*ocb_ptr).object_list[ramee].Oworldx);
  zex_wy= static_cast<int>((*ocb_ptr).object_list[ramee].Oworldy);

  obj_wx= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
  obj_wy= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

  zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);
   
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
   wangle=zangle;
//calc whether to turn left or right
   zangle=(*ocb_ptr).object_list[it].Orotz;
   
   if (wangle!=zangle)
   {
   
   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) 
   {
   if (d2<angular_velocity) angular_velocity=d2;	//so we don't overshoot
    zangle+=angular_velocity;
   }
   else
   {
   if (d1<angular_velocity) angular_velocity=d1;	//so we don't overshoot
   angular_velocity=-angular_velocity;
   zangle+=angular_velocity;
   }
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   }
   
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
   dx+=(*ocb_ptr).object_list[it].extern_movedx;
   dy+=(*ocb_ptr).object_list[it].extern_movedy;

//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;
 return angular_velocity;
}	//end of ram zex


void ram_object_with_roll(int ramee,int it, int angular_velocity)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern	float SinTable[360], CosTable[360];


//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
int slot,obj_x_ang,v_diff,h_diff,h_direction,obj_z_ang;
float shot_x,shot_y,shot_z,mturn,dx,dy;
int zex_wx,zex_wy,obj_wx,obj_wy,cosapb,cosamb;
int zangle,wangle,delta_rot,d1,d2,roll;

//try to ram zex
//calculate angle (Zrot) between monster and zex,
  zex_wx= static_cast<int>((*ocb_ptr).object_list[ramee].Oworldx);
  zex_wy= static_cast<int>((*ocb_ptr).object_list[ramee].Oworldy);

  obj_wx= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
  obj_wy= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

  zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);
   
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
   wangle=zangle;
//calc whether to turn left or right
   zangle=(*ocb_ptr).object_list[it].Orotz;
   
   if (wangle!=zangle)
   {
   
   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   roll=(*ocb_ptr).object_list[it].Orotx;

   if (d1>d2) 
   {
   if (d2<angular_velocity) angular_velocity=d2;	//so we don't overshoot
    zangle+=angular_velocity;
    roll+=angular_velocity;
   }
   else
   {
   if (d1<angular_velocity) angular_velocity=d1;	//so we don't overshoot
   zangle-=angular_velocity;
   roll-=angular_velocity;
   }
   
//get roll back to zero
   if (roll<0) roll+=360;
   if (roll>359) roll-=360;
   if (roll>270) 
   {
   if (roll<330) roll=330;
   }
   if (roll<180)
   {
   if (roll>30) roll=30;
   }
   
   
   if (roll!=0)
   {
   if (roll >180) 
   {
   roll++;
   }
   else
   roll--;
   }
   (*ocb_ptr).object_list[it].Orotx=roll;
   
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   }
   
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
   dx+=(*ocb_ptr).object_list[it].extern_movedx;
   dy+=(*ocb_ptr).object_list[it].extern_movedy;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

}	//end of ram zex


void turn_away_from_zex(int it, int angular_velocity)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern	float SinTable[360], CosTable[360];


//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
int slot,obj_x_ang,v_diff,h_diff,h_direction,obj_z_ang;
float shot_x,shot_y,shot_z,mturn,dx,dy;
int zex_wx,zex_wy,obj_wx,obj_wy,cosapb,cosamb;
int zangle,wangle,delta_rot,d1,d2;

//try to ram zex
//calculate angle (Zrot) between monster and zex,
  zex_wx= static_cast<int>((*ocb_ptr).object_list[zex].Oworldx);
  zex_wy= static_cast<int>((*ocb_ptr).object_list[zex].Oworldy);

  obj_wx= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
  obj_wy= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

  zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);
  zangle=zangle-180;	//anti zex
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;
   
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
   wangle=zangle;
//calc whether to turn left or right
   zangle=(*ocb_ptr).object_list[it].Orotz;
//   wangle=(*ocb_ptr).object_list[it].wanted_velocity;
   
   if (wangle!=zangle)
   {

   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) zangle+=angular_velocity;
   else
   zangle-=angular_velocity;
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   }
   
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
   dx+=(*ocb_ptr).object_list[it].extern_movedx;
   dy+=(*ocb_ptr).object_list[it].extern_movedy;
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

}	//end of ram zex


//returns turn_delta
int turn_away_from_object(int object, int it, int angular_velocity)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern	float SinTable[360], CosTable[360];


//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
int slot,obj_x_ang,v_diff,h_diff,h_direction,obj_z_ang;
float shot_x,shot_y,shot_z,mturn,dx,dy;
int zex_wx,zex_wy,obj_wx,obj_wy,cosapb,cosamb;
int zangle,wangle,delta_rot,d1,d2;

//try to ram zex
//calculate angle (Zrot) between monster and zex,
  zex_wx= static_cast<int>((*ocb_ptr).object_list[object].Oworldx);
  zex_wy= static_cast<int>((*ocb_ptr).object_list[object].Oworldy);

  obj_wx= static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
  obj_wy= static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

  zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);
  zangle=zangle-180;	//anti zex
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;
   
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
   wangle=zangle;
//calc whether to turn left or right
   zangle=(*ocb_ptr).object_list[it].Orotz;
//   wangle=(*ocb_ptr).object_list[it].wanted_velocity;
   
   if (wangle!=zangle)
   {

   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) zangle+=angular_velocity;
   else
   {
   angular_velocity=-angular_velocity;
   zangle+=angular_velocity;
   }
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   }
   
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
   dx+=(*ocb_ptr).object_list[it].extern_movedx;
   dy+=(*ocb_ptr).object_list[it].extern_movedy;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;
 return angular_velocity;
}	//


//еееееееееееееееееееееееееееееееееее
void turn_object(int it, int angular_velocity)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern	float SinTable[360], CosTable[360];


//load_dyn_object_pb the_params;
//_3D object_rotated_normal;
int slot,obj_x_ang,v_diff,h_diff,h_direction,obj_z_ang;
float shot_x,shot_y,shot_z,mturn,dx,dy;
int zex_wx,zex_wy,obj_wx,obj_wy,cosapb,cosamb;
int zangle,wangle,delta_rot,d1,d2;

//try to ram zex
//calculate angle (Zrot) between monster and zex,
//  zex_wx=(*ocb_ptr).object_list[zex].Oworldx;
//  zex_wy=(*ocb_ptr).object_list[zex].Oworldy;
//
//  obj_wx=(*ocb_ptr).object_list[it].Oworldx;
//  obj_wy=(*ocb_ptr).object_list[it].Oworldy;
//
//  zangle=get_angle_to_from(zex_wx,zex_wy,obj_wx,obj_wy);
//
//   
////  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	  //ship yaw
//   wangle=zangle;
//calc whether to turn left or right
   if (RangedRdm(1,10)>5)
   zangle=(*ocb_ptr).object_list[it].Orotz + angular_velocity;
   else
   zangle=(*ocb_ptr).object_list[it].Orotz - angular_velocity;

//   wangle=(*ocb_ptr).object_list[it].wanted_velocity;
   
//   if (wangle!=zangle)
//   {
//
//   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
//   d2=get_diff(wangle,zangle);
//   if (d1>d2) zangle+=angular_velocity;
//   else
//   zangle-=angular_velocity;
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
//   }
   
//calc thrust components   
   if ((*ocb_ptr).object_list[it].extern_movedx>0) (*ocb_ptr).object_list[it].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) (*ocb_ptr).object_list[it].extern_movedx+=1;

   if ((*ocb_ptr).object_list[it].extern_movedy>0) (*ocb_ptr).object_list[it].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) (*ocb_ptr).object_list[it].extern_movedy+=1;   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].current_velocity;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].current_velocity;
   
   dx+=(*ocb_ptr).object_list[it].extern_movedx;
   dy+=(*ocb_ptr).object_list[it].extern_movedy;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

}	//end of ram zex



void gen_callsign(int it)
{
unsigned char temp;
unsigned char temp1;
unsigned long result;
register ZObject * object_ptr;

    object_ptr=&ocb_ptr->object_list[it];
result=0;	
//this next three lines look a bit weird 'cause if one uses if else compiler optimises
//the whole lot away...hmmm.
temp1='R';
        if (object_ptr->object_category==HUMAN)
         temp1='H';	//E for earth


result=result | temp1<<24;



//temp=RangedRdm('A','Z');
//result=result | temp1<<24;

temp=RangedRdm('0','9');
result=result | temp<<16;

temp=RangedRdm('A','Y');
result=result | temp<<8;

temp=RangedRdm('0','9');
result=result | temp;

object_ptr->callsign=result;


//(*ocb_ptr).object_list[it].callsign=result;

}


//used for motehrships - third char is always a Z for motherships
unsigned long make_callsign(int type)
{
unsigned char temp;
unsigned char temp1;
unsigned long result;
register ZObject * object_ptr;

result=0;	
//this next three lines look a bit weird 'cause if one uses if else compiler optimises
//the whole lot away...hmmm.
temp1='R';
        if (type==HUMAN)
         temp1='H';	//E for earth


result=result | temp1<<24;



//temp=RangedRdm('A','Z');
//result=result | temp1<<24;

temp=RangedRdm('0','9');
result=result | temp<<16;

temp='Z';
result=result | temp<<8;

temp=RangedRdm('0','9');
result=result | temp;


return result;
}

void do_callsign_text(const_Ptr text_in,int it, int colour)	//colour=0=green, 1=blue, 2 = red
{
	const int string_len = 80;
	char temp_string[string_len];
	
	temp_string[5] = 0;		// if string_len is less than 6 then the string won't be zero terminated before this...
	
	strncpy(temp_string, text_in, string_len);			// copy to writable version
	temp_string[string_len-1] = 0;						// ensure zero terminated

	safe_number_id_to_4_character_string((*ocb_ptr).object_list[it].callsign, temp_string);

	temp_string[4]=':';
//text_in[5]=' ';
add_to_text_display(temp_string,colour);
}

//called from do_culling
void calc_roll(int it)
{

int diff,this_ang,old_ang,actual_roll;
//calc angular diff from last frame
//get ang as +-

if ((*ocb_ptr).object_list[it].current_velocity>30)
{
this_ang=(*ocb_ptr).object_list[it].Orotz;
old_ang=(*ocb_ptr).object_list[it].last_heading;

diff=this_ang-old_ang;
if (diff>180) diff=360-diff;	//+- result

if (diff==-1 || diff==1) diff=0;	//quick tweak

if (diff>0 && (*ocb_ptr).object_list[it].wanted_roll<45)
(*ocb_ptr).object_list[it].wanted_roll+=2;
else
if (diff<0 && (*ocb_ptr).object_list[it].wanted_roll>-45)
(*ocb_ptr).object_list[it].wanted_roll-=2;
else
if (diff==0)
{
 if ((*ocb_ptr).object_list[it].wanted_roll>0) (*ocb_ptr).object_list[it].wanted_roll-=1;
 else
 if ((*ocb_ptr).object_list[it].wanted_roll<0) (*ocb_ptr).object_list[it].wanted_roll+=1;
}
(*ocb_ptr).object_list[it].last_heading=this_ang;	//store this heading (as +-) for next last
}	//end of have velocity
else	//velocity too low for roll
{
 if ((*ocb_ptr).object_list[it].wanted_roll>0) (*ocb_ptr).object_list[it].wanted_roll-=1;
 else
 if ((*ocb_ptr).object_list[it].wanted_roll<0) (*ocb_ptr).object_list[it].wanted_roll+=1;
}

actual_roll=(*ocb_ptr).object_list[it].Orotx;
if (actual_roll>181) actual_roll=-(360-actual_roll);

if ((*ocb_ptr).object_list[it].wanted_roll<actual_roll) actual_roll-=2;
else
if ((*ocb_ptr).object_list[it].wanted_roll>actual_roll) actual_roll+=2;


(*ocb_ptr).object_list[it].Orotx=actual_roll;

}

//calc_roll(int it, int angular_velocity)
//{
//int actual_roll;
//
//if (angular_velocity==0) return;
//if (angular_velocity>0) angular_velocity=1;
//else
//angular_velocity=-1;
//
//if (angular_velocity>0)
// {
// if ((*ocb_ptr).object_list[it].wanted_roll<30) (*ocb_ptr).object_list[it].wanted_roll+=3; 
// }
//else
//if (angular_velocity<0)
// {
// if ((*ocb_ptr).object_list[it].wanted_roll>-30) (*ocb_ptr).object_list[it].wanted_roll-=3; 
// }
//
//if((*ocb_ptr).object_list[it].Orotx>=0 && (*ocb_ptr).object_list[it].Orotx<180)
//actual_roll=(*ocb_ptr).object_list[it].Orotx;
//else
//actual_roll=(*ocb_ptr).object_list[it].Orotx-360;
//
//if ((*ocb_ptr).object_list[it].wanted_roll>actual_roll)
//(*ocb_ptr).object_list[it].Orotx+=3;
//else
//(*ocb_ptr).object_list[it].Orotx-=3;
//}
//
//zero_roll(int it)
//{
//if ((*ocb_ptr).object_list[it].Orotx>0 && (*ocb_ptr).object_list[it].Orotx<180 ) (*ocb_ptr).object_list[it].Orotx--;
//else
//(*ocb_ptr).object_list[it].Orotx++;
//}

void reduce_extern_velocity(int it, int max_thrust_delta)
{
   if ((*ocb_ptr).object_list[it].extern_movedx>0) 
   {
   if ((*ocb_ptr).object_list[it].extern_movedx>max_thrust_delta)
   (*ocb_ptr).object_list[it].extern_movedx-=max_thrust_delta;   
   else
   (*ocb_ptr).object_list[it].extern_movedx-=1;   
   }
   
   if ((*ocb_ptr).object_list[it].extern_movedx<0) 
   {
   if ((*ocb_ptr).object_list[it].extern_movedx<-max_thrust_delta)
   (*ocb_ptr).object_list[it].extern_movedx+=max_thrust_delta;
   else
   (*ocb_ptr).object_list[it].extern_movedx+=1;
   }
   
   if ((*ocb_ptr).object_list[it].extern_movedy>0) 
   {
   if ((*ocb_ptr).object_list[it].extern_movedy>max_thrust_delta)
   (*ocb_ptr).object_list[it].extern_movedy-=max_thrust_delta;   
   else
   (*ocb_ptr).object_list[it].extern_movedy-=1;   
   }
   
   if ((*ocb_ptr).object_list[it].extern_movedy<0) 
   {
   if ((*ocb_ptr).object_list[it].extern_movedy<-max_thrust_delta)
   (*ocb_ptr).object_list[it].extern_movedy+=max_thrust_delta;
   else
   (*ocb_ptr).object_list[it].extern_movedy+=1;
   }
}



int count_objects(unsigned long the_id)
{
int active_object_counter;
int object_counter;
int ast_count;
extern int dynamic_object_index;

object_counter=active_object_counter=ast_count=0;
while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("triv_rejection looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
   if ((*ocb_ptr).object_list[object_counter].Oid==the_id)ast_count++;
   
   active_object_counter++;
   }
 object_counter++;
  
 }
return ast_count;
}







//alien NPC's
int GenRandomMonsterWithVelocity(int worldx, int worldy, int vel, int heading, int spawner, int the_class)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level,game_running;
extern kit_desc ship_items[ITEMS_COUNT];
extern int current_quad, difficulty;

load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
int monst_select;
int k,j;
int done_phoenix=0;
int return_slot=0;

#define GO_HOME 300;

if (the_class==-1)	//we select?
{
 the_class=RangedRdm(1,100);	//mainly fighters with some travellers and guards
  if (the_class>80) the_class=NPC_CLASS_GUARD;
  else
  if (the_class>60) the_class=NPC_CLASS_TRAVELLER;
  else
  the_class=NPC_CLASS_FIGHTER;
}

        clear_pb(&the_params);
   
   
   
//switch on difficulty
	switch (difficulty)
	{
	int rdm;
		case 16:
			//Space cadet. Here we bias heavily towards 0
			rdm=RangedRdm(0,100);
			if (rdm<60)
			{
				monst_select=0;
			}
			else if (rdm<75)
			{
				monst_select=1;
			}
			else if (rdm<98)
			{
				monst_select=2;
			}
			else
			{
				monst_select=3;
			}
			
		break;
		
		
		case 8:
			//Zexician. Here we aim for even distribution with a bias towards 0
			rdm=RangedRdm(0,100);
			if (rdm<30)
			{
				monst_select=0;
			}
			else if (rdm<50)
			{
				monst_select=1;
			}
			else if (rdm<75)
			{
				monst_select=2;
			}
			else
			{
				monst_select=3;
			}

		break;
		case 1:
			//master Zexician. Here we bias towards 0 even less
			rdm=RangedRdm(0,100);
			if (rdm<10)
			{
				monst_select=0;
			}
			else if (rdm<20)
			{
				monst_select=1;
			}
			else if (rdm<65)
			{
				monst_select=2;
			}
			else
			{
				monst_select=3;	//if a fighter this becomes green
			}

		break;

		default:	//bug if we get here
        monst_select=RangedRdm(0,4)+current_quad;	//current quad is 0 to 3
	}


		monst_select+=current_quad;
//        monst_select=RangedRdm(0,4)+current_quad;	//current quad is 0 to 3
		if (monst_select>7) monst_select=7;
		//printf ("Monster = %i\n",monst_select);
//        monst_select=RangedRdm(0,5);

//         monst_select=5;	  //warper
	the_params.world_x=worldx;
	the_params.world_y=worldy;

//	the_params.world_x=(*ocb_ptr).object_list[0].Oworldx+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.x);
//	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy+((40000+RangedRdm(0,4000)-2000)*ship_rotated_normal.y);	
	the_params.world_z=MONSTER_Z;
        the_params.rot_z=heading;
        the_params.current_velocity=vel;
 if (the_class==NPC_CLASS_FIGHTER)
 {
  if (monst_select==3) monst_select=4;	//if shuttle, gen a grn1 if fighter
 }	
 else
 {
 if (the_class==NPC_CLASS_TRAVELLER)	//we only have one type of shuttle
 monst_select=3;
 }
 
 
 switch (monst_select)
	     {

       case 0:
      {
      static_slot=get_static_object_slot('MON2');	 //go fetch fido (the object)
      if (static_slot==-1) 
      break;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
         return_slot=dest_ocb;
         
	     the_params.controller_ref=NPC_CONTROL;
             the_params.max_vel=350;
             the_params.min_vel=-100;
             the_params.aggressive=1;
             the_params.sub_controller=255;	// used to be -1;
             the_params.shields_class=1;
             the_params.go_home_time=RangedRdm(2000,51000);	//1000 frames and it finds somewhere to dock

     the_params.has_roll=0;
             the_params.cannon_cat=1;
             the_params.laser_cat=1;
	     the_params.shield_value=570; //Beta5 - was 870
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=baddie_object;
        the_params.obj_class=NPC_CLASS_FIGHTER;

     the_params.who_spawned_this_object=-1;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
	gen_callsign(dest_ocb);
	     (*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_SMALL;
        }
                   break;
      }
      
            case 1:
            {
             static_slot=get_static_object_slot('MON3');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)

             {
                  the_params.has_roll=0;
                   return_slot=dest_ocb;
             the_params.max_vel=500;	//beta 5 - was 600
             the_params.min_vel=-200;
             the_params.aggressive=1;
             the_params.sub_controller=255;	// used to be -1;
             the_params.shields_class=1;
             the_params.go_home_time=RangedRdm(1000,50000);	//1000 frames and it finds somewhere to dock
	           the_params.controller_ref=NPC_CONTROL;
	           the_params.shield_value=790;	//Beta5 - was 1890
	           the_params.mass=100;
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;
                   the_params.cannon_cat=2;
	           the_params.laser_cat=1;
     the_params.who_spawned_this_object=-1;
        the_params.obj_class=NPC_CLASS_FIGHTER;
     	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
	     (*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_BLUE;
	gen_callsign(dest_ocb);
              }
             break;
           }

            case 2:	//phoenix - m4a
            {
//            if (ship_items[KLASER].modifier<2) break;
    anim.object_id[0]='MN31';
    anim.object_id[1]='MN32';
    anim.object_id[2]='MN33';
    anim.object_id[3]='MN34';
    anim.object_id[4]=0;
    anim.object_id[5]=0;
             
             static_slot=get_static_object_slot('MN31');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
             the_params.max_vel=800;	//beta 5 - was 1300
             the_params.min_vel=-200;
             the_params.aggressive=1;
             the_params.sub_controller=255;	// used to be -1;
             the_params.shields_class=1;
             the_params.has_roll=1;
             the_params.go_home_time=RangedRdm(2000,8000);	//1000 frames and it finds somewhere to dock
     the_params.has_roll=0;
                   return_slot=dest_ocb;
	the_params.controller_ref=NPC_CONTROL;
	the_params.shield_value=1250;	//Beta5 - was 3650
	the_params.mass=70;	//beta 5 - was 100
	the_params.no_trivial_rejection=1;
	 the_params.object_category=baddie_object;
                   the_params.cannon_cat=1;	//beta 5 - was cat 2 cannon
	 the_params.laser_cat=1;
     the_params.who_spawned_this_object=-1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=2;
        the_params.obj_class=NPC_CLASS_FIGHTER;
    load_animated_object (&anim,4,3,1,1,&the_params,-1);
	gen_callsign(dest_ocb);
	     (*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_PHOENIX;
//	load_dyn_object(static_slot,dest_ocb,&the_params);	//the object, the position (-1=next free)
    }
 

                   break;
    }

            case 3:	//shuttle
            {
			
			 int human;
			 if (RangedRdm(0,100)>50)
			  human=1;
			 else
			 human=0;
			 
			 human=0; 
			 if (human==0)
             static_slot=get_static_object_slot('ASH1');	//go fetch fido (the object)
             else
             static_slot=get_static_object_slot('ESH1');	//go fetch fido (the object)
			 
			 if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)	//wont generate shuttles if not in galaxy
             {
            return_slot=dest_ocb;
              if (human==0)
			  the_params.max_vel=580;
			  else
			  the_params.max_vel=280;
			  
			 the_params.min_vel=-100;
             the_params.aggressive=0;
             the_params.sub_controller=255;	// used to be -1;
             the_params.shields_class=2;
             the_params.go_home_time=7000;	//RangedRdm(1000,5000);	//1000 frames and it finds somewhere to dock

	           the_params.controller_ref=NPC_CONTROL;
	           the_params.shield_value=5990; //Beta5 - was 8990
	           the_params.mass=300;	//beta 5 - was 100
	           the_params.has_roll=1;
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;
             the_params.obj_class=NPC_CLASS_TRAVELLER;
	           
	           the_params.control_use_7=spawner;	//so shuttle knows where it came from
	           the_params.who_spawned_this_object=-1;	//so shuttle knows where it came from
				the_params.obj_class=NPC_CLASS_TRAVELLER;
                   the_params.cannon_cat=0;
              	the_params.laser_cat=3;
 	            load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
				(*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_ASHUTTLE;
				gen_callsign(dest_ocb);
              }
             break;
           }

            case 4:	//GRN1 (mon3a)
            {
             static_slot=get_static_object_slot('GRN1');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
             {
            return_slot=dest_ocb;
             the_params.max_vel=500;	//beta 5 - was 1000
             the_params.min_vel=-200;
             the_params.aggressive=1;
             the_params.sub_controller=255;	// used to be -1;
             the_params.shields_class=1;
             the_params.go_home_time=RangedRdm(2000,5000);	//1000 frames and it finds somewhere to dock
                   the_params.has_roll=1;
	           the_params.controller_ref=NPC_CONTROL;
	           the_params.shield_value=1790;	//Beta5 - was 5290
	           the_params.mass=70;	//beta 5 - was 100
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;
                   the_params.cannon_cat=2;
	           the_params.laser_cat=1;
                   the_params.who_spawned_this_object=-1;
        the_params.obj_class=NPC_CLASS_FIGHTER;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
	gen_callsign(dest_ocb);              
	     (*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_GREEN;

              }
             break;
           }


            case 5:	//warper
            {
             static_slot=get_static_object_slot('WARP');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
             {
            return_slot=dest_ocb;
             the_params.max_vel=1375;
             the_params.min_vel=-200;
             the_params.aggressive=1;
             the_params.sub_controller=warper;
             the_params.shields_class=2;
             the_params.go_home_time=RangedRdm(1000,5000);	//1000 frames and it finds somewhere to dock
                   the_params.has_roll=1;
	           the_params.controller_ref=NPC_CONTROL;
	           the_params.shield_value=5500; //Beta5 - was 7500
	           the_params.mass=160;	//beta 5 - was 100
	           
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;
                   the_params.cannon_cat=3;
	           the_params.laser_cat=2;
                   the_params.who_spawned_this_object=-1;
	 the_params.missile_cat=1;
	 the_params.number_of_missiles=2;

        the_params.obj_class=NPC_CLASS_FIGHTER;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
	gen_callsign(dest_ocb);              
	     (*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_GREEN;

              }
             break;
           }

            case 6:	//python
            {
             static_slot=get_static_object_slot('MON6');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found

             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)
             {
				the_params.has_roll=1;
	           the_params.controller_ref=monster6;
				the_params.max_vel=1800;
				the_params.min_vel=-200;
	           the_params.shield_value=10700;
				the_params.shields_class=3;
	           the_params.mass=300;	//beta 5 - was 200
	           the_params.no_trivial_rejection=1;
	           the_params.object_category=baddie_object;

	           the_params.laser_cat=3;
	           the_params.cannon_cat=3;
	           the_params.missile_cat=2;
	           the_params.number_of_missiles=2;
                   the_params.who_spawned_this_object=-1;
	           load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
              }
             break;
           }



            case 7:	//excalibur
            {
				static_slot=get_static_object_slot('EXC1');	//go fetch fido (the object)
				if (static_slot==-1) 
				break;	//not found

				dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
				if (dest_ocb!=-1)
				{
						return_slot=dest_ocb;
						the_params.has_roll=1;
						the_params.object_category=ALIEN;
						the_params.controller_ref=excalabur;
						the_params.max_vel=320;
						the_params.min_vel=-20;
					   the_params.shield_value=18300;
						the_params.shields_class=4;

					   the_params.mass=400;
					   the_params.no_trivial_rejection=1;
					   the_params.object_category=baddie_object;

					   the_params.laser_cat=3;
					   the_params.cannon_cat=3;
					   the_params.missile_cat=3;
					   the_params.number_of_missiles=500;
						the_params.who_spawned_this_object=-1;
						load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
					}
					
					break;
			}
			
			
            default :
                   break;
    }

return return_slot;
}


// -----Earth NPCs ------------------

int GenRandomHumanNPCWithVelocity(int worldx, int worldy, int vel, int heading, int spawner, int obj_class)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int current_level,game_running;
extern kit_desc ship_items[ITEMS_COUNT];
load_dyn_object_pb the_params;
int i,dest_ocb,static_slot;
anim_pb anim;
int monst_select;
int k,j;
int return_slot=0;

if (obj_class==-1)	//we select?
{
 obj_class=RangedRdm(1,100);	//mainly fighters with some travellers and guards
  if (obj_class>80) obj_class=NPC_CLASS_GUARD;
  else
  if (obj_class>60) obj_class=NPC_CLASS_TRAVELLER;
  else
  obj_class=NPC_CLASS_FIGHTER;
}

  j=1;

for (k=0;k<j;k++)
  {
	clear_pb(&the_params);
   
	monst_select=RangedRdm(0,2);
  
	the_params.world_x=worldx;
	the_params.world_y=worldy;

	the_params.world_z=MONSTER_Z;
	the_params.rot_z=heading;
	the_params.current_velocity=vel;
	the_params.obj_class=obj_class;
             
 if (obj_class==NPC_CLASS_FIGHTER)
 {
  if (monst_select==1) monst_select=0;	//if shuttle, gen a Falcon
 }	
 else
 {
 if (obj_class==NPC_CLASS_TRAVELLER)
 monst_select=1;
 }
 
if (monst_select>1) monst_select=0;

switch (monst_select)   
{


//gen a falcon fighter
       case 0:
      {
      static_slot=get_static_object_slot('FALC');	//go fetch fido (the object)
      if (static_slot==-1) 
      break;	//oops

      dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
      if (dest_ocb!=-1)
        {
         return_slot=dest_ocb;
         int npc_rank=generate_sensible_rank();
		 int cannon_cat=1;
		 int laser_cat=1;
		 if (npc_rank>ENS) cannon_cat=2;
		 if (npc_rank>CDR) cannon_cat=3;
		 if (npc_rank>ENS) laser_cat=2;
		 if (npc_rank>CDR) laser_cat=3;
		 
	     the_params.controller_ref=NPC_CONTROL;
             the_params.has_roll=1;
    	
	     the_params.shield_value=870;
	     the_params.mass=100;
	     the_params.no_trivial_rejection=1;
	     the_params.object_category=zex_object;
	     the_params.laser_cat=laser_cat;
	     the_params.cannon_cat=cannon_cat;
             the_params.number_of_missiles=0;
             
             the_params.max_vel=900;
             the_params.min_vel=-200;
             the_params.aggressive=1;
             the_params.sub_controller=255;	// used to be -1;
             
			 the_params.shields_class=0;
			if (npc_rank>ENS) the_params.shields_class=2;
			if (npc_rank>LT) the_params.shields_class=3;
			if (npc_rank>CDR) the_params.shields_class=4;
			if (npc_rank>CAPT) the_params.shields_class=5;

			 the_params.go_home_time=RangedRdm(1000,5000);	//1000 frames and it finds somewhere to dock
             the_params.who_spawned_this_object=-1;
	     load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
	     (*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_FALCON;
		 (*ocb_ptr).object_list[dest_ocb].rank=npc_rank;
	     gen_callsign(dest_ocb);
	        }
                   break;
      }
      
            case 1:	//shuttle
            {
			 int human;
			 if (RangedRdm(0,100)>50)
			 human=1;
			 else
			 human=0;
			 
			 human=1;
             if (human==0)
			 static_slot=get_static_object_slot('ASH1');	//go fetch fido (the object)
			 else
			 static_slot=get_static_object_slot('ESH1');	//go fetch fido (the object)
             if (static_slot==-1) 
	         break;	//not found
             dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
             if (dest_ocb!=-1)	//wont generate shuttles if not in galaxy
             {
				return_slot=dest_ocb;
				
				if (human==1)
				the_params.max_vel=280;
				else
				the_params.max_vel=580;
				
				the_params.min_vel=-100;
				the_params.aggressive=0;
				the_params.sub_controller=255;	// used to be -1;
				the_params.shields_class=2;
				the_params.go_home_time=5000;	//RangedRdm(1000,5000);	//1000 frames and it finds somewhere to dock
				the_params.controller_ref=NPC_CONTROL;
				the_params.shield_value=8990;
				the_params.mass=1000;
				the_params.has_roll=1;
				the_params.no_trivial_rejection=1;
				the_params.object_category=zex_object;
				the_params.obj_class=NPC_CLASS_TRAVELLER;
				   
				the_params.control_use_7=spawner;	//so shuttle knows where it came from
				the_params.who_spawned_this_object=-1;	//so shuttle knows where it came from
				the_params.obj_class=NPC_CLASS_TRAVELLER;
				the_params.cannon_cat=0;
				the_params.laser_cat=3;
				load_dyn_object(static_slot,dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
				(*ocb_ptr).object_list[dest_ocb].craft_type=TYPE_ESHUTTLE;
				gen_callsign(dest_ocb);
              }
             break;
           }
//



            default :
                   break;
    }
 } //end of for k

return return_slot;
}

int generate_sensible_rank()
{
int temp_rank=RangedRdm(0,100);
	if (temp_rank<45) return ENS;	//45 - distribution %
	if (temp_rank<62) return LT;	//18
	if (temp_rank<75) return LTCR;	//13
	if (temp_rank<85) return CDR;	//10
	if (temp_rank<92) return CAPT;	//7
	if (temp_rank<96) return RADM;	//3
	if (temp_rank<98) return VADM;	//2
	if (temp_rank<99) return ADM;	//1
	return FADM;	//1
	
/* old function based around Zex's rank
extern int gRank;
	//we generate a rank3 below and 1 above zex's rank
	int temp_rank=RangedRdm(0,4);
	temp_rank-=3;
	temp_rank+=rank;
	if (temp_rank<ENS) temp_rank=ENS;
	if (temp_rank>FADM) temp_rank=FADM;
	return temp_rank; */
}


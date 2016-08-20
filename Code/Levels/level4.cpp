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

/*
Level #:4
Auth: 
Date:
Rev:
 */
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

extern int    global_open_doors;
extern int global_control_monsters;
extern	float SinTable[360], CosTable[360];	//fancy see this here!!!



void Level4();
void init_level4();


void quicksave_level4();
void restore_quicksave_level4();
void reinit_level4();


extern int level_complete,can_do_next_level; 	  //game var - 0,1 or 2
extern float dest_x,dest_y;	//from level file
extern	DynObjectsFixedSize *ocb_ptr;
static int ioda_slot;
static int done_find_bid;

static int monster_timer;
static int l4_timer;
void init_level4()
{
extern int mothership_core_slot;

   
   global_open_doors=1;
//   return;
   global_control_monsters=1;	//monsters freeze
  l4_timer=0;
  monster_timer=1200;
////set the ionic device invisible
ioda_slot=find_dynamic_object_slot('IODA');
if (ioda_slot==-1) report_error_c ("Level3: Couldn't find IODA ","",-3);

//simple but effective
(*ocb_ptr).object_list[ioda_slot].Oworldx=-100000000;
(*ocb_ptr).object_list[ioda_slot].Oworldy=-100000000;
done_find_bid=0;
}



void Level4()
{

//return;
//if zex is in the compound, enable monsters
 if (find_dynamic_object_slot('IODA')==-1)
  {

  level_complete=2;
  dest_x=782500;
  dest_y=100000;
       insert_batcom_message("MISSION COMPLETE","                ");
      add_to_text_display( ( char *) "MISSION COMPLETE! ",1);
      add_to_text_display( ( char *) "DOCK WITH A MOTHERSHIP TO END MISSION.",1);

   can_do_next_level=1;
}
//if (level_complete==2) return;	//all done

//if all the bids are gone, show the ioda
 if (done_find_bid==0 && find_dynamic_object_slot('BID1')==-1)
  {
   done_find_bid=1;
   (*ocb_ptr).object_list[ioda_slot].Oworldx=302500;
   (*ocb_ptr).object_list[ioda_slot].Oworldy=-2500;
  }


l4_timer++;

monster_timer--;
 if (monster_timer<0)
 {
     monster_timer=2700;
     GenRandomMonster();
     GenRandomNastyMonster();
if (l4_timer>20000)
     {
     GenRandomNastyMonster();
     monster_timer+=1000;
     }
if (l4_timer>40000)
     {
     GenRandomNastyMonster();
     monster_timer+=1000;
     }
 }

}







void quicksave_level4()
{
}

void restore_quicksave_level4()
{
}

void reinit_level4()
{
}



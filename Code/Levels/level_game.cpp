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
Level #:6
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
#include "explode.h"
#include "object_control_top.h"
#include "sound.h"
#include "Batcom.h"
#include "monster_control_misc.h"
#include "obj_control_misc_routines.h"



static int LFrame_counter,total_frame_counter;	//timer
static int next_ast_time;


void Level6();
void init_level6();
void quicksave_level6();
void restore_quicksave_level6();
void reinit_level6();

void init_level6()
{
 next_ast_time=RangedRdm(3000,10000);	//number of frames to next asteroid set

}

//This is the game level
void Level6()
{
extern int level_complete; 	//game var - 0,1 or 2
extern float dest_x,dest_y;	//from level file
extern	DynObjectsFixedSize *ocb_ptr;
extern kit_desc ship_items[ITEMS_COUNT];	//ships systems
extern _3D ship_rotated_normal; //from engine

int i,dest_ocb,static_slot,ast_x,ast_y;
load_dyn_object_pb the_params;



total_frame_counter++;

if (total_frame_counter>next_ast_time+5)
{
 total_frame_counter=0;
 if (ship_items[KNAV_COMPUTER].status==1)
 {
  if (RangedRdm(1,10)>5)
  add_to_text_display("WARNING: MAGNETIC ANOMALIES DETECTED",1);
  else
  add_to_text_display("WARNING: ABNORMAL NATURAL MAGNETIC FLUX VARIATIONS",1);
 }
 
 next_ast_time=RangedRdm(10000,25000);	//number of frames to next asteroid set
}

if (total_frame_counter>next_ast_time && total_frame_counter<next_ast_time+5)
{

//gen an asteroid
  dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
  if (dest_ocb!=-1)
   {
           clear_pb(&the_params);

	  ast_x=static_cast<int>((*ocb_ptr).object_list[0].Oworldx+40000);
//	ast_x=ast_x+(40000*ship_rotated_normal.x);
	   ast_y=static_cast<int>((*ocb_ptr).object_list[0].Oworldy);
	   ast_y=static_cast<int>(ast_y+(40000*ship_rotated_normal.y));

	the_params.world_x=ast_x+RangedRdm(0,20000)-10000;
	the_params.world_y=ast_y+(RangedRdm(0,20000)-10000);	
	the_params.world_z=MONSTER_Z;
	the_params.Omovedx=RangedRdm(0,120)-140;
	the_params.delta_rot_y=RangedRdm(1,10)-5;
	the_params.delta_rot_z=RangedRdm(1,10)-5;
	the_params.controller_ref=asteroid1;
	the_params.shield_value=31170;
	the_params.mass=11000;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=1;
	 the_params.object_category=scenary_object;
     static_slot=get_static_object_slot('AST1');	//go fetch fido (the object)
	 if (static_slot==-1) 
	     report_error_c("Level6: get_static_object_slot failed.","",-3);
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,dest_ocb,&the_params,-1);	   //the object, the position (-1=next free)
    }
}


}

void quicksave_level6()
{
}

void restore_quicksave_level6()
{
}

void reinit_level6()
{
}



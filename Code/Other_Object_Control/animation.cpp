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

//animation.c
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "animation.h"
#include "3d_structures_def.h"

extern	  DynObjectsFixedSize *ocb_ptr;

void run_animations()
{
extern int dynamic_object_index;

int active_object_counter;
int object_counter;
int new_frame;
//int debug_loop_count=0;
object_counter=0;
active_object_counter=0;

while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("animation looped");
  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
    if ((*ocb_ptr).object_list[object_counter].anim_max_object!=1 && (*ocb_ptr).object_list[object_counter].anim_state!=0)	//if animated object
     {
     //еееееееееееееееееееееееееееееееееееееееееееееееееееееее
     //state=0=no run
      (*ocb_ptr).object_list[object_counter].anim_counter++;
      if ((*ocb_ptr).object_list[object_counter].anim_counter>(*ocb_ptr).object_list[object_counter].anim_frame_rate)
         {
         (*ocb_ptr).object_list[object_counter].anim_counter=0;	//reset
         (*ocb_ptr).object_list[object_counter].anim_frame++;
         if ((*ocb_ptr).object_list[object_counter].anim_frame==(*ocb_ptr).object_list[object_counter].anim_max_object)
         {
            (*ocb_ptr).object_list[object_counter].anim_frame=0;
            //check stop at end of cycle
            if ((*ocb_ptr).object_list[object_counter].anim_control==0) (*ocb_ptr).object_list[object_counter].anim_state=0;	
         }
         //load in the ptrs to the object
         new_frame=(*ocb_ptr).object_list[object_counter].anim_frame;
         (*ocb_ptr).object_list[object_counter].points_ptr=(*ocb_ptr).object_list[object_counter].the_anim_ptrs[new_frame].points_ptr;
         (*ocb_ptr).object_list[object_counter].polys_ptr=(*ocb_ptr).object_list[object_counter].the_anim_ptrs[new_frame].polys_ptr;
         (*ocb_ptr).object_list[object_counter].active_polys_slot=(*ocb_ptr).object_list[object_counter].the_anim_ptrs[new_frame].polys_slot;

         }
     //еееееееееееееееееееееееееееееееееееееееееееееееееееееее
     }	//end of animated object
   active_object_counter++;   
   }	//end of object alive
object_counter++;
 }	//end of while

}


//ееSet a given object to a certain frame. Used by monster controllers - for
//example one that sets the wing sweep.
void set_animation_frame(int the_object, int the_frame)
{
int new_frame;
         new_frame=the_frame;
         (*ocb_ptr).object_list[the_object].points_ptr=(*ocb_ptr).object_list[the_object].the_anim_ptrs[new_frame].points_ptr;
         (*ocb_ptr).object_list[the_object].polys_ptr=(*ocb_ptr).object_list[the_object].the_anim_ptrs[new_frame].polys_ptr;
         (*ocb_ptr).object_list[the_object].active_polys_slot=(*ocb_ptr).object_list[the_object].the_anim_ptrs[new_frame].polys_slot;

}


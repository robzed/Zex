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

// compile_zmap.cpp

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <stdlib.h>		// for atof()

//oldmac//#include <fp.h>
#include "3d_structures_def.h"
#include "compile_zmap.h"
#include "Error_handler.h"
#include "MeshToZex.h"
#include "load_objects.h"

int bg_map_objs_in_column,bg_map_how_many_columns;
int bg_map_top,bg_map_left;

//compile_zmap
//SB 27/11/98
//12/1/98 - map top left no longer part of the zmp file, no passed in as params
void compile_zmap(char *the_file_data)
{
//gets called with a pointer to a .zmp file. Must compile a list of pointers to
//static objects and fill in bg_map_objs_in_column and bg_map_how_many_columns (assumed to
//be the first two numbers found in the file
//not the most thrilling piece of code to write

extern cmap_objects *cmap_ptr_base;
char the_line[512];	//max line size in a mesh file
char the_word[512];	//max length of a word from a line
char *source;		//walk through the file with this
int line_len=0, line_pos=0, word_len=0;		//loop control
//decimal my_dec;
short ix;
short vp;
float dectof_result;	//for the OS routine numtostring
cmap_objects *cmap_ptr;
int i,j; //loop counts
int cmap_objs_slot=0;

unsigned long *id_ptr;
//ZObject *the_static_ptr;
int the_static_object;

source=the_file_data;	//source is incremented through the file


//get number of objects in a column and number of columns in map
the_word[0]='*';	//allow the file to start with commented lines...
while (the_word[0]=='*')
{
 line_len=get_line(source,the_line);
 if (line_len!=-1)
    {
    source+=line_len+1;	//point to next line (accounting for CR)
    word_len=get_word(the_word,the_line);
	}
 }
//the word should now hopefully contain number of objs in a column
 dectof_result = atof(the_word);
 //oldmac//ix=0;
 //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
 //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
 bg_map_objs_in_column =static_cast<int>(dectof_result*3);	//bg, top, high
 
//get number of columns in map
 get_word(the_word,&the_line[word_len]);
 dectof_result = atof(the_word);
 //oldmac//ix=0;
 //oldmac//str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
 //oldmac//dectof_result=dec2f (&my_dec);	//number of objects in a column
 bg_map_how_many_columns =static_cast<int>(dectof_result);


//get map top left
the_word[0]='*';	//allow the file to start with commented lines...
//while (the_word[0]=='*')
//{
// line_len=get_line(source,the_line);
// if (line_len!=-1)
//    {
//    source+=line_len+1;	//point to next line (accounting for CR)
//    word_len=get_word(the_word,the_line);
//	  }
// }
////the word should now hopefully contain number of objs in a column
// ix=0;
// str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
// dectof_result=dec2f (&my_dec);	//number of objects in a column
// bg_map_top=dectof_result;
////get number of columns in map
// get_word(the_word,&the_line[word_len]);
// ix=0;
// str2dec(the_word,&ix,&my_dec,&vp);	//convert from string to float
// dectof_result=dec2f (&my_dec);	//number of objects in a column
// bg_map_left =dectof_result;

//now get the object defs
cmap_ptr=cmap_ptr_base;	//we insert the pointers to the static objects here

//skip comment lines between header and data
the_line[0]='*';	//allow the file to start with commented lines...
while (the_line[0]=='*')
{
 line_len=get_line(source,the_line);
 if (the_line[0]=='*')
    source+=line_len+1;	//point to next line (accounting for CR)
 }
 
cmap_objs_slot=0;
//NB: comments not allowed in map
for (i=0;i<bg_map_how_many_columns;i++)
 {
  line_len=get_line(source,the_line);	//get next line
  word_len=0;
  for (j=0;j<bg_map_objs_in_column;j++)
   {
     if (line_len!=-1)
      {
       word_len+=get_word(the_word,&the_line[word_len]);
		   id_ptr=(unsigned long*) the_word;
		   
		   #if ARCH_LITTLE_ENDIAN	//Fix problem on intel build with cmap objects 
		   unsigned long id=Endian32_Swap(*id_ptr);
		   #else
		   unsigned long id=*id_ptr;
		   #endif
		   the_static_object=get_static_object_slot(id);
		   if (the_static_object==0) report_error_c("Bad zmap. Couldn't find the static BG object.","",667);
		   (*cmap_ptr).cmap_objs[cmap_objs_slot]=the_static_object;
		   cmap_objs_slot++;
		   if (cmap_objs_slot>NUMBER_OF_CMAP_OBJS) report_error_c("Too many bg objects in zmap. NUMBER_OF_CMAP_PTRS max.","",666);
//search static objects for Oid=the_word
	  }

   }	//end of bg objects
 
  
  source+=line_len+1;	//point to next line (accounting for CR)
  if (line_len==-1) 	report_error_c("Bad zmap. Unexpected EOF.","",-1);
 }	//end of number of columns - i

}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//searches static objects for an id, if found returns the address of the object else 0
ZObject * get_static_object_pointer(unsigned long the_id)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	static_object_index;	//how many static objects
ZObject *return_val;

int match=0,index=0;
return_val=0;

while (match==0 && index<static_object_index)
 {
 if ((*ocb_Sptr).object_list[index].Oid==the_id) match=1;
 index++;
 }

if (match==1) return &(*ocb_Sptr).object_list[index-1];	//address of static definition of bg object
else return return_val;

}

//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//searches static objects for an id, if found returns the slot of the object else -1
int get_static_object_slot(unsigned long the_id)
{
extern	StatObjectsFixedSize *ocb_Sptr;
extern int	static_object_index;	//how many static objects
int return_val;

int match=0,active_object_count, object_count;
return_val=-1;

object_count=active_object_count=0;
while (active_object_count<static_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
{
  if ((*ocb_Sptr).object_list[object_count].in_use==1)
  {
   if ((*ocb_Sptr).object_list[object_count].Oid==the_id)
   {
     match=1;
     break;
   }
   active_object_count++;
 }
 object_count++;
}
if (match==1) return object_count;	//slot of static definition of bg object
else return return_val;

}





//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//init_cmap
//SB 1/12/98
//this routine loads in bg_how_manu_columns columns of the cmap into the dynamic objects
void init_cmap(float bg_left, float bg_top)
{
extern int bg_map_objs_in_column;
extern cmap_objects *cmap_ptr_base;
int the_column=0;
int i,dest_ocb;
load_dyn_object_pb the_params;



while (the_column<bg_map_how_many_columns)
 {
//do bg objects
  for (i=0;i<bg_map_objs_in_column/3;i++)	//low level objects first
   {
   if ((*cmap_ptr_base).cmap_objs[i+(bg_map_objs_in_column*the_column)] != -1)
   {
    dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
    the_params.world_x=bg_left+(the_column*4000);
	the_params.world_y=bg_top-(i*4000);
	the_params.world_z=100;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=0;
     the_params.has_roll=0;
	the_params.mass=1000;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=scenary_object;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object((*cmap_ptr_base).cmap_objs[i+(bg_map_objs_in_column*the_column)],dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
   }
//	load_dyn_object((*cmap_ptr_base).cmap_objs[0],dest_ocb,&the_params);	//the object, the position (-1=next free)


   }	//end for bg objs in column

//do top objects
  for (i=bg_map_objs_in_column/3;i<bg_map_objs_in_column/3+bg_map_objs_in_column/3;i++)	//high level objects
   {
   if ((*cmap_ptr_base).cmap_objs[i+(bg_map_objs_in_column*the_column)] != -1)
   {
    dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
    the_params.world_x=bg_left+(the_column*4000);
	the_params.world_y=bg_top-((i-3)*4000);
	the_params.world_z=100;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	the_params.shield_value=1000;
	the_params.mass=1000;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=scenary_object;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
	 the_params.number_of_missiles=0;
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object((*cmap_ptr_base).cmap_objs[i+(bg_map_objs_in_column*the_column)],dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
   }
 }	//end of top objects

//do high objects
  for (i=bg_map_objs_in_column/3+bg_map_objs_in_column/3;i<bg_map_objs_in_column;i++)	//top objects
   {
   if ((*cmap_ptr_base).cmap_objs[i+(bg_map_objs_in_column*the_column)] != -1)
   {
    dest_ocb=find_vacant_dynamic_object_slot();	//find a slot for this object
    the_params.world_x=bg_left+(the_column*4000);
	the_params.world_y=bg_top-((i-6)*4000);
	the_params.world_z=-400;
	the_params.Omovedx=0;
	the_params.Omovedy=0;
	the_params.Omovedz=0;
	the_params.delta_rot_x=0;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=0;
	the_params.rot_y=0;
	the_params.rot_z=0;
	the_params.controller_ref=0;
	the_params.control_use_1=0;
	the_params.control_use_2=0;
	the_params.control_use_3=0;
	the_params.control_use_4=0;
	the_params.control_use_7=0;
	the_params.current_velocity=0;
	the_params.wanted_velocity=0;
	 the_params.attack_mode=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
	the_params.mass=1000;
		 	 the_params.dq=0;
	the_params.no_trivial_rejection=0;
	    the_params.object_category=scenary_object;
	 the_params.laser_cat=0;
	 the_params.cannon_cat=0;
	 the_params.missile_cat=0;
     the_params.who_spawned_this_object=-1;
	 the_params.number_of_missiles=0;
	 the_params.unique_id=0;
	load_dyn_object((*cmap_ptr_base).cmap_objs[i+(bg_map_objs_in_column*the_column)],dest_ocb,&the_params,-1);	//the object, the position (-1=next free)
   }


//	load_dyn_object((*cmap_ptr_base).cmap_objs[0],dest_ocb,&the_params);	//the object, the position (-1=next free)


   }	//end for high objects


 the_column++;
 
 }	//end while



}



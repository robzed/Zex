//aim.c
//given an object "from" this routine picks an object in front of it to aim at
//returns either an object slot or -1
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "aim.h"
#include "monster1.h"
#include "3d_structures_def.h"
#include	"objects_controls.h"	//controller definitions
#include "angular_stuff.h"
#include "monster_control_misc.h"


extern int ship_z_ang,difficulty;
//pick a target with a 50 degrees capture angle
//simple philosophy - examine the ocbs, looking for monsters
//when we find one we decide if it's in front or not.
//if it is, and it's in range, then we return that slot number
int aim_from_zex(int laser_cat,int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
int active_object_counter,object_counter,got_one;
float examine_x,examine_y;	//x and y of object being examined
float zex_x,zex_y;	//relative to zex
int angle_to_object,angular_diff;
int debug_loop_count;
int capture_angle;
//debug_loop_count=0;
active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable

capture_angle=(25*laser_cat)+difficulty;
if (capture_angle>80) capture_angle=80;	//tasers for example

while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1 ) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==baddie_object
         && (*ocb_ptr).object_list[object_counter].is_visible!=0
  )
    {
    //these laods are for debugging
     examine_x=(*ocb_ptr).object_list[object_counter].Oworldx;
     examine_y=(*ocb_ptr).object_list[object_counter].Oworldy;
//from the "angle to" we can compute whether these are in front or behind us.
//use of some nifty routines coming up...
     zex_x=(*ocb_ptr).object_list[zex].Oworldx;
     zex_y=(*ocb_ptr).object_list[zex].Oworldy;
     angle_to_object=get_angle_to_from ((int)examine_x, (int)examine_y, (int)zex_x, (int)zex_y);
//if the angle is roughly comparable to zex's zrot then it's in front of us
     angular_diff=get_diff(angle_to_object,ship_z_ang);
     if (angular_diff<capture_angle || angular_diff>360-capture_angle)	//good enough for me matey
      {
       if (get_distance_to_zex(object_counter)<within_distance) got_one=1;	//got a target!
      }
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while


}



int aim_from_zex_exc_obj(int object_to_exclude, int laser_cat,int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
extern int ship_z_ang;
int active_object_counter,object_counter,got_one;
float examine_x,examine_y;	//x and y of object being examined
float zex_x,zex_y;	//relative to zex
int angle_to_object,angular_diff;
int debug_loop_count;
int capture_angle;
//debug_loop_count=0;
active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable

capture_angle=(35*laser_cat)+difficulty;
if (capture_angle>80) capture_angle=80;	//tasers for example

while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1 ) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==baddie_object
         && (*ocb_ptr).object_list[object_counter].is_visible!=0
         && object_counter!=object_to_exclude)
    {
    //these laods are for debugging
     examine_x=(*ocb_ptr).object_list[object_counter].Oworldx;
     examine_y=(*ocb_ptr).object_list[object_counter].Oworldy;
//from the "angle to" we can compute whether these are in front or behind us.
//use of some nifty routines coming up...
     zex_x=(*ocb_ptr).object_list[zex].Oworldx;
     zex_y=(*ocb_ptr).object_list[zex].Oworldy;
     angle_to_object=get_angle_to_from ((int)examine_x, (int)examine_y, (int)zex_x, (int)zex_y);
//if the angle is roughly comparable to zex's zrot then it's in front of us
     angular_diff=get_diff(angle_to_object,ship_z_ang);
     if (angular_diff<capture_angle || angular_diff>360-capture_angle)	//good enough for me matey
      {
       if (get_distance_to_zex(object_counter)<within_distance) got_one=1;	//got a target!
      }
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while


}




//pick a target with a 50 degrees capture angle
//simple philosophy - examine the ocbs, looking for monsters
//when we find one we decide if it's in front or not.
//if it is, and it's in range, then we return that slot number
int aim_from_zex_rear(int laser_cat,int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
extern int ship_z_ang;
int active_object_counter,object_counter,got_one;
float examine_x,examine_y;	//x and y of object being examined
float zex_x,zex_y;	//relative to zex
int angle_to_object,angular_diff;
int debug_loop_count;
int capture_angle;

//debug_loop_count=0;
active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable

capture_angle=15*laser_cat;

while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1 ) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==baddie_object
         && (*ocb_ptr).object_list[object_counter].is_visible!=0)
    {
    //these laods are for debugging
     examine_x=(*ocb_ptr).object_list[object_counter].Oworldx;
     examine_y=(*ocb_ptr).object_list[object_counter].Oworldy;
//from the "angle to" we can compute whether these are in front or behind us.
//use of some nifty routines coming up...
     zex_x=(*ocb_ptr).object_list[zex].Oworldx;
     zex_y=(*ocb_ptr).object_list[zex].Oworldy;
     angle_to_object=get_angle_to_from ((int)examine_x, (int)examine_y, (int)zex_x, (int)zex_y);
//if the angle is roughly comparable to zex's zrot then it's in front of us
     angular_diff=get_diff(angle_to_object,ship_z_ang);
     if (angular_diff<180+capture_angle && angular_diff>180-capture_angle)	//good enough for me matey
      {
       if (get_distance_to_zex(object_counter)<within_distance) got_one=1;	//got a target!
      }
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while
}


//same as aim from zex but will pick a target irrespective of capture angle
int aim_from_zex_360(int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
extern int ship_z_ang;
int active_object_counter,object_counter,got_one;
//float examine_x,examine_y;	//x and y of object being examined
//float zex_x,zex_y;	//relative to zex
//int angular_diff;
int debug_loop_count;

//debug_loop_count=0;
active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable

while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim_from_zex looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==baddie_object
         && (*ocb_ptr).object_list[object_counter].is_visible!=0)
    {
       if (get_distance_to_zex(object_counter)<within_distance) got_one=1;	//got a target!
    
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while


}



//pick a target from on object with a 50 degrees capture angle
int aim_from_object(int aim_from, int aim_at_category, int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
extern int ship_z_ang;
int active_object_counter,object_counter,got_one;
float examine_x,examine_y;	//x and y of object being examined
float zex_x,zex_y;	//relative to zex
int angle_to_object,angular_diff;
int aim_from_object=0;
//int debug_loop_count=0;
active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable
 
while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim_from_object looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==aim_at_category
         && (*ocb_ptr).object_list[object_counter].is_visible!=0)
    {
    //these laods are for debugging
     examine_x=(*ocb_ptr).object_list[object_counter].Oworldx;
     examine_y=(*ocb_ptr).object_list[object_counter].Oworldy;
//from the "angle to" we can compute whether these are in front or behind us.
//use of some nifty routines coming up...
     zex_x=(*ocb_ptr).object_list[aim_from].Oworldx;
     zex_y=(*ocb_ptr).object_list[aim_from].Oworldy;
     angle_to_object=get_angle_to_from ((int)examine_x, (int)examine_y, (int)zex_x, (int)zex_y);
//if the angle is roughly comparable to zex's zrot then it's in front of us
     angular_diff=get_diff(angle_to_object,(*ocb_ptr).object_list[aim_from].Orotz);
     if (angular_diff<25 || angular_diff>335)	//good enough for me matey
      {
       if (get_distance_to_from(object_counter, aim_from)<within_distance) got_one=1;	//got a target!
      }
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while


}

//pick a target from on object with a 50 degrees capture angle
int aim_from_object_no_limit(int aim_from, int aim_at_category, int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
extern int ship_z_ang;
int active_object_counter,object_counter,got_one;
//float examine_x,examine_y;	//x and y of object being examined
//float zex_x,zex_y;	//relative to zex
//int angular_diff;
//int debug_loop_count=0;

active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable

while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim_from_no_limit looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==aim_at_category
         && (*ocb_ptr).object_list[object_counter].is_visible!=0 
         && (*ocb_ptr).object_list[object_counter].Oid!='EC01')	//dont pick on ejection capsules...
    {  
       
       if (get_distance_to_from(object_counter, aim_from)<within_distance) got_one=1;	//got a target!
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while


}

int aim_from_object_no_limit_wep(int aim_from, int aim_at_category, int within_distance)
{
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic ocb's
extern _3D ship_rotated_normal; //from engine
extern int dynamic_object_index;
extern int ship_z_ang;
int active_object_counter,object_counter,got_one;
//float examine_x,examine_y;	//x and y of object being examined
//float zex_x,zex_y;	//relative to zex
//int angular_diff;
//int debug_loop_count=0;

active_object_counter=0;
object_counter=0;
got_one=0;	//set to one when we find a monster that is aimable

while (active_object_counter<dynamic_object_index && got_one==0  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("aim_from_no_limit looped");

  if ((*ocb_ptr).object_list[object_counter].in_use==1) //if object is alive
   {
    active_object_counter++;
    //is this object a baddie and is it in view
    if ((*ocb_ptr).object_list[object_counter].object_category==aim_at_category
         && (*ocb_ptr).object_list[object_counter].is_visible!=0 
         )	//dont pick on ejection capsules...
    {  
       
       if (get_distance_to_from(object_counter, aim_from)<within_distance) got_one=1;	//got a target!
    }
   }
 object_counter++;	//the thing we return

 //¥ check next object
 }	//end of while
if (got_one==0) return -1;
else
return object_counter-1;	//cause it's inc'd before we get out of while


}




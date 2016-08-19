//objects_misc.c
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "3d_structures_def.h"
#include "object_misc.h"


//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
//searches static objects for an id, if found returns the slot of the object else -1
int find_dynamic_object_slot(unsigned long the_id)
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int	dynamic_object_index;	//how many dynamic objects
int return_val;

int match=0,active_object_count, object_count;
return_val=-1;

object_count=active_object_count=0;
while (active_object_count<dynamic_object_index  && object_count<MAX_NUMBER_OF_DYN_OBJECTS)
{
  if ((*ocb_ptr).object_list[object_count].in_use==1)
  {
   if ((*ocb_ptr).object_list[object_count].Oid==the_id)
   {
     match=1;
     return_val=object_count;
     break;
   }
   active_object_count++;
 }
 object_count++;
}
if (match==1) return return_val;	//slot of static definition of bg object
else return return_val;

}

//returns 0=no, 1=yes
int point_in_rect(int x, int y, LSRect the_rect)
{
if (x<the_rect.left) return 0;
if (x>the_rect.right) return 0;
if (y>the_rect.top) return 0;
if (y<the_rect.bottom) return 0;
return 1;
}

//returns 0=no, 1=yes
int point_in_float_rect(float x, float y, LSFloatRect the_rect)
{
if (x<the_rect.left) return 0;
if (x>the_rect.right) return 0;
if (y>the_rect.top) return 0;
if (y<the_rect.bottom) return 0;
return 1;
}


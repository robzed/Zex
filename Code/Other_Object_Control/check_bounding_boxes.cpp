//check_bounding_boxes
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "3d_structures_def.h"
#include "check_bounding_boxes.h"

#include "object_misc.h"


//this works on the principle of walking the lines.
//as we need speed more than anything else, we don't check all the points on a line
//against the bounding box of obj2, just some....
//all points are floats
//returns 0 =no collision
//1=obj1 collided with obj2
int check_bounding_box(int obj1, int obj2)
{
extern	DynObjectsFixedSize *ocb_ptr;
LSFloatRect obj2_rect;
_2D p1,p2;

//set up our compare rectangle. Note inversion of y
obj2_rect.top=(*ocb_ptr).object_list[obj2].box_max_translated.y;
obj2_rect.left=(*ocb_ptr).object_list[obj2].box_min_translated.x;
obj2_rect.bottom=(*ocb_ptr).object_list[obj2].box_min_translated.y;
obj2_rect.right=(*ocb_ptr).object_list[obj2].box_max_translated.x;

//now walk across the top of obj1 bounding box
//topleft =minx, maxy
//topright = maxx,maxy
p1.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.x);
p1.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.y);

p2.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.x);
p2.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.y);

if (walk_line_in_rect(p1,p2,obj2_rect)) return 1;

//now walk across the bottom of obj1 bounding box
//bottomleft =minx, miny
//bottomright = maxx,miny
p1.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.x);
p1.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.y);

p2.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.x);
p2.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.y);

if (walk_line_in_rect(p1,p2,obj2_rect)) return 1;

//now walk down the right side of obj1 bounding box
//topright =maxx, maxy
//bottomright = maxx,miny
p1.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.x);
p1.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.y);

p2.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.x);
p2.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.y);

if (walk_line_in_rect(p1,p2,obj2_rect)) return 1;

//now walk down the left side of obj1 bounding box
//topleft =minx, maxy
//bottomleft = minx,miny
p1.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.x);
p1.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_max_translated.y);

p2.x=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.x);
p2.y=static_cast<int>((*ocb_ptr).object_list[obj1].box_min_translated.y);

if (walk_line_in_rect(p1,p2,obj2_rect)) return 1;

return 0;
}

//walks the line from p1 to p2 seeing if any points are in the rect
//return 0=no, 1=yes
int walk_line_in_rect(_2D p1, _2D p2, LSFloatRect the_rect)
{
float dx,dy;
float sample_x,sample_y;
int i;	//i should be define as an intrinsic C var imho..

dx=p2.x-p1.x;
dy=p2.y-p1.y;

//we sample along four points of the line
dx=dx/4;
dy=dy/4;

sample_x=p1.x;
sample_y=p1.y;

for (i=0;i<4;i++)
	{
	if (point_in_float_rect(sample_x,sample_y,the_rect)) return 1;
	sample_x+=dx;
	sample_y+=dy;
	}
return 0;	//if we get here there is no collision
}



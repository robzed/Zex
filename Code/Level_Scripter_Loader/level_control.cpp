//level control.c
//SSB 230199

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include <NumberFormatting.h>

#include "monster1.h"
#include "level_control.h"
#include "3d_structures_def.h"
#include "dlp.h"
#include "instrum_int.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include "motherships.h"
#include "txtsplat.h"

#include "zex_misc.h"

float targ_x,targ_y;	//from TARG command in level file
float dest_x,dest_y;	//from dest command in level file
int level_complete;		//this is a three state var - 0=no, 1=main target destroyed, 2=player home
int can_do_next_level;	//0=no, 1=yes

extern mothership_nav_entry mothership_nav [10];



void draw_radar_line(float to_x, float to_y, float from_x, float from_y, unsigned char colour);



//computes and displays nav vectors on radar
//if level complete=0 then it's to target, if 1 then to home
int global_show_target_vector;

void show_nav()
{
float to_x=0;
float to_y=0;
float from_x,from_y;
int angle_to;
extern	DynObjectsFixedSize *ocb_ptr;
extern	float SinTable[360], CosTable[360];
extern kit_desc ship_items[ITEMS_COUNT];
extern int gRadarRange,number_of_motherships;

int distance;
int x1=445;
int y1=430;

int d2_x,d2_y,i;
unsigned char colour;

//draw an L on radar if long range selected

// if (gRadarRange == 1 && ship_items[KNORMAL_FUEL_TANK].value1!=0)
//
// {
//  add_to_dlp(DRAW_LINE_NO_CLIP,441,403,450,403,215,0);
//}
//  else
//  add_to_dlp(DRAW_LINE_NO_CLIP,441,403,450,403,220,0);
  
if (ship_items[KNORMAL_FUEL_TANK].value1==0) return;
if (ship_items[KNAV_COMPUTER].status<=0) return;	//haven't got one!
if (ship_items[KNAV_COMPUTER].status>1) return;	//Damaged

from_x=(*ocb_ptr).object_list[0].Oworldx;
from_y=(*ocb_ptr).object_list[0].Oworldy;



//do vector to motherships - these are in blue and we can handle up to 10 of them

for (i=0;i<number_of_motherships;i++)
 {
	 to_x= mothership_nav[i].x; to_y=mothership_nav[i].y;
	 
	 if (ship_items[KNAV_COMPUTER].modifier==2)	//super duper so coloyr
	 {
	  if (mothership_nav[i].allegiance==zex_object) colour=11;
	  else colour=12;
	  }
	 else 
	 {	
		colour=250;	//level 1 nav computer
	 }
	 draw_radar_line(to_x, to_y, from_x, from_y,colour);
 }

//do vector to current target - this is in green
if (global_show_target_vector==1)
{
if(level_complete==0)
 {
	to_x=targ_x; to_y=targ_y;
	draw_radar_line(to_x, to_y, from_x, from_y,10);	//was green
 }
else
if(level_complete==2)
 {
	to_x=dest_x; to_y=dest_y;
	draw_radar_line(to_x, to_y, from_x, from_y,10);
 }
 if (ship_items[KNAV_COMPUTER].modifier==2)	//super duper Nav Computer so display distance to go
 {
 int dist,colour;
 int number_of_chars;
 unsigned char number_string[12];
 extern int global_show_status,gShowGalacticMap;

	 //show distance (only if we are not showing the galactic map or summat
	if (!global_show_status && !gShowGalacticMap)
	{
		dist=get_distance_to_from_point((int)from_x,(int)from_y,(int)to_x,(int)to_y);
		ZexNumToString(dist/1000,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_of_chars=number_string[0]*8;	//width of string
		number_of_chars=(number_of_chars>>1);	//center the string
		number_string[0]=' ';
		if (dist>140000) colour=10;
		else
		colour=215;
	  
		SplatText_Small_Transparent ((char *)number_string,444-number_of_chars,411,colour);
	}
 }
}	//end global_show_target_vewctor


}




//--------------
//returns the index of the closest mothership in mothership_nav
//if wanted allegiance=-1 any mothership will do
int find_nearest_mothership_to_object(int object, int wanted_allegiance)
{
extern	  DynObjectsFixedSize *ocb_ptr;
int distance = 1000000000;
int zex_x,zex_y,i;
int to_x,to_y,temp;
int closest_index;
extern int gRadarRange,number_of_motherships;

closest_index=-1;
zex_x= static_cast<int>((*ocb_ptr).object_list[object].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[object].Oworldy);

for (i=0;i<number_of_motherships;i++)
 {
 to_x= static_cast<int>(mothership_nav[i].x); 
 to_y= static_cast<int>(mothership_nav[i].y);
 temp=ABS(get_distance_to_from_point(zex_x,zex_y,to_x,to_y));
 
 if (wanted_allegiance==-1)
 {
 if (temp<distance)
     {
     distance=temp;
     closest_index=i;
     }
 }
 else
 {
 if (temp<distance && mothership_nav[i].allegiance==wanted_allegiance)
     {
     distance=temp;
     closest_index=i;
     }
  }
 }

return closest_index;
}



//returns the index of the closest mothership in mothership_nav
//if wanted allegiance=-1 any mothership will do
//used to init travellers
int find_furthest_mothership_from_object(int object, int wanted_allegiance)
{
extern	  DynObjectsFixedSize *ocb_ptr;
int distance = 0;
int zex_x,zex_y,i;
int to_x,to_y,temp;
int closest_index;
extern int gRadarRange,number_of_motherships;

closest_index=-1;
zex_x= static_cast<int>((*ocb_ptr).object_list[object].Oworldx);
zex_y= static_cast<int>((*ocb_ptr).object_list[object].Oworldy);

for (i=0;i<number_of_motherships;i++)
 {
 to_x= static_cast<int>(mothership_nav[i].x); 
 to_y= static_cast<int>(mothership_nav[i].y);
 temp=ABS(get_distance_to_from_point(zex_x,zex_y,to_x,to_y));
 
 if (wanted_allegiance==-1)
 {
 if (temp>distance)
     {
     distance=temp;
     closest_index=i;
     }
 }
 else
 {
 if (temp>distance && mothership_nav[i].allegiance==wanted_allegiance)
     {
     distance=temp;
     closest_index=i;
     }
  }
 }

return mothership_nav[closest_index].slot;	//the ocb slot of the mothership
}



//Transform world coords to radar display coords
void draw_radar_line(float to_x, float to_y, float from_x, float from_y, unsigned char colour)
{
int angle_to;
extern	DynObjectsFixedSize *ocb_ptr;
extern	float SinTable[360], CosTable[360];
extern int camera_mode,ship_z_ang;
int distance;
int x1=445;
int y1=430;

int d2_x,d2_y;
extern int gRadarRange;


angle_to=get_angle_to_from((int)to_x,(int)to_y,(int)from_x,(int)from_y);	//0 is facing right
angle_to=360-angle_to;	//translate from Zex's warped view of reality.
if (camera_mode==2) 
{
	angle_to+=ship_z_ang;
}
//calc length of line
distance=get_distance_to_from_point((int)to_x, (int)to_y, (int)from_x, (int)from_y);

if (gRadarRange == 0)	//radar scaling (long/short range)
distance=distance/4096;
else
distance=distance/16384;


if (distance<5) distance=5;	//make sure line is a minimum length
else
if (distance>23) distance=23;	//and a maximum

//check for sin/cos index limits
if (angle_to<0) angle_to+=360;
if (angle_to>359) angle_to-=360;
	float aspect_ratio_x=1.5;
    d2_x= static_cast<int>(x1+((distance*COS(angle_to))*1.3));
    d2_y= static_cast<int>(y1+((distance*SIN(angle_to))*0.54));
	//now because the radar is oval rather than round we need to multiply the x coord by the aspect ratio
	//(Christmas morning and I'm multiplying by aspect ratios! life is good :))
	//width=130, height=54
    add_to_dlp(DRAW_RADAR_LINE,x1,y1,d2_x,d2_y,colour,0);

}




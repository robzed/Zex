#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include	"3d_structures_def.h"
#include	"zex_misc.h"
#include "monster1.h"
#include "aim.h"
#include "boid.h"
#include "fire_weapon.h"
#include "monster_control_misc.h"
typedef struct
{
int the_count;
int x_inc;
int y_inc;
} bcontrol;

bcontrol boid_array1 [] =
{
	{ 100,50,0 },
	{ 200,0,-50 },
	{ 300,-50,0 },
	{ 400,0,50 },
	{ 999999,0,0 },
};

//boid controller
//c1=velocity
//c2/c3=rotate around which point (x/y)
void handle_boid(int it)	//rotate around a circle
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int monsters_can_fire;

int zangle,wangle,delta_rot,d1,d2,control_use_1,i,count_entry,attacking_this;
float dx,dy,bz;
int zex_wx,zex_wy,obj_wx,obj_wy;

control_use_1=(*ocb_ptr).object_list[it].control_use_1;
//find count entry which is > cu1
i=0;

do
{
  count_entry=boid_array1[i].the_count;
  i++;
}
while (count_entry<control_use_1);

if (count_entry==999999) control_use_1=0;	//reset
else
{
 i--;
 (*ocb_ptr).object_list[it].Oworldx+=boid_array1[i].x_inc;
 (*ocb_ptr).object_list[it].Oworldy+=boid_array1[i].y_inc;
}

//do z
bz=(*ocb_ptr).object_list[it].Oworldz;
if ( (*ocb_ptr).object_list[it].control_use_2==0) (*ocb_ptr).object_list[it].control_use_2=7;
if (bz<-300) (*ocb_ptr).object_list[it].control_use_2=-(*ocb_ptr).object_list[it].control_use_2;
if (bz>100) (*ocb_ptr).object_list[it].control_use_2=-(*ocb_ptr).object_list[it].control_use_2;
bz+=(*ocb_ptr).object_list[it].control_use_2;
(*ocb_ptr).object_list[it].Oworldz=bz;


control_use_1++;
(*ocb_ptr).object_list[it].control_use_1=control_use_1;



if (find_something_to_attack(it,0,20000)==-1)
{
 return;	//didn't find anything
}

else
 {
 attacking_this=(*ocb_ptr).object_list[it].attack_what;
 if (monsters_can_fire==1 && RangedRdm(1,10)>5) fire_laser_at(attacking_this,it,2000);
 }
}



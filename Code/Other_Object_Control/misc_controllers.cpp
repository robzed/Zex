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

//misc controllers
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include	"3d_structures_def.h"

#include	"objects_controls.h"	//controller definitions
#include "misc_controllers.h"
#include "explode.h"
#include "object_control_top.h"
#include "sound.h"
#include "monster1.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
void handle_exploding_zex(int it)	//do the right thing to explode zex on eject
{
extern	DynObjectsFixedSize *ocb_ptr;

//slow to a stop
if ((*ocb_ptr).object_list[it].Omovedx>0) (*ocb_ptr).object_list[it].Omovedx-=1;
if ((*ocb_ptr).object_list[it].Omovedx<0) (*ocb_ptr).object_list[it].Omovedx+=1;
if ((*ocb_ptr).object_list[it].Omovedy>0) (*ocb_ptr).object_list[it].Omovedy-=1;
if ((*ocb_ptr).object_list[it].Omovedy<0) (*ocb_ptr).object_list[it].Omovedy+=1;
(*ocb_ptr).object_list[it].control_use_2--;

if ((*ocb_ptr).object_list[it].control_use_2==12) add_to_explode_list(it,3);
else
if ((*ocb_ptr).object_list[it].control_use_2==7) add_to_explode_list(it,5);

//else

if ((*ocb_ptr).object_list[it].control_use_2==14) 
    play_zsound_relative_to_zex(it,missile_explode);

if ((*ocb_ptr).object_list[it].control_use_2<3)
{
 add_to_explode_list(it,2);
 add_to_kill_list(it);
     play_zsound(explosion4, sound_high_pri, c1_chan1, sound_vol_7);
}

}




void handle_player_ejection_pod(int it)	//drive the ejection pod to the nearest earth mothership
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int tractor;

int mothership_wx,mothership_wy,obj_wx,obj_wy,zangle,wangle,d1,d2,distance;
float dx,dy;

//control 5 and 6 are x and y of nearest
//1 is current thrust
(*ocb_ptr).object_list[it].Orotdy=0;	//fix for eject after running out of fuel
(*ocb_ptr).object_list[it].Oroty=0;	//fix for eject after running out of fuel

 if (tractor==1) return;
 
  mothership_wx=(*ocb_ptr).object_list[it].control_use_4;	//x and y of mothership
  mothership_wy=(*ocb_ptr).object_list[it].control_use_7;

  obj_wx=static_cast<int>((*ocb_ptr).object_list[it].Oworldx);
  obj_wy=static_cast<int>((*ocb_ptr).object_list[it].Oworldy);

//cu3 is timer at start to get velocity up to 70
if ((*ocb_ptr).object_list[it].control_use_3>0)
{
(*ocb_ptr).object_list[it].control_use_3--;
if ((*ocb_ptr).object_list[it].control_use_3==0) play_zsound(zip, sound_high_pri, c1_chan1, sound_vol_7);
(*ocb_ptr).object_list[it].control_use_1+=1;
}
else	//initial velocity achieved
{
 distance=ABS(get_distance_to_from_point (obj_wx,obj_wy, mothership_wx, mothership_wy));	//this was really broke in 0.95
 if ((distance>50000) && ((*ocb_ptr).object_list[it].control_use_1<700)) 
    {
		(*ocb_ptr).object_list[it].control_use_1+=9;	//accelerate (less vicious than in 0.95)
	}
 else	//less than 50000 units away
 {
 if ((*ocb_ptr).object_list[it].control_use_1>50)	//Now much slower closer to motherships
    (*ocb_ptr).object_list[it].control_use_1-=6;	//and decelerate faster
 }
}



  zangle=get_angle_to_from(mothership_wx,mothership_wy,obj_wx,obj_wy);

   
//  ((*ocb_ptr).object_list[it].wanted_velocity)=zangle;	//ship yaw
   wangle=zangle;
//calc whether to turn left or right
   zangle=(*ocb_ptr).object_list[it].Orotz;
   
   if (wangle!=zangle)
   {

   d1=get_diff(zangle,wangle);	//calc whether to thurn left or right
   d2=get_diff(wangle,zangle);
   if (d1>d2) zangle+=1;		
   else
   zangle-=1;
   if (zangle>359) zangle-=360;
   if (zangle<0) zangle+=360;

   (*ocb_ptr).object_list[it].Orotz=zangle;
   }
   
//calc thrust components   


   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*(*ocb_ptr).object_list[it].control_use_1;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*(*ocb_ptr).object_list[it].control_use_1;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;


}



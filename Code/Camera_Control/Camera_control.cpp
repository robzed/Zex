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

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "3d_structures_def.h"
#include "camera.h"
#include	"zex_misc.h"
#include "angular_stuff.h"

int delayed_camera_z=0;
int camera_rock;	//set on heavy impact with player
int camera_spin_on_zex_death;	//set to non -ve when zex is killed

//array of camera rock angles. This array is backwards as camera_rock is set to a non-zero value to start it off 
int rock_angles[12]=
    {0,1,2,3,2,1,0,359,358,357,358,359};
    
float camera_zoom=DEFAULT_CAMERA_ZOOM;	//from input zoom control

int camera_startup=0;
int object_the_camera_is_pointed_at=0;
int camera_pointing_at_coords=0;
float coord_the_camera_is_pointed_at_x,coord_the_camera_is_pointed_at_y;

int camera_mode;	//0=top down, 1=above behind
int actual_cam_z_ang=0;

//camera mode 3 controls
float this_normal_x,last_normal_x;
int flip_down,flip_up,yrot_modifier;
int level_has_camera_control;
int disable_camera_control;
extern int ship_z_ang,game_over;

void reset_camera()
{
extern _3D camera_pos;
extern _3D camera_pos_delta;
extern int camera_rotation_z;	//obviously the camera rot
extern int camera_rotation_x;
extern int camera_rotation_y;
extern int camera_rotation_delta_z; //set this to !0 and caamera rots around z
extern	float SinTable[360], CosTable[360];

point_camera_at(0);

delayed_camera_z=0;
//camera_pos.x=-20000;	//now from level loader 13/1/98
//camera_pos.y=0;
//camera_pos.z=-17100;

camera_pos_delta.x=0;
camera_pos_delta.y=0;
camera_pos_delta.z=0;
    camera_rotation_z=0;
    camera_rotation_x=0;	//alter this for angular look
    camera_rotation_y=0;


//camera_rotation_z=camera_rotation_x=camera_rotation_y=0;
camera_rotation_delta_z=0;
camera_rock=-1;
camera_spin_on_zex_death=-1;

 camera_pos.z=-78550;
 camera_startup=1;	//tell camera control "not yet"
 
// camera_mode=2;
}


void camera_to_zex()
{
extern _3D camera_pos;
extern	DynObjectsFixedSize *ocb_ptr;

point_camera_at(0);

camera_pos.x=-((*ocb_ptr).object_list[0].Oworldx);
camera_pos.y=-((*ocb_ptr).object_list[0].Oworldy);

}


int death_delta=1;	//which way camera is rotating on death
int dd_count=0;	//after n iterations we change death_delta
int death_inc=0;
int prev_diff_ang=0;
void run_camera_control()
{
extern _3D camera_pos;
extern	_3D camera_pos_delta;
extern int camera_rotation_z, camera_rotation_x, camera_rotation_y;
extern int camera_rotation_delta_z,thrust,zex_alive,level_complete,Mode320;
extern _3D ship_rotated_normal;
extern int hide_zex;
extern	DynObjectsFixedSize *ocb_ptr;
float ship_x,offset_x;
int temp_thrust;
float wanted_cam_z;
//modes
//0=normal,
//1=in-cockpit, not used
//2=above, spins with zex
//3=normal with angle towards from (yrot)

if (level_has_camera_control==1 || disable_camera_control==1) return;	     //the level is controlling the camera

//if (camera_mode==1)	  //in-cockpit
//{
//  //in cockpit code
//      camera_rotation_z=90;
//      camera_rotation_x=0;	    //alter this for angular look
//      camera_rotation_y=270;
//  
//  if (camera_pointing_at_coords==0 && zex_alive==1)
//  {
//  ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldx);
//  //if the offset between camera and ship is greater than 2000 then we start miving the camera
//  offset_x=-ship_x-camera_pos.x;
//  offset_x/=5;
//  camera_pos.x+=offset_x;
//  
//  ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldy);
//  //if the offset between camera and ship is greater than 2000 then we start miving the camera
//  offset_x=-ship_x-camera_pos.y;
//  offset_x/=5;
//  camera_pos.y+=offset_x;
//  
//  //if (Mode320==0)
//  //{
//  camera_pos.y+=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
//  camera_pos.x+=ship_rotated_normal.x*(500*(camera_zoom/MIN_CAMERA_ZOOM));
//  
//  camera_rotation_x=(*ocb_ptr).object_list[0].Orotz;
//  
//  }
//  else
//  if (camera_pointing_at_coords==1)
//  {
//  ship_x=coord_the_camera_is_pointed_at_x;
//  //if the offset between camera and ship is greater than 2000 then we start miving the camera
//  offset_x=-ship_x-camera_pos.x;
//  offset_x/=6;
//  camera_pos.x+=offset_x;
//  
//  ship_x=coord_the_camera_is_pointed_at_y;
//  //if the offset between camera and ship is greater than 2000 then we start miving the camera
//  offset_x=-ship_x-camera_pos.y;
//  offset_x/=5;
//  camera_pos.y+=offset_x;
//  
//  //if (Mode320==0)
//  //{
//  camera_pos.y-=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
//  camera_pos.x-=ship_rotated_normal.x*(300*(camera_zoom/MIN_CAMERA_ZOOM));
//  }
//  
//  //else
//  //{
//  //camera_pos.y-=ship_rotated_normal.y*(100*(camera_zoom/MIN_CAMERA_ZOOM));
//  //camera_pos.x-=ship_rotated_normal.x*(160*(camera_zoom/MIN_CAMERA_ZOOM));
//  //}
//  //}
//  
//  //the faster we go, the further out the camera
//  //calc wanted camera.z
//    if (zex_alive==1)
//    temp_thrust=thrust;
//    else
//    temp_thrust=0;
//  
//    if (temp_thrust<0) temp_thrust=-temp_thrust;
//    
//    if (temp_thrust>delayed_camera_z+2)
//    delayed_camera_z+=2;	  //delay camera zoom
//    else
//    if (temp_thrust<delayed_camera_z-2)
//    delayed_camera_z-=2;
//  
//    if (delayed_camera_z>600) delayed_camera_z=600;
//    else
//    if (delayed_camera_z<0) delayed_camera_z=0;
//    
//    wanted_cam_z=-(delayed_camera_z*10+camera_zoom);
//  
//  
//  if (camera_startup==0)
//  {
//  
//    if (zex_alive==1) 
//    { 
//         if (level_complete!=3)
//         {
//          camera_pos.z=wanted_cam_z;
//  // if (Mode320==1) camera_pos.z*=2;
//          }
//         else
//          camera_pos.z-=5;
//    }
//    else
//    {
//    camera_pos.z-=30; 
//        camera_rotation_z++;
//  camera_pos.y-=300;
//    }
////    camera_pos.z=-2900;
//  }
//  else	//camera_startup==1
//  {
//       camera_pos.z+=1160; 
//       if (camera_pos.z>wanted_cam_z) camera_startup=0;
//  }
//  
//  
//  //last bit is if zex is hidden
//  
//    if (camera_spin_on_zex_death>=0)
//       {
//         camera_spin_on_zex_death--;
//         camera_rotation_delta_z=8;
//         if (camera_pos.z<-1000) camera_pos.z+=100;
//       }
//    else
//       {
//         camera_spin_on_zex_death=-1;
//         camera_rotation_delta_z=0;
//  //       camera_rotation_z=0;
//       }
//  
//  
//  return;
//
//}
//
//else	  //top down mode
if (camera_mode==0)	//normal mode
{
//
 //top down view code
 //camera linked to ship
 
        camera_rotation_y=0;
 
 if (camera_pointing_at_coords==0 && zex_alive==1)
 {
 ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldx);
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.x;
 offset_x/=5;
 camera_pos.x+=offset_x;
 
 ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldy);
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.y;
 offset_x/=5;
 camera_pos.y+=offset_x;
 
 //if (Mode320==0)
 //{
// camera_pos.y-=ship_rotated_normal.y*(290*(camera_zoom/(2000)));
// camera_pos.x-=ship_rotated_normal.x*(390*(camera_zoom/(2000)));
 
 camera_pos.y-=ship_rotated_normal.y*(300*(-camera_pos.z/(MIN_CAMERA_ZOOM)));
 camera_pos.x-=ship_rotated_normal.x*(550*(-camera_pos.z/(MIN_CAMERA_ZOOM)));
 
 }
 else
 if (camera_pointing_at_coords==1)
 {
 ship_x=coord_the_camera_is_pointed_at_x;
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.x;
 offset_x/=6;
 camera_pos.x+=offset_x;
 
 ship_x=coord_the_camera_is_pointed_at_y;
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.y;
 offset_x/=5;
 camera_pos.y+=offset_x;
 
 //if (Mode320==0)
 //{
 camera_pos.y-=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
 camera_pos.x-=ship_rotated_normal.x*(300*(camera_zoom/MIN_CAMERA_ZOOM));
 }
 
 //else
 //{
 //camera_pos.y-=ship_rotated_normal.y*(100*(camera_zoom/MIN_CAMERA_ZOOM));
 //camera_pos.x-=ship_rotated_normal.x*(160*(camera_zoom/MIN_CAMERA_ZOOM));
 //}
 //}
 
 //the faster we go, the further out the camera
 //calc wanted camera.z
   if (zex_alive==1)
   temp_thrust=thrust;
   else
   temp_thrust=0;
 
   if (temp_thrust<0) temp_thrust=-temp_thrust;
   
   if (temp_thrust>delayed_camera_z+2)
   delayed_camera_z+=2;	//delay camera zoom
   else
   if (temp_thrust<delayed_camera_z-2)
   delayed_camera_z-=2;
 
   if (delayed_camera_z>600) delayed_camera_z=600;
   else
   if (delayed_camera_z<0) delayed_camera_z=0;
   
   wanted_cam_z=-(delayed_camera_z*10+camera_zoom);
 
 
 if (camera_startup==0)
 {
 
   if (zex_alive==1) 
   { 
        if (level_complete!=3)
        {
         camera_pos.z=wanted_cam_z;
 // if (Mode320==1) camera_pos.z*=2;
         }
        else
         camera_pos.z-=5;
   }
   else
   {
   int temp;
   
   //zex is dead
   camera_pos.z-=30;
   camera_rotation_z+=death_delta;
      
   dd_count++;
   if (dd_count>80) 
   {
   dd_count=0;
   temp=RangedRdm(0,10);
   if (temp>7) death_delta=-1;
   else
   {
   if (temp>4) death_delta=1;
   else death_delta=0;
   }
//   death_inc+=death_delta;
//   dd_count=0;
//   if (death_inc>2)
//   death_delta=-death_delta;
//   if (death_inc<-2)
//   death_delta=-death_delta;
   }
 //camera_pos.y-=300;
   }
 }
 else	 //camera_startup==1
 {
      camera_pos.z+=1160; 
      if (camera_pos.z>wanted_cam_z) camera_startup=0;
 }
 
 
 //last bit is if zex is hidden
 
   if (camera_spin_on_zex_death>=0)
      {
        camera_spin_on_zex_death--;
        camera_rotation_delta_z=8;
        if (camera_pos.z<-1000) camera_pos.z+=100;
      }
   else
      {
        camera_spin_on_zex_death=-1;
        camera_rotation_delta_z=0;
        if (game_over==0) camera_rotation_z=0;
      }
 
 
 if (zex_alive==1)
 {
 if (camera_rock!=-1) 
  {
  if (camera_rock>11) camera_rock=11;	 //protect ourselves from forgetful programmers
  camera_rotation_z=rock_angles[camera_rock];
  camera_rotation_x=camera_rotation_z;
  camera_rock--;
  }
 }
     
 
 //rot camera
 camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
 if (camera_rotation_z > 359) { camera_rotation_z = camera_rotation_z - 359; }	 
 if (camera_rotation_z<0) { camera_rotation_z = camera_rotation_z + 360; }
 
 }
 
else	//top down with rotate
if (camera_mode==2)
{
int wanted_cam_z_ang;
int diff_ang;
extern int var_a,var_b;


//      camera_rotation_z=90;
//      camera_rotation_x=0;	    //alter this for angular look
      camera_rotation_y=0;
wanted_cam_z_ang=(*ocb_ptr).object_list[0].Orotz;
var_a=wanted_cam_z_ang;

//get diff
diff_ang=left_or_right(wanted_cam_z_ang, actual_cam_z_ang);	//-ve =left
//we need to protect against the sign switch at 180
if (diff_ang>70 || diff_ang<-70) 
{
diff_ang=prev_diff_ang;
}
prev_diff_ang=diff_ang;

var_b=diff_ang;

if (diff_ang<0)	//left
{
 diff_ang=-diff_ang;	//+ve
 diff_ang/=12;
 if (diff_ang==0) diff_ang=1;
 actual_cam_z_ang+=diff_ang;
}
else
if (diff_ang>0)	//+ve
{
 diff_ang/=12;
 if (diff_ang==0) diff_ang=1;
 actual_cam_z_ang-=diff_ang;
}

if (actual_cam_z_ang<0) actual_cam_z_ang+=360;
if (actual_cam_z_ang>359) actual_cam_z_ang-=360;

//if (actual_cam_z_ang<wanted_cam_z_ang) actual_cam_z_ang++;
//else
//if (actual_cam_z_ang>wanted_cam_z_ang) actual_cam_z_ang--;

//camera_rotation_z=360-actual_cam_z_ang;
camera_rotation_z=360-actual_cam_z_ang;

if (actual_cam_z_ang<0) actual_cam_z_ang=-actual_cam_z_ang;	//back to normal for next calc.

 if (camera_pointing_at_coords==0 && zex_alive==1)
 {
 ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldx);
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.x;
 offset_x/=6;
 camera_pos.x+=offset_x;
 ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldy);
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.y;
 offset_x/=5;
 camera_pos.y+=offset_x;
 
 //if (Mode320==0)
 //{
 //camera_pos.y-=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
 //camera_pos.x-=ship_rotated_normal.x*(300*(camera_zoom/MIN_CAMERA_ZOOM));
 
  camera_pos.y-=ship_rotated_normal.y*(250*(-camera_pos.z/(MIN_CAMERA_ZOOM)));
 camera_pos.x-=ship_rotated_normal.x*(400*(-camera_pos.z/(MIN_CAMERA_ZOOM)));

 
 }
 else
 if (camera_pointing_at_coords==1)
 {
 ship_x=coord_the_camera_is_pointed_at_x;
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.x;
 offset_x/=6;
 camera_pos.x+=offset_x;
 
 ship_x=coord_the_camera_is_pointed_at_y;
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.y;
 offset_x/=5;
 camera_pos.y+=offset_x;
 
 //if (Mode320==0)
 //{
 camera_pos.y-=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
 camera_pos.x-=ship_rotated_normal.x*(300*(camera_zoom/MIN_CAMERA_ZOOM));
 }
 
 //else
 //{
 //camera_pos.y-=ship_rotated_normal.y*(100*(camera_zoom/MIN_CAMERA_ZOOM));
 //camera_pos.x-=ship_rotated_normal.x*(160*(camera_zoom/MIN_CAMERA_ZOOM));
 //}
 //}
 
 //the faster we go, the further out the camera
 //calc wanted camera.z
   if (zex_alive==1)
   temp_thrust=thrust;
   else
   temp_thrust=0;
 
   if (temp_thrust<0) temp_thrust=-temp_thrust;
   
   if (temp_thrust>delayed_camera_z+2)
   delayed_camera_z+=2;	//delay camera zoom
   else
   if (temp_thrust<delayed_camera_z-2)
   delayed_camera_z-=2;
 
   if (delayed_camera_z>600) delayed_camera_z=600;
   else
   if (delayed_camera_z<0) delayed_camera_z=0;
   
   wanted_cam_z=-(delayed_camera_z*10+camera_zoom);
 
 
 if (camera_startup==0)
 {
 
   if (zex_alive==1) 
   { 
        if (level_complete!=3)
        {
         camera_pos.z=wanted_cam_z;
 // if (Mode320==1) camera_pos.z*=2;
         }
        else
         camera_pos.z-=5;
   }
   else
   {
   camera_pos.z-=30;
       camera_rotation_z++;
 
 //camera_pos.y-=300;
   }
 }
 else	 //camera_startup==1
 {
      camera_pos.z+=1160; 
      if (camera_pos.z>wanted_cam_z) camera_startup=0;
 }
 
 
 //last bit is if zex is hidden
 
   if (camera_spin_on_zex_death>=0)
      {
        camera_spin_on_zex_death--;
        camera_rotation_delta_z=8;
        if (camera_pos.z<-1000) camera_pos.z+=100;
      }
   else
      {
        camera_spin_on_zex_death=-1;
        camera_rotation_delta_z=0;
//        camera_rotation_z=0;
      }
 
 
// if (zex_alive==1)
// {
// if (camera_rock!=-1) 
//  {
//  if (camera_rock>11) camera_rock=11;	//protect ourselves from forgetful programmers
//  camera_rotation_z=rock_angles[camera_rock];
//  camera_rotation_x=camera_rotation_z;
//  camera_rock--;
//  }
// }
//     
 
 //rot camera
 camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
 if (camera_rotation_z > 359) camera_rotation_z = camera_rotation_z - 359;	 //check if >255
}



else	//top down look ahead mode
if (camera_mode==3)
{
//
 //top down view code
 //camera linked to ship
 //camera y angle is changed so camera look ahead of ship
 //we have to flip y rotation from 353 to 7 when normal.x changes from + to -
 //and back the other way in reverse
 this_normal_x=(*ocb_ptr).object_list[object_the_camera_is_pointed_at].object_normal_translated.x;

if (flip_down==0 && flip_up==0)
{
 if (this_normal_x<0 && last_normal_x>0) 
 {
 flip_down=1;
 flip_up=0;
 }
 else
 if (this_normal_x>0 && last_normal_x<0) 
 {
 flip_down=0;
 flip_up=1;
 }
 else
 {
 flip_down=0;
 flip_up=0;
 }
}

if (flip_down==1)
 {
 yrot_modifier++;
 if (yrot_modifier>359) yrot_modifier=0;
 if (yrot_modifier>6 && yrot_modifier<180) flip_down=0;
 }

if (flip_up==1)
 {
 yrot_modifier--;
 if (yrot_modifier<0) yrot_modifier=359;
 if (yrot_modifier<354 && yrot_modifier>180) flip_up=0;
 }


 camera_rotation_y=yrot_modifier;
 
 
 
 last_normal_x=(*ocb_ptr).object_list[object_the_camera_is_pointed_at].object_normal_translated.x;
 
 if (camera_pointing_at_coords==0 && zex_alive==1)
 {
 ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldx);
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.x;
 offset_x/=6;
 camera_pos.x+=offset_x;
 
 ship_x=((*ocb_ptr).object_list[object_the_camera_is_pointed_at].Oworldy);
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.y;
 offset_x/=5;
 camera_pos.y+=offset_x;
 
 //if (Mode320==0)
 //{
 camera_pos.y-=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
 camera_pos.x-=ship_rotated_normal.x*(300*(camera_zoom/MIN_CAMERA_ZOOM));
 
 
 }
 else
 if (camera_pointing_at_coords==1)
 {
 ship_x=coord_the_camera_is_pointed_at_x;
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.x;
 offset_x/=6;
 camera_pos.x+=offset_x;
 
 ship_x=coord_the_camera_is_pointed_at_y;
 //if the offset between camera and ship is greater than 2000 then we start miving the camera
 offset_x=-ship_x-camera_pos.y;
 offset_x/=5;
 camera_pos.y+=offset_x;
 
 //if (Mode320==0)
 //{
 camera_pos.y-=ship_rotated_normal.y*(150*(camera_zoom/MIN_CAMERA_ZOOM));
 camera_pos.x-=ship_rotated_normal.x*(300*(camera_zoom/MIN_CAMERA_ZOOM));
 }
 
 //else
 //{
 //camera_pos.y-=ship_rotated_normal.y*(100*(camera_zoom/MIN_CAMERA_ZOOM));
 //camera_pos.x-=ship_rotated_normal.x*(160*(camera_zoom/MIN_CAMERA_ZOOM));
 //}
 //}
 
 //the faster we go, the further out the camera
 //calc wanted camera.z
   if (zex_alive==1)
   temp_thrust=thrust;
   else
   temp_thrust=0;
 
   if (temp_thrust<0) temp_thrust=-temp_thrust;
   
   if (temp_thrust>delayed_camera_z+2)
   delayed_camera_z+=2;	//delay camera zoom
   else
   if (temp_thrust<delayed_camera_z-2)
   delayed_camera_z-=2;
 
   if (delayed_camera_z>600) delayed_camera_z=600;
   else
   if (delayed_camera_z<0) delayed_camera_z=0;
   
   wanted_cam_z=-(delayed_camera_z*10+camera_zoom);
 
 
 if (camera_startup==0)
 {
 
   if (zex_alive==1) 
   { 
        if (level_complete!=3)
        {
         camera_pos.z=wanted_cam_z;
 // if (Mode320==1) camera_pos.z*=2;
         }
        else
         camera_pos.z-=5;
   }
   else
   {
   camera_pos.z-=30; 
    camera_rotation_z++;
 //camera_pos.y-=300;
   }
 }
 else	 //camera_startup==1
 {
      camera_pos.z+=1160; 
      if (camera_pos.z>wanted_cam_z) camera_startup=0;
 }
 
 
 //last bit is if zex is hidden
 
   if (camera_spin_on_zex_death>=0)
      {
        camera_spin_on_zex_death--;
        camera_rotation_delta_z=8;
        if (camera_pos.z<-1000) camera_pos.z+=100;
      }
   else
      {
        camera_spin_on_zex_death=-1;
        camera_rotation_delta_z=0;
        
        if (game_over==0) camera_rotation_z=0;
      }
 
 
 if (zex_alive==1)
 {
 if (camera_rock!=-1) 
  {
  if (camera_rock>11) camera_rock=11;	 //protect ourselves from forgetful programmers
  camera_rotation_z=rock_angles[camera_rock];
  camera_rotation_x=camera_rotation_z;
  camera_rock--;
  }
 }
     
 
 //rot camera
 camera_rotation_z=camera_rotation_z+camera_rotation_delta_z;
 if (camera_rotation_z > 359) camera_rotation_z = camera_rotation_z - 359;	 //check if >255
 
 }

}





void get_camera_normal(_3D *normal_here)
{
//we need to synthsize a plane for the camera with the points going clockwise!
//frig for now
normal_here->x=0;
normal_here->y=0;
normal_here->z=1;	//camera normal
}


void point_camera_at(int the_slot)
{
 if (the_slot<0) return;	//oops
 object_the_camera_is_pointed_at=the_slot;
camera_pointing_at_coords=0;
}

void point_camera_at_coords(float x, float y)
{
coord_the_camera_is_pointed_at_x=x;
coord_the_camera_is_pointed_at_y=y;
camera_pointing_at_coords=1;
}



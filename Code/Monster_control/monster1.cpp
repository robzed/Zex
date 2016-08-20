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


//oldmac//#include <fp.h>
#include	"3d_structures_def.h"
#include	"object_control_top.h"
#include	"objects_controls.h"	//controller definitions
#include	"load_objects.h"
#include	"zex_misc.h"
#include "explode.h"
#include "sound.h"
#include "monster1.h"
#include "fire_weapon.h"
#include "Error_handler.h"
#include "angular_stuff.h"
#include "monster_control_misc.h"
#include "dlp.h"

//monster controller 1
//SB 13/12/98
//The small monster
//¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
void handle_monster1(int it)	//
{
//control 1 = refire timer
//control 2= ejected
//control 4 = flip
//control 5 = try to ram zex if set
//control 6 = thrust
extern	DynObjectsFixedSize *ocb_ptr;
extern _3D ship_rotated_normal; //from engine
extern int monsters_can_fire;
//load_dyn_object_pb the_params;
_3D object_rotated_normal;
int obj_x_ang,go_up,h_direction,obj_z_ang,attacking_this;
float shot_x,shot_y,shot_z,dx,dy;

//change for d18 for all monsters
//they must now find something to attack and check the thing is still alive
//before attacking
//fills in attack_what
if (find_something_to_attack(it,0,200000)==-1)
{
//go into a large circle
   (*ocb_ptr).object_list[it].Orotz--;
//calc thrust components   

   dy=(*ocb_ptr).object_list[it].object_normal_translated.y*20;	//current velocity
   dx=(*ocb_ptr).object_list[it].object_normal_translated.x*20;
//check if outside of play area and adjust dx and dy accordingly
   (*ocb_ptr).object_list[it].Omovedy=dy;
   (*ocb_ptr).object_list[it].Omovedx=dx;

 return;	//didn't find anything
}

return;
//attacking_this=(*ocb_ptr).object_list[it].attack_what;
//
//    object_rotated_normal=(*ocb_ptr).object_list[it].object_normal_translated;
//
// if ((*ocb_ptr).object_list[it].shield_value>40)	  //if we have enough power for control
// {
//    
//    
////move monster to follow player - x
////see if we need to gen a flip
//    if ( (*ocb_ptr).object_list[it].control_use_4==0) //if we're not doing a flip
//    {
//     h_direction=((*ocb_ptr).object_list[attacking_this].Oworldx - (*ocb_ptr).object_list[it].Oworldx);
//     if (h_direction<0 && object_rotated_normal.x>0)	  //zex is to left of monster and monster facing right
//     (*ocb_ptr).object_list[it].control_use_4=1;	  //set turn to left flag
//     else
//     if (h_direction>0 && object_rotated_normal.x<0)	  //zex is to right of monster and monster facing left
//     (*ocb_ptr).object_list[it].control_use_4=-1;	  //set turn to right flag
//    }
//
//// see if a turn is in progress
//   if ( (*ocb_ptr).object_list[it].control_use_4==-1) //we're turning to right
//   {
//     obj_z_ang=(*ocb_ptr).object_list[it].Orotz;
//     
//     if (obj_z_ang<360) obj_z_ang+=4;
//     if (obj_z_ang>355) 
//     {
//      obj_z_ang=0;
//      (*ocb_ptr).object_list[it].control_use_4=0;	  //turn completed
//     }
//     (*ocb_ptr).object_list[it].Orotz=obj_z_ang;
//   }
//   else
//   if ( (*ocb_ptr).object_list[it].control_use_4==1) //we're turning to right
//   {
//     obj_z_ang=(*ocb_ptr).object_list[it].Orotz;
//     
//     if (obj_z_ang<180) obj_z_ang+=4;
//     if (obj_z_ang>175) 
//     {
//      obj_z_ang=180;
//      (*ocb_ptr).object_list[it].control_use_4=0;	  //turn completed
//     }
//     (*ocb_ptr).object_list[it].Orotz=obj_z_ang;
//   }
//  
//#define MAXV 130
////move monster to follow player - y OR avoid player if just_hit
//    obj_x_ang=(*ocb_ptr).object_list[it].Orotx;
//    
//    if ((*ocb_ptr).object_list[it].just_hit>0)
//    {
//    go_up=((*ocb_ptr).object_list[it].just_hit-10)*100;	//+-800
//    }
//    else	  //if not just hit, follow player in y
//    {
//    go_up=((*ocb_ptr).object_list[attacking_this].Oworldy - (*ocb_ptr).object_list[it].Oworldy);
//    }
//    go_up=go_up/16;	  //decrease this to make monster react faster
//
//    if (object_rotated_normal.x<0)
//    {
////monster facing left
//   	  	  if (go_up>0)
//	  	  {
//	  	      if (go_up>MAXV) go_up=MAXV;
//	    	  	  if (obj_x_ang>315 || obj_x_ang<45)	    //up key
//	        	obj_x_ang=-go_up;
//   	  	  	  if (obj_x_ang>359) obj_x_ang-=359;
//   	  	  	  if (obj_x_ang<0) obj_x_ang+=359;
//	  	      if (obj_x_ang<316 && obj_x_ang>180) obj_x_ang=316;
//	  	  }
//
//   	  	  if (go_up<0)	  //down key
//	  	  {    if (go_up<-MAXV) go_up=-MAXV;
//
//	    	  	  if (obj_x_ang<45 || obj_x_ang>315)
//       		  obj_x_ang=-go_up;	  //right turn Clyde
//   	  	  	  if (obj_x_ang>359) obj_x_ang-=359;
//   	  	  	  if (obj_x_ang<0) obj_x_ang+=359;
// 	  	  	  if (obj_x_ang>44 && obj_x_ang<180) obj_x_ang=44;
//	  	   }
//
////auto center
//	   if (obj_x_ang>180)
//	    { //angle between 180 and 359
//	     ((*ocb_ptr).object_list[it].Oworldy)+=((360-obj_x_ang)*2);	//2 is engine acceleration
//       (*ocb_ptr).object_list[it].thrust_y= ((360-obj_x_ang)*2);	  //update thrust y
//       if (obj_x_ang!=0) obj_x_ang++;	//add to get back to zero
//
//	  }
//	  
// 
//     if (obj_x_ang<180)  //angle between 179 and zero - going down
//	    {
//	       ((*ocb_ptr).object_list[it].Oworldy)-=(obj_x_ang)*2;
//         (*ocb_ptr).object_list[it].thrust_y= -(obj_x_ang)*2;	//update thrust y
//	       
//	     	  if (obj_x_ang!=0) obj_x_ang--;
//      }	
//      }
//     else	  //monster facing right
//
//     {
//   	  	  if (go_up<0)
//	  	  {
//	  	      if (go_up<-MAXV) go_up=-MAXV;
//	    	  	  if (obj_x_ang>315 || obj_x_ang<45)	    //up key
//	        	obj_x_ang=go_up;
//   	  	  	  if (obj_x_ang>359) obj_x_ang-=359;
//   	  	  	  if (obj_x_ang<0) obj_x_ang+=359;
//	  	      if (obj_x_ang<316 && obj_x_ang>180) obj_x_ang=316;
//	  	  }
//
//   	  	  if (go_up>0)	  //down key
//	  	  {   
//	  	      if (go_up>MAXV) go_up=MAXV;
//
//	    	  	  if (obj_x_ang<45 || obj_x_ang>315)
//       		  obj_x_ang=go_up;	  //right turn Clyde
//   	  	  	  if (obj_x_ang>359) obj_x_ang-=359;
//   	  	  	  if (obj_x_ang<0) obj_x_ang+=359;
// 	  	  	  if (obj_x_ang>44 && obj_x_ang<180) obj_x_ang=44;
//	  	   }
//
////auto center
//	   if (obj_x_ang<180)
//	    { //angle between 180 and 359
//	     ((*ocb_ptr).object_list[it].Oworldy)+=((obj_x_ang)*2);	  //2 is engine acceleration
//       (*ocb_ptr).object_list[it].thrust_y= ((obj_x_ang)*2);	  //update thrust y
//      if (obj_x_ang!=0) obj_x_ang--;	  //add to get back to zero
//	    }
//	  
// 
//     if (obj_x_ang>180)  //angle between 179 and zero - going down
//	    {
//	       ((*ocb_ptr).object_list[it].Oworldy)-=(360-obj_x_ang)*2;
//       (*ocb_ptr).object_list[it].thrust_y= -((360-obj_x_ang)*2);	  //update thrust y
//
//	     	  if (obj_x_ang!=0) obj_x_ang++;
//      }	
////   	if (obj_x_ang>359) obj_x_ang-=359;
////   	if (obj_x_ang<0) obj_x_ang+=359;
//
//
//     } //end of else 
//	  	  
//
//
//
////store x ang in object
//   ((*ocb_ptr).object_list[it].Orotx)=obj_x_ang;
//  } //end of enough power for control
//
////check cannon fire time
//if ((*ocb_ptr).object_list[it].control_use_1<0 && (*ocb_ptr).object_list[it].control_use_4==0) //we're not turning
// {
//  (*ocb_ptr).object_list[it].control_use_1=RangedRdm(10,60);	  //set time to fire
//  if ((*ocb_ptr).object_list[it].shield_value>20 && get_distance_to_from(attacking_this,it)<30000)	  //we have enough strenght to fire and move
//  {
//
////fire cannon
//   if (monsters_can_fire==1) fire_not_so_weeny_cannon(it);
//
//   }	  //end of enough shields
//   
//   else	//not enough power left for control
//    if ( (*ocb_ptr).object_list[it].current_velocity==1)
//    {
//    ram_object(attacking_this,it,2);	  //as it says
//    return;
//    }
//
//   
//  }	  //end of time to fire 
// else	//not time to fire
//  (*ocb_ptr).object_list[it].control_use_1-=1;	//dec time to fire
//   //go out of control
//   if ((*ocb_ptr).object_list[it].shield_value<20)
//   {
////generate ejection pod or rram mode
////   if (RangedRdm(0,10)>6) 
////   {
////   Debugger();
////   (*ocb_ptr).object_list[it].current_velocity=1;	  //set raming mode
////   }
////   else
//   if ((*ocb_ptr).object_list[it].control_use_2==0)	  //havent ejected yet
//   {
//    (*ocb_ptr).object_list[it].Orotdx=1;
//    (*ocb_ptr).object_list[it].Orotdy=1;
//    (*ocb_ptr).object_list[it].Orotdz=1;
//    (*ocb_ptr).object_list[it].Omovedx=3;
//    (*ocb_ptr).object_list[it].Omovedy=1;	  //shipp loses control and rotates
//   	  
//   	  (*ocb_ptr).object_list[it].control_use_2=1;	  //have now!
//    fire_ejection_pod(it);
//
//   	  }	  //end of haven't ejected yet
//   }
//
//
//  if ((*ocb_ptr).object_list[it].just_hit>0) (*ocb_ptr).object_list[it].just_hit--;	  //clear down just hit for next frame
}





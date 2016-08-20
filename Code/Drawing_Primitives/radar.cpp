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

// Name:   		   Radar.c
// Description:		These routines, sort of the viewing of the Radar  
//			
// Date Started:	10/12/98
// Author(s):		ID
//
// Copyright(c) Lightsoft Software   		

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <ToolUtils.h>
//oldmac//#include <resources.h>
#include "radar.h"
#include "Error_handler.h"
//oldmac//#include "processor.h"
#include "pictures.h"
#include "OGL.h"

#include"zex_misc.h"

#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif
#include "instrum_int.h"


#define NONE 0

#define ROW_SIZE 130
#define CHAR_LINES 54


void _Draw_to_Double_Buffer(char *char_ptr, int x_pos, int y_pos);
void _Draw_Enemy(int colour, int x_pos, int y_pos); 
void _Draw_Dot(int colour, int x_pos, int y_pos);

static Ptr double_buffer;
static int screen_width;
Handle radar_ptr;
int inval_area [54] [2];
int do_splat_blank;


void InitRadar()
 {
 do_splat_blank=1;	//we only blank radar every other frame
 
 radar_ptr = (Handle) LS_GetResource('ZRAW',135);  //Get the Handle to the Resource 
 if (radar_ptr==0) report_error_c("Resource missing: ZRAW 135","",4);
 LS_HLock (radar_ptr);
 inval_area [0] [0] = 65;
 inval_area [0] [1] = 65;
 inval_area [1] [0] = 65; 
 inval_area [1] [1] = 65;
 inval_area [2] [0] = 49;
 inval_area [2] [1] = 77;
 inval_area [3] [0] = 42;
 inval_area [3] [1] = 89;
 inval_area [4] [0] = 39; 
 inval_area [4] [1] = 92;
 inval_area [5] [0] = 31;
 inval_area [5] [1] = 98;
 inval_area [6] [0] = 28;
 inval_area [6] [1] = 102;
 inval_area [7] [0] = 25; 
 inval_area [7] [1] = 106;
 inval_area [8] [0] = 22;
 inval_area [8] [1] = 108;
 inval_area [9] [0] = 20;
 inval_area [9] [1] = 110;
 inval_area [10] [0] = 17;
 inval_area [10] [1] = 112;
 inval_area [11] [0] = 15; 
 inval_area [11] [1] = 115;
 inval_area [12] [0] = 13;
 inval_area [12] [1] = 116;
 inval_area [13] [0] = 12;
 inval_area [13] [1] = 118;
 inval_area [14] [0] = 11; 
 inval_area [14] [1] = 119;
 inval_area [15] [0] = 9;
 inval_area [15] [1] = 121;
 inval_area [16] [0] = 7;
 inval_area [16] [1] = 122;
 inval_area [17] [0] = 6; 
 inval_area [17] [1] = 124;
 inval_area [18] [0] = 5;
 inval_area [18] [1] = 125;
 inval_area [19] [0] = 4;
 inval_area [19] [1] = 125;
 
 inval_area [20] [0] = 4;
 inval_area [20] [1] = 126;
 inval_area [21] [0] = 3; 
 inval_area [21] [1] = 127;
 inval_area [22] [0] = 2;
 inval_area [22] [1] = 127;
 inval_area [23] [0] = 2;
 inval_area [23] [1] = 128;
 inval_area [24] [0] = 2;
 inval_area [24] [1] = 128;
 inval_area [25] [0] = 2;
 inval_area [25] [1] = 126;
 inval_area [26] [0] = 2;
 inval_area [26] [1] = 126;
 inval_area [27] [0] = 2; 
 inval_area [27] [1] = 126;
 inval_area [28] [0] = 2;
 inval_area [28] [1] = 126;
 inval_area [29] [0] = 2;
 inval_area [29] [1] = 126;
 inval_area [30] [0] = 2;
 inval_area [30] [1] = 126;
 inval_area [31] [0] = 2; 
 inval_area [31] [1] = 126;
 inval_area [32] [0] = 3;
 inval_area [32] [1] = 125;
 inval_area [33] [0] = 4;
 inval_area [33] [1] = 125;
 inval_area [34] [0] = 4; 
 inval_area [34] [1] = 124;
 inval_area [35] [0] = 5;
 inval_area [35] [1] = 123;
 inval_area [36] [0] = 6;
 inval_area [36] [1] = 123;
 inval_area [37] [0] = 7; 
 inval_area [37] [1] = 121;
 inval_area [38] [0] = 8;
 inval_area [38] [1] = 120;
 inval_area [39] [0] = 9;
 inval_area [39] [1] = 119;
 
 inval_area [40] [0] = 11;
 inval_area [40] [1] = 117;
 inval_area [41] [0] = 12; 
 inval_area [41] [1] = 116;
 inval_area [42] [0] = 15;
 inval_area [42] [1] = 114;
 inval_area [43] [0] = 16;
 inval_area [43] [1] = 112;
 inval_area [44] [0] = 18; 
 inval_area [44] [1] = 110;
 inval_area [45] [0] = 21;
 inval_area [45] [1] = 107;
 inval_area [46] [0] = 24;
 inval_area [46] [1] = 105;
 inval_area [47] [0] = 27; 
 inval_area [47] [1] = 101;
 inval_area [48] [0] = 30;
 inval_area [48] [1] = 98;
 inval_area [49] [0] = 35;
 inval_area [49] [1] = 94;
 inval_area [50] [0] = 40;
 inval_area [50] [1] = 89;
 inval_area [51] [0] = 46; 
 inval_area [51] [1] = 82;
 inval_area [52] [0] = 57;
 inval_area [52] [1] = 71;
 inval_area [53] [0] = 65;
 inval_area [53] [1] = 65;
 }



 void update_radar()
 {
 

  int _start_point;
  int _start_y;
  int zex_x;
  int zex_y;
  int enemy_x;
  int enemy_y;
  int i; 
  int splat_colour;
  int range_x, range_y;
  int divisor;
  
  int active_object_counter; 
  OSType ob_type; 
  
extern int dynamic_object_index;  
extern DynObjectsFixedSize *ocb_ptr;
extern int objs_displayed;
extern int gRadarRange,fps;
extern kit_desc ship_items[ITEMS_COUNT];
extern	float SinTable[360], CosTable[360];
extern int camera_mode,ship_z_ang,show_control_panel;
extern int OGL_active;
extern int short_range_reticule_ref,long_range_reticule_ref;
extern LSRAW* short_range_ret_rgba;
extern LSRAW* long_range_ret_rgba;
extern int global_show_status;
int debug_loop_count;
 float normx,normy;

//debug_loop_count=0;
_start_point = 378;
_start_y = 403; 

#define OGL_START_Y 24
#define OGL_START_X 378

if (gRadarRange == 0 || ship_items[KLONG_RANGE_RADAR].status!=1 )	 //short range or long range u/s
   {
    range_x = 26000*3;
    range_y = 32400*3;
    divisor = 400*3;
    
    if (OGL_active==1)
    {
      if(show_control_panel==0 && global_show_status==0)	//if we are showing the cp OR we are not showing inv screen

      {
     
        if (short_range_reticule_ref==-1)
        short_range_reticule_ref = set_as_constant_image(short_range_ret_rgba, OGL_START_X,OGL_START_Y,
                         OGL_START_X+short_range_ret_rgba->pwidth, OGL_START_Y+short_range_ret_rgba->pheight);
        delete_constant_image(long_range_reticule_ref); 
        long_range_reticule_ref=-1;   
      }
      else
      {
       delete_constant_image(short_range_reticule_ref);
       short_range_reticule_ref=-1;   
       delete_constant_image(long_range_reticule_ref); 
       long_range_reticule_ref=-1;   
      }
    }
     else
    {
    radar_ptr = (Handle) LS_GetResource('ZRAW',135);  //Get the Handle to the Resource 
    if (radar_ptr==0) report_error_c("Resource missing: ZRAW 135","",4);
    LS_HLock (radar_ptr);
    }
    
   }
else
  {
   range_x = 130000*3;
   range_y = 162000*3;
   divisor = 2000*3;
   
       if (OGL_active==1)
    {
      if(show_control_panel==0 && global_show_status==0)	//if we are showing the cp OR we are not showing inv screen
      {
       if (long_range_reticule_ref==-1)
       long_range_reticule_ref = set_as_constant_image(long_range_ret_rgba, OGL_START_X,OGL_START_Y,
                         OGL_START_X+long_range_ret_rgba->pwidth+1, OGL_START_Y+long_range_ret_rgba->pheight);
       delete_constant_image(short_range_reticule_ref);
       short_range_reticule_ref=-1;   
      }
      else
      {
       delete_constant_image(short_range_reticule_ref);
       short_range_reticule_ref=-1;   
       delete_constant_image(long_range_reticule_ref); 
       long_range_reticule_ref=-1;   
      }
    }
    else
    {
   radar_ptr = (Handle) LS_GetResource('ZRAW',143);  //Get the Handle to the Resource 
   if (radar_ptr==0) report_error_c("Resource missing: ZRAW 143","",4);
   LS_HLock (radar_ptr);
    }
  }   


//tweak sb 200499
//if fps drops below 26 then we only erase the radar every other frame
//in an effort to provide cycles to main engine
//if (fps<26)
//{
// do_splat_blank=do_splat_blank^1;
// if (do_splat_blank)
// SplatBlankRadar(_start_point,403);
//}
//else
if (OGL_active==0)
{
if(show_control_panel==0) SplatBlankRadar(_start_point,403);
}

if(ship_items[0].status!=1) return;	//no radar
   
    
zex_x =  static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
zex_y =  static_cast<int>((*ocb_ptr).object_list[0].Oworldy);
    
active_object_counter = 0;    

i = 0;
      splat_colour = 0;

while (active_object_counter<dynamic_object_index  && i<MAX_NUMBER_OF_DYN_OBJECTS)	//process all "in use" objects
 {
//Debug code
//  debug_loop_count++;
//  if (debug_loop_count>10000) DebugStr("radar looped");

  if ((*ocb_ptr).object_list[i].in_use==1) //if object is alive
    {    
    bool can_plot;
    enemy_x =  static_cast<int>((*ocb_ptr).object_list[i].Oworldx);
    enemy_y =  static_cast<int>((*ocb_ptr).object_list[i].Oworldy);
    ob_type = (*ocb_ptr).object_list[i].Oid;
    
//Intel and PPC versions of string checking (Intel version was always showing dust because the bytes when compared as strings were backwards)
	UInt32 radar_type_upper_1_char=(ob_type&0xff000000)|0x202020;	//upper char
	UInt32 radar_type_upper_2_char=(ob_type&0xffff0000)|0x2020;	//upper 2 chars
	UInt32 radar_type_upper_3_char=(ob_type&0xffffff00)|0x20;	//upper 3 chars
	
	splat_colour=0; 
	can_plot=true;
	
	if (radar_type_upper_2_char=='PN  ') can_plot=false;		//planet - don't plot
	if (radar_type_upper_1_char=='D   ') can_plot=false;		//dust - don't plot

	if (can_plot==true)
	{
	if ((*ocb_ptr).object_list[i].object_category == zex_object)	//green
	{
      if ( radar_type_upper_2_char=='MI  '/*bp[0] == 'M'  &&  bp[1] == 'I'*/)	//missile
      {
      splat_colour = 180;
      }
      else
      splat_colour = 225;	//some other human object that isn't a missile
	}  //not human object
	else if ((*ocb_ptr).object_list[i].object_category == baddie_object)	//Red
	{
		 if (radar_type_upper_2_char=='MI  ')	//R'mparian missile
		  {
			splat_colour = 180;
		  }
		 else if ( radar_type_upper_3_char=='LAS '/*bp[0] == 'L' &&  bp[1] == 'A' &&  bp[1] == 'S'*/)    
		  {
			splat_colour = 180;	//R'mparian laser
		  }
		else 
			splat_colour = 215;	//Some other R'mparian object
    }	//baddie object
  else if ((*ocb_ptr).object_list[i].object_category == scenary_object)		//Yellow
   {
    if ( radar_type_upper_2_char=='MI  ' /*bp[0] == 'M' &&  bp[1] == 'I'*/)
      {
      splat_colour = 180;	//Third party missile
      }
    else if ( radar_type_upper_3_char=='LAS ' /*bp[0] == 'L'  &&  bp[1] == 'A' &&  bp[1] == 'S'*/)    
      {
      splat_colour = 180;	//third party laser
      }   
    else 
     splat_colour = 5;	//thrid party object
   }	//Scenary object
     
 if ((enemy_x >=  zex_x - range_x)  &&  (enemy_x <=  zex_x + range_x))	//if in range of display
   {
       
    enemy_x -= zex_x;
    enemy_x /= divisor;
    enemy_x += 65;
    if (((enemy_x < 0) || (enemy_x > 130)))  report_error_c("Invalid X Co-Ord","",4);
      
    if ((enemy_y >=  zex_y - range_y) && (enemy_y <=  zex_y + range_y))
     {
      enemy_y = zex_y - enemy_y;
      enemy_y /= (divisor * 3);;
      enemy_y += 27;
      if (((enemy_y < 0) || (enemy_y > 54)))  report_error_c("Invalid Y Co-Ord %d","",enemy_y);
 
 
   if (camera_mode==2)
  {
  int ang_to;
  normx=enemy_x-65;	//transform to 0
  normy=enemy_y-27;
  ang_to=ship_z_ang;	//-90;
//  if (ang_to<0) ang_to+=360;
//  if (ang_to>359) ang_to-=360;

  enemy_x= static_cast<int>(normx*COS(ang_to)-normy*SIN(ang_to));	//rotate
  enemy_y= static_cast<int>(normy*COS(ang_to)+normx*SIN(ang_to));
  enemy_x+=65; enemy_y+=27;
//  newx+=443; newy+=430;	//transform back to where they should be
 }
        
      _start_point += enemy_x;
      _start_y += enemy_y;
        
      #if OGL==0	//we do not need to do this if OGL
      if ((enemy_x >= inval_area[(enemy_y -1)][0]) && (enemy_x <= inval_area[(enemy_y -1)][1]))
       {
      #endif
            {
             if (splat_colour == 180) 
//             if ((splat_colour == 225) || (splat_colour == 180)) 
             {
              #ifdef SHOW_CANNONS
//              SplatEnemy(splat_colour,_start_point,_start_y);
              #endif
              }
             else 

             if (splat_colour==231 || splat_colour==0)	//filter out dust
             {
             }
             else
              {
              SplatEnemy(splat_colour,_start_point,_start_y);
              }


            } 
        #if OGL==0
        } 
        #endif
		
		}	//End of plot
skip_plot:
        _start_point = 378;
  		_start_y = 403; 

        }
    }
   active_object_counter++;  
  }  
i++;           
} // end of while     
    
    
    
   
  
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
extern int OGL_active;

void SplatBlankRadar(int x_pos, int y_pos)
 {
 if (OGL_active==1)
 {} 
 else
  _Draw_to_Double_Buffer(*radar_ptr,x_pos,y_pos);
  
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatEnemy(int colour, int x_pos, int y_pos)
 {
 extern	float SinTable[360], CosTable[360];
 extern int camera_mode,ship_z_ang;
 int newx,newy;
 float normx,normy;
 
// //rotate if camera_mode==2
//  if (camera_mode==2)
//  {
//  normx=x_pos-443;	  //transform to 0
//  normy=y_pos-430;
//  newx=normx*COS(ship_z_ang)-normy*SIN(ship_z_ang);	  //rotate
//  newy=normy*COS(ship_z_ang)+normx*SIN(ship_z_ang);
//  newx+=443; newy+=430;	//transform back to where they should be
//  _Draw_Enemy(colour,newx,newy);
//  }
//  else
  _Draw_Enemy(colour,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void _Draw_to_Double_Buffer(char *char_ptr, int x_pos, int y_pos)
  {
extern Ptr screenaddr;
extern int screenwidth;
char *db_ptr;
int a,b;

double_buffer=screenaddr;
screen_width=screenwidth;



 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < CHAR_LINES; b++)
  {
    for(a=0; a < ROW_SIZE; a++)
     {
      *db_ptr =  *char_ptr;
      char_ptr++;
      db_ptr++;
     }
  db_ptr-=ROW_SIZE;
  db_ptr+=screen_width; 
 }
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void _Draw_Enemy(int colour, int x_pos, int y_pos)
 {
 extern Ptr screenaddr;
 extern int screenwidth;

 char *db_ptr;


#if OGL==1
OGL_plot( x_pos, y_pos, colour);
#else
 double_buffer=screenaddr;
 screen_width=screenwidth;

 db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));
 *db_ptr =  colour;
#endif
 
 }




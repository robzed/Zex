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

// Name:   		    Thrust.c
// Description:		These routines, sort out the thrust meter  
//			
// Date Started:	10/12/98
// Author(s):		ID
//
// Copyright(c) Lightsoft Software   		
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"


//oldmac//#include <resources.h>
//oldmac//#include <ToolUtils.h>
#include "thrust.h"
#include "Error_handler.h"
#include "game_defs.h"

#include "zex_misc.h"

#define NONE 0

#define CHAR_OFFSET 95  // in Pixels to the next character  
#define ROW_SIZE 5
#define CHAR_LINES 19

void SplatThr_pos(int ptr, int x_pos, int y_pos);
void SplatThr_neg(int ptr, int x_pos, int y_pos);
void _Draw_to_Double_Buffer_Thrust(char *char_ptr, int x_pos, int y_pos);

 
static Ptr double_buffer;
static int screen_width;
Handle thr_ptr;
Handle dwn_ptr;


void InitThrust()
 {
 thr_ptr = (Handle) LS_GetResource('ZRAW',138);  //Get the Handle to the Resource 
 if (thr_ptr==0) report_error_c("Resource missing: ZRAW 138","",4);
 LS_HLock(thr_ptr);
 
 dwn_ptr = (Handle) LS_GetResource('ZRAW',139);  //Get the Handle to the Resource 
 if (dwn_ptr==0) report_error_c("Resource missing: ZRAW 139","",4);
 LS_HLock(dwn_ptr);
 }


 
 void update_thrust(int thrust)
 {
  
 
  int _start_point;
  int _start_y;
  int neg_y;
  int i; 
  
  if (thrust>320) thrust=320;
  if (thrust<-100) thrust=-100;
  
  _start_point = 349;
  _start_y = 439;
  neg_y = 459;
 
 if (thrust > 0)
 {
  SplatThr_neg(19,_start_point,neg_y);
//change 210599 for max thrust=360, each seg is 18
  if ((thrust > 0) && (thrust < 19)) { SplatThr_pos(1,_start_point,_start_y);}
  else if ((thrust > 18) && (thrust < 37)) { SplatThr_pos(2,_start_point,_start_y);} 
  else if ((thrust > 36) && (thrust < 55)) { SplatThr_pos(3,_start_point,_start_y);} 
  else if ((thrust > 54) && (thrust < 73)) { SplatThr_pos(4,_start_point,_start_y);} 
  else if ((thrust > 72) && (thrust < 91)) { SplatThr_pos(5,_start_point,_start_y);} 
  else if ((thrust > 90) && (thrust < 109)) { SplatThr_pos(6,_start_point,_start_y);} 
  else if ((thrust > 108) && (thrust < 127)) { SplatThr_pos(7,_start_point,_start_y);} 
  else if ((thrust > 126) && (thrust < 145)) { SplatThr_pos(8,_start_point,_start_y);} 
  else if ((thrust > 144) && (thrust < 163)) { SplatThr_pos(9,_start_point,_start_y);} 
  else if ((thrust > 162) && (thrust < 181)) { SplatThr_pos(10,_start_point,_start_y);} 
  else if ((thrust > 180) && (thrust < 199)) { SplatThr_pos(11,_start_point,_start_y);} 
  else if ((thrust > 198) && (thrust < 217)) { SplatThr_pos(12,_start_point,_start_y);} 
  else if ((thrust > 216) && (thrust < 235)) { SplatThr_pos(13,_start_point,_start_y);} 
  else if ((thrust > 234) && (thrust < 253)) { SplatThr_pos(14,_start_point,_start_y);} 
  else if ((thrust > 252) && (thrust < 271)) { SplatThr_pos(15,_start_point,_start_y);}
  else if ((thrust > 270) && (thrust < 289)) { SplatThr_pos(16,_start_point,_start_y);} 
  else if ((thrust > 288) && (thrust < 307)) { SplatThr_pos(17,_start_point,_start_y);} 
  else if ((thrust > 306) && (thrust < 325)) { SplatThr_pos(18,_start_point,_start_y);} 
  else if ((thrust > 325) && (thrust < 344)) { SplatThr_pos(19,_start_point,_start_y);} 


//  if ((thrust > 0) && (thrust < 13)) { SplatThr_pos(1,_start_point,_start_y);}
//  else if ((thrust > 12) && (thrust < 25)) { SplatThr_pos(2,_start_point,_start_y);} 
//  else if ((thrust > 24) && (thrust < 37)) { SplatThr_pos(3,_start_point,_start_y);} 
//  else if ((thrust > 36) && (thrust < 49)) { SplatThr_pos(4,_start_point,_start_y);} 
//  else if ((thrust > 48) && (thrust < 61)) { SplatThr_pos(5,_start_point,_start_y);} 
//  else if ((thrust > 60) && (thrust < 73)) { SplatThr_pos(6,_start_point,_start_y);} 
//  else if ((thrust > 72) && (thrust < 85)) { SplatThr_pos(7,_start_point,_start_y);} 
//  else if ((thrust > 84) && (thrust < 97)) { SplatThr_pos(8,_start_point,_start_y);} 
//  else if ((thrust > 96) && (thrust < 109)) { SplatThr_pos(9,_start_point,_start_y);} 
//  else if ((thrust > 108) && (thrust < 121)) { SplatThr_pos(10,_start_point,_start_y);} 
//  else if ((thrust > 120) && (thrust < 133)) { SplatThr_pos(11,_start_point,_start_y);} 
//  else if ((thrust > 132) && (thrust < 145)) { SplatThr_pos(12,_start_point,_start_y);} 
//  else if ((thrust > 144) && (thrust < 167)) { SplatThr_pos(13,_start_point,_start_y);} 
//  else if ((thrust > 166) && (thrust < 181)) { SplatThr_pos(14,_start_point,_start_y);} 
//  else if ((thrust > 180) && (thrust < 191)) { SplatThr_pos(15,_start_point,_start_y);}
//  else if ((thrust > 190) && (thrust < 201)) { SplatThr_pos(16,_start_point,_start_y);} 
//  else if ((thrust > 200) && (thrust < 208)) { SplatThr_pos(17,_start_point,_start_y);} 
//  else if ((thrust > 207) && (thrust < 215)) { SplatThr_pos(18,_start_point,_start_y);} 
//  else if ((thrust > 214) && (thrust < 221)) { SplatThr_pos(19,_start_point,_start_y);} 
//   
 }
 else if (thrust < 0)
 {
 SplatThr_pos(0,_start_point,_start_y);
 if ((thrust < 0) && (thrust > -7)) { SplatThr_neg(18,_start_point,neg_y);}
  else if ((thrust < -6) && (thrust > 25)) { SplatThr_neg(17,_start_point,neg_y);} 
  else if ((thrust < -12) && (thrust > -19)) { SplatThr_neg(16,_start_point,neg_y);} 
  else if ((thrust < -18) && (thrust > -25)) { SplatThr_neg(15,_start_point,neg_y);} 
  else if ((thrust < -24) && (thrust > -31)) { SplatThr_neg(14,_start_point,neg_y);} 
  else if ((thrust < -30) && (thrust > -37)) { SplatThr_neg(13,_start_point,neg_y);} 
  else if ((thrust < -36) && (thrust > -43)) { SplatThr_neg(12,_start_point,neg_y);} 
  else if ((thrust < -42) && (thrust > -49)) { SplatThr_neg(11,_start_point,neg_y);} 
  else if ((thrust < -48) && (thrust > -52)) { SplatThr_neg(10,_start_point,neg_y);} 
  else if ((thrust < -51) && (thrust > -57)) { SplatThr_neg(9,_start_point,neg_y);} 
  else if ((thrust < -56) && (thrust > -62)) { SplatThr_neg(8,_start_point,neg_y);} 
  else if ((thrust < -61) && (thrust > -67)) { SplatThr_neg(7,_start_point,neg_y);} 
  else if ((thrust < -66) && (thrust > -73)) { SplatThr_neg(6,_start_point,neg_y);} 
  else if ((thrust < -72) && (thrust > -79)) { SplatThr_neg(5,_start_point,neg_y);} 
  else if ((thrust < -78) && (thrust > -85)) { SplatThr_neg(4,_start_point,neg_y);}
  else if ((thrust < -84) && (thrust > -90)) { SplatThr_neg(3,_start_point,neg_y);} 
  else if ((thrust < -89) && (thrust > -95)) { SplatThr_neg(2,_start_point,neg_y);} 
  else if ((thrust < -94) && (thrust > -98)) { SplatThr_neg(1,_start_point,neg_y);} 
  else if (thrust < -97) { SplatThr_neg(0,_start_point,neg_y);} 
   
 }
 else
 {
  SplatThr_pos(0,_start_point,_start_y);
  SplatThr_neg(19,_start_point,neg_y);
  
 } 
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatThr_pos(int ptr, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (ptr)) + *thr_ptr;
  _Draw_to_Double_Buffer_Thrust(char_ptr,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void SplatThr_neg(int ptr, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (ptr)) + *dwn_ptr;
  _Draw_to_Double_Buffer_Thrust(char_ptr,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void _Draw_to_Double_Buffer_Thrust(char *char_ptr, int x_pos, int y_pos)
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



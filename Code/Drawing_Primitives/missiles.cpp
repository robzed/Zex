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

// Name:   		    Missiles.c
// Description:		These routines, sort of the viewing of the Missile Bar
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
#include "Missiles.h"
#include "Error_handler.h"

#include "zex_misc.h"

#define NONE 0

#define CHAR_OFFSET 60  // in Pixels to the next character
#define ROW_SIZE 5
#define CHAR_LINES 12

#define FLIP_OFFSET 98
#define FLIPROW_SIZE 7
#define FLIPCHAR_LINES 14


void _Draw_to_Double_Buffer_Missile(char *char_ptr, int x_pos, int y_pos);
void Draw_to_Double_Buffer_Flip(char *char_ptr, int x_pos, int y_pos, int colour);


static Ptr double_buffer;
static int screen_width;
Handle miss_pic;
Handle hilight_pic;
static int counter;


void InitMissiles()
 {
 miss_pic = (Handle) LS_GetResource('ZRAW',131);  //Get the Handle to the Resource
 if (miss_pic==0) report_error_c("Resource missing: ZRAW 131","",4);
 LS_HLock(miss_pic);

 hilight_pic = (Handle) LS_GetResource('ZRAW',142);  //Get the Handle to the Resource
 if (hilight_pic==0) report_error_c("Resource missing: ZRAW 142","",4);
 LS_HLock(hilight_pic);


 counter = 3;
 }



 void update_missiles(int missile1, int missile2, int missile3, int flip_missile)
 {

//  int empty_blocks;
//  int remainder;
  int _start_point,start_y;
  int i;
  _start_point = 433;
  start_y = 467;


  // check pylon 1

    SplatMiss(missile1,_start_point+8,start_y-8);

 // check pylon 2

    SplatMiss(missile2,_start_point,start_y);

 // check pylon 3

    SplatMiss(missile3,_start_point+16,start_y);


 switch(flip_missile){
 case 0:
         SplatFlip(0,(_start_point-1+8),(start_y-1)-8);
         SplatFlip(1,(_start_point-1),(start_y -1));
         SplatFlip(1,(_start_point-1)+16,(start_y-1));
         break;

 case 1:
 		 SplatFlip(0,(_start_point-1),(start_y -1));
 		 SplatFlip(1,(_start_point-1)+8,(start_y-1)-8);
 		 SplatFlip(1,(_start_point-1)+16,(start_y-1));
         break;

 case 2:
 		 SplatFlip(0,(_start_point-1)+16,(start_y -1));
 		 SplatFlip(1,(_start_point-1),(start_y-1));
 		 SplatFlip(1,(_start_point-1)+8,(start_y-1)-8);

         break;

 default:
 		 SplatFlip(1,(_start_point-1)+16,(start_y-1));
 		 SplatFlip(1,(_start_point-1)+8,(start_y-1)-8);
 		 SplatFlip(0,(_start_point-1),(start_y-1));
         break;

 }
 counter --;


}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatMiss(int missile, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (missile)) + *miss_pic;
  _Draw_to_Double_Buffer_Missile(char_ptr,x_pos,y_pos);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatFlip(int flip, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (FLIP_OFFSET *(flip)) + *hilight_pic;
  Draw_to_Double_Buffer_Flip(char_ptr,x_pos,y_pos,-1);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void _Draw_to_Double_Buffer_Missile(char *char_ptr, int x_pos, int y_pos)
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

void Draw_to_Double_Buffer_Flip(char *char_ptr, int x_pos, int y_pos, int colour) // Colour is the colour that needs masking
  {
extern Ptr screenaddr;
extern int screenwidth;

char *db_ptr;
int a,b;

double_buffer=screenaddr;
screen_width=screenwidth;


db_ptr=(char *) (double_buffer+x_pos+(y_pos*screen_width));

  for(b=0; b < FLIPCHAR_LINES; b++)
  {
    for(a=0; a < FLIPROW_SIZE; a++)
     {
     if (*char_ptr != colour)
       {
       *db_ptr = (*char_ptr);
       }
     char_ptr++;
     db_ptr++;
     }
  db_ptr-=FLIPROW_SIZE;
  db_ptr+=screen_width;
  }

  }



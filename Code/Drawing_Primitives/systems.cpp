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

// Name:   		    Systems.c
// Description:		These routines, display the system status of an individual system
//
// Date Started:	04/03/99
// Author(s):		ID
//
// Copyright(c) Lightsoft Software

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#error "This file is not in the Zex2005 build"

//oldmac//#include <resources.h>
//oldmac//#include <ToolUtils.h>
#include "systems.h"
#include "Error_handler.h"

#include "zex_misc.h"

#define NONE 0

#define CHAR_OFFSET 924  // in Pixels to the next character
#define ROW_SIZE 66
#define CHAR_LINES 14

void _Draw_to_Double_Buffer_System(char *char_ptr, int x_pos, int y_pos);


Ptr double_buffer;
int screen_width;
Handle sys_pic;



void InitSystem()
 {
 sys_pic = (Handle) LS_GetResource('ZRAW',141);  //Get the Handle to the Resource
 if (sys_pic==0) report_error_c("Resource missing: ZRAW 141","",4);
 LS_HLock(sys_pic);
 }



 void update_system(int system, int condition)
 {

  int _start_point,start_y;
  _start_point = 571;
  start_y = 461;

  if (system != 0)
    {
     system = (system * 2) +1;
    }
  system = system + condition;
  SplatSystem(system,_start_point,start_y);

}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatSystem(int sys_type, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (sys_type)) + *sys_pic;
  _Draw_to_Double_Buffer_System(char_ptr,x_pos,y_pos);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void _Draw_to_Double_Buffer_System(char *char_ptr, int x_pos, int y_pos)
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



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


//oldmac//#include <ToolUtils.h>
//oldmac//#include <resources.h>
#include "fuel.h"
#include "Error_handler.h"
#include "zex_misc.h"

#define NONE 0

#define CHAR_OFFSET 90  // in Pixels to the next character  
#define ROW_SIZE 10
#define CHAR_LINES 9


void _Draw_to_Double_Buffer_Fuel(char *char_ptr, int x_pos, int y_pos); 

static Ptr double_buffer;
static int screen_width;
Handle lft_fuel;
Handle rght_fuel;
Handle ctr_fuel;

static int counter;
  

void InitFuelGuage()
 {
 lft_fuel = (Handle) LS_GetResource('ZRAW',132);  //Get the Handle to the Resource 
 if (lft_fuel==0) report_error_c("Resource missing: ZRAW 132","",4);
 LS_HLock(lft_fuel);

 rght_fuel = (Handle) LS_GetResource('ZRAW',134);  //Get the Handle to the Resource 
 if (rght_fuel==0) report_error_c("Resource missing: ZRAW 133","",4);
 LS_HLock(rght_fuel);
 
 ctr_fuel = (Handle) LS_GetResource('ZRAW',133);  //Get the Handle to the Resource 
 if (ctr_fuel==0) report_error_c("Resource missing: ZRAW 134","",4);
 LS_HLock(ctr_fuel);
 
 
 counter = 3;
 }


 
 void update_fuel(int fuel_level)
 {
  
  int empty_blocks;
  int full_blocks;
  int remainder;
  int _start_point,_start_y;
  int i; 
  
  full_blocks = fuel_level / 10;
  
  empty_blocks = ((180 - fuel_level) / 10);
  remainder = fuel_level - (full_blocks * 10);
  
  _start_point =131;
  _start_y = 428;

//if (counter >0){
  
  if (full_blocks == 18)
    {
    SplatLeftFuel(11,_start_point,_start_y);
    _start_point = _start_point + 10;
    for (i = 0 ; i < 16;i++)
     {
     SplatCentreFuel(10,_start_point +(i * 10),_start_y);
     }
    _start_point = _start_point + 160;
    SplatRightFuel(10,_start_point,_start_y);
     
    }
  else if (empty_blocks == 18)
    {
     SplatLeftFuel(0,_start_point,_start_y);
     _start_point = _start_point + 10;
     for (i = 0 ; i < 16;i++)
     {
     SplatCentreFuel(0,_start_point +(i * 10),_start_y);
     }
     _start_point = _start_point + 160;
     SplatRightFuel(0,_start_point,_start_y);
    
    }  
  else
   {
      if (full_blocks > 0)
        {
         SplatLeftFuel(11,_start_point,_start_y);
         for (i = 1; i < full_blocks;i++)
          {
           SplatCentreFuel(10,_start_point +(i * 10),_start_y);
          }
        }
      
      if (empty_blocks > 0)
        {
        _start_point = _start_point + 170;
         SplatRightFuel(0,_start_point,_start_y);
         for (i = 1; i < empty_blocks;i++)
          {
           SplatCentreFuel(0,_start_point - (i * 10),_start_y);
          }
        }
   
   
   _start_point = 131 + (full_blocks * 10);
   
   switch (remainder){
   
   case 1:
          if (full_blocks == 17)
           {
           SplatRightFuel(1,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(1,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(1,_start_point,_start_y);
           }
           break;
   case 2:
          if (full_blocks == 17)
           {
           SplatRightFuel(2,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(2,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(2,_start_point,_start_y);
           }
           break;            
   case 3:
          if (full_blocks == 17)
           {
           SplatRightFuel(3,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(3,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(3,_start_point,_start_y);
           }
           break;            
  case 4:
          if (full_blocks == 17)
           {
           SplatRightFuel(4,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(4,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(4,_start_point,_start_y);
           }
           break;                     
   
  case 5:
          if (full_blocks == 17)
           {
           SplatRightFuel(5,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(5,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(5,_start_point,_start_y);
           }
           break;                     
  case 6:
          if (full_blocks == 17)
           {
           SplatRightFuel(6,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(6,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(6,_start_point,_start_y);
           }
           break;                     
  case 7:
          if (full_blocks == 17)
           {
           SplatRightFuel(7,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(7,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(7,_start_point,_start_y);
           }
           break;                     
 
  case 8:
          if (full_blocks == 17)
           {
           SplatRightFuel(8,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(8,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(8,_start_point,_start_y);
           }
           break;                     
    case 9:
          if (full_blocks == 17)
           {
           SplatRightFuel(9,_start_point,_start_y); 
           }
          else if (full_blocks == 0)
           {
           SplatLeftFuel(9,_start_point,_start_y);
           }
          else
           {
           SplatCentreFuel(9,_start_point,_start_y);
           }
           break;                     
 
   
   
   
   }
   
   
   }  
// counter --;

//}   


  
}
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatLeftFuel(int amount, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (amount)) + *lft_fuel;
  _Draw_to_Double_Buffer_Fuel(char_ptr,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void SplatRightFuel(int amount, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (amount)) + *rght_fuel;
  _Draw_to_Double_Buffer_Fuel(char_ptr,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
void SplatCentreFuel(int amount, int x_pos, int y_pos)
 {
  char *char_ptr;

  char_ptr =  (CHAR_OFFSET * (amount)) + *ctr_fuel;
  _Draw_to_Double_Buffer_Fuel(char_ptr,x_pos,y_pos);
}   

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


void _Draw_to_Double_Buffer_Fuel(char *char_ptr, int x_pos, int y_pos)
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



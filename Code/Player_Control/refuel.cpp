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

//refuel
//SB 210199
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include <mactypes.h>
#include "3d_structures_def.h"
#include "refuel.h"
#include "sound.h"
#include "instrum_int.h"
#include "dlp.h"

typedef struct
{ Rect refuel_rect;
  int in_use;
} refuel_slot;
int refuel_slots_in_use;


refuel_slot refuel_rects[MAX_REFUEL_SLOTS];	//we can cope with up to ten rects per level
int refuel_sound_timer;	//used to play a sound when refuelling

void init_refuel_rects()
{
int i;

refuel_slots_in_use=0;
for (i=0;i<MAX_REFUEL_SLOTS;i++)
     {
     refuel_rects[i].in_use=0;
     }
refuel_sound_timer=0;

}


void add_refuel_rect(Rect the_rect)
{
if (refuel_slots_in_use<MAX_REFUEL_SLOTS)
 {
  refuel_rects[refuel_slots_in_use].refuel_rect=the_rect;
  refuel_rects[refuel_slots_in_use].in_use=1;
  refuel_slots_in_use++;
 }
 
}

void run_refuelling()
{
extern	DynObjectsFixedSize *ocb_ptr;
extern int thrust;
extern kit_desc ship_items[ITEMS_COUNT];

int ship_x,ship_y,i;

ship_x= static_cast<int>((*ocb_ptr).object_list[0].Oworldx);
ship_y= static_cast<int>((*ocb_ptr).object_list[0].Oworldy);	//convert from float to int here

for (i=0;i<MAX_REFUEL_SLOTS;i++)
    {
    if (refuel_rects[i].in_use==0) break;	 //what refuelling slot?
    
    //check if ship is in rect
    if (ship_x>refuel_rects[i].refuel_rect.left &&
        ship_x<refuel_rects[i].refuel_rect.right)
        {
        //y is funny in that +y is up
          if (ship_y<refuel_rects[i].refuel_rect.top &&
              ship_y>refuel_rects[i].refuel_rect.bottom)
              {
              //we are in the refuel rect
              if (ship_items[KNORMAL_FUEL_TANK].value1<180)	//this stops us filling the lower half of the screen
                                //with fuel bar (been there, done that...)
              {
              if (refuel_sound_timer==0)
              {
              refuel_sound_timer=34;	//roughly once/second
                if (thrust<20 && thrust>-20)
                {
                 ship_items[KNORMAL_FUEL_TANK].value1+=2;
                 if (ship_items[KNORMAL_FUEL_TANK].value1>=180)
                    add_to_text_display("REFUELLED.",0); 
                 play_zsound(refuel_beep, sound_high_pri, c1_chan1, sound_vol_7);
                }
              }
              else
              refuel_sound_timer--;
              }
              break;	//we can only ever be in one refuel rect
              }
        }
    }

}


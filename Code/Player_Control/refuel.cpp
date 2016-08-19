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


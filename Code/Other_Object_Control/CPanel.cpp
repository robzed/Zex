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

//oldmac//#include <NumberFormatting.h>
#include "CPanel.h"
#include "Damage.h"
#include "Missiles.h"
#include "fuel.h"
#include "radar.h"
#include "Score.h"
#include "credits.h"
#include "txtsplat.h"
#include "instrum.h"
#include "thrust.h"
#include "lives.h"
#include "debug_timing.h"
#include "instrum_int.h"
#include "Batcom.h"
#include "zex_misc.h"

#include "3d_structures_def.h"

extern int	thrust;
unsigned char no_string[12];

long score,credits;
int lives;
int sys, con;
int dummy;
int missile_flip;
//scheduling vars
int last_shields,shields_display_count;
int last_fuel,fuel_display_count;
int do_splat_radar;

int need_to_do_cp_update;

void do_ogl_control_panel_update()
{
if (need_to_do_cp_update==0) return;

}
extern kit_desc ship_items[ITEMS_COUNT];

#if OGL==1
void do_control_panel_control()
 {
	extern	DynObjectsFixedSize *ocb_ptr;
	extern int fps,gupdate_cp,show_control_panel;



  ZexNumToString(score,no_string);
  no_string[no_string[0]+1]=0;	  //quick p to c hack
  no_string[0]=' ';
  SplatScore((char *)no_string,22,422);


  ZexNumToString(credits,no_string);
  no_string[no_string[0]+1]=0;	  //quick p to c hack
  no_string[0]=' ';
  SplatCredits((char *)no_string,548,422);

  if (ship_items[KNORMAL_FUEL_TANK].value1!=0) update_radar();	 //always update radar

  update_battlecom();
 if(show_control_panel==1) return;


  update_thrust(thrust);
 update_instruments();



 	update_hull_damage();

//shields update
//check if we're splatting because of a previous change in value
 	if (shields_display_count!=0)	//and we actually have shields!
 	 {
   	update_shield_damage(ship_items[KSHIELDS].value1);
   	shields_display_count--;
         }

//check if value has changed
 if (last_shields!=ship_items[KSHIELDS].value1)
 {
  last_shields=ship_items[KSHIELDS].value1;
  update_shield_damage(ship_items[KSHIELDS].value1);
  shields_display_count=3;	//make sure we do all 3 frame buffers
 }


 update_missiles((int) ship_items[KMISSILE_PYLON1].modifier, (int)ship_items[KMISSILE_PYLON2].modifier, (int) ship_items[KMISSILE_PYLON3].modifier ,missile_flip);

// update_fuel(ship_items[KNORMAL_FUEL_TANK].value1);
//fuel update
//check if we're splatting because of a previous change in value
 if (fuel_display_count!=0)
 {
   update_fuel(ship_items[KNORMAL_FUEL_TANK].value1);
   fuel_display_count--;
 }

//check if value has changed

 if (last_fuel!=ship_items[KNORMAL_FUEL_TANK].value1)
 {
  last_fuel=ship_items[KNORMAL_FUEL_TANK].value1;
  update_fuel(ship_items[KNORMAL_FUEL_TANK].value1);;
  fuel_display_count=2;	//make sure we do all 3 frame buffers
 }




   ZexNumToString(ship_items[KFLATPACK].value1,no_string);
  no_string[no_string[0]+1]=0;	  //quick p to c hack
  no_string[0]=' ';
  SplatLives((char *)no_string,479,466);

// ZexNumToString(thrust,no_string);
// no_string[no_string[0]+1]=0;	//quick p to c hack
// no_string[0]=' ';
// //SplatBox(329,468,33);
// SplatText_Thrust ((char *)no_string,329,468);



}


#else	//ogl==1
void do_control_panel_control()
 {
	extern	DynObjectsFixedSize *ocb_ptr;
	extern int fps,gupdate_cp,show_control_panel;


  ZexNumToString(score,no_string);
  no_string[no_string[0]+1]=0;	  //quick p to c hack
  no_string[0]=' ';
  SplatScore((char *)no_string,22,422);


  ZexNumToString(credits,no_string);
  no_string[no_string[0]+1]=0;	  //quick p to c hack
  no_string[0]=' ';
  SplatCredits((char *)no_string,548,422);

  if (ship_items[KNORMAL_FUEL_TANK].value1!=0) update_radar();	 //always update radar

  update_battlecom();
 if(show_control_panel==1) return;


  update_thrust(thrust);
 update_instruments();



 	update_hull_damage();

//shields update
//check if we're splatting because of a previous change in value
 	if (shields_display_count!=0)	//and we actually have shields!
 	 {
   	update_shield_damage(ship_items[KSHIELDS].value1);
   	shields_display_count--;
         }

//check if value has changed
 if (last_shields!=ship_items[KSHIELDS].value1)
 {
  last_shields=ship_items[KSHIELDS].value1;
  update_shield_damage(ship_items[KSHIELDS].value1);
  shields_display_count=3;	//make sure we do all 3 frame buffers
 }


 update_missiles((int) ship_items[KMISSILE_PYLON1].modifier, (int)ship_items[KMISSILE_PYLON2].modifier, (int) ship_items[KMISSILE_PYLON3].modifier ,missile_flip);

// update_fuel(ship_items[KNORMAL_FUEL_TANK].value1);
//fuel update
//check if we're splatting because of a previous change in value
 if (fuel_display_count!=0)
 {
   update_fuel(ship_items[KNORMAL_FUEL_TANK].value1);
   fuel_display_count--;
 }

//check if value has changed

 if (last_fuel!=ship_items[KNORMAL_FUEL_TANK].value1)
 {
  last_fuel=ship_items[KNORMAL_FUEL_TANK].value1;
  update_fuel(ship_items[KNORMAL_FUEL_TANK].value1);;
  fuel_display_count=2;	//make sure we do all 3 frame buffers
 }




   ZexNumToString(ship_items[KFLATPACK].value1,no_string);
  no_string[no_string[0]+1]=0;	  //quick p to c hack
  no_string[0]=' ';
  SplatLives((char *)no_string,479,466);

// ZexNumToString(thrust,no_string);
// no_string[no_string[0]+1]=0;	//quick p to c hack
// no_string[0]=' ';
// //SplatBox(329,468,33);
// SplatText_Thrust ((char *)no_string,329,468);



}
#endif //ogl==1
void do_cp()
{
//we have to do all three buffers as in

}
void InitControlPanel()
 {
 InitMissiles();
 InitDamageBar();
 InitFuelGuage();
 InitRadar();
 InitScore();
 InitCredits();
 InitInstruments();
 InitThrust();
 InitLives();

 last_shields=0;
 shields_display_count=3;
 do_splat_radar=1;

 last_fuel=0;
 fuel_display_count=3;

 score = 0;
 credits = 0;
 ship_items[KNORMAL_FUEL_TANK].value1=180;
 lives = 0;
 missile_flip = 0;
 need_to_do_cp_update=1;
 }


void force_fuel_update(void)
{
	fuel_display_count=2;
}

static int old_galactic_map=0;
static int old_inventory=0;
extern int gShowGalacticMap,global_show_status;
void check_for_fuel_update(void)
{		//we need to note any changes in inventory or galactic map so we can update the fuel pre-rendered display
		if ((gShowGalacticMap!=global_show_status) || (old_inventory!=global_show_status))
		{
			force_fuel_update();
		}
		old_galactic_map=gShowGalacticMap;
		old_inventory=global_show_status;
}


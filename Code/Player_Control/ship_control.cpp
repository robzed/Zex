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

// *************************************MODULE HEADER*********************************
// * FILENAME           : ship_control.c
// * MODULE TITLE       : ship_control.c
// * PROJECT            : Zex
// * DATE STARTED       : 22:29:16 on 22nd June 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <stdlib.h>			// for abs
#include <string.h>

//ship control
//oldmac//#include <NumberFormatting.h>
#include "ship_control.h"
#include "3d_structures_def.h"
#include	"load_objects.h"
#include	"objects_controls.h"
#include "object_control_top.h"
#include "explode.h"
#include "sound.h"
#include "fire_weapon.h"
#include "monster1.h"
#include "aim.h"
#include	"zex_misc.h"
#include "dlp.h"
#include "input_control.h"
#include "instrum_int.h"
#include "buy_int.h"
#include "Damage.h"
#include	"drawing.h"
#include "pictures.h"
#include "game_pause.h"
#include "Error_handler.h"
#include "compile_zmap.h"
#include "level_control.h"
#include "weapons_damage.h"
#include "camera.h"
#include "game_defs.h"
#include "obj_control_misc_routines.h"
#include "light_control.h"
#include "txtsplat.h"
#include "handle_level.h"
#include "motherships.h"
#include "cargo.h"
#include "command_and_control.h"
#include "monster_control_misc.h"
//oldmac//#include "processor.h"
#include "OGL.h"

// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

_3D ship_normal;	//Start pointing right
_3D ship_rotated_normal; //from engine


int have_ejected;	//1 if so, keeps game alive!

int launch_timer;
int ship_y_ang,ship_x_ang,ship_z_ang;
int last_bullet_fired_frame=0,last_laser_fired_frame=0;
int thrust,zero_crossed;	//-2>>2 if go from + to - or - to +. Generates ship flipped run
int unlim_thrust,directional_control;
int hull,zex_alive,retro,shields_recharge_counter,shields_recharge_reset;
int death_frame_counter,user_abort;
int global_show_status=0;
int gShowGalacticMap=0;

//int shields_quality;	  //starts at 5 and goes down to 1 as better sheilds added
float turn;
int gRadarRange=1;	//0=short, 1=far
int gvel_limiter=0;
unsigned int last_range_toggle_frame=0;	//for debounce
unsigned int last_dock_toggle_frame=0;	//for debounce
unsigned int last_missile_toggle_frame=0;	//for debounce
unsigned int last_camera_toggle_frame=0;
unsigned int last_pause_frame=0;

unsigned int last_inv_toggle_frame=0;	//for debounce
int shields_warning_timer=0;
int tractor;	//if one, we are under tractor beam control and cant fly ourselfs
int ship_laser_category,ship_cannon_category;
int attack_message_timer;	//for motherrship under attack message
int hide_zex;
//170799
int level_has_control;	//important - if 1, the player has no control over the ship!!!

int zex_laser_experience=0;
int thrust_lockout;	//if 1, we dont allow thrust or directions - used in tutorial
int gRank,kills,promotion_points;

extern int gCameraMode,show_control_panel,gCPMode,gCommandControl;
extern int gShowGalacticMap, freeze, difficulty;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


void run_ship_misc_control(void);

void dz1(void);
void run_exhaust_animation();

extern int player_input_counter,jettison_timer;	//reset if we turn left/right

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
void reset_ship()
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_damage;
extern int missile_flip,tutorial;
extern int have_ejected,done_load_flag;
	if (tutorial==1)
	{
		gRadarRange=0;	//short range if tutorial
	}
	else
	{
		gRadarRange=1;	//long range if game
	}
gvel_limiter=0;
have_ejected=0;	//have not ejected
jettison_timer=0;
reinit_screen();
hide_zex=0;	//zex invisible counter
ship_normal.x=1;
ship_normal.y=0;
ship_normal.z=0;
thrust=0;
unlim_thrust=0;
directional_control=1;
//shields=1000;
//hull=25;
zex_alive=1;
death_frame_counter=100;	//frames of circling camera - crucial we do this so ship can get killed
last_bullet_fired_frame=0;
last_laser_fired_frame=0;
tractor=0;
launch_timer=0;
attack_message_timer=0;
missile_flip=0;

#if ZEX_LIVES_FOR_EVER==0
if (done_load_flag==0)
{
 hull_damage=0;
 ship_items[KRADAR].status = 1;  //Radar Available and operative
 ship_items[KRADAR].obj_class=EQUIPMENT;
 ship_items[KRADAR].modifier = 1;  //Radar Available and operative
 
 ship_items[KLONG_RANGE_RADAR].status = 1;  //Long Range Radar Available and operative
 ship_items[KLONG_RANGE_RADAR].obj_class=EQUIPMENT;
 ship_items[KLONG_RANGE_RADAR].modifier = 1;
 
 ship_items[KCANNON].status = 1;  //Cannon Available and Operative
 ship_items[KCANNON].obj_class=WEAPONS;
 ship_items[KCANNON].modifier=1;
 
 ship_items[KLASER].status = 1;  // Laser available
 ship_items[KLASER].obj_class=WEAPONS;
 ship_items[KLASER].modifier=1;

 ship_items[KMISSILES].status = 1;  // Missiles Available
 ship_items[KMISSILES].obj_class=WEAPONS;
 ship_items[KMISSILES].modifier = 1;  // Missiles Available

 ship_items[KREPAIR_SYSTEMS].status = 1;  // Repair systems
 ship_items[KREPAIR_SYSTEMS].obj_class=REPAIRS;
 ship_items[KREPAIR_SYSTEMS].modifier=1;

ship_items[KSHIELDS].status=1;
ship_items[KSHIELDS].value1=1000;
ship_items[KSHIELDS].modifier=1;
ship_items[KSHIELDS].obj_class=EQUIPMENT;

ship_items[KNORMAL_FUEL_TANK].value1=180;	//should be 180
ship_items[KNORMAL_FUEL_TANK].status=1;
ship_items[KNORMAL_FUEL_TANK].obj_class=EQUIPMENT;

ship_items[KMISSILE_PYLON1].status=1;
ship_items[KMISSILE_PYLON1].obj_class=WEAPONS;
ship_items[KMISSILE_PYLON1].modifier=1;

ship_items[KMISSILE_PYLON2].status=1;
ship_items[KMISSILE_PYLON2].obj_class=WEAPONS;
ship_items[KMISSILE_PYLON2].modifier=1;

ship_items[KMISSILE_PYLON3].status=1;
ship_items[KMISSILE_PYLON3].obj_class=WEAPONS;
ship_items[KMISSILE_PYLON3].modifier=1;

ship_items[KFLIGHT_COMPUTER].status=1;
ship_items[KFLIGHT_COMPUTER].obj_class=EQUIPMENT;
ship_items[KFLIGHT_COMPUTER].modifier=1;

ship_items[KNAV_COMPUTER].status=1;
ship_items[KNAV_COMPUTER].obj_class=EQUIPMENT;
ship_items[KNAV_COMPUTER].modifier=1;

ship_items[KENGINE].obj_class=NON_REMOVABLE;
ship_items[KENGINE].status=1;
ship_items[KENGINE].modifier=1;

ship_items[KEJECTION].status=1;		//should be 1
ship_items[KEJECTION].obj_class=EQUIPMENT; 
ship_items[KEJECTION].modifier=1; 
ship_items[KEJECTION].value1=0;	

ship_items[KFLATPACK].status=1;
ship_items[KFLATPACK].modifier=1;
ship_items[KFLATPACK].value1=1;
ship_items[KFLATPACK].obj_class=EQUIPMENT;

//now the bits we dont get at start up
ship_items[KFUEL_MAXIMISER].status=0;
ship_items[KFUEL_MAXIMISER].obj_class=EQUIPMENT;
ship_items[KFUEL_MAXIMISER].modifier=0;
ship_items[KFUEL_MAXIMISER].value1=0;

ship_items[KAUTO_HULL_REPAIR].status=0;
ship_items[KAUTO_HULL_REPAIR].modifier=0;
ship_items[KAUTO_HULL_REPAIR].obj_class=EQUIPMENT;
ship_items[KAUTO_HULL_REPAIR].value1=0;

ship_items[KREAR_LASER].status=0;	//should be 0
ship_items[KREAR_LASER].modifier=0;	//same
ship_items[KREAR_LASER].obj_class=WEAPONS;
ship_items[KREAR_LASER].value1=0;

ship_items[K360].status=0;
ship_items[K360].obj_class=WEAPONS;
ship_items[K360].modifier=0;
ship_items[K360].value1=0;

ship_items[KSHOCKWAVE].status=0;
ship_items[KSHOCKWAVE].obj_class=WEAPONS;
ship_items[KSHOCKWAVE].modifier=1;
ship_items[KSHOCKWAVE].value1=0;

ship_items[KECM].status=0;
ship_items[KECM].obj_class=WEAPONS;
ship_items[KECM].modifier=1;
ship_items[KECM].value1=0;

ship_items[KMISSILE_FACTORY].status=0;
ship_items[KMISSILE_FACTORY].obj_class=WEAPONS;
ship_items[KMISSILE_FACTORY].modifier=1;
ship_items[KMISSILE_FACTORY].value1=0;

ship_items[KFAST_DOCK].status=0;
ship_items[KFAST_DOCK].obj_class=EQUIPMENT;
ship_items[KFAST_DOCK].modifier=0;
ship_items[KFAST_DOCK].value1=0;



ship_items[KRETRO_ROCKETS].status=0;
ship_items[KRETRO_ROCKETS].value1=3;
ship_items[KRETRO_ROCKETS].modifier=0;
ship_items[KRETRO_ROCKETS].obj_class=EQUIPMENT;


ship_items[KHYPERSPACE].status=0;
ship_items[KHYPERSPACE].value1=0;
ship_items[KHYPERSPACE].modifier=0;
ship_items[KHYPERSPACE].obj_class=EQUIPMENT;


ship_items[KTRADING_COMPUTER].status=0;
ship_items[KTRADING_COMPUTER].modifier=0;
ship_items[KTRADING_COMPUTER].obj_class=EQUIPMENT;
ship_items[KTRADING_COMPUTER].value1=0;

ship_items[KAUTO_HULL_REPAIR].status=0;
ship_items[KAUTO_HULL_REPAIR].modifier=0;
ship_items[KAUTO_HULL_REPAIR].value1=0;
ship_items[KAUTO_HULL_REPAIR].obj_class=REPAIRS;


ship_items[KBOID_ADDITION].status=0;
ship_items[KBOID_ADDITION].modifier=0;
ship_items[KBOID_ADDITION].value1=0;
ship_items[KBOID_ADDITION].obj_class=WEAPONS;

ship_items[KASTEROID_ADDITION].status=0;
ship_items[KASTEROID_ADDITION].modifier=0;
ship_items[KASTEROID_ADDITION].value1=0;
ship_items[KASTEROID_ADDITION].obj_class=WEAPONS;
}	//end of done_load_flag
#else	//ZEX_LIVES_FOREVER so give me everything
if (done_load_flag==0)
{
 hull_damage=0;
 ship_items[KRADAR].status = 1;  //Radar Available and operative
 ship_items[KRADAR].obj_class=EQUIPMENT;
 ship_items[KRADAR].modifier = 1;  //Radar Available and operative
 
 ship_items[KLONG_RANGE_RADAR].status = 1;  //Long Range Radar Available and operative
 ship_items[KLONG_RANGE_RADAR].obj_class=EQUIPMENT;
 ship_items[KLONG_RANGE_RADAR].modifier = 1;
 
 ship_items[KCANNON].status = 1;  //Cannon Available and Operative
 ship_items[KCANNON].obj_class=WEAPONS;
 ship_items[KCANNON].modifier=3;
 
 ship_items[KLASER].status = 1;  // Laser available
 ship_items[KLASER].obj_class=WEAPONS;
 ship_items[KLASER].modifier=13;

 ship_items[KMISSILES].status = 1;  // Missiles Available
 ship_items[KMISSILES].obj_class=WEAPONS;
 ship_items[KMISSILES].modifier = 3;  // Missiles Available

 ship_items[KREPAIR_SYSTEMS].status = 1;  // Repair systems
 ship_items[KREPAIR_SYSTEMS].obj_class=REPAIRS;
 ship_items[KREPAIR_SYSTEMS].modifier=1;

ship_items[KSHIELDS].status=1;
ship_items[KSHIELDS].value1=1000;
ship_items[KSHIELDS].modifier=3;
ship_items[KSHIELDS].obj_class=EQUIPMENT;

ship_items[KNORMAL_FUEL_TANK].value1=180;	//should be 180
ship_items[KNORMAL_FUEL_TANK].status=1;
ship_items[KNORMAL_FUEL_TANK].obj_class=EQUIPMENT;

ship_items[KMISSILE_PYLON1].status=1;
ship_items[KMISSILE_PYLON1].obj_class=WEAPONS;
ship_items[KMISSILE_PYLON1].modifier=3;

ship_items[KMISSILE_PYLON2].status=1;
ship_items[KMISSILE_PYLON2].obj_class=WEAPONS;
ship_items[KMISSILE_PYLON2].modifier=3;

ship_items[KMISSILE_PYLON3].status=1;
ship_items[KMISSILE_PYLON3].obj_class=WEAPONS;
ship_items[KMISSILE_PYLON3].modifier=3;

ship_items[KFLIGHT_COMPUTER].status=1;
ship_items[KFLIGHT_COMPUTER].obj_class=EQUIPMENT;
ship_items[KFLIGHT_COMPUTER].modifier=2;

ship_items[KNAV_COMPUTER].status=1;
ship_items[KNAV_COMPUTER].obj_class=EQUIPMENT;
ship_items[KNAV_COMPUTER].modifier=2;

ship_items[KENGINE].obj_class=NON_REMOVABLE;
ship_items[KENGINE].status=1;
ship_items[KENGINE].modifier=1;

ship_items[KEJECTION].status=1;	
ship_items[KEJECTION].obj_class=EQUIPMENT; 
ship_items[KEJECTION].modifier=2; 
ship_items[KEJECTION].value1=0;	

ship_items[KFLATPACK].status=1;
ship_items[KFLATPACK].modifier=1;
ship_items[KFLATPACK].value1=9;
ship_items[KFLATPACK].obj_class=EQUIPMENT;

//now the bits we dont get at start up
ship_items[KFUEL_MAXIMISER].status=1;
ship_items[KFUEL_MAXIMISER].obj_class=EQUIPMENT;
ship_items[KFUEL_MAXIMISER].modifier=1;
ship_items[KFUEL_MAXIMISER].value1=1;

ship_items[KAUTO_HULL_REPAIR].status=1;
ship_items[KAUTO_HULL_REPAIR].modifier=1;
ship_items[KAUTO_HULL_REPAIR].obj_class=REPAIRS;
ship_items[KAUTO_HULL_REPAIR].value1=1;

ship_items[KREAR_LASER].status=1;	//should be 0
ship_items[KREAR_LASER].modifier=1;	//same
ship_items[KREAR_LASER].obj_class=WEAPONS;
ship_items[KREAR_LASER].value1=1;

ship_items[K360].status=1;
ship_items[K360].obj_class=WEAPONS;
ship_items[K360].modifier=1;
ship_items[K360].value1=1;

ship_items[KSHOCKWAVE].status=1;
ship_items[KSHOCKWAVE].obj_class=WEAPONS;
ship_items[KSHOCKWAVE].modifier=1;
ship_items[KSHOCKWAVE].value1=100;

ship_items[KECM].status=1;
ship_items[KECM].obj_class=WEAPONS;
ship_items[KECM].modifier=1;
ship_items[KECM].value1=1;

ship_items[KMISSILE_FACTORY].status=1;
ship_items[KMISSILE_FACTORY].obj_class=WEAPONS;
ship_items[KMISSILE_FACTORY].modifier=1;
ship_items[KMISSILE_FACTORY].value1=1;

ship_items[KFAST_DOCK].status=1;
ship_items[KFAST_DOCK].obj_class=EQUIPMENT;
ship_items[KFAST_DOCK].modifier=1;
ship_items[KFAST_DOCK].value1=1;

ship_items[KRETRO_ROCKETS].status=1;
ship_items[KRETRO_ROCKETS].value1=3;
ship_items[KRETRO_ROCKETS].modifier=1;
ship_items[KRETRO_ROCKETS].obj_class=EQUIPMENT;


ship_items[KHYPERSPACE].status=1;
ship_items[KHYPERSPACE].value1=1;
ship_items[KHYPERSPACE].modifier=1;
ship_items[KHYPERSPACE].obj_class=EQUIPMENT;


ship_items[KTRADING_COMPUTER].status=1;
ship_items[KTRADING_COMPUTER].modifier=1;
ship_items[KTRADING_COMPUTER].obj_class=EQUIPMENT;
ship_items[KTRADING_COMPUTER].value1=3;

ship_items[KAUTO_HULL_REPAIR].status=1;
ship_items[KAUTO_HULL_REPAIR].modifier=1;
ship_items[KAUTO_HULL_REPAIR].value1=1;
ship_items[KAUTO_HULL_REPAIR].obj_class=REPAIRS;


ship_items[KBOID_ADDITION].status=1;
ship_items[KBOID_ADDITION].modifier=1;
ship_items[KBOID_ADDITION].value1=1;
ship_items[KBOID_ADDITION].obj_class=WEAPONS;

ship_items[KASTEROID_ADDITION].status=1;
ship_items[KASTEROID_ADDITION].modifier=1;
ship_items[KASTEROID_ADDITION].value1=1;
ship_items[KASTEROID_ADDITION].obj_class=WEAPONS;
}	//end of done_load_flag
#endif //ZEX_LIVES_FOREVER
}

extern kit_desc ship_items[ITEMS_COUNT];
extern hold_desc hold_items[MAX_HOLD_ITEMS];

void reset_ship_with_carry_over()
{
extern int hull_damage;

int i;
gRadarRange=1;
gvel_limiter=0;

jettison_timer=0;
reinit_screen();
hull_damage=0;
hide_zex=0;	//zex invisible counter
ship_normal.x=1;
ship_normal.y=0;
ship_normal.z=0;
thrust=0;
unlim_thrust=0;
directional_control=1;
//shields=1000;
//hull=25;
zex_alive=1;
death_frame_counter=100;	//frames of circling camera - crucial we do this so ship can get killed
last_bullet_fired_frame=0;
last_laser_fired_frame=0;
tractor=0;
launch_timer=0;
attack_message_timer=0;

//For each item we need to find out if the player already has it

 if (ship_items[KRADAR].status>0)	//hasn't been sold or lost
 {
	ship_items[KRADAR].status = 1;  //Radar Available and operative, leave the modifier alone
 }
 else	//new basic radar - player didn't have it, so new one
 {
	ship_items[KRADAR].status = 1;  //Radar Available and operative
	ship_items[KRADAR].modifier = 1;  //Radar Available and operative
 }
 
 if (ship_items[KLONG_RANGE_RADAR].status>0)
 {
	ship_items[KLONG_RANGE_RADAR].status = 1;  //Long Range Radar Available and operative
 }
 else
 {
	ship_items[KLONG_RANGE_RADAR].status = 1;  //Long Range Radar Available and operative, leave the modifier alone
	ship_items[KLONG_RANGE_RADAR].modifier = 1;
 }
 
 if (ship_items[KCANNON].status>0)
 {
	 ship_items[KCANNON].status = 1;  //Cannon Available and Operative

 }
 else
 {
	 ship_items[KCANNON].status = 1;  //Cannon Available and Operative
	 ship_items[KCANNON].modifier=1;
 }
 
 if (ship_items[KLASER].status>0)
 {
	 ship_items[KLASER].status = 1;  // Laser available, leave the modifier alone
 }
 else
 {
	 ship_items[KLASER].status = 1;  // Laser available
	 ship_items[KLASER].modifier=1;
 }
 
 
 if (ship_items[KMISSILES].status>0)
 {
	 ship_items[KMISSILES].status = 1;  // Missiles Available, leave the modifier alone
 }
 else	//no missile controller, lets have a new one
 {
	 ship_items[KMISSILES].status = 1;  // Missiles Available
	 ship_items[KMISSILES].modifier = 1;  // Missiles Available
 }
 

 if (ship_items[KREPAIR_SYSTEMS].status>0)
 {
	 ship_items[KREPAIR_SYSTEMS].status = 1;  // Repair systems serviceable
 }
 else
 {
	 ship_items[KREPAIR_SYSTEMS].status = 1;  // lets have a new one
	 ship_items[KREPAIR_SYSTEMS].modifier=1;  //at level 1
 }

if (ship_items[KSHIELDS].status>0)
{
	ship_items[KSHIELDS].status=1;
	ship_items[KSHIELDS].value1=1000;	//recharge the shields
}
else
{
	ship_items[KSHIELDS].status=1;
	ship_items[KSHIELDS].value1=1000;
	ship_items[KSHIELDS].modifier=1;	//give player level 1 shields
}

//Full tank always
ship_items[KNORMAL_FUEL_TANK].value1=180;	//should be 180
ship_items[KNORMAL_FUEL_TANK].status=1;
ship_items[KNORMAL_FUEL_TANK].obj_class=EQUIPMENT;

//Missiles pylons - note the modifier indicates the missile fitted, 0 = no missile
if (ship_items[KMISSILE_PYLON1].status>0)
{
	ship_items[KMISSILE_PYLON1].status=1;	//repair the pylon
}
else
{
	ship_items[KMISSILE_PYLON1].status=1;	//new pylon
	ship_items[KMISSILE_PYLON1].modifier=0;	//with no missile
}


if (ship_items[KMISSILE_PYLON2].status>0)
{
	ship_items[KMISSILE_PYLON2].status=1;
}
else
{
	ship_items[KMISSILE_PYLON2].status=1;
	ship_items[KMISSILE_PYLON2].modifier=0;	//no missile if new pylon
}

if (ship_items[KMISSILE_PYLON3].status>0)
{
	ship_items[KMISSILE_PYLON3].status=1;
}
else
{
	ship_items[KMISSILE_PYLON3].status=1;
	ship_items[KMISSILE_PYLON3].modifier=0;	//no missile if new pylon
}

if (ship_items[KFLIGHT_COMPUTER].status>0)
{
	ship_items[KFLIGHT_COMPUTER].status=1;
}
else
{
	ship_items[KFLIGHT_COMPUTER].status=1;
	ship_items[KFLIGHT_COMPUTER].modifier=1;
}

if (ship_items[KNAV_COMPUTER].status>0)
{
	ship_items[KNAV_COMPUTER].status=1;
}
else
{
	ship_items[KNAV_COMPUTER].status=1;
	ship_items[KNAV_COMPUTER].modifier=1;
}

//Always new engine
ship_items[KENGINE].obj_class=NON_REMOVABLE;
ship_items[KENGINE].status=1;
ship_items[KENGINE].modifier=1;

if (ship_items[KEJECTION].status>0)
{
	ship_items[KEJECTION].status=1;
	ship_items[KEJECTION].value1=0;	
}
else
{
	ship_items[KEJECTION].status=1;
	ship_items[KEJECTION].modifier=1; 
	ship_items[KEJECTION].value1=0;	
}

/* - do nothing to the flatpacks */

//now the bits we dont get at start up
if (ship_items[KFUEL_MAXIMISER].status>0)	//fitted?
{
	//yes, just fix it
	ship_items[KFUEL_MAXIMISER].status=1;
}
else	//no fuel maximiser
{
	ship_items[KFUEL_MAXIMISER].status=0;
	ship_items[KFUEL_MAXIMISER].modifier=0;
	ship_items[KFUEL_MAXIMISER].value1=0;
}

if (ship_items[KAUTO_HULL_REPAIR].status>0)
{
	ship_items[KAUTO_HULL_REPAIR].status=1;
}
else
{
	ship_items[KAUTO_HULL_REPAIR].status=0;
	ship_items[KAUTO_HULL_REPAIR].modifier=0;
	ship_items[KAUTO_HULL_REPAIR].value1=0;
}

if (ship_items[KREAR_LASER].status>0)
{
	ship_items[KREAR_LASER].status=1;	
}
else
{
	ship_items[KREAR_LASER].status=0;	//should be 0
	ship_items[KREAR_LASER].modifier=0;	//same
	ship_items[KREAR_LASER].value1=0;
}

if (ship_items[K360].status>0)
{
	ship_items[K360].status=1;
}
else
{
	ship_items[K360].status=0;
	ship_items[K360].obj_class=WEAPONS;
	ship_items[K360].modifier=0;
	ship_items[K360].value1=0;
}

if(ship_items[KSHOCKWAVE].status>0)
{
	ship_items[KSHOCKWAVE].status=1;
}
else
{
	ship_items[KSHOCKWAVE].status=0;
	ship_items[KSHOCKWAVE].modifier=1;
	ship_items[KSHOCKWAVE].value1=0;
}


if (ship_items[KECM].status>0)
{
	ship_items[KECM].status=1;
}
else
{
	ship_items[KECM].status=0;
	ship_items[KECM].modifier=1;
	ship_items[KECM].value1=0;
}

if (ship_items[KMISSILE_FACTORY].status>0)
{
	ship_items[KMISSILE_FACTORY].status=1;
}
else
{
	ship_items[KMISSILE_FACTORY].status=0;
	ship_items[KMISSILE_FACTORY].modifier=1;
	ship_items[KMISSILE_FACTORY].value1=0;
}

if (ship_items[KFAST_DOCK].status>0)
{
	ship_items[KFAST_DOCK].status=1;
}
else
{
	ship_items[KFAST_DOCK].status=0;
	ship_items[KFAST_DOCK].modifier=0;
	ship_items[KFAST_DOCK].value1=0;
}

if (ship_items[KRETRO_ROCKETS].status>0)
{
	ship_items[KRETRO_ROCKETS].status=1;
}
else
{
	ship_items[KRETRO_ROCKETS].status=0;
	ship_items[KRETRO_ROCKETS].value1=3;
	ship_items[KRETRO_ROCKETS].modifier=0;
}

if (ship_items[KHYPERSPACE].status>0)
{
	ship_items[KHYPERSPACE].status=1;
}
else
{
	ship_items[KHYPERSPACE].status=0;
	ship_items[KHYPERSPACE].value1=0;
	ship_items[KHYPERSPACE].modifier=0;
}


if (ship_items[KTRADING_COMPUTER].status>0)
{
	ship_items[KTRADING_COMPUTER].status=1;
}
else
{
	ship_items[KTRADING_COMPUTER].status=0;
	ship_items[KTRADING_COMPUTER].modifier=0;
	ship_items[KTRADING_COMPUTER].value1=0;
}

if (ship_items[KAUTO_HULL_REPAIR].status>0)
{
	ship_items[KAUTO_HULL_REPAIR].status=1;
}
else
{
	ship_items[KAUTO_HULL_REPAIR].status=0;
	ship_items[KAUTO_HULL_REPAIR].modifier=0;
	ship_items[KAUTO_HULL_REPAIR].value1=0;
}

ship_items[KBOID_ADDITION].status=0;
ship_items[KBOID_ADDITION].modifier=0;
ship_items[KBOID_ADDITION].value1=0;

if (ship_items[KASTEROID_ADDITION].status>0)
{
	ship_items[KASTEROID_ADDITION].status=1;
}
else
{
	ship_items[KASTEROID_ADDITION].status=0;
	ship_items[KASTEROID_ADDITION].modifier=0;
	ship_items[KASTEROID_ADDITION].value1=0;
}






}


static int need_to_show_cp;
extern	DynObjectsFixedSize *ocb_ptr;
extern int missile_flip;
extern UInt32 gThrustUp;	//from input_control
extern UInt32 gThrustDown;	//from input_control

void apply_control_inputs()
{
//new routine 14/1298
//SB
extern _3D ship_normal;	//Start pointing out of screen
extern _3D ship_rotated_normal; //from engine
extern int gRadarRange;
extern int game_pause;	//1=pause, 0 = run

extern	int ship_y_ang,ship_x_ang,ship_z_ang;
extern int thrust,zero_crossed,retro;
extern float turn;
extern float camera_zoom;

extern UInt32 gTurn;
extern UInt32 gCannon;
extern UInt32 gMissile;
extern UInt32 gLaser;
extern UInt32 gRetro;
extern UInt32 gWarpDrive;
extern UInt32 gShockwave;

extern UInt32 gAbort;
extern UInt32 gMusic_up;
extern UInt32 gMusic_down;

extern int music_volume;
extern int sound_volume;
extern UInt32 gSound_up;
extern UInt32 gSound_down;
extern UInt32 gRadarRangeInput;
extern UInt32 gvel_limiter_input;
extern UInt32 gEject;
extern UInt32 gCamera_zoom_in;
extern UInt32 gCamera_zoom_out;
extern UInt32 gMissileToggle;
extern UInt32 gShowInventory;
extern UInt32 gAskForDock;
extern UInt32 gGM;

extern int real_frame_counter;
extern int quit;
extern int global_show_status;
extern int camera_mode,added_nav;
extern int global_request_dock,pilot_asleep;

float dx,dy;	//thrust components
SInt32 signed_turn;
float fturn;

int thrust_roll_input;
int tacc,mint,maxt;
static int old_CPMode=0;

//run exhaust anim
run_exhaust_animation();

//check if we added a nav computer
if (added_nav==1)
{
      add_to_text_display("MK I NAV COMPUTER FITTED. 400 CREDITS CHARGED.",1);
      added_nav=0;
}


if ((*ocb_ptr).object_list[0].request_dock==1) global_request_dock=0;	//shove our ID in to request a dock please


//store old x and y
(*ocb_ptr).object_list[0].previous_x=(*ocb_ptr).object_list[0].Oworldx;
(*ocb_ptr).object_list[0].previous_y=(*ocb_ptr).object_list[0].Oworldy;

//ееMISC CONTROLS WE ALWAYS PROCESS
//handle music volume
if (gMusic_up==1)
{ 
  if (music_volume<63) 
   {
   music_volume++;
   music_set_vol(music_volume);
   }
}

if (gMusic_down==1)
{
 if(music_volume>0) 
 {
  music_volume--;
   music_set_vol(music_volume);
 }
}





//handle sound volume
if (gSound_up==1 && sound_volume<63) sound_volume++;

if (gSound_down==1 && sound_volume>0) sound_volume--;


//handle camera zoom
if (gCamera_zoom_in==1 && camera_zoom>MIN_CAMERA_ZOOM) camera_zoom-=60;

if (gCamera_zoom_out==1 && camera_zoom<MAX_CAMERA_ZOOM) camera_zoom+=60;

//handle quit game - esc key
if (gAbort==1 && ZexTickCount()>last_pause_frame)	//ESC key
  {
   last_pause_frame=ZexTickCount()+20;
   game_pause=1;
   if (global_show_status!=0)
   reset_screen_from_status();	//we were showin the status screen
   else
   if (gShowGalacticMap!=0)
   reset_screen_from_status();	//we were showin the status screen
   
  }
  
//Galactic map control
#define TRINARY_GM 0	//make this 1 for trinary mode GM (off, normal, transparent)
#define MAX_GM_MODE TRINARY_GM+1

if (gGM==1 && ZexTickCount()>last_range_toggle_frame)	   //toggle radar range
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 gShowGalacticMap++; if ( gShowGalacticMap > MAX_GM_MODE )  gShowGalacticMap=0;	//normal game, GM or transparent GM

	if (gShowGalacticMap==2) //just become transparent; remember control panel state 'cause we're going to hide it
	{
		old_CPMode=show_control_panel;
		show_control_panel=1;
	}
	if (gShowGalacticMap==0) //just become normal view, restore CP state
	{
		show_control_panel=old_CPMode;
	}

	if (gShowGalacticMap==0 || gShowGalacticMap==2)	//we need to reset the screen if transparent GM
	{
		freeze=0;	//stop all object movement (doesn't stop controllers, just movement)
		extern int hull_strength;
		int old_hull_strength=hull_strength;
		reinit_screen();	//resets hull strength...
		hull_strength=old_hull_strength;
	}
	else	//we are showing the galactic map with no transparency
	{
		//change 040106 - if level is space cadet then freeze the game whilst showing galactic map
		if (difficulty==16)
		{
			freeze=1;
		}
	}

 last_range_toggle_frame=ZexTickCount()+10;
}
  
if (freeze==1) return;	//process no more keys if freeze is true

//command and control
if (gCommandControl==1 && ZexTickCount()>last_pause_frame)	 //ESC key
  {
   last_pause_frame=ZexTickCount()+20;
   if (global_show_status!=0)
   reset_screen_from_status();	//we were showin the status screen
   else
   if (gShowGalacticMap!=0)
   reset_screen_from_status();	//we were showin the status screen
   do_command_and_control();	//file
  }

if (gCameraMode==1 && ZexTickCount()>last_camera_toggle_frame)	//toggle radar range
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 last_camera_toggle_frame=ZexTickCount()+10;
 camera_mode++;
 if (camera_mode==1) 
 {
 camera_mode=2;	//modes 1	 
// add_to_text_display("CAMERA MODE: 1",1);
 
 }
 if (camera_mode==3) 
 {
 camera_mode=0;	//and 3 not allowed in Zex
// add_to_text_display("CAMERA MODE: 3",1);
 }
 if (camera_mode>3) 
 {
 camera_mode=0;
 add_to_text_display("CAMERA MODE: NORMAL",1);
 }
}
//end of inputs always processed

//if (level_has_control==1 || pilot_asleep==1) return;	     //level is controlling zex
if (level_has_control==1) return;	//level is controlling zex


if ((*ocb_ptr).object_list[0].controller!=zex) return;	//probs replaced with an ejection pod

extern int fastdock_in_progress;

//allow the request_dock to be toggled about twice a second max
if (gAskForDock==1 && ZexTickCount()>last_dock_toggle_frame && !fastdock_in_progress)	//request dock
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
(*ocb_ptr).object_list[0].request_dock^=1;
if ((*ocb_ptr).object_list[0].request_dock==1)
     
     add_to_text_display("OBC> Transmitting docking request signal. Controls relaxed.",1);
else
{
     add_to_text_display("OBC> Docking request signal cancelled. Controls engaged.",1);
//if we ere being docked, cancel it
     cancel_dock(0);

}
 last_dock_toggle_frame=ZexTickCount()+10;

}

//handle eject
if (gEject==1 && have_ejected==0 && ZexTickCount()>last_dock_toggle_frame)	//manual eject
{
 if (ship_items[KEJECTION].status<=0)
 {
   add_to_text_display("OBC> What ejection pod?!",0); 
 }
 else
 if (launch_timer==0)
 {
  int closest_mship;
  
     closest_mship=find_nearest_mothership_to_object(0,-1);
      //SB 040699
      if (closest_mship==-1)	//no motherships in area!
      {
      add_to_text_display("OBC> Can't eject! No motherships in vicinity.",1);
      return; 
      }

  do_ejection();
 }
  last_dock_toggle_frame=ZexTickCount()+60;
}


//handle Shockwave
if (gShockwave==1 && ZexTickCount()>last_dock_toggle_frame)	//Fire Shockwave
{
 if (ship_items[KSHOCKWAVE].status<=0)
 {
   add_to_text_display("OBC> Shockwave not fitted",0); 
 }
 else
 {
   if (ship_items[KSHOCKWAVE].value1>0) ship_items[KSHOCKWAVE].value1--;
	add_to_text_display("OBC> Shockwave fired!",1);
	//do shockwave processing here
    do_shockwave(ship_items[KSHOCKWAVE].modifier);
   if (ship_items[KSHOCKWAVE].value1==0)  	//the last shot?
    {
     if (RangedRdm(0,10)>5)
	 add_to_text_display("OBC> It is with great sadness I have to report that the shockwave has expired",1);
     else
	 add_to_text_display("OBC> Shockwave has expired.",1);

	 ship_items[KSHOCKWAVE].status=0;	//yes, remove shockwave
    }
 }
 last_dock_toggle_frame=ZexTickCount()+60;
}


//handle Warp Drive
if (gWarpDrive==1 && ZexTickCount()>last_dock_toggle_frame)	//manual eject
{
 if (ship_items[KHYPERSPACE].status<=0)
 {
   add_to_text_display("OBC> Warp jump not fitted",0); 
 }
 else
 {
   if (ship_items[KHYPERSPACE].value1>0) ship_items[KHYPERSPACE].value1--;
	add_to_text_display("OBC> Warp jump fired!",1);
	play_zsound(space_synth, sound_high_pri, c1_chan1, sound_vol_7);

	//do Warp jump processing here
	do_warp_jump(ship_items[KHYPERSPACE].modifier);

   if (ship_items[KHYPERSPACE].value1==0)  	//the last jump?
    {
	 add_to_text_display("OBC> Warp jump has expired",1);	 
	 ship_items[KHYPERSPACE].status=0;	//yes, remove Warp drive
    }
 }
 last_dock_toggle_frame=ZexTickCount()+60;
}


//allow the radar to be toggled about twice a second max
if (gRadarRangeInput==1 && ZexTickCount()>last_range_toggle_frame)	//toggle radar range
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 gRadarRange^=1;
 last_range_toggle_frame=ZexTickCount()+10;
}





if (gCPMode==1 && ZexTickCount()>last_range_toggle_frame)	//toggle control panel
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 show_control_panel^=1;
 if (show_control_panel==0)
 
     reset_screen_from_status();	//we were showin the status screen, will reenable the control panel

 last_range_toggle_frame=ZexTickCount()+10;
 
 #if OGL==1
 if (show_control_panel==1) ogl_hide_control_panel();
 else
 ogl_load_control_panel;
 #endif
}



if (gvel_limiter_input==1 && ZexTickCount()>last_range_toggle_frame)	  //toggle vel limitter
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 gvel_limiter++;
 if (gvel_limiter>1) gvel_limiter=0;
 
 if (gvel_limiter==1)
       {
       add_to_text_display("ORCS> THRUST CONTROL TO CRUISE MODE",1);
       unlim_thrust=thrust;
       directional_control=1;
       }
 else      
 if (gvel_limiter==0)
       {
       add_to_text_display("ORCS> THRUST CONTROL TO COMBAT MODE",1);
       
       if (unlim_thrust<360)
        thrust=unlim_thrust;
        
        else
        if (unlim_thrust>=360)
        {
        thrust=360;
         play_zsound(out_of_cruise, sound_high_pri, c1_chan1, sound_vol_7);

        }
        else
        if (unlim_thrust>-100) 
        thrust=unlim_thrust;
        else 
        if (unlim_thrust<=-100)
        thrust=-100;
 
       unlim_thrust=0;
       directional_control=1;
       }
 else      
 if (gvel_limiter==2)
       {
       add_to_text_display("ORCS> OFF-LINE!",1);
       unlim_thrust=1;
       directional_control=0;
       }

 last_range_toggle_frame=ZexTickCount()+10;
}


//handle show inv key /
if (gShowInventory==1 && ZexTickCount()>last_inv_toggle_frame)	//toggle radar range
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 last_inv_toggle_frame=ZexTickCount()+10;;
   
   #if OGL==1
    if (global_show_status==0)
    {
     ogl_hide_control_panel();
    }
  #endif

 global_show_status++;	//set/reset global variable
 if(global_show_status == 2)
 {
	 LS_ShowCursor();
 }
 else	// on main game screen and on first screen there is no cursor
 {
	 LS_HideCursor();
 }
 
 if (global_show_status>2) global_show_status=0;
 
 if (global_show_status==0)	//we need to reset the screen
 {
  reset_screen_from_status();
  //reshow control panel
   #if OGL==1
    ogl_hide_control_panel();
    if (show_control_panel==0)
    ogl_load_control_panel();
  #endif
 }
}




//if ((*ocb_ptr).object_list[0].request_dock==1) return;	//dock request



if ((*ocb_ptr).object_list[0].invincible_count>1)
{
// (*ocb_ptr).object_list[0].invincible_count--;	//set when we die
 (*ocb_ptr).object_list[0].eng_invisible=(*ocb_ptr).object_list[0].eng_invisible^1;	//flash it
}

if ((*ocb_ptr).object_list[0].invincible_count==1) 
{
(*ocb_ptr).object_list[0].eng_invisible=0;	//set when we die
(*ocb_ptr).object_list[0].invincible_count=0;
}

//ееSHIP CONTROLS WE ONLY PROCESS IF SHIP IS ACTIVE
//   if (fire_button()) Debugger();	//press space to enter debugger
//see if we need to make zex visible
if (hide_zex>0)
   {
   hide_zex--;
   if (hide_zex==0)
    {
    (*ocb_ptr).object_list[0].eng_invisible=0;
    do_game_dead();
    }
   }

   run_ship_misc_control();

if (pilot_asleep==1) return;	   //level is controlling zex
   

if (tractor==0)
{
   signed_turn=gTurn;
   signed_turn-=0x7fffffff;
   
   fturn=signed_turn;	//+-0x80000000
   fturn=fturn/0x7fffffff;	//cant get rid of this floating div?
//turn is a proportional thing, which we need to get in the range of 0 to 1 from 0 to 7fffffff
   if (gTurn>0x90000000 && turn>-5 && turn<14  && (*ocb_ptr).object_list[0].request_dock==0)	//left arrow
   {
    fturn=gTurn-0x80000000;
    fturn=fturn/0x80000000;
	turn-=fturn;
	player_input_counter=0;	//reset wingmen sarcasm
    }
   else //no left input
   if (gTurn<0x70000000 && turn<5 && turn>-14  && (*ocb_ptr).object_list[0].request_dock==0)	//right arrow
   {
    fturn=0x80000000-gTurn;
    fturn=fturn/0x80000000;
    turn+=fturn;	//right turn Clyde
	player_input_counter=0;	//reset wingmen sarcasm
    }
   
   if (gTurn>0x70000000 && gTurn<0x90000000)	//if no inputs
   {   
//auto center
   if (turn<-0.8) turn+=0.8;
   else
   if (turn>0.8) turn-=0.8;
   else turn=0;	//clamp to zero if between -.2 and +.2
   }
   
   if (ship_items[KFLIGHT_COMPUTER].status<=0) turn*=0.7;	//computer out? Reduce turning moment.
   
   if ( ship_items[KNORMAL_FUEL_TANK].value1>1  ) ship_z_ang+= static_cast<int>(turn);
   


//thrust - vel limiter on
if (gvel_limiter==0)
{
   if (retro>-50)
   {
    if (ship_items[KENGINE].status==1)
    {
    maxt=MAX_VELOCITY;
    mint=-100;
    tacc=15;
    }
    else
    {
    maxt=160;
    mint=-70;
    tacc=5;
    }

if (thrust_lockout==1) gThrustUp=gThrustDown=0;
  
   if (gThrustUp==1  && (*ocb_ptr).object_list[0].request_dock==0)
   {
   player_input_counter=0;	//reset wingmen sarcasm

   if (thrust<maxt && ship_items[KNORMAL_FUEL_TANK].value1>1)	//>
	      thrust+=tacc;
   }
   else
   if (gThrustDown==1  && (*ocb_ptr).object_list[0].request_dock==0)
    {
     player_input_counter=0;	//reset wingmen sarcasm
     if (thrust>mint  && ship_items[KNORMAL_FUEL_TANK].value1>1)	//<
	      thrust-=tacc;
    }
   }
   else	//retro fired
   {
     retro+=(ABS(retro)/32);
     thrust=retro;
   }

//check for out of fuel
if (ship_items[KNORMAL_FUEL_TANK].value1<1)
{
if (thrust>0) thrust--;
else
if (thrust<0) thrust++;
}
   dy=ship_rotated_normal.y*thrust;
   dx=ship_rotated_normal.x*thrust;
(*ocb_ptr).object_list[0].current_velocity=thrust;

}
else	//vel limitter off!
{

   if (retro>-50)
   {
    if (ship_items[KENGINE].status==1)
    {
    maxt=MAX_VELOCITY;
    mint=-100;
    tacc=15;
    }
    else
    {
    maxt=160;
    mint=-70;
    tacc=5;
    }

if (thrust_lockout==1) gThrustUp=gThrustDown=0;
  
   if (gThrustUp==1  && (*ocb_ptr).object_list[0].request_dock==0 && unlim_thrust<1020)
   {
   player_input_counter=0;	//reset wingmen sarcasm
   if (unlim_thrust<20)
   unlim_thrust++;
   else
   unlim_thrust+=9;
   
      
   if (ship_items[KNORMAL_FUEL_TANK].value1>1)	//>
	      thrust=320;
   }
   else
   if (gThrustDown==1  && (*ocb_ptr).object_list[0].request_dock==0 && unlim_thrust>-400)
    {
   if (unlim_thrust>20)
   unlim_thrust-=9;
   else
   unlim_thrust--;
   
     player_input_counter=0;	//reset wingmen sarcasm
     if (ship_items[KNORMAL_FUEL_TANK].value1>1)	//<
	      thrust=-100;
    }
   
   if (gThrustUp==0 && gThrustDown==0) thrust=0;
   }
   else	//retro fired
   {
     
     retro+=(ABS(retro)/32);
     thrust=retro;
     gvel_limiter=0;
   }

//check for out of fuel
if (ship_items[KNORMAL_FUEL_TANK].value1<1)
{
if (thrust>0) thrust--;
else
if (thrust<0) thrust++;


}

if (directional_control==1)
{
   dy=ship_rotated_normal.y*unlim_thrust;
   dx=ship_rotated_normal.x*unlim_thrust;
(*ocb_ptr).object_list[0].current_velocity=unlim_thrust;
}
else	//orcs disabled
{
float actual_thrust;
float current_velocity;

  dx=(*ocb_ptr).object_list[0].Omovedx;
  dy=(*ocb_ptr).object_list[0].Omovedy;

(*ocb_ptr).object_list[0].current_velocity= static_cast<int>(dx+dy);
current_velocity=dx+dy;

actual_thrust=0;
//if (gThrustDown==1) actual_thrust=-10;
//else
//if (gThrustUp==1) actual_thrust=10;


//dx=dx+(sin(heading)*actual thrust)
//dy=dy+(cos(heading)*actual thrust)
if (actual_thrust!=0)
 {
  
  dx=dx+ship_rotated_normal.x*actual_thrust;
  dy=dy+ship_rotated_normal.y*actual_thrust;
 }
else	//no thurst input
 {
  if (dx>0) dx-=0.2;
  else
  if (dx<0) dx+=0.2;
  if (dy>0) dy-=0.2;
  else
  if (dy<0) dy+=0.2;
 }

//do clamping according to computer fitted

//else	  //fc is working
//  {
//   if (dx>1210) dx-=100;
//   else
//   if (dx<-400) dx+=100;
//   
//   if (dy>1210) dy-=100;
//   else
//   if (dy<-400) dy+=100;
//      
//  }

 }	//end of orcs disabled


}	//end of vel limiter off

//calc components


 
//adjust and update extern_rotdz;
if ((*ocb_ptr).object_list[0].extern_rotdz!=0)
{
 if ((*ocb_ptr).object_list[0].extern_rotdz>0) (*ocb_ptr).object_list[0].extern_rotdz--;
 else
 (*ocb_ptr).object_list[0].extern_rotdz++;
 
 ship_z_ang+=(*ocb_ptr).object_list[0].extern_rotdz;
 
}

if (ship_z_ang>359) ship_z_ang-=359;
else
if (ship_z_ang<0) ship_z_ang+=359;

//use and adjust extern_movedx and y
if (ship_items[KFLIGHT_COMPUTER].status!=1)	//computer out?
  {
   if ((*ocb_ptr).object_list[0].extern_movedx>0) (*ocb_ptr).object_list[0].extern_movedx-=1;   
   if ((*ocb_ptr).object_list[0].extern_movedx<0) (*ocb_ptr).object_list[0].extern_movedx+=1;

   if ((*ocb_ptr).object_list[0].extern_movedy>0) (*ocb_ptr).object_list[0].extern_movedy-=1;   
   if ((*ocb_ptr).object_list[0].extern_movedy<0) (*ocb_ptr).object_list[0].extern_movedy+=1;   
  }
else	//fc is working
{
 if (ship_items[KFLIGHT_COMPUTER].modifier==1)	//computer type 1
  {
   if ((*ocb_ptr).object_list[0].extern_movedx>0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedx>2)
    (*ocb_ptr).object_list[0].extern_movedx-=3;   
    else
    (*ocb_ptr).object_list[0].extern_movedx=0;   
    
   }
   else
   if ((*ocb_ptr).object_list[0].extern_movedx<0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedx<-2)
    (*ocb_ptr).object_list[0].extern_movedx+=3;   
    else
    (*ocb_ptr).object_list[0].extern_movedx=0;   
   }
   
   
   if ((*ocb_ptr).object_list[0].extern_movedy>0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedy>2)
    (*ocb_ptr).object_list[0].extern_movedy-=3;   
    else
    (*ocb_ptr).object_list[0].extern_movedy=0;   
    
   }
   else
   if ((*ocb_ptr).object_list[0].extern_movedy<0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedy<-2)
    (*ocb_ptr).object_list[0].extern_movedy+=3;   
    else
    (*ocb_ptr).object_list[0].extern_movedy=0;   
   }
  }	//end of type 1 orcs

 else	//must be type  2
 
 {
   if ((*ocb_ptr).object_list[0].extern_movedx>0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedx>4)
    (*ocb_ptr).object_list[0].extern_movedx-=5;   
    else
    (*ocb_ptr).object_list[0].extern_movedx=0;   
    
   }
   else
   if ((*ocb_ptr).object_list[0].extern_movedx<0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedx<-4)
    (*ocb_ptr).object_list[0].extern_movedx+=5;   
    else
    (*ocb_ptr).object_list[0].extern_movedx=0;   
   }
   
   
   if ((*ocb_ptr).object_list[0].extern_movedy>0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedy>4)
    (*ocb_ptr).object_list[0].extern_movedy-=5;   
    else
    (*ocb_ptr).object_list[0].extern_movedy=0;   
    
   }
   else
   if ((*ocb_ptr).object_list[0].extern_movedy<0) 
   {
    if ((*ocb_ptr).object_list[0].extern_movedy<-4)
    (*ocb_ptr).object_list[0].extern_movedy+=5;   
    else
    (*ocb_ptr).object_list[0].extern_movedy=0;   
   }
 }	//end of type 2 ORCs
 
}

   dx+=(*ocb_ptr).object_list[0].extern_movedx;
   dy+=(*ocb_ptr).object_list[0].extern_movedy;

//if directional control off, half the extern forces
if (directional_control==0)
{
  if ((*ocb_ptr).object_list[0].extern_movedy!=0) 
  (*ocb_ptr).object_list[0].extern_movedy=0;
  if ((*ocb_ptr).object_list[0].extern_movedx!=0) 
  (*ocb_ptr).object_list[0].extern_movedx=0;

}

//   if (dx>1210) dx-=100;
//   else
//   if (dx<-400) dx+=100;
//   
//   if (dy>1210) dy-=100;
//   else
//   if (dy<-400) dy+=100;


   ((*ocb_ptr).object_list[0].Omovedy)=dy;
   ((*ocb_ptr).object_list[0].Omovedx)=dx;

//   ((*ocb_ptr).object_list[0].thrust_y)=dy;
//   ((*ocb_ptr).object_list[0].thrust_x)=dx;


//calc ship roll - no roll if computer knacked
if (gvel_limiter==1)
{
 int temp_vel;
 temp_vel=ABS(unlim_thrust);
 if (temp_vel>320) temp_vel=320;
 thrust_roll_input=temp_vel>>4; 
}
else
thrust_roll_input=abs_nobranch(thrust>>4);	//was div 20

if (ship_items[KFLIGHT_COMPUTER].status==1)	//working
  {
   if (thrust_roll_input>8) thrust_roll_input=8;
     ((*ocb_ptr).object_list[0].Orotx)= static_cast<int>(turn*thrust_roll_input);	//ship roll on turn
  }
     
     ((*ocb_ptr).object_list[0].Orotz)=ship_z_ang;	//ship yaw

//handle fire
if (gCannon==1  && (*ocb_ptr).object_list[0].request_dock==0)	     //Default space bar key
do_player_fire();

if (gLaser==1 &&  ship_items[KNORMAL_FUEL_TANK].value1>1  && (*ocb_ptr).object_list[0].request_dock==0)	//Default L key
do_player_laser();

//handle missile
if (gMissile==1 &&  ship_items[KNORMAL_FUEL_TANK].value1>1  && (*ocb_ptr).object_list[0].request_dock==0)	//Default M key
do_player_missile();

//handle missile pylon toggle - P key
if (gMissileToggle==1 && ZexTickCount()>last_missile_toggle_frame)	//toggle radar range
{
 play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
 last_missile_toggle_frame=ZexTickCount()+10;
 select_next_pylon();	//check missile available
}

if (ship_items[KECM].status!=0 && ship_items[KNORMAL_FUEL_TANK].value1>1)
{
	handle_ecm(0);	//we need fuel for the ECM to scan
}

run_missile_factory();	//only provides level 1 missiles

}	//end of tractor==0 

else	//tractor==1
{

 if (gTurn!=0x7fffffff) add_to_text_display("OBC> Controls disabled - tractor beam.",0); 
}
//еееееееееееееееееееееееееееееееееееееееееееееееееееееBINARY INPUTS



//handle retro rockets
if (gRetro==1 && retro>-50 && ZexTickCount()>last_dock_toggle_frame)	//t key
{
  if (ship_items[KRETRO_ROCKETS].status==1)	//retros fitted
  {
   if (ship_items[KRETRO_ROCKETS].value1>0) ship_items[KRETRO_ROCKETS].value1--;
   if (ship_items[KRETRO_ROCKETS].value1==0)  	//the last shot?
    {
     add_to_text_display("OBC> Sincere apologies boss, but the retro thrust has expired",1);
     if (RangedRdm(0,10)>5)
     add_to_text_display("OBC> I know the word you are thinking of...",1);
     ship_items[KRETRO_ROCKETS].status=0;	//yes, remove retro
    }
    
   retro=-1800;
   thrust=0;
   unlim_thrust=0;
   play_zsound(retro_sound, sound_high_pri, c1_chan1, sound_vol_7);
  }
  else
  add_to_text_display("OBC> Retro thrust not fitted",1);
 last_dock_toggle_frame=ZexTickCount()+60;
}

}



void do_ejection()
{
 //do the ejection.
 //1. Load the ship into a new slot with current position and orientation - controller is exploding_zex.
 //2. Load ejection pod into slot 0 - controller is player_ejection_pod
 //3. Set have_ejected
 //4. Note use part 1 for "ghost ship" weapon!
 load_dyn_object_pb the_params;	//for ejection
 int slot,static_slot,closest_mship,temp;
 extern int	dynamic_object_index,credits,promotion_points;	//how many dynamic objects to look at
 extern mothership_nav_entry mothership_nav [10];

clear_pb(&the_params);	//saves loads of zero code
 	    //knock off 30% of credits
	   if (credits>0) credits=credits-((credits/10)*3);
	   add_to_text_display("EJECTION: CREDITS REDUCED BY 30 PERCENT FOR NEW SHIP AWAITING AT MOTHERSHIP.",1);

 //1 - load ship into new slot
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
    the_params.world_x=(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy;
	the_params.world_z=(*ocb_ptr).object_list[0].Oworldz;
	the_params.Omovedx=(*ocb_ptr).object_list[0].Omovedx;
	the_params.Omovedy=(*ocb_ptr).object_list[0].Omovedy;
	the_params.Omovedz=10;
	the_params.delta_rot_x=RangedRdm(0,10)-5;
//	the_params.delta_rot_y=0;
	the_params.delta_rot_z=RangedRdm(0,10)-5;	//spin zex
	if (the_params.delta_rot_z==0) the_params.delta_rot_z=2;
	the_params.rot_x=(*ocb_ptr).object_list[0].Orotx;
	the_params.rot_y=(*ocb_ptr).object_list[0].Oroty;
	the_params.rot_z=(*ocb_ptr).object_list[0].Orotz;
	the_params.controller_ref=exploding_zex;
//	the_params.control_use_1=0;
	the_params.control_use_2=45; //300 frames to die
//	the_params.control_use_3=0;
//	the_params.control_use_4=0;
//	the_params.control_use_7=0;
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
	the_params.shield_value=1000;
//     the_params.has_roll=0;
	the_params.mass=100;
//		 	 the_params.dq=0;
	the_params.object_category=zex_object;
	the_params.laser_cat=1;
	the_params.cannon_cat=1;
	the_params.missile_cat=1;
//	the_params.number_of_missiles=0;

     the_params.who_spawned_this_object=-1;
	 the_params.unique_id=0;
	load_dyn_object(zex,slot,&the_params,-1);	//the object, the position (-1=next free)
	(*ocb_ptr).object_list[slot].no_trivial_rejection=1;	//so controller will run even if not visible
   }
 //2 - load ejection pod into 0

    the_params.world_x=(*ocb_ptr).object_list[0].Oworldx;
	the_params.world_y=(*ocb_ptr).object_list[0].Oworldy;
	the_params.world_z=(*ocb_ptr).object_list[0].Oworldz;
	the_params.Omovedx=(*ocb_ptr).object_list[0].Omovedx;
	the_params.Omovedy=(*ocb_ptr).object_list[0].Omovedy;
	the_params.Omovedz=(*ocb_ptr).object_list[0].Omovedz;
	the_params.delta_rot_x=RangedRdm(0,30)-15;
	the_params.delta_rot_y=0;
	the_params.delta_rot_z=0;
	the_params.rot_x=(*ocb_ptr).object_list[0].Orotx;
	the_params.rot_y=(*ocb_ptr).object_list[0].Oroty;
	the_params.rot_z=(*ocb_ptr).object_list[0].Orotz;
//	the_params.controller_ref=0;
	the_params.controller_ref=player_ejection_pod;
	
	the_params.control_use_1=0;	//init velocity
	the_params.control_use_2=0;
	the_params.control_use_3=30;
//5 and 6 are x and y of nearest mothership. Find nearest
    closest_mship=find_nearest_mothership_to_object(0,-1);
      //SB 040699
      if (closest_mship==-1)	//no motherships in area!
      {
      add_to_text_display("OBC> Can't eject! No motherships in vicinity.",0);
      return; 
      }
  
	the_params.control_use_4= static_cast<int>(mothership_nav[closest_mship].x);
	the_params.control_use_7= static_cast<int>(mothership_nav[closest_mship].y);
//	the_params.control_use_7=0;
//	the_params.control_use_4=0;
	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
     the_params.has_roll=0;
	the_params.shield_value=100;
	the_params.mass=100;
		 	 the_params.dq=0;
	the_params.object_category=zex_object;
	the_params.laser_cat=1;
	the_params.cannon_cat=1;
	the_params.missile_cat=1;
	the_params.number_of_missiles=0;
    static_slot=get_static_object_slot('EC01');	//go fetch fido (the object)
    if (static_slot==-1) report_error_id ("ship_control: Eject couldn't find the ejector pod in static ocbs: ",'EC01');
	 the_params.unique_id=0;
     the_params.who_spawned_this_object=-1;
	load_dyn_object(static_slot,0,&the_params,-1);	   //the object, the position (-1=next free)
 
    (*ocb_ptr).object_list[0].crashable=0;  
    (*ocb_ptr).object_list[0].request_dock=1;	//050699 - fix to get epods to dock correctly 
//IMPORTANT because load_dyn_object incs dynamic index and we have called it twice!!!! but only actually
//loaded 1 new object, we to dec it
   dynamic_object_index--;
   clear_down_ship_on_eject();
   have_ejected=1;

      add_to_text_display("OBC> Launched ejection pod. Set auto destruct.",0);
	  //0.97.1 - show reduction in promotion points
	  if (promotion_points>0)
	  {
		add_to_text_display("OBC> Message from Command: Promotion points reduced.",1);
      }
      temp=RangedRdm(0,100);
      if (temp<20)
      add_to_text_display("OBC> Goodbye cruel world.",0);
      else
      if (temp<40)
      add_to_text_display("OBC> My name was George. Goodbye.",0);
      else
      if (temp<60)
      add_to_text_display("OBC> I hope my sacrifice will not be in vain. Sob.",0);
      
      
      promotion_points-=100;	
      if (promotion_points<0) promotion_points=0;
     run_ejection_music();
//   camera_mode=1;
//camera_zoom=600;
}






void clear_down_ship_on_eject()
{
extern int hull_damage,hull_strength;
extern int thrust;

thrust=0;
unlim_thrust=0;
directional_control=1;

ship_items[KSHIELDS].value1=0;
//			 hull_damage=50;
			 damage_me_no_sound(180);
//             hull_strength=0;
ship_items[KRADAR].status=0;
ship_items[KLONG_RANGE_RADAR].status=0;
ship_items[KCANNON].status=0;
ship_items[KMISSILES].status=0;
ship_items[KREPAIR_SYSTEMS].status=0;
ship_items[KLASER].status=0;
ship_items[KNORMAL_FUEL_TANK].value1=0;
ship_items[KNAV_COMPUTER].status=0;
ship_items[KMISSILE_PYLON1].modifier=0;
ship_items[KMISSILE_PYLON2].modifier=0;
ship_items[KMISSILE_PYLON3].modifier=0;
ship_items[KFLATPACK].value1=0;	//lost!
}

void reset_screen_from_status()
{
  int old_hull_strength;
  extern int hull_strength;
     old_hull_strength=hull_strength;
     reinit_screen();	//resets hull strength...
     hull_strength=old_hull_strength;
     global_show_status=0;
     gShowGalacticMap=0;
	 freeze=0;
}
//еееMoves pylon toggle to next valid pylon (one with a missile)


void select_next_pylon()
{
int count=0;	//there are only three pylons - this is the check
 
 while (count<3)
 {
 missile_flip++;
 if (missile_flip>2) missile_flip=0;
  {
    //check the pylon to see if a missile exists
    if (ship_items[KMISSILE_PYLON1+missile_flip].modifier!=0) return;	//got one
  }
 count++;	//check next pylon
 }	//end of while

missile_flip=-1;	//no valid pylons 
}
//ееееееееееееееLASERS
int laser_repeat;
extern int zex_alive,difficulty;

void do_player_laser()
{
int slot;
int object_aimed_at_slot=0;
float shot_x,shot_y,shot_z;
//_3D player_normal;
extern _3D ship_rotated_normal; //from engine
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
extern	int last_bullet_fired_frame;
extern int real_frame_counter;
extern kit_desc ship_items[ITEMS_COUNT];
//extern int shields;

_3D *this_object_points;
_ZPoly *the_polys;
textured_connections *pconnections;
short conn;
int laser_type,done_taser;

//int cannon_velocity;	  //depends on type of cannon
int distance_from_zex;
float laser_range;
load_dyn_object_pb the_params;


clear_pb(&the_params);	//saves loads of zero code

laser_type=ship_items[KLASER].modifier;
if (laser_type>10) laser_type-=10;	//take care of tasers

laser_range=(difficulty/2)*1000+laser_type*10000;	//difficulty is one of 16, 8 or 4 - 15000 level 1 //Beta 5 - was *7000
if (laser_range<10000) laser_range=10000;
laser_range*=LASER_RANGE_MULTIPLIER;
done_taser=0;	//loop counter

taser: if (done_taser==2) //reenter here if taser for second shot 
{
do_player_rear_laser();
return;	
} 

laser_repeat=30-laser_type*6;	//24 to 6 //Beta5 - was 40-laser_type*5
if ((real_frame_counter-laser_repeat>last_laser_fired_frame && zex_alive==1
   && ship_items[3].status==1 && ship_items[KNORMAL_FUEL_TANK].value1>0) || done_taser==1)	//cant shoot if dead or no fuel or lasers inop!
 { 
  last_laser_fired_frame=real_frame_counter;

  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{

	if (done_taser==1) //this next line may look a bit weird. Objewct aimed at slot is from the previous pass
	                   //and this time we want to exclude it from the search to pick another target.
	{
	   object_aimed_at_slot=aim_from_zex_exc_obj(object_aimed_at_slot,ship_items[KLASER].modifier,static_cast<int>(laser_range));
	}
	else
	{
		object_aimed_at_slot=aim_from_zex(ship_items[KLASER].modifier,static_cast<int>(laser_range));
    }
    #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad laser aimed at slot from zex >500");
	#endif
	
//	distance_from_zex=get_distance_to_zex(object_aimed_at_slot);
	if (object_aimed_at_slot==-1)
	{ 
	//ееееthis is a NOP in C -- might need to do something here...ееее 
	//add_to_text_display("LASERS: NOTHING TO TARGET.",0); 
        (*ocb_ptr).object_list[0].attack_what=-1;
	}
	else
	{
	 (*ocb_ptr).object_list[0].attack_what=object_aimed_at_slot;	//note what we're targetting
//change 280399 - laser now use fuel instead of shields
     if (RangedRdm(0,10)>6 && ship_items[KNORMAL_FUEL_TANK].value1>0) ship_items[KNORMAL_FUEL_TANK].value1--;
//     ship_items[KSHIELDS].value1=ship_items[KSHIELDS].value1-(*ocb_ptr).object_list[0].laser_category*10;	
//FRIG
//we no laonger use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	

	if (laser_type==1)
	 play_zsound_relative_to_zex(zex,laser1);
     else
	 if (laser_type==2)
	 play_zsound_relative_to_zex(zex,laser2);
     else
	 play_zsound_relative_to_zex(zex,laser3);
//        }
        
	 shot_x=(*ocb_ptr).object_list[0].Oworldx;
	 shot_x=shot_x+(400*ship_rotated_normal.x);
	 shot_y=(*ocb_ptr).object_list[0].Oworldy;
	 shot_y=shot_y+(400*ship_rotated_normal.y);
	 shot_z=(*ocb_ptr).object_list[0].Oworldz;
	
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
//	the_params.Omovedx=the_params.Omovedy=the_params.Omovedz=0;
//	the_params.delta_rot_x=the_params.delta_rot_y=the_params.delta_rot_z=0;
//	the_params.rot_x=the_params.rot_y=0;
//	     
//	   the_params.normal.x =  1;	   //objects are ALWAYS defined facing right
//	   the_params.normal.y =  0;	   //objects are ALWAYS defined facing right
//	   the_params.normal.z =  0;	   //objects are ALWAYS defined facing right
//	   the_params.rot_z=0;
	 the_params.controller_ref=ship_laser1;	//controller
	 the_params.control_use_1=laser_type*7;	//frames to live counter -7to21
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
//	the_params.control_use_3=0;
	 the_params.control_use_4=laser_type;	//laser obj_class
	 
//	the_params.control_use_7=doing_a_miss;
//	the_params.current_velocity=the_params.wanted_velocity=the_params.attack_mode=0;
//	the_params.laser_cat=the_params.cannon_cat=the_params.missile_cat=0;
//	the_params.number_of_missiles=0;

//     the_params.has_roll=the_params.shield_value=0;
	 the_params.mass=0;	//should be 0
//		 	 the_params.dq=0;
	 the_params.object_category=zex_object;
     the_params.who_spawned_this_object=zex;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)

//now set the poly to a zero shape
    this_object_points=(*ocb_ptr).object_list[slot].points_ptr;	//the points for this object
    the_polys=(*ocb_ptr).object_list[slot].polys_ptr;
    pconnections=(*the_polys).poly_connections_ptr;	

     conn=(*pconnections).the_node;
     pconnections++;
    (*(this_object_points+conn)).x=1;	//modify the poly
    (*(this_object_points+conn)).y=1;
    conn=(*pconnections).the_node;
     pconnections++;
   (*(this_object_points+conn)).x=2;
    (*(this_object_points+conn)).y=2;

    conn=(*pconnections).the_node;
     pconnections++;
   (*(this_object_points+conn)).x=2;
    (*(this_object_points+conn)).y=2;


	}	//end of good aimed at slot
    } //end of good slot
    
//check for taser
laser_type=ship_items[KLASER].modifier;
if (laser_type<10) 
{
do_player_rear_laser();
return;	//not a taser
}
laser_type-=10;	//take care of tasers
done_taser++;	//inc loop counter
goto taser;

 }

}	 //end of zex fires its laser


int rear_laser_repeat,last_rear_laser_fired_frame;
void do_player_rear_laser()
{
int slot;
float shot_x,shot_y,shot_z;
//_3D player_normal;
extern _3D ship_rotated_normal; //from engine
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
extern	int last_bullet_fired_frame;
extern int real_frame_counter;
extern int zex_alive;
extern kit_desc ship_items[ITEMS_COUNT];
//extern int shields;

_3D *this_object_points;
_ZPoly *the_polys;
textured_connections *pconnections;
short conn;
//int cannon_velocity;	  //depends on type of cannon
int object_aimed_at_slot,distance_from_zex;
load_dyn_object_pb the_params;

clear_pb(&the_params);	//saves loads of zero code
//calc if rear laser can fire
rear_laser_repeat=30-ship_items[KREAR_LASER].modifier*8;	//8 to 24 

if (real_frame_counter-rear_laser_repeat>last_rear_laser_fired_frame && zex_alive==1
   && ship_items[KREAR_LASER].status==1 && ship_items[KNORMAL_FUEL_TANK].value1>0
   && ship_items[K360].status==1)	//cant shoot if dead or no fuel or lasers inop!
 { 
  last_rear_laser_fired_frame=real_frame_counter;

  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
//	object_aimed_at_slot=aim_from_zex_360(12000);	//pass the distance limit
	object_aimed_at_slot=aim_from_zex_rear(ship_items[KREAR_LASER].modifier,static_cast<int>((12000.0*LASER_RANGE_MULTIPLIER)));	//pass the distance limit
    
    #ifdef studebug
	if (object_aimed_at_slot>500) DebugStr ("Bad rear laser aimed at slot from zex >500");
	#endif
	
//	distance_from_zex=get_distance_to_zex(object_aimed_at_slot);
	if (object_aimed_at_slot==-1)
 	 { 
	//ееееthis is a NOP in C -- might need to do something here...ееее 
	//add_to_text_display("LASERS: NOTHING TO TARGET.",0); 
	 }
	else
	 {
	 (*ocb_ptr).object_list[0].attack_what=object_aimed_at_slot;	//note what we're targetting
//change 280399 - laser now use fuel instead of shields
     if (RangedRdm(0,10)>6 && ship_items[KNORMAL_FUEL_TANK].value1>0) ship_items[KNORMAL_FUEL_TANK].value1--;
//     ship_items[KSHIELDS].value1=ship_items[KSHIELDS].value1-(*ocb_ptr).object_list[0].laser_category*10;	
//FRIG
//we no laonger use the engine to draw the laser - it's done with teh dlp,
//but we still need a laser object in there for the controller, so we insert
//a basic and very small triangle so as not to upset the renderer	
	 play_zsound_relative_to_zex(object_aimed_at_slot,laser1);

	 shot_x=(*ocb_ptr).object_list[0].Oworldx;
	 shot_x=shot_x+(400*ship_rotated_normal.x);
	 shot_y=(*ocb_ptr).object_list[0].Oworldy;
	 shot_y=shot_y+(400*ship_rotated_normal.y);
	 shot_z=(*ocb_ptr).object_list[0].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;
//	the_params.Omovedx=0;
//	the_params.Omovedy=0;
//	the_params.Omovedz=0;
//	the_params.delta_rot_x=0;
//	the_params.delta_rot_y=0;
//	the_params.delta_rot_z=0;
//	the_params.rot_x=0;
	
//	the_params.rot_y=0;
	   
//	the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	the_params.rot_z=0;
	 the_params.controller_ref=rear_ship_laser;	//controller
	 the_params.control_use_1=7;	//frames to live counter
	 the_params.control_use_2=object_aimed_at_slot;	//used by laser controller to control the poly.
//	the_params.control_use_3=0;
	 the_params.control_use_4=ship_items[KREAR_LASER].modifier;	//laser obj_class
//	the_params.control_use_7=0;
//	the_params.current_velocity=0;
//	the_params.wanted_velocity=0;
//	the_params.attack_mode=0;
//	the_params.laser_cat=0;
//	the_params.cannon_cat=0;
//	the_params.missile_cat=0;
//     the_params.has_roll=0;
//	the_params.number_of_missiles=0;

//	the_params.shield_value=0;
	the_params.mass=0;	//should be 0
//		the_params.dq=0;
     the_params.who_spawned_this_object=zex;
    the_params.object_category=zex_object;
	 the_params.no_trivial_rejection=1;
//	the_params.unique_id=0;
	 load_dyn_object(ship_laser1,slot,&the_params,-1);	//the static object, the position (-1=next free)

//now set the poly to a zero shape
    this_object_points=(*ocb_ptr).object_list[slot].points_ptr;	//the points for this object
    the_polys=(*ocb_ptr).object_list[slot].polys_ptr;
    pconnections=(*the_polys).poly_connections_ptr;	

     conn=(*pconnections).the_node;
     pconnections++;
    (*(this_object_points+conn)).x=1;	//modify the poly
    (*(this_object_points+conn)).y=1;
    conn=(*pconnections).the_node;
     pconnections++;
    (*(this_object_points+conn)).x=2;
    (*(this_object_points+conn)).y=2;

    conn=(*pconnections).the_node;
     pconnections++;
    (*(this_object_points+conn)).x=2;
    (*(this_object_points+conn)).y=2;


	 }	//end of aimed_at_slot!=-1
	}	//end of slot=ok

 }	//end of rear laser can fire


}	//end of zex fires its rear laser


//fire a missile - very similar to laser
void do_player_missile()
{
int slot;
float shot_x,shot_y,shot_z;
//_3D player_normal;
extern _3D ship_rotated_normal; //from engine
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
extern	int last_bullet_fired_frame;
extern int real_frame_counter;
extern int zex_alive;
//extern int instruments[6];

//_3D *this_object_points;
//_ZPoly *the_polys;
//textured_connections *pconnections;
//short conn;
//int cannon_velocity;	  //depends on type of cannon
int object_aimed_at_slot,distance_from_zex, pylon;
load_dyn_object_pb the_params;
anim_pb anim;	//animation pb


clear_pb(&the_params);	//saves loads of zero code
pylon=get_pylon();	//returns index into ships_items of selected pylon

if (real_frame_counter-20>last_laser_fired_frame && zex_alive==1 &&
    pylon!=-1 && ship_items[4].status==1 && ship_items[pylon].modifier>0)	//cant shoot if dead or no missiles or missiles inop!
  { 
  last_laser_fired_frame=real_frame_counter;
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	 (*ocb_ptr).object_list[0].attack_what=-1;	//object_aimed_at_slot;	//note what we're targetting

    play_zsound(missile_launch, sound_low_pri, c1_chan1, sound_vol_7);
    
    if (ship_items[pylon].modifier==1)
    {
    anim.object_id[0]='ZZ1A';
    anim.object_id[1]='ZZ1B';
    anim.object_id[2]='ZZ1C';
    the_params.dq=MISSILE_DAMAGE1;
    }
    else
    if (ship_items[pylon].modifier==2)
    {
    anim.object_id[0]='ZZ2A';
    anim.object_id[1]='ZZ2B';
    anim.object_id[2]='ZZ2C';
    the_params.dq=MISSILE_DAMAGE2;
    }
    else
    if (ship_items[pylon].modifier>2)
    {
    anim.object_id[0]='ZZ3A';
    anim.object_id[1]='ZZ3B';
    anim.object_id[2]='ZZ3C';
    the_params.dq=MISSILE_DAMAGE3;
    }
    
    
    ship_items[pylon].modifier=0;	//missile fired
    select_next_pylon();
	

	 shot_x=(*ocb_ptr).object_list[0].Oworldx;
	 shot_x=shot_x+(1000*ship_rotated_normal.x);
	 shot_y=(*ocb_ptr).object_list[0].Oworldy;
	 shot_y=shot_y+(1000*ship_rotated_normal.y);
	 shot_z=(*ocb_ptr).object_list[0].Oworldz;
		
	 the_params.world_x=shot_x;
	 the_params.world_y=shot_y;
	 the_params.world_z=shot_z;

        if(gvel_limiter==1)
        {
	the_params.Omovedx=(abs_nobranch(unlim_thrust)+200)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(unlim_thrust)+200)*ship_rotated_normal.y;
        }
        else
        {
	the_params.Omovedx=(abs_nobranch(thrust)+200)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(thrust)+200)*ship_rotated_normal.y;
        }



	the_params.rot_z=ship_z_ang;
	 the_params.controller_ref=missile1;	//controller
	 the_params.control_use_1=300;	//frames to live counter
	 the_params.control_use_2=-1;	//used by laser controller to control the poly.
	 the_params.control_use_3=zex;	//who fired this laser?


	the_params.shield_value=200;
	 the_params.mass=20;	//small for a missile
	 the_params.object_category=zex_object;
	 the_params.no_trivial_rejection=1;

//	load_dyn_object(ship_laser1,slot,&the_params);	//the static object, the position (-1=next free)
     the_params.who_spawned_this_object=zex;
    load_animated_object (&anim,3,1,1,1,&the_params,-1);


//	}
	}

 }


}	//end of zex fires a missile

//returns the selected pylon and missile type as a ship_items index
int get_pylon()
{
 if (missile_flip<0) return -1;
 return KMISSILE_PYLON1+missile_flip;
}
//еееееееееееееееееееееееееееееееееееееееееее


void do_player_fire()
{
int slot;
float shot_x,shot_y,shot_z;
//_3D player_normal;
extern _3D ship_rotated_normal; //from engine
extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
extern	int last_bullet_fired_frame;
extern int real_frame_counter;
int cannon_velocity;	//depends on type of cannon
load_dyn_object_pb the_params;
//extern int instruments[6];
if (freeze) return;	//on easy level firing gun when galactic map displayed causes destruction to Zex (because freese means the bullets dont move)

clear_pb(&the_params);	//saves loads of zero code
//weeny cannon
//if we're within 8 frames of last shot, then we can't fire
switch (ship_items[KCANNON].modifier)
{

case 1:
{
if (real_frame_counter>last_bullet_fired_frame && ship_items[2].status==1)
 { 

  if (difficulty==16)
  last_bullet_fired_frame=real_frame_counter+7;
  else
  if (difficulty==8)
  last_bullet_fired_frame=real_frame_counter+12;
  else
  if (difficulty==1)
  last_bullet_fired_frame=real_frame_counter+18;
  
  
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	cannon_velocity=550;	
	shot_x=(*ocb_ptr).object_list[0].Oworldx;
	shot_x=shot_x+(400*ship_rotated_normal.x);
	shot_y=(*ocb_ptr).object_list[0].Oworldy;
	shot_y=shot_y+(400*ship_rotated_normal.y);
	shot_z=(*ocb_ptr).object_list[0].Oworldz;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;

        if(gvel_limiter==1)
        {
	the_params.Omovedx=(abs_nobranch(unlim_thrust)+cannon_velocity)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(unlim_thrust)+cannon_velocity)*ship_rotated_normal.y;
        }
        else
        {
	the_params.Omovedx=(abs_nobranch(thrust)+cannon_velocity)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(thrust)+cannon_velocity)*ship_rotated_normal.y;
        }
//	the_params.Omovedz=0;
	the_params.delta_rot_x=10;

	the_params.rot_z=ship_z_ang;
	the_params.controller_ref=player_bullet_weeny;	//controller

	the_params.current_velocity=cannon_velocity;
	the_params.wanted_velocity=cannon_velocity;
//	the_params.attack_mode=0;
//	the_params.shield_value=0;
	the_params.mass=30; //beta 5 - mass was 20
	the_params.dq=GUN_DAMAGE1;
	the_params.object_category=zex_object;
	the_params.no_trivial_rejection=1;
//	the_params.laser_cat=0;
     the_params.who_spawned_this_object=zex;


	load_dyn_object(player_bullet_weeny,slot,&the_params,-1);	//the static object, the position (-1=next free)
	
//	play_zsound(short_explosion,sound_high_pri, c1_chan1, sound_vol_4);

	}

 }
 break;
}	//end of case 1


case 2:
{
if (real_frame_counter>last_bullet_fired_frame && ship_items[2].status==1)
 { 
  if (difficulty==16)
  last_bullet_fired_frame=real_frame_counter+4;
  else
  if (difficulty==8)
  last_bullet_fired_frame=real_frame_counter+8;
  else
  if (difficulty==1)
  last_bullet_fired_frame=real_frame_counter+12;
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	cannon_velocity=650; 
	shot_x=(*ocb_ptr).object_list[0].Oworldx;
	shot_x=shot_x+(400*ship_rotated_normal.x);
	shot_y=(*ocb_ptr).object_list[0].Oworldy;
	shot_y=shot_y+(400*ship_rotated_normal.y);
	shot_z=(*ocb_ptr).object_list[0].Oworldz;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;

        if(gvel_limiter==1)
        {
	the_params.Omovedx=(abs_nobranch(unlim_thrust)+cannon_velocity)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(unlim_thrust)+cannon_velocity)*ship_rotated_normal.y;
        }
        else
        {
	the_params.Omovedx=(abs_nobranch(thrust)+cannon_velocity)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(thrust)+cannon_velocity)*ship_rotated_normal.y;
        }


	the_params.Omovedz=0;
	the_params.delta_rot_x=10;
	the_params.rot_z=ship_z_ang;
	the_params.controller_ref=player_bullet_not_so_weeny;	//controller
	the_params.current_velocity=cannon_velocity;
	the_params.wanted_velocity=cannon_velocity;
	the_params.mass=50; //beta 5 - mass was 35
	the_params.dq=GUN_DAMAGE2;
	the_params.object_category=zex_object;
	the_params.no_trivial_rejection=1;
     the_params.who_spawned_this_object=zex;
	load_dyn_object(player_bullet_not_so_weeny,slot,&the_params,-1);	//the static object, the position (-1=next free)
//	play_zsound_always(short_explosion,sound_high_pri, stereo1_chan, sound_vol_7);
	}

 }
 break;
}	//end of case 2


case 3:
{
if (real_frame_counter>last_bullet_fired_frame && ship_items[2].status==1)
 { 
  if (difficulty==16)
  last_bullet_fired_frame=real_frame_counter+4;
  else
  if (difficulty==8)
  last_bullet_fired_frame=real_frame_counter+7;
  else
  if (difficulty==1)
  last_bullet_fired_frame=real_frame_counter+10;
  slot=find_vacant_dynamic_object_slot();
  if (slot!=-1)
	{
	cannon_velocity=850; 
	shot_x=(*ocb_ptr).object_list[0].Oworldx;
	shot_x=shot_x+(400*ship_rotated_normal.x);
	shot_y=(*ocb_ptr).object_list[0].Oworldy;
	shot_y=shot_y+(400*ship_rotated_normal.y);
	shot_z=(*ocb_ptr).object_list[0].Oworldz;
		
	the_params.world_x=shot_x;
	the_params.world_y=shot_y;
	the_params.world_z=shot_z;
        if(gvel_limiter==1)
        {
	the_params.Omovedx=(abs_nobranch(unlim_thrust)+cannon_velocity)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(unlim_thrust)+cannon_velocity)*ship_rotated_normal.y;
        }
        else
        {
	the_params.Omovedx=(abs_nobranch(thrust)+cannon_velocity)*ship_rotated_normal.x;
	the_params.Omovedy=(abs_nobranch(thrust)+cannon_velocity)*ship_rotated_normal.y;
        }


//	  the_params.Omovedx=(ABS(thrust)+720)*ship_rotated_normal.x;
//	  the_params.Omovedy=(ABS(thrust)+720)*ship_rotated_normal.y;
	the_params.Omovedz=0;
	the_params.delta_rot_x=10;
	the_params.rot_z=ship_z_ang;
	the_params.controller_ref=player_bullet_not_so_weeny;	//controller
	the_params.current_velocity=cannon_velocity;
	the_params.wanted_velocity=cannon_velocity;
	the_params.mass=80;	//beta 5 - mass was 50
	the_params.dq=GUN_DAMAGE3;
	the_params.object_category=zex_object;
	the_params.no_trivial_rejection=1;
     the_params.who_spawned_this_object=zex;
	load_dyn_object(player_bullet_not_so_weeny,slot,&the_params,-1);	//the static object, the position (-1=next free)
//	play_zsound_always(short_explosion,sound_high_pri, stereo1_chan, sound_vol_7);
	}

 }
 break;
}	//end of case 3


}	//end of switch

}

//еееееееееееееееееееееееееее
int update_fuel_timer;	//timer for when we recalc fuel - every "thrust" frames we dec 1
int damage_repair_timer;
void run_ship_misc_control(void)
{
//this routine responsible for misc ship systems and damage repair
extern int shields_recharge_counter;
extern int thrust,shields_warning_timer,zex_alive;
extern kit_desc ship_items[ITEMS_COUNT];

extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects

if (launch_timer>0) launch_timer--;	//stops various systems from running if >0
if (attack_message_timer>0) attack_message_timer--;	//stops various systems from running if >0

if (zex_alive==0) return;	//obvious - we dont run systems if we're deaded
//depelete fuel
//we do this by decrementing by 1 when ever our timer runs out. The timer is max_thrust-thrust
//so at max thrust we dec 1 unit every frame
update_fuel_timer--;
if (update_fuel_timer<0)
{

 if(gvel_limiter==1)
 {
   if (ship_items[KFUEL_MAXIMISER].status==1)
   update_fuel_timer=1500-abs_nobranch(unlim_thrust);	//reset timer
   else
   update_fuel_timer=1150-abs_nobranch(unlim_thrust);	 //reset timer
 }
 else
 {
   if (ship_items[KFUEL_MAXIMISER].status==1)
   update_fuel_timer=800-abs_nobranch(thrust);	    //reset timer
   else
   update_fuel_timer=460-abs_nobranch(thrust);	    //reset timer
 }
 
 if (gRadarRange==1) update_fuel_timer-=20;	//long range radar uses more power
 
 if (ship_items[KNORMAL_FUEL_TANK].value1>0) ship_items[KNORMAL_FUEL_TANK].value1--;
 
 if (ship_items[KNORMAL_FUEL_TANK].value1==19) add_to_text_display("OBC> Fuel low.",1);
 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==10) add_to_text_display("OBC> WARNING: Fuel very low.",1);
 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==5) add_to_text_display("OBC> Fuel dangerously low. will lose control shortly!",1);
 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==2) 
 {
 add_to_text_display("OBC> NO FUEL! Laser and missiles going off line",1);
 ship_items[KLASER].status=2;
 ship_items[KMISSILES].status=2;
 ship_items[KREPAIR_SYSTEMS].status=2;
 }

 else
 if (ship_items[KNORMAL_FUEL_TANK].value1==1) 
 {
 add_to_text_display("OBC> NO FUEL! Switching to batteries for essentials",1);
 add_to_text_display("OBC> Systems going off line!",1);
 add_to_text_display("OBC> Daisy, daisy, give me your answer do...",1);
 ship_items[KRADAR].status=2;
 ship_items[KLONG_RANGE_RADAR].status=2;
 (*ocb_ptr).object_list[0].Orotdy=1;	//fix for no dock if no fuel
 ship_items[KNORMAL_FUEL_TANK].value1=0; 
 }

 
}

//repair a damaged system
damage_repair_timer--;
if (damage_repair_timer<0  && ship_items[KNORMAL_FUEL_TANK].value1>1)
   {
	   int time=1000-(200*ship_items[KREPAIR_SYSTEMS].modifier);	//thus a level 3 repair system will repair in 1000-(200*3)=400 frames. Level 2=1000-(200*2)=600, level3 = 800 frames
	   damage_repair_timer=time;	//RangedRdm(200,600);

	//we repair in order of priority - 
	//repair hull if hull auto repair present
		if (ship_items[KAUTO_HULL_REPAIR].status==1)
		{
		extern int hull_strength, hull_damage;

			if (hull_strength<50)
			{
				hull_strength++;
				hull_damage = 50 - hull_strength;	//hull_damage is normally only calculated when we are actually damaged
				if (hull_damage >50) { hull_damage = 50;}
				
				ship_items[KNORMAL_FUEL_TANK].value1--;	//we'll take some fuel for that
				if (hull_strength==50)
				{
					add_to_text_display("HULL FULLY REPAIRED",0);
				}
			}
		}


	//REPAIR, missiles, laser,FLIGHT, cannon, radar ,long range
		if (ship_items[KREPAIR_SYSTEMS].status>1)	//Is repair systems present but damaged?
		{
			ship_items[KREPAIR_SYSTEMS].status=1;	//no, repair it
			if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
			add_to_text_display("REPAIR SYSTEMS BACK ON-LINE",0);
		}
		else
		if (ship_items[KREPAIR_SYSTEMS].status==1)
		{
		   if (ship_items[KMISSILES].status>1 && (*ocb_ptr).object_list[zex].number_of_missiles!=0 )	//no point in doing missiles if we have none! 
		   {
			   ship_items[KMISSILES].status=1;
			   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
			   add_to_text_display("MISSILE CONTROLLER BACK ON-LINE",0);

		   }
		   else
		   if (ship_items[KLASER].status>1)
		   {
			   ship_items[KLASER].status=1;
			   if (ship_items[KNORMAL_FUEL_TANK].value1>40) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
			   add_to_text_display("LASER BACK ON-LINE",0);
		   }
		   else
		   if (ship_items[KENGINE].status>1)	
		   {
			   ship_items[KENGINE].status=1;
			   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
			   add_to_text_display("ENGINE REPAIRED",0);

		   }

		   else
		   if (ship_items[KFLIGHT_COMPUTER].status>1 )
		   {
			   ship_items[KFLIGHT_COMPUTER].status=1;
			   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
			   add_to_text_display("ORCS BACK ON-LINE",0);
		   }
		   else
		   if (ship_items[KCANNON].status>1) 
		   {
			   ship_items[KCANNON].status=1;
			   if (ship_items[KNORMAL_FUEL_TANK].value1>40) ship_items[KNORMAL_FUEL_TANK].value1-=15;
			   add_to_text_display("CANNON BACK ON-LINE",0);
		   
		   }
		   else
		   if (ship_items[KRADAR].status>1) 
		   {
			   ship_items[KRADAR].status=1;
			   add_to_text_display("RADAR BACK ON-LINE",0);

		   }
		   else
		   if (ship_items[KLONG_RANGE_RADAR].status>1) 
		   {
			   ship_items[KLONG_RANGE_RADAR].status=1;
			   add_to_text_display("LONG RANGE RADAR BACK ON-LINE",0);
			}
			else	//finally check missiles again
		   if (ship_items[KMISSILES].status>1 ) 
		   {
			   ship_items[KMISSILES].status=1;
			   if (ship_items[KNORMAL_FUEL_TANK].value1>30) ship_items[KNORMAL_FUEL_TANK].value1-=15;   
			   add_to_text_display("MISSILES BACK ON-LINE",0);

		   }
		}
	//   instruments[RangedRdm(0,5)]=1;	//repair a system at random

   }	//damage repair timer
   
//recharge shields - fixed Ian 9/3/99
if (ship_items[KSHIELDS].value1<0) ship_items[KSHIELDS].value1=0;
if (ship_items[KSHIELDS].modifier<1) ship_items[KSHIELDS].value1=0;	//do we have shields? Saves loads of procedural code

if (shields_recharge_counter<0 &&  ship_items[KNORMAL_FUEL_TANK].value1>1 && ship_items[KSHIELDS].modifier>0) //and we actually have shields!
   {
    if (ship_items[KSHIELDS].value1+5<1000)
     ship_items[KSHIELDS].value1+=5;
     else
     ship_items[KSHIELDS].value1 = 1000;
     
     shields_recharge_counter=shields_recharge_reset;
   }
   else
     shields_recharge_counter--;
//shields are recharged at a rate depending on thrust - the more thrust the less energy
//to recharge the shields
//shield quality is 4 (bad) to 1(good)
   shields_recharge_reset=abs_nobranch(thrust)/5+3;	 ///(MAX_VELOCITY/40);	 //somewhere between 0 and 22
   shields_recharge_reset/=(ship_items[KSHIELDS].modifier+1);	//1 to 5 - 1=bad, 5=good
//   shields_recharge_reset=(MAX_VELOCITY+(shields_quality))-(MAX_VELOCITY-ABS(thrust));	//times 220-(220-thrust)
//  if (shields_recharge_reset>10) shields_recharge_reset=shields_recharge_reset/(6-shields_quality);
//playing warning voice
  if (ship_items[KSHIELDS].value1<150)
  {
   if (shields_warning_timer<=0 && zex_alive==1)
   {
      play_zsound(danger, sound_low_pri, speech_chan, sound_vol_7);
      shields_warning_timer=260;
   }
   shields_warning_timer--;
  } 

  else
  if (ship_items[KSHIELDS].value1<250)
  {
   if (shields_warning_timer<=0 && zex_alive==1)
   {
      play_zsound(warning, sound_low_pri, speech_chan, sound_vol_7);
      shields_warning_timer=270;
   }
   shields_warning_timer--;
  } 

}


//old code below
//void apply_control_inputs()
//{
//extern	DynObjectsFixedSize *ocb_ptr;
//extern _3D ship_normal;	//Start pointing out of screen
//extern _3D ship_rotated_normal; //from engine
//extern	int ship_y_ang,ship_x_ang,ship_z_ang;
//extern int thrust,zero_crossed;
//int yaw_input,pitch_input;
//float rotdx,rotdy,rotdz;
//
//
////handle ship flip rotation
// if (zero_crossed==2)
// {
// if ((*ocb_ptr).object_list[0].Oroty!=180)  (*ocb_ptr).object_list[0].Oroty+=10; //we need to rotate the ship 180 around z, and then 180 around x
// else zero_crossed-=1;	//set to 2 initially
//
// if ((*ocb_ptr).object_list[0].Oroty>180 || (*ocb_ptr).object_list[0].Oroty>170)
//    (*ocb_ptr).object_list[0].Oroty=180;	  //clamp it
//
// }
// 
// if (zero_crossed==1)
// {
// if ((*ocb_ptr).object_list[0].Orotx!=180)  (*ocb_ptr).object_list[0].Orotx+=10; //we need to rotate the ship 180 around z, and then 180 around x
// else zero_crossed-=1;	//will be zero by time maneouver is complete
//
// if ((*ocb_ptr).object_list[0].Orotx>180 || (*ocb_ptr).object_list[0].Orotx>170)
//    (*ocb_ptr).object_list[0].Orotx=180;	  //clamp it
// ship_z_ang=180;
// }
//
// if (zero_crossed==-2)
// {
// if ((*ocb_ptr).object_list[0].Oroty!=0)  (*ocb_ptr).object_list[0].Oroty+=10; //we need to rotate the ship 180 around z, and then 180 around x
// else zero_crossed+=1;	//set to 2 initially
//
// if ((*ocb_ptr).object_list[0].Oroty>350 || (*ocb_ptr).object_list[0].Oroty<10)
//    (*ocb_ptr).object_list[0].Oroty=0;	//clamp it
//
// }
// 
// if (zero_crossed==-1)
// {
// if ((*ocb_ptr).object_list[0].Orotx!=0)   (*ocb_ptr).object_list[0].Orotx-=10; //we need to rotate the ship 180 around z, and then 180 around x
// else zero_crossed+=1;	//will be zero by time maneouver is complete
//
// if ((*ocb_ptr).object_list[0].Orotx>350 || (*ocb_ptr).object_list[0].Orotx<10)
//    (*ocb_ptr).object_list[0].Orotx=0;	//clamp it
// ship_z_ang=0;
// }
//
////еееееееееееееееееееееееееееееееееееееееееееееее
//
//////handle explode ship - test for now
////   if (check_key(0xe)==1) 
////   {
////   //do_explode(zex);	//"E" to explode ship
////   add_to_explode_list(zex);
////   add_to_kill_list (zex);
////   }
//   
////handle roll
//
//if (ship_rotated_normal.x>0)	  //if facing right
//
//  {
//   if (check_key(0x7d)==1)
//	  {
//	    if (ship_z_ang>315 || ship_z_ang<45)	//up key
//	        ship_z_ang-=9;
//
//  }
//
//   if (check_key(0x7e)==1)	//down key
//	  {
//	    if (ship_z_ang<45 || ship_z_ang>315)
//       	ship_z_ang+=9;	//right turn Clyde
//	  }
//
//   if (ship_z_ang>359) ship_z_ang-=359;
//   if (ship_z_ang<0) ship_z_ang+=359;
//
////now auto center
////if between 180 and 359, add 1 to get to zero
////if between 1 and 179, sub 1 to get to zero
//   if (zero_crossed==0)
//   {  
////these calcs if facing to the right
//	  if (ship_z_ang>180)
//	   { //angle between 180 and 359
//      if ( ((*ocb_ptr).object_list[0].Oworldy)>SHIP_MIN_Y)
//      {
//	     ((*ocb_ptr).object_list[0].Oworldy)-=((360-ship_z_ang)*5);
//      }
//       if (ship_z_ang!=0) ship_z_ang++;	//add to get back to zero
//	   }
//	   
//	   
//	  if (ship_z_ang<180)  //angle between 179 and zero - going down
//	   {
//      if ( ((*ocb_ptr).object_list[0].Oworldy)<SHIP_MAX_Y)
//      {
//	     ((*ocb_ptr).object_list[0].Oworldy)+=(ship_z_ang)*5;
//      }
//	     if (ship_z_ang!=0) ship_z_ang--;
//     }	
//   ((*ocb_ptr).object_list[0].Orotx)=ship_z_ang;
//  
//   }	  //end of zero_crossed==0
//
//}	//end of facing right
//
////еееее
//else	  //ship facing left
//{
//
//   if (check_key(0x7d)==1)	//down key
//	  {
//	    if (ship_z_ang>135 && ship_z_ang<225)	//down key
//	        ship_z_ang-=9;
//
//  }
//
//   if (check_key(0x7e)==1)	//up key
//	  {
//	    if (ship_z_ang<225 && ship_z_ang>135)
//       	ship_z_ang+=9;	//right turn Clyde
//	  }
//
//   if (ship_z_ang>359) ship_z_ang-=359;
//   if (ship_z_ang<0) ship_z_ang+=359;
//
////now auto center
////if between 180 and 359, add 1 to get to zero
////if between 1 and 179, sub 1 to get to zero
//
//   if (zero_crossed==0)
//   {  
////these calcs if facing to the left
//	  if (ship_z_ang>180)
//	   { //angle between 180 and 225 = ship going up
//      if ( ((*ocb_ptr).object_list[0].Oworldy)<SHIP_MAX_Y)
//      {
//	     ((*ocb_ptr).object_list[0].Oworldy)+=((ship_z_ang-180)*5);
//      }
//       if (ship_z_ang!=180) ship_z_ang--;	//add to get back to zero
//	   }
//	   
//	   
//	   if (ship_z_ang<180) //angle between 180 and 135 - ship going down
//	   {
//      if ( ((*ocb_ptr).object_list[0].Oworldy)>-SHIP_MAX_Y)
//      {
//	     ((*ocb_ptr).object_list[0].Oworldy)-=(180-ship_z_ang)*5;
//      }
//	     if (ship_z_ang!=180) ship_z_ang++;
//     }	
//       ((*ocb_ptr).object_list[0].Orotx)=ship_z_ang;
//  
//   }	  //end of zero_crossed==0
//}




//ееееееееееееееееееееееееееееееееееееееееееееееееееееееее
//handle thrust
//if (check_key(0x7c)==1)	//right arrow
//{
//if (thrust<220) thrust+=5;
//if (thrust>=0 && thrust-5<0)  
// {
// zero_crossed=-2;	//left to right
// }
//}
//
//if (check_key(0x7b)==1)	//left arrow
//{
// if (thrust>-220) thrust-=5;
// if (thrust<0 && thrust+5>=0)
// {  zero_crossed=2;	//right to left
// }
//}
//
//(*ocb_ptr).object_list[0].Oworldx+=thrust;
//(*ocb_ptr).object_list[0].thrust_x=thrust;
//
//
////еееееееееееееееееееееееееееееееееееееееееееееееееееее
////handle fire
//if (check_key(0x3)==1)	//F key
//do_player_fire();
//
//}
//
//void do_player_fire()
//{
//int slot;
//float shot_x,shot_y,shot_z;
//_3D player_normal;
//extern _3D ship_rotated_normal; //from engine
//extern	DynObjectsFixedSize *ocb_ptr;	//the dynamic objects
//extern	int last_bullet_fired_frame;
//extern int real_frame_counter;
//
//load_dyn_object_pb the_params;
//
//
////weeny missile
////if we're within 8 frames of last shot, then we can't fire
//
//if (real_frame_counter-10>last_bullet_fired_frame)
// { 
//  last_bullet_fired_frame=real_frame_counter;
//  slot=find_vacant_dynamic_object_slot();
//  if (slot!=-1)
//	  {
//	  shot_x=(*ocb_ptr).object_list[0].Oworldx;
//	  shot_x=shot_x+(300*ship_rotated_normal.x);
//	  shot_y=(*ocb_ptr).object_list[0].Oworldy;
//	  shot_z=(*ocb_ptr).object_list[0].Oworldz;
////	player_normal=ship_rotated_normal;
//	  	  
//	  the_params.world_x=shot_x;
//	  the_params.world_y=shot_y;
//	  the_params.world_z=shot_z;
//	  the_params.Omovedx=200*ship_rotated_normal.x+thrust;
//	  the_params.Omovedy=0;
//	  the_params.Omovedz=0;
//	  the_params.delta_rot_x=10;
//	  the_params.delta_rot_y=0;
//	  the_params.delta_rot_z=0;
//	  the_params.rot_x=0;
//	  
//	  if (ship_rotated_normal.x>0)
//	     the_params.rot_y=0;
//	  else
//	     the_params.rot_y=180;	  //Get missile facing right way
//	     
//	  the_params.normal.x =  1;	//objects are ALWAYS defined facing right
//	  the_params.normal.y =  0;	//objects are ALWAYS defined facing right
//	  the_params.normal.z =  0;	//objects are ALWAYS defined facing right
//	  the_params.rot_z=0;
//	  the_params.controller_ref=player_bullet_weeny;	//controller
//	  the_params.control_use_1=0;	//frames to live counter
//	  the_params.control_use_2=ship_rotated_normal.x;	//use to control deceleration.
//	  the_params.control_use_3=0;
//	  the_params.control_use_4=0;
//	  the_params.current_velocity=0;
//	  the_params.wanted_velocity=0;
//	  the_params.shield_value=0;
//	  the_params.mass=3;
//	  load_dyn_object(1,slot,&the_params);	  //the static object, the position (-1=next free)
//	  }
// }
//}
//

void ship_init()
{
extern int ship_y_ang,ship_x_ang,ship_z_ang,thrust,zex_alive,retro;
extern int shields_recharge_counter,shields_warning_timer;
extern float turn;
//extern int instruments[6];
extern int damage_repair_timer,tractor,done_load_flag;

int i;

damage_repair_timer=300;
ship_y_ang=0; ship_x_ang=0; ship_z_ang=0; unlim_thrust=thrust=0;
directional_control=1;

zero_crossed=0;
zex_alive=1;	//0 if not
tractor=0;
//shields=1000;
if (done_load_flag==0)
{
ship_items[KSHIELDS].status=1;
ship_items[KSHIELDS].value1=1000;
ship_items[KSHIELDS].modifier=1;
ship_items[KSHIELDS].obj_class=EQUIPMENT;
ship_items[KNORMAL_FUEL_TANK].value1=180;
     for (i=0;i<6;i++)
     ship_items[i].status=1;	//clear instrument lights
}
retro=0;	//special controller -- retro rockets
turn=0;
shields_recharge_reset=40;
shields_recharge_counter=shields_recharge_reset;	//recharge 1 unit every 10 frames initially
//shields_quality=1;	//initial shields - 4 is best
shields_warning_timer=0;
update_fuel_timer=0;
//additional items would be reset here

//init hold items
for (i=0;i<MAX_HOLD_ITEMS;i++)
 {
 hold_items[i].id=-1;
 }

}


//is a key of char code pressed?
//oldmac//
/*short check_key(unsigned short k)
//k=any keyboard code 0-127 (Code from Think Ref)
{
unsigned char km[16];
GetKeys ((unsigned long *)km);
return ((km[k>>3]>>(k&7))&1);
}*/



//oldmac//  (note: 0x31 is the space key
/*short fire_button()
//k=any keyboard code 0-127 (Code from Think Ref)
{
unsigned char km[16];
GetKeys ((unsigned long *)km);
return ((km[0x31>>3]>>(0x31&7))&1);
}*/


extern int camera_rotation_delta_z,zex_alive;
extern int flash_screen,planet_on_screen;


void kill_zex()
{
//extern int instruments[6];
extern int tractor,lives,showing_stats,credits;
extern int camera_spin_on_zex_death;
int i;



if (global_show_status!=0)
 reset_screen_from_status();	//we were showin the status screen
if (gShowGalacticMap!=0)
 reset_screen_from_status();	//we were showin the status screen

//spill out any cargo
for (i=0;i<MAX_HOLD_ITEMS;i++)
 {
 if (hold_items[i].id!=-1) gen_specific_cargo(0,hold_items[i].id);	//spill out the (correct!) cargo
 
 hold_items[i].id=-1;
 }
 
 
if ((*ocb_ptr).object_list[zex].in_use==0) return;	//Zex IS dead!

//we have a flatpack
     if (ship_items[KFLATPACK].value1>0 )
     {
		 ship_items[KFLATPACK].value1--;
		 if (ship_items[KFLATPACK].value1==0)
		 {
			ship_items[KFLATPACK].status=0;	//all gone
		 }
		  do_large_explode(zex);     
		 (*ocb_ptr).object_list[zex].eng_invisible=1;	//lose zex from screen
		(*ocb_ptr).object_list[0].extern_movedx=0;
		(*ocb_ptr).object_list[0].extern_movedy=0;
		 play_zsound(explosion4, sound_high_pri, stereo1_chan, sound_vol_7);
		 (*ocb_ptr).object_list[zex].hit_by_slot=-1;
		 add_to_text_display("SHIP DESTROYED! USING NEW FLATPACK",1);	//clear down any messages
		 reset_ship_with_carry_over();
		 hide_zex=50;	//dec'd in apply_control_inputs = makes zex invisible
		 camera_spin_on_zex_death=45;
		 showing_stats=1;	//stop everything firing
		 unlim_thrust=thrust=0;
		directional_control=1;
     }
     else
     
//no flatpacks left, see if we can eject
     if (ship_items[KEJECTION].status==1 && have_ejected==0)
      {
       int closest_mship;
  
       closest_mship=find_nearest_mothership_to_object(0,-1);
      //SB 040699
       if (closest_mship==-1)	 //no motherships in area!
       {
       add_to_text_display("CAN'T AUTO EJECT! NO MOTHERSHIPS IN VICINITY.",1);
         dz1();	//kill zex
       return; 
       }


       add_to_text_display("EJECTION POD AUTO ACTIVATED",1);
       do_ejection();
       } 
     
     
       else	//cant eject
     
     {
//       if (have_ejected==0)
//        {
         dz1();	//kill zex
       }
     
}

//private routine
void dz1(void)
{
int i;
         for (i=0;i<6;i++)
         ship_items[i].status=0;	//clear instrument lights
         if ((*ocb_ptr).object_list[zex].Oid=='EC01' || (*ocb_ptr).object_list[zex].Oid=='EC02')
	     add_to_text_display("EJECTION POD DESTROYED! GAME OVER.",1);
         set_light_amplitude(65530);	//32767 is max
		 fade_and_play_zex_resource(132);	 //Spring by Purple Motion

         camera_rotation_delta_z=3;
         zex_alive=0;
         tractor=0;
         add_to_explode_list(zex,4);
         add_to_kill_list (zex);
     play_zsound(explosion4, sound_high_pri, c1_chan1, sound_vol_7);
//         play_zsound(short_explosion, sound_high_pri, twin1_chan, sound_vol_7);
         (*ocb_ptr).object_list[zex].hit_by_slot=-1;
         flash_screen=2;

//	     add_to_text_display(" ",0);	//clear down any messages
//	     add_to_text_display(" ",0);	//clear down any messages
}




//End the game RIGHT NOW - typically clled from a level controllere
//EOGCode is a termination code, where zero means OK and -1 means show the reason in the string 
//pointed to by param2
void end_game_right_here_right_now(int EOLCode, const char * the_reason_string)
{
extern int end_of_level_code;
extern char end_of_level_reason_string[];	//we can print this if eolc is not zero

//Set up termination reasons to show player why it's all over
 end_of_level_code=EOLCode;
 strcpy (&end_of_level_reason_string[0],the_reason_string);

//And STOP
 ship_items[KFLATPACK].value1=0;
 ship_items[KEJECTION].status=0;
 kill_zex();
}


//twiddles with the vertices of the exhaust cones of Zex depending on current thrust
//which vertices?
#define EXHAUST_TIP_1 38
#define EXHAUST_TIP_2 47

#define EXHAUST_TIP_1_Y 96
#define EXHAUST_TIP_2_Y 102


#define EXHAUST_MAX_LENGTH 900
#define EXHAUST_MAX_LENGTH_CRUISE 1500

static int exhaust_length=-500;
static int exhaust_delta=+5;
static int exhaust_y_wobble;
static int old_thrust_up,old_thrust_down;

void run_exhaust_animation()
{
register ZObject * current_object_ptr;
_3D *zex_points_ptr;
int test_thrust;


//zex is ALWAYS in slot 0 of the dynamic OCB's
zex_points_ptr=ocb_ptr->object_list[0].points_ptr;
//zex_points_ptr+=EXHAUST_TIP_1;
zex_points_ptr[EXHAUST_TIP_1].x=exhaust_length;
zex_points_ptr[EXHAUST_TIP_2].x=exhaust_length;

//zex_points_ptr[EXHAUST_TIP_2].y+=exhaust_y_wobble;

//zex_points_ptr[EXHAUST_TIP_2].y+=exhaust_y_wobble;

//if (zex_points_ptr[EXHAUST_TIP_1].y>140) zex_points_ptr[EXHAUST_TIP_1].y=140;
//if (zex_points_ptr[EXHAUST_TIP_1].y<80) zex_points_ptr[EXHAUST_TIP_1].y=80;
//
exhaust_length+=exhaust_delta;

if (gvel_limiter==1)
{	//cruise thrust
test_thrust=unlim_thrust;
if (exhaust_length<-EXHAUST_MAX_LENGTH_CRUISE) exhaust_length=-EXHAUST_MAX_LENGTH_CRUISE;	//max size
}
else	//combat thrust
{
test_thrust=thrust;
if (exhaust_length<-EXHAUST_MAX_LENGTH) exhaust_length=-EXHAUST_MAX_LENGTH;	//max size
}




if (gThrustDown && ship_items[KNORMAL_FUEL_TANK].value1>1  && test_thrust>-100)	//reverse thrust
{
if (exhaust_length>400) exhaust_length=400;	//exhaust off
exhaust_y_wobble=RangedRdm(EXHAUST_TIP_1_Y-30,EXHAUST_TIP_1_Y+30);

zex_points_ptr[EXHAUST_TIP_1].y=exhaust_y_wobble;

exhaust_y_wobble=RangedRdm(EXHAUST_TIP_2_Y-30,EXHAUST_TIP_2_Y+30);

zex_points_ptr[EXHAUST_TIP_2].y=-exhaust_y_wobble;

}
else	//not reverse thrust
{
if (exhaust_length>-366) exhaust_length=-366;	//exhaust off
else
{
exhaust_y_wobble=RangedRdm(EXHAUST_TIP_1_Y-30,EXHAUST_TIP_1_Y+30);

zex_points_ptr[EXHAUST_TIP_1].y=exhaust_y_wobble;

exhaust_y_wobble=RangedRdm(EXHAUST_TIP_2_Y-30,EXHAUST_TIP_2_Y+30);

zex_points_ptr[EXHAUST_TIP_2].y=-exhaust_y_wobble;

}
}

//calc required exhaust length
exhaust_delta=+50;	//decay

if (gvel_limiter==1)
{

if (gThrustUp==1 && ship_items[KNORMAL_FUEL_TANK].value1>1  && unlim_thrust<1020)
    {
    exhaust_delta=-30;	//apply thrust
    if (old_thrust_up==0)
     play_zsound(thrust_sound, sound_high_pri, c1_chan2, sound_vol_7);
    old_thrust_up=1;
    }
else
old_thrust_up=0;

//play reverse thrust sound
if (gThrustDown==1 && ship_items[KNORMAL_FUEL_TANK].value1>1  && unlim_thrust>-100)
    {
        exhaust_delta=+100;	//apply -ve thrust
    if (old_thrust_down==0)
     play_zsound(thrust_reverse_sound, sound_high_pri, c1_chan3, sound_vol_7);
    old_thrust_down=1;
    }
else
{
old_thrust_down=0;

}

}

else	//normal thrust

{

if (gThrustUp==1 && ship_items[KNORMAL_FUEL_TANK].value1>1  && thrust<360)
    {
    exhaust_delta=-30;	//apply thrust
    if (old_thrust_up==0)
     play_zsound(thrust_sound, sound_high_pri, c1_chan2, sound_vol_7);
    old_thrust_up=1;
    }
else
{
//play reverse thrust sound
if (gThrustDown==1 && ship_items[KNORMAL_FUEL_TANK].value1>1  && thrust>-100)
    {
        exhaust_delta=+100;	//apply -ve thrust

    if (old_thrust_down==0)
     play_zsound(thrust_reverse_sound, sound_high_pri, c1_chan3, sound_vol_7);
    old_thrust_down=1;
    }
else
{
old_thrust_down=0;

}

old_thrust_up=0;
if (gThrustDown==1)
 {
 exhaust_delta=+50;	//decay
 }
else
exhaust_delta=0;	//decay

}

}



}


#define SHOCKWAVE_BASIC_DAMAGE 6000	//this is multiplied by the modifier, 1 or 2
#define SHOCKWAVE_BASIC_RANGE 30000
const int SHOCKWAVE_DAMAGE_HAPPENS_RANGE=15000;

void do_shockwave(int modifier)
{
//extern int flash_screen;
	//seriously damage all enemy ships within a given range. Of course, every great thing has a downside
	//and in the case of a Shockwave it's that you get no bounty. Generally saves yer skin though.
	//this only works for Zex, NPC's can't have shockwaves

	play_zsound(short_explosion_with_echo, sound_high_pri, c1_chan1, sound_vol_7);
	//flash_screen=2;
	
	gen_shockwave_object(SHOCKWAVE_BASIC_RANGE*modifier, SHOCKWAVE_BASIC_DAMAGE*modifier, SHOCKWAVE_DAMAGE_HAPPENS_RANGE);	
}

void damage_object_due_to_shockwave(int distance, int damage)
{
	//seriously damage all enemy ships within a given range. Of course, every great thing has a downside
	//and in the case of a Shockwave it's that you get no bounty. Generally saves yer skin though.
	//this only works for Zex, NPC's can't have shockwaves
	int active_object_counter=0;	
	int object_counter=0;
	ZObject * current_object_ptr;
	
	extern int dynamic_object_index;
	while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
	{
		current_object_ptr=&ocb_ptr->object_list[object_counter];
		
		if (current_object_ptr->in_use==1)
		{
			if (current_object_ptr->is_visible==1)
			{
				if (current_object_ptr->object_category==baddie_object)
				{
					//if within distance
					//20000 for level 1, 40000 for level2
					float distance_to=get_distance_to_from(object_counter,0);
					if (distance_to < distance)
					{
						damage_object(object_counter, damage);	//does all the right things including killing and animating
					}
				}
			}
			active_object_counter+=1;
		}
		object_counter++;	//make sure we stay within the bounds of the OCB's
	}
}

void do_warp_jump(int modifier)
{
	//jump to a random location in this quadrant relative to zex
	//this only works for Zex, NPC's can't have warp jumps
	int x=RangedRdm(0,65000)-32000;
	int y=RangedRdm(0,65000)-32000;
	if (x<0) x-=30000;	//make sure we at least move somewhere
	if (x>0) x+=30000;
	if (y<0) y-=30000;
	if (y>0) y+=30000;
	x*=16*modifier;
	y*=16*modifier;
	ocb_ptr->object_list[0].Oworldx+=x;
	ocb_ptr->object_list[0].Oworldy+=y;
}

#define ECM_BASIC_RANGE 2000	//quite short range to make it 'interesting'
#define ECM_BASIC_DAMAGE 4000
//This only works for Zex at the mo
void handle_ecm(unsigned int for_whom)
{
	int active_object_counter=0;	
	int object_counter=0;
	ZObject * current_object_ptr;
	if (for_whom>MAX_NUMBER_OF_DYN_OBJECTS) return;	//unknown object
	UInt8 our_allegiance=ocb_ptr->object_list[for_whom].object_category;
	int watch_allegiance=0;
	if (our_allegiance==baddie_object) watch_allegiance=zex_object;
	else
	if (our_allegiance==zex_object) watch_allegiance=baddie_object;

	extern int dynamic_object_index;
	while (active_object_counter<dynamic_object_index  && object_counter<MAX_NUMBER_OF_DYN_OBJECTS)
	{
		current_object_ptr=&ocb_ptr->object_list[object_counter];

		if (current_object_ptr->in_use==1)
		{
			if (current_object_ptr->is_visible==1)
			{
				if (current_object_ptr->object_category==watch_allegiance)
				{
					//is it a missile?
					UInt32 id=current_object_ptr->Oid;
					id>>=16;
					id|=0x20200000;
					if (id=='  ZZ')	//Missiles are ZZ1A,B,C, ZZ2A,B,C etc (animated objects)
					{
						float distance_to=get_distance_to_from(object_counter,0);
						if (distance_to<ECM_BASIC_RANGE)
						{
							add_to_text_display("OBC> E.C.M. Activated!",1);

							play_zsound(refuel_beep, sound_high_pri, c1_chan1, sound_vol_7);
							flash_screen=2;
							damage_object(object_counter, ECM_BASIC_DAMAGE);	//does all the right things including killing and animating
						}
						
					}

				}
			}
			active_object_counter+=1;
		}
	object_counter++;	//make sure we stay within the bounds of the OCB's
	}
}


#define MF_FUEL_USAGE 2
static int missile_factory_timer=0;
static int MF_processing;
void run_missile_factory(void)	//only provides level 1 missiles, uses fuel
{
	missile_factory_timer++;

	if (missile_factory_timer==120)	//check if we need to make a missile
	{
		if (ship_items[KMISSILE_FACTORY].status==1 && ship_items[KNORMAL_FUEL_TANK].value1>MF_FUEL_USAGE)
		{
			MF_processing=0;

			if ( ship_items[KMISSILE_PYLON1].modifier==0)	//by not checking the pylon status here we allow
			{												//the player to sell MF generated missiles
				MF_processing=1;							//so he can dock, flog 'em, relaunch, make 3 missiles, dock, flog em etc.
				add_to_text_display("OBC> Missile Factory active, pylon 1",1);	
			}
			else
			if ( ship_items[KMISSILE_PYLON2].modifier==0)
			{
			add_to_text_display("OBC> Missile Factory active, pylon 2",1);
				MF_processing=2;
			}
			else
			if ( ship_items[KMISSILE_PYLON3].modifier==0)
			{
			add_to_text_display("OBC> Missile Factory active, pylon 3",1);
				MF_processing=3;

			}
			if (MF_processing==0) missile_factory_timer=0;	//reset
		}
	}

	if (missile_factory_timer>=(MAX_FRAME_RATE*30))	//at least 30 seconds to generate a missile
	{
													
		if ( MF_processing==1 && ship_items[KNORMAL_FUEL_TANK].value1>MF_FUEL_USAGE)	//only produce missile if enough fuel
		{
			add_to_text_display("OBC> Missile Factory complete, pylon 1 ready",1);
			play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
			ship_items[KNORMAL_FUEL_TANK].value1-=MF_FUEL_USAGE;
			ship_items[KMISSILE_PYLON1].status=1;
			ship_items[KMISSILE_PYLON1].obj_class=WEAPONS;
			ship_items[KMISSILE_PYLON1].modifier=1;
		}

		if ( MF_processing==2 && ship_items[KNORMAL_FUEL_TANK].value1>MF_FUEL_USAGE)
		{
			add_to_text_display("OBC> Missile Factory complete, pylon 2 ready",1);
			play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
			ship_items[KNORMAL_FUEL_TANK].value1-=MF_FUEL_USAGE;
			ship_items[KMISSILE_PYLON2].status=1;
			ship_items[KMISSILE_PYLON2].obj_class=WEAPONS;
			ship_items[KMISSILE_PYLON2].modifier=1;
		}
		if ( MF_processing==3 && ship_items[KNORMAL_FUEL_TANK].value1>MF_FUEL_USAGE)
		{
			add_to_text_display("OBC> Missile Factory complete, pylon 3 ready",1);
			play_zsound(click, sound_high_pri, c1_chan1, sound_vol_7);
			ship_items[KNORMAL_FUEL_TANK].value1-=MF_FUEL_USAGE;
			ship_items[KMISSILE_PYLON3].status=1;
			ship_items[KMISSILE_PYLON3].obj_class=WEAPONS;
			ship_items[KMISSILE_PYLON3].modifier=1;
		}
	missile_factory_timer=0;	//reset back to start of sequence

	}
}



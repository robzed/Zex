// *************************************MODULE HEADER*********************************
// * FILENAME           : light_control.c
// * MODULE TITLE       : Lighting
// * PROJECT            : Zex
// * DATE STARTED       : 22:55:43 on 22nd June 1999
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

#include	"3d_structures_def.h"
#include "light_control.h"


// ***********************************************************************************
// * CONSTANTS 
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *
extern _3D light_normal;	//the light vecterre
float light_delta_z=+.001;
int day_night_timer=0;
int light_red, light_green,light_blue;	//0-32767
int light_amplitude;	//0-32767
int reset_brightness=25000;
// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


/*
Zex lighting. 220699 SB
A simple omni directional lighting environment.
The light is an object in 3d space moving along Z to simulate a sun
The light is given a brightness between 0 and 32767 with set_light_amp
The light is given a colour with set_light_colour
More than that we need not know :)
*/


//set up a maximum brightness white light
void init_light()
{
light_normal.x=1;
light_normal.y=1;
light_normal.z=.5;
day_night_timer=1000;	//initial delay before sun moves
light_red=32767;
light_green=32767;
light_blue=32767;
light_amplitude=reset_brightness;
}

void set_light_amplitude(int amplitude)
{
light_amplitude=amplitude;
}

int get_light_amplitude()
{
return light_amplitude;
}

void set_light_reset_amplitude(int amplitude)
{
reset_brightness=amplitude;
}




void do_light_control()
{
if (day_night_timer!=0) day_night_timer--;	//if timer is running dec it

else	//timer not running so alter light

{
 light_normal.z+=light_delta_z;	//adjust lighting
 if (light_normal.z<.1)	//if max z
 {
  light_delta_z=-light_delta_z;	//max bightness
  day_night_timer=4000;	//set timer for day
 }
 else
 if (light_normal.z>1.0)  //min z (+z is into screen)
 {
  light_delta_z=-light_delta_z;	//min bright
  day_night_timer=600;	//set timer - shorter night
 }
}

}





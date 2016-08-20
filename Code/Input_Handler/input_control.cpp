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

//input_control.c
//SB 22/12/98

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
#include "key_setting.h"

//oldmac//#include <InputSprocket.h>
#include "input_control.h"
#include "Error_handler.h"
#include	"splat_control.h"
#define numInputs 26
//0=not isping, 1=we are
UInt32 gISPState=0;

//Our controller positions
UInt32 gThrustUp;
UInt32 gThrustDown;
UInt32 gTurn;
UInt32 gCannon;
UInt32 gMissile;
UInt32 gLaser;
UInt32 gRetro;
UInt32 gWarpDrive;
UInt32 gShockwave;
UInt32 gEject;	//as it says
UInt32 gAbort;
UInt32 gMusic_up;
UInt32 gMusic_down;
UInt32 gSound_up;
UInt32 gSound_down;
UInt32 gRadarRangeInput;
UInt32 gCamera_zoom_in;
UInt32 gCamera_zoom_out;
UInt32 gMissileToggle;	//select a missile pylon
UInt32 gShowInventory;	//show the inventory screen
UInt32 gAskForDock;	//ask mothership for dock
UInt32 gCameraMode;	//camera
UInt32 gCPMode;	//Display control panel
UInt32 gvel_limiter_input;	//velocity limiter
UInt32 gCommandControl;	//Auto Pilot
UInt32 gGM;	//Galactic Map
UInt32 gResSwitch;		// ResSwitch key

//oldmac//ISpElementReference	gInputElements[numInputs];


typedef struct { 
	int enumerator;
	SDLKey key;
	SDLKey default_key;
	UInt32 *control_variable;
	UInt32 not_pressed;
	UInt32 pressed;
} def_key_type;

static UInt32 left_turn;
static UInt32 right_turn;

/*
The default keyboard layout:
	Forward Thrust 				> (Full Stop)
	Reverse Thrust 				< (Comma)
	Turn Left 					Left arrow key
	Turn Right					Right arrow key
	Fire Cannon				Space
	Fire Laser					L
	Fire Missile				M
	Toggle Combat/Cruise thrust	V
	Retro Thrust				T (When fitted)
	Select Missile Pylon			P
	Radar Range Select			R
	Request Dock				D
	Manual Eject				E

	Show Ship Status Screens		/
	Pause/Resume Game			ESC
	Increase Music Volume			]
	Decrease Music Volume		[
	Increase Sound Volume			=
	Decrease Sound Volume		-
	Camera Zoom In				Z
	Camera Zoom Out				X
	Change Camera Mode			C

	Toggle Instrument Panel		I
	Display Galactic Map          		G
	Command and Control			K
*/
// Note: should be kept in sync with enums in input_control.h
def_key_type def_key [] =
{
	{ turn_left_key,           SDLK_0, SDLK_LEFT,		&left_turn, 0, 1 },
	{ turn_right_key,          SDLK_0, SDLK_RIGHT,		&right_turn, 0, 1 },
	{ thrust_up_key,           SDLK_0, SDLK_UP,			&gThrustUp, 0, 1 },				// comment that says 0 to 0xffffffff is lying...
	{ thrust_down_key,         SDLK_0, SDLK_DOWN,		&gThrustDown, 0, 1 },
	{ cannon_key,              SDLK_0, SDLK_SPACE,		&gCannon, 0, 1 },
	{ laser_key,               SDLK_0, SDLK_l,			&gLaser, 0, 1 },
	{ missile_key,             SDLK_0, SDLK_m,			&gMissile, 0, 1 },
	{ retro_key,               SDLK_0, SDLK_t,			&gRetro, 0, 1 },
	{ warp_drive_key,          SDLK_0, SDLK_w,		    &gWarpDrive,  0, 1 },
	{ shockwave_key,           SDLK_0, SDLK_TAB,		&gShockwave,  0, 1 },
	{ abort_key,               SDLK_0, SDLK_ESCAPE,		&gAbort,  0, 1 },
	{ music_up_key,            SDLK_0, SDLK_RIGHTBRACKET,	&gMusic_up,  0, 1 },
	{ music_down_key,          SDLK_0, SDLK_LEFTBRACKET,  &gMusic_down,  0, 1 },
	{ sound_up_key,            SDLK_0, SDLK_EQUALS,		&gSound_up,  0, 1 },
	{ sound_down_key,          SDLK_0, SDLK_MINUS,		&gSound_down,  0, 1 },
	{ radar_range_key,         SDLK_0, SDLK_r,			&gRadarRangeInput,  0, 1 },
	{ camera_zoom_in_key,      SDLK_0, SDLK_z,			&gCamera_zoom_in,  0, 1 },
	{ camera_zoom_out_key,     SDLK_0, SDLK_x,			&gCamera_zoom_out,  0, 1 },
	{ missle_toggle_key,       SDLK_0, SDLK_p,			&gMissileToggle, 0, 1 },
	{ show_inventory_key,      SDLK_0, SDLK_SLASH,		&gShowInventory, 0, 1 },
	{ eject_key,               SDLK_0, SDLK_e,			&gEject, 0, 1 },
	{ dock_request_key,        SDLK_0, SDLK_d,			&gAskForDock, 0, 1 },
	{ camera_mode_key,         SDLK_0, SDLK_c,			&gCameraMode, 0, 1 },
	{ control_panel_key,       SDLK_0, SDLK_i,			&gCPMode, 0, 1 },  // display control panel
	{ velocity_limter_key,     SDLK_0, SDLK_v,			&gvel_limiter_input, 0, 1 }, // velocity limiter
	{ command_and_control_key, SDLK_0, SDLK_k,			&gCommandControl, 0, 1 },  // commandeering box
	{ galaxy_map_key,          SDLK_0, SDLK_g,			&gGM, 0, 1 },  // galaxy map
	{ res_switch_key,          SDLK_0, SDLK_F1,			&gResSwitch, 0, 1 }
};


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 13 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void set_internal_default_keys()
{
	for(int i=0; i<NUMBER_OF_KEYS; i++)
	{
		def_key[i].key = def_key[i].default_key;
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

SDLKey get_programmed_keycode(int key_number)
{
	if(key_number >= NUMBER_OF_KEYS)
	{
		report_error_c("get_programmed_keycode - number exceeded","",0);
	}
	return def_key[key_number].key;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void load_single_key_def(int key_number, SDLKey key_code)
{
	if(key_number >= NUMBER_OF_KEYS)
	{
		report_error_c("load_single_key_def - number exceeded","",0);
	}
	
	def_key[key_number].key = key_code;
}


/*
 1. sort out when "key res switch allowed ()" is to be called
 2. write a little program to write out callsigns
 3. change res switch allow flag default to false
 */
#if CALLSIGN_DEBUG_MODE
#include "object_control_top.h"
#endif

#include "main.h"
#include "timing.h"

static bool key_res_switch_allowed_flag = false;		// don't allow to start with

void res_switch_key_pressed()
{
	static bool video_just_switched = false;
	static Uint32 ticks;

	if(video_just_switched)
	{
		// Wait a small time if video just change before switching again
		// so we don't get in a strange (theoretical) multiple switch
		// state.
		//
		// Notice: the res change time itself doesn't count in this delay
		//
		if(ticks+700 < LS_GetTicks())
		{
			video_just_switched = false;
		}
	}
	
	// check again (flag might have cleared) and allow if video hasn't just switched
	if(!video_just_switched)
	{
		if(key_res_switch_allowed_flag)
		{
#if !CALLSIGN_DEBUG_MODE
			extern int Change_res;
			Change_res=Change_res^1;
			toggle_video();
#else
			write_callsigns();
#endif
			video_just_switched = true;
			ticks = LS_GetTicks();
		}
	}
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 17 Jan 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void key_res_switch_allowed(bool allowed)
{
	key_res_switch_allowed_flag = allowed;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

const int MAX_NUMBER_OF_STORED_KEYS = 400;
bool key_pressed_store[MAX_NUMBER_OF_STORED_KEYS];		// could use bit array, but we have loads of memory...

void handle_key_change(SDLKey key, bool pressed)
{
	// search the key array. Not the fastest way - but certainly the shortest code lines...
	for(int i=0; i < NUMBER_OF_KEYS; i++)
	{
		if(key == def_key[i].key)
		{
			if(pressed)
			{
				*(def_key[i].control_variable) = def_key[i].pressed;
				if(i==res_switch_key) res_switch_key_pressed();
			}
			else
			{
				*(def_key[i].control_variable) = def_key[i].not_pressed;
			}
			break;
		}
	}
	
	// store keys in case anyone whats to check if one if down
	if(key >= 0 && key <= MAX_NUMBER_OF_STORED_KEYS)
	{
		key_pressed_store[key] = pressed;
	}
	
	if(right_turn && !left_turn)
	{
		gTurn = 0xffffffff;
	}
	else if (left_turn && !right_turn)
	{
		gTurn = 0;
	}
	else
	{
		gTurn = 0x7fffffff;
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s):              | DATE STARTED: 
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
//routine samples the inputs and sets the relevant gVars.
//called every frame from main game loop
void poll_input_device()
{
/*
#define thrustUp 0
#define thrustDown 1
#define turn 2
#define cannon 3
#define laser 4
#define missile 5
#define retro 6
#define missile_toggle 7
#define radar_range 8
#define AskForDock 9
#define do_eject 10
#define vel_limiter 11
#define COMMAND 12
#define GM 13
#define show_inventory 14
#define abort 15
#define music_up 16
#define music_down 17
#define sound_up 18
#define sound_down 19
#define camera_zoom_in 20
#define camera_zoom_out 21
#define CameraMode 22
#define CPMode 23
*/
	process_standard_event_functions();
	
	
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
ISpElement_GetSimpleState (gInputElements[turn], &gTurn);	//0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[thrustUp], &gThrustUp); //0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[thrustDown], &gThrustDown); //0 to 0xffffffff
ISpElement_GetSimpleState (gInputElements[cannon], &gCannon); //0 or 1
ISpElement_GetSimpleState (gInputElements[laser], &gLaser); //0 or 1
ISpElement_GetSimpleState (gInputElements[missile], &gMissile); //0 or 1
ISpElement_GetSimpleState (gInputElements[retro], &gRetro);  //0 or 1
ISpElement_GetSimpleState (gInputElements[abort], &gAbort);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[music_up], &gMusic_up);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[music_down], &gMusic_down);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[sound_up], &gSound_up);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[sound_down], &gSound_down);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[radar_range], &gRadarRangeInput);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[camera_zoom_in], &gCamera_zoom_in);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[camera_zoom_out], &gCamera_zoom_out);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[missile_toggle], &gMissileToggle);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[show_inventory], &gShowInventory);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[do_eject], &gEject);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[AskForDock], &gAskForDock);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[CameraMode], &gCameraMode);  // 0 or 1
ISpElement_GetSimpleState (gInputElements[CPMode], &gCPMode);  // display control panel
ISpElement_GetSimpleState (gInputElements[vel_limiter], &gvel_limiter_input);  // velocity limiter
ISpElement_GetSimpleState (gInputElements[COMMAND], &gCommandControl);  // commandeering box
ISpElement_GetSimpleState (gInputElements[GM], &gGM);  // velocity limiter
#endif
}

void init_input_device()
{
	set_internal_default_keys();
	load_saved_keys();

	InitNeeds();
	init_ip_globs();
}



void init_ip_globs()
{
gThrustUp=0;
gThrustDown=0;
gTurn=0x7fffffff;
gCannon=0;
gMissile=0;
gLaser=0;
gRetro=0;
gWarpDrive=0;
gShockwave=0;
gAbort=0;
gMusic_up=0;
gMusic_down=0;
gSound_up=0;
gSound_down=0;
gRadarRangeInput=0;
gCamera_zoom_in=0;
gCamera_zoom_out=0;
gMissileToggle=0;
gShowInventory=0;
gEject=0;
gAskForDock=0;
gCameraMode=0;
gvel_limiter_input=0;
gCPMode=0;
gCommandControl=0;
gGM=0;
}





OSStatus InitNeeds()
{
#define kNumNeeds numInputs
#define kIconSuiteID_Cannon 129
#define   kIconSuiteID_YThrustUp 130
#define   kIconSuiteID_Turn 131
#define kIconSuiteID_Laser 132
#define kIconSuiteID_Missile 133
#define kIconSuiteID_Retro 134
#define kIconSuiteID_Abort 135
#define kIconSuiteID_mvol_up 136
#define kIconSuiteID_mvol_down 137
#define kIconSuiteID_svol_up 138
#define kIconSuiteID_svol_down 139
#define   kIconSuiteID_YThrustDown 140
#define   kIconSuiteID_YRadarRange 141
#define   kIconSuiteID_zoom_in 143
#define   kIconSuiteID_zoom_out 142	//I buggered these up a bit
#define   kIconSuiteID_MissileToggle 145
#define   kIconSuiteID_ShowInventory 146
#define   kIconSuiteID_Eject 147
#define   kIconSuiteID_Dock 148
#define   kIconSuiteID_CameraMode 149
#define   kIconSuiteID_vel_limiter 150
#define   kIconSuiteID_CPMode 151
#define   kIconSuiteID_Command 152
#define   kIconSuiteID_GM 153


int theError;

//oldmac//extern ISpElementReference	gInputElements[numInputs];

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
ISpNeed myNeeds[kNumNeeds] =
{
 {
  "Forward Thrust",     //human name
  kIconSuiteID_YThrustUp, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Axis_Throttle,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

  {
  "Reverse Thrust",     //human name
  kIconSuiteID_YThrustDown, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Axis_Throttle,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },


  
 {
  "Direction",     //human name
  kIconSuiteID_Turn, //icon
  0,0,	//player and group
 kISpElementKind_Axis,  //kind
  kISpElementLabel_Axis_Rudder,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },


  {
  "Cannon",
  kIconSuiteID_Cannon,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Laser",
  kIconSuiteID_Laser,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Missile",
  kIconSuiteID_Missile,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Retro Thrust",
  kIconSuiteID_Retro,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Missile Pylon Select",
  kIconSuiteID_MissileToggle,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Radar Range Toggle",
  kIconSuiteID_YRadarRange,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Request Mothership To Dock",
  kIconSuiteID_Dock,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Eject",
  kIconSuiteID_Eject,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

 {
  "Engine Control",     //engine control
  kIconSuiteID_vel_limiter, //icon
  0,0,	//player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },
 
 {
  "Command And Control",     //ommand and control
  kIconSuiteID_Command, //icon
  0,0,	      //player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

 {
  "Galactic Map",     //galactic map
  kIconSuiteID_GM, //icon
  0,0,	      //player and group
 kISpElementKind_Button,  //kind
  kISpElementLabel_Btn_Fire,  //label
  0,    //flags
  0,	//rsvd 1
  0,	//rsvd 2
  0     //rsvd 3
  },

  {
  "Show Ship Status Screen",
  kIconSuiteID_ShowInventory,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Pause Game",
  kIconSuiteID_Abort,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Music Volume Up",
  kIconSuiteID_mvol_up,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },
  {
  "Music Volume Down",
  kIconSuiteID_mvol_down,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Sound Volume Up",
  kIconSuiteID_svol_up,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },
  
  {
  "Sound Volume Down",
  kIconSuiteID_svol_down,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Camera Zoom In",
  kIconSuiteID_zoom_in,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },

  {
  "Camera Zoom Out",
  kIconSuiteID_zoom_out,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Camera Mode",
  kIconSuiteID_CameraMode,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },


  {
  "Toggle Instrument Panel",	   //toggle display panel
  kIconSuiteID_CPMode,
  0,0,
  kISpElementKind_Button,
  kISpElementLabel_Btn_Fire,
  0,
  0,
  0,
  0
  },




  };
#endif


	//¥	Setup the input sprocket elements
//oldmac//	theError = ISpElement_NewVirtualFromNeeds(kNumNeeds, myNeeds, gInputElements, 0);
	theError = 0;
	if (theError)
		report_error_c("Could not create ISp virtual controls from needs.","",theError);

	//¥	Init InputSprocket and tell it our needs
	//oldmac// theError = ISpInit(numInputs, myNeeds, gInputElements, 'LsZx', '0002', 0, 128, 0);
	if (theError)
		report_error_c("Could not initialize Input","",theError);

	//¥	Turn on the keyboard and mouse handlers
//	  ISpDevices_ActivateClass (kISpDeviceClass_SpeechRecognition);
//take_mouse_keyboard_as_input();
// return result;
return 0;
}

void take_mouse_keyboard_as_input()
{
//oldmac//if (gISPState==1)
//oldmac//	ISpDevices_ActivateClass (kISpDeviceClass_Keyboard);
//	ISpDevices_ActivateClass (kISpDeviceClass_Mouse);
}


void untake_mouse_keyboard_as_input()
{
//oldmac//if (gISPState==1)
//oldmac//	ISpDevices_DeactivateClass (kISpDeviceClass_Keyboard);
//	ISpDevices_DeactivateClass (kISpDeviceClass_Mouse);
}

extern int gCheckWatchDog;
void configure_input_device()
{
UInt32 old_state=gISPState;

   gCheckWatchDog=0;	//disable the watchdog

if (gISPState==0)
{
 enable_inputs();
}

do_key_setting();

//oldmac//ISpConfigure(0);

if (old_state==0)
{
 disable_inputs();
}
   gCheckWatchDog=1;	//reenable the watchdog

}




void SetKeyboardMouseActivation(Boolean active)
{
	
#if 0 
enum { kSimple = 100 };
ISpDeviceReference buffer[kSimple];
UInt32 count;

ISpDevices_ExtractByClass(kISpDeviceClass_Mouse, kSimple, &count, buffer);
if (active) 
   { ISpDevices_Activate(count,buffer); }
else 
   { ISpDevices_Deactivate(count, buffer); }

ISpDevices_ExtractByClass(kISpDeviceClass_Keyboard,kSimple,&count, buffer);
if (active) 
   { ISpDevices_Activate(count,buffer); }
else 
   { ISpDevices_Deactivate(count, buffer); }
#endif
}


void kill_inputs()
{
gISPState=0;
//untake_mouse_keyboard_as_input();
//ISpSuspend();
//ISpStop();
	
	
//oldmac//ISpShutdown();
}

void enable_inputs()
{
gISPState=1;
//oldmac//ISpResume();
 take_mouse_keyboard_as_input();

}

void disable_inputs()
{
gISPState=0;
 untake_mouse_keyboard_as_input();
//oldmac//ISpSuspend();

}


int game_fire_button()
{
extern UInt32 gCannon;
extern UInt32 gMissile;
extern UInt32 gLaser;
    poll_input_device();
return ( gCannon || gLaser || gMissile || LS_Left_Button());
}

void ZexGetMouse (Point *the_point)
{
//oldmac//extern	  DSpContextReference		      mDisplayContext;
extern int splat_offset_x, splat_offset_y;

//oldmac//DSpGetMouse (the_point);
//oldmac//DSpContext_GlobalToLocal (mDisplayContext,the_point);

	int x, y;
	get_mouse_x_and_y(&x, &y);
	
	the_point->h = x;
	the_point->v = y;
	
//correct for our splat offsets
the_point->h-=splat_offset_x;
(*the_point).v-=splat_offset_y;

}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: IsKeyPressed            | AUTHOR(s): Rob Probin   | DATE STARTED: 04 June 05
// +
// | DESCRIPTION: Returns whether a single, specific key is pressed.
// +----------------------------------------------------------------ROUTINE HEADER----

bool IsKeyPressed(SDLKey key)
{
	process_standard_event_functions();
	
	if(key >= 0 && key <= MAX_NUMBER_OF_STORED_KEYS)
	{
		return key_pressed_store[key];
	}
	else
	{
		report_error_c("Key requested is out of range  in IsKeyPressed()","",0);
		return false;
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: IsKeyPressed            | AUTHOR(s): Rob Probin   | DATE STARTED: 04 June 05
// +
// | DESCRIPTION: Returns whether one key is pressed, and the code for that key (if the pointer is not NULL).
// +----------------------------------------------------------------ROUTINE HEADER----

bool is_one_key_pressed(SDLKey* key)
{
	
	process_standard_event_functions();

	// scan all the keys - and count how many are pressed
	int key_pressed_count=0;
	for(int i=0; i<MAX_NUMBER_OF_STORED_KEYS; i++)
	{
		if(key_pressed_store[i])
		{
			key_pressed_count++;
			if(key_pressed_count==1 && key)			// return the first key if pointer is not NULL
			{
				*key = static_cast<SDLKey>(i);
			}
		}
	}
	
	return key_pressed_count==1;
}


//oldmac//
/*short isPressed(unsigned short k )
// k =  any keyboard scan code, 0-127
{
	unsigned char km[16];

	GetKeys( (unsigned long *) km);
	return ( ( km[k>>3] >> (k & 7) ) & 1);
}*/

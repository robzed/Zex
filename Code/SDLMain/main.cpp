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


/* Simple program:  Create a blank window, wait for keypress, quit.
*
* Please see the SDL documentation for details on using the SDL API:
* /Developer/Documentation/SDL/docs.html
*
*
* $Author: rob $
* $Date: 2005/12/11 21:25:39 $
* $Revision: 1.18 $
*
* For log see end of file...
*
*
*/
#include "options.h"		// should always be first include
#include "SDL.h"

#include "LS_Standard.h"
#include "splat_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ZEX.h"
#include "main.h"

// ***********************************************************************************
// * INTERNAL VARIABLES
// *

static bool SDL_running_flag = false;			// probably don't need this, but still
static bool video_mode_windowed;		// are we running in a window?
static bool video_mode_initialised = false;	// are we initialised?

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static void exit_function();
static void close_down_video();
static void start_up_video(bool full_screen);

// ***********************************************************************************
// * ACTUAL CODE
// *


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: main                       | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Apr 2005
// +
// | This function handles setting up the screen then calling the zex_main control structure.
// +----------------------------------------------------------------ROUTINE HEADER----
int main(int argc, char *argv[])
{
	Uint32 initflags = SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO;  /* See documentation for details */

	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",
				SDL_GetError());
		exit(1);
	}
	
	SDL_WM_SetCaption("Zex", "Zex");
	//SDL_WM_SetIcon(SDL_Surface *icon, Uint8 *mask);			// This function must be called before the first call to SDL_SetVideoMode.
	
	// set up a function so that SDL gets cleaned up when we exit
	SDL_running_flag = true;
	atexit(exit_function);
	
	// what video mode do we start up in...
	//full_screen_video();
	windowed_video();
	
	// MOUSE CURSOR NOTES:
	// Don't enable the cursor here. Initialising sound mixer turns it on, 
	// sometimes (e.g. when the mouse is moving). However if we've turned it off already
	// then the internal lock on turning it off is active ... meaning we can't
	// turn it off - or something like this. Basically - it works ... if you touch it
	// test it wwll...
	// SDL_ShowCursor(SDL_DISABLE);
	
	//
	// this is the main loop
	//
	process_standard_event_functions();
	zex_main();
	
	/* Clean up the SDL library */
	exit(0);				// do it via exit function (so we don't quit twice?)
	//	SDL_running_flag = false;
	//	SDL_Quit();

	return(0);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: exit_function              | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Apr 2005
// +
// | This function is called on exit
// +----------------------------------------------------------------ROUTINE HEADER----

static void exit_function()
{
	if(SDL_running_flag)
	{
		SDL_Quit();
		SDL_running_flag = false;
	}
	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: toggle_video               | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
void toggle_video()
{
	if(video_mode_windowed)
	{
		full_screen_video();
	}
	else
	{
		windowed_video();
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: full_screen_video          | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
void full_screen_video()
{
	if(video_mode_windowed || !video_mode_initialised)
	{
		close_down_video();
		start_up_video(true);
		video_mode_windowed = false;
		video_mode_initialised = true;
		cursor_inside_of_window(true);	// just in case the warp mouse doesn't give us a active event
		SDL_WarpMouse(320,240);			// center the mouse, and give us a mouse moved event
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: windowed_video             | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void windowed_video()
{
	if(!video_mode_windowed || !video_mode_initialised)
	{
		close_down_video();
		start_up_video(false);
		video_mode_windowed = true;
		video_mode_initialised = true;		
		cursor_inside_of_window(true);	// just in case the warp mouse doesn't give us a active event
		SDL_WarpMouse(320,240);			// center the mouse, and give us a mouse moved event
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: close_down_video             | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void close_down_video()
{
	if(video_mode_initialised)
	{
		video_mode_initialised = false;
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: close_down_video             | AUTHOR(s): Rob Probin   | DATE STARTED: 4 June 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void start_up_video(bool full_screen_flag)
{
	if(!video_mode_initialised)
	{
		SDL_Surface *screen;
		Uint8  video_bpp = 8;	// we want 8 bit mode. If you choose 0 if will try to get any bpp;
		Uint32 videoflags = SDL_SWSURFACE;
		if(full_screen_flag)
		{
			videoflags |= SDL_FULLSCREEN;
			videoflags |= SDL_HWPALETTE;		// Give SDL exclusive palette access. Without this flag you may not always get the the colors you request with SDL_SetColors or SDL_SetPalette.
		}
		
		/* Set 640x480 video mode */
		screen=SDL_SetVideoMode(WIND_WIDTH, WIND_HEIGHT, video_bpp, videoflags);
		if (screen == NULL) {
			fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n",
					video_bpp, SDL_GetError());
			SDL_running_flag = false;
			SDL_Quit();
			exit(3);
		}

		//void printf_surface_data(const char* name, SDL_Surface *surface);
		//printf_surface_data("Screen", screen);

		set_up_game_screen_buffers(screen);
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: close_down_video             | AUTHOR(s): Rob Probin   | DATE STARTED: 4 Dec 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

bool are_we_in_full_screen_mode()
{
	return !video_mode_windowed;
}


/* CVS LOG
*
* $Log: main.cpp,v $
* Revision 1.18  2005/12/11 21:25:39  rob
* Zex2005: Minor comment and debug change.
*
* Revision 1.17  2005/12/10 18:59:48  rob
* Zex2005: Center cursor on mode change so that we know it's definately on screen...
*
* Revision 1.16  2005/12/10 10:50:26  rob
* Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.
*
* Revision 1.15  2005/11/26 21:33:13  rob
* Zex2005: Removed code to print SDL versions now that we have the info in the boot text.
*
* Revision 1.14  2005/11/25 14:36:30  rob
* Zex2005: Added code to print SDL versions for temporary debugging.
*
* Revision 1.13  2005/10/10 21:27:27  rob
* Zex2005: Added newline at end to stop compiler whinging.
*
* Revision 1.12  2005/06/04 13:11:03  rob
* Zex2005: Basic full screen / windowed mode change.
*
* Revision 1.10  2005/06/04 08:54:09  rob
* Zex2005: We now have keyboard control in the game!!!
*
* Revision 1.9  2005/06/01 19:07:20  rob
* Zex2005: Updated project - now runs main menu.
*
* Revision 1.8  2005/05/23 21:13:45  rob
* Zex2005: More changes to files.
*
* Revision 1.7  2005/05/15 17:42:01  rob
* Zex2005: ZexNumToString used throughout, file case changes, general gcc4.0 build changes.
*
* Revision 1.6  2005/05/13 09:24:51  rob
* Zex2005: Changed Button to internal version. Altered string to dec code to use atof().
*
* Revision 1.5  2005/05/04 21:08:39  rob
* Zex2005: Added some screen splat code.
*
* Revision 1.4  2005/05/02 10:30:12  rob
* Zex2005: More Updates (code)
*
* Revision 1.3  2005/04/25 20:39:54  rob
* Zex2005: Check in flush events.
*
* Revision 1.2  2005/04/24 10:09:52  rob
* ZEX: Started to work on ZEX.cpp. Replaced TickCount. Added some void return types. Commented out some Mac init functions. Altered main.cpp to do the correct thing on an exit().
*
*
*
*/




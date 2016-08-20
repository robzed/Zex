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

// *****************************************************************MODULE HEADER*****
// * FILENAME		: options.h
// * MODULE TITLE	: ============ZEX BUILD OPTIONS FILE=========
//
// * PROJECT		: Zex
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// Notes:
//
// All flags should be 1 or 0 to enable or disable, NOT commented out/uncommented, since you cannot
// tell if a header (e.g. options.h) has been left out, rather than disabled. The end
// result is that you can have different parts of the program built in different manners.
//
// This file should always be first include in a source file.
//
#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#define PCI_TIMING_DEBUG	0			// use PCI timing to generate percentage time spent in different sections
										// DISABLE FOR TEST AND FINAL BUILDS - non-pci macs don't support this!

#define FIND_PERCENTAGES_DEBUG 0		// show percentages of different sections on screen
										// best to run it without splat in vbl !!!!! (all interrupt costs are hidden)



#define Z_DEBUG 1	//if 1, Pers checks Z coordinates are not behind the camera


#define USE_EXEC_WATCHDOG 0	//if set, vbl task decs exec_watchdog. If it gets to zero we error!

#define MESSAGES_CENTER 0	//all messages centered if 1
#define TEXT_DISPLAY_TIME 600	//frame count for a message to stay on screen
#define OGL 0	//set to 1 for OGL version, 0 for SW renderer


// when getting running on SDL - some of this needs to go back in
#define DISABLED_BY_ROB_WHEN_GETTING_RUNNING 0
// used like this
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
#endif	// DISABLED_BY_ROB_WHEN_GETTING_RUNNING

/* If EXPIRE is 1 then game will expire after below date */
#define EXPIRE 0
#define EXPIRE_YEAR 2015
#define EXPIRE_MONTH 12	//End of December 2015

#define ZEX_LIVES_FOR_EVER 0	//if not 0 then Zex can't be damaged, doesn't run out of fuel and gets all possible equipment
#define BUY_SHOW_ALL_OFFSET -1	//if -1, then normal buy operation otherwise will show all items starting from this offset (in sets of 15)



#define CALLSIGN_DEBUG_MODE 0		// if not 0 then enable callsign debug

#define DEBUG_GAME_STATE 0			// 0=off, 1=overview, 2=medium, 3=verbose

//Messages to display if unregistered in display_unreg
#define UNREG_LOAD 0		//Cant load if unreg
#define UNREG_WARPHOLE 1	//Can't enter warpholes
#define UNREG_PERIODIC_NAGSCREEN 2   //Periodic Nag screen
#define UNREG_CAPTAIN_LOAD 3		// can't load captain if unreg

//if NO_LOAD_IF_UNREG is 1 then you can't load games in the unreg version
#define NO_LOAD_IF_UNREG 0
#define ENABLE_PERIODIC_NAGSCREEN 1		// show a message asking people to register
#define NO_CAPTAIN_LOAD_IF_UNREG 0		// don't load if unreg and captain or above
#define FLASH_MAIN_MENU_UNREG 0			// flash the sign on the main menu ...

// set this to make the game registered
#define FORCE_REGISTRATION 1

#endif // _OPTIONS_H_


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

/*
 *  game_file_selector.cpp
 *  Z_dungeon
 *
 *  Created by Rob Probin on Thu Jan 08 2004.
 *  Copyright (c) 2004 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/10 22:50:13 $
 * $Revision: 1.7 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include "general_include.h"

#include "game_file_parser.h"
#include "game_file_selector.h"

#include "game_save.h"

//#include "Game_Controller.h"
//#include "Player_Interface.h"

//#include "fonts.h"

#include <string>
#include <sstream>

#include "mac_emulation.h"
#include "Error_handler.h"
#include "get_text_string.h"
#include "zdungeon_printing_emulation.h"

#if NO_CAPTAIN_LOAD_IF_UNREG
// for the unregistered limitations..
#include "map_control.h"
#include "file_io.h"
#include "game_defs.h"
// end of includes for unregistered limitations.
#endif

// ***********************************************************************************
// * 
// *


// ***********************************************************************************
// * CONSTANTS 
// *

const int NO_FLIP = 0;			// graphic reference - not used in Zex



// ***********************************************************************************
// * MACROS 
// *

// ***********************************************************************************
// * TYPE DEFS
// *

// ***********************************************************************************
// * LOCAL FUNCTIONS
// *

// these are function intermediates from the menu system that vector to the object methods
static void print_slot_details(menu_table_type *this_menu);
static void set_dialog_title(menu_table_type *this_menu);
static void cancel_click(menu_table_type *this_menu);
static void previous_button_init(menu_table_type *this_menu);
static void previous_button_click(menu_table_type *this_menu);
static void next_button_init(menu_table_type *this_menu);
static void next_button_click(menu_table_type *this_menu);
static void slot_init(menu_table_type *this_menu);
static void slot_click(menu_table_type *this_menu);


// the actual functions
static void print_slot_details(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->print_slot_details(this_menu); }
static void set_dialog_title(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->set_dialog_title(this_menu); }
static void cancel_click(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->cancel_click(this_menu); }
static void previous_button_init(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->previous_button_init(this_menu); }
static void previous_button_click(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->previous_button_click(this_menu); }
static void next_button_init(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->next_button_init(this_menu); }
static void next_button_click(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->next_button_click(this_menu); }
static void slot_init(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->slot_init(this_menu); }
static void slot_click(menu_table_type *this_menu) 
	{ game_file_selector::get_current_instance()->slot_click(this_menu); }

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *


//
// page_slot_manager class
// 

// +--------------------------------+-------------------------+-----------------------
// | TITLE: page_slot_manager       | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Apr 04
// +
// | DESCRIPTION: Create the page slot manager
// +----------------------------------------------------------------ROUTINE HEADER----

game_file_selector::page_slot_manager::page_slot_manager() 
{ 
	this->page_start_slot_number = 1;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: return_page_slot_number | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Apr 04
// +
// | DESCRIPTION: Create the page slot manager
// +----------------------------------------------------------------ROUTINE HEADER----

int game_file_selector::page_slot_manager::return_page_slot_number() 
{ 
	return page_start_slot_number; 
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: increase_page_slot      | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Apr 04
// +
// | DESCRIPTION: Create the page slot manager
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_selector::page_slot_manager::increase_page_slot(int increase_by_count) 
{
	this->page_start_slot_number += increase_by_count;
	if(this->page_start_slot_number+NUMBER_SLOTS_PER_MENU-1 > NUMBER_OF_SAVED_GAME_SLOTS)
	{
		this->page_start_slot_number = (NUMBER_OF_SAVED_GAME_SLOTS-NUMBER_SLOTS_PER_MENU)+1;
	}
	
	// catch the case where it underflows (broken code anyway)
	if(this->page_start_slot_number < 1)
	{
		this->page_start_slot_number = 1;
	}
	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: decrease_page_slot      | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Apr 04
// +
// | DESCRIPTION: Create the page slot manager
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_selector::page_slot_manager::decrease_page_slot(int decrease_by_count) 
{
	// if we aren't at the bottom, decrease the current slot
	if(this->page_start_slot_number > 1) 
	{
		this->page_start_slot_number -= decrease_by_count;
	}

	// ensure it never goes below 1 - whatever happens
	if(this->page_start_slot_number < 1) 
	{
		this->page_start_slot_number = 1;
	}
	
}


// +---------------------------------+-------------------------+-----------------------
// | TITLE: game_file_selector::load | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

game_file_selector* game_file_selector::get_current_instance() 
{ 	
	if(!instance_valid) { report_error_c("Invalid current instance","game_file_selector",0); }
	if(current_instance==NULL) { report_error_c("Invalid current instance pointer","game_file_selector",0); }
	return current_instance;
}


game_file_selector *game_file_selector::current_instance = NULL;	// where we store the instance
bool game_file_selector::instance_valid = false;					// is the instance valid



/*
 
 menu structure...
 
 SAVE FILE SELECTOR				-> 1 item
 
 previous (if start>0)						-> 1 item
 
 slot 0 (start+0)  <user description>  <save date/time>			-> 1 item (composite item)
 Player 1 - Level: <number> Room: <roomname>
 single player (OR if 2 player game)  p2 character level, p2 room name
 
 slot 1 (start+1)  No game found  (if start + 1 < number_of_entries)
 
 slot 2 (start+2)  No game found  (if start + 2 < number_of_entries)
 
 slot 3 (start+3)  No game found  (if start + 3 < number_of_entries)
 
 slot 4 (start+4)  No game found  (if start + 4 < number_of_entries)
 
 
 next (if start + 5perscreen <= number_of_entries)			-> 1 item
 
 button=CANCEL    button=SAVE					-> 2 items
 
 
 Other features: Ability to edit name...
 
 */

const menu_table_type /*game_file_selector::*/menu_template[] = 
{
    {	// **** MENU TITLE ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 0,  display_item_only, 220,  10, -20, true,	    false,		false,		-1,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 220, 10, 0, 0, }, 
		// standard                           highlight           clicked             selected
		file_dialog_save_as_title,        empty_text, 		   empty_text,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		NULL,          NULL,          NULL,             NULL,	     set_dialog_title,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** previous ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 0,  event_item, 50,  45, -20, true,	    false,		false,		SDLK_p,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 45, 130, 30, }, 
		// standard                           highlight           clicked             selected
		file_dialog_previous,        Mhighlight_char, file_dialog_previous_click,	empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		NULL,     previous_button_click,NULL,     previous_button_init,	     previous_button_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** SLOT 1 ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 0,  event_item, 50,  100, -20, true,	    false,		false,		-1,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 100, 550, 55, }, 
		// standard                           highlight           clicked             selected
		empty_text,        Mhighlight_char, 		   empty_text,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		print_slot_details, slot_click,          NULL, slot_init, slot_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** SLOT 2 ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 1,  event_item, 50,  160, -20, true,	    false,		false,		-1,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 160, 550, 55, }, 
		// standard                           highlight           clicked             selected
		empty_text,        Mhighlight_char, 		   empty_text,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		print_slot_details, slot_click,          NULL, slot_init, slot_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** SLOT 3 ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 2,  event_item, 50,  220, -20, true,	    false,		false,		-1,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 220, 550, 55, }, 
		// standard                           highlight           clicked             selected
		empty_text,        Mhighlight_char, 		   empty_text,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		print_slot_details, slot_click,          NULL, slot_init, slot_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** SLOT 4 ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 3,  event_item, 50,  280, -20, true,	    false,		false,		-1,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 280, 550, 55, }, 
		// standard                           highlight           clicked             selected
		empty_text,        Mhighlight_char, 		   empty_text,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		print_slot_details, slot_click,          NULL, slot_init, slot_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** SLOT 5 ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 4,  event_item, 50,  340, -20, true,	    false,		false,		-1,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 340, 550, 55, }, 
		// standard                           highlight           clicked             selected
		empty_text,        Mhighlight_char, 		   empty_text,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		print_slot_details, slot_click,          NULL, slot_init, slot_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** next ***
		//  last? user_info        _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 0,  event_item, 50,  419, -20, true,	    false,		false,		SDLK_n,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 40, 419, 130, 30, }, 
		// standard                           highlight           clicked             selected
		file_dialog_next,        Mhighlight_char, 	file_dialog_next_click,			empty_text,
		//  display_call   click_call   save_call       run_loop_call    init_call
		NULL,  next_button_click,       NULL,       next_button_init,	     next_button_init,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
    {	// **** CANCEL button ***
		//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		false, 0,  event_item, 400, 419, -40,   true,		false,		false,		SDLK_c,	    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 400, 419, 200, 40, }, 
		// standard             highlight           clicked             selected
		file_dialog_cancel, highlight_char,    file_dialog_cancel_click, empty_text,
		//  display_call          click_call   save_call       run_loop_call    init_call
		NULL,          cancel_click,          NULL,             NULL,	     NULL,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
	
	
	/*    {	// **** SAVE/LOAD button ***
			//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false, 0,  event_item, 460, 550, -20,   true,		false,		false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 460, 550, 150, 40, }, 
	// standard             highlight           clicked             selected
	file_dialog_save, highlight_char,    file_dialog_save_click, empty_text,
	//  display_call          click_call   save_call       run_loop_call    init_call
	NULL,          load_save_click,          NULL,             NULL,	     set_save_load_button_title,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    },
*/	
	
	// -------------------------- 
	// Last item
	//
    {	// **** HIDDEN KEY ENTRY - hit the escape key to quit ***
		//  last? user_info  _menu_control_       x   y  hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
		true,       0,  event_item,          0, 0, 0,  false,    false,    	false,	    SDLK_ESCAPE,    false,
		//  value  graphics_attrib    mouse rect - x y w h 
		0,		NO_FLIP,          { 0, 0, 0, 0, }, 
		// standard             highlight           clicked             selected
		empty_text, empty_text, empty_text, empty_text,
		//  display_call   click_call                 save_call       run_loop_call    init_call
		NULL,          cancel_click,          NULL,             NULL,           NULL,
		// click sound       hover sound
		//    "",			"",
		// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
		false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
    }
	
	
};


// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

// ***********************************************************************************
// * ACTUAL CODE
// *

// +---------------------------------+-------------------------+-----------------------
// | TITLE: game_file_selector::load | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

int game_file_selector::load()
{
	mode = fs_load;
	return file_selector_core();
}

// +---------------------------------+-------------------------+-----------------------
// | TITLE: game_file_selector::save | AUTHOR(s): Rob Probin   | DATE STARTED: 20 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

int game_file_selector::save()
{
	mode = fs_save;
	return file_selector_core();
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: game_file_selector      | AUTHOR(s): Rob Probin   | DATE STARTED: 8 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

game_file_selector::game_file_selector()
{
	selected_game = 0;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: file_selector_core      | AUTHOR(s): Rob Probin   | DATE STARTED: 8 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

int game_file_selector::file_selector_core()
{
	int slot;
	
	
	// create individually (rather than in an array?)
	for(slot=0; slot<NUMBER_OF_SAVED_GAME_SLOTS; slot++)
	{
		games[slot] = new game_file_parser(slot+1);		// zero check not needed - new throws an exception on out of memory
	}
	
	
	menu_table_type* file_selector_dialog = create_menu();
	
	// we only maintain a valid current instance for this currently running menu.
	// therefore there can only be one active fileselector at a time.
	// that is not a problem for the menu system
	current_instance = this;
	instance_valid = true;
	
	run_menu(file_selector_dialog, true); 

#if NO_CAPTAIN_LOAD_IF_UNREG
#if EXPIRE		//don't show this if it's a test version
#else
	while(true)
	{
		if(selected_game != 0 && mode == fs_load)
		{
			extern prefs gzex_prefs;
			if (gzex_prefs.registered!=1 && games[selected_game-1]->get_game_rank() >= CAPT)
			{
				//can't load if not registered
				display_unreg(UNREG_CAPTAIN_LOAD);
				run_menu(file_selector_dialog, true); 
				continue;
			}
		}
		break;
	}
#endif	//EXPIRE
#endif //NO_CAPTAIN_LOAD_IF_UNREG
	
	instance_valid = false;
	current_instance = NULL;
	 
	remove_menu(file_selector_dialog);
	
	
	// delete individually
	for(slot=0; slot<NUMBER_OF_SAVED_GAME_SLOTS; slot++)
	{
		delete games[slot];
	}
	
	return selected_game; // this is the slot number from 1 to NUMBER_OF_SAVED_GAME_SLOTS-1
						// 0 means no game selected					
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: create_menu      | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

menu_table_type* game_file_selector::create_menu()
{
	int items_in_menu = (sizeof(menu_template) / sizeof(menu_table_type));
	
	menu_table_type* file_selector_dialog = new menu_table_type[items_in_menu];		// zero check not needed - new throws an exception on out of memory
	
	// copy across menu template
	for(int i=0; i<items_in_menu; i++)
	{
		file_selector_dialog[i] = menu_template[i];
	}
	
	
	//
	// now do customisation...
	//
	
	// need to check the slots for 0 and print blanks/error if so(?)
	// also if success = 0 then 
	
	
	// construct a menu
	// display a menu
	
	// next (if one screen less available)
	// previous (if less available)
	
	// items enabled if selectable (otherwise displayed as empty or can't load)
	
	
	return file_selector_dialog;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: remove_menu             | AUTHOR(s): Rob Probin   | DATE STARTED: 23 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_selector::remove_menu(menu_table_type* file_selector_dialog)
{
	delete [] file_selector_dialog;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: user_type_dispatch      | AUTHOR(s): Rob Probin   | DATE STARTED: 25 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::set_dialog_title(menu_table_type *this_menu)
{
	if(mode == fs_load) 
	{
		this_menu->standard_string_name = file_dialog_load_as_title;
	}
	else 
	{
		this_menu->standard_string_name = file_dialog_save_as_title;
	}
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: set_save_load_button_title | AUTHOR(s): Rob Probin   | DATE STARTED: 25 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
/* void game_file_selector::set_save_load_button_title(menu_table_type *this_menu)
{
	if(mode == fs_load) 
	{
		this_menu->standard_string_name = file_dialog_load;
		this_menu->clicked_string_name = file_dialog_load_click;
		this_menu->direct_key_value = SDLK_l;
	}
	else 
	{
		this_menu->standard_string_name = file_dialog_save;
		this_menu->clicked_string_name = file_dialog_save_click;
		this_menu->direct_key_value = SDLK_s;
	}
}
*/

// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_slot_details      | AUTHOR(s): Rob Probin   | DATE STARTED: 25 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
/*
 slot 0 (start+0)  <user description>  <save date/time>			-> 1 item (composite item)
 Player 1 - Level: <number> Room: <roomname>
 single player (OR if 2 player game)  p2 character level, p2 room name
 
 or 
 
 slot 0 (start+1)  No game found  (if start + 1 < number_of_entries)
 */

void game_file_selector::print_slot_details(menu_table_type *this_menu)
{
	std::ostringstream slot_string;
	int base_x = this_menu->print_location_x;
	int base_y = this_menu->print_location_y;
	int slot = page_slot.return_page_slot_number()+this_menu->user_defined_data;

	// print the slot number
	slot_string << get_text(file_dialog_slot_number_text) << " " << slot;
	print_string(MEDIUM_MENU_WHITE_NO_BG, base_x, base_y, slot_string.str().c_str());
	
	// determine if there is any game
	
	if(games[slot-1]->game_present())
	{
		int font = SMALL_MENU_WHITE_NO_BG;
		TextSize(extract_size_from_font_ref(font));
		int font_height = get_font_height()+2;		// give us some spacing between the lines
		games[slot-1]->print_line1(font, base_x+100, base_y);
		games[slot-1]->print_line2(SMALL_MENU_YELLOW_NO_BG, base_x+100, base_y+font_height);
		games[slot-1]->print_line3(SMALL_MENU_ORANGE_NO_BG, base_x+100, base_y+(2*font_height));
		games[slot-1]->print_line4(SMALL_MENU_RED_NO_BG, base_x+100, base_y+(3*font_height));
	}
	else
	{
		print_string(MEDIUM_MENU_WHITE_NO_BG, base_x+100, base_y, get_text(file_dialog_no_game_text));
	}
	
}
// string notes

//char slot_string[200];
//sprintf(slot_string, "%s %i",get_text(file_dialog_slot_number_text), slot_number);
//print_string(MEDIUM_MENU_WHITE_NO_BG, base_x, base_y, slot_string);

//std::string slot_string;
//slot_string = "string";
//print_string(MEDIUM_MENU_WHITE_NO_BG, base_x, base_y, slot_string.c_str());

// +--------------------------------+-------------------------+-----------------------
// | TITLE: slot_init               | AUTHOR(s): Rob Probin   | DATE STARTED: 14 Apr 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::slot_init(menu_table_type *this_menu)
{
	int slot = page_slot.return_page_slot_number()+this_menu->user_defined_data;
	
	
	if(mode == fs_load) 
	{   // in load menu slots are only available if there is an actual game present
		if(games[slot-1]->game_present())
		{
			// game present - this slot should be available for selection
			this_menu->menu_control = event_item; // make event item
		}
		else
		{
			// game no present - this slot should be unselected
			this_menu->menu_control = display_item_only; // de-select
			unhighlight_menu_entry(this_menu);
		}
	}
	else	// save menu always has all slots
	{
		this_menu->menu_control = event_item; // make event item
	}
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: slot_click              | AUTHOR(s): Rob Probin   | DATE STARTED: 25 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::slot_click(menu_table_type *this_menu)
{
	int slot = page_slot.return_page_slot_number()+this_menu->user_defined_data;
		
	if(mode == fs_save || games[slot-1]->game_present()) 
	{   // in load menu slots are only available if there is an actual game present
		selected_game = slot;
		terminate_menu_mode();
	}
		
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: cancel_click            | AUTHOR(s): Rob Probin   | DATE STARTED: 25 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::cancel_click(menu_table_type *this_menu)
{
	selected_game = 0;
	terminate_menu_mode();
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE: previous_button_init    | AUTHOR(s): Rob Probin   | DATE STARTED: 28 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::previous_button_init(menu_table_type *this_menu)
{
	if(page_slot.return_page_slot_number() > 1)
	{
		this_menu->menu_control = event_item; // make event item
	}
	else
	{
		this_menu->menu_control = non_display_item; // de-select
		unhighlight_menu_entry(this_menu);
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: previous_button_click   | AUTHOR(s): Rob Probin   | DATE STARTED: 28 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_selector::previous_button_click(menu_table_type *this_menu)
{
	page_slot.decrease_page_slot(NEXT_PREVIOUS_STEP);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: next_button_init        | AUTHOR(s): Rob Probin   | DATE STARTED: 28 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::next_button_init(menu_table_type *this_menu)
{
	if((page_slot.return_page_slot_number() + NUMBER_SLOTS_PER_MENU - 1 ) < NUMBER_OF_SAVED_GAME_SLOTS)
	{
		this_menu->menu_control = event_item; // make event item
	}
	else 
	{
		this_menu->menu_control = non_display_item; // de-select
		unhighlight_menu_entry(this_menu);
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: next_button_click       | AUTHOR(s): Rob Probin   | DATE STARTED: 28 Mar 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_selector::next_button_click(menu_table_type *this_menu)
{
	page_slot.increase_page_slot(NEXT_PREVIOUS_STEP);
}





/* CVS LOG
*
* $Log: game_file_selector.cpp,v $
* Revision 1.7  2006/08/10 22:50:13  rob
* Zex2005: Extra save metadata - score, credits, promoition points.
*
* Revision 1.6  2006/06/26 21:04:52  rob
* Zex2005: Disabled no load if captain or above.
*
* Revision 1.5  2006/06/25 16:08:56  rob
* Zex2005: Don't know whether we need this - but here is some code that limits file loading to below captain on the unregistered version.
*
* Revision 1.4  2006/05/18 07:08:46  rob
* Zex2005: Menu for registration code entry.
*
* Revision 1.3  2005/12/16 22:14:18  rob
* Zex2005: Not enough initialisers for menu - Minor warning removal after -Wall -W
*
* Revision 1.2  2005/12/14 21:41:51  rob
* Zex2005: Removed unused screen reference.
*
* Revision 1.1  2005/11/15 22:14:28  rob
* Zex2005: Added new files for game file selector in Zex.
*
* Revision 1.1  2005/01/01 14:12:03  robp
* ZD: Moved files from individual directories under root directory to a source directory.
*
* Revision 1.9  2004/04/28 22:38:57  robp
* ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
*
* Revision 1.8  2004/04/16 20:40:23  robp
* ZD: Altered menu's to use menu highlight offset rather than hardwired offset of 20 in menu driver. Also added correct offsets for file selector. Also disabled event debugging code from menu driver.
*
* Revision 1.7  2004/04/15 22:24:11  robp
* ZD: Many file selector changes - basically making it a working system - including mouse area, functionality changes, etc.
*
* Revision 1.6  2004/04/10 07:36:00  robp
* ZD: File selector changes to date.
*
* Revision 1.5  2004/04/07 21:11:04  robp
* ZD: Latest file selector changes
*
* Revision 1.4  2004/04/07 19:55:41  robp
* ZD: File selector Menu change updates
*
* Revision 1.3  2004/03/28 16:41:00  robp
* ZD: Work in progress checkin.
*
* Revision 1.2  2004/03/20 14:52:54  robp
* ZD: Load/Save file selector changes to date for backup.
*
* Revision 1.1  2004/02/01 12:00:42  robp
* More saved game work up to start of file selector.
*
*
*
*
*/


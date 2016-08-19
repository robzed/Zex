/*
 *  text_entry_menu.cpp
 *  Z_dungeon
 *
 *  Created by Rob Probin on Wed Apr 21 2004.
 *  Copyright (c) 2004 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/12 10:38:29 $
 * $Revision: 1.5 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//#include "general_include.h"

#include "menu_driver.h"
#include "text_entry_menu.h"

#include "get_text_string.h"
#include "zdungeon_printing_emulation.h"


// ***********************************************************************************
// * CONSTANTS 
// *

const int TextEntryMenu::CURSOR_FLASH_TIME_IN_MS = 250;

const int NO_FLIP = 0;			// graphic reference - not used in Zex

// ***********************************************************************************
// * MACROS 
// *

// ***********************************************************************************
// * TYPE DEFS
// *

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static bool TextEntryMenu_handle_input_events(SDL_Event event);
static void TextEntryMenu_display_text(menu_table_type *this_menu);
static void TextEntryMenu_accept_click(menu_table_type *this_menu);
static void TextEntryMenu_cancel_click(menu_table_type *this_menu);
static void TextEntryMenu_clear_click(menu_table_type *this_menu);
static void TextEntryMenu_previous_click(menu_table_type *this_menu);


// ***********************************************************************************
// * THIS MENU DEFINITION
// *


static menu_table_type file_save_description_menu[] = 
{
{	// **** MENU TITLE ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  display_item_only, 180,  70, -40, true,	    false,	false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 180, 70, 0, 0, }, 
	// standard             highlight           clicked             selected
	save_as_dialog_title,   empty_text, 	   empty_text,			empty_text,
	//  display_call   click_call   save_call       run_loop_call    init_call
	NULL,          NULL,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// **** MENU SUB-TITLE - the description edit box ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  display_item_only, 20,  200, -40,		false,	    false,		 false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 20, 200, 0, 0, }, 
	// standard             highlight           clicked             selected
	empty_text,      empty_text, 		    empty_text,			empty_text,
	//  display_call                     click_call   save_call       run_loop_call    init_call
	TextEntryMenu_display_text,          NULL,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// *** SUB OPTION - Clear ***
	//  last? user_info  _menu_control_       x   y   hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  event_item,         380,  250, -40, true,    false,	false,   SDLK_ESCAPE,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 360, 240, 100, 30, }, 
	// standard             highlight           clicked             selected
	save_as_dialog_clear, highlight_char, save_as_dialog_clear_click,empty_text,
	//  display_call   click_call   		     save_call       run_loop_call    init_call
	NULL,         TextEntryMenu_clear_click,    NULL,             NULL,            NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// *** SUB OPTION - Use Previous ***
	//  last? user_info  _menu_control_       x   y   hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  event_item,         500,  250, -40, true,    false,	false,   SDLK_ESCAPE,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 480, 240, 140, 30, }, 
	// standard             highlight           clicked             selected
	save_as_dialog_use_previous, highlight_char, save_as_dialog_use_previous_click,empty_text,
	//  display_call   click_call   		     save_call       run_loop_call    init_call
	NULL,         TextEntryMenu_previous_click,    NULL,             NULL,            NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// *** SUB OPTION - Cancel [escape] ***
	//  last? user_info  _menu_control_       x   y   hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  event_item,         150,  400, -40, true,    false,	false,   SDLK_ESCAPE,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 140, 400, 200, 40, }, 
	// standard             highlight           clicked             selected
	save_as_dialog_cancel, highlight_char, save_as_dialog_cancel_click,empty_text,
	//  display_call   click_call   		     save_call       run_loop_call    init_call
	NULL,         TextEntryMenu_cancel_click,    NULL,             NULL,            NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},


{	// *** SUB OPTION - Accept [return] ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	true,       0,  event_item,         400,  400, -40, true,    false,	false,		SDLK_RETURN,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 390, 400, 235, 40, }, 
	// standard             highlight             clicked                  selected
	save_as_dialog_accept, highlight_char, save_as_dialog_accept_click, empty_text,
	//  display_call   click_call   		          save_call       run_loop_call    init_call
	NULL,               TextEntryMenu_accept_click,   NULL,             NULL,            NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

};


// ***********************************************************************************
// * ACTUAL CODE
// *



// +-----------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu constructor  | AUTHOR(s): Rob Probin   | DATE STARTED: 22Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

TextEntryMenu::TextEntryMenu(int maximum_text_length, const char* old_name)
	: textline(maximum_text_length),accept_description_on_finish(false), cursor_flash_timer(CURSOR_FLASH_TIME_IN_MS)
{
		current_instance = this;
		old_name_store = old_name;
		
		// enable key repeat - notice with key repeat we get multiple key downs and only one key up...
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);			// we ignore return - failure means there will be no repeat in save dialog

		run_menu_advanced(file_save_description_menu, true, TextEntryMenu_handle_input_events);

		SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);			// disable key repeat before we go further...

		current_instance = NULL;
		
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: accepted                   | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
bool TextEntryMenu::user_accepted()
{
	return accept_description_on_finish;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: get_description_c_str      | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: fetch the description string
// +----------------------------------------------------------------ROUTINE HEADER----
const char *TextEntryMenu::get_description_c_str()
{
	return textline.c_str();
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: get_description_str        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: fetch the description string
// +----------------------------------------------------------------ROUTINE HEADER----
std::string TextEntryMenu::get_description_str()
{
	return textline.str();
}


TextEntryMenu *TextEntryMenu::current_instance = NULL;		// where we store the instance

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: get_current_instance       | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
TextEntryMenu *TextEntryMenu::get_current_instance()
{
	return current_instance;
}
// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu_display_text        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void TextEntryMenu_display_text(menu_table_type *this_menu)
{
	TextEntryMenu *the_instance = TextEntryMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->display_text(this_menu);
	}	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: display_text               | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void TextEntryMenu::display_text(menu_table_type *this_menu)
{
	int base_x = this_menu->print_location_x;
	int base_y = this_menu->print_location_y;
	static bool cursor_on = true;
		
	if(cursor_flash_timer.frame_update_and_flag())
	{
		cursor_on = !cursor_on;
	}

	std::string actual_string = textline.str();
	if(cursor_on)
	{
		actual_string += "_";
	}

	print_string(MEDIUM_MENU_WHITE_NO_BG, base_x, base_y, actual_string.c_str());
}

// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu_cancel_click        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void TextEntryMenu_cancel_click(menu_table_type *this_menu)
{
	TextEntryMenu *the_instance = TextEntryMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->cancel_click(this_menu);
	}	
}


// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu_clear_click        | AUTHOR(s): Rob Probin   | DATE STARTED: 11Aug2006
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void TextEntryMenu_clear_click(menu_table_type *this_menu)
{
	TextEntryMenu *the_instance = TextEntryMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->clear_click(this_menu);
	}	
}

// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu_previous_click        | AUTHOR(s): Rob Probin   | DATE STARTED: 11Aug2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void TextEntryMenu_previous_click(menu_table_type *this_menu)
{
	TextEntryMenu *the_instance = TextEntryMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->previous_click(this_menu);
	}	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: cancel_click               | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void TextEntryMenu::cancel_click(menu_table_type *this_menu)
{
	accept_description_on_finish = false;
	terminate_menu_mode();
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: clear_click               | AUTHOR(s): Rob Probin   | DATE STARTED: 11Aug2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void TextEntryMenu::clear_click(menu_table_type *this_menu)
{
	textline.erase();
}
// +-----------------------------------+-------------------------+-----------------------
// | TITLE: previous_click               | AUTHOR(s): Rob Probin   | DATE STARTED: 11Aug2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
#include <string>

void TextEntryMenu::previous_click(menu_table_type *this_menu)
{	
	if(old_name_store)
	{
		textline.load_string(old_name_store);
	}
}



// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu_accept_click        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void TextEntryMenu_accept_click(menu_table_type *this_menu)
{
	TextEntryMenu *the_instance = TextEntryMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->accept_click(this_menu);
	}	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: accept_click               | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void TextEntryMenu::accept_click(menu_table_type *this_menu)
{
	accept_description_on_finish = true;
	terminate_menu_mode();
}



// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: TextEntryMenu_handle_input_events | AUTHOR(s): Rob Probin   | DATE STARTED: 25Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static bool TextEntryMenu_handle_input_events(SDL_Event event)
{
	bool allow_menu_to_process_key = true;		// allow if are broken

	TextEntryMenu *the_instance = TextEntryMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		allow_menu_to_process_key = the_instance->handle_input_events(event);
	}
	
	return allow_menu_to_process_key;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: handle_input_events        | AUTHOR(s): Rob Probin   | DATE STARTED: 25Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
bool TextEntryMenu::handle_input_events(SDL_Event event)
{
	InputLine::process_return_t key_system_return;
	bool allow_menu_to_process_key = false;
	SDLKey key = event.key.keysym.sym;
	
	switch (event.type) {
		
		case SDL_KEYDOWN:
			if(key==SDLK_RETURN)
			{
				allow_menu_to_process_key = true;	// accept
			}
			else if(key==SDLK_ESCAPE)
			{
				allow_menu_to_process_key = true;	// cancel
			}
			else // pass through to the edit box
			{
				key_system_return = textline.process_keypress(event.key.keysym.sym, event.key.keysym.unicode);
				if(key_system_return == textline.FINISHED_ACCEPT)
				{
					accept_description_on_finish = true;
					terminate_menu_mode();
				}
				else if(key_system_return == textline.FINISHED_CANCEL)
				{
					accept_description_on_finish = false;
					terminate_menu_mode();
				}
			}
			break;
			
		case SDL_KEYUP:		// no action here
			if(key==SDLK_RETURN)
			{
				allow_menu_to_process_key = true;	// accept
			}
			else if(key==SDLK_ESCAPE)
			{
				allow_menu_to_process_key = true;	// cancel
			}
			break;
			
		case SDL_QUIT:		// just handle this one just in case
			// this is what the Zex event processing does - it cleans up SDL via atexit().
			exit(0);			// quit immediately
//			request_quit();
			break;
			
		default:	// we can safely ignore other events and assume these are handled by the menu sub-system.
			break;
	} // switch(event.type)
	
	return allow_menu_to_process_key;
}




/* CVS LOG
 *
 * $Log: text_entry_menu.cpp,v $
 * Revision 1.5  2006/08/12 10:38:29  rob
 * Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
 *
 * Revision 1.4  2006/05/18 07:08:29  rob
 * Zex2005: Menu for registration code entry.
 *
 * Revision 1.3  2005/12/16 22:14:15  rob
 * Zex2005: Not enough initialisers for menu - Minor warning removal after -Wall -W
 *
 * Revision 1.2  2005/12/14 21:41:55  rob
 * Zex2005: Removed unused screen reference.
 *
 * Revision 1.1  2005/11/15 22:14:41  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:11:59  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.9  2004/05/01 22:07:29  robp
 * ZD: Fine adjustment to save as text entry dialog positions.
 *
 * Revision 1.8  2004/05/01 22:01:45  robp
 * ZD: Updated mouse boundaries for text entry dialog.
 *
 * Revision 1.7  2004/05/01 21:55:44  robp
 * ZD: (1) Delegated menu key handlers (in this case the save as description entry text line) now return a flag saying whether they want the menu system to handle a specific key press. In this specific case we return "yes please handle" for the return and escape keys. This has the effect as changing the Accept and Cancel "pseudo-icons" and audibly clicking the button - a much better UI feel. The only case this doesn't happen is for the numeric enter key. It is possible to change this over as well (using a dummy menu element entry and a line in the text entry event handler - but it was seen as "not a major problem and not worth it". (2) Also deleted commented out code that was the start of a hand-rolled key repeat system (see previous check-in regarding SDL repeat).
 *
 * Revision 1.6  2004/05/01 21:09:19  robp
 * ZD: Altered comments to be english
 *
 * Revision 1.5  2004/05/01 20:58:45  robp
 * ZD: Added key repeat to save as description dialog with two lines (one to enable, one to disable) - SDL is really cool.
 *
 * Revision 1.4  2004/04/30 21:52:30  robp
 * ZD: Timer code that's not going to be used because I've found the repeat command in the SDL.
 *
 * Revision 1.3  2004/04/29 21:05:12  robp
 * ZD: Added a timer class - based on animation timer - then replaced animation timer with an instance. Then used new class to create a flashing cursor for the saved game description entry box.
 *
 * Revision 1.2  2004/04/29 06:59:10  robp
 * ZD: Corrected project file. Replaced double quotes in user description (it would break the forth parser) and changed the user description to size 40 so that it doesn't overflow the screen.
 *
 * Revision 1.1  2004/04/28 22:38:53  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 *
 *
 */


/*
 *  game_registration_menu.cpp
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "game_registration_menu.h"
#include "menu_driver.h"

#include "get_text_string.h"
#include "zdungeon_printing_emulation.h"

using std::string;

// ***********************************************************************************
// * CONSTANTS 
// *

const int maximum_reg_length = 40;

const int GameRegistrationMenu::CURSOR_FLASH_TIME_IN_MS = 250;

const int NO_FLIP = 0;			// graphic reference - not used in Zex

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static bool GameRegistrationMenu_handle_input_events(SDL_Event event);
static void GameRegistrationMenu_display_text(menu_table_type *this_menu);
static void GameRegistrationMenu_accept_click(menu_table_type *this_menu);
static void GameRegistrationMenu_cancel_click(menu_table_type *this_menu);
static void GameRegistrationMenu_textfield_click(menu_table_type *this_menu);

// ***********************************************************************************
// * THIS MENU DEFINITION
// *


static menu_table_type game_registration_menu[] = 
{
{	// **** MENU TITLE ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  display_item_only, 180,  70, -40, true,	    false,	false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 180, 100, 0, 0, }, 
	// standard             highlight           clicked             selected
	registration_title,   empty_text, 	   empty_text,			empty_text,
	//  display_call   click_call   save_call       run_loop_call    init_call
	NULL,          NULL,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// **** MENU field 1 label ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  display_item_only, 20,  180, -40, true,	    false,	false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 20, 180, 0, 0, }, 
	// standard             highlight           clicked             selected
	registration_name_label,   empty_text, 	   empty_text,			empty_text,
	//  display_call   click_call   save_call       run_loop_call    init_call
	NULL,          NULL,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// **** MENU SUB-TITLE - the description edit box ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  event_item, 20,  220, -40,		false,	    false,		 false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 20, 180, 600, 80, }, 
	// standard             highlight           clicked             selected
	empty_text,      empty_text, 		    empty_text,			empty_text,
	//  display_call                     click_call   save_call       run_loop_call    init_call
	GameRegistrationMenu_display_text,          GameRegistrationMenu_textfield_click,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// **** MENU field 1 label ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  display_item_only, 20,  270, -40, true,	    false,	false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 20, 270, 0, 0, }, 
	// standard             highlight           clicked             selected
	registration_code_label,   empty_text, 	   empty_text,			empty_text,
	//  display_call   click_call   save_call       run_loop_call    init_call
	NULL,          NULL,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// **** MENU SUB-TITLE - the description edit box ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       1,  event_item, 20,  310, -40,		false,	    false,		 false,		-1,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 20, 270, 600, 80, }, 
	// standard             highlight           clicked             selected
	empty_text,      empty_text, 		    empty_text,			empty_text,
	//  display_call                     click_call   save_call       run_loop_call    init_call
	GameRegistrationMenu_display_text,          GameRegistrationMenu_textfield_click,          NULL,             NULL,	     NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

{	// *** SUB OPTION - Cancel [escape] ***
	//  last? user_info  _menu_control_       x   y   hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	false,       0,  event_item,         150,  400, -40, true,    false,	false,   SDLK_ESCAPE,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 150, 400, 200, 40, }, 
	// standard             highlight           clicked             selected
	save_as_dialog_cancel, highlight_char, save_as_dialog_cancel_click,empty_text,
	//  display_call   click_call   		     save_call       run_loop_call    init_call
	NULL,         GameRegistrationMenu_cancel_click,    NULL,             NULL,            NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},


{	// *** SUB OPTION - Accept [return] ***
	//  last? user_info  _menu_control_       x   y hioffset draw_gfx?  alpha_gfx? hilit_alpha? direct_key  unicode_key
	true,       0,  event_item,         400,  400, -40, true,    false,	false,		SDLK_RETURN,	    false,
	//  value  graphics_attrib    mouse rect - x y w h 
	0,		NO_FLIP,          { 400, 400, 235, 40, }, 
	// standard             highlight             clicked                  selected
	save_as_dialog_accept, highlight_char, save_as_dialog_accept_click, empty_text,
	//  display_call   click_call   		          save_call       run_loop_call    init_call
	NULL,               GameRegistrationMenu_accept_click,   NULL,             NULL,            NULL,
	// click sound       hover sound
	//    "",			"",
	// highlighted    original_value   curr state    driver_method, std_gid    hlite_gid  click_gid  selected_gid
	false,		false,  0,		 DRIVER_NONE//,	EMPTY_GID, EMPTY_GID, EMPTY_GID, EMPTY_GID
},

};

// ***********************************************************************************
// * ACTUAL CODE
// *




// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 18 Apr 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

GameRegistrationMenu::GameRegistrationMenu()
: textline(maximum_reg_length), 
textline2(maximum_reg_length), 
accept_description_on_finish(false), 
selected_input_field(0),
cursor_flash_timer(CURSOR_FLASH_TIME_IN_MS)
{
	current_instance = this;
	
	// enable key repeat - notice with key repeat we get multiple key downs and only one key up...
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);			// we ignore return - failure means there will be no repeat in save dialog
	
	run_menu_advanced(game_registration_menu, true, GameRegistrationMenu_handle_input_events);
	
	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);			// disable key repeat before we go further...
	
	current_instance = NULL;	
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 18 Apr 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

string GameRegistrationMenu::get_reg_name()
{
	return textline.str();
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 18 Apr 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

string GameRegistrationMenu::get_reg_code()
{
	return textline2.str();
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: GameRegistrationMenu constructor  | AUTHOR(s): Rob Probin   | DATE STARTED: 22Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

//GameRegistrationMenu::GameRegistrationMenu(int maximum_text_length)
//: textline(maximum_text_length),accept_description_on_finish(false), cursor_flash_timer(CURSOR_FLASH_TIME_IN_MS)
//void GameRegistrationMenu::run_menu()
//{
//}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: accepted                   | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
bool GameRegistrationMenu::user_accepted()
{
	return accept_description_on_finish;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: get_description_c_str      | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: fetch the description string
// +----------------------------------------------------------------ROUTINE HEADER----
//const char *GameRegistrationMenu::get_description_c_str()
//{
//	return textline.c_str();
//}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: get_description_str        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: fetch the description string
// +----------------------------------------------------------------ROUTINE HEADER----
//std::string GameRegistrationMenu::get_description_str()
//{
//	return textline.str();
//}


GameRegistrationMenu *GameRegistrationMenu::current_instance = NULL;		// where we store the instance

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: get_current_instance       | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
GameRegistrationMenu *GameRegistrationMenu::get_current_instance()
{
	return current_instance;
}
// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: GameRegistrationMenu_display_text        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void GameRegistrationMenu_display_text(menu_table_type *this_menu)
{
	GameRegistrationMenu *the_instance = GameRegistrationMenu::get_current_instance();
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
void GameRegistrationMenu::display_text(menu_table_type *this_menu)
{
	int base_x = this_menu->print_location_x;
	int base_y = this_menu->print_location_y;
	static bool cursor_on = true;
	
	if(cursor_flash_timer.frame_update_and_flag())
	{
		cursor_on = !cursor_on;
	}
	
	std::string actual_string;
	if(this_menu->user_defined_data)
	{ actual_string = textline2.str(); }
	else
	{ actual_string = textline.str(); }

	if(cursor_on && selected_input_field == this_menu->user_defined_data)
	{
		actual_string += "_";
	}
	
	print_string(MEDIUM_MENU_WHITE_NO_BG, base_x, base_y, actual_string.c_str());

	// draw underline
	RGBColor c = { 0, 0, 32000 };		// aka dark blue
	RGBForeColor(&c);
	
	MoveTo(this_menu->print_location_x, this_menu->print_location_y+20);
	LineTo(this_menu->print_location_x+100, this_menu->print_location_y+20);

}

// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: GameRegistrationMenu_cancel_click        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void GameRegistrationMenu_cancel_click(menu_table_type *this_menu)
{
	GameRegistrationMenu *the_instance = GameRegistrationMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->cancel_click(this_menu);
	}	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: cancel_click               | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void GameRegistrationMenu::cancel_click(menu_table_type *this_menu)
{
	accept_description_on_finish = false;
	terminate_menu_mode();
}

// +---------------------------------------------+-------------------------+-----------------------
// | TITLE: GameRegistrationMenu_textfield_click | AUTHOR(s): Rob Probin   | DATE STARTED: 17May2006
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----

static void GameRegistrationMenu_textfield_click(menu_table_type *this_menu)
{
	GameRegistrationMenu *the_instance = GameRegistrationMenu::get_current_instance();
	if(the_instance != NULL)		// don't call if not a valid instance
	{
		the_instance->textfield_click(this_menu);
	}	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: textfield_click               | AUTHOR(s): Rob Probin   | DATE STARTED: 17May2006
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void GameRegistrationMenu::textfield_click(menu_table_type *this_menu)
{
	if(this_menu->user_defined_data)
	{
		selected_input_field = 1;
	}
	else
	{
		selected_input_field = 0;
	}
}


// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: GameRegistrationMenu_accept_click        | AUTHOR(s): Rob Probin   | DATE STARTED: 27Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static void GameRegistrationMenu_accept_click(menu_table_type *this_menu)
{
	GameRegistrationMenu *the_instance = GameRegistrationMenu::get_current_instance();
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
void GameRegistrationMenu::accept_click(menu_table_type *this_menu)
{
	accept_description_on_finish = true;
	terminate_menu_mode();
}



// +-------------------------------------------+-------------------------+-----------------------
// | TITLE: GameRegistrationMenu_handle_input_events | AUTHOR(s): Rob Probin   | DATE STARTED: 25Apr2004
// +
// | DESCRIPTION: This acts as a patch between the C and C++ routines. Can be replaced with a
// | protocol (abstract base class) when the menu system is changed to C++.
// +----------------------------------------------------------------ROUTINE HEADER----
static bool GameRegistrationMenu_handle_input_events(SDL_Event event)
{
	bool allow_menu_to_process_key = true;		// allow if are broken
	
	GameRegistrationMenu *the_instance = GameRegistrationMenu::get_current_instance();
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
bool GameRegistrationMenu::handle_input_events(SDL_Event event)
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
			else if(key==SDLK_TAB)
			{
				selected_input_field = 1-selected_input_field;
			}
			else if(key==SDLK_UP)
			{
				selected_input_field = 0;
			}
			else if(key==SDLK_DOWN)
			{
				selected_input_field = 1;
			}
			else // pass through to the edit box
			{
				InputLine* textline_ptr = &textline;
				if(selected_input_field)
				{
					textline_ptr = &textline2;
				}
				
				// actual processing
				key_system_return = textline_ptr->process_keypress(event.key.keysym.sym, event.key.keysym.unicode);
				if(key_system_return == textline_ptr->FINISHED_ACCEPT)
				{
					accept_description_on_finish = true;
					terminate_menu_mode();
				}
				else if(key_system_return == textline_ptr->FINISHED_CANCEL)
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
* $Log: game_registration_menu.cpp,v $
* Revision 1.1  2006/05/18 07:08:12  rob
* Zex2005: Menu for registration code entry.
*
*
*/



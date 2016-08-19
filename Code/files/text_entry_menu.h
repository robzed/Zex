/*
 *  text_entry_menu.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Wed Apr 21 2004.
 *  Copyright (c) 2004 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/12 10:38:32 $
 * $Revision: 1.3 $
 *
 * For log see end of file...
 *
 */

#ifndef TEXT_ENTRY_MENU_H
#define TEXT_ENTRY_MENU_H

#include <string>
#include "timing.h"
#include "input_string_for_menu.h"

//
// This class produces a text entry line type menu
//
class TextEntryMenu
{
public:
	
	// main accessor functions
	TextEntryMenu(int maximum_text_length, const char* old_name);
	std::string get_description_str();
	const char *get_description_c_str();
	bool user_accepted();
	
	// internal menu callback functions
	bool handle_input_events(SDL_Event event);  // public to allow access from menu via c function re-direct
	void display_text(menu_table_type *this_menu);
	void cancel_click(menu_table_type *this_menu);
	void accept_click(menu_table_type *this_menu);
	void clear_click(menu_table_type *this_menu);
	void previous_click(menu_table_type *this_menu);
		
	// c to c++ callback support
	static TextEntryMenu *get_current_instance();

private:
	// constants

	// functions
	
	// data
	InputLine textline;
	bool accept_description_on_finish;
	static TextEntryMenu *current_instance;
	
	MaximumRateTimer cursor_flash_timer;
	static const int CURSOR_FLASH_TIME_IN_MS;
	
	const char* old_name_store;
};


#endif // TEXT_ENTRY_MENU_H

/* CVS LOG
 *
 * $Log: text_entry_menu.h,v $
 * Revision 1.3  2006/08/12 10:38:32  rob
 * Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
 *
 * Revision 1.2  2006/05/18 07:08:36  rob
 * Zex2005: Menu for registration code entry.
 *
 * Revision 1.1  2005/11/15 22:15:04  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:00  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.4  2004/05/01 21:55:45  robp
 * ZD: (1) Delegated menu key handlers (in this case the save as description entry text line) now return a flag saying whether they want the menu system to handle a specific key press. In this specific case we return "yes please handle" for the return and escape keys. This has the effect as changing the Accept and Cancel "pseudo-icons" and audibly clicking the button - a much better UI feel. The only case this doesn't happen is for the numeric enter key. It is possible to change this over as well (using a dummy menu element entry and a line in the text entry event handler - but it was seen as "not a major problem and not worth it". (2) Also deleted commented out code that was the start of a hand-rolled key repeat system (see previous check-in regarding SDL repeat).
 *
 * Revision 1.3  2004/04/30 21:52:31  robp
 * ZD: Timer code that's not going to be used because I've found the repeat command in the SDL.
 *
 * Revision 1.2  2004/04/29 21:05:13  robp
 * ZD: Added a timer class - based on animation timer - then replaced animation timer with an instance. Then used new class to create a flashing cursor for the saved game description entry box.
 *
 * Revision 1.1  2004/04/28 22:38:54  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 *
 *
 */


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
 *  game_file_selector.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Thu Jan 08 2004.
 *  Copyright (c) 2004 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2005/11/15 22:15:07 $
 * $Revision: 1.1 $
 *
 * For log see end of file...
 *
 */


#ifndef _GAME_FILE_SELECTOR_H_
#define _GAME_FILE_SELECTOR_H_

#include "menu_driver.h"
//#include "text_loader.h"
//#include "Surface_Controller.h"
#include "game_file_parser.h"
#include "game_save.h"

class game_file_selector
{
public:
	int save();
	int load();
	game_file_selector();
	static game_file_selector* get_current_instance();
	
	// file selector callbacks
	void print_slot_details(menu_table_type *this_menu);
	void set_dialog_title(menu_table_type *this_menu);
//	void set_save_load_button_title(menu_table_type *this_menu);

	void cancel_click(menu_table_type *this_menu);
	void previous_button_init(menu_table_type *this_menu);
	void previous_button_click(menu_table_type *this_menu);
	void next_button_init(menu_table_type *this_menu);
	void next_button_click(menu_table_type *this_menu);
	void slot_init(menu_table_type *this_menu);
	void slot_click(menu_table_type *this_menu);
	
private:
	enum mode_enum { fs_save, fs_load };

	// private functions
	int file_selector_core();
	menu_table_type* create_menu();
	void remove_menu(menu_table_type* file_selector_dialog);

	// private class data
	//static const menu_table_type menu_template[];
//	static const menu_table_type load_menu_template[];
	static const int NUMBER_SLOTS_PER_MENU = 5;
	static const int NEXT_PREVIOUS_STEP = NUMBER_SLOTS_PER_MENU;	// can alter this to one to step through individually

	// instance data
	int selected_game;
	mode_enum mode;
	
	static bool instance_valid;
	static game_file_selector *current_instance;

	game_file_parser *(games[NUMBER_OF_SAVED_GAME_SLOTS]);

//
	class page_slot_manager
	{
	public:
		page_slot_manager();
		void increase_page_slot(int increase_by_count);
		void decrease_page_slot(int decrease_by_count);
		int return_page_slot_number();
		
	private:
		int page_start_slot_number;
	};
	// a storage container for the current page slot
	page_slot_manager page_slot;
	
};


#endif // _GAME_FILE_SELECTOR_H_

/* CVS LOG
 *
 * $Log: game_file_selector.h,v $
 * Revision 1.1  2005/11/15 22:15:07  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:10  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.8  2004/04/15 22:24:12  robp
 * ZD: Many file selector changes - basically making it a working system - including mouse area, functionality changes, etc.
 *
 * Revision 1.7  2004/04/10 07:36:00  robp
 * ZD: File selector changes to date.
 *
 * Revision 1.6  2004/04/07 21:11:04  robp
 * ZD: Latest file selector changes
 *
 * Revision 1.5  2004/04/07 19:59:04  robp
 * ZD: test checkin
 *
 * Revision 1.4  2004/04/07 19:55:41  robp
 * ZD: File selector Menu change updates
 *
 * Revision 1.3  2004/03/28 16:41:01  robp
 * ZD: Work in progress checkin.
 *
 * Revision 1.2  2004/03/20 14:52:53  robp
 * ZD: Load/Save file selector changes to date for backup.
 *
 * Revision 1.1  2004/02/01 12:00:43  robp
 * More saved game work up to start of file selector.
 *
 *
 *
 *
 */


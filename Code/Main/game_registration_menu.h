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
 *  game_registration_menu.h
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef GAME_REGISTRATION_MENU_H
#define GAME_REGISTRATION_MENU_H

#include "menu_driver.h"
#include "input_string_for_menu.h"
#include <string>
#include "timing.h"

class GameRegistrationMenu
{
public:
	GameRegistrationMenu();
	std::string get_reg_name();
	std::string get_reg_code();

	// whether user accepted text
	bool user_accepted();

	
	//
	// support functions
	//
	// internal menu callback functions
	bool handle_input_events(SDL_Event event);  // public to allow access from menu via c function re-direct
	void display_text(menu_table_type *this_menu);
	void cancel_click(menu_table_type *this_menu);
	void accept_click(menu_table_type *this_menu);
	void textfield_click(menu_table_type *this_menu);
	
	// c to c++ callback support
	static GameRegistrationMenu *get_current_instance();

private:
	InputLine textline;				// name
	InputLine textline2;			// code
	// 
	// support stuff
	//
	bool accept_description_on_finish;
	static GameRegistrationMenu *current_instance;
	int selected_input_field;
	
	MaximumRateTimer cursor_flash_timer;
	static const int CURSOR_FLASH_TIME_IN_MS;
	
};



#endif // GAME_REGISTRATION_MENU_H

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

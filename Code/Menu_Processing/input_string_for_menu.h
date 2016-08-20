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
 *  input_string_for_menu.h
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef INPUT_STRING_FOR_MENU_H
#define INPUT_STRING_FOR_MENU_H


#include <string>

//
// This class manages the keyboard input and makes it into a string
//
class InputLine
{
public:
	// constants
	enum process_return_t { NOT_FINISHED, FINISHED_ACCEPT, FINISHED_CANCEL };
	
	// functions
	InputLine(int max_input_line_size);
	process_return_t process_keypress(SDLKey key, Uint16 unicode_key);
	void erase();					// erase the input string
	void load_string(const std::string& text);	// load the string
	
	std::string str();				// return the string
	const char *c_str();		// return the c style zero terminated string
	
private:
	// constants
		
	// functions
	void add_character(char input_char);
	void remove_character();
	
	int max_input_line_length;
	//	InputLine();
	
	// data
	std::string input_line;
	
};


#endif // INPUT_STRING_FOR_MENU_H

/* CVS LOG
*
* $Log: input_string_for_menu.h,v $
* Revision 1.2  2006/08/12 10:38:42  rob
* Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
*
* Revision 1.1  2006/05/18 07:08:33  rob
* Zex2005: Menu for registration code entry.
*
*
*/

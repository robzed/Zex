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

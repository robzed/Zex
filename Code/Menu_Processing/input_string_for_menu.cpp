/*
 *  input_string_for_menu.cpp
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "input_string_for_menu.h"


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: InputLine constructor      | AUTHOR(s): Rob Probin   | DATE STARTED: 21Apr2004
// +
// | DESCRIPTION: Make an input line
// +----------------------------------------------------------------ROUTINE HEADER----

InputLine::InputLine(int max_size) 
{
	input_line = "";
	max_input_line_length = max_size;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: process_keypress           | AUTHOR(s): Rob Probin   | DATE STARTED: 21Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

InputLine::process_return_t InputLine::process_keypress(SDLKey key, Uint16 unicode_key)
{	
	process_return_t return_signal = NOT_FINISHED;		// we haven't finished by default
	
	if(key==SDLK_RETURN || key==SDLK_KP_ENTER)
	{
		return_signal = FINISHED_ACCEPT;	// finished and accept
	}
	else if(key==SDLK_ESCAPE)
	{
		return_signal = FINISHED_CANCEL;	// finished and cancel
	}
	else if(key==SDLK_BACKSPACE)
	{
		remove_character();
	}
	else // must be character for input stream
	{
		if((unicode_key & 0xff80) == 0 && unicode_key>=' ')	// ensure only printable characters
		{
			add_character(unicode_key);
		}
	}
	
	return return_signal;	
}



// +-----------------------------------+-------------------------+-----------------------
// | TITLE: add_character              | AUTHOR(s): Rob Probin   | DATE STARTED: 21Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void InputLine::add_character(char input_char)
{
	int length = input_line.length();
	
	if(length < max_input_line_length)		// if there is any string...
	{
		input_line += input_char;
	}
	// warn if too long?
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: remove_character           | AUTHOR(s): Rob Probin   | DATE STARTED: 21Apr2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void InputLine::remove_character()
{
	int length = input_line.length();
	
	if(length)		// if there is any string...
	{
		input_line = input_line.substr(0, length-1);
	}
	// warn if empty?
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: erase                      | AUTHOR(s): Rob Probin   | DATE STARTED: 11Aug2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void InputLine::erase()
{
	input_line.erase();
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: load_string                | AUTHOR(s): Rob Probin   | DATE STARTED: 11Aug2004
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void InputLine::load_string(const std::string& text)
{
	input_line = text;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: str                        | AUTHOR(s): Rob Probin   | DATE STARTED: 21Apr2004
// +
// | DESCRIPTION: return the string
// +----------------------------------------------------------------ROUTINE HEADER----
std::string InputLine::str()
{
	return input_line;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: c_str                      | AUTHOR(s): Rob Probin   | DATE STARTED: 21Apr2004
// +
// | DESCRIPTION: return a C string (zero terminated) version of the string
// +----------------------------------------------------------------ROUTINE HEADER----
const char *InputLine::c_str()
{
	return input_line.c_str();
}


/* CVS LOG
*
* $Log: input_string_for_menu.cpp,v $
* Revision 1.2  2006/08/12 10:38:40  rob
* Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
*
* Revision 1.1  2006/05/18 07:08:32  rob
* Zex2005: Menu for registration code entry.
*
*
*/

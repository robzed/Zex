/*
 *  simple_command_interpreter.cpp
 *  Zex
 *
 *  Created by Rob Probin on 30/10/2005.
 *  Copyright 2005 Lightsoft. All rights reserved.
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "simple_command_interpreter.h"



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

simple_command_interpreter::simple_command_interpreter() :
lower_case_arguments(true)
{
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


void simple_command_interpreter::add_command(const char* command_name, function_type_new_args function)
{
	string command = command_name;
	add_command(command, function);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


void simple_command_interpreter::add_command(const string& command_name, function_type_new_args function)
{
	function_composite_type func;
	func.old_arg_type = false;
	func.new_func = function;
	func.old_func = 0;

	commands[command_name] = func;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


void simple_command_interpreter::add_command(const char* command_name, function_type_old_args function)
{
	string command = command_name;
	add_command(command, function);
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


void simple_command_interpreter::add_command(const string& command_name, function_type_old_args function)
{
	function_composite_type func;
	func.old_arg_type = true;
	func.old_func = function;
	func.new_func = 0;
	
	commands[command_name] = func;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


//bool simple_command_interpreter::parse_line(const char* line)
//{
//}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void split_line(string line, vector<string>& arg_list)
{
	arg_list.clear();			// ensure output list is clear
	
	//split up argument list...
	enum search_mode_type { looking_for_word, word_in_progress, in_quotes };

	search_mode_type mode = looking_for_word;
	int word_start = 0;
	
	for(unsigned int i=0; i<line.length(); i++)
	{
		int space_char = isspace(line[i]);
		if(mode == looking_for_word && !space_char)
		{
			if(line[i] == '"') {
				mode = in_quotes;
				word_start = i+1;			// we are at the start of a word ... 
			}
			else { 
				mode = word_in_progress;
				word_start = i;			// we are at the start of a word ... 
			}
			
		}
		else if((mode == word_in_progress && space_char)		// we are consuming a word but we have just went past the end
				|| (mode == in_quotes && line[i] == '"'))
		{
			arg_list.push_back(line.substr(word_start,i-word_start));
			mode = looking_for_word;
		}
	}
	
	// check for words at the end of a line
	if(mode == word_in_progress || mode == in_quotes)
	{
		arg_list.push_back(line.substr(word_start,line.length()));
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool simple_command_interpreter::operator()(const string line)
{
	vector<string> arg_list;
	split_line(line, arg_list);
	
	if(arg_list.size() == 0) return true;		// no arguments to run? then return.
		
	map<string, function_composite_type>::iterator p = commands.find(arg_list[0]);
	if(p != commands.end())	// if argument was found
	{
		if(!(p->second.old_arg_type))
		{
			if(p->second.new_func)
			{
				p->second.new_func(arg_list);		// call the function with a string argument list
			}
			return true;
		}
		else // an old-style argument type function
		{
			// We need to create an old style argument list
			//
			// This consists of:
			//	1. a set of pointers to char*	... arg_list.size() of them
			//  2. a set of char arrays ... arg_list.size()+1 arrays, and each one being string.length(), except 
			// the last that is set to NULL (as per the main() convention)
			//
			// As a side note, this results in string_length() + 1 objects total.
			char**argv = new char*[arg_list.size()+1];		// create the list of pointers to strings plus a NULL
			vector<char*> strings_list;						// (could use argv but don't trust it)
			for(unsigned int i=0; i<arg_list.size(); i++)
			{
				char* string_p = new char[arg_list[i].length()+1];	// +1 is for zero terminator
				arg_list[i].copy(string_p, string::npos);		// copy the string out of the string and into our new argument string entry
				string_p[arg_list[i].length()] = 0;				// terminate string
				strings_list.push_back(string_p);				// populate the deletion vector
				argv[i] = string_p;								// populate the argument vector
			}
			argv[arg_list.size()] = NULL;						// terminate argument vector
			
			// *** THE WORK FUNCTION ***
			if(p->second.old_func)
			{
				p->second.old_func(arg_list.size(), argv);	// call the function with the argument list
			}
			
			// delete all those objects ... but don't trust argv (users might have trashed it!)
			for(unsigned int i=0; i<strings_list.size(); i++)
			{
				delete [] strings_list[i];			// delete each individual string (could use argv but don't trust it)
			}
			delete [] argv;					// and the argument vector itself

			// we successfully executed the command
			return true;
		}
	}
	
	return false;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


//void simple_command_interpreter::make_input_command_arguments_lower_case()
//{
//}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


//void simple_command_interpreter::leave_case_of_input_command_arguments()
//{
//}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


//void simple_command_interpreter::remove_command(const char* command_name)
//{
//}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


//void simple_command_interpreter::remove_command(const string& command_name)
//{
//}


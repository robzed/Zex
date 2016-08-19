/*
 *  simple_command_interpreter.h
 *  Zex
 *
 *  Created by Rob Probin on 30/10/2005.
 *  Copyright 2005 Lightsoft. All rights reserved.
 *
 */


#ifndef _SIMPLE_COMMAND_INTERPRETER_H_

//
// Required includes for this header to import properly
//
#include <string>			// required for string type below
#include <vector>			// required for vector type below
#include <map>

//
// include specific things we use rather than including entire namespace with "using namespace std;"
//
using std::vector;
using std::string;
using std::map;

//
// class definitions
//

// this class interprets a line of text. The command always 
class simple_command_interpreter {
	
public:
	typedef void (*function_type_old_args)(int argc, /*const*/ char* argv[]);
	typedef void (*function_type_new_args)(/*const*/ vector</*const*/ string>& argument_list);
	
	//simple_command_interpreter(const vector<command_table_type1>* command_table);
	//simple_command_interpreter(const vector<command_table_type2>* command_table);
	//simple_command_interpreter(const vector<command_table_type3>* command_table);
	//simple_command_interpreter(const vector<command_table_type4>* command_table);
	simple_command_interpreter();

	// add_command creates a link between a command and a function.
	//
	// these two are the prefered C++ versions - that use a vector of strings to communicate the argument list
	// you can use these two with an object's member functions using the mem_fun() adaptor which is part of <functional>
	void add_command(const char* command_name, function_type_new_args function);
	void add_command(const string& command_name, function_type_new_args function);
	
	// these two are for working more in a C-style. Notice the parameter list is similar to main...
	void add_command(const char* command_name, function_type_old_args function);
	void add_command(const string& command_name, function_type_old_args function);

	// take a line of text and dispatch the command at the beginning
	//bool parse_line(const char* line);
	//bool parse_line(const string line);		// returns success or failure to find command
	bool operator()(const string line);

	// select whether you want case insensitivity or not
	//void make_input_command_arguments_lower_case();	
	// case insensensitive commands, use before
	//void leave_case_of_input_command_arguments();		// ensure the commands are case sensensitive - the default

	// want to remove a command? (To delete all the commands delete the object and re-create)
	//void remove_command(const char* command_name);
	//void remove_command(const string& command_name);
	
private:
	struct function_composite_type {
		bool old_arg_type;
		function_type_old_args old_func;
		function_type_new_args new_func;		
	};
	// some sort of map to find words...
	map<string, function_composite_type /*function_type*/ > commands;
	bool lower_case_arguments;
};



// utility functions
void split_line(string line, vector<string>& arg_list);


#endif // _SIMPLE_COMMAND_INTERPRETER_H_

/*
 *  save_path_destination.cpp
 *  Zex
 *
 *  Created by Rob Probin on 27/03/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "enhanced_string.h"
#include "file_io.h"

#include "save_path_destination.h"

using namespace std;
using namespace enhanced_string;

// return a c string that has the path to the save files
const string Save_Path::get()
{
	return loaded_save_path;
}


// load the save path
Save_Path::Save_Path(const string save_path_filename)
: loaded_save_path("Saved Games") // default the load save path
{	
	// usually "/ZD3/P/save_file_path.txt"
	ifstream input_f(save_path_filename.c_str(), ios::in | ios::binary);
	if(input_f == NULL)
	{
		return; // there was a problem opening the file - return.
	}
	

	if (input_f.good())
	{
		string buffer;

		lsf_getline(buffer, input_f);		// get first line

		// kill leading and trailing spaces, and trailing path seperators, etc.
		strip(buffer);		// remove leading and trailing spaces
		rstrip1(buffer, '"');
		lstrip1(buffer, '"');
		rstrip1(buffer, '/');	// Unix path seperator
		rstrip1(buffer, '\\');	// PC path seperator

		if(buffer.length())
		{
			loaded_save_path = buffer;	// copy the name if not empty
		}
	}
	
	input_f.close();		// close the file that we opened (although this would also be done by the destructor
}

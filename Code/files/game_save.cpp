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
 *  game_save.cpp
 *  Z_dungeon
 *
 *  Created by Rob Probin on Tue Jul 08 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/12 10:38:27 $
 * $Revision: 1.8 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "time.h"
//#include "general_include.h"

#include "game_save.h"
//#include "system_independant_file.h"
//#include "Local_Server.h"
//#include "Local_Object_Server.h"
//#include "Game_Controller.h"

#include "game_file_selector.h"
#include "text_entry_menu.h"

#include "Error_handler.h"
#include "file_io.h"
#include "simple_command_interpreter.h"

#include "load_level.h"

#include <string>
#include <vector>

// ***********************************************************************************
// * CONSTANTS 
// *

#define SAVED_GAMES_FILENAME	"%s/ZexSavedGame_%d.zsl"


// ***********************************************************************************
// * MACROS 
// *

// ***********************************************************************************
// * TYPE DEFS
// *

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

static int current_save_slot=0;
std::string current_save_description = "";

static int current_load_slot=0;

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

static void save_game_to_slot(int slot_num, std::string game_name);

// lower level save routines
static FILE *create_saved_game_file(int saved_slot);
static void close_saved_game_file(FILE *file_id);

static bool load_rooms_from_saved_game(const char* path_filename);


// ***********************************************************************************
// * ACTUAL CODE
// *

/* `/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/

To do / design
     1. file selector
     2. parse files to get names
     3. limited number of saved games (say 10)
     4. save game to current or selected file
     5. save game saves sections to one file
     6. need new_room macro
     7. need new_object macro
     8. save section - save individual rooms
     9. save section - save individual objectives
    10. file selector - name, select, cancel, ... use menu mode to simplify
    11. save as -- warn about overwrite
    12. 10 minute autosave??
    13. backup copy of old save file?
    14. description with each file
    15. save as warns about overwrites
    
*/


// +--------------------------------+-------------------------+-----------------------
// | TITLE: init_game_save_system   | AUTHOR(s): Rob Probin   | DATE STARTED: 14 Apr 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void init_game_save_system(void)
{
current_save_slot=0;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: get_saved_game_path     | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool get_saved_game_path(char *path_filename, int slot)
{
if(slot>NUMBER_OF_SAVED_GAME_SLOTS || slot<1)	// only allow legal slots
  {
  return false;
  }

sprintf(path_filename, SAVED_GAMES_FILENAME, get_saved_file_path().c_str(), slot);	// make filename path

return true;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 9 July 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool load_saved_game() //saved_room_loading_class* room_loader_instance)
{
char path_filename[FILENAME_LENGTH];


if(get_saved_game_path(path_filename, current_load_slot) == false)
  {
  return false;
  }

bool success = /*room_loader_instance->*/load_rooms_from_saved_game(path_filename);


if(success == false)
  {
    report_error_id_non_fatal("*** Game load of saved game failed...",current_load_slot);
	//request_restart_to_main_menu();
  }

current_load_slot = 0;		// make invalid for the next time (i.e. new game next time)
return success;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: load_file_selector      | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Dec 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool load_file_selector(void)
{
	game_file_selector selector;		// make a game_file_selector
	bool load_ok = false;
	
	current_load_slot = selector.load();
	
	if(current_load_slot<=NUMBER_OF_SAVED_GAME_SLOTS && current_load_slot>=1)	// only allow legal slots
	{
		load_ok = true;
	}
	
	return load_ok;		// if load fails...
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: save_game_as            | AUTHOR(s): Rob Probin   | DATE STARTED: 8 July 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void save_game_as(void)
{
	game_file_selector selector;		// make a game_file_selector
	
	current_save_slot = selector.save();
	
	if(current_save_slot<=NUMBER_OF_SAVED_GAME_SLOTS && current_save_slot>=1)	// only allow legal slots
	{
		game_file_parser* gamedata = new game_file_parser(current_save_slot);		// create one that has the name in
		
//		TextEntryMenu test;					// same as next one (just included when I was testing classes)
		TextEntryMenu save_description  = TextEntryMenu(SAVED_FILE_USER_DESCRIPTION_MAX, gamedata->name());
		
		if(save_description.user_accepted())
		{   
			current_save_description = save_description.get_description_str();		// copy it to our own string (for save not as)
			save_game_to_slot(current_save_slot,current_save_description.c_str());
		}
		
		delete gamedata;
	}
	
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 8 July 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void save_game(void)
{
if(current_save_slot>NUMBER_OF_SAVED_GAME_SLOTS || current_save_slot<1)	// only allow legal slots
  {
  save_game_as();
  }
else
  {
  save_game_to_slot(current_save_slot,current_save_description.c_str());
  }
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 8 July 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
#include "3d_structures_def.h"

#define FILE_NEWLINE "\x0d\x0a"

static void save_game_to_slot(int slot_num, std::string game_name)
{
	FILE* save_file_id;
	FILE *save_file_ptr;
	time_t mytime;
	char time_string[SAVED_FILE_DATE_STRING_MAX+1];
	
	save_file_id = create_saved_game_file(slot_num);	// open a file for saving
	
	if(save_file_id)
	{
		save_file_ptr = save_file_id;
		//save_file_ptr = lsf_get_system_file_pointer(save_file_id);
		if(save_file_ptr)
		{
			// this needs to be the first token in the file for it to parse or load
			fprintf(save_file_ptr,"Zex1SaveGame" FILE_NEWLINE);

			// file header information is for reference only
			fprintf(save_file_ptr,"// Original Slot %d" FILE_NEWLINE,slot_num);

			// get rid of those pesky double quotes that will kill our interpreter
			std::string::size_type pos;
			while(game_name.npos != (pos = game_name.find("\"")))
			{
				game_name = game_name.replace(pos,1,"'");		// replace with single quotes
			}
			fprintf(save_file_ptr,"game_name \"%s\"" FILE_NEWLINE,game_name.c_str());
			time(&mytime);
			if(0==strftime(time_string, SAVED_FILE_DATE_STRING_MAX, "%a %b %d %Y %X", localtime(&mytime)))
			{
				time_string[SAVED_FILE_DATE_STRING_MAX] = 0;  // ensure the time is zero terminated
			}
			
			fprintf(save_file_ptr,"save_date \"%s\"" FILE_NEWLINE, time_string);
			//save_player_summary(save_file_ptr);

			// mothership id
			extern int docked_at;
			extern	DynObjectsFixedSize *ocb_ptr;

			unsigned long docked_at_mothership_id = (*ocb_ptr).object_list[docked_at].mship_ptr->callsign;
			signed char id[4];
			id[0] = 0xff & (docked_at_mothership_id >> 24); if(id[0]<33) { id[0] = '?'; }
			id[1] = 0xff & (docked_at_mothership_id >> 16); if(id[1]<33) { id[1] = '?'; }
			id[2] = 0xff & (docked_at_mothership_id >>  8); if(id[2]<33) { id[2] = '?'; }
			id[3] = 0xff & (docked_at_mothership_id      ); if(id[3]<33) { id[3] = '?'; }
			fprintf(save_file_ptr,"mothership_id %c%c%c%c" FILE_NEWLINE, id[0], id[1], id[2], id[3]);		// save the mothership id
			
			extern int difficulty;
			fprintf(save_file_ptr,"difficulty_copy %i" FILE_NEWLINE, difficulty);		// save difficulty
			extern int gRank;
			fprintf(save_file_ptr,"rank_copy %i" FILE_NEWLINE, gRank+1000);		// save rank (offset from 1000)

			// line 4 data
			extern int score;
			fprintf(save_file_ptr,"score_copy %i" FILE_NEWLINE, score+1000);		// save score (offset from 1000)
			extern int promotion_points;
			fprintf(save_file_ptr,"promotion_points_copy %i" FILE_NEWLINE, promotion_points+1000);		// save pp (offset from 1000)
			extern int credits;
			fprintf(save_file_ptr,"credits_copy %i" FILE_NEWLINE, credits+1000);		// save credits (offset from 1000)
			
			// architecture of game
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
			fprintf(save_file_ptr,"endian little" FILE_NEWLINE);
			}
			else if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				fprintf(save_file_ptr,"endian big" FILE_NEWLINE);
			}
			else { report_error_c("Endianess not defined on save!","",0); }

			fprintf(save_file_ptr,"save_version %i" FILE_NEWLINE, return_save_version());
			
			fprintf(save_file_ptr,"end_saved_game_header" FILE_NEWLINE); // this line is required to get a file-list!
			fprintf(save_file_ptr, FILE_NEWLINE);	// data must immediately follow this file newline.
			
			append_save_game_data(save_file_ptr);
			// save the individual game save sections
			//save_rooms(save_file_ptr);
			//save_objects(save_file_ptr);
			//fprintf(save_file_ptr,"\\ End of Saved Game File" FILE_NEWLINE);
		}
		
		close_saved_game_file(save_file_id);
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 8 July 05
// +
// | DESCRIPTION: Zex port of Save system - undoes the header and passes the data back to Zex
// +----------------------------------------------------------------ROUTINE HEADER----

static bool load_rooms_from_saved_game(const char* path_filename)
{
	using namespace std;
	
	FILE* file = fopen(path_filename, "rb");
	string line;
	vector<string> output;

	if(!file) return false;		// couldn't open file

	// check for initial marker...
	lsf_getline(line, file);
	split_line(line, output);
	if(output.size() == 0 || output[0] != "Zex1SaveGame") { fclose(file); return false; }

	// load the header first....
	while(!feof(file) && !ferror(file))
	{
		lsf_getline(line, file);
		split_line(line, output);
		if(output.size() >= 1 && output[0] == "end_saved_game_header")
		{
			break;
		}
	}
	
	if(feof(file) || ferror(file))
	{
		fclose(file); return false;
	}

	bool status = false;
	
	int c1 = fgetc(file);
	int c2 = fgetc(file);
	if(c1 == 13 && c2 == 10)			// must be this combination of characters...
	{
		// load the main game...
		if(load_core_game_data(file)) {
			status = true; }
		else {
			status = false; }
	}

	fclose(file);
	return status;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 8 July 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


static FILE* create_saved_game_file(int saved_slot)
{
FILE* file_id;
char path_filename[FILENAME_LENGTH];

if(get_saved_game_path(path_filename, saved_slot) == false)
  {
  return 0;
  }

file_id = fopen(path_filename, "wb");	// delete old file, make new one
return file_id;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 8 July 03
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void close_saved_game_file(FILE* file_id)
{
  fclose(file_id);				// finished changing saved data
}

//void file_selector(void)
//{
//}



/* CVS LOG
 *
 * $Log: game_save.cpp,v $
 * Revision 1.8  2006/08/12 10:38:27  rob
 * Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
 *
 * Revision 1.7  2006/08/10 22:50:20  rob
 * Zex2005: Extra save metadata - score, credits, promoition points.
 *
 * Revision 1.6  2006/04/01 23:17:22  rob
 * Zex2005: Rather trivial change to allow an optional "save_file_path.txt" file in the P folder to define where saved game data (games, prefs, keys) go. Normally they all default to the "Saved Games" folder. Includes 2 new C files and 2 headers.
 *
 * Revision 1.5  2006/02/24 21:11:05  rob
 * Zex2005: Added rank to file selector dialog.
 *
 * Revision 1.4  2006/02/21 23:10:20  rob
 * Zex2005: Added level to game fileselector (for new saves). Should be full compatible with previous saved games.
 *
 * Revision 1.3  2005/12/29 23:25:36  rob
 * Zex2005: Added mothership reference in file selector. Also warns about different architecture and about incompatible version in file selector. Will still attempt to load therefore loading routines will give normal failed warnings in these cases.
 *
 * Revision 1.2  2005/11/26 21:27:58  rob
 * Zex2005: Removed unused parameter from printf.
 *
 * Revision 1.1  2005/11/15 22:14:45  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:02  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.10  2004/04/29 06:59:10  robp
 * ZD: Corrected project file. Replaced double quotes in user description (it would break the forth parser) and changed the user description to size 40 so that it doesn't overflow the screen.
 *
 * Revision 1.9  2004/04/28 22:38:54  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 * Revision 1.8  2004/04/17 16:35:10  robp
 * ZD: General debug option changed to const BOOL.
 *
 * Revision 1.7  2004/04/15 22:27:26  robp
 * ZD: Now properly causes game load and save. Also included new init routine.
 *
 * Revision 1.6  2004/03/28 16:41:01  robp
 * ZD: Work in progress checkin.
 *
 * Revision 1.5  2004/02/01 12:00:43  robp
 * More saved game work up to start of file selector.
 *
 * Revision 1.4  2003/12/24 21:54:12  robp
 * Structural changes to allow game load to take place at any point in the game via pause. Also changed load menu entries to perform correct actions.
 *
 * Revision 1.3  2003/12/12 17:24:30  robp
 * Name change of saved game file
 *
 * Revision 1.2  2003/09/20 18:02:13  robp
 * *** NAME CHANGE OF ALL C FILES FROM .c TO .cpp *** 
Before this point you will need to rename the files back to .c (i.e. remove the pp from .cpp) to build older versions.
 *
 * Revision 1.1  2003/08/23 15:03:39  robp
 * Added start of save code
 *
 *
 *
 *
 */


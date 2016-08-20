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
 *  game_file_parser.cpp
 *  Z_dungeon
 *
 *  Created by Rob Probin on Thu Jan 08 2004.
 *  Copyright (c) 2004 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/12 10:38:38 $
 * $Revision: 1.9 $
 *
 * For log see end of file...
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "menu_driver.h"

//#include "general_include.h"

#include "game_file_parser.h"
#include "game_save.h"
//#include "z_dungeon_interpreter_link.h"
//#include "Game_Controller.h"
//#include "system_independant_file.h"

//#include "fonts.h"

//#include "text_loader.h"
#include "map_control.h"


#include <string>
#include <sstream>
#include "file_io.h"

#include "Error_handler.h"

#include <istream>
#include "simple_command_interpreter.h"

#include "inventory.h"
#include "get_text_string.h"
#include "zdungeon_printing_emulation.h"

// ***********************************************************************************
// * CONSTANTS 
// *

// ***********************************************************************************
// * MACROS 
// *

// ***********************************************************************************
// * TYPE DEFS
// *

// ***********************************************************************************
// * VARIABLE DECLARATIONS 
// *

game_file_parser* game_file_parser::currently_loading_instance = NULL;  // not in this mode initially

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *

// ***********************************************************************************
// * ACTUAL CODE
// *


// +--------------------------------+-------------------------+-----------------------
// | TITLE: game_file_parser        | AUTHOR(s): Rob Probin   | DATE STARTED: 8 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

game_file_parser::game_file_parser(int game_slot_number)
{
char path_filename[FILENAME_LENGTH];

slot_number = game_slot_number;
game_present_var = false;

// blank unknown variables
user_description[0] = 0;
time_game_saved[0] = 0;
num_players = 0;
room_name_p1 = 0;
room_name_p2 = 0;
character1_level = 0;
character2_level = 0;

endian_known = false;
save_game_version_known = false;
mothership_id_known = false;
difficulty_known = false;
rank_known = false;

extended_info_known = false;
score=0;
promotion_points=0;
credits=0;

if(get_saved_game_path(path_filename, game_slot_number))
  {
  if(does_file_exist(path_filename, "rb"))
    {
    game_file_parser::currently_loading_instance = this;
    game_present_var = load_header(path_filename);
    game_file_parser::currently_loading_instance = NULL;
	}
  }
else	// invalid save path - probably incorrect slot number
  {
  
  }
  

}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: load_header             | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Oct 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
static void create_initial_game_load_words(simple_command_interpreter &interpreter_ref);
static void create_proper_game_load_words(simple_command_interpreter &interpreter_ref);

static int file_terminated = 0;
static simple_command_interpreter* parser_copy = 0;		// very bad hack
static game_file_parser* current_instance = 0;			// very bad hack

bool game_file_parser::load_header(char *file_name)
{
	//using namespace std;

	//ifstream input_f(file_name, ios::in | ios::binary);	
	
	FILE* input_f = fopen(file_name,"rb");
	//if(! input_f.is_open()) 
	if(input_f == NULL)
	{
		return false;  // there was a problem opening it - return false.
	}

	string buffer;
	//const int MAX_LENGTH_OF_LINE = 200;
	
	/*
	if(! input_f.eof())
	{
		input_f.getline(buffer, MAX_LENGTH_OF_LINE);
		if(buffer != "Zex1SaveGame")
		{
			return false;			// bad if we haven't got this marker
		}
	}
	*/
	
	file_terminated = 0;
	simple_command_interpreter parser;
	parser_copy = &parser;
	current_instance = this;
	
	create_initial_game_load_words(parser);
	
	bool command_found = true;
	//while (! input_f.eof() && file_terminated == 0)
	while (! feof(input_f) && ! ferror(input_f) && file_terminated == 0 && command_found==true)
	{
		lsf_getline(buffer, input_f);
		command_found = parser(buffer);
	}

	if(command_found == false)
	{
		file_terminated = 2;	// unknown commands are bad
	}
	
	fclose(input_f);		// close the file that we opened
	parser_copy = 0;
	current_instance = 0;

	if(file_terminated == 1)		// return of 1 means finished ok
	{
		return true;			// looks ok
	}
	else
	{
		return false;			// looks bad (file_terminated == 2)
	}
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


static void comment(int argc, char* argv[])
{
	// ignore this command
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void start_save_game_marker(int argc, char* argv[])
{
	if(parser_copy)
	{
		create_proper_game_load_words(*parser_copy);
	}
	else
	{
		file_terminated = 2;
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void game_name(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_game_name(argv[1], strlen(argv[1]));
	}
	else
	{
		file_terminated = 2;
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: save_date               | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void save_date(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_save_date(argv[1], strlen(argv[1]));
	}
	else
	{
		file_terminated = 2;
	}
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Dec 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void endian(vector<string> &arguments)
{
	if(arguments.size()==2 && current_instance)
	{
		if(arguments[1]=="big")
		{
			current_instance->set_endian(true);
		}
		else if(arguments[1]=="little")
		{
			current_instance->set_endian(false);
		}
		else
		{
			file_terminated = 2;
		}
	}
	else
	{
		file_terminated = 2;
	}	
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Dec 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void save_version(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_save_version(atoi(argv[1]));		// zero returned if no conversion possible
	}
	else
	{
		file_terminated = 2;
	}	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Dec 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void mothership_id(int argc, char* argv[])
{
	if(argc==2 && current_instance && strlen(argv[1]) == 4)
	{
		current_instance->set_mothership_id(argv[1]);
	}
	else
	{
		file_terminated = 2;
	}	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Feb 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void difficulty_copy(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_difficulty(atoi(argv[1]));		// zero returned if no conversion possible
	}
	else
	{
		file_terminated = 2;
	}	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 24 Feb 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void rank_copy(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_rank(atoi(argv[1])-1000);		// zero returned if no conversion possible
	}
	else
	{
		file_terminated = 2;
	}	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Aug 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void score_copy(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_score(atoi(argv[1])-1000);		// zero returned if no conversion possible
	}
	else
	{
		file_terminated = 2;
	}	
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Aug 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void promotion_points_copy(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_promotion_points(atoi(argv[1])-1000);		// zero returned if no conversion possible
	}
	else
	{
		file_terminated = 2;
	}	
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Aug 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void credits_copy(int argc, char* argv[])
{
	if(argc==2 && current_instance)
	{
		current_instance->set_credits(atoi(argv[1])-1000);		// zero returned if no conversion possible
	}
	else
	{
		file_terminated = 2;
	}	
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

static void end_saved_game_header(int argc, char* argv[])
{
	file_terminated = 1;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


static void create_initial_game_load_words(simple_command_interpreter& interpreter_ref)
{
	interpreter_ref.add_command("//",comment);
	interpreter_ref.add_command("Zex1SaveGame",start_save_game_marker);
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Nov 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----


static void create_proper_game_load_words(simple_command_interpreter &interpreter_ref)
{
	interpreter_ref.add_command("game_name",game_name);
	interpreter_ref.add_command("save_date",save_date);
	interpreter_ref.add_command("endian",endian);
	interpreter_ref.add_command("save_version",save_version);
	interpreter_ref.add_command("mothership_id",mothership_id);
	interpreter_ref.add_command("end_saved_game_header",end_saved_game_header);	
	interpreter_ref.add_command("difficulty_copy",difficulty_copy);
	interpreter_ref.add_command("rank_copy",rank_copy);
	interpreter_ref.add_command("score_copy",score_copy);
	interpreter_ref.add_command("promotion_points_copy",promotion_points_copy);
	interpreter_ref.add_command("credits_copy",credits_copy);
}


//int game_file_parser::parse_line(input_line)
//{
//	
//}


	
	
// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 11 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool game_file_parser::is_header_read()
{
if(currently_loading_instance) { return true; }
else { return false; }
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
game_file_parser *game_file_parser::load_instance()
{
// we should never get here ... but still...
if(currently_loading_instance==0)  { report_error_c("There has been an internal error with the game file parser (1912b)","Game file parser error: No game parser at this point!",0); }

return currently_loading_instance;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_parser::set_game_name(const char *string, int len)
{
	if(len > SAVED_FILE_USER_DESCRIPTION_MAX) { len = SAVED_FILE_USER_DESCRIPTION_MAX; }
	
	int index = 0;
	while(len)
	{
		user_description[index] = *string;
		string++; index++; len--;
	}
	
	user_description[index] = 0;
}



// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Dec 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_endian(bool big_endian)
{
	endian_known = true;
	big_endian_flag = big_endian;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Dec 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_save_version(int version)
{
	save_game_version_known = true;
	save_game_version = version;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 29 Dec 05
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_mothership_id(const char id[4])
{
	// verify it's ok and copy it to our storage
	for(int i=0; i<4; i++)
	{
		if(id[i] < 32) return;
		mothership_id[i] = id[i];
	}
	mothership_id[4] = 0;		// make it a c string
	
	mothership_id_known = true;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Aug 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_score(int score_in)
{
	score = score_in;
	extended_info_known = true;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Aug 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_promotion_points(int promotion_points_in)
{
	promotion_points = promotion_points_in;
	extended_info_known = true;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 10 Aug 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_credits(int credits_in)
{
	credits = credits_in;
	extended_info_known = true;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Feb 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_difficulty(int difficulty_level)
{
	difficulty = difficulty_level;
	
	difficulty_known = true;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 21 Feb 06
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_rank(int file_rank)
{
	rank = file_rank;
	
	rank_known = true;
}

//
//
//
int game_file_parser::get_game_rank()
{
	if(rank_known)
	{
		return rank;
	}
	else
	{
		return 0;		// equivalent to ENS
	}
}
// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_parser::set_save_date(const char *string, int len)
{
	if(len > SAVED_FILE_DATE_STRING_MAX) { len = SAVED_FILE_DATE_STRING_MAX; }
	
	int index = 0;
	while(len)
	{
		time_game_saved[index] = *string;
		string++; index++; len--;
	}
	
	time_game_saved[index] = 0;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_num_players(int num_players_in)
{
num_players = num_players_in;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_player_room_names(int p1_room, int p2_room)
{
room_name_p1 = p1_room;
room_name_p2 = p2_room;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

void game_file_parser::set_player_levels(int p1_level, int p2_level)
{
character1_level = p1_level;
character2_level = p2_level;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE:                         | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Jan 04
// +
// | DESCRIPTION: 
// +----------------------------------------------------------------ROUTINE HEADER----

bool game_file_parser::game_present()
{
return game_present_var;
}


// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_line1				| AUTHOR(s): Rob Probin   | DATE STARTED: 12 Apr 06
// +
// | DESCRIPTION: Returns the game name for this slot
// +----------------------------------------------------------------ROUTINE HEADER----
const char* game_file_parser::name()
{
	return user_description;
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_line1				| AUTHOR(s): Rob Probin   | DATE STARTED: 12 Apr 04
// +
// | DESCRIPTION: Prints the first line of the file selector
// +----------------------------------------------------------------ROUTINE HEADER----
// slot 0 (start+0)  <user description>  <save date/time>			-> 1 item (composite item)
// Player 1 - Level: <number> Room: <roomname>
// single player (OR if 2 player game)  p2 character level, p2 room name

void game_file_parser::print_line1(int font, int x, int y)
{
	std::ostringstream output_string;
	
	if(strlen(user_description)==0)
	{
		output_string << get_text(file_dialog_no_description);
	}
	else
	{
		output_string << user_description;
	}
	
//	output_string << " - " << time_game_saved;
	print_string(font, x, y, output_string.str().c_str());
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_line2             | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Apr 04
// +
// | DESCRIPTION: Prints the second line of the file selector
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_parser::print_line2(int font, int x, int y)
{
	std::ostringstream output_string;

	output_string << get_text(file_dialog_time_and_date) << ": " << time_game_saved;
//	output_string << get_text(file_dialog_player_1_text) << character1_level << " - ";
//	output_string << get_text(file_dialog_room_text) << get_text(room_name_p1);
	
	if(difficulty_known)
	{
		switch(difficulty)
		{
			case 1:
				output_string << " - Level: Master Zexician";
				break;
			case 8:
				output_string << " - Level: Zexician";
				break;
			case 16:
				output_string << " - Level: Space Cadet";
				break;
				
			// no default case ... we don't print anything
		}
	}

	if(rank_known) output_string << " - Rank:" << rank_to_string(rank);	

	print_string(font, x, y, output_string.str().c_str());
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_line3             | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Apr 04
// +
// | DESCRIPTION: Prints the third line of the file selector
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_parser::print_line3(int font, int x, int y)
{
	std::ostringstream output_string;
	
	
	//if(game_type == )
	//output_string << file_dialog_powerpc_game;
	//output_string << file_dialog_intel_x86_game;
	output_string << get_text(file_dialog_zex_saved_game);

	if(mothership_id_known)
	{
		output_string << " - " << "Mothership " << mothership_id;
	}
	
	
	if(endian_known)
	{
		if(big_endian_flag)
		{
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
				output_string << " - " << "PowerPC game" << " (INCOMPATIBLE)";
			}
		}
		else
		{
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				output_string << " - " << "Intel x86 game" << " (INCOMPATIBLE)";
			}
		}
	}
	
	
	if(save_game_version_known)
	{
		if(save_game_version != return_save_version())
		{
			output_string << " - INCOMPATIBLE SAVE VERSION";
		}
	}
	
	
	print_string(font, x, y, output_string.str().c_str());

	
	/*
	if(num_players == 2)
	{
		output_string << get_text(file_dialog_player_2_text) << character2_level << " - ";
		output_string << get_text(file_dialog_room_text) << get_text(room_name_p2);
		print_string(font, x, y, output_string.str().c_str());
	}
	else if(num_players == 1)
	{
		output_string << get_text(file_dialog_single_player_text);
		print_string(font, x, y, output_string.str().c_str());
	}*/
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: print_line4             | AUTHOR(s): Rob Probin   | DATE STARTED: 12 Apr 04
// +
// | DESCRIPTION: Prints the fourth line of the file selector
// +----------------------------------------------------------------ROUTINE HEADER----
void game_file_parser::print_line4(int font, int x, int y)
{
	std::ostringstream output_string;
		
	if(extended_info_known)
	{
		output_string << "Score " << score;
		output_string << " - " << "Promotion points " << promotion_points;
		
		if(rank_known)
		{
			int next_pp = return_promotion_points_for_rank(rank);
			if(next_pp)
			{
				output_string << " / " << next_pp;
			}
		}
		
		output_string << " - " << "Credits " << credits;
	}
	
	
	print_string(font, x, y, output_string.str().c_str());

}


/* CVS LOG
 *
 * $Log: game_file_parser.cpp,v $
 * Revision 1.9  2006/08/12 10:38:38  rob
 * Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
 *
 * Revision 1.8  2006/08/10 22:50:07  rob
 * Zex2005: Extra save metadata - score, credits, promoition points.
 *
 * Revision 1.7  2006/06/25 16:09:07  rob
 * Zex2005: Don't know whether we need this - but here is some code that limits file loading to below captain on the unregistered version.
 *
 * Revision 1.6  2006/05/18 07:08:51  rob
 * Zex2005: Menu for registration code entry.
 *
 * Revision 1.5  2006/04/01 23:13:37  rob
 * Zex2005: Comment out unused line.
 *
 * Revision 1.4  2006/02/24 21:11:02  rob
 * Zex2005: Added rank to file selector dialog.
 *
 * Revision 1.3  2006/02/21 23:10:13  rob
 * Zex2005: Added level to game fileselector (for new saves). Should be full compatible with previous saved games.
 *
 * Revision 1.2  2005/12/29 23:25:39  rob
 * Zex2005: Added mothership reference in file selector. Also warns about different architecture and about incompatible version in file selector. Will still attempt to load therefore loading routines will give normal failed warnings in these cases.
 *
 * Revision 1.1  2005/11/15 22:14:32  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:02  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.3  2004/04/28 22:38:53  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 * Revision 1.2  2004/04/15 22:26:08  robp
 * ZD: Added file parser ability to print entry, corrected error in description name and time storage and stopped warnings to console on missing files.
 *
 * Revision 1.1  2004/02/01 12:00:44  robp
 * More saved game work up to start of file selector.
 *
 *
 *
 *
 */


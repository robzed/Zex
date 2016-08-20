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
 *  game_file_parser.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Thu Jan 08 2004.
 *  Copyright (c) 2004 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2006/08/12 10:38:35 $
 * $Revision: 1.7 $
 *
 * For log see end of file...
 *
 */


#ifndef _GAME_FILE_PARSER_H_
#define _GAME_FILE_PARSER_H_

#include <string>

const int SAVED_FILE_DATE_STRING_MAX=50;				// the maximum buffer size for the date (and also for date production and loading)
const int SAVED_FILE_USER_DESCRIPTION_MAX=40;			// how long can names be set and also show up on loading? (ensure W doesn't overflow the description and fileselector screens)
 

class game_file_parser
{
public:
	// data accessing
	bool game_present();
	void print_line1(int font, int x, int y);
	void print_line2(int font, int x, int y);
	void print_line3(int font, int x, int y);
	void print_line4(int font, int x, int y);
	const char* name();					// returns the game name for this slot
	
	game_file_parser(int game_slot_number);

	// transmit data back to us
	void set_game_name(const char *string, int len);
	void set_save_date(const char *string, int len);
	void set_num_players(int num_players);
	void set_player_room_names(int p1_room, int p2_room);
	void set_player_levels(int p1_level, int p2_level);

	void set_endian(bool big_endian);
	void set_save_version(int version);
	void set_mothership_id(const char id[4]);
	void set_difficulty(int difficulty_level);
	void set_rank(int file_rank);
	
	// line 4 stuff
	void set_score(int score_in);
	void set_promotion_points(int promotion_points_in);
	void set_credits(int credits_in);
		
	// finding the one currently being loaded - these are used by the interpreter
	static bool is_header_read();
	static game_file_parser *load_instance();

	int get_game_rank();
	
private:
	// data from header
	char user_description[SAVED_FILE_USER_DESCRIPTION_MAX+1];
	char time_game_saved[SAVED_FILE_DATE_STRING_MAX+1];
	int num_players;
	int room_name_p1;
	int room_name_p2;
	int character1_level;
	int character2_level;
	// these weren't present on a previous version...
	bool endian_known;
	bool big_endian_flag;
	bool save_game_version_known;
	int save_game_version;
	bool mothership_id_known;
	char mothership_id[5];
	bool difficulty_known;
	int difficulty;
	bool rank_known;
	int rank;

	// line 4 data
	bool extended_info_known;
	int score, promotion_points, credits;
	
	// control variables
	int slot_number;
	bool game_present_var;
	static game_file_parser* currently_loading_instance;
	
	// functions
	bool load_header(char *file_name);
};


#endif // _GAME_FILE_PARSER_H_

/* CVS LOG
 *
 * $Log: game_file_parser.h,v $
 * Revision 1.7  2006/08/12 10:38:35  rob
 * Zex2005: Save file user description dialog has 'recall previous' and 'clear' buttons.
 *
 * Revision 1.6  2006/08/10 22:50:16  rob
 * Zex2005: Extra save metadata - score, credits, promoition points.
 *
 * Revision 1.5  2006/06/25 16:09:12  rob
 * Zex2005: Don't know whether we need this - but here is some code that limits file loading to below captain on the unregistered version.
 *
 * Revision 1.4  2006/02/24 21:11:08  rob
 * Zex2005: Added rank to file selector dialog.
 *
 * Revision 1.3  2006/02/21 23:10:17  rob
 * Zex2005: Added level to game fileselector (for new saves). Should be full compatible with previous saved games.
 *
 * Revision 1.2  2005/12/29 23:25:45  rob
 * Zex2005: Added mothership reference in file selector. Also warns about different architecture and about incompatible version in file selector. Will still attempt to load therefore loading routines will give normal failed warnings in these cases.
 *
 * Revision 1.1  2005/11/15 22:14:54  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:01  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.5  2004/04/29 21:05:15  robp
 * ZD: Added a timer class - based on animation timer - then replaced animation timer with an instance. Then used new class to create a flashing cursor for the saved game description entry box.
 *
 * Revision 1.4  2004/04/29 06:59:09  robp
 * ZD: Corrected project file. Replaced double quotes in user description (it would break the forth parser) and changed the user description to size 40 so that it doesn't overflow the screen.
 *
 * Revision 1.3  2004/04/28 22:38:52  robp
 * ZD: Changes for the save dialog - mostly the save as description dialog box - partially implemented with some clean up required. Also some minor constant (from #defines to const int) also located in their native homes rather than in the general_include.h.
 *
 * Revision 1.2  2004/04/15 22:26:09  robp
 * ZD: Added file parser ability to print entry, corrected error in description name and time storage and stopped warnings to console on missing files.
 *
 * Revision 1.1  2004/02/01 12:00:44  robp
 * More saved game work up to start of file selector.
 *
 *
 *
 *
 */


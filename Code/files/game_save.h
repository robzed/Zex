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
 *  game_save.h
 *  Z_dungeon
 *
 *  Created by Rob Probin on Tue Jul 08 2003.
 *  Copyright (c) 2003 Lightsoft. All rights reserved.
 *
 * $Author: rob $
 * $Date: 2005/11/15 22:14:35 $
 * $Revision: 1.1 $
 *
 * For log see end of file...
 *
 */
#ifndef _GAME_SAVE_H_
#define _GAME_SAVE_H_

//#include "Local_Server.h"

void init_game_save_system(void);

void save_game(void);
void save_game_as(void);
bool load_saved_game(); //saved_room_loading_class* room_loader_instance);
bool load_file_selector(void);

bool get_saved_game_path(char *path_file, int slot);

const int NUMBER_OF_SAVED_GAME_SLOTS = 15;

// more Z_dungeon emulation
const int FILENAME_LENGTH = 255;			// some big number. Overrun unlike...


#endif // _GAME_SAVE_H_

/* CVS LOG
 *
 * $Log: game_save.h,v $
 * Revision 1.1  2005/11/15 22:14:35  rob
 * Zex2005: Added new files for game file selector in Zex.
 *
 * Revision 1.1  2005/01/01 14:12:10  robp
 * ZD: Moved files from individual directories under root directory to a source directory.
 *
 * Revision 1.5  2004/04/15 22:27:26  robp
 * ZD: Now properly causes game load and save. Also included new init routine.
 *
 * Revision 1.4  2004/04/07 19:55:42  robp
 * ZD: File selector Menu change updates
 *
 * Revision 1.3  2004/02/01 12:00:50  robp
 * More saved game work up to start of file selector.
 *
 * Revision 1.2  2003/12/24 21:54:13  robp
 * Structural changes to allow game load to take place at any point in the game via pause. Also changed load menu entries to perform correct actions.
 *
 * Revision 1.1  2003/08/23 15:03:57  robp
 * Added start of save code
 *
 *
 *
 *
 */




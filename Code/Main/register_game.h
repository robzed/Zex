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
 *  register_game.h
 *  Zex
 *
 *  Created by Rob Probin on 17/05/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef REGISTER_GAME_H
#define REGISTER_GAME_H

void handle_register_game();
void check_game_parameters(void);			// fake name for what is a blacklist check
void check_game_param_slowly(void);			// background blacklist check

#endif // REGISTER_GAME_H

/* CVS LOG
*
* $Log: register_game.h,v $
* Revision 1.3  2006/06/25 11:16:10  rob
* Zex2005: Game has now got a blacklist list which can be used to remove rogue serial numbers flying around the internet when we update the application.
*
* Revision 1.2  2006/05/20 10:36:21  stu
* Zex2005: Code: Registration in progress
*
* Revision 1.1  2006/05/18 07:08:26  rob
* Zex2005: Menu for registration code entry.
*
*
*/

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
 *  zdungeon_printing_emulation.h
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */


//
// hack to get Z_dungeon code to compile in Zex
//

const int NOT_USED_FONT_ID = 0;			// no font
const int _SMALL_MENU_FONT_ = 9;
const int _MEDIUM_MENU_FONT_ = 14;
const int _LARGE_MENU_FONT_ = 24;

const int SMALL_PROP_WHITE_NO_BG = (('W'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int SMALL_MENU_WHITE_NO_BG = (('W'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int MEDIUM_MENU_WHITE_NO_BG = (('W'<<8) + _MEDIUM_MENU_FONT_);
const int LARGE_MENU_WHITE_NO_BG = (('W'<<8) + _LARGE_MENU_FONT_);

const int SMALL_MENU_RED_NO_BG = (('R'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int MEDIUM_MENU_RED_NO_BG = (('R'<<8) + _MEDIUM_MENU_FONT_);
const int LARGE_MENU_RED_NO_BG = (('R'<<8) + _LARGE_MENU_FONT_);

const int SMALL_MENU_GREEN_NO_BG = (('G'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int MEDIUM_MENU_GREEN_NO_BG = (('G'<<8) + _MEDIUM_MENU_FONT_);
const int LARGE_MENU_GREEN_NO_BG = (('G'<<8) + _LARGE_MENU_FONT_);

const int SMALL_MENU_ORANGE_NO_BG = (('O'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int MEDIUM_MENU_ORANGE_NO_BG = (('O'<<8) + _MEDIUM_MENU_FONT_);
const int LARGE_MENU_ORANGE_NO_BG = (('O'<<8) + _LARGE_MENU_FONT_);

const int SMALL_MENU_BLUE_NO_BG = (('B'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int MEDIUM_MENU_BLUE_NO_BG = (('B'<<8) + _MEDIUM_MENU_FONT_);
const int LARGE_MENU_BLUE_NO_BG = (('B'<<8) + _LARGE_MENU_FONT_);

const int SMALL_MENU_YELLOW_NO_BG = (('Y'<<8) + _SMALL_MENU_FONT_);	// 9 12 14 24
const int MEDIUM_MENU_YELLOW_NO_BG = (('Y'<<8) + _MEDIUM_MENU_FONT_);
const int LARGE_MENU_YELLOW_NO_BG = (('Y'<<8) + _LARGE_MENU_FONT_);

inline char extract_colour_from_font_ref(int font_ref) { return (font_ref >> 8); }
inline int extract_size_from_font_ref(int font_ref) { return (font_ref & 0xff); }
inline int construct_font_ref(int font_size, char font_colour) { return (font_colour << 8) + font_size; }

void print_string(int which_font, int x, int y, const char* the_string);
void print_string_relative(int which_font, int x_off, int y_off, const char* the_string);

void print_formatted_string(int x,int y, char* formatted_string);
void print_formatted_string(int x,int y, int string_id);


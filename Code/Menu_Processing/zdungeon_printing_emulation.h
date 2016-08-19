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


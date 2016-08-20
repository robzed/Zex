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
 *  zdungeon_printing_emulation.cpp
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */


#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "zdungeon_printing_emulation.h"
#include "get_text_string.h"
#include "Error_handler.h"




// TITLE:		 print_string
// AUTHOR(s):    Rob Probin
// DATE STARTED: 15th October 2005
//
// DESCRIPTION:  Emulate Z_dungeon print_string for Zex
//               
static void convert_font_ref_to_real_stuff(int which_font);

void print_string(int which_font, int x, int y, const char* the_string)
{
	convert_font_ref_to_real_stuff(which_font);
	MoveTo(x,y+get_font_height());	// get_font_height avoid the bottom of the font type offset that occurs with drawstring...
	DrawString_c(the_string);
}

// TITLE:		 print_string_relative
// AUTHOR(s):    Rob Probin
// DATE STARTED: 15th October 2005
//
// DESCRIPTION:  Emulate Z_dungeon print_string_relative for Zex
//  

void print_string_relative(int which_font, int x_off, int y_off, const char* the_string)
{
	if(x_off!=0 || y_off!=0)
	{
		report_error_c("offsets not zero in print_string_relative","",0);
	}
	
	convert_font_ref_to_real_stuff(which_font);
	DrawString_c(the_string);
}

// TITLE:		 convert_font_ref_to_real_stuff
// AUTHOR(s):    Rob Probin
// DATE STARTED: 25th October 2005
//
// DESCRIPTION:  
//  

void convert_font_ref_to_real_stuff(int which_font)
{		
	TextSize(extract_size_from_font_ref(which_font));
	switch(extract_colour_from_font_ref(which_font))
	{
		case 'R':	// red
		{
			RGBColor c = { 65535,     0,     0}; RGBForeColor(&c);
			break;
		}
		case 'G':	// green
		{
			RGBColor c = {     0, 65535,     0}; RGBForeColor(&c);
			break;
		}
		case 'O':	// orange
		{
			RGBColor c = { 65535, 32000,     0}; RGBForeColor(&c);
			break;
		}
		case 'B':	// blue
		{
			RGBColor c = { 10000, 10000, 65535}; RGBForeColor(&c);
			break;
		}
		case 'Y':	// yellow
		{
			RGBColor c = { 65535, 65535,     0}; RGBForeColor(&c);
			break;
		}
		default:		// includes 'W'
		{
			RGBColor c = { 65535, 65535, 65535}; RGBForeColor(&c);
			break;
		}
	}
	
}




// +---------------------------------+-------------------------+-----------------------
// | TITLE: print_formatted_string_id| AUTHOR(s): Stuart Ball  | DATE STARTED: 10 Jan 2004
// +
// | DESCRIPTION: prints a formatted string_id  at a given location
// | Formatting as follows:
// | %L, %M, %S - font size, large, medium or small
// | %R - red, %W - white, %G - Green, %O - Orange
// | %X - 2 digit hex character code follows
// | NB: Not all colours are available in all sizes
// +-----------------------------------------------------------------ROUTINE HEADER----
static int process_escape(char current_char);
const int SUBSTRING_MAX = 512;

void print_formatted_string(int x,int y, const char* formatted_string)
{
	const char* source_string = formatted_string;
	char substring[SUBSTRING_MAX];
	int substring_index=0;
	char current_char;
	int the_font = LARGE_MENU_WHITE_NO_BG; //default value
	bool escape;
	bool first_substring;
	first_substring=true;
	escape=false;
	
	for(;;)
	{
		current_char=*source_string;
		source_string++;
		if (current_char==0) break; //finished
		
		if (escape==true)
		{
			//blow out the previous string
			if (substring_index>0) //do we have substring to print?
			{
				substring[substring_index]=0; //terminate
				
				if (the_font==NOT_USED_FONT_ID) //check font is OK to print with
				{
					report_error_c("ERROR: print_formatted_string - unknown font attributes in string. Probably font isn't available in the colour you have specified.", formatted_string, 0);
					return; //lets not crash
				}
				if (first_substring==true)
				{
					print_string(the_font,x,y,substring);
					first_substring=false;
				}
				else
				{
					print_string_relative(the_font, 0, 0, substring); //continue from last internal x,y
				}
				substring_index=0; //reset substring after we've printed it
			}
			
			//now process the escape sequence
			the_font=process_escape(current_char); //this may be invalid at this time, s'ok
			
			escape=false;
			continue;
		} //end of escape==true
		
		if (current_char=='%') 
		{ 
			if (*source_string=='%') //escaped %?
			{
				source_string++;
			}
			else
				if ((*source_string=='X') || (*source_string=='x') ) //escaped %?
				{
					char hex_char;
					bool hex_error=false;
					source_string++;
					hex_char=(*source_string); source_string++;
					if (hex_char>='A') { hex_char=hex_char-'A'+10; }
					else
						if (hex_char>='0') { hex_char-='0'; }
					if (hex_char<0 || hex_char>15)
					{
						report_error_c("ERROR: print_formatted_string - unknown hex value in string",formatted_string,0);
						hex_error=true;
					}	 
					
					current_char=hex_char<<4;
					
					hex_char=(*source_string); source_string++;
					if (hex_char>='A') { hex_char=hex_char-'A'+10; }
					else
						if (hex_char>='0') { hex_char-='0'; }
					current_char+=hex_char;
					if (hex_char<0 || hex_char>15)
					{
						report_error_c("ERROR: print_formatted_string - unknown hex value in string",formatted_string,0);
						hex_error=true;
					}	 
					
					if (hex_error==true) { current_char='!'; }
					
				}
			else
			{
				escape=true; 
				continue;
			}
		} //end of % check
		
		//otherwise just store the character in the substring and move on
		substring[substring_index]=current_char;
		substring_index++;
		
		//can't put this check on debug only as user may be changing the strings
		if (substring_index>=SUBSTRING_MAX)
		{
			report_error_c("ERROR: print_formatted_string - substring too long. Maximum of characters and String below", formatted_string, SUBSTRING_MAX);
			break;
		}
	} //end of loop
	
	//finally blow out the last substring
	substring[substring_index]=0; //terminate
	
	if (the_font==NOT_USED_FONT_ID) //check font is OK to print with
	{
		report_error_c("ERROR: print_formatted_string - unknown font attributes in string. Probably font isn't available in the colour you have specified.", formatted_string, 0);
		return; //lets not crash
	}
	if (first_substring==true)
	{
		print_string(the_font,x,y,substring);
	}
	else
	{
		print_string_relative(the_font, 0, 0, substring); //continue from last internal x,y
	}
	
}

// +--------------------------------+-------------------------+-----------------------
// | TITLE: process_escape          | AUTHOR(s): Stuart Ball  | DATE STARTED: 10 Jan 2004
// +
// | DESCRIPTION: Handles escape sequences from formatted string
// | Formatting as follows:
// | %L, %M, %S - font size, large, medium or small
// | %R - red, %W - white, %G - Green, %O - Orange
// | NB: Not all colours are available in all sizes
// |		      
// +----------------------------------------------------------------ROUTINE HEADER----

static int process_escape(char current_char)
{
	static char current_colour = 'W';
	static int current_size = _LARGE_MENU_FONT_;
	
	if (current_char=='L') { current_size = _LARGE_MENU_FONT_; }
	else
		if (current_char=='M') { current_size = _MEDIUM_MENU_FONT_; }
	else
		if (current_char=='S') { current_size = _SMALL_MENU_FONT_; }
	else
		if (current_char=='R') { current_colour = 'R'; }
	else
		if (current_char=='W') { current_colour = 'W'; }
	else
		if (current_char=='G') { current_colour = 'G'; }
	else
		if (current_char=='O') { current_colour = 'O'; }
	else
		if (current_char=='B') { current_colour = 'B'; }
	else
		if (current_char=='Y') { current_colour = 'Y'; }
	else
	{
		report_error_c("ERROR: process_escape - Unknown escape sequence in string","Character Code below",current_char);
		return NOT_USED_FONT_ID;
		
	}
	
	return construct_font_ref(current_size, current_colour);
	/*
	 //calc return font
	 if (current_size==LARGE)
	 {
		 if (current_colour==WHITE) { return LARGE_MENU_WHITE_NO_BG; }
		 if (current_colour==RED) { return LARGE_MENU_RED_NO_BG; }
		 if (current_colour==ORANGE) { return LARGE_MENU_ORANGE_NO_BG; }
		 if (current_colour==GREEN) { return LARGE_MENU_GREEN_NO_BG; }
		 if (current_colour==BLUE) { return LARGE_MENU_BLUE_NO_BG; }
		 if (current_colour==YELLOW) { return LARGE_MENU_YELLOW_NO_BG; }
	 }
	 else
	 if (current_size==MEDIUM)
	 {
		 if (current_colour==WHITE) { return MEDIUM_MENU_WHITE_NO_BG; }
		 if (current_colour==RED) { return MEDIUM_MENU_RED_NO_BG; }
		 if (current_colour==ORANGE) { return MEDIUM_MENU_ORANGE_NO_BG; }
		 if (current_colour==GREEN) { return MEDIUM_MENU_GREEN_NO_BG; }
	 }
	 else
	 {
		 if (current_colour==WHITE) { return SMALL_MENU_WHITE_NO_BG; }
		 if (current_colour==RED) { return SMALL_MENU_RED_NO_BG; }
		 if (current_colour==ORANGE) { return NOT_USED_FONT_ID; }
		 if (current_colour==GREEN) { return NOT_USED_FONT_ID; }
	 }
	 
	 return NOT_USED_FONT_ID;
	 */
}



// TITLE:		 print_formatted_string
// AUTHOR(s):    Rob Probin
// DATE STARTED: 16th October 2005
//
// DESCRIPTION:  Emulate Z_dungeon print_formatted_string for Zex
//               
void print_formatted_string(int x,int y, int string_id)
{
	const char* formatted_string;
	formatted_string=get_text(string_id); //our formatted string
	print_formatted_string(x, y, formatted_string);	
}


// Error_handler.c
//
// BIG IMPORTANT NOTE:
//
// All the routines that this file calls HAVE to be safe. We can assume very little -
// we know we have SDL and a screen (for screen failures result in a print to stderr
// and an immediate exit).
//
// Therefore before a routine is used we need to check it doesn't rely on resources
// that might not be available.
//
// As well as displaying the information we also log it to stderr - FIRST. This
// ensures even when the more complicated stuff fails, we should get some error
// data out of the program.
//
//

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zex_misc.h"
#include "Error_handler.h"
#include "splat_control.h"
#include "emergency_varfont.h"

//
// Internal Function Prototypes
//
static void do_error_screen(const char* line1, const char* line2, const char* line3, bool fatal);
static void safe_print(int x, int y, const char* text);
static void centered_safe_print(int y, const char* text);

//
// The code
//
void report_error(const char *error_string, const Str255 error_string2, int error_number)
{
	char cstring[255];
	PascalToC(error_string2,cstring);
	report_error_c(error_string, cstring, error_number);	// call C version
}

void report_error_c(const char *error_string, const char *error_string2, int error_number)
{
	fprintf(stderr, "Zex Error - %s (number %i) - %s\n", error_string, error_number, error_string2);
	char temp[20];
	sprintf(temp, "%i", error_number);
	do_error_screen(error_string, error_string2, temp, true);
	exit(1);
}

void report_error_id(const char *error_string, unsigned long id)
{
	char id_string[12];
	//make id a c string
	safe_number_id_to_4_character_string(id,id_string);
	id_string[4]=0;

	fprintf(stderr, "Zex Error - %s (id = %s)\n", error_string, id_string);
	do_error_screen(error_string, "", id_string, true);
	exit(1);
}

void report_error_id_non_fatal(const char *error_string, unsigned long id)
{
	char id_string[12];
	//make id a c string
	safe_number_id_to_4_character_string(id,id_string);
	id_string[4]=0;

	fprintf(stderr, "Zex Error - %s (id = %s)\n", error_string, id_string);
	do_error_screen(error_string, "", id_string, false);
}


void report_error_id_number(const char *error_string, unsigned long id, int number)
{
	char id_string[12];
	//make id a c string
	safe_number_id_to_4_character_string(id,id_string);
	id_string[4]=0;

	fprintf(stderr, "Zex Error - %s (id = %s, number = %i)\n", error_string, id_string, number);
	char temp[40];
	sprintf(temp, "Error Number = %i", number);
	do_error_screen(error_string, temp, id_string, true);
	exit(1);
}


//
// Utility routines for report error to do its job
//

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 16 Nov 2005
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

static bool in_error_handler_already = false;		// preserve sanity and avoid recursion!
RGBColor bc = { 0, 0, 20000 };			// dark blue

static void do_error_screen(const char* line1, const char* line2, const char* line3, bool fatal)
{
	if(in_error_handler_already) return;		// preserve sanity and avoid recursion!
	in_error_handler_already = true;			// if recursion does happen we will just print to stderr

	// set the colours rather than rely on some random default
	LS_NormalGamma();						// don't be faded
	RGBBackColor(&bc);
	cls();									// cls uses the background colour to erase with

	if(fatal)
	{
		centered_safe_print(10, "Zex Error Report - Game Aborted");
	}
	else
	{
		centered_safe_print(10, "Zex Error Report - Game Halted");
	}

	safe_print(50, 150, "--->"); safe_print(100, 150, line1);
	safe_print(50, 200, "--->"); safe_print(100, 200, line2);
	safe_print(50, 250, "--->"); safe_print(100, 250, line3);

#if(__APPLE__)
	safe_print(170, 390, "(A record of this has been put into your console log)");
//#warning "Apple build"
#elif(WIN32)
	safe_print(170, 390, "(A record of this has been put into the file stderr.txt)");
//#warning "Win32 build"
#else
#warning "Unknown platform"
#endif

	if(fatal)
	{
		centered_safe_print(430, "Press a key or mouse button to QUIT");
	}
	else
	{
		centered_safe_print(430, "Press a key or mouse button to continue game");
	}

	Show_it();			// swap screens

	int old_cursor_state = Zex_ShowCursor(SDL_QUERY);
	Zex_ShowCursor(SDL_ENABLE);
	zex_polling_switch(false);

	//
	// wait for key or mouse button here, while polling events
	//
	SDL_Event event;
	bool stop = false;

	while(not stop)
	{
		while ( SDL_PollEvent(&event) ) {

			update_zex_main_event_state(event);	// keep Zex's state up to date... (Zex is entirely state polled)
			switch (event.type) {

				case SDL_MOUSEBUTTONDOWN:
				case SDL_KEYDOWN:
				case SDL_QUIT:
					stop = true;
					break;

				default:
					break;
			} // switch(event.type)

		} // while(SDL_PollEvent())
	}

	zex_polling_switch(true);
	Zex_ShowCursor(old_cursor_state);

	//
	// clear oiur screen buffer before returning
	//
	RGBColor bc2 = { 0, 0, 0 };			// black
	RGBBackColor(&bc2);
	cls();								// cls uses the background colour to erase with

	in_error_handler_already = false;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 26 Nov 2005
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----

static void centered_safe_print(int y_pos, const char* text)
{
	int pixel_width = evf_get_string_width(text);
	int x = (640-pixel_width) / 2;
	if(x<0) { x=0; }

	safe_print(x, y_pos, text);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 16 Nov 2005
// +
// |
// +----------------------------------------------------------------ROUTINE HEADER----
// we have to take care of wrap here
// we also take care of out of bounds printing
//
// We use Drawstring if it's set up ... otherwise we use the emergency text system.
//
static void safe_print(int x_pos, int y_pos, const char* text)
{
	// We check the boundaries and correct to try to ensure printing success
	// and the avoidance of generating more errors!
	//
	if(x_pos < 0)
	{
		fprintf(stderr, "Zex Error Printing off left of screen!\n");	// report to the log that we have an error, but...
		x_pos=0;
	}

	if(y_pos < 0)
	{
		fprintf(stderr, "Zex Error Printing off top of screen!\n");
		y_pos=0;
	}

	if(y_pos+evf_get_character_height() > 479)
	{
		fprintf(stderr, "Zex Error Printing off bottom of screen!\n");
		y_pos = 0;		// fix because it's unlikely the font height will be > 479 ... so let's try anyway
	}

	// Off the right of the screen is rather more complicated...
	int pixel_width = evf_get_string_width(text);
	if((x_pos+pixel_width) > 639)
	{
		//fprintf(stderr, "Zex Error Printing off right of screen!\n");
		if(pixel_width > 639)
		{
			// let's break up the line...
			int length = strlen(text);
			if(length <= 2) { return; }		// nothing else we can do!
			int length_part1 = length / 2;

			// copy first part of text into temporary string
			char* newtext = new char[length_part1+1];
			for(int i=0; i<length_part1; i++)
			{
				newtext[i] = text[i];
			}
			newtext[length_part1]=0;

			// NOTE: if the line is very very long causing 4 lines to be emitted then line 3 will overlap line 2. Hopefully this will never happen.
			safe_print(x_pos, y_pos, newtext);
			safe_print(x_pos, y_pos+evf_get_character_height(), &text[length_part1]);
			return;
		}
		else
		{
			x_pos = 0;	// can print the string if we start at the beginning of the line...
		}
	}

	SDL_Surface *screen = Get_back_buffer();
	Uint32 background_colour_for_SDL = SDL_MapRGB(screen->format,(bc.red)>>8,(bc.green)>>8,(bc.blue)>>8);
	Uint32 foreground_colour_for_SDL = SDL_MapRGB(screen->format,255,255,255);

	evf_print_string(x_pos, y_pos, text, background_colour_for_SDL, foreground_colour_for_SDL);
}

/*
$Header: /home/ls_cvs/Zex2005/Code/Screen_Splat_Control/Splat_control.cpp,v 1.27 2006/06/19 22:03:04 rob Exp $
$Log: Splat_control.cpp,v $
Revision 1.27  2006/06/19 22:03:04  rob
Zex: Memory check to include display plus screen checking.

Revision 1.26  2006/01/16 22:37:13  rob
Zex2005: Corrected (minor) build warnings from x86 gcc.

Revision 1.25  2006/01/01 17:49:07  rob
Zex2005: Added back in screen fading, especially for fast dock.

Revision 1.24  2005/12/30 13:09:53  rob
Added buffers in front and behind the screen to try to ensure non-clipped writes don't corrupt other memory. There shouldn't be any non-clipped writes - but still this should reduce any corruption by that method.

Revision 1.23  2005/12/20 17:45:53  rob
Zex2005: Signed/Unsigned change.

Revision 1.22  2005/12/11 13:46:30  rob
Zex2005: Software cursors are so much more fun.

Revision 1.21  2005/12/11 12:46:01  rob
Zex2005: Penance to the gods of efficiency for wrongly comparing in an inner loop. BONUS FEATURE: black cursor.

Revision 1.20  2005/12/10 23:16:14  rob
Zex2005: Don't create a new back buffer with every switch. Don't erase screen/log on screen switch.

Revision 1.19  2005/12/10 10:50:38  rob
Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.

Revision 1.18  2005/11/26 21:30:22  rob
Zex2005: Spotted possible problem with Show_it (in splat_control) where if SDL failed to get memory we wouldn't throw an error - probably with nasty results - oops.

Revision 1.17  2005/10/11 00:00:44  stu
Zex2005: Code: Sound and music

Revision 1.16  2005/10/10 21:28:24  rob
Zex2005: Added newline at end to stop compiler whinging.

Revision 1.15  2005/10/08 13:14:00  rob
Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.

Revision 1.14  2005/06/27 19:56:15  rob
Zex2005: Files changed so that use of a string constant (aka a string literal) is only used by const char * routines - this way we can alter the compiler to warning about access by char *. Real reason for altering this is because we had a case of altering these string constants (to insert call signs) which caused a bus error. This has been corrected by copying the string before modification.

Revision 1.13  2005/06/04 18:25:47  rob
Zex2005: Load screen changes - centred title, removed later unnecessary messages, increased delay on each message (so it looks more impressive) and replaced delay algorithm with SDL delay function to (perhaps) return CPU time to system.

Revision 1.12  2005/06/04 13:10:56  rob
Zex2005: Basic full screen / windowed mode change.

Revision 1.11  2005/06/01 19:07:10  rob
Zex2005: Updated project - now runs main menu.

Revision 1.10  2005/05/23 21:13:25  rob
Zex2005: More changes to files.

Revision 1.9  2005/05/17 13:11:02  rob
Zex2005: Various assorted conversion fixes.

Revision 1.8  2005/05/15 19:30:55  rob
Zex2005: Buffer overrun protection.

Revision 1.7  2005/05/15 17:43:11  rob
Zex2005: ZexNumToString used throughout, file case changes, general gcc4.0 build changes.

Revision 1.6  2005/05/04 21:08:41  rob
Zex2005: Added some screen splat code.

Revision 1.5  2005/05/04 17:32:04  rob
Zex2005: More changes for build.

Revision 1.4  2005/05/02 10:31:51  rob
Zex2005: More Updates (code)

Revision 1.3  2005/04/24 10:10:05  rob
ZEX: Started to work on ZEX.cpp. Replaced TickCount. Added some void return types. Commented out some Mac init functions. Altered main.cpp to do the correct thing on an exit().

Revision 1.2  2005/04/23 20:13:44  rob
ZEX: First set of changes for SDL build. (remainder of files)

Revision 1.26  1999/09/19 05:38:50  stu
0.93 as posted

Revision 1.25  1999/08/30 22:22:54  stu
300899

Revision 1.24  1999/08/15 22:16:47  stu
Cleanup 150899

Revision 1.23  1999/07/29 05:09:37  stu
Version 0.87 changes - minot bug fixes etc.

Revision 1.22  1999/07/26 19:04:13  stu
Jul 27 - Just before 0.86 test

Revision 1.21  1999/06/25 01:13:29  stu
Mods to accomodate level 2

Revision 1.20  1999/06/22 10:57:27  stu
Lurkers clean up

Revision 1.19  1999/06/14 07:25:44  stu
*** empty log message ***

Revision 1.18  1999/06/08 23:07:48  stu
Final checkin before cvs pro client upgrade.

Revision 1.17  1999/06/07 05:41:40  stu
Changes for tumbling_to_explode_object

Revision 1.16  1999/06/03 18:26:43  stu
Mod to load frag for 320*240 screen splat

Revision 1.15  1999/06/03 18:14:23  stu
Mod to load frag for 320*240 screen splat

Revision 1.14  1999/05/23 22:28:10  rob
Add options.h

Revision 1.13  1999/05/23 16:57:35  stu
Work completed on:
EF-003 - Screen Centering 
and 
EF-004 - No resolution change.

Revision 1.12  1999/05/22 13:06:24  stu
Added new pref -- Use_VBL

Revision 1.11  1999/05/22 12:16:05  stu
This update finalises the changes to the VBL screen driver (Hopefully :)) Now fixed at two buffers so a few things in Zex that assumed 3 buffers need changing but it basically works.

Revision 1.10  1999/05/21 03:31:18  stu
1. New screen driver.
2. VBL Driven music fade and play

Revision 1.9  1999/05/18 15:13:44  stu
1.9 - 320*240 splat now only splats the control panel when it is actually updatd

Revision 1.8  1999/05/17 18:12:23  stu
1.8 - More work for pixel double screen

Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c

Revision 1.7  1999/05/17 04:34:30  stu
Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c

Revision 1.6  1999/05/14 17:16:45  stu
Removed CVS Tag and Date expansion. Removed request for HW page
flipping due to slowdown on translucency,

Added CVS Tag and Date expansion to revision printing

Changed printed to screen version to 0.85

*/

/*
How this works.
We have MAXBUFFERS buffers as GWorlds so we can use quickfraw functions where necessary.
These are held in an array call backbuffers_array which is simply an array of GWorld pointers filled in init_screen.
along with their widths and start addresses (they are locked) and a pending flag.

A routine, getbackbuffer does the following.
 A backbuffer_counter runs from 0 to MAXBUFFERS-1
 getbackbuffer simply returns the GWorld pointer out of backbuffer_array[backbuffer_counter]


A routine ShowIt places the current backbuffer GWorldPtr into our vbl_queue along with a queue serial number and it's backbuffer index and 
sets the pending flag in backbuffer_array. If the current slot in the vbl queue is in-use, it waits 9although this is impossoble
due to the blocking on the serving of backbuffers - see next line).

The pending flag is used in getbackbuffer to determine if the buffer is available to be served up.
If not it waits.


The vbl queue is maintained by two variables - vbl_queue_write and vbl_queue_read. These can range between 0 and MAXBUFFERS.
write is the index for insertion and read is the index for splatting.

Showit then increments/rolls over backbuffer_counter

Our vbl routin splats the current read entry to screen and then clears the waiting flag in the backbuffer array and the queue entry.

*/

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

//oldmac//#include	<Resources.h>
//oldmac//#include <Palettes.h>
//oldmac//#include <QDOffscreen.h>
//oldmac//#include <mactypes.h>
//oldmac//#include <fixmath.h>

#include <string.h>

#include	"Error_handler.h"
#include	"splat_control.h"
#include	"splat_control_int.h"
#include	"zex_misc.h"
#include "sound.h"

#include "pictures.h"
#include	"screen_splats.h"
#include    "fpoly4.h"
#include "OGL.h"
#include "main.h"

#include "memory_debug.h"

#define MAXBUFFERS 2	//how many screen buffers we using - valid is 2 or 3

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
Boolean ZexVBLProc (DSpContextReference the_context, void *inRefCon);
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING

int show_control_panel;
extern int gFadeMusic;

//oldmac//DSpContextReference		mDisplayContext;
//oldmac//DSpContextAttributes	mDisplayAttributes;
//oldmac//GDHandle				mZexGDH;
CTabHandle	 	 	    mZexCLUT;
//oldmac//LS_CGrafPtr	backBuff;
//oldmac//GDHandle				origGDH;

int Mode320;	//if 1 we are in 320*240 pixel double mode
int Use_Mode320;	//from prefs - if 1 then game sets Mode320 for in-game display and clears it at end of game
int Use_SpeedLimit;
int Use_VBL;
int Change_res;

int flash_screen=0;	//if 1 screen is erased to white rather than black

int splat_offset_x, splat_offset_y;	//video splat offsets also used in ZexGetMouse()
//oldmac//int vram_width;
//oldmac//Ptr vram_base;

extern int global_show_status;

//New code using gworlds and custom splat



//our backbuffers_array
typedef struct {
//oldmac//GWorldPtr TheGWorld;
char * buffer_address;
int buffer_width;
int pending;
} bbtype;

bbtype backbuffers_array[MAXBUFFERS];

int backbuffer_counter=0;	//used in get back buffer
//int current_backbuffer;	//used for showit to get the current buffer

//our VBL queue
typedef struct {
char * dest_address;	//where splat to
int dest_width;		//width of destination
char * source_address;
int source_width;
int bb_array_index;	//where this backbuffer came from so we can clr the pending bit
} vblqueuetype;

vblqueuetype vbl_queue[MAXBUFFERS];

int vbl_queue_write=0;
int vbl_queue_read=0;

//oldmac//GrafPtr		        savePort;
//oldmac//GrafPtr		        origPort;
//Zex screen driver
//A vbl driven, double buffered with pixel oubling driver




// TEST TEST TEST

#include "debug_timing.h"
//oldmac//#include <DriverServices.h>

#define MAXLOG 400
int logbuf[MAXLOG];
int logindex=0;

//oldmac//LS_UnsignedWide gentime;
//oldmac//LS_UnsignedWide *blah=&gentime;

#define logit(xx) logbuf[logindex]=(int) xx; logindex++; if(logindex==MAXLOG) logindex=0;

#define logum(yy) logit(yy); start_timer(blah); logit(blah->lo);

/*

4 bytes as follows:-

   -1     start of record
   yy     user long (e.g. 'init')
   index  from 2 to 398 in steps of 4
   time   absolute time in arb units
  
Incantation:-

logum(yy)    where yy is the user long


 ----------- end of TEST   */

// 
// FUNCTION PROTOTYPES
//
static void set_screen_clut_from_resource_data(unsigned short* clut_data, int size);
static void set_clut_if_known();


//
// THE CODE
//

int init_screen() 
{
//oldmac//LS_CGrafPtr	frontBuff;
int i;
OSStatus the_error;
Rect offrect = {0,0,480,640};
extern	      CTabHandle mZexCLUT;
extern int gupdate_cp;

	Mode320=0;
//	gupdate_cp=1;
//	Use_Mode320=1;	//now read from prefs

	mZexCLUT = LS_GetCTable(128);	  //our clut
	LS_DetachResource((Handle) mZexCLUT);

	set_screen_clut_from_resource_data(reinterpret_cast<unsigned short*>(*mZexCLUT), GetHandleSize(mZexCLUT));	// de-reference and pass to routine for processing

									   

// new lines
erase_zex_rect(640,480);	//in pictures
show_loading_screen(); 

return 0;
}



static LS_CGrafPtr our_screen;
static char *our_SDL_back_buffer;
const int our_screen_width = WIND_WIDTH;
const int our_screen_height = WIND_HEIGHT;
void set_up_game_screen_buffers(LS_CGrafPtr screen_to_set)
{
	char *actual_screen_buffer_memory_with_buffer;

	our_screen = screen_to_set;
	if(!our_SDL_back_buffer)
	{
		// we allocate one screen worth of pixels before and another afterwards. This is to stop 
		// crashes we were having with corruption. Ideally we would find all the places it was crashing
		// by overwriting the end of the buffer. However, this was it makes it more stable.
		const int screen_pixels = our_screen_width*our_screen_height;
		const int buffer_multiplier = 3;
		actual_screen_buffer_memory_with_buffer = new char[screen_pixels*buffer_multiplier];		// one screen buffer before and another afterwards
		// ensure clear memory...
		memset(actual_screen_buffer_memory_with_buffer, 0, buffer_multiplier*screen_pixels);

		new InPlace_MemoryDebug((long unsigned *)(actual_screen_buffer_memory_with_buffer), screen_pixels/4 /* clear bottom 2 bits */);
		new InPlace_MemoryDebug((long unsigned *)(actual_screen_buffer_memory_with_buffer + (2*screen_pixels)), screen_pixels/4 /* clear bottom 2 bits */);
		// copy the offset pointer into screen back buffer pointer
		our_SDL_back_buffer = actual_screen_buffer_memory_with_buffer + screen_pixels;	// will add size of char

	}
	
	
	Get_back_buffer();		// set up the screen address and screen width just in case
	
	set_clut_if_known();
}

LS_CGrafPtr Get_back_buffer(void)
{
	extern Ptr screenaddr;
	extern int screenwidth;

	screenaddr = our_SDL_back_buffer;
	screenwidth = our_screen_width;
		
	return our_screen;
}

//Heavy mods - SB 17 05 99 to use our own splat routine
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
//returns the backbuffer as a LS_CGrafPtr
LS_CGrafPtr Get_back_buffer(void) {
extern	      LS_CGrafPtr	            backBuff;
extern Ptr screenaddr;
extern int screenwidth;
int start_monitor_time,current_monitor_time;


//A routine, getbackbuffer does the following.
// A backbuffer_counter runs from 0 to MAXBUFFERS-1
// getbackbuffer simply returns the GWorld pointer out of backbuffer_array[backbuffer_counter]
start_monitor_time=LS_TickCount60()+20;	//we allow a max of 20 ticks for operation to succeed


    while (backbuffers_array[backbuffer_counter].pending==1)	//block until the buffer has been splatted. Pending set in ShowIt()
    {
//    var_a++;    
    if (LS_TickCount60()>start_monitor_time) report_error_c("Get_Back_buffer: Backbuffer Server WatchDog Fired! VBL inactive?","",2);;
    }
    backBuff=backbuffers_array[backbuffer_counter].TheGWorld;
    screenaddr=((*(backBuff)->portPixMap))->baseAddr;
    screenwidth=0x3fff & ((*(backBuff)->portPixMap))->rowBytes;
//    SetGWorld(backBuff,0);	  //so we can use QD
    SetPort((GrafPort*) backBuff);	 //so we can use QD
//    current_backbuffer=backbuffer_counter;

//    logum(-1); // TEST TEST TEST TEST
//    logum(backBuff);
    
//showit incs the backbuffer counter!
//    backbuffer_counter++;
//    if (backbuffer_counter=MAXBUFFERS) backbuffer_counter=0;
    
return backBuff;

}
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING

// +--------------------------------------------+-------------------------+-----------------------
// | TITLE: set_screen_clut_from_resource_data  | AUTHOR(s): Rob Probin   | DATE STARTED: 29 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
static SDL_Color zex_colors[256];
static bool clut_data_valid = false;

static void set_screen_clut_from_resource_data(unsigned short* clut_data, int size)
{	
	if(size != (2*4*257))
	{
		report_error_c("Clut resource is wrong size", "",0);
	}
	
	clut_data+=4;	// first 4 shorts are boring (0000 0000 0000 00FF)

	/* Fill colors with color information */
	for(int i=0;i<256;i++)
	{
		if(*clut_data++ != i)
		{
			report_error_c("Error decoding clut data!", "",0);
		}
		zex_colors[i].r=(*clut_data++)>>8;
		zex_colors[i].g=(*clut_data++)>>8;
		zex_colors[i].b=(*clut_data++)>>8;
	}				

	clut_data_valid = true;
	set_clut_if_known();
}

// +--------------------------------------------+-------------------------+-----------------------
// | TITLE: set_clut_if_known                   | AUTHOR(s): Rob Probin   | DATE STARTED: 29 May 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static int clut_modification_red = 0xffff;
static int clut_modification_green = 0xffff;
static int clut_modification_blue = 0xffff;
static bool clut_modification_active = false;

static void set_clut_if_known()
{
	if(clut_data_valid)
	{
		int result;
		
		if(! clut_modification_active)
		{
			result = SDL_SetColors(our_screen, zex_colors, 0, 256);
		}
		else
		{
			SDL_Color modified_zex_colors[256];
			// we have to copy the graphics
			for(int i=0; i<256; i++)
			{
				modified_zex_colors[i].r = static_cast<Uint8>(zex_colors[i].r * (clut_modification_red/65535.0));
				modified_zex_colors[i].g = static_cast<Uint8>(zex_colors[i].g * (clut_modification_green/65535.0));
				modified_zex_colors[i].b = static_cast<Uint8>(zex_colors[i].b * (clut_modification_blue/65535.0));
			}
			
			result = SDL_SetColors(our_screen, modified_zex_colors, 0, 256);
		}
		
		if(result != 1)
		{
			report_error_c("Setting surface clut partially failed","",0);
		}
	}
}

// +--------------------------------------------+-------------------------+-----------------------
// | TITLE: set_screen_clut_from_resource_data  | AUTHOR(s): Rob Probin   | DATE STARTED: 31 Dec 2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void set_clut_modification(int red_level, int green_level, int blue_level)
{
	clut_modification_active = red_level != 0xffff || green_level != 0xffff || blue_level != 0xffff;
	clut_modification_red = red_level;
	clut_modification_green = green_level;
	clut_modification_blue = blue_level;
	set_clut_if_known();
}







/* Stolen from the mailing list */
/* Creates a new mouse cursor from an XPM */

/* XPM */
static const char *arrow[] = {
	/* width height num_colors chars_per_pixel */
	"    32    32        3            1",
	/* colors */
	"X c #000000",
	". c #ffffff",
	"  c None",
	/* pixels */
	"X                               ",
	"XX                              ",
	"X.X                             ",
	"X..X                            ",
	"X...X                           ",
	"X....X                          ",
	"X.....X                         ",
	"X......X                        ",
	"X.......X                       ",
	"X........X                      ",
	"X.....XXXXX                     ",
	"X..X..X                         ",
	"X.X X..X                        ",
	"XX  X..X                        ",
	"X    X..X                       ",
	"     X..X                       ",
	"      X..X                      ",
	"      X..X                      ",
	"       XX                       ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"                                ",
	"0,0"
};

const int cursor_size_x = 32;
const int cursor_size_y = 32;

static unsigned char original_screen[cursor_size_x*cursor_size_y];
static unsigned char* screen_pointers[cursor_size_x*cursor_size_y];
static int number_plotted = 0;

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void print_cursor(int x, int y, unsigned char core_colour)
{
	const int hot_x = 1, hot_y = 2;
	
	number_plotted = 0;
//	sscanf(arrow[4+32], "%d,%d", &hot_x, &hot_y);

	x -= hot_x; y -= hot_y;		// adjust for hot spot

	//
	// calculate how much we need to print
	//	
	int start_row  = 0;
	int start_col = 0;
	
	int width_row = cursor_size_y;
	int width_col = cursor_size_x;

	int end_row_plus1 = cursor_size_y;
	int end_col_plus1 = cursor_size_x;

	// totally off screen checks
	if(x < -cursor_size_x) 
	{
		return; 
	}
	if(y < -cursor_size_y) 
	{ 
		return; 
	}
	if(x >= 640) 
	{ 
		return; 
	}
	if(y >= 480) 
	{
		return; 
	}
	
	// just a little off the screen
	if(x < 0) 
	{
		start_col = -x; width_col = width_col+x; x=0;
	}
	if(y < 0) 
	{
		start_row = -y; width_row = width_row+y; y=0;
	}
	if(x+cursor_size_x-1 >= 640) 
	{
		width_col = 640-x; end_col_plus1 = width_col;
	}
	if(y+cursor_size_y-1 >= 480) 
	{
		width_row = 480-y; end_row_plus1 = width_row;
	}
	
	// calculate the screen destination position
	extern Ptr screenaddr;
	extern int screenwidth;
	int screen_line_step = screenwidth;
	
	unsigned char *dest_addr = (unsigned char *) (((unsigned char*)screenaddr)+x+(y*screen_line_step));
	
	//
	// actually do the printing
	//
	for (int row = start_row; row < end_row_plus1; ++row ) {
		for (int col = start_col; col < end_col_plus1; ++col ) {
			
			// out of bounds testing
			//if(x<0 || x>=640 && y<0 && y>=480)		// if this pixel is in range
			//{
			//	report_error_c("oh no","",0);
			//}
			
			char c = arrow[4+row][col];
			if(c == 'X')
			{
				original_screen[number_plotted] = *dest_addr;
				screen_pointers[number_plotted] = dest_addr;
				number_plotted++;
				*dest_addr = 0;			// hand picked colour (can use 'X' for testing)
			}
			if(c == '.')
			{
				original_screen[number_plotted] = *dest_addr;
				screen_pointers[number_plotted] = dest_addr;
				number_plotted++;
				*dest_addr = core_colour;				// hand picked colour (can use '.' for testing)
			}

			x++;
			dest_addr++;
		}
		// go back to start of pixel printing
		x -= width_col;
		dest_addr -= width_col;
		// and advance onto next line
		y++;
		dest_addr += screen_line_step;
	}
}
// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static bool software_cursor_on = false;
static bool cursor_inside_window = true;

static unsigned char cursor_colours[] = { 
	/***/ 255, // 0 0 0
	244, // 0 0 17
	243, // 0 0 34
	/* */ 214,  // 0 0 51
	242, // 0 0 68
	241, // 0 0 85
	/* */ 213, // 0 0 102
	240, // 0 0 119
	239, // 0 0 136
	/* */ 212, // 0 0 153
	238, // 0 0 170
	237, // 0 0 187
	/* */ 211, // 0 0 204
	236, // 0 0 221
	235, // 0 0 238
	/* */ 210, // 0 0 255
};

static void display_zex_cursor()
{
	if(software_cursor_on && cursor_inside_window && has_mouse_ever_moved())
	{
		static unsigned int colour_index = 0;
		static bool colour_direction_up = true;
		int x,y;
		get_mouse_x_and_y(&x, &y);
		print_cursor(x, y, cursor_colours[colour_index]);
		
		// do colour change
		if(colour_direction_up) 
		{ 
			colour_index++;
			if(colour_index >= sizeof(cursor_colours)) { colour_index--; colour_direction_up = false; }
		} 
		else 
		{ 
			if(colour_index != 0)
			{
				colour_index--; 
			}
			else	// once we hit zero, reverse the direction
			{
				colour_direction_up = true; 
			}
		}
		
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

static void erase_zex_cursor()
{
	if(software_cursor_on)
	{
		for(int i=0; i<number_plotted; i++)
		{
			*(screen_pointers[i]) = original_screen[i];
		}
		number_plotted = 0;		// only do this once
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 7-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----
void cursor_inside_of_window(bool inside_window)
{
	cursor_inside_window = inside_window;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 4-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

enum cursor_mode_type { HARDWARE_CURSOR, SOFTWARE_CURSOR_IN_FULL_SCREEN, SOFTWARE_CURSOR_ALWAYS };
const cursor_mode_type CURSOR_MODE = SOFTWARE_CURSOR_ALWAYS;

int Zex_ShowCursor(int state)
{
	if(CURSOR_MODE == SOFTWARE_CURSOR_IN_FULL_SCREEN)
	{
		int return_state;
		static bool last_cursor_state_on = true;
		
		//
		// First we find out what we are trying to do and update variables
		//
		if(state == SDL_DISABLE)
		{
			last_cursor_state_on = false;
		}
		else if(state == SDL_ENABLE)
		{
			last_cursor_state_on = true;
		}
		// return the current state
		if(last_cursor_state_on)
		{
			return_state = SDL_ENABLE;
		}
		else
		{
			return_state = SDL_DISABLE;
		}
		
		//
		// Next we update the internal or system cursors with this formation
		//
		if(are_we_in_full_screen_mode())
		{
			SDL_ShowCursor(SDL_DISABLE);		// disable hardware cursor in full screen mode
			software_cursor_on = last_cursor_state_on;
		}
		else // windowed
		{
			SDL_ShowCursor(return_state);	// drop return state from sdl show cursor ... it might not be up to date
			software_cursor_on = false;		// windowed mode uses the system cursor
		}
		
		return return_state;	
	}
	// ==============================================
	else if(CURSOR_MODE == HARDWARE_CURSOR)
	{
		software_cursor_on = false;
		return SDL_ShowCursor(state);
	}
	// ==============================================
	else // SOFTWARE_CURSOR_ALWAYS
	{
		if(state == SDL_DISABLE)
		{
			software_cursor_on = false;
			SDL_ShowCursor(SDL_DISABLE);		// disable hardware cursor definately
		}
		else if(state == SDL_ENABLE)
		{
			software_cursor_on = true;
		}
		//
		// return the state in all conditions, including SDL_QUERY
		//
		int return_state = SDL_DISABLE;
		
		if(software_cursor_on)
		{
			return_state = SDL_ENABLE;
		}
		
		return return_state;
	}
		
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 10-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void printf_surface_data(const char* name, SDL_Surface *surface)
{
	printf("%s Surface: w:%d h:%d bpp:%d\n", name, surface->w, surface->h, surface->format->BitsPerPixel);
	printf("%s Surface: pitch:%d flags:%x Bpp:%d\n", name, surface->pitch, surface->flags, surface->format->BytesPerPixel);
	printf("%s Surface clip: x:%d y:%d h:%d w:%d\n", name, surface->clip_rect.x, surface->clip_rect.y, surface->clip_rect.h, surface->clip_rect.w);
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob          | DATE STARTED: 10-Dec-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void printf_palette_data(const char* name, SDL_Palette *palette)
{
	SDL_Color* colour = palette->colors;
	for(int i=0; i < palette->ncolors; i++)
	{
		if((i%8)==0) printf("\n%s Palette (%d): ", name, i);
		printf("%02x%02x%02x ", colour->r, colour->g, colour->b);
		colour++;
	}
	printf("\n");
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: translated_surface_copy      | AUTHOR(s): Rob Probin   | DATE STARTED: 31-12-2005
// +
// | Copy work surface to screen surface translating each pixel by a pre-determined palette change.
// +----------------------------------------------------------------ROUTINE HEADER----

static bool palette_translation = false;
static unsigned char translation_array[256];

static void translated_surface_copy()
{
	/* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(our_screen) ) {
        if ( SDL_LockSurface(our_screen) < 0 ) {
            report_error_c("Can't lock the screen!", SDL_GetError(), 0);
            return;
        }
    }

	// We could assume bpp is 8. However, just in case we alter the step based on this variable.
	// It will fail - but semi-gracefully - but it will still set the wrong pixels.
	int bpp = our_screen->format->BytesPerPixel;

	// screen pixels address
	Uint8 *p = (Uint8 *)our_screen->pixels;
	int go_to_next_line_from_end = our_screen->pitch - (our_screen_width*bpp);
	// now the simple pointer for the back buffer
	Uint8 *s = (Uint8*)(our_SDL_back_buffer);
	
	//
	// The copy loop
	//
	for(int height = 0; height < our_screen_height; height++)
	{
		for(int width = 0; width < our_screen_width; width++)
		{
			//*p = translation_array[*s];			// do the translated copy
			*p = *s;
			p += bpp;							// next pixel
			s++;
		}
		
		p -= (our_screen_width*bpp);
		p += go_to_next_line_from_end;
	}
	
	// unlock screen again
	if ( SDL_MUSTLOCK(our_screen) ) {
        SDL_UnlockSurface(our_screen);
    }
	
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 31-12-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void set_translation_array(unsigned char* pixels_to_copy)
{
	for(int i=0; i<256; i++)
	{
		translation_array[i] = *pixels_to_copy;
		pixels_to_copy++;
	}
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 31-12-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void enable_pixel_translation()
{
	palette_translation = true;
}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE:                            | AUTHOR(s): Rob Probin   | DATE STARTED: 31-12-2005
// +
// | 
// +----------------------------------------------------------------ROUTINE HEADER----

void disable_pixel_translation()
{
	palette_translation = false;
}


// +-----------------------------------+-------------------------+-----------------------
// | TITLE: straight_surface_copy      | AUTHOR(s): Rob Probin   | DATE STARTED: ??
// +
// | Copy work surface to screen surface.
// +----------------------------------------------------------------ROUTINE HEADER----

static void straight_surface_copy()
{
	//
	// this is a hack. A bad, nasty hack.
	// Probably slow as well - copying between two screen sized buffers rather than one (from our buffer into screen then onto the real screen)...
	// There also needs to be some vbl sync done - but let's get it working first!!
	//
	// Because "No operating system or library calls should be made between lock/unlock pairs, as critical 
	// system locks may be held during this time."
	// this allows us to splat without re-writing the entire of Zex.
	//
	SDL_Surface* temp_surface = SDL_CreateRGBSurfaceFrom(our_SDL_back_buffer, our_screen_width, our_screen_height, 
														 /*depth*/8, /*pitch*/our_screen_width, /*Rmask*/0, /*Gmask*/0, /*Bmask*/0, /*Amask*/0);
	
	if(!temp_surface)			// nothing we can do on a temporary surface failure
	{
		report_error_c("Create surface couldn't allocate memory", "Show_it", 0); 
		return; 
	}
	
	//
	// Here we copy the new image surface palette from the screen palette - which was set up earlier.
	//
	// This is backwards from normal - perhaps normally we'd copy the screen
	// surface palette from the image palette to get better colour matching - but
	// in our case we have no palette on the image!!
	if (temp_surface->format->palette && our_screen->format->palette) 
	{
		SDL_SetColors(temp_surface, our_screen->format->palette->colors, 0,
					  our_screen->format->palette->ncolors);
		
		// **WARNING*** with dynamic palette setting now used zex_colors here aren't correctly
		// used - adjust before uncommenting.
		//		if(clut_data_valid)
		//		{
		//			int result = SDL_SetColors(temp_surface, zex_colors, 0, 256);
		//			result ++;
		//			// ignore result
		//		}
	}
	
	/*	************* show variation data ***********
		static bool print_temp = true;
	if(print_temp)
	{
		//	printf_surface_data("Temp_surface", temp_surface);
		
		//printf_palette_data("Screen", our_screen->format->palette);
		//printf_palette_data("Temp_surface", temp_surface->format->palette);
		print_temp = false;
	}*/
	
	//
	// NOTE: We might be better not creating a temporary surface here, and instead
	// just locking the surface and copying across if necessary with memcpy()
	//
	
    /* Blit onto the screen surface */
    if(SDL_BlitSurface(temp_surface, NULL, our_screen, NULL) != 0)
	{
		const char *sdl_error_message = SDL_GetError();				// get error message
		if(strlen(sdl_error_message) > 100) sdl_error_message = "<SDL error message too big>";		// if the error message is too big, replace it.
		char my_string[200];	// string to store composite message in
		sprintf(my_string, "BlitSurface error - %s", sdl_error_message); // perhaps we should use snprintf(string, length, formatter, ... ) and check return value??
        report_error_c(my_string, "", 0);
	}
	
	/* Free the allocated temp surface */
	SDL_FreeSurface(temp_surface);

}

// +-----------------------------------+-------------------------+-----------------------
// | TITLE: Show_it                    | AUTHOR(s): Stu / Rob    | DATE STARTED: ??
// +
// | Display the latest frame
// +----------------------------------------------------------------ROUTINE HEADER----

void Show_it(void)
{
#if 0			// make 1 to test fade
	static int percent = 100;
	//LS_FadeGamma(percent,0);		// fade to black
	//RGBColor test_colour = {0xffff,0,0};		// red
	//RGBColor test_colour = {0,0,0xffff};		// blue
	//RGBColor test_colour = {0,0xffff,0};		// green
	RGBColor test_colour = {0xffff,0x7fff,0};		// orange
	LS_FadeGamma(percent,&test_colour);
	percent--; if(percent == -100) percent = 100;
#endif
	
	display_zex_cursor();		// last thing that is printed
	
	
	if(palette_translation)
	{
		translated_surface_copy();
	}
	else
	{
		straight_surface_copy();
	}

	SDL_UpdateRect(our_screen, 0, 0, our_screen_width, our_screen_height);
	
	if (gFadeMusic==1)
	   {
		 do_fade_out_music();
	   }

	erase_zex_cursor();
}


//A routine ShowIt places the current backbuffer GWorldPtr into our vbl_queue along with a queue serial number and it's backbuffer index and 
//sets the pending flag in backbuffer_array. If the current slot in the vbl queue is in-use, it waits 9although this is impossoble
//due to the blocking on the serving of backbuffers - see next line).
//
//The pending flag is used in getbackbuffer to determine if the buffer is available to be served up.
//If not it waits.
//
//
//The vbl queue is maintained by two variables - vbl_queue_write and vbl_queue_read. These can range between 0 and MAXBUFFERS.
//write is the index for insertion and read is the index for splatting.
//
//Showit then increments/rolls over backbuffer_counter
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
void Show_it(void)
{
LS_CGrafPtr	frontBuff;
extern int OGL_active;

#if OGL
if (OGL_active==1)
{
OGL_show_it();
}
else
{
#endif
//    logum(-2); // TEST TEST TEST TEST
   DSpContext_GetFrontBuffer(mDisplayContext, &frontBuff);	//get vram address
   
//set up vbl queue at vbl_write
if (Use_VBL==1)
{
//#if USE_VBL
   vbl_queue[vbl_queue_write].source_address=backbuffers_array[backbuffer_counter].buffer_address;
   vbl_queue[vbl_queue_write].source_width=backbuffers_array[backbuffer_counter].buffer_width;
//set splat to items
   vbl_queue[vbl_queue_write].dest_width=vram_width;
//mod here for offset splat
//   vbl_queue[vbl_queue_write].dest_address=((*(frontBuff)->portPixMap))->baseAddr
//   + (splat_offset_y*vram_width)	  //offset in y
//   + splat_offset_x
//   ;
   vbl_queue[vbl_queue_write].dest_address=vram_base
   + (splat_offset_y*vram_width)	//offset in y
   + splat_offset_x
   ;
   
   vbl_queue[vbl_queue_write].bb_array_index=backbuffer_counter;
//set pending in backbuffer_array
   backbuffers_array[backbuffer_counter].pending=1;
//inc write_queue
   vbl_queue_write++;	//the buffer index in buffer-array so splat can clear pending
   if (vbl_queue_write==MAXBUFFERS) vbl_queue_write=0;

    backbuffer_counter++;
    if (backbuffer_counter==MAXBUFFERS) backbuffer_counter=0;
//    current_backbuffer=backbuffer_counter;
}
//#else
else
{
  
//simply splat it
  if (Mode320==1 && global_show_status==0) 
  splat_320by240(backbuffers_array[backbuffer_counter].buffer_address, backbuffers_array[backbuffer_counter].buffer_width, 
//  ((*(frontBuff)->portPixMap))->baseAddr
   vram_base
   + (splat_offset_y*vram_width)	//offset in y
   + splat_offset_x, vram_width);
  else 
  splat_640by480(backbuffers_array[backbuffer_counter].buffer_address, backbuffers_array[backbuffer_counter].buffer_width, 
//  ((*(frontBuff)->portPixMap))->baseAddr
   vram_base
   + (splat_offset_y*vram_width)	//offset in y
   + splat_offset_x, vram_width);
}

Get_back_buffer();
//old drawsprocket code
//    DSpContext_InvalBackBufferRect(mDisplayContext,&no_rect);
//	DSpContext_SwapBuffers(mDisplayContext, NULL, 0);
#if OGL
}	//end of ogl_active
#endif
}
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING





////show current buffer, switch and erase next buffer ready for drawing
void show_and_get_next_screen()
{
extern Ptr screenaddr;
extern int screenwidth;

extern const RGBColor	rgbBlack;
LS_CGrafPtr the_drawing_buffer;

   
     Show_it();	//switch pages
    
	 the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures

//check for music fade out
}


// 
int exec_watchdog=100;	//if this gets to zero we exec out
int gCheckWatchDog;



//void deferred_vbl_proc()
//{
//if (vbl_queue[0].queued_for_splat==1 && vbl_queue[1].queued_for_splat==1)
//{
////we need to examine the sernos to find out which is first
// if (vbl_queue[0].serial_number<vbl_queue[1].serial_number)
//  {
//  if (Mode320==1) splat_320by240(vbl_queue[0].source_address, vbl_queue[0].source_width, vbl_queue[0].dest_address, vbl_queue[0].dest_width);
//  else splat_640by480 (vbl_queue[0].source_address, vbl_queue[0].source_width, vbl_queue[0].dest_address, vbl_queue[0].dest_width);
//  vbl_queue[0].queued_for_splat=0;
//  return;
//  }
//  else	//slot 1 is newer than slot 0
//  {
//  if (Mode320==1) splat_320by240(vbl_queue[1].source_address, vbl_queue[1].source_width, vbl_queue[1].dest_address, vbl_queue[1].dest_width);
//  else splat_640by480 (vbl_queue[1].source_address, vbl_queue[1].source_width, vbl_queue[1].dest_address, vbl_queue[1].dest_width);
//  vbl_queue[1].queued_for_splat=0;
//  return;
//  }
//}	//end of both are queued
//else	  //both are not queued
//{
// if (vbl_queue[0].queued_for_splat==1)
// {
//  if (Mode320==1) splat_320by240(vbl_queue[0].source_address, vbl_queue[0].source_width, vbl_queue[0].dest_address, vbl_queue[0].dest_width);
//  else splat_640by480 (vbl_queue[0].source_address, vbl_queue[0].source_width, vbl_queue[0].dest_address, vbl_queue[0].dest_width);
//  vbl_queue[0].queued_for_splat=0;
//  return;
// }
// else
// if (vbl_queue[1].queued_for_splat==1)
// {
//  if (Mode320==1) splat_320by240(vbl_queue[1].source_address, vbl_queue[1].source_width, vbl_queue[1].dest_address, vbl_queue[1].dest_width);
//  else splat_640by480 (vbl_queue[1].source_address, vbl_queue[1].source_width, vbl_queue[1].dest_address, vbl_queue[1].dest_width);
//  vbl_queue[1].queued_for_splat=0;
// }
//}
//
//}
//
////returns the address of the buffer to write into as a LS_CGrafPtr
////new routine, old dsp version below
//LS_CGrafPtr Get_back_buffer(void) {
//extern	LS_CGrafPtr	  backBuff;
//extern GWorldPtr offport;
//
//return offport;
//}

void screen_quit() 	//Call before quitting game
{
	
//unsigned char OutBusyFlag;
//	while (OutBusyFlag!=0)
//	{
//	DSpContext_IsBusy(mDisplayContext,&OutBusyFlag);
//	}

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING

	SetGDevice(origGDH);	//these are here so I can SEE them!
	SetPort(origPort);
	
	DSpContext_FadeGammaOut(NULL, NULL);
	DSpContext_SetState(mDisplayContext, kDSpContextState_Inactive);
#endif //DISABLED_BY_ROB_WHEN_GETTING_RUNNING

	LS_ShowCursor();
	
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
	DSpContext_FadeGammaIn(NULL, NULL);

	DSpContext_Release(mDisplayContext);
	DSpShutdown();
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING

}

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING

//Mod SB 230599 so as not to change resolution
void GraphicsSelectContext()
{
extern	      CTabHandle mZexCLUT;
extern int Change_res;
ColorSpec	trueGreen	= { 0, 0x0000, 0xFFFF, 0x0000 };
OSStatus		theError;
DisplayIDType	displayID;
int wanted_width, wanted_height;
GDHandle current_device;
PixMapHandle GDevice_pixmapH;


wanted_width=640;
wanted_height=480;	//zex native resolution

if (Change_res==0)
{
	//get res of main display
	current_device=GetGDevice();
	GDevice_pixmapH=(**current_device).gdPMap;
	wanted_width=(**GDevice_pixmapH).bounds.right - (**GDevice_pixmapH).bounds.left;
	wanted_height=(**GDevice_pixmapH).bounds.bottom - (**GDevice_pixmapH).bounds.top;
//calc splat offsets - h has to be mod 8
splat_offset_x=(wanted_width>>1)-320;
splat_offset_x = splat_offset_x & 0xfffffff8;

splat_offset_y=(wanted_height>>1) - 240;

}	

else

{ splat_offset_x=0; splat_offset_y=0; }

	//€	Initialize our context description
	GraphicsInitAttributes(&mDisplayAttributes);

	//€	Tell DrawSprocket the exact needs we have for our game
	mDisplayAttributes.displayWidth		            	= wanted_width; //640;
	mDisplayAttributes.displayHeight		= wanted_height; //480;
	mDisplayAttributes.colorNeeds		            	= kDSpColorNeeds_Require;
	mDisplayAttributes.backBufferDepthMask	      = kDSpDepthMask_8;
	mDisplayAttributes.displayDepthMask		    = kDSpDepthMask_8;
	mDisplayAttributes.backBufferBestDepth	      = 8;
	mDisplayAttributes.displayBestDepth		    = 8;
	mDisplayAttributes.pageCount			= 1;	//not used any more....
	mDisplayAttributes.colorTable		            	= mZexCLUT;
//    mDisplayAttributes.frequency   = Long2Fix(67);
	//	  Find the best context for these attributes
	//	  This call will check to see if multiple monitors can handle the request
	//	  If there is more than one valid device it will put up the selection interface
//	theError = DSpFindBestContext(&mDisplayAttributes, &mDisplayContext); //enable for release

	theError = DSpUserSelectContext(&mDisplayAttributes, 0L, nil, &mDisplayContext); //disable for release

	if (theError)
		report_error_c("A suitable display could not be found.","",666);


//	mDisplayAttributes.contextOptions |= kDSpContextOption_PageFlip; //if possible give us hw page flipping
//	mDisplayAttributes.contextOptions |= kDSpContextOption_PageFlip; //if possible give us hw page flipping
//	mDisplayAttributes.contextOptions |= kDSpContextOption_DontSyncVBL; //dont sync to vbl
	
	//	  Set the blanking color to black
	DSpSetBlankingColor(&rgbBlack);

	//	  Find the DisplayID for this draw context and then find the GDevice
	//	  that goes along with it.  We need this because if we need to perform
	//	  QuickDraw operations in a context that isn't on the main monitor then
	//	  we need to have the proper GDevice set or QuickDraw dies.
	theError =  DSpContext_GetDisplayID(mDisplayContext, &displayID);
	if (theError)
		report_error_c("DSpContext_GetDisplayID() had an error.","",theError);

	theError = DMGetGDeviceByDisplayID(displayID, &mZexGDH, false);
	if (theError)
		report_error_c("DMGetGDeviceByDisplayID() had an error.","",theError);

	//	  Reserve the draw context so that we get the attributes we want
	theError = DSpContext_Reserve(mDisplayContext, &mDisplayAttributes);
	if (theError)
		report_error_c("Could not reserve display context.","",theError);

  	  // Fade the screen to black and make our context activ
	DSpContext_FadeGammaOut(NULL, NULL);
	GraphicsActive();
	
//AND THE FIX FOR BAD COLOURS WAS...we needed to set all the clut entries
    theError=DSpContext_SetCLUTEntries(mDisplayContext, (**mZexCLUT).ctTable, 0, 255);
	if (theError)
		report_error_c("CLUT error","",theError);
	theError=DSpContext_SetVBLProc(mDisplayContext, ZexVBLProc,0);
	
//	DSpContext_SetCLUTEntries (mDisplayContext, &trueGreen,9,1);	//fix to get green
	DSpContext_FadeGammaIn(NULL, NULL);

}
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
void GraphicsInitAttributes(DSpContextAttributes *inAttributes)
{
	if (! inAttributes)
		report_error_c("Nil context passed to GraphicsInitAttributes().","",-1);

   inAttributes->frequency=0;
   inAttributes->displayWidth=0;
   inAttributes->displayHeight=0;
   inAttributes->reserved1=0;
   inAttributes->reserved2=0;
   inAttributes->colorNeeds=0;
   inAttributes->colorTable=NULL;
   inAttributes->contextOptions=0;
   inAttributes->backBufferDepthMask=0;
   inAttributes->displayDepthMask=0;
   inAttributes->backBufferBestDepth=0;
   inAttributes->displayBestDepth=0;
   inAttributes->pageCount=0;
   inAttributes->gameMustConfirmSwitch=false;
   inAttributes->reserved3[0]=0;
   inAttributes->reserved3[1]=0;
   inAttributes->reserved3[2]=0;
   inAttributes->reserved3[3]=0;

   inAttributes->filler[0]=0;
   inAttributes->filler[1]=0;
   inAttributes->filler[2]=0;


//	  BlockZero(inAttributes, sizeof (DSpContextAttributes));
}
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING

void GraphicsActive(void)
{
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
OSStatus	theErr;
	SetGDevice(origGDH);	//these are here so I can SEE them!
	SetPort(origPort);

	
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING
	LS_HideCursor();
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING

	theErr = DSpContext_SetState(mDisplayContext, kDSpContextState_Active);
	if (theErr)
		DebugStr("SetState active failed in GraphicsActive().");
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING
}


void GraphicsInactive(void)
{
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
OSStatus	theErr;
	SetGDevice(origGDH);	//these are here so I can SEE them!
	SetPort(origPort);

	theErr = DSpContext_SetState(mDisplayContext, kDSpContextState_Inactive);
	if (theErr)
		DebugStr("SetState inactive failed in GraphicsActive().");
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING
}


void
GraphicsPaused(void)
{
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
	SetGDevice(origGDH);	//these are here so I can SEE them!
	SetPort(origPort);

	DSpContext_SetState(mDisplayContext, kDSpContextState_Paused);
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING
}


void display_wait()
{
 user_delay(2);

}


unsigned long red_values[18]=
     {
     0xe0e0e0e0,
     0xdfdfdfdf,
     0xdededede,
     0xdddddddd,
     0xdcdcdcdc,
     0xdbdbdbdb,
     0xdadadada,
     0xd9d9d9d9,
     0xd8d8d8d8,
     0xd7d7d7d7,
     0xd8d8d8d8,
     0xd9d9d9d9,
     0xdadadada,
     0xdbdbdbdb,
     0xdcdcdcdc,
     0xdddddddd,
     0xdededede,
     0xdfdfdfdf,     };

unsigned long green_values[18]=
     {
     0xeaeaeaea,
     0xe9e9e9e9,
     0xe8e8e8e8,
     0xe7e7e7e7,
     0xe6e6e6e6,
     0xe5e5e5e5,
     0xe4e4e4e4,
     0xe3e3e3e3,
     0xe2e2e2e2,
     0xe2e2e2e2,
     0xe3e3e3e3,
     0xe4e4e4e4,
     0xe5e5e5e5,
     0xe6e6e6e6,
     0xe7e7e7e7,
     0xe8e8e8e8,
     0xe9e9e9e9,
     0xeaeaeaea,     };
     
unsigned long blue_values[18]=
     {
     0xf3f3f3f3,
     0xf2f2f2f2,
     0xf1f1f1f1,
     0xf0f0f0f0,
     0xefefefef,
     0xeeeeeeee,
     0xedededed,
     0xecececec,
     0xebebebeb,
     0xebebebeb,
     0xecececec,
     0xedededed,
     0xefefefef,
     0xf0f0f0f0,
     0xf1f1f1f1,
     0xf2f2f2f2,
     0xf3f3f3f3,
     0xf4f4f4f4,     };
    
int red_start_count=0;
int red_direction=1;

int blue_start_count=170;
int blue_direction=1;

int green_start_count=380;
int green_direction=1;	//down


void red_bar()
{
extern Ptr screenaddr;
extern int screenwidth;
unsigned long *vram;
unsigned long *vram_line_pointer;
unsigned long colour;
int line,x;
int red_counter=0;


vram=(unsigned long*)screenaddr;
vram+=screenwidth*red_start_count/4;	//pointer to longs remember
vram_line_pointer=vram;

for (line=red_start_count; line<red_start_count+17;line++)
    {
	vram=vram_line_pointer;
    colour=red_values[red_counter];
    for (x=0;x<640/4;x++)
       {
        *vram=colour;
        vram++;
       }
    vram_line_pointer+=screenwidth/4;
    red_counter+=1;
    if (red_counter>17) red_counter=0;	//an "oops" check
    }
    
red_start_count+=red_direction;
if (red_start_count>400) 
{
red_direction=-2;
}
if (red_start_count<6) red_direction=+1;

}


void green_bar()
{
extern Ptr screenaddr;
extern int screenwidth;
unsigned long *vram;
unsigned long *vram_line_pointer;
unsigned long colour;
int line,x;
int green_counter=0;


vram=(unsigned long*)screenaddr;
vram+=screenwidth*green_start_count/4;	//pointer to longs remember
vram_line_pointer=vram;

for (line=green_start_count; line<green_start_count+17;line++)
    {
	vram=vram_line_pointer;
    colour=green_values[green_counter];
    for (x=0;x<640/4;x++)
       {
        *vram=colour;
        vram++;
       }
    vram_line_pointer+=screenwidth/4;
    green_counter+=1;
    if (green_counter>17) green_counter=0;	//an "oops" check
    }
    
green_start_count+=green_direction;
if (green_start_count>400) 
{
green_direction=-2;
}
if (green_start_count<6) green_direction=+1;

}


void blue_bar()
{
extern Ptr screenaddr;
extern int screenwidth;
unsigned long *vram;
unsigned long *vram_line_pointer;
unsigned long colour;
int line,x;
int blue_counter=0;


vram=(unsigned long*)screenaddr;
vram+=screenwidth*blue_start_count/4;	//pointer to longs remember
vram_line_pointer=vram;

for (line=blue_start_count; line<blue_start_count+17;line++)
    {
	vram=vram_line_pointer;
    colour=blue_values[blue_counter];
    for (x=0;x<640/4;x++)
       {
        *vram=colour;
        vram++;
       }
    vram_line_pointer+=screenwidth/4;
    blue_counter+=1;
    if (blue_counter>17) blue_counter=0;	//an "oops" check
    }
    
blue_start_count+=blue_direction;
if (blue_start_count>400) 
{
blue_direction=-2;
}
if (blue_start_count<6) blue_direction=+1;

}
//void my_flashy_erase_back()
//{
//extern Ptr screenaddr;
//extern int screenwidth;
//unsigned long *vram;
//unsigned long colour;
//int line,x;
//int red_counter=colour_start_count;
//int red_inc=1;
//
//vram=(unsigned long*)screenaddr;
//
//for (line=0; line<480;line++)
//    {
//    colour=red_values[red_counter];
//    for (x=0;x<640/4;x++)
//       {
//        *vram=colour;
//        vram++;
//       }
//    red_counter+=red_inc;
//    if (red_counter>17) red_counter=0;
//    }
//colour_start_count+=3;
//if (colour_start_count>17) colour_start_count-=17;
//}


void do_down_bars()
{
if (red_direction>0) red_bar();
if (blue_direction>0) blue_bar();
if (green_direction>0) green_bar();

}

void do_up_bars()
{
if (red_direction<0) red_bar();
if (green_direction<0) green_bar();
if (blue_direction<0) blue_bar();

}


// ********************

#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING

Handle splat_code_handle;
UniversalProcPtr splat_code_ptr;

//(const void * source_bitmap, int source_rowbytes, int source_width, int source_height, void * dest_bitmap, 
//int dest_rowbytes)
unsigned long splatProcInfo=0xFFFC1;	//7 in as above, 0 out, C

//NOTE: Using 3.2 headers knewcopy et. al are obsolete
//This function uses the new names 
void load_screen_splat()
{
int hsize;
OSErr my_err;
CFragConnectionID splat_conn_id;
Str255 ErrName;
Str63 fragname = "FantScreenSplat";
ProcPtr splat_main_addr;



splat_code_handle=LS_GetResource('Fppc',128);
if (splat_code_handle==0) report_error_c("Corrupt ZEX. Couldn't load Fppc 128 code resource.","",4);
MoveHHi(splat_code_handle);
LS_HLock(splat_code_handle);
hsize=GetHandleSize(splat_code_handle);
my_err=GetMemFragment(*splat_code_handle, hsize, fragname, kPrivateCFragCopy, &splat_conn_id, 
               (Ptr *) &splat_main_addr,ErrName);

if (my_err!=0) report_error_c("Couldn't link Fppc code resource. Try giving ZEX more memory.","",my_err);
splat_code_ptr=NewRoutineDescriptor (splat_main_addr,splatProcInfo, GetCurrentISA());

}
#endif // DISABLED_BY_ROB_WHEN_GETTING_RUNNING

// *******************Exec watchdog functions

void reset_watchdog()
{
exec_watchdog=700;	//made this 700 frames after running zex off a Zip disk!
}

void enable_watchdog()
{
exec_watchdog=700;
gCheckWatchDog=1;
}

void disable_watchdog()
{
gCheckWatchDog=0;
}



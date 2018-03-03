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

/*йLightsoft 1998/9
060599 - Put under CVS
Zex 1
Here is what people have been up to:
$Header: /home/ls_cvs/Zex2005/Code/Main/ZEX.cpp,v 1.60 2006/09/09 09:48:58 stu Exp $
$Log: ZEX.cpp,v $
Revision 1.60  2006/09/09 09:48:58  stu
Zex2005: Code: Static lasers now affected by the laser range multiplier. Default thrust mode set back to combat (since we display a message about switching thrust modes shortly after starting).

Revision 1.59  2006/08/16 22:53:38  stu
Zex2005: Code: Forgot to subject thrust note training or tutorial modes. Also Galactic map note no-longer only on Space Cadet level.

Revision 1.58  2006/08/16 22:34:14  stu
Zex2005: Code: Added in-game note about thrust modes.

Revision 1.57  2006/08/15 21:49:47  stu
Zex2005: Code: Make sure monsters can't fire when game frozen.

Revision 1.56  2006/08/06 17:37:11  stu
Zex2005: Code: Can now sell all. Now get dialog when logged out of Galanet due to no credits, and the same when trying to log in. Can no-longer jettison cargo when docked.

Revision 1.55  2006/07/01 22:15:39  stu
Zex2005: Code: Now only show dock and gm keys if space cadet

Revision 1.54  2006/06/28 23:00:20  stu
Zex2005: Code: just a comment added

Revision 1.52  2006/06/25 11:16:14  rob
Zex2005: Game has now got a blacklist list which can be used to remove rogue serial numbers flying around the internet when we update the application.

Revision 1.51  2006/06/24 21:31:56  rob
ZEX: Nag screen - every 20 minutes for an unregistered game - currently only when flying about.

Revision 1.50  2006/06/19 22:03:02  rob
Zex: Memory check to include display plus screen checking.

Revision 1.49  2006/06/19 20:48:07  rob
Zex: Added memory debug

Revision 1.48  2006/02/26 00:08:38  rob
Zex2005: Added callsign debug and fixed init_memory.cpp build problem (oops!).

Revision 1.47  2006/02/21 21:15:05  stu
Zex: Code: Fix for #116

Revision 1.46  2006/02/20 23:06:05  rob
Zex2005: Allow res switch when game pause change to inhibit only during main menu (and sub menu's - specifically key change menu).

Revision 1.45  2006/02/05 19:30:10  rob
Zex2005: Debug stuff, currently disabled.

Revision 1.44  2006/01/22 17:14:48  rob
Zex2005: Debugging extensions

Revision 1.43  2006/01/22 12:39:23  rob
Zex2005: Switch res in game with a key plus optional callsign check (both UNFINISHED).

Revision 1.42  2006/01/12 21:30:28  rob
Zex2005: Compiler version printed if mouse button held down to freeze action on booting screen.

Revision 1.41  2006/01/08 00:39:09  rob
Zex2005: Commented out code - might be useful for future debugging.

Revision 1.40  2006/01/04 23:10:50  stu
Zex2005: A variety of mothership bugs fixed and some other stuff - object movement now frozen when displaying GM if Space Cadet, trinary mode GM (off as booked in), ZEX_LIVES_FOREVER now gives you everything and rank 3.

Revision 1.39  2005/12/30 09:33:51  stu
Zex2005: Code: Added logging code to log max number of dyn objects and visible polys. Adjusted accordingly.

Revision 1.38  2005/12/30 06:05:58  stu
Zex2005: Code: Bug #41 - quitting in a motehrship should show your score.

Revision 1.37  2005/12/28 04:07:59  stu
Zex2005: Code: Upped the max number of visibl polys as we'vew had a stop because not enough. Now 3000 from 2000. MAX_NUMBER_OF_VISIBLE_POLYS in 3d_top_int.h

Revision 1.36  2005/12/25 09:27:22  stu
Zex2005: Code: Fix for fuel display not updating when launching from dock (Bug #64)

Revision 1.35  2005/12/23 22:45:57  rob
Zex2005: Remove frame rate display and invunerability for release.

Revision 1.34  2005/12/22 22:25:19  stu
Zex2005: Code: Changes so Zex doesn't get damaged and doesn't run out of fuel (set in options.h)

Revision 1.33  2005/12/20 21:21:44  stu
Zex2005: Code: Cross platform beta expiry code.

Revision 1.32  2005/12/20 14:56:00  rob
Zex2005: Ensure start time is always initialised with something, even if we are quitting.

Revision 1.31  2005/12/11 03:07:37  stu
Zex2005: Code: Fix for bug 38, DLP not cleared between games.

Revision 1.30  2005/12/11 01:42:23  stu
Zex2005: Code: Fix for bug 52, control panel displayed on quit.

Revision 1.27  2005/12/10 22:57:49  stu
Zex2005: Code: Training missions should now end correctly

Revision 1.26  2005/12/10 16:26:02  rob
Zex2005: Fixed black cursor/double cursor in inventory screen 2 plus no cursor visible in buy screens. (Also added some pauses to avoid 100% processor usage.)

Revision 1.25  2005/12/10 10:50:44  rob
Zex2005: Fixed full screen cursor flicker (hopefully) by implementing our own cursor.

Revision 1.24  2005/11/30 21:09:35  stu
Zex2005: Further bug fixes

Revision 1.23  2005/11/29 23:22:41  stu
Zex2005: Code: Now goes back to main menu at end of training missions; no hiscores for training, hiscore clipping fixed and now prints 'Left, right and fire to enter name'. Fixes a potential crash bug in Ionic device.

Revision 1.22  2005/11/26 21:33:59  rob
Zex2005: Added pause on mouse click, so anyone can see the boot text before it scrolls away too quickly.

Revision 1.21  2005/11/16 00:11:07  stu
Zex2005: Code: Inserted delay in credits screen

Revision 1.20  2005/11/12 23:52:26  stu
Zex2005: Code: Removed the frame rate and objects display

Revision 1.19  2005/10/10 21:31:19  rob
Zex2005: Added newline at end to stop compiler whinging.

Revision 1.17  2005/10/02 23:20:38  rob
Zex2005: Not sure how we used to get from source copy (i.e. background overwrite) Text Mode to source or (i.e. background no write) in Zex. Doesn't seem there is a way apart from buy.cpp and inventory.cpp. Bizzare. Anyhow, this fixes that problem.

Revision 1.16  2005/09/30 00:00:52  stu
Zex2005: Code:x86 - fixed crashes to do with statics (they appear to have been overridden by extern declarations within the file - hmmm)

Revision 1.15  2005/09/29 00:48:43  stu
Zex2005: Code: Basic x86 port

Revision 1.14  2005/08/01 22:19:07  rob
Zex2005: Added two clip routines so show inventory and Galaxy would display properly. (Also reformatted whitespace of code for main loop.)

Revision 1.13  2005/07/19 19:07:28  rob
Zex2005: Get rid of constant image load for non-OGL build

Revision 1.12  2005/06/04 18:25:45  rob
Zex2005: Load screen changes - centred title, removed later unnecessary messages, increased delay on each message (so it looks more impressive) and replaced delay algorithm with SDL delay function to (perhaps) return CPU time to system.

Revision 1.11  2005/06/01 19:07:22  rob
Zex2005: Updated project - now runs main menu.

Revision 1.10  2005/05/23 21:13:07  rob
Zex2005: More changes to files.

Revision 1.9  2005/05/17 13:51:27  rob
Zex2005: More file changes.

Revision 1.8  2005/05/15 17:42:43  rob
Zex2005: ZexNumToString used throughout, file case changes, general gcc4.0 build changes.

Revision 1.7  2005/05/04 17:32:42  rob
Zex2005: More changes for build.

Revision 1.6  2005/05/02 10:32:28  rob
Zex2005: More Updates (code)

Revision 1.5  2005/04/25 20:40:09  rob
Zex2005: Check in flush events.

Revision 1.3  2005/04/24 10:10:15  rob
ZEX: Started to work on ZEX.cpp. Replaced TickCount. Added some void return types. Commented out some Mac init functions. Altered main.cpp to do the correct thing on an exit().

Revision 1.2  2005/04/23 20:14:43  rob
ZEX: First set of changes for SDL build. (remainder of files)

Revision 1.52  1999/11/23 17:42:07  stu
0.9.4.

Revision 1.51  1999/11/07 19:12:06  stu
Adds level_game for magnetic anomolies etc.

Revision 1.50  1999/11/07 17:33:28  stu
*** empty log message ***

Revision 1.49  1999/11/07 04:04:16  stu
0.94.3
071199 SB

Revision 1.48  1999/11/01 19:58:31  stu
OGL Text - SB 011199

Revision 1.47  1999/11/01 02:28:29  stu
1st Nov 99 SB

Revision 1.46  1999/10/01 04:11:44  stu
0.94.2 011099 SB

Revision 1.45  1999/09/27 14:49:26  stu
Fixes NPC's bust on 260999

Revision 1.44  1999/09/26 18:34:37  stu
260999

Revision 1.43  1999/09/20 19:03:31  stu
0.93.3

Revision 1.42  1999/09/19 05:38:45  stu
0.93 as posted

Revision 1.41  1999/09/14 15:06:12  stu
140999

Revision 1.40  1999/09/12 00:56:08  stu
120999

Revision 1.39  1999/08/30 22:22:53  stu
300899

Revision 1.38  1999/08/20 09:57:26  stu
20 Aug 99

Revision 1.37  1999/08/15 22:16:58  stu
Cleanup 150899

Revision 1.36  1999/07/27 05:35:09  stu
Final update for test 270799

Revision 1.35  1999/07/26 19:08:40  stu
Pre 0.86 test 27 July 99

Revision 1.34  1999/07/20 06:04:24  stu
More work on level 4

Revision 1.33  1999/07/19 17:42:21  stu
200799

Revision 1.32  1999/07/16 11:40:03  rob
Made it start on level 5, whether you want it or not!

Revision 1.31  1999/07/13 04:07:10  stu
Clean up 130799

Revision 1.30  1999/07/09 06:03:11  stu
Clean up 09-7-99

Revision 1.29  1999/07/04 18:37:16  stu
Clean up, Sunday 4th july

Revision 1.28  1999/07/04 00:21:23  stu
Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86

Revision 1.27  1999/06/28 22:35:10  stu
Changes for Change Camera View Mod (C Key)
Enables MODE 1 which is in-cockpit view

Revision 1.26  1999/06/27 19:27:41  stu
Clean up 27 June 99

Revision 1.25  1999/06/25 01:12:54  stu
Epilogue text control

Revision 1.24  1999/06/23 00:51:46  stu
Changes for new lighting control

Revision 1.23  1999/06/22 10:57:32  stu
Lurkers clean up

Revision 1.22  1999/06/20 05:40:04  stu
Changes for Level 2

Revision 1.21  1999/06/16 00:25:57  stu
FPS and other info removed for now.

Revision 1.20  1999/06/14 07:25:40  stu
*** empty log message ***

Revision 1.19  1999/06/10 04:37:49  stu
Fix Hull on load_game

Revision 1.18  1999/06/10 03:15:48  stu
No changes

Revision 1.17  1999/06/08 23:07:47  stu
Final checkin before cvs pro client upgrade.

Revision 1.16  1999/06/06 00:30:20  stu
Adds Choone 132 for when Zex dies.

Revision 1.15  1999/06/05 17:25:53  stu
Lurkers clear up commit.

Revision 1.14  1999/05/25 04:49:34  stu
*** empty log message ***

Revision 1.13  1999/05/24 23:45:47  rob
Added millisecond timing.

Revision 1.12  1999/05/24 16:17:36  stu
Additional timing points over the 3d engine:
Matrix, Culling and Pers.

Revision 1.11  1999/05/23 21:37:32  rob
1. Added percentage timing ability to Zex
2. Rearranged headers in project file

Revision 1.10  1999/05/22 12:16:03  stu
This update finalises the changes to the VBL screen driver (Hopefully :)) Now fixed at two buffers so a few things in Zex that assumed 3 buffers need changing but it basically works.

Revision 1.9  1999/05/21 03:31:22  stu
1. New screen driver.
2. VBL Driven music fade and play

Revision 1.8  1999/05/17 18:12:20  stu
1.7 - Now switches back to 640*480 when docked.


Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c

Revision 1.7  1999/05/17 12:54:09  stu
1.7 - Now switches back to 640*480 when docked.


Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c

Revision 1.6  1999/05/17 04:34:33  stu
Basic Support for pixel doubled screen. This change affects 23 files. See Splat_Control.c to turn off by Setting Use_Mode320 to 0. Basic pixel doubler also in Splat_Control.c

Revision 1.5  1999/05/06 17:48:54  stu
*** empty log message ***

Revision 1.4  1999/05/06 17:30:05  stu
Very important bug fix to prevent crash when starting next level. Reset_3d was not being called before starting a level. Initially reported in the 0.83 test (or at least I think this is the same bug).

Experiment with CVS Log and Header replacements

Revision 1.3  1999/05/06 16:56:17  stu
Very important bug fix to prevent crash when starting next level. Reset_3d was not being called before starting a level. Initially reported in the 0.83 test (or at least I think this is the same bug).

Experiment with CVS Log and Header replacements


 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include <stdlib.h>
#include <time.h>

#include "ZEX.h"

//oldmac//#include <quickdraw.h>
//oldmac//#include <SegLoad.h>
//oldmac//#include <timer.h>
//oldmac//#include <NumberFormatting.h>
#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif
#include "game_defs.h"

//oldmac//#include	<Processes.h>
#include	"Z_init.h"
#include	"sound.h"
#include	"splat_control.h"
#include	"Error_handler.h"
#include	"drawing.h"
#include "3d_top.h"
#include "camera.h"
#include "init_memory.h"
#include "ship_control.h"
#include	"object_control_top.h"
#include "explode.h"
#include "txtsplat.h"
#include "do_intro.h"
#include "LS_Standard.h"
#include "fpoly4.h"
#include "CPanel.h"
#include "load_objects.h"
#include "main_sel_screen.h"
#include "input_control.h"
#include "animation.h"
#include "zex_misc.h"
#include "pictures.h"
#include "dlp.h"
#include "game_pause.h"
#include "refuel.h"
#include "level_control.h"
#include "handle_level.h"
#include "docked.h"
#include "debug_timing.h"
#include "show_stats.h"
#include "hiscore.h"
#include "inventory.h"
#include "instrum_int.h"
#include "Batcom.h"
#include "Damage.h"
#include "light_control.h"
#include "OGL.h"
#include "load_objects.h"
#include "object_misc.h"
#include "motherships.h"
#include "Galaxy.h"
#include "map_control.h"
#include "file_io.h"
#include "real_time.h"
#include "buy_int.h"
#include "texture_management.h"
//#include "read_zex_ini.h"
#include "command_and_control.h"

#include "memory_debug.h"
#include "timing.h"
#include "register_game.h"
// ***********************************************************************************
// * CONSTANTS
// *

//#define equalise 1
//#define debug_info
//fps

//define the following to get various info up on screen
//#define timings
//#define copyright
//#define ship_pos
//#define object_info
//#define fps_info
//#define showfc
//#define eng_limits	//log max number of dyn objects, vis polys



// ***********************************************************************************
// * LOCAL VARIABLES
// *


//oldmac//#if !(MAC68K)
//oldmac//QDGlobals qd;
//oldmac//#endif
int quit=0;	//global - if set to 1 we quit game
int difficulty,tutorial;
int show_fps;
int training_mission_complete;
int game_pause;	//1=pause, 0=run
int game_over=0;
int number_of_polys_displayed;
int frame_counter;
int real_frame_counter;
int objs_displayed;
int do_launch,do_next_level;
int done_load_flag;	//from main sel screen - if 1 we've loaded ship from saved game
int ask_for_load;	//when we come bak from game if this is set we go straight into new game from load game
int next_level;
int have_initted;
static int dynamic_max=0;	//debug
static int number_of_visible_polys_max=0; //debug
#if PCI_TIMING_DEBUG
LS_UnsignedWide stu_timer1;
#endif
int var_a,var_b;	//debug vars
int fps;	//global frames per second reading
int end_of_level_code;	//if not zero, we display end_of_level_reason_string
char end_of_level_reason_string[512];	//we can print this if eolc is not zero
int in_wormhole;
int training_mission,game_running;	//>0 if we are in galaxy

static int first_frame;

extern int Use_SpeedLimit,user_abort;
extern int exec_watchdog;
extern int wingmen_guarding_what_object;	//the object wingmen are guarding
extern UInt32 gLaser;
extern int global_control_monsters;
extern int  level_has_control,level_has_camera_control;
extern int show_control_panel,gShowGalacticMap;

extern LSRAW *inv_picture_rgba;
extern LSRAW *inv_cursor_picture_rgba;
extern int inv_pict_reference;	//from contant images or -1 if not set
extern int inv_cursor_reference;
extern LSRAW *inv_picture;
int training_complete;	//if 1, game end - set from training missions
extern int global_show_status;

// ***********************************************************************************
// * TEST VARIABLES
// *

// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *


void render_frame(int width, int height);
void render_frame_no_erase(int width, int height);
void render_frame_colour_bg(int width, int height, int colour);
bool check_beta_expiry(void);
void print_compiler(void);
void periodic_freeze_processing(void);
void reset_bootup_messages(void);
void handle_bootup_messages(void);
// ***********************************************************************************
// * ACTUAL CODE
// *

//ееееееееееееееееееееееееееееееееееееееееееееMAIN
void zex_main(void) {
	//extern DynObjectsFixedSize *ocb_ptr;
	//extern _3D ship_rotated_normal; //from engine
	//extern _3D camera_pos;
	//extern	int ship_y_ang,ship_x_ang,ship_z_ang
	extern int level_complete;
	extern int mothership_cat;
	//extern int	dynamic_object_index,thrust;
	extern int zex_alive,death_frame_counter;	//how many dynamic objects to look at
												//extern _3D light_normal;
												//extern int music_volume;
												//extern int gTurn;
	
	debug_printf(debug_overview, " -------------------------------- \n");
	debug_printf(debug_overview, "Zex LAUNCHED \n");
	extern int number_of_stars;
	//extern Ptr screenaddr;
	//extern int screenwidth;
#if PCI_TIMING_DEBUG
	extern LS_UnsignedWide stu_timer1;
#endif
	extern int have_bpic;
	extern int have_ejected;
	//extern int score;
	extern int current_level,command_slot_dialog;
	extern int Mode320,Use_Mode320,gupdate_cp,docked_at_time;
	//extern int camera_mode;
	extern int credits;
	int execute_quicksave;
	
	//oldmac//GrafPtr		savePort;
	//oldmac//GDHandle	saveGDH;
	//oldmac//LS_CGrafPtr the_drawing_buffer;
	
	int	cant_run=0;
	
	unsigned char number_string[12];
	int start_time=LS_TickCount60();		// start time is always initialised with something, even if we are quitting
	int end_time;
	//oldmac//LS_UnsignedWide start_micros,end_micros;
	int next_frame_start_time;	//equaliser
								//char test_string[]="TEST";
								//LS_UnsignedWide test_timer;
	int had_bg_image;
	
	//unsigned long date;
	
	//extern const RGBColor	rgbWhite;
	//extern const RGBColor	rgbGreen;
	//_ZPoly *the_polys;
	//oldmac//extern GDHandle				mZexGDH;
	//extern int gCheckWatchDog;
	extern int global_open_doors;	//door master on/off switch
	extern int quicksave_available,warphole_slot;
	//еееееееееееStart
	have_initted=0;
	show_fps=0;			//display fps by default?

	//oldmac//MaxApplZone();	//almost mandatory for any mac prog these days (sadly).
	cant_run |= init_mac();	//do this or die...
							//oldmac//	GetPort(&savePort);
							//oldmac//	saveGDH = GetGDevice();	//these are here so I can SEE them being done
							//	show_loading_screen();	//in misc
	
#if DISABLED_BY_ROB_WHEN_GETTING_RUNNING
	reset_watchdog();
	disable_watchdog();
#endif	// DISABLED_BY_ROB_WHEN_GETTING_RUNNING
	
	InitText();				// required for showing text in loading screen
	
	//show_text_in_loading_screen("Exec: Initialising Memory ");	  
	
	cant_run |= init_ocbs();
	
	//show_text_in_loading_screen("Exec: Initialising F4");	
	
	//read_zex_ini();


	cant_run |= init_game();	//as it says
	
	cant_run|= check_beta_expiry();
	
	show_text_in_loading_screen("Exec: Loading 32 bit graphics...");	
	
	load_32bit_graphics();
	
	show_text_in_loading_screen("Exec: Checking we can run...");	
	if (cant_run) exit(1);	//stop if error before we get going
	
	
	show_text_in_loading_screen("Exec: Enabling Exec Watchdog");	
	
	enable_watchdog();	   //have to keep reseting this else we'll error
	show_text_in_loading_screen("Exec: Watchdog did not enable (Disabled by SB Aug 99)");	
	
	show_text_in_loading_screen("Exec: Resetting F4 OCB's");	
	
	reset_objects();	//clear out ocb's
	take_mouse_keyboard_as_input();
	show_text_in_loading_screen("Exec: Handing over to Zex");	
	
	show_text_in_loading_screen("Zex: Requesting free memory check...");	
	
	show_text_in_loading_screen("Exec: Final Prep.");	
	show_text_in_loading_screen("Zex: Setting GMUpdate to 76");
	set_map_update_rate(76);
	//show_text_in_loading_screen("Zex: NOTE: This test version expires 19 Oct 99.");
	//show_text_in_loading_screen("Exec: Checking Expiration");	  
	//date check
	//   GetDateTime(&date);
	//   if (date>0xb431968d) report_error("Zex Expired. Check www.lightsoft.co.uk/zex/","",-1);
	
	//user_delay(60);	
	check_game_parameters();			// check blacklist
	
	if(LS_Left_Button())
	   {
		   print_compiler();
		   show_text_in_loading_screen("[Game held for mouse up]");
		   while(LS_Left_Button())
		   {
			   user_delay(10);			// wait a few ms and test again
			   Show_it();				// allow mouse to move
		   }
	   }

	show_text_in_loading_screen("Zex: Good Luck Player!");	
	user_delay(60);

	
	Zex_ShowCursor(SDL_DISABLE);		// don't show cursor during movie
	do_intro();	  //run the intro bits
	Zex_ShowCursor(SDL_ENABLE);			// let's have the cursor on again...

	enable_inputs();

	Use_SpeedLimit=1;	//limit to ~36 fps?

    //fade_and_play_zex_resource(131);
	
	//   take_mouse_keyboard_as_input();	//120299 - take mouse and keyboard as input sprocket devices
	//
	//   insert_score(100,5);
	//   untake_mouse_keyboard_as_input();	  //120299 - take mouse and keyboard as input sprocket devices
run_main_menu:   
	training_mission_complete=0;
	clear_all_cont_sound_chans();	//clear out all continuous channels
	
	reset_watchdog();	//have to keep reseting this else we'll error
	quicksave_available=0;	//none available
	next_level=2;	//new game
	tutorial=0;
	disable_inputs();
	reset_objects();	//clear out ocb's
	reset_3D();	//reset engine vars
	have_bpic=0;
	init_dlp();
	done_load_flag=0;
	set_light_reset_amplitude(25000);
	set_erase_colour(255);
	//oldmac//show_text_in_loading_screen("Zex: Main Sel Screen");	
	
	key_res_switch_allowed(false);			// don't allow res switches in main menu or key setup
	do_main_sel_screen();
	key_res_switch_allowed(true);			// allow in load screen, main game, level intro
	
	training_complete=0;	//set to 1 to end training
	if (done_load_flag==1)
	{
		training_mission=0; tutorial=0; next_level=6;	//load game so load level 6
	}
	if (tutorial==1) { next_level=1; warphole_slot=-1;}
	if (training_mission==1) { next_level=2; warphole_slot=-1;}	//ionic device
	if (training_mission==2) { next_level=3; warphole_slot=-1;}	//Journey
	if (training_mission==3) { next_level=5; warphole_slot=-1;}	//Simulation
	TextMode(1);
	
run_next_level:

		init_ip_globs();
	set_erase_colour(255);
	clear_all_cont_sound_chans();	//clear out all continuous channels
	global_open_doors=0;
	global_control_monsters=1;	//allow monster moves
	user_abort=0;
	docked_at_time=0;
	wingmen_guarding_what_object=0;	//object wingmen are following
	remove_batcom_message();
	
	reset_3D();	//reset engine vars (important bug fix to stop crash when changing levels 6/5/99 SB)
	
	if (quit==0)	//if we're not quitting
	{   
		enable_inputs();
		//    reset_volume_after_fade();	//wait for fade to end and then reset the volume
		//    music_set_vol(music_volume);
		reset_watchdog();	//have to keep reseting this else we'll error
		
		//    fade_out_music();
		
		//    if (next_level==1 || next_level==2 || next_level==3)	  //1=tutorial, 2= start game
		//    {
		//oldmac//show_text_in_loading_screen("Zex: reinit_game");	
		reinit_game(next_level);	//sets mode320 as appropriate
									//    }
									//    else
									//    {
									//    reinit_game_for_next_level(next_level);
									//    reinit_game_for_next_level(next_level);	  	  //sets mode320 as appropriate
									//    }
									//oldmac//show_text_in_loading_screen("Zex: reinit_screen");	
									//    if(done_load_flag==0)
		reinit_screen();	//set up control panel etc - in drawing.c
		
		init_constant_image();
		//   if (Use_Mode320==1) Mode320=1;	//set up 320*240 screen
		
		//reset_ship();
		start_time=LS_TickCount60();
		//    play_tune(130);
		//reset_kill_list();
		//reset_explode_list();
		
		if (training_mission==0 && tutorial==0 && done_load_flag==0) 
		{
			global_open_doors=1;
			make_new_galaxy();
		}
		set_poly_clipping(0,639,0,388);	//set poly clip rect - sb 181298
		
		//    SetKeyboardMouseActivation(1);	//mouse and keyboard as input devices
		
    }
    reset_watchdog();	//have to keep reseting this else we'll error
						//show_text_in_loading_screen("Zex: camera setup");	
	
    camera_to_zex();
    do_next_level=0;	//if 1, we load next level
						//level_complete=2;
						//mothership_cat=baddie_object;
						//   if (done_load_flag==1) 
						//   {
						//   Show_Hull_NewLife();
						//   update_hull_damage();
						//   }
						//   done_load_flag=0;	     //load complete by here
	ask_for_load=0;	//when we come back in it means player loaded a game
	level_has_control=0;
	level_has_camera_control=0;
	in_wormhole=0;
	//   camera_mode=0;	//top down
	//insert_batcom_message("TEST1","TEST2");
	//   credits=10000;
	
	
	
#if OGL
	
	if (!quit && done_load_flag==0) 
    { 
		ogl_active();	//as it says, in ogl.c
		
		load_ogl_textures();
		//     ogl_load_control_panel();
		
	}
#endif
	first_frame=1;	//flag for main loop telling it is first frame
	
	if (quit==0) reinit_screen();
	
	if(done_load_flag==1) 
	{
		int loaded;
		loaded=load_game();
		if (loaded==0) goto run_main_menu;	//user canx load
		goto finish_load;
	}
	
	if (training_mission==0 && tutorial==0)
	{
	char print_string[132];
		add_to_text_display("WELCOME TO ZEX!",0); 
		if (difficulty==16)	//only on space cadet
		{
			sprintf(print_string,"Press '%s' to dock with a Mothership",SDL_GetKeyName(get_programmed_keycode(dock_request_key)));
			add_to_text_display(print_string,0); 
		}
		sprintf(print_string,"Press '%s' for Galactic Map",SDL_GetKeyName(get_programmed_keycode(galaxy_map_key)));
		add_to_text_display(print_string,0); 
	}
	do_fixed_frame_rate(FRAME_RATE_REINIT);
	force_fuel_update();	//Fix for bug 116
	
	check_all_memories();		// just before game loop
	reset_bootup_messages();
	//<<<<<<<<<<<<<<<Game loop>>>>>>>>>>>>>>>>>>>>>>>>>>>

	while(!quit)	//not end of game
	{
	extern kit_desc ship_items[ITEMS_COUNT];

	handle_bootup_messages();		//'V' to switch engine modes etc
	debug_printf(debug_verbose, "Start of frame %i\n", real_frame_counter);

	periodic_freeze_processing();	//update engine variables (e.g monsters_can_fire) if we are frozen
		
#if CALLSIGN_DEBUG_MODE
		parse_ocb_for_bad_callsigns();
#endif
		
		#if ZEX_LIVES_FOR_EVER
		ship_items[KNORMAL_FUEL_TANK].value1=100;
		#endif
		
		get_system_ticks();	//time for ZexTickCount
		
		if (training_mission==0 && tutorial==0) 
		{
			check_sleep();
			run_map_control();
		}
		
		
		//wait for camera to zoom in then gen initial dust
		if (first_frame==120)	//wait for camera zoom in
		{
			populate_dust_for_start();	//load in dust after map loaded
			first_frame=0;
			//add_to_text_display("GO!",0); 
			
		}
		if (first_frame>0) first_frame++;
		
		
		
		update_motherships();
		
		//if(training_mission==0 && tutorial==0) run_map_control();
		
		//if (fps>MAX_FRAME_RATE-6 && objs_displayed<180) gen_dust();
		
		if (number_of_stars<80) gen_dust();
		
		//if (fps>MAX_FRAME_RATE-8 && objs_displayed<65 && RangedRdm(0,fps)>MAX_FRAME_RATE-fps) gen_dust();	//prevent clumping
		//
		//if (objs_displayed<65 && RangedRdm(0,fps)>MAX_FRAME_RATE-fps) gen_dust();	//prevent clumping
		//else
		//if (number_of_stars<10) gen_dust();
		
		next_frame_start_time=(ZexTickCount()+2);	//30 fps
													//oldmac//Microseconds(&start_micros);
		
		run_periodic_sound_tasks();
		
		reset_watchdog();	    //have to keep reseting this else we'll error
		
		kill_kill_list();
		reset_kill_list();	//as it says
		
		handle_level();
		
		poll_input_device();
		apply_control_inputs();
		
		run_animations();	//in animation.c
		
		run_refuelling();	//see if we're over a refuel rect
		
		number_of_polys_displayed=0;
		frame_counter++;
		
		run_camera_control();	//in camera.c
		
		check_for_fuel_update();
		
		
		if (global_show_status==0)	//we can render the frame
		{
			
			if (show_control_panel==0)
			{
				
				if (Mode320==1)
				{
					set_poly_clipping(0,639>>1,0,388>>1);	     //set poly clip rect - sb 181298
					render_frame(640>>1,390>>1);	//in this file - calls all the needed 3d stuff
				}
				else  
				{
					set_poly_clipping(0,639,0,388);	//set poly clip rect - sb 181298
					render_frame(640,390);	      //in this file - calls all the needed 3d stuff
				}
				//    run_dlp();	  //run the 2d display processor - 2d stuff appears under 3d stuff
				
				if (in_wormhole==0) do_control_panel_control();
				
				run_dlp_text();	//run for text items and clear
				
			}
			else	//cp off
			{
				
				if (Mode320==1)
				{
					set_poly_clipping(0,639>>1,0,480>>1);	//set poly clip rect - sb 181298
					render_frame(640>>1,480>>1);	//in this file - calls all the needed 3d stuff
				}
				else  
				{
					set_poly_clipping(0,639,0,479);	//set poly clip rect - sb 181298
					render_frame(640,480);	//in this file - calls all the needed 3d stuff
				}
				//    run_dlp();	  //run the 2d display processor - 2d stuff appears under 3d stuff
				if (in_wormhole==0) do_control_panel_control();
				
				run_dlp_text();	//run for text items and clear
				
				
			}
			
			if (inv_pict_reference!=-1)	//the inventory picture is not resident
			{
				delete_constant_image(inv_pict_reference);
				inv_pict_reference=-1;
			}
			ogl_hide_mouse_cursor();
		}	//end of global show status==0
			//else	//show status screen
		else	//global_show_status==1
		{
			ogl_hide_radar_reticules();	//no radar reticules pse Mr Zex.
			if (Use_Mode320==1)
			{
				Mode320=0;
			}
			set_poly_clipping(0,639,0,479);	//set poly clip rect - rp 01-08-2005

			if (inv_pict_reference==-1)
			{
				//load up inv_picture to card
#if OGL==1
				inv_pict_reference = set_as_constant_image(inv_picture_rgba, 100, 100, 100+inv_picture_rgba->pwidth, 100+inv_picture_rgba->pheight);
#endif
			}
			do_inventory_single_frame(false);	//file buy.c
											//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
											//   glPixelStorei(GL_UNPACK_ROW_LENGTH, 314);
											////    glPixelStorei(GL_PACK_ALIGNMENT, 1); 
											//
											//    glRasterPos2f(int_to_ogl_x(100), int_to_ogl_y(100));
											//    glDrawPixels(314,278,GL_RGBA, GL_UNSIGNED_BYTE,
											//    inv_picture_rgba->raw_data);
											//	print_crunched32_trans_pict(inv_picture_rgba,100,100);
			
			if (Use_Mode320==1)
			{
				Mode320=1;
			}
			
			do_3d();	//3d stuff - transilates active object points to world_points_list_ptr
			
		}
		
		
		if (gShowGalacticMap>0)
		{
			
			if (Use_Mode320==1)
			{
				Mode320=0;
			}
			set_poly_clipping(0,639,0,479);	//set poly clip rect - rp 01-08-2005

			handle_galactic_map();	//file buy.c
			
			if (Use_Mode320==1)
			{
				Mode320=1;
			}
			
			do_3d();	//3d stuff - transilates active object points to world_points_list_ptr
			
		}
		
		
		if (in_wormhole==0) show_nav();	//show directional arrow
		
		check_all_memories();
		
#ifdef object_info
		ZexNumToString(number_of_polys_displayed,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("PD",20,70,10);
		SplatText_Transparent ((char *)number_string,70,70,10);
		
		
		ZexNumToString(objs_displayed,number_string);
		//ZexNumToString(dynamic_object_index,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("ACTIVE OBJS",20,130,10);
		SplatText_Transparent ((char *)number_string,120,130,10);
		
		
		ZexNumToString(number_of_stars,number_string);
		//ZexNumToString(dynamic_object_index,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("DUST",200,130,10);
		SplatText_Transparent ((char *)number_string,320,130,10);
		
#endif
		
#ifdef ship_pos
		ZexNumToString((*ocb_ptr).object_list[0].Oworldy,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("SHIPY",20,110,10);
		SplatText_Transparent ((char *)number_string,70,110,10);
		
		
		ZexNumToString((*ocb_ptr).object_list[0].Oworldx,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("SHIPX",120,110,10);
		SplatText_Transparent ((char *)number_string,170,110,10);
		
#endif
		
		//compiled option fps display
#ifdef fps_info
		ZexNumToString(fps,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("FPS",20,50,10);
		SplatText_Transparent ((char *)number_string,70,50,10);
#endif

#ifdef eng_limits
		extern int dynamic_object_index, number_of_polys_displayed;
		if (dynamic_object_index>dynamic_max) dynamic_max=dynamic_object_index;
		if (number_of_polys_displayed>number_of_visible_polys_max) number_of_visible_polys_max=number_of_polys_displayed;
		ZexNumToString(dynamic_object_index,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("Dcurr:",20,60,10);
		SplatText_Transparent ((char *)number_string,80,60,10);

		//max number of dyn objs
		ZexNumToString(dynamic_max,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("Dmax: ",20,72,10);
		SplatText_Transparent ((char *)number_string,80,72,10);

		//current number of visible polys
		ZexNumToString(number_of_polys_displayed,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("Pcurr: ",20,84,10);
		SplatText_Transparent ((char *)number_string,80,84,10);

		//max number of visible polys
		ZexNumToString(number_of_visible_polys_max,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("Pmax: ",20,96,10);
		SplatText_Transparent ((char *)number_string,80,96,10);

#endif
		
		
		//dynamic fps display
		if (show_fps==1)
		{
			ZexNumToString(fps,number_string);
			number_string[number_string[0]+1]=0;	//quick p to c hack
			number_string[0]=' ';
			SplatText_Transparent ("FPS",20,50,10);
			SplatText_Transparent ((char *)number_string,70,50,10);
		}
		
		
		
#if FIND_PERCENTAGES_DEBUG
		
		SplatText_Transparent ("%",20,110,10);
		
		
		{
			extern double timepercent[];
			extern double timeav[];
			int count;
			
			for(count=T_MIN; count<=T_MAX; count++)
			{
				ZexNumToString((int)(timeav[count]/100000),number_string);
				number_string[number_string[0]+1]=0;	//quick p to c hack
				number_string[0]=' ';
				SplatText_Transparent ((char *)number_string,70+(30*count),110,10);
			}
		}
		
#endif
		
		
		
#ifdef debug_info
		
		//stus debug timer
		ZexNumToString(camera_pos.x,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("x",20,70,10);
		SplatText_Transparent ((char *)number_string,130,70,10);
		
		
		ZexNumToString(camera_pos.y,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("y",20,90,10);
		SplatText_Transparent ((char *)number_string,130,90,10);
		
		var_a=find_dynamic_object_slot('MS10');
		var_a=(*ocb_ptr).object_list[var_a].shield_value;
		ZexNumToString(var_a,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("var_a",20,110,10);
		SplatText_Transparent ((char *)number_string,130,110,10);
		
		ZexNumToString(var_b,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("var_b",20,130,10);
		SplatText_Transparent ((char *)number_string,130,130,10);
		
#endif
		
		
		
		
#ifdef showfc
		ZexNumToString(fps,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("FPS",20,50,10);
		SplatText_Transparent ((char *)number_string,70,50,10);
		
		
		ZexNumToString(real_frame_counter,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("FR",20,90,10);
		SplatText_Transparent ((char *)number_string,70,90,10);
		
#endif
		
#if PCI_TIMING_DEBUG
		T(T_POSTDLP_END);
#endif
		if (command_slot_dialog!=-1)
			run_control_dialog();	//in command_and_control
		
#if OGL==1
		draw_constant_images();
#endif
		
		debug_printf(debug_verbose, "Main game - Show_it() on frame %i\n", real_frame_counter);
		Show_it();	//splat 3d portion of window

		debug_printf(debug_verbose, "Main game - past Show_it() on frame %i\n", real_frame_counter);

#if PCI_TIMING_DEBUG
		T(T_SWAP_END);
#endif
		//set up global addr and width vars for next frame
		//oldmac//the_drawing_buffer=Get_back_buffer();
		
		//enable next line to make a movie
		// DebugStr ("Taking Picture;FKEY 3;G");
		
		//Obj Control and collision
		
		
		do_object_control();
		
		debug_printf(debug_verbose, "Main game - past do_object_control() on frame %i\n", real_frame_counter);
		
#ifdef timings
		Microseconds(&end_micros);
		timer=(end_micros.lo-start_micros.lo);
#endif
		
		do_light_control();	//found in object_control_top
		explode_explode_list();
		reset_explode_list();	//as it says
#if PCI_TIMING_DEBUG
		start_timer(&stu_timer1);
		
#endif
		
		// if (in_wormhole==0) do_control_panel_control();
#if PCI_TIMING_DEBUG
		stop_timer(&stu_timer1);
#endif
		if(Use_SpeedLimit)
		{
			do_fixed_frame_rate(FRAME_RATE_DELAY);		//equalise frame rate (if enabled)
		}
		else
		{
			do_fixed_frame_rate(FRAME_RATE_REINIT);		//re-initialise fixed frame rate for the next time this is turned on
		}
		
		
		//measure real fps (might affect delta values)
		end_time=LS_TickCount60();	//end of frame time
		if (end_time-start_time>=30)	//calc fps 2 times/sec
		{
			start_time=LS_TickCount60();
			fps=frame_counter*2;
			frame_counter=0;
		}
		
		real_frame_counter++;
		
		
		debug_printf(debug_verbose, "Main game - frame just incremented on frame %i\n", real_frame_counter);
		
		//if dead, restart
		if (zex_alive==0)
		{
			if ((death_frame_counter>0) && (user_abort==0))
			{ death_frame_counter--; }
			else
			{     
				//deaded
				
				if (ask_for_load==0)
				{
					game_over=1;
					delete_all_constant_images();
					//    fade_and_play_zex_resource(132);	//Spring by Purple Motion
					Mode320=0;		//back to 640*480
									//no readers!! therefore //oldmac//gupdate_cp=1;
					if (quicksave_available==1)
					{
						execute_quicksave=do_game_restore();
						if (execute_quicksave==1)
						{
							goto run_next_level;	//load this one again, the loader will catch quicksave_available
													//and do the right thing... that's the theory anyway.
						}
					}
					quicksave_available=0;	//clear out the quicksave as we are ending the current game
											//<MANIC LAUGHTER> Should we allow people to end the game </MANIC LAUGHTER>
					ogl_inactive();
					
					if (tutorial==0 && training_mission==0) show_stats();	//score and hi-score, credits etc - file show_stats.c
					if (RangedRdm(0,10)>5)
						fade_and_play_zex_resource(131);	//main theme - starshine by Purple Motion
					else
						fade_and_play_zex_resource(134);	//main theme - war remix
					
					handle_hiscore();
				}
				//     SetKeyboardMouseActivation(0);	//we want the mouse and keyboard as normal
				display_wait();	//
				if (ask_for_load==1) goto run_next_level;
				else
				{
					goto run_main_menu;
				}
			}
		}
		
finish_load: if (training_complete) goto run_main_menu;

		debug_printf(debug_verbose, "Main game - before if(level_complete==3) on frame %i\n", real_frame_counter);

		if (level_complete==3)	//we have docked
		{
			//docked
			Mode320=0;
			if (have_ejected==1) 
			{
				extern kit_desc ship_items[ITEMS_COUNT];
				reset_ship();	//set up systems again
				ship_items[KEJECTION].status=0;	//test =1 
				ship_items[KFLATPACK].status=0;
				ship_items[KFLATPACK].modifier=0;
				ship_items[KFLATPACK].value1=0;
				
				ship_items[KMISSILE_PYLON1].status=0;
				ship_items[KMISSILE_PYLON1].obj_class=WEAPONS;
				ship_items[KMISSILE_PYLON1].modifier=0;
				
				ship_items[KMISSILE_PYLON2].status=0;
				ship_items[KMISSILE_PYLON2].obj_class=WEAPONS;
				ship_items[KMISSILE_PYLON2].modifier=0;
				
				ship_items[KMISSILE_PYLON3].status=0;
				ship_items[KMISSILE_PYLON3].obj_class=WEAPONS;
				ship_items[KMISSILE_PYLON3].modifier=0;
			}
			//     display_wait();	  //
			//     score+=100*current_level;
			if (done_load_flag==0)	//if we're mot loading a game
				show_stats();	//score and hi-score, credits etc - file show_stats.c
			
			if (training_mission_complete) goto run_main_menu;	
			
			done_load_flag=0;	//finally finish the load game processing
			
			had_bg_image=have_bpic;
			have_bpic=0;
			clear_dlp();
			ogl_inactive();
			//     unload_textures();
			//     load_textures();
			do_docked(mothership_cat);	//from mothership controller which sets this up from it's catagory
			have_bpic=had_bg_image;
			camera_to_zex();
			if (do_next_level==1)
			{
				next_level=current_level+1;
				quit=0;
				goto run_next_level;
			}
			
			if (do_launch==0)	//end level
			{
				handle_hiscore(); 
				goto run_main_menu;	//if we reuturn and this is 1 it means we are relaunching from motehrship
			}
			
			if (Use_Mode320==1) Mode320=1;	 //back to 320*240 screen
			
		}

		if (game_pause==1)
		{
			do_game_pause();	//can set end of game or continue - file game_pause.c
			game_pause=0;
		}

#if ENABLE_PERIODIC_NAGSCREEN
//#if EXPIRE		//don't show this if it's a test version
//#else
		//
		// create a local nag class.
		// Because it's a static, we get a free bool to tell us when it was 
		// first initialised automatically. Also it gives us a bit of self-contained code. 
		// Also I like classes. This was originally was going to be a struct with an "if !nag.flag"
		// so pretty much the same functionality.
		class nag_type { 
			Uint32 timer; 
			Uint32 last;
			void set_timer() { timer = LS_GetTicks()+20*60*1000; last = LS_GetTicks(); }
		public:
			nag_type() { set_timer(); }		// 20 minutes from now
			void run() { 
				Uint32 now = LS_GetTicks();
				// the code means we don't count menu menu, docked, buy, trading, paused, etc.
				// notice: the message doesn't appear in these screens anyway...
				if( (last+10*1000) < now)	// more than 10 seconds outside the game loop means we shouldn't count it...
				{
					timer += now-last;	// adjust for this time outside the game loop
				}
				last = now;
				// now check for registered...
				extern prefs gzex_prefs;
				if (gzex_prefs.registered!=1 && timer < LS_GetTicks())
				{
					display_unreg(UNREG_PERIODIC_NAGSCREEN);
					set_timer();				// set the timer after so that it is 20 minutes from the end - just in case we end up in the nag screen for a few hours...
				}
			}
		};
		// now create an actual variable of the nag type. Notice that this will be initialised
		// on the first game ... and no matter how many times they load or do a new game it 
		// will come up every 20 minutes.
		static nag_type nag;		// this variable that contains the nag code. 
		nag.run();			// execute the nag code
//#endif	//EXPIRE
#endif //ENABLE_PERIODIC_NAGSCREEN


	}	//end of while !quit


//еееееееееееееееееееееееEND GAME
ogl_inactive();
cleanupAGL();
fade_out_music();
display_wait();	//
do_exit();

screen_quit();
LS_FlushEvents(-1,0);	//flush all

//oldmac	SetGDevice(saveGDH);	//these are here so I can SEE them!
//oldmac	SetPort(savePort);
kill_inputs();	//stop input sprocket
				//if we dont save and restore the GDH and port QD will freeze on quit
exit(0);		// was ExitToShell();
}

//еееееееееееееееееееееееееееееееееееееееее
void render_frame(int width, int height)
{
//extern int pilot_asleep;

//timing/debugging
//int ms_misc_start,ms_do_3d,ms_do_culling,ms_do_pers,ms_draw_it;
//int ms_misc_end,ms_total,ms_text_display,end_micros;
//exec_watchdog=100;	//have to keep reseting this else we'll error
#if PCI_TIMING_DEBUG
T(T_MISC_END);
#endif
do_3d();	//3d stuff - translates active object points to world_points_list_ptr

#if PCI_TIMING_DEBUG
T(T_MATRIX);			//maths
#endif
do_culling2();
#if PCI_TIMING_DEBUG
T(T_CULLING);
#endif
do_pers2(width,height);	//translate points to view
#if PCI_TIMING_DEBUG
T(T_PERS);

T(T_3D_END);
#endif
//do_zsort();	//sort polys furthest first - this can safely be commented out for testing

if ((gShowGalacticMap==0 || gShowGalacticMap==2) && global_show_status==0)
Draw_it(width,height); //furthest poly first

#if PCI_TIMING_DEBUG
T(T_POLYDRAW_END);
#endif
//quick hack to show fps
#ifdef copyright
SplatText_Transparent ("ZEX BY LSG й 1998-2017",120,30);
#endif







//Show_it(CONTROL_PANEL_TOP);	//splat 3d portion of window
}	//end of render frame

void render_frame_no_erase(int width, int height)
{
//timing/debugging
//int ms_misc_start,ms_do_3d,ms_do_culling,ms_do_pers,ms_draw_it;
//int ms_misc_end,ms_total,ms_text_display,end_micros;

exec_watchdog=100;	//have to keep reseting this else we'll error

do_3d();	//3d stuff - translates active object points to world_points_list_ptr
			//etc.

do_culling2();


do_pers2(width,height);	//translate points to view
Draw_it_no_erase(width,height); //furthest poly first

//quick hack to show fps
#ifdef copyright
SplatText_Transparent ("ZEX BY LSG й 1998-2017",120,30);
#endif

//Show_it(CONTROL_PANEL_TOP);	//splat 3d portion of window
}	//end of render frame


//render with a certain bg colour
void render_frame_colour_bg(int width, int height, int colour)
{
//timing/debugging
//int ms_misc_start,ms_do_3d,ms_do_culling,ms_do_pers,ms_draw_it;
//int ms_misc_end,ms_total,ms_text_display,end_micros;
exec_watchdog=100;	//have to keep reseting this else we'll error

#if PCI_TIMING_DEBUG
T(T_MISC_END);
#endif
do_3d();	//3d stuff - translates active object points to world_points_list_ptr
#if PCI_TIMING_DEBUG
T(T_MATRIX);			//maths
#endif
do_culling2();
#if PCI_TIMING_DEBUG
T(T_CULLING);
#endif
do_pers2(width,height);	//translate points to view
#if PCI_TIMING_DEBUGT
(T_PERS);

T(T_3D_END);
#endif
//do_zsort();	//sort polys furthest first - this can safely be commented out for testing


Draw_it_colour_bg(width,height,colour); //furthest poly first
#if PCI_TIMING_DEBUG
T(T_POLYDRAW_END);
#endif
//quick hack to show fps
#ifdef copyright
SplatText_Transparent ("ZEX BY LSG й 1998-2017",120,30);
#endif

}



bool check_beta_expiry(void)
{
	//return true if beta has expired - set beta expiry date in options.h
	#if EXPIRE==1
	bool expired=false;
	time_t actual_time;
	tm* current_time_struct_ptr;
	actual_time=time(0);	
	
	current_time_struct_ptr=gmtime(&actual_time);
	int year, month;
	
	year=current_time_struct_ptr->tm_year+1900;	//years since 1900
	month=current_time_struct_ptr->tm_mon;	//Lovely 8 char maximums...
	if (year>EXPIRE_YEAR) 
	{ 
		expired=true; 
	}
	else
	if ((year==EXPIRE_YEAR) && (month>=EXPIRE_MONTH))	//Won't run after this date
	{
		expired=true;
	}
	
	if (expired==true)
	{
		report_error_c("This test version of Zex has expired.","Please check http://www.lightsoft.co.uk/zex for an updated version.",666);
		return true;
	}
	else
	{
		return false;
	}
	#else
		return false;
	#endif
}



//
// print_compiler
//
void print_compiler(void)
{
#ifdef __GNUC__  // the following line obviously won't work for MrCpp, or Intel's or IBM's complier.
	char string_out[400];
	sprintf(string_out,"GCC version %i.%i.%i   (version string= \"%s\")",__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__, __VERSION__);
	show_text_in_loading_screen(string_out);
#else
	show_text_in_loading_screen("Unknown compiler");
#endif
}


//Make sure if freeze is true, then monsters can't fire.
void periodic_freeze_processing(void)
{
extern int freeze, monsters_can_fire;
		
		if (freeze==1)
		{
			monsters_can_fire=0;
		}
		else
		{
			monsters_can_fire=1;
		}
}

#define DISPLAY_MESSAGES_TIMER 600
static int bootup_messages_timer=0;
void handle_bootup_messages(void)
{
	char print_string[132];
	if (training_mission==0 && tutorial==0)
	{
		if (bootup_messages_timer==DISPLAY_MESSAGES_TIMER)
		{
					sprintf(print_string,"Press '%s' to switch thrust modes.",SDL_GetKeyName(get_programmed_keycode(velocity_limter_key)));
					add_to_text_display(print_string,0); 
					add_to_text_display("Cruise mode offers high speed with low fuel consumption but reduced agility.",0); 
					add_to_text_display("Combat mode is slower, uses more fuel but provides more agility.",0); 
					bootup_messages_timer++;
		}
		else
		if (bootup_messages_timer<DISPLAY_MESSAGES_TIMER)
		{
			bootup_messages_timer++;
		}
	}
}

void reset_bootup_messages(void)
{
	bootup_messages_timer=0;
}

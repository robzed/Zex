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
$Header: /home/ls_cvs/Zex2005/Code/static_stuff/buy.cpp,v 1.64 2006/11/05 21:08:23 rob Exp $
$Log: buy.cpp,v $
Revision 1.64  2006/11/05 21:08:23  rob
Zex2005: Added reserved logo to items you can't buy. Also changed the description text to more reflect this change.

Revision 1.63  2006/09/09 10:16:28  stu
Zex2005: Code: Fix for crash introduced in #3886 in buy when docked during training or tutorial.

Revision 1.62  2006/08/14 23:28:43  stu
Zex2005: Code: Following changes: Can now cancel C&C dialog with mouse button. Fixed halt when trying to sell a missile pylon. Default camera start position now higher and max zoom out increased.

Revision 1.61  2006/08/13 22:56:36  stu
Zex2005: Code: Shouldn't get hi-score screen anymore if hte player aborts the game. When buying hull repair or refuelling we now treat these as special cases to improve the grammer of the messages given. ('You have been refuelled at a cost of x' versus 'You have bought the refuel for x'.

Revision 1.60  2006/08/06 17:37:01  stu
Zex2005: Code: Can now sell all. Now get dialog when logged out of Galanet due to no credits, and the same when trying to log in. Can no-longer jettison cargo when docked.

Revision 1.59  2006/08/06 00:36:31  stu
Zex2005: Code: Added a base price multiplier for commodities. Also increased the stock if manufacturer of commodity is same as alegience of mship. (Difference is now 30% up if allegiance match, else 20% down, was 10% up and 20% down)

Revision 1.58  2006/07/12 22:21:15  stu
Zex2005: Code: If we sell a flatpack and we still have some left then set the status back to 1

Revision 1.56  2006/07/09 03:04:46  stu
Zex2005: Code: Ship items available for sale are now affected by the mships supply rating and tech level. So on a mship on a poor supply route supplied by an agri planet there won't be much but on a mship on close to a tech planet the range will be much wider (honouring what must and must not be avail).

Revision 1.55  2006/07/08 20:34:12  rob
Zex2005: Make sure player can never ever (on tutorial or main game) buy a boid addition.

Revision 1.54  2006/07/08 16:58:08  stu
Zex2005: Code: One can no-longer sell items that have been destroyed. In addition we now show what the markdown is when selling a damged item.

Revision 1.53  2006/07/06 19:39:33  stu
Zex2005: Code: text change to show that the rank must be at least that displayed to buy the item.

Revision 1.52  2006/07/05 22:06:19  rob
Zex2005: Alteration to make rank required clearer.

Revision 1.51  2006/07/05 21:34:12  rob
Zex2005: Minor correct to logic to rank-based buy logic avoid trixie players breaking it ;-) plus some code tidy up.

Revision 1.50  2006/07/05 17:39:52  stu
Zex2005: Code: All ship items are now available for sale (apart from those that are ship allegiance sensitive). Rank is now checked when players tries to buy an item and an appropriate message displayed if rank isn't high enough. Not extensively tested.

Revision 1.49  2006/07/04 22:33:12  rob
Zex2005: Hull repairs always available at human motherships and never at alien motherships.

Revision 1.48  2006/07/04 22:24:53  rob
Zex2005: Centralised buy item availability data in main data table and fixed sellers selling things that shouldn't be available to our player (hopefully!)

Revision 1.47  2006/07/02 23:42:25  stu
Zex2005: Code; Fix for ships items that shouldn't be available at a given rank being available.

Revision 1.46  2006/05/30 21:25:43  stu
Zex2005: Code: Some last minute tweaks. 1. Make sure we get the toll message before registration fail. 2. Reduce shockwave basic damage to 6000. 3. Reduce price of level 1 trading computer and added a 1000 to shockwave 1.

Revision 1.45  2006/05/27 23:45:54  stu
Zex2005: Code: Make sure we can't index with an item_selected of -1

Revision 1.44  2006/05/26 22:03:11  rob
Zex2005: Fixed buy bug where you can select invalid lin es and sell non-existant them.

Revision 1.43  2006/04/09 01:12:07  stu
Zex2005: Human and alien shuttles, faster falcons, Pythons and Excaliburs.

Revision 1.42  2006/04/05 00:09:25  stu
Zex2005: Code: Progression changes

Revision 1.41  2006/02/27 01:42:38  stu
Zex2005: Code: A few changes to Motherships. 1. Can't connect to GalaNet if power rating is too low - typical after forcing a mship to surrender. Wait and try again. 2. You now get score and credits for forcing a surrender.
3. Motherships surrender later.

Revision 1.40  2006/02/23 00:22:31  stu
Zex2005: You can no-longer sell a trading computer on an alien mothership. this fixes bug #108.

Revision 1.38  2006/02/22 01:36:18  stu
Zex2005: Code: Fix for #104. Selling broken goods now gives you a greatly reduced price, in addition the dialog text is coloured red. Also fixed a bug whereby if the NavComp was damaged it still actually worked.

Revision 1.37  2006/02/21 21:40:42  stu
Zex: Code: Fix for #139 - call selct_next_pylon when we buy a missile

Revision 1.36  2006/02/21 21:15:15  stu
Zex: Code: Fix for #116

Revision 1.35  2006/01/14 02:04:19  stu
Zex2005: Code: What items are available on motherships now depends on a) the allegiance of the mothership. b) Your rank.

Revision 1.33  2006/01/14 00:35:27  stu
Zex2005: Code: The purchase of level 3 guns, level 3 rear laser and ECM was broke. Fixes #125 (guns) and #128 (ECM not working until you get a flatpack).

Revision 1.32  2006/01/14 00:00:27  stu
Zex2005: Code: Some important bug fixes. 1. Tasers didn't work - they fired like lasers (single beam). 2. Finally found the crash in motherships where my_mem would be zero - only the controller parts of a mothership has the aux mem - if something hit another part of a mothership then we'd crash (found whilst fixing the tasers). Also fixes a few gameplay bugs.

Revision 1.31  2006/01/08 22:27:54  stu
Zex2005: Code: Updated item prices

Revision 1.30  2006/01/08 03:44:04  stu
Zex2005: Code: You can now sell items of class REPAIR. This fixes #97. Also fixes #95.

Revision 1.29  2006/01/08 03:05:54  stu
Zex2005: Code: Removed all remaining warnings. There should now be no compile warnings

Revision 1.28  2006/01/07 13:55:57  stu
Zex2005: Code: A change in options.h to make buy show all ships items. As booked in this is on for items 0-14.

Revision 1.27  2005/12/30 21:48:59  stu
Zex2005: Code: Fixes selling of engine and selling ships kit with trade computer #89, #92.

Revision 1.26  2005/12/25 10:29:22  stu
Zex2005: Unable to sell certain ship items fix. This actually goes a little deeper in that the ships items weren't actually initialised, so all previous saved games are invalid. Also improved item seelction when selling stuff.

Revision 1.25  2005/12/20 20:41:46  rob
Zex2005: Various signed/unsigned comparison things plus one might be used uninitialised (not really - but got rid of warning).

Revision 1.24  2005/12/17 10:48:02  rob
Zex2005: Added full nested braces for nested structure/array initialisations. Not strictly necessary, but it only took me a few minutes and I did find one minor error in the movie data.

Revision 1.23  2005/12/16 22:00:05  stu
Zex2005: Code: more work

Revision 1.22  2005/12/11 01:01:45  stu
Zex2005: Code: Fixes a fatal error in do_string_dialog (C strings, not Pascal)

Revision 1.21  2005/12/10 16:26:06  rob
Zex2005: Fixed black cursor/double cursor in inventory screen 2 plus no cursor visible in buy screens. (Also added some pauses to avoid 100% processor usage.)

Revision 1.20  2005/11/16 01:05:20  stu
Zex2005: Code: GalaNet should no longer take 100% processor cycles

Revision 1.19  2005/11/15 22:20:27  rob
Zex2005: File selector patched in.

Revision 1.18  2005/11/13 03:42:58  stu
Zex2005: Code: Fixed the bug whereby withdrawn items would be made immediately available again.

Revision 1.15  2005/11/08 22:45:07  stu
Zex2005: Code: Various bug fixes as per the TODO

Revision 1.14  2005/10/10 21:26:53  rob
Zex2005: Added newline at end to stop compiler whinging.

Revision 1.13  2005/10/09 14:54:24  rob
Zex2005: Got rid of stupid conversion build warnings (float to int) in various files whilst waiting on fink to finish compiling.

Revision 1.12  2005/10/08 13:13:31  rob
Zex2005: "/p" pascal string size marker is not supported in normal gcc - it's a apple specific extension turned on by the option -fpascal_strings. This change removes that string so that the code will run both on Mac and Windows. Lots of files have been changed - but nearly all the changes are trivial. By the fun-jobs-are-us dept.

Revision 1.11  2005/10/03 23:18:29  rob
ZEX2005: Some of the cases of processing power sucked out during fixed screen have been fixed. Also included test code for variable font checking.

Revision 1.10  2005/09/29 00:49:08  stu
Zex2005: Code: Basic x86 port

Revision 1.9  2005/05/19 20:23:54  rob
Zex2005: More files changed.

Revision 1.8  2005/05/17 16:21:54  rob
Zex2005: Even more work sorting out build problems on gcc4.0/MacOSX/SDL conversion.

Revision 1.7  2005/05/15 17:42:04  rob
Zex2005: ZexNumToString used throughout, file case changes, general gcc4.0 build changes.

Revision 1.6  2005/05/13 09:24:58  rob
Zex2005: Changed Button to internal version. Altered string to dec code to use atof().

Revision 1.5  2005/05/04 17:30:59  rob
Zex2005: More changes for build.

Revision 1.4  2005/05/02 10:27:15  rob
Zex2005: More Updates (code)

Revision 1.3  2005/04/24 10:09:16  rob
ZEX: Started to work on ZEX.cpp. Replaced TickCount. Added some void return types. Commented out some Mac init functions. Altered main.cpp to do the correct thing on an exit().

Revision 1.2  2005/04/23 20:08:24  rob
ZEX: First set of changes for SDL build. (remainder of files)

Revision 1.30  1999/09/26 18:34:42  stu
260999

Revision 1.29  1999/09/20 19:03:29  stu
0.93.3

Revision 1.28  1999/09/19 05:38:52  stu
0.93 as posted

Revision 1.27  1999/09/12 00:56:08  stu
120999

Revision 1.26  1999/08/30 22:22:58  stu
300899

Revision 1.25  1999/08/20 09:57:22  stu
20 Aug 99

Revision 1.24  1999/08/15 22:16:55  stu
Cleanup 150899

Revision 1.23  1999/08/03 18:25:55  stu
*** empty log message ***

Revision 1.22  1999/07/29 05:10:14  stu
*** empty log message ***

Revision 1.21  1999/07/27 05:35:13  stu
Final update for test 270799

Revision 1.20  1999/07/26 22:22:22  stu
Pre test 3 270799

Revision 1.19  1999/07/26 19:08:43  stu
Pre 0.86 test 27 July 99

Revision 1.18  1999/07/16 13:52:37  stu
Sat 1355

Revision 1.17  1999/07/16 02:09:00  stu
Clean up 170799

Revision 1.16  1999/07/13 04:07:25  stu
Clean up 130799

Revision 1.15  1999/07/09 06:03:04  stu
Clean up 09-7-99

Revision 1.14  1999/07/04 00:21:14  stu
Major changes and development.
Colldet changes.
Level 2,3,4
Version 0.86

Revision 1.13  1999/06/27 19:27:42  stu
Clean up 27 June 99

Revision 1.12  1999/06/23 06:08:17  stu
The computer may be busy or damaged.

Revision 1.11  1999/06/23 04:54:11  stu
More fixes related to motherships and weapons - still ongoing. Still docking sometimes dodgy.

Revision 1.10  1999/06/20 05:37:22  stu
Changed Hull repair and flatpack pricing to make hull repair more realistic.

Revision 1.9  1999/06/14 07:25:40  stu
*** empty log message ***

Revision 1.8  1999/06/08 23:07:48  stu
Final checkin before cvs pro client upgrade.

Revision 1.7  1999/06/07 05:45:43  stu
Lurkers clear up

Revision 1.6  1999/05/30 14:53:46  stu
Mods for added function
erase_zex_rect_colour

Revision 1.5  1999/05/23 16:57:33  stu
Work completed on:
EF-003 - Screen Centering 
and 
EF-004 - No resolution change.

Revision 1.4  1999/05/21 03:31:20  stu
1. New screen driver.
2. VBL Driven music fade and play

*/
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"
#include "fpoly4.h"
#include "game_defs.h"
//#include <stdlib.h>
#include <new>				// for new to replace NewPtr
using namespace std;

//oldmac//#include <memory.h>
//oldmac//#include <timer.h>
//oldmac//#include <textutils.h>
//oldmac//#include <files.h>
#include "sound.h"
#include "pictures.h"
#include "splat_control.h"
#include "Error_handler.h"
#include "zex_misc.h"
#include "input_control.h"
#include	"txtsplat.h"
#include	"3d_structures_def.h"
#include "instrum_int.h"

#include "buy.h"
#include "buy_int.h"
#include "inventory.h"
#include "cargo.h"
#include "Galaxy.h"
#include "dlp.h"
#include "real_time.h"
#include "ship_control.h"

extern const RGBColor	rgbBlack;
extern const RGBColor	rgbWhite;
extern const RGBColor	rgbRed;
extern const RGBColor	rgbOrange;
extern const RGBColor	rgbBlue		;
extern const RGBColor	rgbDarkBlue;
extern const RGBColor	rgbYellow;
extern const RGBColor	rgbPurple;
extern const RGBColor	rgbGreen;
extern const RGBColor	rgbLightBlue;
extern const RGBColor	rgbGrey;
extern const RGBColor	rgbDarkGrey;

RGBColor	rgbDarkLightBlue	= { 0x0000, 0x9999, 0x9999 };

extern int training_mission;
extern	DynObjectsFixedSize *ocb_ptr;
extern galactic_map *GM_ptr;	//ptr to the galactic map
extern int current_quad;
#define CLEAR_WITHDRAWNS 0
#define DO_NOT_CLEAR_WITHDRAWNS 1
//array for items and services available
//#define NUMBER_OF_GOODS 44

//items must be defined as per their enum found in buy.int
//as it is used as an index into this array
typedef struct {
	int quantity; 
	int basic_price;
	int actual_price;
	int obj_class;
	int rcz_id; 
	int type; 
	//
	// Buy availablity control
	//
	int earth_rank_blocked;
	int earth_min;
	int earth_max;
	int alien_rank_blocked;
	int alien_min;
	int alien_max;
	
	//
	// Other data
	//
	int temp_removed;		// withdrawn from sale?
	Str255 short_description; 
	Str255 long_description; 
	
} stock;


// flags for special ranks for availability
const int _always = 999;
const int _never = 888;


//do not move these about - the enums in buy.int specify array index
//NOTE THAT AS OF 090706 these figures are maximums given a supply rating of 100 and a tech rating of 10.
stock ship_stock [NUMBER_OF_GOODS] =
{
//      price                picture                        earth  earth        alien  alien         description
//qty   basic actual class   id     type                    rank   min max      rank   min max  temp  short     long
{ 1,	56,     0,	WEAPONS, 3000,	MISSILE1,               _always,3, 25,      _always,1, 15,    0, "Missile 1","" },	   //got gfx
{ 0,	462,	0,	WEAPONS, 3001,	MISSILE2,               CDR,    1,  5,      LT,     1,  5,    0, "Missile 2","" },
{ 0,	624,	0,	WEAPONS, 3002,	MISSILE3,               RADM,   1,  5,      RADM,   1,  2,    0,"Missile Type 3","" },
{ 1,	352,	0,	WEAPONS, 3003,	LASER1,                 _always,1,  4,      _always,1,  4,    0,"Laser Type 1","" },	  //got gfx
{ 0,	5741,	0,	WEAPONS, 3004,	LASER2,                 LT,     1,  2,      CDR,    1,  2,    0,"Laser Type 2","" },	  //got gfx
{ 0,	26139,	0,	WEAPONS, 3005,	LASER3,                 RADM,   0,  2,      RADM,   0,  2,    0,"Laser Type 3","" },	  //gpt gfx

{ 1,	2112,	0,	WEAPONS, 3044,	TASER1,                 _never, 0,  0,      CDR,    1,  4,    0,"Taser Type 1","" },	  //got gfx
{ 0,	8712,	0,	WEAPONS, 3045,	TASER2,                 _never, 0,  0,      CAPT,   1,  2,    0,"Taser Type 2","" },	  //got gfx
{ 0,	48012,	0,	WEAPONS, 3046,	TASER3,                 _never, 0,  0,      VADM,   0,  2,    0,"Taser Type 3","" },	  //gpt gfx
{ 0,	2375,	0,	WEAPONS, 3006,	REAR_LASER1,            LTCR,   1,  4,      ENS,    1,  4,    0,"Rear Laser Type 1","" },	//got gfx
{ 0,	5600,	0,	WEAPONS, 3007,	REAR_LASER2,            CAPT,   1,  4,      LTCR,   1,  4,    0,"Rear Laser Type 2","" },	//got gfx
{ 0,	25560,	0,	WEAPONS, 3008,	REAR_LASER3,            VADM,   0,  4,      RADM,   0,  4,    0,"Rear laser Type 3","" },	//got gfx
{ 1,	293,	0,	WEAPONS, 3009,	CANNON1,                _always,1,  4,      _always,1,  4,    0,"Cannon Type 1","" },
{ 0,	5691,	0,	WEAPONS, 3010,	CANNON2,                CDR,    1,  4,      LT,     0,  4,    0,"Cannon Type 2","" },
{ 0,	18903,	0,	WEAPONS, 3011,	CANNON3,                CAPT,   0,  4,      RADM,   1,  4,    0,"Cannon Type 3","" },
{ 0,	4654,	0,	WEAPONS, 3012,	SHOCKWAVE1,             RADM,   1,  2,      CAPT,   1,  2,    0,"Shockwave Type 1","" },
{ 0,	23644,	0,	WEAPONS, 3013,	SHOCKWAVE2,             _never, 0,  0,      ADM,    1,  2,    0,"Shockwave Type 2","" },
{ 0,	16987,	0,	WEAPONS, 3014,	ECM,                    CDR,    0,  2,      LTCR,   1,  2,    0,"ECM","" },	//got gfx
{ 0,	89309,	0,	WEAPONS, 3015,	MISSILE_FACTORY,        _never, 0,  0,      VADM,   0,  2,    0,"Missile Factory","" },	//got gfx
{ 0,	4092,	0,	WEAPONS, 3016,	TARGETTER_360,          _never, 0,  0,      ENS,    1,  2,    0,"360 Degrees Targeting","" },	  //got gfx
 
{ 0,	1918,	0,	EQUIPMENT, 3017, FAST_DOCK,             ENS,    1,  2,      ENS,    1,  2,    0,"Fast Dock","" },	  //got gfx
{ 0,	2998,	0,	EQUIPMENT, 3018, FLATPACK,              _always,1,  5,      _never, 0,  0,    0,"Flatpack Ship","" },	//got gfx
{ 0,	10852,	0,	EQUIPMENT, 3019, EFFICIENT_ENGINE,      LTCR,   0,  2,      LTCR,   0,  2,    0,"Fuel Maximizer","" }, 	  //got gfx
{ 0,	423,	0,	EQUIPMENT, 3020, RETRO_ROCKETS,         ENS,    0,  2,      CDR,    0,  2,    0,"Retro Thrust","" },	  //got gfx
{ 0,	42930,	0,	EQUIPMENT, 3021, HYPERSPACE,            CAPT,   0,  2,      CAPT,   0,  2,    0,"Warp Jump","" },	  //got gfx
{ 0,	515,	0,	EQUIPMENT, 3022, FLIGHT_COMPUTER1,      _always,0,  2,      _never, 0,  0,    0,"Ship Computer","" },
{ 0,	832,	0,	EQUIPMENT, 3023, BULLET_PROOF_EJECTION, LTCR,   0,  2,      LT,     0,  2,    0,"Indestructable Ejection Pod","" },		  //got gfx
{ 0,	89,		0,	EQUIPMENT, 3024, SHIELDS1,              _always,1,  2,      _always,1,  2,    0,"Shields 1","" },	  //got gfx
{ 0,	1601,	0,	EQUIPMENT, 3025, SHIELDS2,              ENS,    1,  3,      ENS,    0,  2,    0,"Shields 2","" },	  //got gfx
{ 0,	12608,	0,	EQUIPMENT, 3026, SHIELDS3,              LTCR,   0,  2,      LTCR,   0,  2,    0,"Shields 3","" },	  //got gfx
{ 0,	55892,	0,	EQUIPMENT, 3027, SHIELDS4,              CAPT,   0,  2,      CAPT,   0,  2,    0,"Shields 4","" },	  //got gfx
{ 0,	185222,	0,	EQUIPMENT, 3028, SHIELDS5,              VADM,   0,  2,      VADM,   0,  2,    0,"Shields 5","" },	  //got gfx

{ 0,	12,		0,	REPAIRS, 3029,   HULL_REPAIR,           _always,1,  1,      _never, 0,  0,    0,"Hull Repair","" },
{ 0,	12,		0,	REPAIRS, 3030,   FUEL,                  _always,0,500,		_always,0,500,    0,"Fuel","" },	//got gfx
{ 0,	1421,	0,	REPAIRS, 3031,   REPAIR_SYSTEMS1,       _always,0,  2,		_always,0,  2,    0,"Repair Systems 1","" }, //need gfx
{ 0,	9513,	0,	REPAIRS, 3032,   REPAIR_SYSTEMS2,       CDR,    0,  2,      CAPT,   0,  2,    0,"Repair Systems 2","" },
{ 0,	68898,	0,	REPAIRS, 3033,   REPAIR_SYSTEMS3,       VADM,   0,  2,      VADM,   0,  2,    0,"Repair Systems 3","" },

{ 0,	634,	0,	EQUIPMENT, 3034, EJECTION,              _always,0,  5,      _always,1,  2,    0,"Standard Ejection Pod","" },	//got gfx
{ 0,	4815,	0,	EQUIPMENT, 3035, FLIGHT_COMPUTER2,      LTCR,   0,  2,      _never, 0,  0,    0,"Ship Computer","" }, //need gfx
{ 0,	303,	0,	EQUIPMENT, 3036, NAV_COMPUTER1,         _always,1,  2,      _always,0,  2,    0,"","" },	//got gfx
{ 0,	10515,	0,	EQUIPMENT, 3037, NAV_COMPUTER2,         LT,     0,  2,      LT,     0,  2,    0,"","" },	//got gfx
{ 0,	729,	0,	EQUIPMENT, 3038, TRADING_COMPUTER1,     _always,1,  3,      _always,1,  3,    0,"","" }, //got gfx
{ 0,	8941,	0,	EQUIPMENT, 3039, TRADING_COMPUTER2,     CDR,    0,  2,      CDR,    1,  3,    0,"","" },	//got gfx
{ 0,	38882,	0,	EQUIPMENT, 3040, TRADING_COMPUTER3,     RADM,   0,  4,      RADM,   0,  4,    0,"","" },
{ 0,	25482,	0,	EQUIPMENT, 3041, AUTO_HULL_REPAIR,      _never, 0,  0,      RADM,   0,  2,    0,"","" },

{ 0,	1092,	0,	WEAPONS,   3042, BOID_ADDITION,         _never, 0,  0,		_never, 0,  0,    0,"","" },  // NO BOIDS - special in the case of BOID handled by code. No traders.
{ 0,	2500,	0,	WEAPONS,   3043, ASTEROID_ADDITION,     _always,0,  3,      ENS,    1,  1,    0,"","" },

};

//weapons......... stock, basic price
//stock buy_missiles1 = {1,100,WEAPONS,MISSILE1};
//stock buy_missiles2 = {0,150,WEAPONS,MISSILE2};
//stock buy_missiles3 = {0,200,WEAPONS,MISSILE3};
//
//stock buy_laser1 = {1,100,WEAPONS,LASER1};
//stock buy_laser2 = {0,200,WEAPONS,LASER2};
//stock buy_laser3 = {0,300,WEAPONS,LASER3};
//
//stock buy_rear_laser1 = {0,400,WEAPONS,REAR_LASER1};
//stock buy_rear_laser2 = {0,600,WEAPONS,REAR_LASER2};
//stock buy_rear_laser3 = {0,800,WEAPONS,REAR_LASER3};
//
//stock buy_cannon1 = {1,20,WEAPONS,CANNON1};
//stock buy_cannon2 = {0,190,WEAPONS,CANNON2};
//stock buy_cannon3 = {0,300,WEAPONS,CANNON3};
//
//stock buy_shockwave1 = {0,400,WEAPONS,SHOCKWAVE1};	//smart bomb 1 - 3000 units [S]
//stock buy_shockwave2 = {0,600,WEAPONS,SHOCKWAVE2};	//smart bomb 2 - 5000 units [S]	
//stock buy_ecm = {0,2000,WEAPONS,ECM};		  //destroys missiles [E]
//stock buy_missile_factory = {0,3000,WEAPONS,MISSILE_FACTORY};
//stock buy_360_degree_targetter = {0,2000,WEAPONS,TARGETTER_360};
////misc bits
//stock buy_fast_dock = {0,400,EQUIPMENT,FAST_DOCK};	//[F]
//stock buy_flatpack = {0,1000,EQUIPMENT,FLATPACK};	//spare life
//stock buy_aux_fuel_tank = {0,200,EQUIPMENT,AUX_FUEL_TANK};
//stock buy_retro_rockets = {0,1000,EQUIPMENT,RETRO_ROCKETS};	//[R]
//stock buy_hyperspace = {0,930,EQUIPMENT,HYPERSPACE};	  //[H]
//stock buy_nav_computer = {0,100,EQUIPMENT,NAV_COMPUTER};
//stock buy_bullet_proof_ejection = {0,3000,EQUIPMENT,BULLET_PROOF_EJECTION};	//cant be destroyed
//
//stock buy_shields1 = {0,200,EQUIPMENT,SHIELDS1};	  //diff level 1
//stock buy_shields2 = {0,350,EQUIPMENT,SHIELDS2};
//stock buy_shields3 = {0,700,EQUIPMENT,SHIELDS3};
//stock buy_shields4 = {0,1000,EQUIPMENT,SHIELDS4};
//stock buy_shields5 = {0,2000,EQUIPMENT,SHIELDS5};	//diff level 5
////repairs
//stock buy_hull_repair = {0,120,REPAIRS,HULL_REPAIR};	  //120 creds per 10 units needed
//stock buy_fuel = {0,2,REPAIRS,FUEL};	  //2 creds per unit
//stock buy_repair_systems1 = {0,200,REPAIRS,REPAIR_SYSTEMS1};	  //standard if destroyed
//stock buy_repair_systems2 = {0,800,REPAIRS,REPAIR_SYSTEMS2};	  //super duper
//stock buy_repair_systems3 = {0,1800,REPAIRS,REPAIR_SYSTEMS3};	//super duper and uses no fuel

//еееееееееееееееееееее

int old_mothership_stock_slot_number=0;
int docked_at_allegiance;
static int mship_supply_rating;
static int mship_supply_type;	//<5=agri, >=5 = tech

static int how_many_users;	//how many other users logged on?
extern int difficulty;

void do_buy_intro();
void buy_repairs();
void buy_weapons();
void buy_equipment();
void display_goods(int what_class);
void init_show_stock();
void show_stock(int stock_index);
void wait_for_mouse();
void draw_windows();
void setup_prices(int clear_withdrawns);
int do_buy_connect();
void combat_buy(LSRAW* exit_button);
void combat_sell(LSRAW* exit_button);
void display_ship_goods(int what_class);
void run_other_users(int from_class);
void reset_other_users();
void sell_equipment(int what_class);
void do_yer_actual_sell(int ship_item_id);
int translate_ship_to_stock(int item_id);
void display_commodities(LSRAW * exit_button);
void draw_lines(int trading_open);
void draw_commodity_strings(int from_index,int selected_item);
void setup_commodity_stocks();
void run_commodities();
void setup_commodity_stock_prices();
void draw_hold_items_docked(LSRAW* bar_pict, int trading_open);
void draw_bottom_bar(LSRAW*bar_pict,int can_exit, int can_buy, int can_sell, int can_inv);
void draw_inventory_strings(int from_index, int selected_item);
void sell_cargo(int item_selected);
void buy_cargo(int item_selected);
void do_string_dialog(char * the_string);
static bool can_item_be_sold_to_player (int item, int player_rank);
static int get_minimum_rank_for_sale(int item_id);
void show_no_credits(void);
void show_logout(void);

extern int credits,score;
extern kit_desc ship_items[ITEMS_COUNT];

extern int comms_will_fail,docked_at_time;	//from docked
extern int trainig,tutorial;

void buy(int slot_docked_at)
{
extern const RGBColor	rgbBlack;
extern Ptr screenaddr;
extern int screenwidth;
//oldmac//extern	  DSpContextReference		          mDisplayContext;
extern int showing_stats;

Point mouse_xy;
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;

LSRAW *the_zoom_picture;
Handle zoom_H;
LSRAW *exit_button;
Handle exit_button_H;


int wait,i;
int button_state,exit;



mship_supply_rating=get_supply_rating_docked(slot_docked_at);
//1 to 5=agri, 6-9=tech
if (training_mission==0 && tutorial==0)
{
  mship_supply_type=(*ocb_ptr).object_list[slot_docked_at].mship_ptr->supply_planet_index;
  mship_supply_type=GM_ptr->the_quadrant[current_quad].quadrant_planets[mship_supply_type].type;	//<5 agri
}
else
mship_supply_type=5;

setup_commodity_stock_prices();

//see if player has been waiting long enough to get on the computer
//if (LS_TickCount60()>docked_at_time+(60*60*3)) comms_will_fail=0;	      //can now connect
comms_will_fail=0;
if (difficulty<16)	//never fail on easy setting
{
if (RangedRdm(0,100)>90) comms_will_fail=1;
}

 showing_stats=0;	//we're not any more
   
    disable_watchdog();
    zoom_H=GetZexPicture ('RCZ ',2005);	//galanet  
 	if (zoom_H==0) report_error_c("Memory Error: buy, zoom_H","",-1);
	LS_HLock(zoom_H);
	the_zoom_picture = (LSRAW*)*zoom_H;
    exit_button_H=GetZexPicture ('RCZ ',2006);	//exit button   
 	if (exit_button_H==0) report_error_c("Memory Error: buy, exit_button_H","",-1);
	LS_HLock(exit_button_H);
	exit_button = (LSRAW*)*exit_button_H;

reset_other_users();	//reset other buyrs/sellers

do_buy_intro();	//zoom computer screen onto screen

if (do_buy_connect()==0)
exit=0;
else
exit=1;	//skip if we couldn't connect

while (!exit)
{
//set up first screen

reset_main:	wait=0;
	for (i=0;i<2;i++)
	{
     the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
	 print_crunched_pict(the_zoom_picture,0,0);
 	 print_crunched_trans_pict(exit_button,260,380);
    Show_it();
    }
 while (LS_Left_Button()) { SDL_Delay(10); Show_it(); }	//wait for mouse up -  give computer back time and allow mouse to move (if on screen)
 
while (wait==0)
{
	Show_it();
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor

//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     button_state=LS_Left_Button();
     if (button_state==1)
      {
        if (mouse_xy.h>260 && mouse_xy.h<392 && mouse_xy.v>410 && mouse_xy.v<460)
        { wait=1; exit=1; }	//exit


        if (mouse_xy.h>108 && mouse_xy.h<247 && mouse_xy.v>288 && mouse_xy.v<348)
        wait=2;	//cbuy

        if (mouse_xy.h>247 && mouse_xy.h<378 && mouse_xy.v>288 && mouse_xy.v<348)
        wait=3;	//csell


        if (mouse_xy.h>378 && mouse_xy.h<539 && mouse_xy.v>288 && mouse_xy.v<348)
        wait=4;	//csell

//        else       
//        if (mouse_xy.h<395)
//        wait=3;	//weapons
//        else
//        wait=4;	//repairs
      }
    
    
    if (wait==2) 
    {
	if (credits<1)	//can't afford Galanet
	{
		show_no_credits();
	}
	else
	{
		combat_buy(exit_button);	//pass the exit button
    }
	goto reset_main;
    }

    if (wait==3) 
    {
	if (credits<1)	//can't afford Galanet
	{
		show_no_credits();
	}
	else
	{
		combat_sell(exit_button);	//pass the exit button
    }
	goto reset_main;
    }

    if (wait==4) 
    {
	if (credits<1)	//can't afford Galanet
	{
		show_no_credits();
	}
	else
	{
		display_commodities(exit_button);	//pass the exit button
	}
    goto reset_main;
    }

//    else
//    if (wait==3) buy_weapons();
//    else
//    if (wait==4) buy_repairs();
  
 }	//end of !exit
 
}
    
    LS_DisposeHandle(zoom_H);
    LS_DisposeHandle(exit_button_H);

enable_watchdog();
}

void combat_buy(LSRAW* exit_button)
{
LSRAW *cbuy_picture;
Handle cbuy_H;
int wait,button_state;
Point mouse_xy;
int exit,i;
LS_CGrafPtr the_drawing_buffer;

        cbuy_H=GetZexPicture ('RCZ ',6000);	//galanet  
	LS_HLock(cbuy_H);
	cbuy_picture = (LSRAW*)*cbuy_H;


reset_cbuy:	for (i=0;i<2;i++)
	{
     the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
	 print_crunched_pict(cbuy_picture,0,0);
 	 print_crunched_trans_pict(exit_button,260,380);
    Show_it();
    }
    
 while (LS_Left_Button())  { SDL_Delay(10); Show_it(); }	//wait for mouse up // give computer back time and allow mouse to move (if on screen)
wait=0;
//buy equipment
while (wait==0)
{
	Show_it();
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor

//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
     button_state=LS_Left_Button();
     if (button_state==1)
      {
        if (mouse_xy.h>260 && mouse_xy.h<392 && mouse_xy.v>410 && mouse_xy.v<460)
        { wait=1; exit=1; }	//exit

        if (mouse_xy.h>60 && mouse_xy.h<565 && mouse_xy.v>180 && mouse_xy.v<390)
        {
        if (mouse_xy.h<230)	//equipment
        wait=2;
        else       
        if (mouse_xy.h<395)
        wait=3;	//weapons
        else
        wait=4;	//repairs
        }
      }
    
    
    if (wait==2) 
     {
      buy_equipment();
      goto reset_cbuy;
     }
    else
    if (wait==3)
    {
     buy_weapons();
     goto reset_cbuy;
    }
    else
    if (wait==4) 
    {
     buy_repairs();
     goto reset_cbuy;
    }
 }	//end of !exit
    LS_DisposeHandle(cbuy_H);

}



void combat_sell(LSRAW* exit_button)
{
LSRAW *csell_picture;
Handle csell_H;
int wait,button_state;
Point mouse_xy;
int exit,i;
LS_CGrafPtr the_drawing_buffer;

        csell_H=GetZexPicture ('RCZ ',6001);	 //combat net sell  
	LS_HLock(csell_H);
	csell_picture = (LSRAW*)*csell_H;


reset_csell:	for (i=0;i<2;i++)
	{
     the_drawing_buffer=Get_back_buffer();
    erase_zex_rect(640,480);	//in pictures
	 print_crunched_pict(csell_picture,0,0);
 	 print_crunched_trans_pict(exit_button,260,380);
    Show_it();
    }
    
 while (LS_Left_Button())  { SDL_Delay(10); Show_it(); }	//wait for mouse up // give computer back time and allow mouse to move (if on screen)
wait=0;
//buy equipment
while (wait==0)
{
	Show_it();
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
     button_state=LS_Left_Button();
     if (button_state==1)
      {
        if (mouse_xy.h>260 && mouse_xy.h<392 && mouse_xy.v>410 && mouse_xy.v<460)
        { wait=1; exit=1; }	//exit

        if (mouse_xy.h>162 && mouse_xy.h<324 && mouse_xy.v > 180 && mouse_xy.v<408 )	//equipment
        wait=2;
        else       
        if (mouse_xy.h>324 && mouse_xy.h<483 && mouse_xy.v > 180 && mouse_xy.v<408)
        wait=3;	//weapons
      }
    
    
    if (wait==2) 
     {
      sell_equipment(EQUIPMENT);
      goto reset_csell;
     }
    else
    if (wait==3)
    {
      sell_equipment(WEAPONS);
     goto reset_csell;
    }
 }	//end of !exit
    LS_DisposeHandle(csell_H);

}

void sell_equipment(int what_class)
{
int button_state,exit;
int ax,ay,index,wait,sell_what;
Point mouse_xy;
extern int global_show_status;
     button_state=LS_Left_Button();
     while (button_state==1) { button_state=LS_Left_Button(); user_delay(1); Show_it(); }	//wait for mouse up 

//display the inventory screen with prices
//    global_show_status=1;
   exit=0;
   while(exit==0)
   {
    sell_what=do_sell_kit(what_class);	//in inventory
    if (sell_what!=-1)
      {
      //do sell dialog
      do_yer_actual_sell(sell_what);
      }
      else
      exit=1;
   }
   
}

//these routines have to display goods and tie up a mouse click with a good
//the goods are displayed on a grid where each node is 128*128 - this gives a grid
//of 5*3.
//we note what good is at each location with an array
int display_item_array[15];

void buy_equipment()
{
int button_state,exit;
int ax,ay,index,wait;
Point mouse_xy;

     button_state=LS_Left_Button();
     while (button_state==1) { button_state=LS_Left_Button(); user_delay(1); Show_it(); }	//wait for mouse up

display_goods(EQUIPMENT);
    Show_it();
//display_item_array[] 0-14 is now full of items
    exit=0;
    while (exit==0)
     {
//     wait_for_mouse();
		user_delay(1);	//Some time for the system so we don't hog the processor

    
     wait=0;
     //button down
      while (wait==0)
     {
      run_other_users(EQUIPMENT);	//others buy/sell!
      if (credits==0) 
	  {
		show_logout();
		break;
	  }
         button_state=LS_Left_Button();
         if (button_state==1) wait=1;
		 Show_it();
     }
    //button up
    wait=0;
    while (wait==0)
    {
	 user_delay(2); Show_it();
     button_state=LS_Left_Button();
     if (button_state==0) wait=1;
    }

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
     if ((mouse_xy.h>540 && mouse_xy.h<640 && mouse_xy.v>430 && mouse_xy.v<480) || credits==0)
     break;
     
//check if y<400, if so it's a bona fide selection
     if (mouse_xy.v<400)
      {
      //calc display_item_array index
      ax=mouse_xy.h/(640/5);	//5 rows	
      ay=mouse_xy.v/(400/3);	//3 columns
      index=ay*5+ax;
      if (index>14) index=14;	//cant be, but just in case
      if (display_item_array[index]==-1)	//nothiong there
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      else
      {
      //buy it...
      do_yer_actual_buy(display_item_array[index],EQUIPMENT);
      display_goods(EQUIPMENT);	//redraw and update display
    Show_it();
      }     
      }
     else	//y>400
     play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }

}

void buy_repairs()
{
int button_state,exit;
int ax,ay,index,wait;
Point mouse_xy;

     button_state=LS_Left_Button();
     while (button_state==1) { button_state=LS_Left_Button(); user_delay(1); Show_it(); }	//wait for mouse up

display_goods(REPAIRS);
    Show_it();
//display_item_array[] 0-14 is now full of items
    exit=0;
    while (exit==0)
     {
//     wait_for_mouse();

    
     wait=0;
     //button down
      while (wait==0)
     {
      run_other_users(REPAIRS);	//others buy/sell!
      if (credits==0) 
	  {
		show_logout();
		break;
	  }
      
         button_state=LS_Left_Button();
         if (button_state==1) wait=1;
		 Show_it();
     }
    //button up
    wait=0;
    while (wait==0)
    {
		user_delay(2); Show_it();
     button_state=LS_Left_Button();
     if (button_state==0) wait=1;
    }
//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
     if (mouse_xy.h>540 && mouse_xy.h<640 && mouse_xy.v>430 && mouse_xy.v<480 || credits==0)
     break;
     
//check if y<400, if so it's a bona fide selection
     if (mouse_xy.v<400)
      {
      //calc display_item_array index
      ax=mouse_xy.h/(640/5);	//5 rows	
      ay=mouse_xy.v/(400/3);	//3 columns
      index=ay*5+ax;
      if (index>14) index=14;	//cant be, but just in case
      if (display_item_array[index]==-1)	//nothiong there
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      else
      {
      //buy it...
      do_yer_actual_buy(display_item_array[index],REPAIRS);
      display_goods(REPAIRS);	//redraw and update display
    Show_it();
      }     
      }
     else	//y>400
     play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }

//int button_state;
//
//     button_state=LS_Left_Button();
//     while (button_state==1) button_state=LS_Left_Button();	//wait for mouse up
//display_goods(REPAIRS);
//    Show_it();
//     wait_for_mouse();
}


void buy_weapons()
{
int button_state,exit;
int ax,ay,index,wait;
Point mouse_xy;

     button_state=LS_Left_Button();
     while (button_state==1) { button_state=LS_Left_Button(); user_delay(1); Show_it(); }	//wait for mouse up

    display_goods(WEAPONS);
    Show_it();
//display_item_array[] 0-14 is now full of items
    exit=0;
    while (exit==0)
     {
    
     wait=0;
     //button down
      while (wait==0)
     {
      run_other_users(WEAPONS);	//others buy/sell!
      if (credits==0) 
	  {
		show_logout();
		break;
	  }
         button_state=LS_Left_Button();
         if (button_state==1) wait=1;
		 Show_it();
     }
    //button up
    wait=0;
    while (wait==0)
    {
		user_delay(2); Show_it();
     button_state=LS_Left_Button();
     if (button_state==0) wait=1;
    }
//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
     if (mouse_xy.h>540 && mouse_xy.h<640 && mouse_xy.v>430 && mouse_xy.v<480 || credits==0)
     break;
     
//check if y<400, if so it's a bona fide selection
     if (mouse_xy.v<400)
      {
      //calc display_item_array index
      ax=mouse_xy.h/(640/5);	//5 rows	
      ay=mouse_xy.v/(400/3);	//3 columns
      index=ay*5+ax;
      if (index>14) index=14;	//cant be, but just in case
      if (display_item_array[index]==-1)	//nothiong there
      play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
      else
      {
      //buy it...
      do_yer_actual_buy(display_item_array[index],WEAPONS);
      display_goods(WEAPONS);	//redraw and update display
    Show_it();
      }     
      }
     else	//y>400
     play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);

    }


//int button_state;
//
//     button_state=LS_Left_Button();
//     while (button_state==1) button_state=LS_Left_Button();	//wait for mouse up
//display_goods(WEAPONS);
//    Show_it();
//     wait_for_mouse();
}

extern int hull_strength;

//routine that checks and buys an item
void do_yer_actual_buy(int stock_item_id, int screen_class)
{
extern int credits, gRank;
//oldmac//extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
LS_CGrafPtr the_drawing_buffer;
unsigned char number_string[12];
Point mouse_xy;
int buyers_price,needed_fuel;
int tc_tries,tc_max_tries;

     if (check_needed(stock_item_id)==0) 	//not needed?
     {
           play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
           return;
     }
//calc tc_max_tries
tc_tries=0;
tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
     
rehaggle:	//err, a label!
     the_drawing_buffer=Get_back_buffer();

     //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

     if (ship_items[KTRADING_COMPUTER].status==1)
     dialog_H=GetZexPicture ('RCZ ',2011);	  //haggle dialog box
     else
     dialog_H=GetZexPicture ('RCZ ',2012);	//confirm dialog box
        
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//accept price or negatiate?

//    if (LS_Left_Button()==1);	//wait for mouse up
    
    display_goods(screen_class);

	print_crunched_pict(dialog_picture,180,130);
    TextFont(21);
TextSize(12);
TextFace(normal);
RGBForeColor(&rgbBlue);
     MoveTo(426,150);
     ZexNumToString(ship_stock[stock_item_id].actual_price,number_string);
     DrawString_c("Cr ");
     DrawString(number_string);
     
     draw_string_with_cr(200,150,ship_stock[stock_item_id].long_description);
//if we have a trading computer calculate price
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
     //if fuel or hull
     if (stock_item_id==FUEL)
      {
        needed_fuel=180-ship_items[KNORMAL_FUEL_TANK].value1;
        if (needed_fuel>ship_stock[stock_item_id].quantity) needed_fuel=ship_stock[stock_item_id].quantity;
        
        buyers_price=do_trading_computer
                     (ship_stock[stock_item_id].actual_price*needed_fuel, stock_item_id);
      }
     else if (stock_item_id==HULL_REPAIR)
      {
        buyers_price=do_trading_computer(ship_stock[stock_item_id].actual_price
                  *(50-hull_strength), stock_item_id);
      }
     else
     buyers_price=do_trading_computer(ship_stock[stock_item_id].actual_price, stock_item_id);

     MoveTo (200,220);
	 DrawString_c ("Trading Computer gets a price of ");
	 ZexNumToString(buyers_price,number_string);
     DrawString(number_string);
     }
     else	//no trading computer
     {
          if (stock_item_id==FUEL)
      {
        buyers_price=ship_stock[stock_item_id].actual_price
                  *(180-ship_items[KNORMAL_FUEL_TANK].value1);
      }
      else if (stock_item_id==HULL_REPAIR)
      {
        buyers_price=ship_stock[stock_item_id].actual_price*(50-hull_strength);
      }
      else	//not fuel or hull
       buyers_price=ship_stock[stock_item_id].actual_price;
     }
     

     MoveTo (200,240);
     if (ship_items[KTRADING_COMPUTER].status==1)
     DrawString_c("Accept the price, re-negotiate or cancel?");
     else
     {
     DrawString_c("Accept the price of ");
	 ZexNumToString(buyers_price,number_string);
     DrawString(number_string);
     DrawString_c(" credits or cancel?");
     
     }
 	 //oldmac//SetGDevice(saveGDH);	 //these are here so I can SEE them!
	//oldmac//SetPort(savePort);
   Show_it();
   LS_DisposeHandle(dialog_H);
     wait_for_mouse();

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
 
//have a trading computer?
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
     if (mouse_xy.h>380 && mouse_xy.h<480 && mouse_xy.v>250 && mouse_xy.v<280) return;	//cancel
     if (mouse_xy.h>280 && mouse_xy.h<380 && mouse_xy.v>250 && mouse_xy.v<280) 
        {
          //check if exceeeded number of tries
          tc_tries++;
          if (tc_tries>tc_max_tries)
               {
				the_drawing_buffer=Get_back_buffer();
				dialog_H=GetZexPicture ('RCZ ',2010);	     //blank dialog box   
        
				LS_HLock(dialog_H);
				dialog_picture = (LSRAW*)*dialog_H;
				ship_stock[stock_item_id].temp_removed=1;	//seller got bored and removed the item
				display_goods(screen_class);
				RGBForeColor(&rgbRed);

				print_crunched_pict(dialog_picture,180,130);
				MoveTo (200,180);
				DrawString_c("The seller became frustrated with");
				MoveTo (200,200);
				DrawString_c("your trading computer and has");
				MoveTo (200,220);
				DrawString_c("withdrawn the item from sale.");
				Show_it();
			   
				wait_for_mouse();
				LS_DisposeHandle(dialog_H);
			   
               return;
               }
          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
          goto rehaggle;	//recalc
        }
     }
     
     else
     if (mouse_xy.h>320 && mouse_xy.h<480 && mouse_xy.v>256 && mouse_xy.v<278) return;	//cancel
     if (mouse_xy.h>180 && mouse_xy.h<320 && mouse_xy.v>256 && mouse_xy.v<278)
     goto buy_it;
     else
     {
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        goto rehaggle;	      //do nothing
     }
//must be OK!

   
buy_it:   dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
       //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);
   display_goods(screen_class);

	print_crunched_pict(dialog_picture,180,130);

      TextFont(21);
TextSize(14);
TextFace(bold);
RGBForeColor(&rgbBlue);

//do text

	//check if players rank is sufficient to buy this item
	if ( !can_item_be_sold_to_player (stock_item_id, gRank))
	{
	char needed_rank_string[128];
		 MoveTo (200,170);
		 DrawString_c("This item is reserved. To override this");
		 MoveTo (200,188);
		 DrawString_c("command your rank needs to be at");
		 MoveTo (200,188+18);
		 sprintf ( needed_rank_string, "least %s.",rank_to_string_no_space(get_minimum_rank_for_sale(stock_item_id)));
		 DrawString_c(needed_rank_string);
		 MoveTo (200,188+18+18+18);
		 sprintf ( needed_rank_string, "Your rank is %s.",rank_to_string_no_space(gRank));
		 DrawString_c(needed_rank_string);

	}
	else
    if (buyers_price>credits)
    {
		 MoveTo (200,170);

		 if (ship_stock[stock_item_id].short_description[1]=='R' && 
			ship_stock[stock_item_id].short_description[2]=='e' &&
			ship_stock[stock_item_id].short_description[3]=='f')
		  {
		   DrawString_c("You don't have enough credit to refuel.");
		  }
		 else
		 {

		  DrawString_c("You don't have enough credit to buy");
		  MoveTo (200,188);
		  if (ship_stock[stock_item_id].short_description[1]=='S' && 
			ship_stock[stock_item_id].short_description[2]=='h' )
		   {
		   //if shields, dont draw "the"
		   }    
		  else
		   {
		   DrawString_c("the ");
		   }
		  DrawString (ship_stock[stock_item_id].short_description);
		  DrawString_c (".");
		 }
		 MoveTo (200,210);
		 DrawString_c ("You have ");
		 ZexNumToString(credits,number_string);
		 DrawString(number_string);
		 DrawString_c (" credits.");
	   
		 MoveTo (200,228);
		 DrawString_c ("It costs ");
		 ZexNumToString(buyers_price, number_string);
		 DrawString(number_string);
		 DrawString_c (" credits.");
    }	//end of price too great    
    else	//price OK
    {
			MoveTo (200,170);			 
		
			if (stock_item_id==HULL_REPAIR)
			{
				 DrawString_c("Your hull has been repaired at a cost");
				 MoveTo (200,188);
				 DrawString_c("of ");
				 ZexNumToString(buyers_price, number_string);
				 //MoveTo (200,188);
				 DrawString(number_string);
				 DrawString_c (" credits.");
			}
			else
			if (stock_item_id==FUEL)
			{
				 DrawString_c("You have been refuelled at a cost");
				 MoveTo (200,188);
				 DrawString_c("of ");
				 ZexNumToString(buyers_price, number_string);
				 DrawString(number_string);
				 DrawString_c (" credits.");
			}
			else
			{
				 DrawString_c("You have bought ");
				 if (ship_stock[stock_item_id].short_description[1]=='S' && 
					ship_stock[stock_item_id].short_description[2]=='h' )
				   {
				   //if shields, dont draw "the"
				   }    
				 else
				  {
				   DrawString_c("the");
				   MoveTo (200,188);
				  }
				 
				 DrawString (ship_stock[stock_item_id].short_description);
				 DrawString_c (" for ");
				 ZexNumToString(buyers_price, number_string);
				 MoveTo (200,206);
				 DrawString(number_string);
				 DrawString_c (" credits.");
			 }
		
			
			 credits=credits-buyers_price;
			 ship_stock[stock_item_id].quantity--;
			 //update ship
			 update_ship(stock_item_id);	//add the item to zex     
    }	//end of price ok
   
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
    Show_it();

    LS_DisposeHandle(dialog_H);
    wait_for_mouse();
}



void do_yer_actual_sell(int ship_item_id)
{
extern int credits;
extern int hull_strength;
//oldmac//extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
LS_CGrafPtr the_drawing_buffer;
unsigned char number_string[12];
Point mouse_xy;
int buyers_price,needed_fuel;
int tc_tries,tc_max_tries;
int stock_item_id;
float temp_price,fdiscount;

if (ship_item_id==KMISSILE_PYLON1 || ship_item_id==KMISSILE_PYLON2 || ship_item_id==KMISSILE_PYLON3)	//can't sell a pylon!
{
	if (ship_items[ship_item_id].modifier==0)	//no missile fitted
	return;
} 
//translate ship item to stock item
stock_item_id=translate_ship_to_stock(ship_item_id);

if (stock_item_id==-1)             report_error_c("translate_to_ship_item returned error!","",ship_item_id);

//calc tc_max_tries
tc_tries=0;
tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
     
rehaggle:	//err, a label!
     the_drawing_buffer=Get_back_buffer();

     //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

     if (ship_items[KTRADING_COMPUTER].status==1)
     dialog_H=GetZexPicture ('RCZ ',2011);	  //haggle dialog box
     else
     dialog_H=GetZexPicture ('RCZ ',2012);	//confirm dialog box
        
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//accept price or negatiate?

//    if (LS_Left_Button()==1);	//wait for mouse up
    
//    display_goods(screen_class);

	print_crunched_pict(dialog_picture,180,130);
    TextFont(21);
	TextSize(12);
	TextFace(normal);

	if (ship_items[ship_item_id].status>1)
	{
		RGBForeColor(&rgbOrange);
	}
	else
	{
		RGBForeColor(&rgbBlue);
	}
     MoveTo(426,150);
     ZexNumToString(ship_stock[stock_item_id].actual_price,number_string);
     DrawString_c("Cr ");
     DrawString(number_string);
     
     draw_string_with_cr(200,150,ship_stock[stock_item_id].long_description);


//if we have a trading computer calculate price
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
		 float buying_price,percent;
		 buying_price=ship_stock[stock_item_id].actual_price;
		  temp_price=buying_price;
		  fdiscount=.40;	//40% reduction if trading computer
		  temp_price=temp_price-(temp_price*fdiscount);
		  if (temp_price<1) temp_price=1;	//dont want credits!     
		  buyers_price=static_cast<int>(temp_price);
		 
		 buyers_price=do_trading_computer((int)buyers_price, stock_item_id);
		 if (ship_items[ship_item_id].status>1)
		 {
			buyers_price/=2;	//Just 50% if damaged
			MoveTo (200,222);
			DrawString_c("Damaged Item - Price reduced by 50%");
		 }
		 MoveTo (200,234);
		   DrawString_c ("Trading Computer gets a price of ");
		   ZexNumToString(buyers_price,number_string);
		 DrawString(number_string);
     }
     else	//no trading computer
     {

		   buyers_price=ship_stock[stock_item_id].actual_price;
		  temp_price=buyers_price;
		  fdiscount=.60;	//60% reduction if no trading computer
		  temp_price=temp_price-(temp_price*fdiscount);
		  if (temp_price<1) temp_price=1;	//dont want credits!     
		  buyers_price=static_cast<int>(temp_price);
		 if (ship_items[ship_item_id].status>1)	//2 or 3 is damaged
		 {
			buyers_price/=3;	//33% of normal buy priceif damaged
			MoveTo (200,222);
			DrawString_c("Damaged Item - Price reduced by 66%");
		 }

     }
     
	
     MoveTo (200,246);
     if (ship_items[KTRADING_COMPUTER].status==1)
     DrawString_c("Accept the offer, re-negotiate or cancel?");
     else
     {
     DrawString_c("Accept the offer of ");
	 ZexNumToString(buyers_price,number_string);
     DrawString(number_string);
     DrawString_c(" credits or cancel?");
     
     }
   Show_it();
   LS_DisposeHandle(dialog_H);
     wait_for_mouse();

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
 
//have a trading computer?
     if (ship_items[KTRADING_COMPUTER].status==1)
     {
     if (mouse_xy.h>380 && mouse_xy.h<480 && mouse_xy.v>250 && mouse_xy.v<280) return;	//cancel
     if (mouse_xy.h>280 && mouse_xy.h<380 && mouse_xy.v>250 && mouse_xy.v<280) 
        {
          //check if exceeeded number of tries
          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
          goto rehaggle;	//recalc
        }
     }
     
     else
     if (mouse_xy.h>320 && mouse_xy.h<480 && mouse_xy.v>256 && mouse_xy.v<278) return;	//cancel
     if (mouse_xy.h>180 && mouse_xy.h<320 && mouse_xy.v>256 && mouse_xy.v<278)
     goto buy_it;
     else
     {
        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
        goto rehaggle;	      //do nothing
     }
//must be OK!

   
buy_it:   dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
       //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180,130);

      TextFont(21);
TextSize(14);
TextFace(bold);
RGBForeColor(&rgbBlue);
//do text
     MoveTo (200,170);
     
      {
     DrawString_c("You have sold ");
 	   DrawString_c("the");
       MoveTo (200,188);
     DrawString (ship_stock[stock_item_id].short_description);
     DrawString_c (" for ");
     ZexNumToString(buyers_price, number_string);
     MoveTo (200,206);
     DrawString(number_string);
     DrawString_c (" credits.");
      }
    
     credits=credits+buyers_price;
     ship_stock[stock_item_id].quantity++;
     ship_items[ship_item_id].modifier=0;
	 
		if (ship_item_id==KMISSILE_PYLON1 || ship_item_id==KMISSILE_PYLON2 || ship_item_id==KMISSILE_PYLON3)	//can't sell a pylon!
		{
			//if it's a missile pylon don't set its status to 0
		}
		else
		{
		 ship_items[ship_item_id].status=0;
		}

	  if((ship_item_id==KFLATPACK) && (ship_items[ship_item_id].value1>0))
	 {
		ship_items[ship_item_id].value1--;
		if (ship_items[ship_item_id].value1>0)	//if we still have some flatpacks left then make it present again
		{
			 ship_items[ship_item_id].status=1;
			 ship_items[ship_item_id].modifier=1;
		}
	 }
	 
    Show_it();

    LS_DisposeHandle(dialog_H);
    wait_for_mouse();
}

//takes an item and return 0 - fitted already, or 1 if needed
int check_needed(int item_id)
{
extern int hull_strength;
int return_value=0;

if (ship_stock[item_id].temp_removed==1) return 0;	  //withdrawn

   switch (item_id)
        {
        case MISSILE1:
           if (ship_items[KMISSILE_PYLON1].modifier==0 ||
               ship_items[KMISSILE_PYLON2].modifier==0 ||
               ship_items[KMISSILE_PYLON3].modifier==0)
             {
              return_value=1;
             }
           break;

        case MISSILE2:
           if (ship_items[KMISSILE_PYLON1].modifier==0 ||
               ship_items[KMISSILE_PYLON2].modifier==0 ||
               ship_items[KMISSILE_PYLON3].modifier==0)
             {
              return_value=1;
             }
           break;
           
        case MISSILE3:
           if (ship_items[KMISSILE_PYLON1].modifier==0 ||
               ship_items[KMISSILE_PYLON2].modifier==0 ||
               ship_items[KMISSILE_PYLON3].modifier==0)
             {
              return_value=1;
             }
           break;
           
           
        case LASER1:
             if (ship_items[KLASER].status<=0)  return_value=1;
             if(ship_items[KLASER].modifier<1)  return_value=1;
           break;
        case LASER2:
             if (ship_items[KLASER].status<=0) return_value=1;
             else
             if(ship_items[KLASER].modifier<2)  return_value=1;

           break;
        case LASER3:
             if (ship_items[KLASER].status<=0)  return_value=1;
             else
             if(ship_items[KLASER].modifier<3)  return_value=1;
           break;

        case TASER1:
             if (ship_items[KLASER].status<=0)  return_value=1;
             if(ship_items[KLASER].modifier<11)  return_value=1;
           break;
        case TASER2:
             if (ship_items[KLASER].status<=0) return_value=1;
             else
             if(ship_items[KLASER].modifier<12)  return_value=1;

           break;
        case TASER3:
             if (ship_items[KLASER].status<=0)  return_value=1;
             else
             if(ship_items[KLASER].modifier<13)  return_value=1;
           break;
           
        case REAR_LASER1:
             if (ship_items[KREAR_LASER].status<=0) return_value=1;
             if(ship_items[KREAR_LASER].modifier<1)  return_value=1;
           break;
        case REAR_LASER2:
             if (ship_items[KREAR_LASER].status<=0)  return_value=1;
             else
             if(ship_items[KREAR_LASER].modifier<2)  return_value=1;
           break;
        case REAR_LASER3:
             if(ship_items[KREAR_LASER].status<=0)  return_value=1;
             else
             if(ship_items[KREAR_LASER].modifier<3)  return_value=1;
           break;

           break;

        case CANNON1:
             if(ship_items[KCANNON].status<=0)  return_value=1;
             if(ship_items[KCANNON].modifier==0)  return_value=1;
           break;
           
        case CANNON2:
             if(ship_items[KCANNON].status<=0)  return_value=1;
             else
             if(ship_items[KCANNON].modifier<2)  return_value=1;
           break;

        case CANNON3:
             if(ship_items[KCANNON].status<=0)  return_value=1;
             else
             if(ship_items[KCANNON].modifier<3)  return_value=1;
           break;

        case SHOCKWAVE1:
             if(ship_items[KSHOCKWAVE].status<=0)  return_value=1;
            break;

        case SHOCKWAVE2:
             if(ship_items[KSHOCKWAVE].status<=0)  return_value=1;
             else
             if(ship_items[KSHOCKWAVE].modifier<2)  return_value=1;
           break;

        case ECM:
             if(ship_items[KECM].status<=0) return_value=1;
           break;

        case MISSILE_FACTORY:
             if(ship_items[KMISSILE_FACTORY].status<=0) return_value=1;
           break;

        case TARGETTER_360:
             if(ship_items[K360].status<=0)  return_value=1;
           break;

        case FAST_DOCK:
             if(ship_items[KFAST_DOCK].status<=0)  return_value=1;
           break;

        case FLATPACK:
             if(ship_items[KFLATPACK].status<=0)  return_value=1;
             else
             if(ship_items[KFLATPACK].value1<3)  return_value=1;
             
           break;

        case EFFICIENT_ENGINE:
             if(ship_items[KFUEL_MAXIMISER].status<=0)  return_value=1;
           break;

        case RETRO_ROCKETS:
             if(ship_items[KRETRO_ROCKETS].status<=0)  return_value=1;
           break;

        case HYPERSPACE:
             if(ship_items[KHYPERSPACE].status<=0)  return_value=1;
           break;

        case FLIGHT_COMPUTER1:
             if(ship_items[KFLIGHT_COMPUTER].status<=0)  return_value=1;
           break;

        case FLIGHT_COMPUTER2:
             if(ship_items[KFLIGHT_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KFLIGHT_COMPUTER].modifier==1)  return_value=1;
           break;

        case NAV_COMPUTER1:
             if(ship_items[KNAV_COMPUTER].status<=0)  return_value=1;
             if (ship_items[KNAV_COMPUTER].modifier==0) return_value=1;
           break;
        case NAV_COMPUTER2:
             if(ship_items[KNAV_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KNAV_COMPUTER].modifier<2)  return_value=1;
           break;


        case BULLET_PROOF_EJECTION:
             if(ship_items[KEJECTION].status<=0)  return_value=1;
             else
             if(ship_items[KEJECTION].modifier<2)  return_value=1;
           break;

        case SHIELDS1:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             if (ship_items[KSHIELDS].modifier==0) return_value=1;
           break;

        case SHIELDS2:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<2)  return_value=1;
           break;

        case SHIELDS3:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<3)  return_value=1;
           break;

        case SHIELDS4:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<4)  return_value=1;
           break;

        case SHIELDS5:
             if(ship_items[KSHIELDS].status<=0)  return_value=1;
             else
             if(ship_items[KSHIELDS].modifier<5)  return_value=1;
           break;

        case HULL_REPAIR:
        if(hull_strength!=50) return_value=1;
           break;

        case FUEL:
			 if(ship_items[KNORMAL_FUEL_TANK].value1!=180) return_value=1;

           break;

        case REPAIR_SYSTEMS1:
             if(ship_items[KREPAIR_SYSTEMS].status<=0)  return_value=1;
           break;

        case REPAIR_SYSTEMS2:
             if(ship_items[KREPAIR_SYSTEMS].status<=0)  return_value=1;
             else
             if(ship_items[KREPAIR_SYSTEMS].modifier<2)  return_value=1;
           break;

        case REPAIR_SYSTEMS3:
             if(ship_items[KREPAIR_SYSTEMS].status<=0)  return_value=1;
             else
             if(ship_items[KREPAIR_SYSTEMS].modifier<3)  return_value=1;
           break;

        case EJECTION:
             if(ship_items[KEJECTION].status<=0) return_value=1;
           break;

        case TRADING_COMPUTER1:
             if(ship_items[KTRADING_COMPUTER].status<=0)  return_value=1;
           break;

        case TRADING_COMPUTER2:
             if(ship_items[KTRADING_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KTRADING_COMPUTER].modifier<2)  return_value=1;
           break;

        case TRADING_COMPUTER3:
             if(ship_items[KTRADING_COMPUTER].status<=0)  return_value=1;
             else
             if(ship_items[KTRADING_COMPUTER].modifier<3)  return_value=1;
           break;

        case AUTO_HULL_REPAIR:
             if(ship_items[KAUTO_HULL_REPAIR].status<=0) return_value=1;
           break;

        case BOID_ADDITION:
             if(ship_items[KBOID_ADDITION].status<=0) return_value=1;
           break;

        case ASTEROID_ADDITION:
             if(ship_items[KASTEROID_ADDITION].status<=0) return_value=1;
           break;

        default:
            report_error_c("Unknown item in check_needed!","",item_id);
           break;
        }
    return return_value;
}	//end of check_needed



void update_ship(int item_id)
{
extern int hull_damage;

   switch (item_id)
        {
        case MISSILE1:
			if (ship_items[KMISSILE_PYLON1].modifier==0)
             {
             ship_items[KMISSILE_PYLON1].status=1;
             ship_items[KMISSILE_PYLON1].modifier=1;
             }
           else
           if (ship_items[KMISSILE_PYLON2].modifier==0)
             {
             ship_items[KMISSILE_PYLON2].status=1;
             ship_items[KMISSILE_PYLON2].modifier=1;
             }
           else
           if (ship_items[KMISSILE_PYLON3].modifier==0)
             {
             ship_items[KMISSILE_PYLON3].status=1;
             ship_items[KMISSILE_PYLON3].modifier=1;
             }
			 select_next_pylon();	//fix for 139
                      
           break;
        case MISSILE2:
           if (ship_items[KMISSILE_PYLON1].modifier==0)
             {
             ship_items[KMISSILE_PYLON1].status=1;
             ship_items[KMISSILE_PYLON1].modifier=2;
             }
           else
           if (ship_items[KMISSILE_PYLON2].modifier==0)
             {
             ship_items[KMISSILE_PYLON2].status=1;
             ship_items[KMISSILE_PYLON2].modifier=2;
             }
           else
           if (ship_items[KMISSILE_PYLON3].modifier==0)
             {
             ship_items[KMISSILE_PYLON3].status=1;
             ship_items[KMISSILE_PYLON3].modifier=2;
             }
			select_next_pylon();
                      
           break;
        case MISSILE3:
           if (ship_items[KMISSILE_PYLON1].modifier==0)
             {
             ship_items[KMISSILE_PYLON1].status=1;
             ship_items[KMISSILE_PYLON1].modifier=3;
             }
           else
           if (ship_items[KMISSILE_PYLON2].modifier==0)
             {
             ship_items[KMISSILE_PYLON2].status=1;
             ship_items[KMISSILE_PYLON2].modifier=3;
             }
           else
           if (ship_items[KMISSILE_PYLON3].modifier==0)
             {
             ship_items[KMISSILE_PYLON3].status=1;
             ship_items[KMISSILE_PYLON3].modifier=3;
             }
			select_next_pylon();
                      
           break;
           
           
        case LASER1:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=1;
           break;
        case LASER2:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=2;
           break;
        case LASER3:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=3;
           break;

        case TASER1:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=11;
           break;
        case TASER2:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=12;
           break;
        case TASER3:
             ship_items[KLASER].status=1;
             ship_items[KLASER].modifier=13;
           break;


        case REAR_LASER1:
             ship_items[KREAR_LASER].status=1;
             ship_items[KREAR_LASER].modifier=1;
           break;
        case REAR_LASER2:
             ship_items[KREAR_LASER].status=1;
             ship_items[KREAR_LASER].modifier=2;
           break;
        case REAR_LASER3:
             ship_items[KREAR_LASER].status=1;
             ship_items[KREAR_LASER].modifier=3;
           break;

        case CANNON1:
             ship_items[KCANNON].status=1;
             ship_items[KCANNON].modifier=1;
           break;
        case CANNON2:
             ship_items[KCANNON].status=1;
             ship_items[KCANNON].modifier=2;
           break;
        case CANNON3:
             ship_items[KCANNON].status=1;
             ship_items[KCANNON].modifier=3;
           break;

        case SHOCKWAVE1:
			ship_items[KSHOCKWAVE].status=1;
			ship_items[KSHOCKWAVE].obj_class=WEAPONS;
			ship_items[KSHOCKWAVE].modifier=1;
			ship_items[KSHOCKWAVE].value1=3;

           break;
        case SHOCKWAVE2:
			ship_items[KSHOCKWAVE].status=1;
			ship_items[KSHOCKWAVE].obj_class=WEAPONS;
			ship_items[KSHOCKWAVE].modifier=2;
			ship_items[KSHOCKWAVE].value1=3;
           break;

        case ECM:
			ship_items[KECM].status=1;
			ship_items[KECM].obj_class=WEAPONS;
			ship_items[KECM].modifier=1;
			ship_items[KECM].value1=1;
			break;

        case MISSILE_FACTORY:
			ship_items[KMISSILE_FACTORY].status=1;
			ship_items[KMISSILE_FACTORY].obj_class=WEAPONS;
			ship_items[KMISSILE_FACTORY].modifier=1;
			ship_items[KMISSILE_FACTORY].value1=1;
           break;

        case TARGETTER_360:
			ship_items[K360].status=1;
			ship_items[K360].obj_class=WEAPONS;
			ship_items[K360].modifier=1;
			ship_items[K360].value1=1;
           break;

        case FAST_DOCK:
			ship_items[KFAST_DOCK].status=1;
			ship_items[KFAST_DOCK].obj_class=EQUIPMENT;
			ship_items[KFAST_DOCK].modifier=1;
			ship_items[KFAST_DOCK].value1=1;
           break;

        case FLATPACK:
             ship_items[KFLATPACK].status=1;
             ship_items[KFLATPACK].value1++;
           break;

        case EFFICIENT_ENGINE:
             ship_items[KFUEL_MAXIMISER].status=1;
             ship_items[KFUEL_MAXIMISER].modifier=1;
           break;

        case RETRO_ROCKETS:
             ship_items[KRETRO_ROCKETS].status=1;
             ship_items[KRETRO_ROCKETS].value1=3;	//three shots
             ship_items[KRETRO_ROCKETS].modifier=1;
           break;

        case HYPERSPACE:
			ship_items[KHYPERSPACE].status=1;
			ship_items[KHYPERSPACE].value1=3;	//You get 3 shots
			ship_items[KHYPERSPACE].modifier=1;
			ship_items[KHYPERSPACE].obj_class=EQUIPMENT;
           break;

        case FLIGHT_COMPUTER1:
             ship_items[KFLIGHT_COMPUTER].status=1;
             ship_items[KFLIGHT_COMPUTER].modifier=1;
           break;

        case FLIGHT_COMPUTER2:
             ship_items[KFLIGHT_COMPUTER].status=1;
             ship_items[KFLIGHT_COMPUTER].modifier=2;
           break;

        case NAV_COMPUTER1:
             ship_items[KNAV_COMPUTER].status=1;
             ship_items[KNAV_COMPUTER].modifier=1;
           break;
        case NAV_COMPUTER2:
             ship_items[KNAV_COMPUTER].status=1;
             ship_items[KNAV_COMPUTER].modifier=2;
           break;


        case BULLET_PROOF_EJECTION:
             ship_items[KEJECTION].status=1;
             ship_items[KEJECTION].modifier=2;
           break;

        case SHIELDS1:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=1;
             ship_items[KSHIELDS].value1=1000;
           break;

        case SHIELDS2:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=2;
             ship_items[KSHIELDS].value1=1000;

           break;

        case SHIELDS3:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=3;
            ship_items[KSHIELDS].value1=1000;
           break;

        case SHIELDS4:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=4;
             ship_items[KSHIELDS].value1=1000;

           break;

        case SHIELDS5:
             ship_items[KSHIELDS].status=1;
             ship_items[KSHIELDS].modifier=5;
             ship_items[KSHIELDS].value1=1000;

           break;

        case HULL_REPAIR:
			 hull_damage=0;
             hull_strength=50;
           break;

        case FUEL:
			 ship_items[KNORMAL_FUEL_TANK].value1=180;

           break;

        case REPAIR_SYSTEMS1:
             ship_items[KREPAIR_SYSTEMS].status=1;
             ship_items[KREPAIR_SYSTEMS].modifier=1;
           break;

        case REPAIR_SYSTEMS2:
             ship_items[KREPAIR_SYSTEMS].status=1;
             ship_items[KREPAIR_SYSTEMS].modifier=2;
           break;

        case REPAIR_SYSTEMS3:
             ship_items[KREPAIR_SYSTEMS].status=1;
             ship_items[KREPAIR_SYSTEMS].modifier=3;
           break;

        case EJECTION:
             ship_items[KEJECTION].status=1;
             ship_items[KEJECTION].modifier=1;
           break;

        case TRADING_COMPUTER1:
             ship_items[KTRADING_COMPUTER].status=1;
             ship_items[KTRADING_COMPUTER].modifier=1;
           break;

        case TRADING_COMPUTER2:
             ship_items[KTRADING_COMPUTER].status=1;
             ship_items[KTRADING_COMPUTER].modifier=2;
           break;

        case TRADING_COMPUTER3:
             ship_items[KTRADING_COMPUTER].status=1;
             ship_items[KTRADING_COMPUTER].modifier=3;
           break;

        case AUTO_HULL_REPAIR:
			ship_items[KAUTO_HULL_REPAIR].status=1;
			ship_items[KAUTO_HULL_REPAIR].modifier=1;
			ship_items[KAUTO_HULL_REPAIR].value1=1;
			ship_items[KAUTO_HULL_REPAIR].obj_class=REPAIRS;
           break;

        case BOID_ADDITION:
			ship_items[KBOID_ADDITION].status=1;
			ship_items[KBOID_ADDITION].modifier=1;
			ship_items[KBOID_ADDITION].value1=1;
			ship_items[KBOID_ADDITION].obj_class=WEAPONS;
           break;

        case ASTEROID_ADDITION:
			ship_items[KASTEROID_ADDITION].status=1;
			ship_items[KASTEROID_ADDITION].modifier=1;
			ship_items[KASTEROID_ADDITION].value1=1;
			ship_items[KASTEROID_ADDITION].obj_class=WEAPONS;
           break;

        default:
            report_error_c("Unknown item in update_ship!","",item_id);
           break;
        }
}


int translate_ship_to_stock(int item_id)
{
extern int hull_damage;

   switch (item_id)
        {
        case KMISSILE_PYLON1:
           
		   if (ship_items[KMISSILE_PYLON1].modifier==1)
			return 	MISSILE1;
           else
           if (ship_items[KMISSILE_PYLON1].modifier==2)
	        return 	MISSILE2;
	    else
           if (ship_items[KMISSILE_PYLON1].modifier==3)
                return 	MISSILE3;                      
           break;
        
        case KMISSILE_PYLON2:
           if (ship_items[KMISSILE_PYLON2].modifier==1)
		return 	MISSILE1;
           else
           if (ship_items[KMISSILE_PYLON2].modifier==2)
	        return 	MISSILE2;
	    else
           if (ship_items[KMISSILE_PYLON2].modifier==3)
                return 	MISSILE3;                      
                      
           break;
        case KMISSILE_PYLON3:
           if (ship_items[KMISSILE_PYLON3].modifier==1)
		return 	MISSILE1;
           else
           if (ship_items[KMISSILE_PYLON3].modifier==2)
	        return 	MISSILE2;
	    else
           if (ship_items[KMISSILE_PYLON3].modifier==3)
                return 	MISSILE3;                      
                      
                      
           break;
           
           
        case KLASER:
           if (ship_items[KLASER].modifier==1)
		return 	LASER1;
           else
           if (ship_items[KLASER].modifier==2)
	        return 	LASER2;
	    else
           if (ship_items[KLASER].modifier==3)
                return 	LASER3;                      

           if (ship_items[KLASER].modifier==11)
		return 	TASER1;
           else
           if (ship_items[KLASER].modifier==12)
	        return 	TASER2;
	    else
           if (ship_items[KLASER].modifier==13)
                return 	TASER3;                      
                      
                      
           break;



        case KREAR_LASER:
           if (ship_items[KREAR_LASER].modifier==1)
		return 	REAR_LASER1;
           else
           if (ship_items[KREAR_LASER].modifier==2)
	        return 	REAR_LASER2;
	    else
           if (ship_items[KREAR_LASER].modifier==3)
                return 	REAR_LASER3;                      
           break;


        case KCANNON:
           if (ship_items[KCANNON].modifier==1)
		return 	CANNON1;
           else
           if (ship_items[KCANNON].modifier==2)
	        return 	CANNON2;
	    else
           if (ship_items[KCANNON].modifier==3)
                return 	CANNON3;                      
           break;


        case KSHOCKWAVE:
           if (ship_items[KSHOCKWAVE].modifier==1)
		return 	SHOCKWAVE1;
           else
           if (ship_items[KSHOCKWAVE].modifier==2)
	        return 	SHOCKWAVE2;

           break;

        case KECM:
                 return ECM;
           break;


        case KMISSILE_FACTORY:
		return MISSILE_FACTORY;
           break;

        case K360:
		return TARGETTER_360; 
	    break;

        case KFAST_DOCK:
		return FAST_DOCK;
           break;

        case KFLATPACK:
		return FLATPACK;
           break;

        case KFUEL_MAXIMISER:
		return EFFICIENT_ENGINE;
           break;

        case KRETRO_ROCKETS:
		return RETRO_ROCKETS;
           break;

        case KHYPERSPACE:
		return HYPERSPACE;
           break;

        case KFLIGHT_COMPUTER:
             if (ship_items[KFLIGHT_COMPUTER].modifier==1)
                 return FLIGHT_COMPUTER1;
             else
             if (ship_items[KFLIGHT_COMPUTER].modifier==2)
                 return FLIGHT_COMPUTER2;
                 
           break;


        case KNAV_COMPUTER:
             if (ship_items[KNAV_COMPUTER].modifier==1)
                 return NAV_COMPUTER1;
             else
             if (ship_items[KNAV_COMPUTER].modifier==2)
                 return NAV_COMPUTER2;
                 
           break;


        case KEJECTION:
             if (ship_items[KEJECTION].modifier<2)
                 return EJECTION;
             else
             if (ship_items[KEJECTION].modifier==2)
                 return BULLET_PROOF_EJECTION;
                 
           break;


        case KSHIELDS:
             if (ship_items[KSHIELDS].modifier==1)
                 return SHIELDS1;
             else
             if (ship_items[KSHIELDS].modifier==2)
                 return SHIELDS2;
             else
             if (ship_items[KSHIELDS].modifier==3)
                 return SHIELDS3;
             else
             if (ship_items[KSHIELDS].modifier==4)
                 return SHIELDS4;
             else
             if (ship_items[KSHIELDS].modifier==5)
                 return SHIELDS5;
                 
           break;



        case KREPAIR_SYSTEMS:
             if (ship_items[KREPAIR_SYSTEMS].modifier==1)
                 return REPAIR_SYSTEMS1;
             else
             if (ship_items[KREPAIR_SYSTEMS].modifier==2)
                 return REPAIR_SYSTEMS2;
             else
             if (ship_items[KREPAIR_SYSTEMS].modifier==3)
                 return REPAIR_SYSTEMS3;
           break;


        case KTRADING_COMPUTER:
             if (ship_items[KTRADING_COMPUTER].modifier==1)
                 return TRADING_COMPUTER1;
             else
             if (ship_items[KTRADING_COMPUTER].modifier==2)
                 return TRADING_COMPUTER2;
             else
             if (ship_items[KTRADING_COMPUTER].modifier==3)
                 return TRADING_COMPUTER3;
           break;



        case KAUTO_HULL_REPAIR:
             return AUTO_HULL_REPAIR;	//this was returning HULL_REPAIR which is much cheaper!
           break;

        case KBOID_ADDITION:
             return BOID_ADDITION;
           break;

        case KASTEROID_ADDITION:
             return ASTEROID_ADDITION;
           break;

        case KRADAR:
             return ASTEROID_ADDITION;
           break;

        default:
            report_error_c("Unknown item in translate_ship_to_stock!","",item_id);
           break;
        }
return -1;
}



int do_trading_computer(int actual_price, int stock_index)
{
float temp_price,fdiscount;
int tc_class;
int discount=5;

temp_price=actual_price;

     if (ship_items[KTRADING_COMPUTER].status==1)
     {
      tc_class=ship_items[KTRADING_COMPUTER].modifier;
      if (tc_class==1) 
      {
       discount=5;
       discount=RangedRdm(discount*100,(discount+ship_stock[stock_index].quantity)*100 );
      }
      else
      if (tc_class==2)
      {
       discount=16;
       discount=RangedRdm(discount*100,(discount+(ship_stock[stock_index].quantity*4)*100) );
      }
      else
      if (tc_class==3)
      {
       discount=25;
       discount=RangedRdm(discount*100,(discount+(ship_stock[stock_index].quantity*8)*100) );
      }

      fdiscount=(float)discount/100;
      if (fdiscount>60) fdiscount=60;
      temp_price=temp_price-(temp_price*(fdiscount/100));
      if (temp_price<1) temp_price=1;	//dont want credits!     
      return static_cast<int>(temp_price);
     }
     else
     return actual_price;

}
void draw_string_with_cr(int x, int y, Str255 the_string)
{
int end=1;
int line_end;
int in_index=1;
int char_count;
int total_char_count;
Str255 line_str;	//this must be pascal

total_char_count=(int)the_string[0];

if (the_string[0]==0)
{
	return;	//oops, Stu has cocked up
}
while (end==1)	//another line?
	{
		MoveTo(x,y);
		//now copy a line
		char_count=0;	//chars on this line
		line_end=0;
		//copy char to line
		while(line_end==0)
		{
			line_str[char_count+1]=the_string[in_index];
			if ((line_str[char_count+1]==13) || (line_str[char_count+1]==10)) line_end=1;	//it's a cr
			in_index++;
			if (in_index==total_char_count) 
			{
				end=0;	//done all chars?
				line_end=1;	//eol as well
			}
			char_count++;
		}
		line_str[0]=(unsigned char) char_count;
		DrawString(line_str);
		y=y+15;
	}
}

void wait_for_mouse()
{
int wait=0;
int button_state;

wait=0;
//button down
while (wait==0)
 {
	button_state=LS_Left_Button();
	if (button_state==1) wait=1;
	else 
	{ SDL_Delay(20);		// don't hog entire processor waiting on the mouse
		Show_it();			// display mouse cursor
	}
 }

//button up
wait=0;
while (wait==0)
 {
     button_state=LS_Left_Button();
     if (button_state==0) wait=1;
	 else 
	 {
		 SDL_Delay(20);		// don't hog entire processor waiting on the mouse
		 Show_it();			// disable mouse cursor	 
	 }
 }

}


void display_goods(int what_class)
{
extern int credits,tutorial;

int display_item_array_index,ship_stock_index,x;
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
unsigned char number_string[12];

init_show_stock();	//reset prinitng coords
     the_drawing_buffer=Get_back_buffer();
//draw 15 small windows
     //oldmac//GetPort (&savePort);
	//oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	//oldmac//SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	   EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect_colour(640,480,242);	//in pictures
     TextFace(0);
     draw_windows();

display_item_array_index=ship_stock_index=0;
for (x=0;x<15;x++) display_item_array[x]=-1;	//clear out display array

#if BUY_SHOW_ALL_OFFSET == -1
        for (x=0;x<NUMBER_OF_GOODS+1;x++)
           {
             if (ship_stock[ship_stock_index].obj_class==what_class && ship_stock[ship_stock_index].quantity>0)
                {
                  show_stock(ship_stock_index);
                  display_item_array[display_item_array_index]=ship_stock_index;
                  display_item_array_index++;
                  if (display_item_array_index>14) x=1000;	//prevent weird crashes - too many goods to display
                }  
             ship_stock_index++;               
           }
#else	//for debug, we show all items
		ship_stock_index=BUY_SHOW_ALL_OFFSET;
        for (x=0;x<NUMBER_OF_GOODS+1;x++)
           {
                if (ship_stock_index<NUMBER_OF_GOODS)
				{
                  show_stock(ship_stock_index);
                  display_item_array[display_item_array_index]=ship_stock_index;
                  display_item_array_index++;
                  if (display_item_array_index>14) x=1000;	//prevent weird crashes - too many goods to display
                }  
             ship_stock_index++;               
           }
#endif
if (training_mission!=0 || tutorial==1)
   {
    MoveTo(245,420);
    RGBForeColor(&rgbRed);
    DrawString_c("*Simulated items for training purposes*");
   }

//show credits in bottom of screen

TextFont(21);
TextSize(14);
TextFace(bold);
RGBForeColor(&rgbBlue);

//MoveTo (0,440);
//DrawString("Available credit:");
ZexNumToString(credits,number_string);
MoveTo (130,454);
DrawString(number_string);

	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
//    Show_it();

}



//runs through ships items displaying those that fit the obj_class
void display_ship_goods(int what_class)
{
extern int credits;

int display_item_array_index,ship_stock_index,x;
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
unsigned char number_string[12];

init_show_stock();	//reset prinitng coords
     the_drawing_buffer=Get_back_buffer();
//draw 15 small windows
     //oldmac//GetPort (&savePort);
	//oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	//oldmac//SetGDevice(mZexGDH);
	RGBBackColor (&rgbBlack);
//	   EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect_colour(640,480,242);	//in pictures
     TextFace(0);
     draw_windows();

display_item_array_index=ship_stock_index=0;
for (x=0;x<15;x++) display_item_array[x]=-1;	//clear out display array


      for (x=0;x<NUMBER_OF_GOODS+1;x++)
           {
             if (ship_stock[ship_stock_index].obj_class==what_class && ship_stock[ship_stock_index].quantity>0)
                {
                  show_stock(ship_stock_index);
                  display_item_array[display_item_array_index]=ship_stock_index;
                  display_item_array_index++;
                  if (display_item_array_index>14) x=1000;	//prevent weird crashes - too many goods to display
                }  
             ship_stock_index++;               
           }
//show credits in bottom of screen
TextFont(21);
TextSize(14);
TextFace(bold);
RGBForeColor(&rgbBlue);

//MoveTo (0,440);
//DrawString("Available credit:");
ZexNumToString(credits,number_string);
MoveTo (130,454);
DrawString(number_string);

	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
//    Show_it();

}

void draw_windows()
{
LSRAW *the_window_picture;
Handle window_H;
LSRAW *the_bar_picture;
Handle bar_H;

int x,y,i;
x=0;
y=0;

    window_H=GetZexPicture ('RCZ ',2008);	//small window   
	LS_HLock(window_H);
	the_window_picture = (LSRAW*)*window_H;

    bar_H=GetZexPicture ('RCZA',129);	//bottom bar   
	LS_HLock(bar_H);
	the_bar_picture = (LSRAW*)*bar_H;
	print_pict(the_bar_picture,0,430);

   for (i=0; i<15;i++)
   {
	print_crunched_pict(the_window_picture,x*127+2,y*140);
     x++; 
     if (x>4) 
     {
      x=0;
      if (y<4)	//a little protection
      y++;
     }
    }
    
LS_DisposeHandle(window_H);
LS_DisposeHandle(bar_H);
}

int show_x,show_y;
void init_show_stock()
{
show_x=show_y=0;
}

void show_stock(int stock_index)
{
extern kit_desc ship_items[ITEMS_COUNT];
extern int hull_strength;
int x,y,width,price;
Rect erase_desc = {111,20,123,100};
unsigned char number_string[12];
LSRAW *item_picture;
Handle item_picture_H;

x=show_x*128; y=show_y*140;

//get the items picture
    item_picture_H=GetZexPicture ('RCZ ',ship_stock[stock_index].rcz_id);	//the picture handle  
	LS_HLock(item_picture_H);
	item_picture = (LSRAW*)*item_picture_H;
	print_crunched_trans_pict(item_picture,x+30,y+24);
	LS_DisposeHandle(item_picture_H);

//draw the string
RGBBackColor(&rgbBlack);

TextFont(21);
TextSize(9);
RGBForeColor(&rgbLightBlue);
//center the string
width=StringWidth(ship_stock[stock_index].short_description);
width=128-width;
width/=2;
erase_desc.top+=y;
erase_desc.left+=x;
erase_desc.bottom+=y;
erase_desc.right+=x;
EraseRect(&erase_desc);

MoveTo(x+width,y+120);
DrawString(ship_stock[stock_index].short_description);
//show stock level
MoveTo (x+46,y+100);
  ZexNumToString(ship_stock[stock_index].quantity,number_string);
  DrawString(number_string);


//show price
MoveTo (x+43,y+19);
if (ship_stock[stock_index].temp_removed==1)
{
RGBForeColor(&rgbRed);
  DrawString_c("Withdrawn!");
RGBForeColor(&rgbLightBlue);
show_x++; 
if (show_x>4) 
 {
  show_x=0;
  if (show_y<4)	//a little protection
  show_y++;
 }
return;
}

if (check_needed(stock_index))
{
  //if stock is fuel or hull repairs we multiply needed by basic
  price=ship_stock[stock_index].actual_price;
  if (stock_index==FUEL)
  {
  price=price*(180-ship_items[KNORMAL_FUEL_TANK].value1);
  }
  else if (stock_index==HULL_REPAIR)
  {
  price=price*(50-hull_strength);
  }
  ZexNumToString(price,number_string);
  DrawString(number_string);
}
else
  DrawString_c("Not Needed");

//
// Overwrite "Reserved" if we can't get this item yet
//
extern int gRank;
if ( !can_item_be_sold_to_player (stock_index, gRank))
{
	TextSize(14);
	RGBForeColor(&rgbBlack);
	for(int i=-3; i<4; i++)
	{
		int sign = (i>=0)?1:-1;
		int x_mod = sign * ((i&2)>>1);
		int y_mod = sign * (i&1);
		MoveTo (x+25+x_mod, y+60+y_mod);
		DrawString_c("RESERVED");
	}
	MoveTo (x+25,y+60);
	RGBForeColor(&rgbRed);
	DrawString_c("RESERVED");
}



show_x++; 
if (show_x>4) 
 {
  show_x=0;
  if (show_y<4)	//a little protection
  show_y++;
 }
}




//flash connecting on screen
int do_buy_connect()
{
extern int mothership_cat;
Handle zoom_H;

LSRAW *the_zoom_picture;
LS_CGrafPtr the_drawing_buffer;
int i, toggle;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
int return_val;

return_val=0;	//asume OK

toggle=1;
     //oldmac//GetPort (&savePort);
     //oldmac//saveGDH = GetGDevice();	    //All calls to QD are surrounded by save and set gdevices
     //oldmac//SetGDevice(mZexGDH);
//first fade the screen o blue
for (i=243;i>235;i--)
{
    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(640,480,i);	//in pictures
    Show_it();
    user_delay(5);

}
//no show connection porgress
for (i=0;i<20;i++)
{
    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(640,480,235);	//in pictures
    toggle=toggle^1;
    if (toggle==0) 
    {
     	TextFont(21);
	TextSize(24);
	TextFace(normal);
	RGBForeColor(&rgbLightBlue);
     	MoveTo(240,200);
        if (i<10)
      	DrawString_c ("Connecting...");
        else if (i<16) DrawString_c ("Negotiating...");
        else 
        {
		extern int docked_at_shields;
		extern float docked_at_power_rating;
		
		if (docked_at_shields<8000 && RangedRdm(0,docked_at_shields)<4000) comms_will_fail=2;	//if mothership damaged comms may fail
		if (docked_at_power_rating<20.0) 
		{
			docked_at_power_rating+=5;	//make sure player can eventually connect (simulate mship repair)
			comms_will_fail=2;
		}   
		     
		if (comms_will_fail>0)
		{
			RGBForeColor(&rgbRed);
			MoveTo(210,200);
			DrawString_c ("Couldn't negotiate!");        
			return_val=-2;
         }
         else
         //if alien ship and we have no trading computer then fail
         if (mothership_cat==ALIEN && ship_items[KTRADING_COMPUTER].status<=0)
         {
 	 RGBForeColor(&rgbRed);
      	 MoveTo(210,200);
         DrawString_c ("Couldn't negotiate!");        
 	 return_val=-1;
         }
         else if (mothership_cat==ALIEN)
         DrawString_c ("Translating.");
         else
         DrawString_c ("Connected.");
        }
    }
    Show_it();
    user_delay(12);
}

if (return_val==-1)
{
    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(640,480,235);	//in pictures
	RGBForeColor(&rgbRed);
     	MoveTo(50,200);
        DrawString_c ("Failed! Trading Computer needed for alien systems.");        
	TextSize(12);
     	MoveTo(240,300);
        DrawString_c ("Click mouse button to exit.");        
    Show_it();
    wait_for_mouse();


}
else
if (return_val==-2)
{
    the_drawing_buffer=Get_back_buffer();
    erase_zex_rect_colour(640,480,235);	//in pictures
	RGBForeColor(&rgbRed);
	MoveTo(60,200);
	if (comms_will_fail==1)
        DrawString_c ("Connect failed; network too busy. Please try later.");        
	else
        DrawString_c ("Connect failed; network damaged.  Please try later.");        
	
	TextSize(12);
     	MoveTo(240,300);
        DrawString_c ("Click mouse button to exit.");        
    Show_it();
    wait_for_mouse();


}



	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);

if (return_val==0)
{
 play_zsound(sound_test, sound_high_pri, ls1_chan, sound_vol_7);
   zoom_H=GetZexPicture ('RCZ ',2005);	//zoomed   
 	if (zoom_H==0) report_error_c("Memory Error: do_buy_connect, zoom_H","",-1);
	LS_HLock(zoom_H);
	the_zoom_picture = (LSRAW*)*zoom_H;

    the_drawing_buffer=Get_back_buffer();
//	  SetPort((GrafPtr)the_drawing_buffer);
//    GetPort (&savePort);
//	  saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
//	  SetGDevice(mZexGDH);
//	  RGBBackColor (&rgbBlack);
//	  EraseRect(&the_drawing_buffer->portRect);
    erase_zex_rect(640,480);	//in pictures
//	  SetGDevice(saveGDH);	  //these are here so I can SEE them!
//	  SetPort(savePort);
	print_crunched_pict(the_zoom_picture,0,0);
    Show_it();

	LS_DisposeHandle(zoom_H);
}
return return_val;
}

void do_buy_intro()
{
//get the picture - rcz 2004
extern const RGBColor	rgbBlack;
extern Ptr screenaddr;
extern int screenwidth;

LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
Handle computer_H;

LSRAW *the_picture;
LSRAW *the_zoom_picture;
Handle zoom_H;

LSRAW* pictbuffer;
int memsize,i,j,y_correct,x_correct;
float mag;


    computer_H=GetZexPicture ('RCZ ',2004);	//computer   
 	if (computer_H==0) report_error_c("Memory Error: do_buy_intro, computer_H","",-1);
	LS_HLock(computer_H);
	the_picture = (LSRAW*)*computer_H;

//    zoom_H=GetZexPicture ('RCZ ',2005);	//zoomed   
// 	if (zoom_H==0) report_error("Memory Error: do_buy_intro, zoom_H","",-1);
//	  LS_HLock(zoom_H);
//	  the_zoom_picture = (LSRAW*)*zoom_H;

//we need to decompress this picture using my ever-so-useful picture utilities
	memsize=(*the_picture).pwidth;
	memsize*=(*the_picture).pheight;
	memsize+=80;
//	pictbuffer=(LSRAW*) malloc(memsize); //new(nothrow) char[memsize];
	pictbuffer=(LSRAW*) new(nothrow) char[memsize];
 	if (pictbuffer==0) report_error_c("Memory Error: do_buy_intro","",-1);
 	decrunch_pict(the_picture,pictbuffer);


 	mag=1.0;
 	y_correct=0;
    	x_correct=0;
    
	the_drawing_buffer=Get_back_buffer();
	print_pict_magnified(pictbuffer,0,0,y_correct,x_correct,mag);
    	Show_it();	//show us what happened!!!
    	user_delay(30);

 	for (i=0;i<11;i++)
 	{
	the_drawing_buffer=Get_back_buffer();
	print_pict_magnified(pictbuffer,0,0,y_correct,x_correct,mag);
     Show_it();	//show us what happened!!!
     user_delay(1);
     y_correct+=34;
     x_correct-=8;	//these get us centered as we zoom in
     mag+=0.2;
    }


//now print the zoomed picture
//    the_drawing_buffer=Get_back_buffer();
////	    SetPort((GrafPtr)the_drawing_buffer);
////    GetPort (&savePort);
////	    saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
////	    SetGDevice(mZexGDH);
////	    RGBBackColor (&rgbBlack);
////	    EraseRect(&the_drawing_buffer->portRect);
//    erase_zex_rect(640,480);	  //in pictures
////	    SetGDevice(saveGDH);	//these are here so I can SEE them!
////	    SetPort(savePort);
//	  print_crunched_pict(the_zoom_picture,0,0);
//    Show_it();
//	LS_DisposeHandle(zoom_H);
	LS_DisposeHandle(computer_H);
	//DisposePtr((Ptr) pictbuffer);
	delete[] pictbuffer;
	//free(pictbuffer);
}

//
// These routines do the limiting of stock
//
static int get_minimum_rank_for_sale(int item_id)
{
	if (docked_at_allegiance==HUMAN)
	{
		return ship_stock[item_id].earth_rank_blocked+1;
	}
	else
	{
		return ship_stock[item_id].alien_rank_blocked+1;
	}
}

static bool can_item_be_sold_to_player (int item, int player_rank)
{
	if (docked_at_allegiance==HUMAN)
	{
		if (ship_stock[item].earth_rank_blocked==_always) return true;	// always available so let player buy it...
		
		if (ship_stock[item].earth_rank_blocked==_never) return true;	// this look wierd. However it is right, since the only way 
																		// the player could have got this at this mothership was by 
																		// selling it himself.
		
		if (player_rank>ship_stock[item].earth_rank_blocked) return true;	// the player's rank is correct
		
		return false;													// the players rank is too low
	}
	else	// same applies to the alien mothership
	{
		if (ship_stock[item].alien_rank_blocked==_always) return true;
		if (ship_stock[item].alien_rank_blocked==_never) return true;
		if (player_rank>ship_stock[item].alien_rank_blocked) return true;
		return false;
	}
}

static bool is_item_allowed_in_earth_mothership(int item, int player_rank)
{
	if( ship_stock[item].earth_rank_blocked == _never ) return false;
	return true;
}

static bool is_item_allowed_in_alien_mothership(int item, int player_rank)
{
	if( ship_stock[item].alien_rank_blocked == _never ) return false;
	return true;		
}


static int get_random_setup_quantity_for_earth_mothership(int item, int player_rank)
{
	if(is_item_allowed_in_earth_mothership(item, player_rank))
	{
		int min = ship_stock[item].earth_min;
		int max = ship_stock[item].earth_max;
		if(min==max) return min; else return RangedRdm(min, max);
	}
	
	return 0;	// if not allowed, return nothing
}

static int get_random_setup_quantity_for_alien_mothership(int item, int player_rank)
{
	if(is_item_allowed_in_alien_mothership(item, player_rank))
	{
		int min = ship_stock[item].alien_min;
		int max = ship_stock[item].alien_max;
		if(min==max) return min; else return RangedRdm(min, max);
	}
	
	return 0;	// if not allowed, return nothing
}


//
//this routine has the tricky job of setting up the stock for a mothership. 
//
void setup_stock(int slot_number)	//of mothership
{
//we only recalculate if this mothership is different to last visited
extern int score,credits,current_level;

extern int in_ramparian_space, gRank;


for (int n=0;n<NUMBER_OF_GOODS;n++)
{
	ship_stock[n].quantity=0;
	if(ship_stock[n].type != n)
	{
		report_error_c("Ships stock is corrupt or is not set up correctly -  ABORTING", "Contact: support@lightsoft.co.uk", n);
	}
}

//if (in_ramparian_space==1) DebugStr("Mothership in RMParian space!");

docked_at_allegiance=(*ocb_ptr).object_list[slot_number].object_category;
//if (old_mothership_stock_slot_number==slot_number) return;
old_mothership_stock_slot_number=slot_number;
setup_commodity_stocks();	//set up the ommodities area
setup_prices(CLEAR_WITHDRAWNS);	//calculate actual kit from basic prices

//mothership_block* ms_data=ocb_ptr->object_list[slot_number].mship_ptr;
//int supply_planet=ms_data->supply_planet_index;
//get its tech rating
float supply_rating;
int mship_supply_type;
//3886
if (training_mission || tutorial)
{
	supply_rating=50; 
	mship_supply_type=9;
}
else	//game
{
	supply_rating=get_supply_rating_docked(slot_number); 
	mship_supply_type=(*ocb_ptr).object_list[slot_number].mship_ptr->supply_planet_index;
	mship_supply_type=GM_ptr->the_quadrant[current_quad].quadrant_planets[mship_supply_type].type;	//<5 agri
}

float tech_rating=mship_supply_type;	//0-10
if (docked_at_allegiance==zex_object)	//Human mothership
{

	for (int n=0;n<NUMBER_OF_GOODS;n++)
	{
		float quantity=get_random_setup_quantity_for_earth_mothership(n, gRank);
		if (ship_stock[n].earth_rank_blocked==_always)	//things we must have
		{
			ship_stock[n].quantity=static_cast<int>(quantity);
		}
		else
		if (ship_stock[n].earth_rank_blocked==_never)	//things we must not have
		{
			ship_stock[n].quantity=0;
		}
		else
		{
		quantity*=(supply_rating/100.0);	//0-100
		quantity*=tech_rating/5;	//0-10, so a 5 gives us 1, 10 gives us 2, 0 gives us 0.
		ship_stock[n].quantity= static_cast<int>(quantity);
		}
	}
	
}	//end of zex category

else	//alien mothership
{
	for (int n=0;n<NUMBER_OF_GOODS;n++)
	{
		float quantity=get_random_setup_quantity_for_alien_mothership(n, gRank);

		if (ship_stock[n].alien_rank_blocked==_always)	//things we must have
		{
			ship_stock[n].quantity=static_cast<int>(quantity);
		}
		else
		if (ship_stock[n].alien_rank_blocked==_never)	//things we must not have
		{
			ship_stock[n].quantity=0;
		}
		else
		{
		quantity*=(supply_rating/100.0);	//0-100
		quantity*=tech_rating/5;	//0-10
		ship_stock[n].quantity= static_cast<int>(quantity);
		}
	}

	//
	// boids are special case we don't cover with out generic table.
	//
	// Disabled by Rob - because boids aren't in the game...
	// 
    //if (current_level>=2 && RangedRdm(1,100)>70)
	//	ship_stock[BOID_ADDITION].quantity = 1;
	
}

}


void setup_prices(int clear_withdrawns)
{

//setup prices
//these are based on a players credit of 5000 max.
//so at 5000 credits prices are most expensive
int i,temp_quantity;
float temp_credits,temp_price,add_price;
float discount,discountc,fprice,fdiff;

temp_credits=credits;
if (temp_credits>50000) temp_credits=50000;
temp_credits/=200;	//between 0 and 200 - this is a percentage value
//go through array filling actual price
for (i=0;i<NUMBER_OF_GOODS;i++)
  {
	   temp_price=ship_stock[i].basic_price;
	//   add_price=temp_price*(temp_credits/100);

	   ship_stock[i].actual_price=static_cast<int>(temp_price);
	//calc discount
	temp_quantity=ship_stock[i].quantity;

	if (i==FUEL) temp_quantity/=10;

	if (temp_quantity<2) discount=0.0;
	else
	if (temp_quantity<3) discount=.03;
	else
	if (temp_quantity<4) discount=.04;
	else
	if (temp_quantity<5) discount=.05;
	else
	if (temp_quantity<6) discount=.07;
	else
	if (temp_quantity<8) discount=.10;
	else
	if (temp_quantity<10) discount=.20;
	else
	if (temp_quantity<16) discount=.25;
	else
	if (temp_quantity<31) discount=.35;
	else
	if (temp_quantity<61) discount=.45;
	else
	if (temp_quantity<201) discount=.50;
	else
	if (temp_quantity<401) discount=.55;
	else
	if (temp_quantity<701) discount=.60;
	else
	if (temp_quantity>=701) discount=.65;

	//difficulty=16 for easy, 8 for medium and 1 for hard
	fdiff=difficulty;
	fdiff/=100;	//percentage deduction

	fprice=ship_stock[i].actual_price;

	discountc=fprice*(discount+fdiff);	//proces go up as difficulty increases

	fprice-=discountc;
	ship_stock[i].actual_price=static_cast<int>(fprice);

    if (clear_withdrawns==CLEAR_WITHDRAWNS) 
	{ 
		ship_stock[i].temp_removed=0; 
	}
  }

}



//Loads up the ships stock strings from resource STR#128
void load_up_stock_strings()
{
extern Str255 ship_items_strings[35];	//read from str resource 130

//Str255 short_string;
Str255 long_string;
unsigned char cstring[256];

int i,j,k;

for (i=1;i<NUMBER_OF_GOODS+1; i++)
 {
  GetIndString((unsigned char*)&ship_stock[i-1].short_description,128,i);
  GetIndString((unsigned char*)&ship_stock[i-1].long_description,129,i);
//  GetIndString((unsigned char*)&ship_items_strings[i-1],130,i);	//in inventory
   
 }	//end of i


for (i=1;i<KLAST+1; i++)
 {
  GetIndString((unsigned char*)&ship_items_strings[i-1],130,i);	//in inventory
   
 }	//end of i


}

unsigned int other_buyers_timer;
unsigned int credits_timer;

//From 0.88 onwards we have simulated other buyers/sellers
void reset_other_users()
{
other_buyers_timer=LS_TickCount60()+120;
credits_timer=LS_TickCount60()+10*60;	//we dock 2 credits every ten seconds
how_many_users=RangedRdm(1,6);	//how many stocks to buy and sell

}


void run_other_users(int from_class)	//needs obj_class to redisplay
{
int buy_what,sell_what;

user_delay(1);	//Some time for the system so we don't hog the processor

if (LS_TickCount60()>credits_timer)
{
int creds_dec;

     if (ship_items[KTRADING_COMPUTER].status==1)
     creds_dec=3;
     else
     creds_dec=5;
     
credits_timer=LS_TickCount60()+10*60;
if (credits>creds_dec) credits-=creds_dec;
else credits=0;
}


if (LS_TickCount60()<other_buyers_timer) return;
other_buyers_timer=LS_TickCount60()+(60*RangedRdm(1,10));
//now we can run
//buy something?

buy_what=RangedRdm(0,NUMBER_OF_GOODS-1);

if (buy_what!=FUEL)
{
   ship_stock[buy_what].quantity--;
}
else
   ship_stock[buy_what].quantity-=RangedRdm(30,200);

if (ship_stock[buy_what].quantity<0) ship_stock[buy_what].quantity=0;



//sell something?
sell_what=RangedRdm(0,NUMBER_OF_GOODS-1);

extern int gRank;
// - enable simulated other users to sell stuff
bool allowed;

if(docked_at_allegiance==zex_object)	//Human mothership
{
	allowed = is_item_allowed_in_earth_mothership(sell_what, gRank);
}
else {
	allowed = is_item_allowed_in_alien_mothership(sell_what, gRank);
}

if(allowed)		// only rank allowed items

//check the seller can sell this item on this mothership (tasers for instance are not allowed)
{
	if (sell_what!=FUEL)
	{
		if (ship_stock[sell_what].quantity<10)
			ship_stock[sell_what].quantity++;
	} 
	else
		ship_stock[sell_what].quantity+=RangedRdm(30,200);
}

//reshow stock
	setup_prices(DO_NOT_CLEAR_WITHDRAWNS);
      display_goods(from_class);	//redraw and update display
    Show_it();



}


// -----------COMMODITIES PROCESSING ----------
hold_desc commodity_array[NUMBER_OF_COMMODITIES];


//simulate buyrs and sellers
void run_commodities()
{
int buy_what,sell_what;
int max,base_price,i,how_many,temp,next_run_time;

if (LS_TickCount60()>credits_timer)
{
int creds_dec;
     if (ship_items[KTRADING_COMPUTER].status==1)
     creds_dec=3;
     else
     creds_dec=5;
     
credits_timer=LS_TickCount60()+10*60;
if (credits>creds_dec) credits-=creds_dec;
else credits=0;
}



if (LS_TickCount60()<other_buyers_timer) return;
next_run_time=15-how_many_users;
if (next_run_time<1) next_run_time=1;
other_buyers_timer=LS_TickCount60()+(20*next_run_time);

temp=(RangedRdm(1,100));

     if (temp>70)
    {
    how_many_users++;
    }
    else
    if (temp>40)
    {
    how_many_users--;
    }
    else {};
 if (how_many_users<0) how_many_users=0;


 if (difficulty==8) { if (how_many_users<5) how_many_users=5; }

 if (difficulty==1) { if (how_many_users<11) how_many_users=11; }
    
if (how_many_users==0) return;	//"oops"

for (i=0;i<2;i++)	
{
//now we can run
//buy something?
buy_what=RangedRdm(0,NUMBER_OF_COMMODITIES);

//calc how many
base_price=commodity_array[buy_what].base_price_human;
max=5;	//in case I bugger it up...
//rarity based on price
if (base_price<11) max=200;
else
if (base_price<21) max=100;
else
if (base_price<31) max=80;
else
if (base_price<41) max=60;
else
if (base_price<50) max=40;
else
if (base_price<80) max=30;
else
if (base_price<100) max=20;
else
if (base_price<150) max=18;
else
if (base_price<200) max=10;
else
if (base_price<300) max=5;
else
if (base_price<500) max=2;
else
if (base_price>=500) max=1;



     commodity_array[buy_what].stock-=RangedRdm(1,max);

if (commodity_array[buy_what].stock<0) commodity_array[buy_what].stock=0;

//sell something?


 sell_what=RangedRdm(0,NUMBER_OF_COMMODITIES);
base_price=commodity_array[sell_what].base_price_human;
max=5;	//in case I bugger it up...
//rarity based on price
if (base_price<11) max=200;
else
if (base_price<21) max=100;
else
if (base_price<31) max=80;
else
if (base_price<41) max=60;
else
if (base_price<50) max=40;
else
if (base_price<80) max=30;
else
if (base_price<100) max=20;
else
if (base_price<150) max=18;
else
if (base_price<200) max=10;
else
if (base_price<300) max=5;
else
if (base_price<500) max=2;
else
if (base_price>=500) max=1;


     commodity_array[sell_what].stock+=RangedRdm(1,max);	//sell max 50%
}	//end of for how_many

setup_commodity_stock_prices();

}




extern hold_desc cargos_array[NUMBER_OF_COMMODITIES];

//draws stock with buy and sell prices
//25 lines per page, sorted by category
void display_commodities(LSRAW * exit_button)
{
extern int credits;

int display_item_array_index,ship_stock_index,x;
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
unsigned char number_string[12];
int i,wait,quit,button_state;
LSRAW *bottom_bar;
Handle bottom_bar_H;
LSRAW *bottom_bar1;
Handle bottom_bar1_H;
LSRAW *fwd;
Handle fwd_H;
LSRAW *back;
Handle back_H;
int item_selected;
Point mouse_xy;
int display_base_index;
real_time_struct time_struct;
int trading_open;

while (LS_Left_Button()==1)  { SDL_Delay(10); Show_it(); }	//standard // give computer back time and allow mouse to move (if on screen)

 update_real_time();
 get_real_time_struct(&time_struct);
 //trade between 0800 and 1800 hours only
 if (time_struct.hours>18 || time_struct.hours<8) trading_open=0;
 else trading_open=1;
 

	  if (ship_items[KTRADING_COMPUTER].status==1)
      {
      LSRAW *dialog_picture;
      Handle dialog_H;
      
           the_drawing_buffer=Get_back_buffer();

        dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;


      print_crunched_pict(dialog_picture,180,130);

      TextFont(21);
      TextSize(14);
      TextFace(bold);
      RGBForeColor(&rgbBlue);
//do text
      MoveTo (200,170);
      DrawString_c("Your Trading Computer is reducing ");
      MoveTo (200,190);
      DrawString_c("on-line charges to 18Cr per minute.");
      LS_DisposeHandle(dialog_H);
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
         Show_it();
     wait_for_mouse();

      }

         bottom_bar_H=GetZexPicture ('RCZ ',3400);	//computer   
	LS_HLock(bottom_bar_H);
        bottom_bar = (LSRAW*)*bottom_bar_H;
         bottom_bar1_H=GetZexPicture ('RCZ ',3401);	//computer   
	LS_HLock(bottom_bar1_H);
        bottom_bar1 = (LSRAW*)*bottom_bar1_H;

        fwd_H=GetZexPicture ('RCZ ',3402);  
	LS_HLock(fwd_H);
        fwd = (LSRAW*)*fwd_H;
        
        back_H=GetZexPicture ('RCZ ',3403);  
	LS_HLock(back_H);
        back = (LSRAW*)*back_H;
        

display_base_index=0;
//init_show_stock();	//reset prinitng coords
//     setup_commodity_stocks();
     while(LS_Left_Button()==1)  { SDL_Delay(10); Show_it(); } // give computer back time and allow mouse to move (if on screen)
     
     the_drawing_buffer=Get_back_buffer();
	RGBBackColor (&rgbBlack);
item_selected=-1;

quit=0;

while(quit==0)	//from exit button
{
//find out where mouse was when down
     wait=0;

//the loop     
 while (wait==0)	//while mouse isn't pressed
     {
     int can_buy;
     the_drawing_buffer=Get_back_buffer();
     //oldmac//GetPort (&savePort);
	//oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
      user_delay(1);	//Some time for the system so we don't hog the processor
      if (trading_open==1)
      run_commodities();	//others buy/sell!
      
      erase_zex_rect_colour(640,480,242);	//in pictures
	  set_poly_clipping(0,639,0,479);	//set poly clip rect - rp 01-08-2005

      draw_lines(trading_open);	//light/dark blue
      
      if (trading_open==1)
      draw_commodity_strings(display_base_index,item_selected);	//draw the strings from index 0
      else
      {
      RGBForeColor (&rgbWhite);
      MoveTo(220,197);
      DrawString_c("TradeNet available between 08:00 and 18:00 hours.");
      MoveTo(260,219);
      DrawString_c("Please come back in the morning.");

      }
//if something is selected then show buy else buy blanked out
     if (item_selected==-1) can_buy=0;
     else can_buy=1;
     
     if (trading_open==0) can_buy=0;
     
     draw_bottom_bar(bottom_bar1,1,can_buy,0,1);
//draw the little fwd and back arrows
      if (trading_open==1)
      {
       if (display_base_index>0)
       print_crunched_trans_pict(back,602,412);
       if (display_base_index<NUMBER_OF_COMMODITIES-36)
       print_crunched_trans_pict(fwd,620,412);
      }
      
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
      Show_it();
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
      if (credits==0) 
	  {
		show_logout();
		break;
	  }

//wait for mouse down/up
     button_state=LS_Left_Button();
     if (button_state==1) wait=1;
     }
    //button up

    wait=0;
    while (wait==0)
    {
     button_state=LS_Left_Button();
     if (button_state==0) wait=1;
	 Show_it();
	 user_delay(1);
    }
   
//ok, mouse has gone down and up, decode it
      if (credits==0) 
	  {
		show_logout();
		break;
	  }
	 ZexGetMouse (&mouse_xy);
     
     if (mouse_xy.v<430)
     {
     //select an item or fwd back
       if (mouse_xy.v<411)
          {
          int pos_y,old_item_selected;
           //select an item
           //convert to line number
           pos_y=mouse_xy.v;
           pos_y-=12;	//title
           pos_y=pos_y/11;
           old_item_selected=item_selected;
           item_selected=display_base_index+pos_y;
           if (old_item_selected==item_selected) item_selected=-1;	//toggle
          }
       else
         {
         //fwd back
         if (mouse_xy.v<428)
           {
            if (mouse_xy.h>602 && mouse_xy.h<618 && display_base_index>0)
                  {
                  item_selected=-1;
                  display_base_index-=36;
                  if (display_base_index<0) display_base_index=0;
                  }
            else
            if (mouse_xy.h>620 && mouse_xy.h<636 && display_base_index<NUMBER_OF_COMMODITIES-36)
                  {
                  item_selected=-1;
                  display_base_index+=36;
                  if (display_base_index>NUMBER_OF_COMMODITIES-36) display_base_index=NUMBER_OF_COMMODITIES-36;
                  
                  }
           }
         }
     }
     else	//bottom bar
     {
     if (mouse_xy.h>283 && mouse_xy.h<358) quit=1;
//     else
//     if (trading_open==0) quit=1;
     else
     if (mouse_xy.h>529 && mouse_xy.h<630) draw_hold_items_docked(bottom_bar1,trading_open);	//inventory. returns on exit
     else
     if (mouse_xy.h>360 && mouse_xy.h<438 && item_selected!=-1 && trading_open==1) buy_cargo(item_selected);	   //inventory. returns on exit
     
     }
 
}
LS_DisposeHandle(bottom_bar_H);
LS_DisposeHandle(bottom_bar1_H);
LS_DisposeHandle(back_H);
LS_DisposeHandle(fwd_H);
}

void draw_bottom_bar(LSRAW*bar_pict,int can_exit, int can_buy, int can_sell, int can_inv)
{
unsigned char number_string[12];
//static RGBColor	rgbDarkBlue = {0,0,0x3333};
RGBColor	rgbDarkRed = {0x3333,0,0};

      print_crunched_pict(bar_pict,0,430);
      
//show credits in bottom of screen
      TextFont(21);
      TextSize(14);
      TextFace(bold);
      RGBForeColor(&rgbBlue);
      MoveTo (10,454);
      DrawString_c("Available credit:");

       ZexNumToString(credits,number_string);
       MoveTo (130,454);
       DrawString(number_string);
//draw exit
       if (can_exit==1)
       RGBForeColor(&rgbRed);
       else
       RGBForeColor(&rgbDarkRed);
       
       MoveTo(305,455);
       DrawString_c("Exit");
       
//draw buy
       if (can_buy==1)
       RGBForeColor(&rgbBlue);
       else
       RGBForeColor(&rgbDarkBlue);
       MoveTo(390,455);
       DrawString_c("Buy");

//draw sell
       if (can_sell==1)
       RGBForeColor(&rgbBlue);
       else
       RGBForeColor(&rgbDarkBlue);
       MoveTo(470,455);
       DrawString_c("Sell");
       
//draw inventory
       if (can_inv==1)
       RGBForeColor(&rgbBlue);
       else
       RGBForeColor(&rgbDarkBlue);
       MoveTo(570,455);
       DrawString_c("Sell");
       
       
       TextFace(0);

}

void draw_lines(int trading_open)
{
Rect temp_rect={12,0,23,639};
int i;
unsigned char number_string[12];
RGBColor LightBlue = {0,0,0x7fff};
RGBColor DarkBlue = {0,0,0xafff};
TextFont(21);
TextSize(9);
TextFace(bold);
RGBForeColor(&rgbLightBlue);

MoveTo(2,10);
DrawString_c("Description");

MoveTo(240,10);
DrawString_c("DofQ");

MoveTo(300,10);
DrawString_c ("Category");

MoveTo(400,10);
DrawString_c ("Current Stock");

MoveTo(480,10);
DrawString_c("We Buy At");

MoveTo(560,10);
DrawString_c("We Sell At");

MoveTo(0,11);
LineTo (640,11);
for (i=0;i<36;i++)
  {
  int colour;
//  if (i&1==1) RGBForeColor(&LightBlue);
//   else RGBForeColor(&DarkBlue);
  if ((i&1)==1) colour=239;
   else colour=204;
//  PaintRect(&temp_rect);
  dlp_draw_box(temp_rect.top,temp_rect.left,temp_rect.bottom,temp_rect.right,colour);
  temp_rect.top+=11; temp_rect.bottom+=11;
  }
//draw vert lines
RGBForeColor(&rgbWhite);

MoveTo(238,0);
LineTo(238,408);
MoveTo(298,0);	//cat
LineTo(298,408);

MoveTo(398,0);
LineTo(398,408);

MoveTo(478,0);
LineTo(478,408);

MoveTo(558,0);
LineTo(558,408);
//bottom H line
MoveTo(0,408);
LineTo(639,408);
MoveTo(0,0);
LineTo(639,0);
MoveTo(0,0);
LineTo(0,408);
MoveTo(639,0);
LineTo(639,408);

//show how many users
TextFont(21);
TextSize(9);
TextFace(0);

if (trading_open==1)
{
RGBForeColor(&rgbDarkLightBlue);
MoveTo(2,420);
if (training_mission!=0 || tutorial==1)
DrawString_c("Current simulated system load: ");
else
DrawString_c("Current system load: ");
     ZexNumToString(how_many_users+1,number_string);
     DrawString(number_string);
     if (how_many_users+1>1)
     DrawString_c (" users.");
     else
     DrawString_c (" user.");
}

      
TextFace(0);

}



void draw_commodity_strings(int from_index,int selected_item)	//draw 35 lines from index
{
Rect temp_rect={12,0,23,640};
int i,py;
unsigned char temp_string[100];
int temp;
unsigned char number_string[12];

TextFont(21);
TextSize(9);
TextFace(0);
RGBForeColor(&rgbLightBlue);
py=21;
if (training_mission!=0 || tutorial==1)
   {
    MoveTo(245,420);
    RGBForeColor(&rgbRed);
    DrawString_c("*Simulated stock for training purposes*");
   }
   
for (i=from_index;i<from_index+36;i++)
  {
  if (i==NUMBER_OF_COMMODITIES-1) break;	//don't do an "oops"
  
  if (i==selected_item) TextMode(4);	//highlight selected line
  else TextMode(1);	
  
  if (commodity_array[i].stock<1)
    RGBForeColor(&rgbRed);
  else
  if (commodity_array[i].stock<4)
    RGBForeColor(&rgbOrange);
  else
  if (commodity_array[i].stock>150)
    RGBForeColor(&rgbYellow);
  else
    RGBForeColor(&rgbLightBlue);
    
  
  MoveTo(6,py);
  CToPascal(commodity_array[i].string,temp_string);

  DrawString((unsigned char const*) temp_string);  
  
  if ((commodity_array[i].MFR & 8) ==8)
  DrawString_c (" (Illegal)");
  
//dofq
  MoveTo(242,py);
  temp=  commodity_array[i].dofq;
  if (temp==1)
  DrawString_c("Tonne");  
  else
  if (temp==2)
  DrawString_c("Barrel");  
  else
  if (temp==3)
  DrawString_c("Case");  
  else
  if (temp==4)
  DrawString_c("Kilogramme");  


//category
  MoveTo(302,py);
  temp=  commodity_array[i].obj_class;
  if (temp==10)
  DrawString_c("Materials");  
  else
  if (temp==11)
  DrawString_c("Foodstuffs");  
  else
  if (temp==12)
  DrawString_c("Tech.");  
  else
  if (temp==13)
  DrawString_c("Luxury/Textiles");  
  else
  if (temp==14)
  DrawString_c("Firearms");  
  else
  if (temp==15)
  DrawString_c("Drinks");  
  else
  if (temp==16)
  DrawString_c("Livestock");  
//stock
  MoveTo(402,py);
     ZexNumToString(commodity_array[i].stock,number_string);
     DrawString(number_string);
     if (commodity_array[i].stock!=0)
     
    {
     if (commodity_array[i].stock==1)
     {
     if (commodity_array[i].dofq==1) DrawString_c (" tonne");
     else
     if (commodity_array[i].dofq==2) DrawString_c (" barrel");
     else
     if (commodity_array[i].dofq==3) DrawString_c (" case");
     else
     if (commodity_array[i].dofq==4) DrawString_c (" kilogramme");
     }

     else //more than 1
     
     {
     if (commodity_array[i].dofq==1) DrawString_c (" tonnes");
     else
     if (commodity_array[i].dofq==2) DrawString_c (" barrels");
     else
     if (commodity_array[i].dofq==3) DrawString_c (" cases");
     else
     if (commodity_array[i].dofq==4) DrawString_c (" kilogrammes");
     }
    }
//buy price
  MoveTo(482,py);
     ZexNumToString(commodity_array[i].buy_price,number_string);
     DrawString(number_string);

//sell price
  MoveTo(562,py);
  if (commodity_array[i].stock==0)
     DrawString_c("-");
     else
     {
     ZexNumToString(commodity_array[i].sell_price,number_string);
     DrawString(number_string);
     }
     
  py+=11;

  }
//draw vert lines
RGBForeColor(&rgbWhite);
TextMode(1);

}




//This called just once when we dock

void setup_commodity_stocks()
{
int i,manufacturer,supply_rating;

init_zex_random();	//good time to do this...

//copy the base stock strings to commodity_array
     for (i=0;i<NUMBER_OF_COMMODITIES;i++)
     commodity_array[i]=cargos_array[i];	//who sed C wasn't a high level language???

supply_rating=mship_supply_rating;
supply_rating/=10;
if (supply_rating<1) supply_rating=1;

for (i=0;i<NUMBER_OF_COMMODITIES;i++)
{
 int max,base_price;
 base_price=commodity_array[i].base_price_human;
 max=5;	//in case I bugger it up...
 //rarity based on price
 if (base_price<11*COMMODITIES_MULTIPLIER) max=40*supply_rating;
 else
 if (base_price<21*COMMODITIES_MULTIPLIER) max=30*supply_rating;
 else
 if (base_price<31*COMMODITIES_MULTIPLIER) max=20*supply_rating;
 else
 if (base_price<41*COMMODITIES_MULTIPLIER) max=15*supply_rating;
 else
 if (base_price<50*COMMODITIES_MULTIPLIER) max=12*supply_rating;
 else
 if (base_price<80*COMMODITIES_MULTIPLIER) max=9*supply_rating;
 else
 if (base_price<100*COMMODITIES_MULTIPLIER) max=7*supply_rating;
 else
 if (base_price<150*COMMODITIES_MULTIPLIER) max=5*supply_rating;
 else
 if (base_price<200*COMMODITIES_MULTIPLIER) max=4*supply_rating;
 else
 if (base_price<300*COMMODITIES_MULTIPLIER) max=3*supply_rating;
 else
 if (base_price<500*COMMODITIES_MULTIPLIER) max=2*supply_rating;
 else
 if (base_price>=500*COMMODITIES_MULTIPLIER) max=1*supply_rating;
 
 
 if (max!=0)
 commodity_array[i].stock=RangedRdm(0,max);
 else
 commodity_array[i].stock=0;
 
 manufacturer=(commodity_array[i].MFR & 3);
 
 //if it's illegal (bit 3 set) then the chances are there won't be any
 if ((commodity_array[i].MFR & 8)==8)	  //illegal
 {
   if (docked_at_allegiance==0)	//earth mship
   {
   if (RangedRdm(0,100)<80)
     commodity_array[i].stock=0;
   }
   else	//alien mship
   if (RangedRdm(0,100)<20)
     commodity_array[i].stock=0;
  
}

if (manufacturer-1==docked_at_allegiance)
    {
     float fstock;
     //add on 30%
     fstock=commodity_array[i].stock;
     fstock+=fstock*0.3;
     commodity_array[i].stock=static_cast<short int>(fstock);
    }
    else	//subtract 20%
    {
     float fstock;
     fstock=commodity_array[i].stock;
     fstock-=fstock*0.2;
     if (fstock<0) fstock=0;
     commodity_array[i].stock=static_cast<short int>(fstock);
    
    }
}
//calc buy and sell price
setup_commodity_stock_prices();

}


void setup_commodity_stock_prices()
{
int i,temp_quantity;
float basic_price,discount,buy_price,sell_price,cut;
float bulk_discount,fmarkup,supply_rating_multiplier;
int manufacturer;


supply_rating_multiplier=(100-mship_supply_rating)/33+1;
supply_rating_multiplier/=2;	//range 0.5 to 2

if (supply_rating_multiplier<=0.5) supply_rating_multiplier=0.5;	//just in case i've miscalculated
if (supply_rating_multiplier>=2) supply_rating_multiplier=2.0;	//just in case i've miscalculated

for (i=0;i<NUMBER_OF_COMMODITIES;i++)
  {
    if (docked_at_allegiance==zex_object)	//earth mothership
    basic_price=commodity_array[i].base_price_human;
    else
    basic_price=commodity_array[i].base_price_alien;
if (difficulty==8) basic_price+=basic_price*0.1;
else
if (difficulty==1) basic_price+=basic_price*0.3;

//do supply rating
//if supply rating is 100, we want a multiplier of 0.5
//if supply rating is 0 we want a multiplier of 2

basic_price*=supply_rating_multiplier;
//do planet type adjustment
//is this an agri or tech stock or neither?
//1 to 5 is agri, 6 to 10 is tech
if(commodity_array[i].obj_class==ORES || commodity_array[i].obj_class==FOOD
   || commodity_array[i].obj_class==DRINKS || commodity_array[i].obj_class==LIVESTOCK)
   {
   if (mship_supply_type<6) basic_price-=basic_price*(mship_supply_type)/20;
   }
   else	//tech item
   {
   if (mship_supply_type>=6) basic_price-=basic_price*(mship_supply_type-5)/20;
   }
   
//calc bulk_discount which ups the buy and drops the sell price
temp_quantity=commodity_array[i].stock;
if (temp_quantity<10) bulk_discount=-.09;	//up the price!
else
if (temp_quantity<20) bulk_discount=.05;
else
if (temp_quantity<31) bulk_discount=.07;
else
if (temp_quantity<61) bulk_discount=.09;
else
if (temp_quantity<101) bulk_discount=.11;
else
if (temp_quantity<141) bulk_discount=.13;
else
if (temp_quantity<200) bulk_discount=.15;
else
if (temp_quantity>=200) bulk_discount=.20;

//if manufacturer is same as this allegiance then dec price by 10%

manufacturer=commodity_array[i].MFR & 3;

    if (manufacturer!=3) //made in both galaxies
     {
     if (commodity_array[i].MFR-1==docked_at_allegiance)	//1 is earth, 2 is alien, in code 0 is earth, 1 is alien
          basic_price-=basic_price*0.1;
     }
//    cut=basic_price*0.20;
    fmarkup=commodity_array[i].markup;
    fmarkup/=100;
    cut=basic_price*fmarkup;
    
    bulk_discount=basic_price*bulk_discount;
    
    buy_price=(basic_price-cut)-bulk_discount;
    sell_price=(basic_price+cut)-bulk_discount;
    
    if (buy_price==sell_price) sell_price+=1;	//will happen
    commodity_array[i].buy_price=static_cast<short int>(buy_price);
    commodity_array[i].sell_price=static_cast<short int>(sell_price);

    
  }
}


extern hold_desc hold_items[MAX_HOLD_ITEMS];

//show items in hold and trailer (if fitted)
void draw_hold_items_docked(LSRAW* bar_pict, int trading_open)
{
extern int score,credits,start_time,accumulated_game_time;
extern int jettison_timer;
LS_CGrafPtr the_drawing_buffer;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
int i;
unsigned char number_string[12];
int alive_time,hours,mins,secs,stop_time;
char number_string1[12];
Point mouse_xy;
int quit,wait,button_state;
int item_selected;

item_selected=-1;
quit=0;
while (LS_Left_Button()==1)  { SDL_Delay(10); Show_it(); } // give computer back time and allow mouse to move (if on screen)
bool sell_all=false;
while(quit==0)
{
//loop
  wait=0;
  while(wait==0)	//wait for mouse to go down or key 's' or 'a'.
  {
     the_drawing_buffer=Get_back_buffer();

	cls();	//I'm almost temped to put set_poly_clipping(0,639,0,479) in here, but that would be bad. 
	set_poly_clipping(0,639,0,479);	//set poly clip rect

	draw_lines(trading_open);
	draw_inventory_strings(0,item_selected);
	if (item_selected==-1 || trading_open==0)
        draw_bottom_bar(bar_pict,1,0,0,0);
	else
        draw_bottom_bar(bar_pict,1,0,1,0);	//enable sell
        
        if (trading_open==1)
        run_commodities();	  //others buy/sell!
		
		//find out if we have more than 1 item
		bool have_cargo=false;
		for (int n=0;n<4;n++)
		{
				if (hold_items[n].id!=-1) have_cargo=true;
		}
		
		if (have_cargo)
		{
			MoveTo(270,420);
			TextFont(21);
			TextSize(9);
			TextFace(0);

			RGBForeColor(&rgbDarkLightBlue);
			DrawString_c("Press 'A' to sell all");
		}
		
        Show_it();
		SDL_Delay(10);
     button_state=LS_Left_Button();
     if (button_state==1) wait=1;
	 if (IsKeyPressed(SDLK_a)) {sell_all=true; wait=1; }
     }
	 
    //button up

    if (sell_all==false) wait=0;
    while (wait==0)
    {
     button_state=LS_Left_Button();
     if (button_state==0) wait=1;
	 Show_it();
	 user_delay(1);
    }

if ((sell_all==true) && trading_open)
{
	for (int n=0;n<4;n++)
	{
		if (hold_items[n].id!=-1) sell_cargo(n);
	}
	sell_all=false;
}
else
{
			ZexGetMouse (&mouse_xy);
		 
	//	wait_for_mouse();	//wait for mouse button

		 if (mouse_xy.v<430 && trading_open==1)
		 {
		 //select an item or fwd back
		   if (mouse_xy.v<411)
			  {
			  int pos_y,old_item_selected;
			   //select an item
			   //convert to line number
			   pos_y=mouse_xy.v;
			   pos_y-=12;	//title
			   pos_y=pos_y/11;
			   old_item_selected=item_selected;
			   item_selected=pos_y;
			   if(item_selected > 3 || item_selected < 0) 
			   {
				   item_selected=-1;
			   }
			   else	//don't index using item_selected if it's -1
			   {
				if (hold_items[item_selected].id==-1) item_selected=-1;	//nothing there
			   }
			   
			   if (old_item_selected==item_selected) item_selected=-1;	//toggle
			  }

		 }
		 else	//mouse on bar
		 {
		 if (mouse_xy.h>283 && mouse_xy.h<358) 
		   {
		   item_selected=-1;
		   quit=1;
		   }
		 else
		 if (mouse_xy.h>440 && mouse_xy.h<525 && item_selected!=-1) 
		   {
		   //sell
		   sell_cargo(item_selected);
		   item_selected=-1;
		   }
		 
		 }
	}	//end of not sell all
}	//end of while quit


while (LS_Left_Button()==1)  { SDL_Delay(10); Show_it(); } // give computer back time and allow mouse to move (if on screen)

}





void draw_inventory_strings(int from_index, int selected_item)
{
Rect temp_rect={12,0,23,640};
int i,py;
unsigned char temp_string[100];
int temp,hold_slot;
unsigned char number_string[12];

TextFont(21);
TextSize(9);
TextFace(0);
RGBForeColor(&rgbLightBlue);

py=21;

for (hold_slot=from_index;hold_slot<4;hold_slot++)
  {
//  if (i==NUMBER_OF_COMMODITIES-1) break;	  //don't do an "oops"
  i=hold_items[hold_slot].id;	//get id of item at this slot
  

  if (hold_slot==selected_item) TextMode(4);	//highlight selected line
  else TextMode(1);	
  
  MoveTo(6,py);
  
  if (i!=-1)
  {
  CToPascal(cargos_array[i].string,temp_string);

  DrawString((unsigned char const*) temp_string);  
  }
  else
  DrawString_c("****Empty****");  

  if (i!=-1)
  {  
//dofq
  MoveTo(242,py);
  temp=  cargos_array[i].dofq;
  if (temp==1)
  DrawString_c("Tonne");  
  else
  if (temp==2)
  DrawString_c("Barrel");  
  else
  if (temp==3)
  DrawString_c("Case");  
  else
  if (temp==4)
  DrawString_c("Kilogramme");  


//category
  MoveTo(302,py);
  temp=  cargos_array[i].obj_class;
  if (temp==10)
  DrawString_c("Materials");  
  else
  if (temp==11)
  DrawString_c("Foodstuffs");  
  else
  if (temp==12)
  DrawString_c("Tech.");  
  else
  if (temp==13)
  DrawString_c("Luxury/Textiles");  
  else
  if (temp==14)
  DrawString_c("Firearms");  
  else
  if (temp==15)
  DrawString_c("Drinks");  
  else
  if (temp==16)
  DrawString_c("Livestock");  
//stock
  MoveTo(400,py);
     ZexNumToString(commodity_array[i].stock,number_string);
     DrawString(number_string);

//buy price
  MoveTo(482,py);
     ZexNumToString(commodity_array[i].buy_price,number_string);
     DrawString(number_string);

//sell price
  MoveTo(562,py);
     ZexNumToString(commodity_array[i].sell_price,number_string);
     DrawString(number_string);
  }	//end of not -1
  py+=11;
    }	  //end of for
//draw vert lines
RGBForeColor(&rgbWhite);
TextMode(1);

}


//this is (basically) a paste of do_yer_actual_sell
//time, time, time...
void sell_cargo(int item_selected)
{
extern int credits;
extern int hull_strength;
//oldmac//extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
LS_CGrafPtr the_drawing_buffer;
unsigned char number_string[12];
unsigned char temp_string[300];	//how long is a piece of string????
Point mouse_xy;
int buyers_price,needed_fuel;
int tc_tries,tc_max_tries;
int commodity_id;

//translate ship item to stock item
commodity_id=hold_items[item_selected].id;

//stock_item_id=translate_ship_to_stock(ship_item_id);

if (commodity_id==-1)             report_error_c("sell_cargo translate error. Pooh!","",commodity_id);

//calc tc_max_tries
tc_tries=0;
tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
     
rehaggle:	//err, a label!
     the_drawing_buffer=Get_back_buffer();

     //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
// 	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

//     if (ship_items[KTRADING_COMPUTER].status==1)
//     dialog_H=GetZexPicture ('RCZ ',2011);	    //haggle dialog box
//     else
//     dialog_H=GetZexPicture ('RCZ ',2012);	  //confirm dialog box
//        
//	  LS_HLock(dialog_H);
//	  dialog_picture = (LSRAW*)*dialog_H;
//
////accept price or negatiate?
//
////    if (LS_Left_Button()==1);	//wait for mouse up
//    
////    display_goods(screen_class);
//
//	  print_crunched_pict(dialog_picture,180,130);
//    TextFont(21);
//TextSize(12);
//TextFace(normal);
//RGBForeColor(&rgbBlue);
//     MoveTo(426,150);
//     ZexNumToString(commodity_array[commodity_id].buy_price,number_string);
//     DrawString("Cr ");
//     DrawString(number_string);
//     CToPascal(commodity_array[commodity_id].string,temp_string);
//     MoveTo(200,150);
//     DrawString((unsigned char *) temp_string);
////if we have a trading computer calculate price
////     if (ship_items[KTRADING_COMPUTER].status==1)
////     {
////     float buying_price,percent;
////     buying_price=commodity_array[commodity_id].buy_price;
////
////     percent=buying_price/RangedRdm(30,60);	//between 40 and 60% off
////     buying_price=buying_price-percent;
////     
////     buyers_price=do_trading_computer(buying_price, stock_item_id);
////
////     MoveTo (200,220);
////	     DrawString ("Trading Computer gets a price of ");
////	     ZexNumToString(buyers_price,number_string);
////     DrawString(number_string);
////     }
////     else	  //no trading computer
//     {
//      float temp_price,fdiscount;
       buyers_price=commodity_array[commodity_id].buy_price;
////      temp_price=buyers_price;
////      fdiscount=.40;
////      temp_price=temp_price-(temp_price*fdiscount);
////      if (temp_price<1) temp_price=1;	//dont want credits!     
////      buyers_price=temp_price;
//     }
//     
//
//     MoveTo (200,240);
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     DrawString("Accept the offer, re-negotiate or cancel?");
//     else
//     {
//     DrawString("Accept the offer of ");
//	   ZexNumToString(buyers_price,number_string);
//     DrawString(number_string);
//     DrawString(" credits or cancel?");
//     
//     }
// 	SetGDevice(saveGDH);	//these are here so I can SEE them!
//	  SetPort(savePort);
//   Show_it();
//   LS_DisposeHandle(dialog_H);
//     wait_for_mouse();
//
////bottom 20, right 30 is exit
//     ZexGetMouse (&mouse_xy);
////     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
// 
////have a trading computer?
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     {
//     if (mouse_xy.h>380 && mouse_xy.h<480 && mouse_xy.v>250 && mouse_xy.v<280) return;	//cancel
//     if (mouse_xy.h>280 && mouse_xy.h<380 && mouse_xy.v>250 && mouse_xy.v<280) 
//        {
//          //check if exceeeded number of tries
//          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
//          goto rehaggle;	  //recalc
//        }
//     }
//     
//     else
//     if (mouse_xy.h>320 && mouse_xy.h<480 && mouse_xy.v>256 && mouse_xy.v<278) return;	//cancel
//     if (mouse_xy.h>180 && mouse_xy.h<320 && mouse_xy.v>256 && mouse_xy.v<278)
//     goto buy_it;
//     else
//     {
//        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
//        goto rehaggle;	//do nothing
//     }
////must be OK!

   
buy_it:   dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor

//show you have bought
       //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180,130);

      TextFont(21);
TextSize(14);
TextFace(bold);
RGBForeColor(&rgbBlue);
//do text
     MoveTo (200,170);
     
      {
     DrawString_c("You have sold ");
 	   DrawString_c("the");
       MoveTo (200,188);
     CToPascal(commodity_array[commodity_id].string,temp_string);
     DrawString ((unsigned char const *) temp_string);	//cast this, cast that...
     DrawString_c (" for ");
     ZexNumToString(buyers_price, number_string);
     MoveTo (200,206);
     DrawString(number_string);
     DrawString_c (" credits.");
      }
    
     credits=credits+buyers_price;
     commodity_array[commodity_id].stock++;
     hold_items[item_selected].id=-1;
   
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
    Show_it();

    LS_DisposeHandle(dialog_H);
    wait_for_mouse();

}



void buy_cargo(int item_selected)
{
extern int credits;
extern int hull_strength;
//oldmac//extern GDHandle				mZexGDH;
LSRAW *dialog_picture;
Handle dialog_H;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
LS_CGrafPtr the_drawing_buffer;
unsigned char number_string[12];
unsigned char temp_string[300];	//how long is a piece of string????
Point mouse_xy;
int buyers_price,needed_fuel;
int tc_tries,tc_max_tries;
int commodity_id,hold_slot,i;

//translate ship item to stock item
//commodity_id=hold_items[item_selected].id;
commodity_id=item_selected;

//stock_item_id=translate_ship_to_stock(ship_item_id);

if (commodity_id==-1)             report_error_c("buy_cargo translate error. Rats!","",commodity_id);


//find a hold slot
hold_slot=-1;
//can we find an empty slot in the hold?
for (i=0;i<MAX_HOLD_ITEMS;i++)
    {
    if (hold_items[i].id==-1) 
     {
      hold_slot=i;
      break;
     }
    }

if (hold_slot==-1) 
 {
	do_string_dialog((char*) "You have no space for the item.");
 return;	//all slots full
 }


if (commodity_array[commodity_id].stock==0)
 {
	do_string_dialog((char*) "That item is not in stock.");
 return;	//all slots full
 }

if (credits<commodity_array[commodity_id].sell_price)
 {
	do_string_dialog((char*) "You can't afford the item.");
 return;	//all slots full
 }

 
//calc tc_max_tries
//tc_tries=0;
//tc_max_tries=ship_items[KTRADING_COMPUTER].modifier*5;
//     
//rehaggle:	//err, a label!
//     the_drawing_buffer=Get_back_buffer();
//
//     GetPort (&savePort);
//	   saveGDH = GetGDevice();	  //All calls to QD are surrounded by save and set gdevices
//// 	  SetPort((GrafPtr)the_drawing_buffer);
//	   SetGDevice(mZexGDH);
//
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     dialog_H=GetZexPicture ('RCZ ',2011);	    //haggle dialog box
//     else
//     dialog_H=GetZexPicture ('RCZ ',2012);	  //confirm dialog box
//        
//	  LS_HLock(dialog_H);
//	  dialog_picture = (LSRAW*)*dialog_H;
//
////accept price or negatiate?
//
////    if (LS_Left_Button()==1);	//wait for mouse up
//    
////    display_goods(screen_class);
//
//	  print_crunched_pict(dialog_picture,180,130);
//    TextFont(21);
//TextSize(12);
//TextFace(normal);
//RGBForeColor(&rgbBlue);
//     MoveTo(426,150);
//     ZexNumToString(commodity_array[commodity_id].sell_price,number_string);
//     DrawString("Cr ");
//     DrawString(number_string);
//     CToPascal(commodity_array[commodity_id].string,temp_string);
//     MoveTo(200,150);
//     DrawString((unsigned char *) temp_string);
////if we have a trading computer calculate price
////     if (ship_items[KTRADING_COMPUTER].status==1)
////     {
////     float buying_price,percent;
////     buying_price=commodity_array[commodity_id].buy_price;
////
////     percent=buying_price/RangedRdm(30,60);	//between 40 and 60% off
////     buying_price=buying_price-percent;
////     
////     buyers_price=do_trading_computer(buying_price, stock_item_id);
////
////     MoveTo (200,220);
////	     DrawString ("Trading Computer gets a price of ");
////	     ZexNumToString(buyers_price,number_string);
////     DrawString(number_string);
////     }
////     else	  //no trading computer
//     {
//      float temp_price,fdiscount;
      buyers_price=commodity_array[commodity_id].sell_price;
////      temp_price=buyers_price;
////      fdiscount=.40;
////      temp_price=temp_price-(temp_price*fdiscount);
////      if (temp_price<1) temp_price=1;	//dont want credits!     
////      buyers_price=temp_price;
//     }
//     
//
//     MoveTo (200,240);
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     DrawString("Accept the price, re-negotiate or cancel?");
//     else
//     {
//     DrawString("Accept the price of ");
//	   ZexNumToString(buyers_price,number_string);
//     DrawString(number_string);
//     DrawString(" credits or cancel?");
//     
//     }
// 	SetGDevice(saveGDH);	//these are here so I can SEE them!
//	  SetPort(savePort);
//   Show_it();
//   LS_DisposeHandle(dialog_H);
//     wait_for_mouse();
//
////bottom 20, right 30 is exit
//     ZexGetMouse (&mouse_xy);
////     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);
// 
////have a trading computer?
//     if (ship_items[KTRADING_COMPUTER].status==1)
//     {
//     if (mouse_xy.h>380 && mouse_xy.h<480 && mouse_xy.v>250 && mouse_xy.v<280) return;	//cancel
//     if (mouse_xy.h>280 && mouse_xy.h<380 && mouse_xy.v>250 && mouse_xy.v<280) 
//        {
//          //check if exceeeded number of tries
//          play_zsound_always(sound_test, sound_high_pri, c1_chan1, sound_vol_7);
//          goto rehaggle;	  //recalc
//        }
//     }
//     
//     else
//     if (mouse_xy.h>320 && mouse_xy.h<480 && mouse_xy.v>256 && mouse_xy.v<278) return;	//cancel
//     if (mouse_xy.h>180 && mouse_xy.h<320 && mouse_xy.v>256 && mouse_xy.v<278)
//     goto buy_it;
//     else
//     {
//        play_zsound_always(low_beep, sound_high_pri, c1_chan1, sound_vol_7);
//        goto rehaggle;	//do nothing
//     }
//must be OK!

   
buy_it:   dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

//bottom 20, right 30 is exit
	ZexGetMouse (&mouse_xy);
	user_delay(1);	//Some time for the system so we don't hog the processor
//     DSpContext_GlobalToLocal (mDisplayContext,&mouse_xy);

//show you have bought
       //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180,130);

      TextFont(21);
TextSize(14);
TextFace(bold);
RGBForeColor(&rgbBlue);
//do text
     MoveTo (200,170);
     
      {
     DrawString_c("You have bought ");
 	   DrawString_c("the");
       MoveTo (200,188);
     CToPascal(commodity_array[commodity_id].string,temp_string);
     DrawString ((unsigned char const *) temp_string);	//cast this, cast that...
     DrawString_c (" for ");
     ZexNumToString(buyers_price, number_string);
     MoveTo (200,206);
     DrawString(number_string);
     DrawString_c (" credits.");
      }
    
     credits=credits-buyers_price;
     commodity_array[commodity_id].stock--;
     hold_items[hold_slot].id=commodity_id;
   
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
    Show_it();

    LS_DisposeHandle(dialog_H);
    wait_for_mouse();

}


void do_string_dialog(char * the_string)
{
LSRAW *dialog_picture;
Handle dialog_H;
//oldmac//GDHandle	saveGDH;
//oldmac//GrafPtr		savePort;
LS_CGrafPtr the_drawing_buffer;

        dialog_H=GetZexPicture ('RCZ ',2010);	     //blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;

       //oldmac//GetPort (&savePort);
	 //oldmac//saveGDH = GetGDevice();	//All calls to QD are surrounded by save and set gdevices
 
     the_drawing_buffer=Get_back_buffer();
//	SetPort((GrafPtr)the_drawing_buffer);
	 //oldmac//SetGDevice(mZexGDH);

	print_crunched_pict(dialog_picture,180,130);

      TextFont(21);
      TextSize(14);
      TextFace(bold);
      RGBForeColor(&rgbBlue);
//do text
     MoveTo (200,170);
     
      {
     DrawString_c((const char *)the_string);
      }
    
   
	//oldmac//SetGDevice(saveGDH);	//these are here so I can SEE them!
	//oldmac//SetPort(savePort);
    Show_it();
    wait_for_mouse();
    LS_DisposeHandle(dialog_H);

}

void show_no_credits(void)
{
  LSRAW *dialog_picture;
  Handle dialog_H;
  LS_CGrafPtr the_drawing_buffer;
  
	the_drawing_buffer=Get_back_buffer();

	dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;


	print_crunched_pict(dialog_picture,180,130);

	TextFont(21);
	TextSize(14);
	TextFace(bold);
	RGBForeColor(&rgbBlue);
//do text
	MoveTo (200,170);
	DrawString_c("You do not have enough credit");
	MoveTo (200,190);
	DrawString_c("to enter Galanet trading.");
	LS_DisposeHandle(dialog_H);
	Show_it();
	while (	LS_Left_Button()==1) user_delay(30);

	wait_for_mouse();
	return;

}

void show_logout(void)
{
  LSRAW *dialog_picture;
  Handle dialog_H;
  LS_CGrafPtr the_drawing_buffer;
  
	the_drawing_buffer=Get_back_buffer();

	dialog_H=GetZexPicture ('RCZ ',2010);	//blank dialog box   
	LS_HLock(dialog_H);
	dialog_picture = (LSRAW*)*dialog_H;


	print_crunched_pict(dialog_picture,180,130);

	TextFont(21);
	TextSize(14);
	TextFace(bold);
	RGBForeColor(&rgbBlue);
//do text
	MoveTo (200,170);
	DrawString_c("Your credits have dropped to zero.");
	MoveTo (200,190);
	DrawString_c("You will be logged out");
	LS_DisposeHandle(dialog_H);
	Show_it();
	while (	LS_Left_Button()==1) user_delay(30);

	wait_for_mouse();
	return;

}

// *************************************MODULE HEADER*********************************
// * FILENAME           : Untitled 1
// * MODULE TITLE       : Untitled 1
// * PROJECT            : Zex
// * DATE STARTED       : 00:22:42 on 15th September 1999
// * FIRST AUTHOR       : Stuart Ball
// *
// * COPYRIGHT (c)        Lightsoft
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial         Descrip.
// *
// *
// *
// ***********************************************************************************


// ***********************************************************************************
// * INCLUDED FILES
// *
#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#error "This file is not in the Zex2005 build"

#include "read_zex_ini.h"


// ***********************************************************************************
// * CONSTANTS
// *



// ***********************************************************************************
// * TYPE DEFINITIONS
// *



// ***********************************************************************************
// * VARIABLE DECLARATIONS
// *



// ***********************************************************************************
// * INTERNAL FUNCTION PROTOTYPES
// *




// +----------------------------------------------------------------ROUTINE HEADER----
// | TITLE              :
// | AUTHOR(s)          :
// | DATE STARTED       :
// +----------------------------------------------------------------------------------
// | DESCRIPTION:
// |
// |
// |
// |
// +----------------------------------------------------------------------------------
// | PARAM IN   : none
// |
// | PARAM OUT  : none
// |
// | ASSUMPTIONS:
// | LIMITATIONS:
// +----------------------------------------------------------------------------------
// | HISTORY:
// | Date Initial                Descrip.
// |
// |
// +----------------------------------------------------------------------------------
// | ADDITIONAL NOTES:
// |
int use_fuel,locked_rate,seed1,seed2,seed3,show_background_gfx_on_map;
int charge_warphole_toll,show_zex_warnings;

void read_zex_ini()
{
//first set up default values
use_fuel=1;
locked_rate=38;
seed1=0x23621c69;
seed2=0x12345678;
seed3=0x87654321;
show_background_gfx_on_map=1;
charge_warphole_toll=1;
show_zex_warnings=1;

}



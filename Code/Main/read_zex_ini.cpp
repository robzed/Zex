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



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

// *****************************************************************MODULE HEADER*****
// * FILENAME		: debug_timing.ext
// * MODULE TITLE	: debug timing external (public) definitions.
//
// * PROJECT		: Zex
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *           (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************

#if PCI_TIMING_DEBUG

void start_timer(LS_UnsignedWide *user_time_var);
void stop_timer(LS_UnsignedWide *user_time_var);

void T(int routine_code);

#endif


#ifndef PCI_TIMING_DEBUG
#error "options.def" not included where "debug_timing.h" is included!!
#endif


#define T_MISC_END 0
#define T_MATRIX 1
#define T_CULLING 2
#define T_PERS 3

#define T_3D_END 4
#define T_POLYDRAW_END 5
#define T_POSTDLP_END 6
#define T_SWAP_END 7



#define T_MIN 0
#define T_MAX 7



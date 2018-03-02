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

//game_defs.h
#ifndef __zex_defs__
#define __zex_defs__
#endif

#define MAX_NUMBER_OF_OBJECTS 1000
#define MAX_FRAME_RATE 35

#define MAX_VELOCITY 360	//zex's maximum velocity
#if OGL==1
#define VERS_STRING "0.99.7 - Open GLª"
#else
#define VERS_STRING "0.99.7" 
#endif
#define DATE_STRING " 2 March 17"

#define ENS 0
#define LT 1
#define LTCR 2
#define CDR 3
#define CAPT 4
#define RADM	5
#define VADM 6
#define ADM 7
#define FADM 8

#define COMMODITIES_MULTIPLIER 12	//commodities base price multiplier
#define LASER_RANGE_MULTIPLIER 1.4	//All laser ranges multiplied by this (float)


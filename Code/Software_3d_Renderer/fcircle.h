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
// * FILENAME		: fcircle.h
// * MODULE TITLE	: Circle drawing external definitions file
// * PROJECT		: Zex
// * DATE STARTED	: 11th April 1999, Sun 10:00pm
// * ORIGINAL AUTHOR: Rob Probin
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial		  Descrip.
// *
// *
// *
// ***********************************************************************************


void init_circle(void);
void fcircle(int x, int y, int r, unsigned char colour, int mode);

#define MODE_SOLID 0
#define MODE_TRANSPARENT 1

void create_outer_flare(int x, int y, int size, int brightness);


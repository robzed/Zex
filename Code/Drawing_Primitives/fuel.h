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

// Name:   	Fuel.h
// Date Started:	20/12/98
// Author(s):		ID
//
// Copyright(c) Lightsoft Software   		


void InitFuelGuage();
void update_fuel(int fuel_level);
void SplatLeftFuel(int amount, int x_pos, int y_pos);
void SplatRightFuel(int amount, int x_pos, int y_pos);
void SplatCentreFuel(int amount, int x_pos, int y_pos);


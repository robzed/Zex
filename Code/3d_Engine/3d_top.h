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

//3d_top.h

#ifndef __3d_structures__
#include "3d_structures_def.h"
#endif
void do_3d ();
void do_pers2(int width, int height);
void calc_normal(int the_object, int poly_select);
void do_culling2();
void do_zsort();
void calc_normal(int the_object, int poly_select);
void insert_to_z_buffer(_ZPoly *the_poly);
void reset_zsort_insert();
void do_gourad_polys();
_2D get_screen_center_coord(int object_slot);
_2D get_screen_front_coord(int object_slot);
_2D get_screen_rear_coord(int object_slot);
_2D point_to_screen(float x, float y, float z, int width, int h);
_2D get_any_screen_coord(int object_slot);


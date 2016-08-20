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

//camera_control.h

#define DEFAULT_CAMERA_ZOOM 13000
#define MIN_CAMERA_ZOOM 3500
#define MAX_CAMERA_ZOOM 19500

void run_camera_control(void);
void get_camera_normal(_3D *normal_here);
void reset_camera();
void camera_to_zex();
void point_camera_at(int the_slot);
void point_camera_at_coords(float x, float y);


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

//aim.h
int aim_from_zex(int capture_angle,int within_distance);	//returns the object that has been targetted
int aim_from_zex_exc_obj(int object_to_exclude, int laser_cat,int within_distance);

int aim_from_zex_rear(int laser_cat,int within_distance);
int aim_from_object(int aim_from, int aim_at_category, int within_distance);
int aim_from_zex_360(int within_distance);
int aim_from_object_no_limit(int aim_from, int aim_at_category, int within_distance);
int aim_from_object_no_limit_wep(int aim_from, int aim_at_category, int within_distance);






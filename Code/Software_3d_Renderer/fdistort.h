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

//fdistort.h

typedef struct {
		int x;
		int y;
		FLOAT_TYPE u;
		FLOAT_TYPE v;
		} dcoord;



//void distort3(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width);
void distort3(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent);
void distort_window_clipper3(dcoord *input_list, dcoord *temp_list, dcoord *output_list,int *polys_out);

void distort3_mask(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent);

void distortm(char *saddr, int swidth, dcoord *point_list, char *texture_addr, int texture_width, int light_value, int transparent, int num_connections);
void distort_window_clipper_m(dcoord *input_list, dcoord *temp_list, dcoord *output_list,int *points_out);



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

// pictures.h
typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;
} LSRAW;


typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;	//rgba quads actually
} LSRAW_RGBA;

void print_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_pict_C(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 top, UInt32 bott);
void print_crunched_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched32_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_translucent_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
Handle GetZexPicture (unsigned long res_type, short resid);
void decrunch_pict(LSRAW *the_picture, LSRAW *where);
void print_pict_scaled(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 sf, UInt32 trans_on);
void print_pict_magnified(LSRAW *the_picture, UInt32 x, UInt32 y, int y_correction, int x_correction, float magnification);

void print_pict_quick(LSRAW *the_picture, int x, int y);

void erase_zex_rect(int w, int h);	//erase to black (0xff)

void erase_zex_rect_colour(int w, int h, unsigned char colour_8);	//erase to a given colour

void set_erase_colour(unsigned char colour_8);

void invert_y_pict(LSRAW *the_picture);



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

// olg_text.h
int init_ogl_text();
void OGL_draw_monaco_bold(const char* the_string);
void OGL_draw_helvetica(char* the_string);
void OGL_draw_helvetica_small(char* the_string);

void OGL_draw_helveticaP(unsigned char* the_string);	//passcal string version


void OGL_DrawStr(char *str,int the_len,int x, int y);
void ZDrawString(const unsigned char * the_string);
void ZDrawString_c(const char * the_string);
void ZMoveTo(int x, int y);
void ZLineTo(int x, int y);
void ZRGBForeColor (const RGBColor * RGB_Colour);


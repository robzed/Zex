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

//splat_control_sw.h
#include <DrawSprocket.h>
int init_screen();
DSpContextReference		mDisplayContext;
DSpContextAttributes	mDisplayAttributes;
GDHandle				mZexGDH;
CTabHandle	 	 	    mZexCLUT;
LS_CGrafPtr	backBuff;


int init_screen();
void create_blackout_window();
void create_game_window();
void switch_to_256();
void create_virtual_screen();
void start_modifying_picture();
void stop_modifying_picture();

//old dsp functions
void screen_quit();
void GraphicsSelectContext();
void GraphicsInitAttributes(DSpContextAttributes *inAttributes);
void GraphicsActive(void);
void Show_it(int line_count);


LS_CGrafPtr Get_back_buffer(void);
void show_and_get_next_screen();


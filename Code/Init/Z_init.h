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

//Z_init.h
//public defs for init

//protos
int init_mac(void);
int init_game(void);
int	reinit_game(int the_level);
void clear_out_dyn();
void reset_static_slots();
void reset_3D();
int	reinit_game_for_next_level(int the_level);
void load_misc_graphics();
void load_32bit_graphics();



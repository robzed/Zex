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

// game_pause.h
void do_game_pause(void);
void do_game_dead();
void do_game_alert(const char* the_string);
int do_game_restore();

void setup_screen(LSRAW* the_picture, LSRAW* the_button);
int handle_pause_mouse(LSRAW* button_down, LSRAW* button_up);

void check_balls();
void get_level();



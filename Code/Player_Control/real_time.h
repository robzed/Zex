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

// real_time.h
#define MAX_AWAKE 38*60*60*13

typedef struct {
int days,hours,minutes,seconds;
} real_time_struct;


void get_real_time_struct(real_time_struct* time_struct);
void update_real_time();
void draw_real_time(int x, int y, int colour);
void init_game_time();
void add_a_minute_to_real_time();
void add_an_hour_to_real_time();
void check_sleep();
int get_hours();





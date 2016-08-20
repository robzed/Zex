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

// wingman.h
//wingmen commands
#define SEND_STATUS 1
#define HOLD_POSITION 2
#define FOLLOW_ME 3
#define STAND_DOWN 4

void handle_wingman1(int it);
void handle_wingman_for_game(int it);
void wingman_returning_text(int it);
void wingman_attacking_text(int it);
void wingman_ready_text(int it);
void wingman_mayday_text(int it);
void wingman_repaired_text(int it);
void check_if_wingman_hit_zex();
void wingman_hit_text(int it);
void wingman_shot_text(int it);
void wingman_bounced_text(int it);
void wingman_diealien_text(int it);
void wingman_diealienbyboss_text(int it);
void wingman_listen_to_them_text(int it);

void wingman_heyboss_text(int it);
void wingman_heyboss1_text(int it);

int find_a_wingman();
int find_nth_wingman(int n);



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

int do_avoid(int it, int with_roll, int can_reverse, int fwd_vel, int rev_vel, int max_angle);
int do_avoid_scenary(int it, int with_roll, int can_reverse, int fwd_vel, int rev_vel, int max_angle);

void turn_object(int it, int angular_velocity);
int turn_away_from_object(int object, int it, int angular_velocity);
void turn_away_from_zex(int it, int angular_velocity);
void ram_object_with_roll(int ramee,int it, int angular_velocity);
int ram_object(int ramee,int it, int angular_velocity);
void ram_zex(int it, int angular_velocity);

int get_distance_to_zex(int it);
int get_distance_to_from_point(int zex_x, int zex_y, int obj_x, int obj_y);
int get_distance_to_from_all_types(int obj_to, int obj_from);
int get_distance_to_from(int obj_to, int obj_from);
int find_something_to_attack(int it, int can_do_escape_caps, int max_distance);
int turn_left_or_right(int object_to_avoid, int it, int velocity);
void gen_callsign(int it);
void do_callsign_text(const_Ptr text_in,int it, int colour);	//colour=0=green, 1=blue, 2 = red
void calc_roll(int it);
void reduce_extern_velocity(int it, int max_thrust_delta);
void process_extern_movements(int it);

void GenRandomMonster();
void GenRandomNastyMonster();
int GenRandomMonsterWithVelocity(int worldx, int worldy, int vel, int heading, int spawner, int the_class);

int GenRandomHumanNPCWithVelocity(int worldx, int worldy, int vel, int heading, int spawner, int the_class);

int count_objects(unsigned long the_id);

void GenPhoenix();
void GenM2();
void GenM1();
void GenM1WithRoll();


unsigned long make_callsign(int type);




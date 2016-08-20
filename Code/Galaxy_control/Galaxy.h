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

#define GALAXY_WIDTH NUMBER_OF_SECTIONS_IN_A_QUAD_X * 2 * SECTION_SIZE
#define GALAXY_H NUMBER_OF_SECTIONS_IN_A_QUAD_Y * 2 * SECTION_SIZE

//galactic scenary etc.
#define ASTEROID_FIELD1 0X00060001	//6 ASTEROIDS
#define ASTEROID_FIELD2 0x000C0001	//12 asteroids
#define COWS1 0x00020002	//3 space cows
#define COWS2 0x00060002	//10 space cows
#define CORRAL_FACING_RIGHT 0x00000003	//right facing corral
 
typedef struct {
int number_of_earth_mships;
int total_earth_supply_rating;
int av_earth_supply_rating;
int total_earth_power_rating;
int av_earth_power_rating;

int number_of_alien_mships;
int total_alien_supply_rating;
int av_alien_supply_rating;
int total_alien_power_rating;
int av_alien_power_rating;
} galactic_stats;


void make_new_galaxy();
int get_supply_rating(int mothership_index);
void handle_galactic_map();
int get_supply_rating_docked(int ocb_slot);

void get_galactic_stats(galactic_stats* the_stats);
int get_current_warphole_toll();

void set_all_motherships_to_alegiance(int allegiance, int quadrant);

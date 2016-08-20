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

// Planet_Generator.h
void gen_planet_name(unsigned int seed, unsigned char * output_string);

void planet_gen(int seed, int is_inhabited, unsigned char *scratch_address, unsigned char *output_address);
//void galaxy_gen(int seed, char *output_address);
void galaxy_gen(int seed, unsigned char *output_address, int squish_flag);
void init_gen_planet_name(void);



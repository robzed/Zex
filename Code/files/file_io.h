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

//prototypes
Handle read_file (FSSpec the_file);
void get_zex_fsspec();
void read_prefs();
void write_prefs();

typedef struct {
int score;
char name[63];
} hiscore;

typedef struct {
int Change_res;
int Use_VBL;
int Use_SpeedLimit;
int Use_Mode320;
int Use_Interlace;
int sound_volume;
int music_volume;
int registered;
int version;

int camera_zoom;
int camera_mode;

int ticks_at_installation_time;
int random_number_at_installation_time;	//from ticks - show to user for registration
int key_from_random_number_at_installation_time;	//user sends this to get a registration ky
UInt32 registration_key;

hiscore the_scores[10];
} prefs;



void append_save_game_data(FILE* file_ptr);
int load_game();
int load_core_game_data(FILE* file_ptr);
OSErr WriteZexFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen );
OSErr ReadZexFile( FSSpec fileSpec, Ptr buffer, unsigned long bufferLen );

// two version of does_file_exist. Static polymophism determines which to call.
bool does_file_exist(const char *filename, const char *mode);
bool does_file_exist(FSSpec* the_fsspec);

#include <string>
#include <fstream>
// lightsoft file version of getline (supports any line endings)
void lsf_getline(std::string& output_string, FILE *file);
void lsf_getline(std::string& output_string, std::ifstream& file);

int return_save_version();

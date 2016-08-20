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

// sound.h
//ZEX sound public defs
//channels
#define ls1_chan	0	//mono1
#define ls2_chan	1	//mono2
#define ls3_chan	2	//mono3
#define ls4_chan 3	//mono4
#define ls5_chan	4	//stereo 1
#define ls6_chan	5	//stereo 2
#define speech_chan 6
#define c1_chan1	7
#define stereo1_chan	8

#define c1_chan2 9
#define c1_chan3	10
//#define laser_middle	  11
//
//#define laser2_right 12
//#define laser2_left	  13
//#define laser2_middle	14

#define continuous1	11
#define continuous2	12


//priorities
#define sound_low_pri	0	//if a channel is busy, this sound is ignored
#define sound_high_pri	1	//if a channel is busy, chan is flushed and this sound is played

//channel volumes - linear to non-linear translation (audio gain is a logarithmic function)
#define sound_vol_default	0	//don't set sound level
#define sound_vol_0	1
#define sound_vol_1	10
#define sound_vol_2	50
#define sound_vol_3	90
#define sound_vol_4	120
#define sound_vol_5	150
#define sound_vol_6	210
#define sound_vol_7	255	//max

//misc defs
#define max_sounds 80	//maximum number of sounds we can load

//sound definitions
enum the_sounds 
	{
	short_explosion,	//128 - ok
	short_explosion_with_echo, //129 - ok
	shield_scratch, //130 - ok
	space_synth,	//131 - ok
	zip,			//132
	retro_sound,	//133
	mono_roar,		//134
	LS,				//135
	laser1,			//136
	warning,		//137
	danger,			//138
	missile_explode,	//139
	missile_launch,	//140
	refuel_beep,	//141
	low_beep,		//142
	sound_test,		//143
	thud,			//144
	laser2,			//145
	laser3,			//146
	click,			//147
	large_shield_sound,	//148
	setup_sound,	//149
	bleeper,		//150
	growl,			//151
	explosion4,		//152
	screech,		//153
	tractor_beam_loop,	//continuous sound - 154
	rumble_loop,	//continuous sound - 155
	mac_startup,	//156
	pickup,			//157
	jettison,		//158
	select_mship,	//159
	select_planet,	//160
	select_unknown,	//161
	message_from_command,	//162
	launch,			//163
	out_of_cruise,	//164
	thrust_sound,	//165
	thrust_reverse_sound,	//166
	};
	
//sound functions protos
int	init_zsound();
void play_zsound(int sound_number, int priority, int channel, int volume);
void play_zsound_relative_to_zex(int the_object, int sound_number);
void play_zsound_always(int sound_number, int priority, int channel, int volume);
//void play_laser_relative_to_object(int the_object, int sound_number, int priority, int volume);
//void play_laser_relative_to_object2(int the_object, int sound_number, int priority, int volume);
void run_periodic_sound_tasks();

void load_mod_player();
void music_set_vol(int volume);
void play_tune(unsigned long res_id);
int is_music_busy(void);
void fade_out_music();
void do_fade_out_music();
void fade_and_play(unsigned long res_id);
void fade_and_play_zex_resource (unsigned long res_id);

void run_level_music(FSSpec * the_file);
void play_level_music();
void stop_music_now();

void run_level_aux_music(int index);
void play_level_aux_music(int index);

//simple sound routines
void simple_stop_sound();
void simple_play_sound(int id);


//continuous sound
void play_simple_continuous_sound(int soundID, int midi_note, float normalised_volume /*0-1*/);
void stop_simple_continuous_sound();



int insert_continuous_sound(int object, int soundID, int midi_note);
void clear_cont_sound(int object);
void clear_all_cont_sound_chans();
void set_cont_sound_chan_volume(int object, int percent);

void set_cont_sound2_note(int midi_note);

void run_ejection_music(void);
bool are_we_running_ejection_music(void);


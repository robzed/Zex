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

// input_control.h
void init_input_device();
void configure_input_device();
void process_input_device();
OSStatus InitNeeds();
void SetKeyboardMouseActivation(Boolean active);
void poll_input_device();
void kill_inputs();
void enable_inputs();
void disable_inputs();
void take_mouse_keyboard_as_input();
void untake_mouse_keyboard_as_input();
int game_fire_button();
void ZexGetMouse (Point *the_point);
//oldmac//short isPressed(unsigned short k );	//k is keycode
void init_ip_globs();

void handle_key_change(SDLKey key, bool pressed);
bool IsKeyPressed(SDLKey key);
bool is_one_key_pressed(SDLKey* key);


// should be kept in sync with in def_key[] input_control.cpp
enum { 
	turn_left_key, 
	turn_right_key, 
	thrust_up_key,
	thrust_down_key,
	cannon_key,
	laser_key,
	missile_key,
	retro_key,
	warp_drive_key,
	shockwave_key,
	abort_key,
	music_up_key,
	music_down_key,
	sound_up_key,
	sound_down_key,
	radar_range_key,
	camera_zoom_in_key,
	camera_zoom_out_key,
	missle_toggle_key,
	show_inventory_key,
	eject_key,
	dock_request_key,
	camera_mode_key,
	control_panel_key,
	velocity_limter_key,
	command_and_control_key,
	galaxy_map_key,
	res_switch_key,
	NUMBER_OF_KEYS
};

SDLKey get_programmed_keycode(int key_number);	// key number is as above
void load_single_key_def(int key_number, SDLKey key_code);
void set_internal_default_keys();

void key_res_switch_allowed(bool allowed);

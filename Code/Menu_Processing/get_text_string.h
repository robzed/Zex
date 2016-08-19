/*
 *  get_text_string.h
 *  Zex
 *
 *  Created by Rob Probin on 18/04/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

// moved here from menu_driver.h (which was the wrong place for it...)


#ifndef GET_TEXT_STRING_H
#define GET_TEXT_STRING_H


// fetch semi-localised text for menu
const char *get_text(int text_ref);

// these strings need putting into a resource for localisation
enum {
	empty_text,
	highlight_char,
	Mhighlight_char,
	
	file_dialog_save_as_title,
	file_dialog_load_as_title,
	
	file_dialog_slot_number_text,
	file_dialog_no_game_text,
	file_dialog_player_1_text,
	file_dialog_player_2_text,
	file_dialog_room_text,
	file_dialog_single_player_text,
	file_dialog_no_description,
	file_dialog_time_and_date,
	file_dialog_powerpc_game,
	file_dialog_x86_game,
	file_dialog_zex_saved_game,
	
	file_dialog_previous,
	file_dialog_previous_click,
	file_dialog_next,
	file_dialog_next_click,
	file_dialog_cancel,
	file_dialog_cancel_click,
	
	save_as_dialog_title,
	save_as_dialog_cancel,
	save_as_dialog_cancel_click,
	save_as_dialog_accept,
	save_as_dialog_accept_click,
	save_as_dialog_clear,
	save_as_dialog_clear_click,
	save_as_dialog_use_previous,
	save_as_dialog_use_previous_click,
	
	registration_title,
	registration_name_label,
	registration_code_label,
	
};


#endif // GET_TEXT_STRING_H


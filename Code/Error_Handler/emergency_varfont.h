/*
 *  emergency_varfont.h
 *  Zex
 *
 *  Created by Rob Probin on 16/11/2005.
 *  Copyright 2005 Lightsoft. All rights reserved.
 *
 */


int evf_get_string_width(const char *text);
int evf_get_character_height();
void evf_print_string(int x, int y, const char* text, Uint32 back_colour, Uint32 fore_colour);




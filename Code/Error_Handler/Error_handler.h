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

//Error_handler.h

#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include "mac_emulation.h"

void report_error(const char *error_string, const Str255 error_string2, int error_number);
void report_error_c(const char *error_string, const char * error_string2, int error_number);

void report_error_id(const char *error_String,unsigned long id);
void report_error_id_number(const char *error_string, unsigned long id, int number);
void report_error_id_non_fatal(const char *error_string, unsigned long id);

#endif



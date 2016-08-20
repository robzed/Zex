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

/*
 *  save_path_destination.h
 *  Zex
 *
 *  Created by Rob Probin on 27/03/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef SAVE_PATH_DESTINATION_H
#define SAVE_PATH_DESTINATION_H

#include "enhanced_string.h"

class Save_Path
{
public:
	const std::string get();
	Save_Path(const std::string save_path_filename);
	
private:
	std::string loaded_save_path;
};

#endif // SAVE_PATH_DESTINATION_H

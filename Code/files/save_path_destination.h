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

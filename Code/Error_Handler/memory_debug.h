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
 *  memory_debug.h
 *  Zex
 *
 *  Created by Rob Probin on 12/06/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef MEMORY_DEBUG_H
#define MEMORY_DEBUG_H

//static bool memory_check_enabled = true;
#define memory_check_enabled false


class MemoryDebugServices;

class MemoryDebug {
	
public:
	virtual ~MemoryDebug() { };	
	virtual void check_memory() = 0;
	//MemoryDebug();
	
private:
};

//
// This class checks a block of memory allocated in place.
//
class Fixed_MemoryDebug : public MemoryDebug {
public:
	Fixed_MemoryDebug();
#if memory_check_enabled
	static const int SIZE_TO_FILL = 2000;
#else
	static const int SIZE_TO_FILL = 1;
#endif
	void check_memory();
	~Fixed_MemoryDebug();

private:
	long unsigned int memory_to_fill[SIZE_TO_FILL];
	MemoryDebugServices	*md;
};


//
// This class checks a block of memory allocated by the user.
//
class InPlace_MemoryDebug : public MemoryDebug {
public:
	InPlace_MemoryDebug(unsigned long int* pointer_to_memory, size_t size_in_longs);
	void check_memory();
	~InPlace_MemoryDebug();
private:
	int checksum;
	unsigned long int* memory;
	size_t longwords;
	MemoryDebugServices	*md;
};


//
// This class checks a block of memory allocated by 'New'
//
class New_MemoryDebug : public MemoryDebug {
public:
	New_MemoryDebug(size_t min_size_in_bytes);
	void check_memory();
	~New_MemoryDebug();
private:
	InPlace_MemoryDebug* Ipmd;	// leverage off existing code, similar to in-place but with new create storage
};

//
// Service call
//
void check_all_memories();


#endif // MEMORY_DEBUG_H

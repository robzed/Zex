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
 *  memory_debug.cpp
 *  Zex
 *
 *  Created by Rob Probin on 12/06/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#include "memory_debug.h"
#include "txtsplat.h"
#include "mac_emulation.h"
#include "timing.h"

#include <list>
using std::list;

//
// Constants and Configuration
//
enum scan_rate_type { SCAN_SLOWLY, SCAN_FAST, SCAN_OFF };

// configuration
static scan_rate_type scan_rate = SCAN_FAST;


#if memory_check_enabled==true
#warning "DISABLE MEMORY CHECK BEFORE RELEASE!!!"
#endif

//
// Private functions
//

//
//
//

//struct memory_debug_slot {
//	MemoryDebug* pointer;
//	bool active;
//};

//
// Private data
//
static list<MemoryDebug*> pointer_store;


//
// Code
//
void register_memory_debug(MemoryDebug* next)
{
	pointer_store.push_back(next);
}

void unregister_memory_debug(MemoryDebug* next)
{
	pointer_store.remove(next);
}

void check_all_memories()
{
	if(memory_check_enabled)
	{
		Uint32 start = LS_GetTicks();
		int counter = 0;
		list<MemoryDebug*>::iterator theIterator;
		for( theIterator = pointer_store.begin(); theIterator != pointer_store.end(); theIterator++ ) 
		{
			(*theIterator)->check_memory();
			counter++;
		}
		
		Uint32 time_length = LS_GetTicks() - start;
		unsigned char number_string[12];
		ZexNumToString(time_length,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ("MEMCHK",120,350,10);
		SplatText_Transparent ((char *)number_string,180,350,10);

		ZexNumToString(counter,number_string);
		number_string[number_string[0]+1]=0;	//quick p to c hack
		number_string[0]=' ';
		SplatText_Transparent ((char *)number_string,200,350,10);
	}
	
}


class MemoryDebugServices {
public:
	void fill(unsigned long int* pointer_to_memory, size_t size_in_longs);
	void stop_program();
	void check(unsigned long int* pointer_to_memory, size_t size_in_longs);
};

const unsigned long int memory_constant = 0xDECAFBAD;			// magic number
//#if sizeof(unsigned long int) != 4
//#error "value won't be much good"
//#endif

void MemoryDebugServices::fill(unsigned long int* pointer_to_memory, size_t size_in_longs)
{
	for(size_t i=0; i<size_in_longs; i++)
	{
		*pointer_to_memory = memory_constant;		// store a value we can recognise later on
		pointer_to_memory++;
	}
}

void MemoryDebugServices::stop_program()
{
	if(memory_check_enabled)
	{

		// do something here
		while(1)
		{
			// nothing
		}

	}
}


void MemoryDebugServices::check(unsigned long int* pointer_to_memory, size_t size_in_longs)
{
	for(size_t i=0; i<size_in_longs; i++)
	{
		if(*pointer_to_memory != memory_constant)
		{
			stop_program();
		}
		pointer_to_memory++;
	}
}

//
// --------------
//
New_MemoryDebug::New_MemoryDebug(size_t min_size_in_bytes)
{
	if(memory_check_enabled)
	{
		unsigned long int* memory;
		size_t longwords;

		min_size_in_bytes += 3;		// round up
		longwords = min_size_in_bytes/4;
		memory = new unsigned long[longwords];
		
		Ipmd = new InPlace_MemoryDebug(memory, longwords);
	}
}

//
// -----------
//
void New_MemoryDebug::check_memory()
{
	Ipmd->check_memory();
}

New_MemoryDebug::~New_MemoryDebug()
{
	delete Ipmd;
}


//
// InPlace_MemoryDebug
//
InPlace_MemoryDebug::InPlace_MemoryDebug(unsigned long int* pointer_to_memory, size_t size_in_longs)
{
	if(memory_check_enabled)
	{
		longwords = size_in_longs;
		memory = pointer_to_memory;
		checksum = (int)(memory) + (int)(longwords);
		
		md->fill(memory, longwords);
		
		register_memory_debug(this);	
	}

}

void InPlace_MemoryDebug::check_memory()
{
	if(checksum != (int)(memory) + (int)(longwords))
	{
		md->stop_program();
	}
	
	md->check(memory, longwords);
}

InPlace_MemoryDebug::~InPlace_MemoryDebug()
{
	unregister_memory_debug(this);	
}

//
// Fixed_MemoryDebug
//
Fixed_MemoryDebug::Fixed_MemoryDebug()
{
	md->fill(memory_to_fill, SIZE_TO_FILL);

	register_memory_debug(this);	
}

void Fixed_MemoryDebug::check_memory()
{
	md->check(memory_to_fill, SIZE_TO_FILL);
}

Fixed_MemoryDebug::~Fixed_MemoryDebug()
{
	unregister_memory_debug(this);	
}


//MemoryDebug::MemoryDebug()
//: md(new MemoryDebugServices)
//{
//	register_memory_debug(this);	
//}


//MemoryDebug::~MemoryDebug()
//{
//	unregister_memory_debug(this);	
//	
//	delete MemoryDebugServices;
//}



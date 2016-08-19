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

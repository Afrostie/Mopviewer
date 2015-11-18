#pragma once
/**
* This class is used to hold the entire memory content of a particle when it is extracted
*/

#include "MemoryEntry.h"

struct MemoryContent {
	/**
	* the current size of memory
	*/
	int memorySize;
	/**
	* the memory block itself
	*/
	MemoryEntry *content;
};
#pragma once

#include <stdlib.h>

#include "Error/ReturnCodes.h"

typedef struct Core_VectorWriter
{
	void* Core_VectorWriter_UserObject;
	void (*PushBack)(void* p_dataStructure, void* p_value);
	Core_ReturnCodes (*Swap)(void* p_dataStructure, size_t p_left, size_t p_right);
	Core_ReturnCodes (*InsertArrayAtRealloc)(void* p_dataStructure, void* p_insertedDataStructure, size_t p_index);
} Core_VectorWriter;

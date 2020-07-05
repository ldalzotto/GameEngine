#pragma once

#include <stdlib.h>

#include "Error/ReturnCodes.h"

typedef struct Core_VectorWriter
{
	void* Core_VectorWriter_UserObject;
	Core_ReturnCodes (*PushBack)(void* p_dataStructure, void* p_value);
	Core_ReturnCodes (*Swap)(void* p_dataStructure, size_t p_left, size_t p_right);
	Core_ReturnCodes (*InsertArrayAt)(void* p_dataStructure, void* p_insertedDataStructure, size_t p_index);
} Core_VectorWriter;

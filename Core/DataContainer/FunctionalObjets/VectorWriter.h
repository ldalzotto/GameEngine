#pragma once

#include <stdlib.h>

typedef struct Core_VectorWriter
{
	void* Core_VectorWriter_UserObject;
	void (*PushBack)(void* p_dataStructure, void* p_value);
	void (*Swap)(void* p_dataStructure, size_t p_left, size_t p_right);
} Core_VectorWriter;

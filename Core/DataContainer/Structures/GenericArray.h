
#pragma once

typedef struct Core_GenericArray
{
	void* Memory;
	size_t Size;
	size_t Capacity;
	size_t ElementSize;
} Core_GenericArray;

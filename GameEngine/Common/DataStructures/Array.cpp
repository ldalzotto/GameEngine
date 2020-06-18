#include "Array.h"

#include <stdlib.h>
#include <cstdint>
#include <string>

#include <stdexcept>

namespace _GameEngine::_Core
{
	size_t array_getElementOffset(Array* p_array, size_t p_index)
	{
		return p_array->ElementSize * p_index;
	}

	void  Array_alloc(Array* p_array, size_t p_capacity, size_t p_elementSize)
	{
		p_array->Capacity = p_capacity;
		p_array->Size = 0;
		p_array->ElementSize = p_elementSize;
		p_array->Memory = malloc(Array_getTotalSize(p_array));
	};

	void  Array_free(Array* p_array)
	{
		free(p_array->Memory);
	};

	size_t Array_getTotalSize(Array* p_array)
	{
		return p_array->ElementSize * p_array->Capacity;
	};

	void  Array_clear(Array* p_array)
	{
		p_array->Size = 0;
	};

	void Array_zeroing(Array* p_array)
	{
		memset(p_array, 0, Array_getTotalSize(p_array));
	};

	void* Array_at(Array* p_array, size_t p_index)
	{
		if (p_index < p_array->Capacity)
		{
			return ((char*)p_array->Memory) + array_getElementOffset(p_array, p_index);
		}
		return nullptr;
	};

	void Array_pushBack(Array* p_array, void* p_value)
	{
		if (p_array->Size >= p_array->Capacity)
		{
			throw std::runtime_error("Array_pushBack : out of range.");
		}

		void* p_targetMemory = (char*)p_array->Memory + array_getElementOffset(p_array, p_array->Size);
		memcpy(p_targetMemory, p_value, p_array->ElementSize);
		p_array->Size += 1;
	};
}
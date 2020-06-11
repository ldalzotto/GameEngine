#include "Vector.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <limits.h>

namespace _GameEngine::_Core
{
	size_t Vector_getTotalSize(Vector* p_vector)
	{
		return p_vector->ElementSize * p_vector->Capacity;
	}

	size_t Vector_getElementOffset(Vector* p_vector, size_t p_index)
	{
		return p_vector->ElementSize * p_index;
	}

	void* Vector_getEnd(Vector* p_vector)
	{
		return (char*)p_vector->Memory + (p_vector->ElementSize * p_vector->Capacity);
	}

	void Vector_alloc(Vector* p_vector, size_t p_initialCapacity, size_t p_elementSize)
	{
		p_vector->ElementSize = p_elementSize;
		p_vector->Capacity = p_initialCapacity;
		p_vector->Memory = malloc(Vector_getTotalSize(p_vector));
		p_vector->Size = 0;
	}

	void Vector_free(Vector* p_vector)
	{
		free(p_vector->Memory);
		p_vector->Memory = nullptr;
		p_vector->Capacity = 0;
		p_vector->ElementSize = 0;
		p_vector->Size = 0;
	}

	void Vector_resize(Vector* p_vector, size_t p_newCapacity)
	{
		if (p_newCapacity > p_vector->Capacity)
		{
			void* l_newMemory = realloc(p_vector->Memory, p_newCapacity * p_vector->ElementSize);
			if (l_newMemory)
			{
				p_vector->Memory = l_newMemory;
				p_vector->Capacity = p_newCapacity;
			}
		}
	}

	void Vector_pushBack(Vector* p_vector, void* p_value)
	{
		if (p_vector->Size >= p_vector->Capacity)
		{
			Vector_resize(p_vector, p_vector->Capacity * 2);
			Vector_pushBack(p_vector, p_value);
		}
		else
		{
			void* p_targetMemory = (char*)p_vector->Memory + Vector_getElementOffset(p_vector, p_vector->Size);
			memcpy(p_targetMemory, p_value, p_vector->ElementSize);
			p_vector->Size += 1;
		}
	}

	void Vector_erase(Vector* p_vector, size_t p_index)
	{
		if (p_index >= p_vector->Size)
		{
			throw std::runtime_error("Vector : Erase out of range.");
		}

		// If we are not erasing the last element, then we move memory. Else, we have nothing to do.
		if (p_index + 1 != p_vector->Size)
		{
			void* p_targetMemory = (char*)p_vector->Memory + Vector_getElementOffset(p_vector, p_index);
			memmove(p_targetMemory, (char*)p_targetMemory + p_vector->ElementSize, (p_vector->Size - p_index - 1) * p_vector->ElementSize);
		}

		p_vector->Size -= 1;
	}

	void* Vector_at(Vector* p_vector, size_t p_index)
	{
		if (p_index > p_vector->Size)
		{
			throw std::runtime_error("Vector : At out of range.");
		}

		return  (char*)p_vector->Memory + Vector_getElementOffset(p_vector, p_index);
	}

	void* Vector_get(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject)
	{
		void* l_cursor = p_vector->Memory;
		for (size_t i = 0; i < p_vector->Size; i++)
		{
			if (p_comparator(l_cursor, p_userObject))
			{
				return l_cursor;
			}

			l_cursor = (char*)l_cursor + p_vector->ElementSize;
		}
		return nullptr;
	};

	size_t Vector_getIndex(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject)
	{
		for (size_t i = 0; i < p_vector->Size; i++)
		{
			void* l_element = Vector_at(p_vector, i);
			if (p_comparator(l_element, p_userObject))
			{
				return i;
				break;
			}
		}
		return std::numeric_limits<size_t>::max();
	};

}

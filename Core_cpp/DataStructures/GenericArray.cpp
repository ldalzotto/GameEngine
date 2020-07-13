#include "GenericArray.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include "Functional/Iterator/Iterator.hpp"
#include "Functional/Sort/ElementSorter.hpp"
#include "Algorithm/Sort/SortAlgorithm.hpp"

namespace _Core
{
	inline size_t GenericArray_getTotalSize(GenericArray* p_genericArray)
	{
		return p_genericArray->ElementSize * p_genericArray->Capacity;
	};

	inline size_t GenericArray_getElementOffset(GenericArray* p_genericArray, size_t p_index)
	{
		return p_genericArray->ElementSize * p_index;
	};

	void GenericArray_alloc(GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity)
	{
		p_genericArray->ElementSize = p_elementSize;
		p_genericArray->Capacity = p_initialCapacity;
		p_genericArray->Memory = malloc(GenericArray_getTotalSize(p_genericArray));
		p_genericArray->Size = 0;
	};
	
	void GenericArray_free(GenericArray* p_genericArray)
	{
		free(p_genericArray->Memory);
		p_genericArray->Memory = NULL;
		p_genericArray->Capacity = 0;
		p_genericArray->ElementSize = 0;
		p_genericArray->Size = 0;
	};

	void GenericArray_resize(GenericArray* p_genericArray, size_t p_newCapacity)
	{
		if (p_newCapacity > p_genericArray->Capacity)
		{
			void* l_newMemory = realloc(p_genericArray->Memory, p_newCapacity * p_genericArray->ElementSize);
			if (l_newMemory)
			{
				p_genericArray->Memory = l_newMemory;
				p_genericArray->Capacity = p_newCapacity;
			}
		}
	}

	void* GenericArray_at_unchecked(GenericArray* p_genericArray, size_t p_index)
	{
		return (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_index);
	};
	
	void* GenericArray_at(GenericArray* p_genericArray, size_t p_index)
	{
		if (p_index >= p_genericArray->Size)
		{
			return NULL;
		}
		return GenericArray_at_unchecked(p_genericArray, p_index);
	};

	void GenericArray_buildIterator(GenericArray* p_genericArray, VectorIterator* out_iterator)
	{
		out_iterator->Array = p_genericArray;
		out_iterator->Current = NULL;
		out_iterator->CurrentIndex = -1;
	};

	void GenericArray_pushBack_realloc(GenericArray* p_genericArray, void* p_value)
	{
		if (p_genericArray->Size >= p_genericArray->Capacity)
		{
			GenericArray_resize(p_genericArray, p_genericArray->Capacity == 0 ? 1 : (p_genericArray->Capacity * 2));
			GenericArray_pushBack_realloc(p_genericArray, p_value);
		}
		else
		{
			void* p_targetMemory = (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_genericArray->Size);
			memcpy(p_targetMemory, p_value, p_genericArray->ElementSize);
			p_genericArray->Size += 1;
		}

	}

	void GenericArray_pushBack_noRealloc(GenericArray* p_genericArray, void* p_value)
	{
		if (p_genericArray->Size >= p_genericArray->Capacity)
		{
			throw std::runtime_error("Core_GenericArray_pushBack_noRealloc : impossible to push back. array is already full.");
		}

		GenericArray_pushBack_realloc(p_genericArray, p_value);
	};

	void GenericArray_pushBack_realloc_sorted(GenericArray* p_genericArray, void* p_value, ElementSorter* p_elementSorter)
	{
		VectorIterator l_it;
		GenericArray_buildIterator(p_genericArray, &l_it);
		Sort_min(&l_it, 0, p_elementSorter);
		GenericArray_isertAt_realloc(p_genericArray, p_value, 1, l_it.CurrentIndex);
	};

	void GenericArray_swap(GenericArray* p_genericArray, size_t p_left, size_t p_right)
	{
		if (p_left >= p_genericArray->Size || p_right >= p_genericArray->Size) { throw std::runtime_error("Core_GenericArray_swap : out_of_range"); }
		if (p_left > p_right) { throw std::runtime_error("Core_GenericArray_swap : invalid indices."); }
		if (p_left == p_right) { return; }

		char* l_leftMemoryTarget = (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_left);
		char* l_rightMemoryTarget = (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_right);

		for (size_t i = 0; i < p_genericArray->ElementSize; i++)
		{
			char l_rightTmp = l_rightMemoryTarget[i];
			l_rightMemoryTarget[i] = l_leftMemoryTarget[i];
			l_leftMemoryTarget[i] = l_rightTmp;
		}
	};

	void GenericArray_erase(GenericArray* p_genericArray, size_t p_index)
	{
		if (p_index >= p_genericArray->Size)
		{
			throw std::runtime_error("Core_GenericArray_erase : Erase out of range.");
		}

		// If we are not erasing the last element, then we move memory. Else, we have nothing to do.
		if (p_index + 1 != p_genericArray->Size)
		{
			void* p_targetMemory = (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_index);
			memmove(p_targetMemory, (char*)p_targetMemory + p_genericArray->ElementSize, (p_genericArray->Size - p_index - 1) * p_genericArray->ElementSize);
		}

		p_genericArray->Size -= 1;
	}

	void GenericArray_isertAt_realloc(GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index)
	{
		if (p_index > p_genericArray->Size)
		{
			throw std::runtime_error("Core_GenericArray_isertAt_realloc : out of range");
		}

		if (p_genericArray->Size + p_elementNb > p_genericArray->Capacity)
		{
			GenericArray_resize(p_genericArray, p_genericArray->Capacity == 0 ? 1 : (p_genericArray->Capacity * 2));
			GenericArray_isertAt_realloc(p_genericArray, p_value, p_elementNb, p_index);
		}
		else
		{
			void* l_initialElement = (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_index);
			// If we insert between existing elements, we move down memory to give space for new elements
			if (p_genericArray->Size - p_index > 0)
			{
				void* l_targetElement = (char*)p_genericArray->Memory + GenericArray_getElementOffset(p_genericArray, p_index + p_elementNb);
				memmove(l_targetElement, l_initialElement, p_genericArray->ElementSize * (p_genericArray->Size - p_index));
			}
			memcpy(l_initialElement, p_value, p_genericArray->ElementSize * p_elementNb);
			p_genericArray->Size += p_elementNb;
		}
	};

	void GenericArray_isertAt_noRealloc(GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index)
	{
		if ((p_genericArray->Size + p_elementNb) >= p_genericArray->Capacity)
		{
			throw std::runtime_error("Core_GenericArray_isertAt_neRealloc : inserted array is too large !");
		}

		return GenericArray_isertAt_realloc(p_genericArray, p_value, p_elementNb, p_index);
	};

	GenericArray GenericArray_deepCopy(GenericArray* p_genericArray)
	{
		GenericArray l_copiedArray;
		memcpy(&l_copiedArray, p_genericArray, sizeof(GenericArray));
		l_copiedArray.Memory = malloc(GenericArray_getTotalSize(p_genericArray));
		memcpy(l_copiedArray.Memory, p_genericArray->Memory, GenericArray_getTotalSize(p_genericArray));
		return l_copiedArray;
	};
}
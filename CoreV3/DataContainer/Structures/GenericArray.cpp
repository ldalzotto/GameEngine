#pragma message(__FILE__)

namespace _CoreV3
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

	inline void* GenericArray_at_unchecked(GenericArray* p_genericArray, size_t p_index)
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

	void GenericArray_clear(GenericArray* p_genericArray)
	{
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

		// return CR_OK;
	};

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
		if ((p_genericArray->Size + p_elementNb) > p_genericArray->Capacity)
		{
			throw std::runtime_error("Core_GenericArray_isertAt_neRealloc : inserted array is too large !");
		}

		GenericArray_isertAt_realloc(p_genericArray, p_value, p_elementNb, p_index);
	};

	void GenericArray_isertArrayAt_realloc(GenericArray* p_genericArray, GenericArray* p_insertedArray, size_t p_index)
	{
		if (p_genericArray->ElementSize != p_insertedArray->ElementSize)
		{
			throw std::runtime_error("Core_GenericArray_isertAt_realloc : element size doesn't match !");
		}
		GenericArray_isertAt_realloc(p_genericArray, p_insertedArray->Memory, p_insertedArray->Size, p_index);
	};

	void GenericArray_isertArrayAt_noRealloc(GenericArray* p_genericArray, GenericArray* p_insertedArray, size_t p_index)
	{
		if ((p_genericArray->Size + p_insertedArray->Size) >= p_genericArray->Capacity)
		{
			throw std::runtime_error("Core_GenericArray_isertArrayAt_noRealloc : inserted array is too large !");
		}

		GenericArray_isertArrayAt_realloc(p_genericArray, p_insertedArray, p_index);
	};
}
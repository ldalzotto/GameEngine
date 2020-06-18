#pragma once

#include "Array.h"

namespace _GameEngine::_Core
{
	template<class T>
	struct ArrayT
	{
		Array Array;

		void   alloc(size_t p_capacity, size_t p_elementSize) { Array_alloc(&Array, p_capacity, p_elementSize); };
		void   free() { Array_free(&Array); };
		size_t getTotalSize() { return Array_getTotalSize(&Array); };
		size_t size() { return Array.Size; }
		void   clear() { Array_clear(&Array); };
		void   zeroing() { Array_zeroing(&Array); };
		void push_back(T* p_value) { Array_pushBack(&Array, p_value); };
		T* at(size_t p_index) { return (T*)Array_at(&Array); };
	};
}
#pragma once

#include "Array.h"

namespace _GameEngine::_Core
{
	template<class T>
	struct ArrayT
	{
		Array Array;

		void   alloc(size_t p_capacity) { Array_alloc(&Array, p_capacity, sizeof(T)); };
		void   build(T* p_memory, size_t p_capacity) { Array_build(&Array, p_memory, p_capacity, sizeof(T)); };
		void   free() { Array_free(&Array); };
		size_t getTotalSize() { return Array_getTotalSize(&Array); };
		size_t size() { return Array.Size; }
		void   clear() { Array_clear(&Array); };
		void   zeroing() { Array_zeroing(&Array); };
		void push_back(T* p_value) { Array_pushBack(&Array, p_value); };
		T* at(size_t p_index) { return (T*)Array_at(&Array, p_index); };
	};

	template<class T>
	void ArrayT_interpret(ArrayT<T>* p_array, T* p_memory, size_t p_size) {
		Array_interpret(&p_array->Array, p_memory, p_size, sizeof(T));
	};
}
#pragma once

namespace _GameEngine::_Core
{
	struct Array
	{
		void* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	void  Array_alloc		(Array* p_array, size_t p_capacity, size_t p_elementSize);
	void Array_interpret(Array* p_array, void* p_memory, size_t p_size, size_t p_elementSize);
	void Array_build	    (Array* p_array, void* Memory, size_t p_capacity, size_t p_elementSize);
	void  Array_free		(Array* p_array);
	size_t Array_getTotalSize(Array* p_array);
	void  Array_clear		(Array* p_array);
	void Array_zeroing(Array* p_array);
	void* Array_at(Array* p_array, size_t p_index);
	void Array_pushBack(Array* p_array, void* p_value);
}
#pragma once

#include "DataStructures/Vector.h"

namespace _GameEngine::_Core
{
	template <class T>
	struct VectorT
	{
		Vector Vector;
		void alloc(size_t p_initialCapacity = 0) { Vector_alloc(&Vector, p_initialCapacity, sizeof(T)); };
		void free() { Vector_free(&Vector); };
		size_t size() { return Vector.Size; };
		void push_back(T* p_value) { Vector_pushBack(&Vector, p_value); };
		void erase(size_t p_index) { Vector_erase(&Vector, p_index); };
		void erase(bool(*p_comparator)(T*, T*), T* p_userObject) { Vector_erase(&Vector, (VectorElementComparator)p_comparator, p_userObject); };
		T* at(size_t p_index) { return (T*)Vector_at(&Vector, p_index); };
		T* get(bool(*p_comparator)(T*, T*), T* p_userObject) { return (T*)Vector_get(&Vector, (VectorElementComparator)p_comparator, p_userObject); };
		size_t getIndex(bool(*p_comparator)(T*, T*), T* p_userObject) { return Vector_getIndex(&Vector, (VectorElementComparator)p_comparator, p_userObject); };
	};

}
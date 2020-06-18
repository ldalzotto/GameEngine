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
		void deepCopy(VectorT<T>* p_target) { Vector_deepCopy(&Vector, (_Core::Vector*)p_target); };
		size_t size() { return Vector.Size; };
		size_t capacity() { return Vector.Capacity; };
		void push_back(T* p_value) { Vector_pushBack(&Vector, p_value); };
		void erase(size_t p_index) { Vector_erase(&Vector, p_index); };
		void clear() { Vector_clear(&Vector); };

		template <class COMPARED_OBJECT>
		void erase(bool(*p_comparator)(T*, COMPARED_OBJECT*), COMPARED_OBJECT* p_userObject) { Vector_erase(&Vector, (VectorElementComparator)p_comparator, p_userObject); };
		
		T* at(size_t p_index) { return (T*)Vector_at(&Vector, p_index); };

		template <class COMPARED_OBJECT>
		T* get(bool(*p_comparator)(T*, COMPARED_OBJECT*), COMPARED_OBJECT* p_userObject) { return (T*)Vector_get(&Vector, (VectorElementComparator)p_comparator, p_userObject); };

		template <class COMPARED_OBJECT>
		size_t getIndex(bool(*p_comparator)(T*, COMPARED_OBJECT*), COMPARED_OBJECT* p_userObject) { return Vector_getIndex(&Vector, (VectorElementComparator)p_comparator, p_userObject); };

		template <class USER_OBJECT>
		void forEachReverse(void(*p_callback)(T*, USER_OBJECT*), USER_OBJECT* p_userObject) { Vector_forEachReverse(&Vector, (VectorElementCallback)p_callback, p_userObject); };
	};

}
#pragma once

namespace _GameEngine::_Core
{
	struct Vector
	{
		void* Memory;
		size_t Size;
		size_t ElementSize;
		size_t Capacity;
	};

	using VectorElementComparator = bool(*)(void*, void*);

	// < 0 means left < right ...
	using SortElementComparator = short int(*)(void*, void*);
	using SortElementComparatorWithUserObject = short int(*)(void*, void*, void*);

	void Vector_alloc(Vector* p_vector, size_t p_initialCapacity, size_t p_elementSize);
	void Vector_free(Vector* p_vector);
	void Vector_resize(Vector* p_vector, size_t p_newCapacity);
	void Vector_deepCopy(Vector* p_source, Vector* p_target);
	void Vector_pushBack(Vector* p_vector, void* p_value);
	void Vector_insertAt(Vector* p_vector, void* p_value, size_t p_index);
	void Vector_insertAt(Vector* p_vector, void* p_value, size_t p_elementNb, size_t p_index);
	void Vector_erase(Vector* p_vector, size_t p_index);	
	void Vector_erase(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject = nullptr);
	void Vector_clear(Vector* p_vector);
	void Vector_reserve(Vector* p_vector, size_t p_elementNumber);
	void* Vector_at(Vector* p_vector, size_t p_index);
	void* Vector_get(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject = nullptr);
	size_t Vector_getIndex(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject = nullptr);
	void* Vector_min(Vector* p_vector, SortElementComparatorWithUserObject p_sortComparator, void* p_userObject = nullptr);
	bool Vector_contains(Vector* p_vector, VectorElementComparator p_comparator, void* p_userObject = nullptr);

	using VectorElementCallback = void(*)(void*, void*);
	void Vector_forEach(Vector* p_vector, VectorElementCallback p_callback, void* p_userObject = nullptr);
	void Vector_forEachReverse(Vector* p_vector, VectorElementCallback p_callback, void* p_userObject = nullptr);

	struct SortedVector
	{
		Vector Vector;
		SortElementComparator SortComparator;
	};

	void SortedVector_alloc(SortedVector* p_vector, size_t p_initialCapacity, size_t p_elementSize, SortElementComparator p_sortComparator);
	void SortedVector_free(SortedVector* p_vector);
	void SortedVector_pushBack(SortedVector* p_vector, void* p_value);

}


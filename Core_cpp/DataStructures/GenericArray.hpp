#pragma once

namespace _Core
{
	struct VectorIterator;
	struct ElementSorter;
	struct Comparator;
	struct Asserter;
}

namespace _Core
{
	struct GenericArray
	{
		void* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	void GenericArray_alloc(GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity);
	void GenericArray_free(GenericArray* p_genericArray);

	void GenericArray_resize(GenericArray* p_genericArray, size_t p_newCapacity);

	void* GenericArray_at_unchecked(GenericArray* p_genericArray, size_t p_index);
	void* GenericArray_at(GenericArray* p_genericArray, size_t p_index);

	void GenericArray_buildIterator(GenericArray* p_genericArray, VectorIterator* out_iterator);

	void GenericArray_clear(GenericArray* p_genericArray);

	void GenericArray_pushBack_realloc(GenericArray* p_genericArray, void* p_value);
	void GenericArray_pushBack_noRealloc(GenericArray* p_genericArray, void* p_value);
	void GenericArray_pushBack_realloc_sorted(GenericArray* p_genericArray, void* p_value, ElementSorter* p_elementSorter);
	void GenericArray_pushBack_realloc_guarded(GenericArray* p_genericArray, void* p_value, Asserter* p_insertGuard);
	void GenericArray_pushBack_realloc_guarded_sorted(GenericArray* p_genericArray, void* p_value, Asserter* p_insertGuard, ElementSorter* p_elementSorter);

	void GenericArray_swap(GenericArray* p_genericArray, size_t p_left, size_t p_right);

	void GenericArray_erase(GenericArray* p_genericArray, size_t p_index);
	void GenericArray_eraseCompare(GenericArray* p_genericArray, Comparator* p_comparator);

	void GenericArray_isertAt_realloc(GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index);
	void GenericArray_isertAt_noRealloc(GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index);
	void GenericArray_isertArrayAt_realloc(GenericArray* p_genericArray, GenericArray* p_insertedArray, size_t p_index);
	void GenericArray_isertArrayAt_noRealloc(GenericArray* p_genericArray, GenericArray* p_insertedArray, size_t p_index);

	GenericArray GenericArray_deepCopy(GenericArray* p_genericArray);

}

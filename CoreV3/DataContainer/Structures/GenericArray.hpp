#pragma message(__FILE__)

namespace _CoreV3
{
	struct GenericArray
	{
		void* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	template <typename T>
	inline GenericArray Convert(T* p_from, size_t p_elementNumber)
	{
		return GenericArray{
			p_from, p_elementNumber, p_elementNumber, sizeof(T)
		};
	};



	extern void GenericArray_alloc(GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity);
	extern void GenericArray_free(GenericArray* p_genericArray);

	extern void* GenericArray_at_unchecked(GenericArray* p_genericArray, size_t p_index);
	extern void* GenericArray_at(GenericArray* p_genericArray, size_t p_index);

	extern void GenericArray_clear(GenericArray* p_genericArray);

	extern void GenericArray_pushBack_realloc(GenericArray* p_genericArray, void* p_value);
	extern void GenericArray_pushBack_noRealloc(GenericArray* p_genericArray, void* p_value);
	extern void GenericArray_swap(GenericArray* p_genericArray, size_t p_left, size_t p_right);

	extern void GenericArray_isertAt_realloc(GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index);
	extern void GenericArray_isertAt_noRealloc(GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index);
	extern void GenericArray_isertArrayAt_realloc(GenericArray* p_genericArray, GenericArray* p_insertedArray, size_t p_index);
	extern void GenericArray_isertArrayAt_noRealloc(GenericArray* p_genericArray, GenericArray* p_insertedArray, size_t p_index);
}
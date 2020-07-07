#pragma message(__FILE__)

namespace _CoreV3
{
	template <class T>
	struct ArrayT
	{
		T* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	template <class T>
	__forceinline void Alloc(ArrayT<T>* p_container, size_t p_initialCapacity)
	{
		GenericArray_alloc((GenericArray*)p_container, p_initialCapacity);
	};

	template <class T>
	__forceinline void Free(ArrayT<T>* p_container)
	{
		GenericArray_free((GenericArray*)p_container);
	};

	template <class T>
	__forceinline T* At(ArrayT<T>* p_container, size_t p_index)
	{
		GenericArray_at((GenericArray*)p_container, p_index);
	};

	template <class T>
	__forceinline T* Clear(ArrayT<T>* p_container)
	{
		GenericArray_clear((GenericArray*)p_container);
	};

	template <class T>
	__forceinline void PushBack(ArrayT<T>* p_container, T* p_value)
	{
		GenericArray_pushBack_noRealloc((GenericArray*)p_container, p_value);
	};

	template <class T>
	__forceinline void PushBack(ArrayT<T>* p_container, T&& p_value)
	{
		GenericArray_pushBack_noRealloc((GenericArray*)p_container, &p_value);
	};

	template <class T>
	__forceinline void Swap(ArrayT<T>* p_container, size_t p_left, size_t p_right)
	{
		GenericArray_swap((GenericArray*)p_container, p_left, p_right);
	};

	template <class T>
	__forceinline void InsertAt(ArrayT<T>* p_container, T* p_insertValue, size_t p_index)
	{
		GenericArray_isertAt_noRealloc((GenericArray*)p_container, p_insertValue, p_index);
	};

	template <class T>
	__forceinline void InsertArrayAt(ArrayT<T>* p_container, VectorT<T>* p_insertedArray, size_t p_index)
	{
		GenericArray_isertArrayAt_noRealloc((GenericArray*)p_container, (GenericArray*)p_insertedArray, p_index);
	};


}
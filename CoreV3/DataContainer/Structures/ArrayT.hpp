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
		GenericArray_alloc(Convert(p_container), sizeof(T), p_initialCapacity);
	};

	template <class T>
	__forceinline void Free(ArrayT<T>* p_container)
	{
		GenericArray_free(Convert(p_container));
	};

	template <class T>
	__forceinline T* At(ArrayT<T>* p_container, size_t p_index)
	{
		return (T*)GenericArray_at(Convert(p_container), p_index);
	};

	template <class T>
	__forceinline T* Clear(ArrayT<T>* p_container)
	{
		GenericArray_clear(Convert(p_container));
	};

	template <class T>
	__forceinline void PushBack(ArrayT<T>* p_container, T* p_value)
	{
		GenericArray_pushBack_noRealloc(Convert(p_container), p_value);
	};

	template <class T>
	__forceinline void PushBack(ArrayT<T>* p_container, T&& p_value)
	{
		GenericArray_pushBack_noRealloc(Convert(p_container), &p_value);
	};

	template <class T>
	__forceinline void PushBackArray(ArrayT<T>* p_container, GenericArray* p_insertedArray)
	{
		InsertArrayAt(p_container, p_insertedArray, p_container->Size);
	};

	template <class T>
	__forceinline void Swap(ArrayT<T>* p_container, size_t p_left, size_t p_right)
	{
		GenericArray_swap(Convert(p_container), p_left, p_right);
	};

	template <class T>
	__forceinline void InsertAt(ArrayT<T>* p_container, T* p_insertValue, size_t p_index)
	{
		GenericArray_isertAt_noRealloc(Convert(p_container), p_insertValue, 1, p_index);
	};

	template <class T>
	__forceinline void InsertAt(ArrayT<T>* p_container, T&& p_insertValue, size_t p_index)
	{
		InsertAt(p_container, &p_insertValue, p_index);
	};

	template <class T>
	__forceinline void InsertArrayAt(ArrayT<T>* p_container, GenericArray* p_insertedArray, size_t p_index)
	{
		GenericArray_isertArrayAt_noRealloc(Convert(p_container), p_insertedArray, p_index);
	};
}
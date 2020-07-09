#pragma message(__FILE__)

namespace _CoreV3
{
	template <class T>
	struct VectorT
	{
		T* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	template <class T>
	__forceinline void Alloc(VectorT<T>* p_container, size_t p_initialCapacity)
	{
		GenericArray_alloc(Convert(p_container), sizeof(T), p_initialCapacity);
	};

	template <class T>
	__forceinline VectorT<T> Alloc(size_t p_initialCapacity)
	{
		VectorT<T> l_vec;
		GenericArray_alloc(Convert(&l_vec), sizeof(T), p_initialCapacity);
		return std::move(l_vec);
	}

	template <class T>
	__forceinline void Free(VectorT<T>* p_container)
	{
		GenericArray_free(Convert(p_container));
	};

	template <class T>
	__forceinline T* At(VectorT<T>* p_container, size_t p_index)
	{
		return (T*)GenericArray_at(Convert(p_container), p_index);
	};

	template <class T>
	__forceinline T* At_unchecked(VectorT<T>* p_container, size_t p_index)
	{
		return (T*)GenericArray_at_unchecked(Convert(p_container), p_index);
	};

	template <class T>
	__forceinline void Clear(VectorT<T>* p_container)
	{
		GenericArray_clear(Convert(p_container));
	};

	template <class T>
	__forceinline void PushBack(VectorT<T>* p_container, T* p_value)
	{
		GenericArray_pushBack_realloc(Convert(p_container), p_value);
	};

	template <class T>
	__forceinline void PushBack(VectorT<T>* p_container, T&& p_value)
	{
		GenericArray_pushBack_realloc(Convert(p_container), &p_value);
	};

	template <class T>
	__forceinline void Swap(VectorT<T>* p_container, size_t p_left, size_t p_right)
	{
		GenericArray_swap(Convert(p_container), p_left, p_right);
	};

	template <class T>
	__forceinline void InsertAt(VectorT<T>* p_container, T* p_insertValue, size_t p_index)
	{
		GenericArray_isertAt_realloc(Convert(p_container), p_insertValue, 1, p_index);
	};

	template <class T>
	__forceinline void InsertAt(VectorT<T>* p_container, T&& p_insertValue, size_t p_index)
	{
		InsertAt(p_container, &p_insertValue, p_index);
	};

	template <class T>
	__forceinline void InsertArrayAt(VectorT<T>* p_container, GenericArray* p_insertedArray, size_t p_index)
	{
		GenericArray_isertArrayAt_realloc(Convert(p_container), p_insertedArray, p_index);
	};

	template <class T>
	__forceinline void PushBackArray(VectorT<T>* p_container, GenericArray* p_insertedArray)
	{
		InsertArrayAt(p_container, p_insertedArray, p_container->Size);
	};

	template <class T>
	__forceinline void PushBackArray(VectorT<T>* p_container, GenericArray&& p_insertedArray)
	{
		PushBackArray(p_container, &p_insertedArray);
	};

	template <class T, typename INSERTED_ARRAY_0>
	__forceinline void PushBackArrays(VectorT<T>* p_container, INSERTED_ARRAY_0* p_insertedArray0)
	{
		PushBackArray(p_container, p_insertedArray0);
	};

	template <class T, typename INSERTED_ARRAY_0, typename... INSERTED_ARRAYS>
	__forceinline void PushBackArrays(VectorT<T>* p_container, INSERTED_ARRAY_0* p_insertedArray0, INSERTED_ARRAYS... p_insertedArray)
	{
		PushBackArray(p_container, p_insertedArray0);
		PushBackArrays(p_container, p_insertedArray...);
	};

	template <class T, typename INSERTED_ARRAY_0, typename... INSERTED_ARRAYS>
	__forceinline VectorT<T> PushBackArrays(VectorT<T>&& p_container, INSERTED_ARRAY_0* p_insertedArray0, INSERTED_ARRAYS... p_insertedArray)
	{
		PushBackArray(&p_container, p_insertedArray0);
		PushBackArrays(&p_container, p_insertedArray...);
		return std::move(p_container);
	};

}
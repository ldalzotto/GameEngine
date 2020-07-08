#pragma message(__FILE__)

namespace _CoreV3
{
	template <>
	void Alloc<Char>(VectorT<Char>* p_container, size_t p_initialCapacity)
	{
		GenericArray_alloc(Convert(p_container), sizeof(char), p_initialCapacity + 1);
		Char l_nul = CHAR(NULL);
		PushBack(p_container, &l_nul);
	};

	template <>
	VectorT<Char> Alloc(size_t p_initialCapacity)
	{
		VectorT<Char> l_vector;
		GenericArray_alloc(Convert(&l_vector), sizeof(char), p_initialCapacity);
		Char l_nul = CHAR(NULL);
		PushBack(&l_vector, &l_nul);
		return std::move(l_vector);
	};

	template <>
	void Clear<Char>(VectorT<Char>* p_container)
	{
		GenericArray_clear((GenericArray*)p_container);
		Char l_nul = CHAR(NULL);
		PushBack(p_container, &l_nul);
	};


	template <>
	void PushBack<Char>(VectorT<Char>* p_container, Char* p_value)
	{
		if (p_container->Size >= 2)
		{
			InsertAt(p_container, p_value, p_container->Size - 1);
		}
		else
		{
			InsertAt(p_container, p_value, 0);
		}
	};

	template <>
	void PushBackArray<Char>(VectorT<Char>* p_container, GenericArray* p_insertedArray)
	{
		if (p_container->Size >= 2)
		{
			InsertArrayAt(p_container, p_insertedArray, p_container->Size - 1);
		}
		else
		{
			InsertArrayAt(p_container, p_insertedArray, 0);
		}
	};

}
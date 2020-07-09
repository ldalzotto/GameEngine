#pragma message(__FILE__)

#define CHAR(Raw) (_CoreV3::Char)Raw
#define STR(Raw) (_CoreV3::Char*)Raw

namespace _CoreV3
{
	typedef char Char;
	
	typedef VectorT<Char> String;

	template <>
	extern void Alloc<Char>(VectorT<Char>* p_container, size_t p_initialCapacity);

	template <>
	extern VectorT<Char> Alloc(size_t p_initialCapacity);

	template <>
	extern void Clear<Char>(VectorT<Char>* p_container);

	template <>
	extern void PushBack<Char>(VectorT<Char>* p_container, Char* p_value);

	template <>
	extern void PushBackArray<Char>(VectorT<Char>* p_container, GenericArray* p_insertedArray);

	template <>
	inline GenericArray Convert(Char* p_from)
	{
		return Convert(p_from, strlen(p_from));
	};

	template <>
	inline ArrayT<Char> Convert<Char*, ArrayT<Char>>(Char* p_from)
	{
		size_t l_size = strlen(p_from);
		return ArrayT<Char>{ p_from, l_size, l_size, sizeof(char)};
	}

	inline void PushBackArray(VectorT<Char>* p_container, Char* p_insertedArray)
	{
		PushBackArray(p_container, Convert<Char*, GenericArray>(p_insertedArray));
	};

	inline void PushBackArray(VectorT<Char>* p_container, VectorT<Char>* p_insertedArray)
	{
		PushBackArray(p_container, Convert<Char*, GenericArray>(STR(p_insertedArray->Memory)));
	};
}

namespace _CoreV3
{
	template <>
	inline size_t Hash<Char>(Char* p_value)
	{
		return std::hash<std::string>()(p_value);
	};

	template <template < class Char > class DATA_STRUCTURE>
	inline size_t Hash(DATA_STRUCTURE<Char>* p_value)
	{
		return Hash(p_value->Memory);
	}
}
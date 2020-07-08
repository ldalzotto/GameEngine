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
	__forceinline GenericArray Convert<Char*, GenericArray>(Char* p_from)
	{
		return Convert(p_from, strlen(p_from));
	};

	template <>
	__forceinline ArrayT<Char> Convert<Char*, ArrayT<Char>>(Char* p_from)
	{
		size_t l_size = strlen(p_from);
		return ArrayT<Char>{ p_from, l_size, l_size, sizeof(char)};
	}

	__forceinline void PushBackArray(VectorT<Char>* p_container, Char* p_insertedArray)
	{
		PushBackArray(p_container, Convert<Char*, GenericArray>(p_insertedArray));
	};

	__forceinline void PushBackArray(VectorT<Char>* p_container, VectorT<Char>* p_insertedArray)
	{
		PushBackArray(p_container, Convert<Char*, GenericArray>(STR(p_insertedArray->Memory)));
	};
}
#pragma message(__FILE__)

#define CHAR(Raw) (_CoreV3::Char)Raw
#define STR(Raw) (_CoreV3::Char*)Raw

namespace _CoreV3
{
	typedef char Char;
	
	/*
	template <>
	__forceinline char* Convert<Char, char*>(Char p_from)
	{
		return p_from.character;
	};

	template <>
	__forceinline Char Convert<char*, Char>(char* p_from)
	{
		return { p_from };
	};
	*/

	typedef VectorT<Char> String;

	template <>
	extern void Alloc<Char>(VectorT<Char>* p_container, size_t p_initialCapacity);

	template <>
	extern void Clear<Char>(VectorT<Char>* p_container);

	template <>
	extern void PushBack<Char>(VectorT<Char>* p_container, Char* p_value);

	template <>
	extern void PushBackArray<Char>(VectorT<Char>* p_container, GenericArray* p_insertedArray);

	template <>
	__forceinline GenericArray Convert<Char*, GenericArray>(Char* p_from)
	{
		size_t l_fromLength = strlen(p_from);
		return { p_from, l_fromLength, l_fromLength, sizeof(Char) };
	};
}
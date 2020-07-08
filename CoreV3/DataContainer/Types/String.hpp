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

	template <>
	__forceinline GenericArray Convert<VectorT<Char>*, GenericArray>(VectorT<Char>* p_from)
	{
		Char* l_str = STR(p_from->Memory);
		size_t l_fromLength = strlen((char*)l_str);
		return { l_str, l_fromLength, l_fromLength, sizeof(Char) };
	};

	template <>
	__forceinline void PushBackArray(VectorT<Char>* p_container, VectorT<Char>* p_insertedArray)
	{
		PushBackArray(p_container, Convert<VectorT<Char>*, GenericArray>(p_insertedArray));
	};

}
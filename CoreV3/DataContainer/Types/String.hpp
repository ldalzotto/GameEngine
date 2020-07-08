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

	__forceinline GenericArray Convert(Char* p_from)
	{
		return Convert(p_from, strlen(p_from));
	};

	__forceinline void PushBackArray(VectorT<Char>* p_container, Char* p_insertedArray)
	{
		PushBackArray(p_container, Convert(p_insertedArray));
	};

	__forceinline void PushBackArray(VectorT<Char>* p_container, VectorT<Char>* p_insertedArray)
	{
		PushBackArray(p_container, Convert(STR(p_insertedArray->Memory)));
	};
}
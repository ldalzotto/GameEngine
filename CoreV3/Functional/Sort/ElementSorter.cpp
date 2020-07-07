#pragma message(__FILE__)

namespace _CoreV3
{
	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT>
	__forceinline short int genericSortMethod(SOURCE_ELEMENT p_left, COMPARED_ELEMENT p_right)
	{
		if (p_left > p_right) { return 1; }
		else if (p_left < p_right) { return -1; }
		else { return 0; }
	};

	template <>
	short int SortMethod<float, float, void>(float* p_left, float* p_right, void* p_userObject)
	{
		return genericSortMethod<float, float>(*p_left, *p_right);
	};

	template <>
	short int SortMethod<int, int, void>(int* p_left, int* p_right, void* p_userObject)
	{
		return genericSortMethod<int, int>(*p_left, *p_right);
	};

}
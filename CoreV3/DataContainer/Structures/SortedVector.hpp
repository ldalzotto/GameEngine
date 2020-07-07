#pragma message(__FILE__)

namespace _CoreV3
{
	using SortFunction = short int (*)(void* p_left, void* p_right, void* p_userObject);

	struct SortedVector
	{
		GenericArray GenericArray;
		SortFunction Function;
	};

	void SortedVector_alloc(SortedVector* p_sortedVector, size_t p_elementSize, size_t p_initialCapcity, SortFunction p_function);

	void SortedVector_pushBack(SortedVector* p_sortedVector, void* p_value);
}
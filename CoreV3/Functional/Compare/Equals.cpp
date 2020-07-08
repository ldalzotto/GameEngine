#pragma message(__FILE__)

# define FLOAT_TOLERANCE 0.000001f

namespace _CoreV3
{
	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	bool genericNumericEqualsMethod(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject)
	{
		return abs((*p_left) - (*p_right)) <= FLOAT_TOLERANCE;
	};

	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	bool genericExactEqualsMethod(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject)
	{
		return *p_left == *p_right;
	};

	template <>
	bool EqualsMethod<float, float, void>(float* p_left, float* p_right, void* p_userObject)
	{
		return genericNumericEqualsMethod<float, float, void>(p_left, p_right, NULL);
	};

	template <>
	bool EqualsMethod<size_t, int, void>(size_t* p_left, int* p_right, void* p_userObject)
	{
		return genericExactEqualsMethod<size_t, int, void>(p_left, p_right, NULL);
	};

	template <>
	bool EqualsMethod<char, char, void>(char* p_left, char* p_right, void* p_userObject)
	{
		return genericExactEqualsMethod(p_right, p_right, (void*)NULL);
	};

	template <>
	bool EqualsMethod<void*, void, void>(void** p_left, void* p_right, void* p_userObject)
	{
		return genericExactEqualsMethod<void*, void*, void>(p_left, &p_right, p_userObject);
	};
}
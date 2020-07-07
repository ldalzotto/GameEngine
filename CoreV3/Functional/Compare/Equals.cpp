#pragma message(__FILE__)

namespace _CoreV3
{
	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	bool genericEqualsMethod(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject)
	{
		return p_left == p_right;
	};

	template <>
	bool EqualsMethod<float, float, void>(float* p_left, float* p_right, void* p_userObject)
	{
		return genericEqualsMethod<float, float, void>(p_left, p_right, NULL);
	};
}
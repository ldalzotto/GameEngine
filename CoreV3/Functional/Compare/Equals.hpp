#pragma message(__FILE__)

namespace _CoreV3
{
	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	bool EqualsMethod(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject);

	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	using CompareFunctionT = bool(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject);

	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	struct ElementComparatorT
	{
		bool(*Function)(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject);
		COMPARED_ELEMENT* Right;
		USER_OBJECT* UserObject;
	};
}
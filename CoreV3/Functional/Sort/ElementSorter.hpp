#pragma message(__FILE__)

namespace _CoreV3
{
	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	short int SortMethod(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject);

	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	using SortFunctionT = short int (*)(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject);

	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	struct ElementSorterT
	{
		SortFunctionT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT> Function;
		USER_OBJECT* UserObject;
	};

}
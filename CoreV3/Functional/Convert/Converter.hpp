#pragma message(__FILE__)

namespace _CoreV3
{
	template <class FROM, class TO>
	TO Convert(FROM p_from);

	template <class FROM, class TO, class USER_OBJECT>
	TO Convert(FROM p_from, USER_OBJECT p_userObject);
}
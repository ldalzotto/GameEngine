#pragma message(__FILE__)

namespace _CoreV3
{
	template<typename T>
	String ToString(T* p_element);

	template<typename T>
	String ToString(T&& p_element);
}
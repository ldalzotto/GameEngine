#pragma message(__FILE__)

namespace _CoreV3
{
	template <>
	String ToString<float>(float* p_element)
	{
		String l_str;
		Alloc(&l_str, 50);
		snprintf(l_str.Memory, sizeof(char) * 50, "%f", *p_element);
		return l_str;
	};

	template <>
	String ToString<float>(float&& p_element)
	{
		String l_str;
		Alloc(&l_str, 50);
		snprintf(l_str.Memory, sizeof(char) * 50, "%f", p_element);
		return l_str;
	};

	template <>
	String ToString<int>(int* p_element)
	{
		String l_str;
		Alloc(&l_str, 20);
		snprintf(l_str.Memory, sizeof(char) * 20, "%i", *p_element);
		return l_str;
	};
}
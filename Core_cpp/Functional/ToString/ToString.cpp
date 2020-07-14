#include "ToString.hpp"

#include <stdio.h>
#include "DataStructures/Specifications/String.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"

namespace _Core
{
	void ToString_float(String* p_string, float* p_element)
	{
		char l_str[50];
		snprintf(l_str, sizeof(char) * 50, "%f", *p_element);
		String_append(p_string, l_str);
	};
	
	void ToString_int(String* p_string, int* p_element)
	{
		char l_str[20];
		snprintf(l_str, sizeof(char) * 20, "%i", *p_element);
		String_append(p_string, l_str);
	};
	
	void ToString_intv(char p_string[50], int* p_element)
	{
		ArrayT<char> l_arr = ArrayT_fromCStyleArray(p_string, 50);
		ToString_int((String*)&l_arr, p_element);
	};
}
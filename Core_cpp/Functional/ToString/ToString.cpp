#include "ToString.hpp"

#include <stdio.h>
extern "C"
{
#include "DataStructures/String.h"
}
#include "DataStructures/Specifications/ArrayT.hpp"

namespace _Core
{
	void ToString_float(String_PTR p_string, float* p_element)
	{
		char l_str[50];
		snprintf(l_str, sizeof(char) * 50, "%f", *p_element);
		String_AppendRaw(p_string, l_str);
	};
	
	void ToString_int(String_PTR p_string, int* p_element)
	{
		char l_str[20];
		snprintf(l_str, sizeof(char) * 20, "%i", *p_element);
		String_AppendRaw(p_string, l_str);
	};
	
	void ToString_intv(char p_string[50], int* p_element)
	{
		ArrayT<char> l_arr = ArrayT_fromCStyleArray(p_string, 50);
		_Core::ArrayT_clear(&l_arr);
		char l_null = NULL;
		_Core::ArrayT_pushBack(&l_arr, &l_null);
		String l_arr_c = { l_arr .Memory, l_arr .Size, l_arr .Capacity};
		ToString_int(&l_arr_c, p_element);
	};
}
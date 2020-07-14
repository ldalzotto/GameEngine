#pragma once

namespace _Core
{
	struct String;
}
namespace _Core
{
	void ToString_float(String* p_string, float* p_element);
	void ToString_int(String* p_string, int* p_element);
	void ToString_intv(char p_string[50], int* p_element);
}
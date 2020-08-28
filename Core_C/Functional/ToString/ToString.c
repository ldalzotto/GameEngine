#include "ToString.h"

#include <stdio.h>
#include <string.h>

#include "DataStructures/String.h"

void ToString_float(String_PTR p_string, float* p_element)
{
	char l_str[50];
	snprintf(l_str, sizeof(char) * 50, "%f", *p_element);
	String_AppendRawNoRealloc(p_string, l_str);
};

void ToString_int(String_PTR p_string, int* p_element)
{
	char l_str[20];
	snprintf(l_str, sizeof(char) * 20, "%i", *p_element);
	String_AppendRawNoRealloc(p_string, l_str);
};

void ToString_intv(char p_string[20], int* p_element)
{
	String l_arr = (String){ p_string, strlen(p_string), strlen(p_string) };
	String_ClearNoRealloc(&l_arr);
	ToString_int(&l_arr, p_element);
};

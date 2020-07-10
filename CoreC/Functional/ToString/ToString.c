#include "ToString.h"

#include <stdio.h>
#include "Algorithm/String/StringAlgorithm.h"

void Core_toString_float(struct Core_GenericArray* p_string, float* p_element)
{
	char l_str[50];
	snprintf(l_str, sizeof(char) * 50, "%f", *p_element);
	Core_string_append(p_string, l_str);
};

void Core_toString_int(struct Core_GenericArray* p_string, int* p_element)
{
	char l_str[20];
	snprintf(l_str, sizeof(char) * 20, "%i", *p_element);
	Core_string_append(p_string, l_str);
};
#pragma once

#include <stdlib.h>

size_t Hash_string(char* p_str);
size_t HashCombine_string(size_t p_seed, char* p_str);

size_t Hash_uint(unsigned int* p_value);
size_t HashCombine_uint(size_t p_seed, unsigned int* p_value);
#pragma once

#include <stdlib.h>

#include "Constants.h"

// http://www.cse.yorku.ca/~oz/hash.html
size_t Core_Hash_string(char* p_str);
size_t Core_HashCombine_string(size_t p_seed, char* p_str);

size_t Core_Hash_uint(unsigned int* p_value);
size_t Core_HashCombine_uint(size_t p_seed, unsigned int* p_value);